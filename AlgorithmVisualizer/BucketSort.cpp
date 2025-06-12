#include "BucketSort.h"
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace Algorithms {

    BucketSort::BucketSort() : Algorithm("Bucket Sort") {
        reset();
    }

    void BucketSort::reset() {
        buckets.clear();
        bucketSizes.clear();
        currentBucket = 0;
        currentElement = 0;
        outputIndex = 0;
        distributionPhase = true;
        initialized = false;
    }

    bool BucketSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void BucketSort::initializeBuckets(const Visualization::VisualizationData& data) {
        const auto& array = data.getArray();
        size_t n = array.size();
        
        int minVal = array[0].value;
        int maxVal = array[0].value;
        for (size_t i = 1; i < n; i++) {
            if (array[i].value < minVal) minVal = array[i].value;
            if (array[i].value > maxVal) maxVal = array[i].value;
        }
        
        // Create an appropriate number of buckets based on array size
        size_t numBuckets = static_cast<size_t>(std::sqrt(n));
        
        // Ensure we have at least 2 buckets for visualization purposes
        if (numBuckets < 2) numBuckets = 2;
        if (numBuckets > n/2) numBuckets = n/2;
        if (numBuckets > 10) numBuckets = 10;
        
        buckets.resize(numBuckets);
        bucketSizes.resize(numBuckets, 0);
        
        initialized = true;
    }

    void BucketSort::step(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.sortingComplete || !stats.isSorting) {
            return;
        }

        auto& array = data.getArray();
        size_t n = array.size();

        if (stats.stateRestored) {
            currentBucket = stats.lastRestoredI;
            currentElement = stats.lastRestoredJ;
            stats.stateRestored = false;
        } else {
            data.resetHighlighting();
            
            stats.saveState(data, currentBucket, currentElement, false);
        }

        // Initialize buckets if not already done
        if (!initialized) {
            initializeBuckets(data);
            stats.currentStep++;
            return;
        }

        // Distribution phase: distribute elements into buckets
        if (distributionPhase) {
            stepDistribution(data, stats);
        }
        // Collection phase: collect elements from buckets
        else {
            stepCollection(data, stats);
        }

        if (stats.steppingMode) {
            stats.isSorting = false;
        }
    }

    void BucketSort::stepDistribution(Visualization::VisualizationData& data, SortingStats& stats) {
        auto& array = data.getArray();
        size_t n = array.size();
        
        static int minVal = 0;
        static int maxVal = 0;
        
        // Find min and max on first element only
        if (currentElement == 0) {
            minVal = array[0].value;
            maxVal = array[0].value;
            for (size_t i = 1; i < n; i++) {
                stats.comparisons += 2;
                if (array[i].value < minVal) minVal = array[i].value;
                if (array[i].value > maxVal) maxVal = array[i].value;
            }
        }
        
        if (currentElement < n) {
            array[currentElement].isComparing = true;
            stats.currentStep++;
            
            // Calculate which bucket this element belongs to
            int value = array[currentElement].value;
            size_t bucketIndex = 0;
            
            if (maxVal > minVal) {
                double normalizedValue = static_cast<double>(value - minVal) / (maxVal - minVal);
                bucketIndex = static_cast<size_t>(normalizedValue * buckets.size());
                if (bucketIndex >= buckets.size()) bucketIndex = buckets.size() - 1;
            }
            
            // Add element to appropriate bucket
            buckets[bucketIndex].push_back(value);
            bucketSizes[bucketIndex]++;
            
            stats.swaps++;
            
            array[currentElement].isSwapping = true;
            
            currentElement++;
        } else {
            for (size_t i = 0; i < buckets.size(); i++) {
                insertionSortBucket(buckets[i], stats);
            }
            
            distributionPhase = false;
            currentBucket = 0;
            currentElement = 0;
            outputIndex = 0;
        }
    }

    void BucketSort::stepCollection(Visualization::VisualizationData& data, SortingStats& stats) {
        auto& array = data.getArray();
        
        if (currentBucket < buckets.size()) {
            if (currentElement < bucketSizes[currentBucket]) {
                int value = buckets[currentBucket][currentElement];
                
                if (outputIndex > 0) {
                    array[outputIndex-1].isSwapping = false;
                }
                
                array[outputIndex].isComparing = true;
                array[outputIndex].isSwapping = true;
                
                int oldValue = array[outputIndex].value;
                array[outputIndex].value = value;
                
                stats.comparisons++;
                
                if (oldValue != value) {
                    stats.swaps++;
                }
                
                stats.currentStep++;
                
                currentElement++;
                outputIndex++;
            } else {
                currentBucket++;
                currentElement = 0;
            }
        } else {
            stats.sortingComplete = true;
            stats.isSorting = false;
            data.resetHighlighting();
        }
    }

    /// <summary>
	/// Insertion sort is used to sort individual buckets.
    /// </summary>
    void BucketSort::insertionSortBucket(std::vector<int>& bucket, SortingStats& stats) {
        if (bucket.size() <= 1) return;
        
        for (size_t i = 1; i < bucket.size(); i++) {
            int key = bucket[i];
            int j = static_cast<int>(i) - 1;
            
            while (j >= 0) {
                stats.comparisons++;
                
                if (bucket[j] > key) {
                    bucket[j + 1] = bucket[j];
                    stats.swaps++;
                    j--;
                } else {
                    break;
                }
            }
            
            if (j + 1 != i) {
                bucket[j + 1] = key;
                stats.swaps++;
            }
        }
    }

    void BucketSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.currentStep == 0) {
            reset();
            stats.reset();
        }
        stats.isSorting = true;

        if (stats.steppingMode) {
            step(data, stats);
            return;
        }

        while (stats.isSorting && !stats.sortingComplete) {
            step(data, stats);
            std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
        }

        data.resetHighlighting();
        if (stats.isSorting) {
            stats.sortingComplete = true;
            stats.isSorting = false;
        }
    }

}