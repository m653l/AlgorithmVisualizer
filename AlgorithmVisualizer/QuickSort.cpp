#include "QuickSort.h"
#include <thread>
#include <chrono>

namespace Algorithms {

    QuickSort::QuickSort() : Algorithm("Quick Sort") {
        reset();
    }

    void QuickSort::reset() {
        while (!sortStack.empty()) {
            sortStack.pop();
        }
        currentLow = 0;
        currentHigh = 0;
        pivotIndex = 0;
        i = 0;
        j = 0;
        partitioning = false;
        swapped = false;
    }

    bool QuickSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void QuickSort::step(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.sortingComplete || !stats.isSorting) {
            return;
        }

        auto& array = data.getArray();
        int n = array.size();

        if (stats.stateRestored) {
            currentLow = stats.lastRestoredI;
            currentHigh = stats.lastRestoredJ;
            swapped = stats.lastRestoredSwapped;
            stats.stateRestored = false;
        } else {
            data.resetHighlighting();
            
            stats.saveState(data, currentLow, currentHigh, swapped);
        }

        // Initialize the stack if this is the first step
        if (sortStack.empty() && !partitioning) {
            sortStack.push({0, n - 1});
        }

        // If we're currently partitioning, continue the partition step
        if (partitioning) {
            stepPartition(data, stats);
            return;
        }

        // If stack is empty and we're not partitioning, sorting is complete
        if (sortStack.empty()) {
            stats.sortingComplete = true;
            stats.isSorting = false;
            data.resetHighlighting();
            return;
        }

        // Get the next range to sort
        auto range = sortStack.top();
        sortStack.pop();
        currentLow = range.first;
        currentHigh = range.second;

        // If the range has more than one element, start partitioning
        if (currentLow < currentHigh) {
            // Choose pivot (last element)
            pivotIndex = currentHigh;
            array[pivotIndex].isComparing = true;
            
            // Initialize partition variables
            i = currentLow - 1;
            j = currentLow;
            partitioning = true;
            
            stats.currentStep++;
            stats.comparisons++;
        }
        // If the range has 0 or 1 element, it's already sorted, continue to next range
        // This prevents infinite loops !!!
        
        if (stats.steppingMode) {
            stats.isSorting = false;
        }
    }

    void QuickSort::stepPartition(Visualization::VisualizationData& data, SortingStats& stats) {
        auto& array = data.getArray();
        
        if (j < currentHigh) {
            if (j > currentLow) {
                array[j - 1].isComparing = false;
            }
            
            array[j].isComparing = true;
            array[pivotIndex].isComparing = true;
            
            stats.comparisons++;
            stats.currentStep++;
            
            // If current element is smaller than or equal to pivot
            if (array[j].value <= array[pivotIndex].value) {
                i++;
                
                if (i != j) {
                    array[i].isSwapping = true;
                    array[j].isSwapping = true;
                    std::swap(array[i], array[j]);
                    stats.swaps++;
                    swapped = true;
                } else {
                    swapped = false;
                }
            } else {
                swapped = false;
            }
            
            j++;
        } else {
            // Partition is complete, place pivot in correct position
            if (i + 1 != pivotIndex) {
                array[i + 1].isSwapping = true;
                array[pivotIndex].isSwapping = true;
                std::swap(array[i + 1], array[pivotIndex]);
                stats.swaps++;
                stats.currentStep++;
            }
            
            data.resetHighlighting();
            
            int pivotPos = i + 1;
            
            if (pivotPos - 1 > currentLow) {
                sortStack.push({currentLow, pivotPos - 1});
            }
            
            if (pivotPos + 1 < currentHigh) {
                sortStack.push({pivotPos + 1, currentHigh});
            }
            
            partitioning = false;
            
            if (sortStack.empty()) {
                stats.sortingComplete = true;
                stats.isSorting = false;
            }
        }
        
        if (stats.steppingMode) {
            stats.isSorting = false;
        }
    }

    int QuickSort::partition(Visualization::VisualizationData& data, SortingStats& stats, int low, int high) {
        auto& array = data.getArray();
        int pivot = array[high].value;
        int i = low - 1;
        
        stats.comparisons++;
        
        for (int j = low; j < high; j++) {
            if (!stats.isSorting) break;
            
            data.resetHighlighting();
            array[j].isComparing = true;
            array[high].isComparing = true;
            stats.comparisons++;
            
            if (array[j].value <= pivot) {
                i++;
                if (i != j) {
                    array[i].isSwapping = true;
                    array[j].isSwapping = true;
                    std::swap(array[i], array[j]);
                    stats.swaps++;
                }
            }
            
            stats.currentStep++;
            std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
        }
        
        if (stats.isSorting && i + 1 != high) {
            array[i + 1].isSwapping = true;
            array[high].isSwapping = true;
            std::swap(array[i + 1], array[high]);
            stats.swaps++;
            stats.currentStep++;
        }
        
        return i + 1;
    }

    void QuickSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
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