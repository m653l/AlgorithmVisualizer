#pragma once
#include "Algorithm.h"
#include <vector>

namespace Algorithms {
    class BucketSort : public Algorithm {
    public:
        BucketSort();

        void step(Visualization::VisualizationData& data, SortingStats& stats) override;

        void run(Visualization::VisualizationData& data, SortingStats& stats) override;

        bool isComplete(const SortingStats& stats) const override;

        void reset() override;

    private:
        std::vector<std::vector<int>> buckets;
        std::vector<size_t> bucketSizes;
        size_t currentBucket;
        size_t currentElement;
        size_t outputIndex;
        bool distributionPhase;
        bool initialized;
        
        // Helper methods
        void initializeBuckets(const Visualization::VisualizationData& data);
        void stepDistribution(Visualization::VisualizationData& data, SortingStats& stats);
        void stepCollection(Visualization::VisualizationData& data, SortingStats& stats);
        void insertionSortBucket(std::vector<int>& bucket, SortingStats& stats);
    };
}