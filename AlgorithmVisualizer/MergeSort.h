#pragma once
#include "Algorithm.h"

namespace Algorithms {

    class MergeSort : public Algorithm {
    public:
        MergeSort();

        void step(Visualization::VisualizationData&, SortingStats&) override {} // brak trybu krokowego
        void run(Visualization::VisualizationData& data, SortingStats& stats) override;
        bool isComplete(const SortingStats& stats) const override;
        void reset() override;

    private:
        void mergeSort(Visualization::VisualizationData& data, SortingStats& stats, int left, int right);
        void merge(Visualization::VisualizationData& data, SortingStats& stats, int left, int mid, int right);
        bool completed = false;
    };

}
