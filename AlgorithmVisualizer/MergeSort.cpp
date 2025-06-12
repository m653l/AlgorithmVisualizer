#include "MergeSort.h"
#include <thread>
#include <chrono>

namespace Algorithms {
    MergeSort::MergeSort() : Algorithm("Merge Sort") {
        reset();
    }

    void MergeSort::step(Visualization::VisualizationData& data, SortingStats& stats) {
    
    }

    void MergeSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
        // Only reset if we're starting a new sort
        if (stats.currentStep == 0) {
            reset();
            stats.reset();
        }
        stats.isSorting = true;

        // If in stepping mode, we'll only do one step at a time
        if (stats.steppingMode) {
            step(data, stats);
            return;
        }

        // Normal continuous execution
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

    bool MergeSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void MergeSort::reset() {
        i = 0;
        j = 0;
        temp = 0;
    }
}