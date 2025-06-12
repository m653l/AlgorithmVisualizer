#pragma once
#include "Algorithm.h"
#include <stack>

namespace Algorithms {
    class QuickSort : public Algorithm {
    public:
        QuickSort();

        void step(Visualization::VisualizationData& data, SortingStats& stats) override;

        void run(Visualization::VisualizationData& data, SortingStats& stats) override;

        bool isComplete(const SortingStats& stats) const override;

        void reset() override;

    private:
        std::stack<std::pair<int, int>> sortStack;
        int currentLow = 0;
        int currentHigh = 0;
        int pivotIndex = 0;
        int i = 0;
        int j = 0;
        bool partitioning = false;
        bool swapped = false;
        
        int partition(Visualization::VisualizationData& data, SortingStats& stats, int low, int high);
        void stepPartition(Visualization::VisualizationData& data, SortingStats& stats);
    };
}