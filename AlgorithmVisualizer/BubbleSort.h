#pragma once
#include "Algorithm.h"

namespace Algorithms {

    class BubbleSort : public Algorithm {
    public:
        BubbleSort();

        void step(Visualization::VisualizationData& data, SortingStats& stats) override;

        void run(Visualization::VisualizationData& data, SortingStats& stats) override;

        bool isComplete(const SortingStats& stats) const override;

        void reset() override;

    private:
        int i = 0;
        int j = 0;
        bool swapped = false;
    };

}