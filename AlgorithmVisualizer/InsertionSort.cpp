#include "InsertionSort.h"
#include <thread>
#include <chrono>

namespace Algorithms {
    InsertionSort::InsertionSort() : Algorithm("Insertion Sort") {
        reset();
    }

    void InsertionSort::step(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.sortingComplete || !stats.isSorting) {
            return;
        }

        auto& array = data.getArray();
        int n = array.size();

        data.resetHighlighting();

        if (i >= n) {
            stats.sortingComplete = true;
            stats.isSorting = false;
            return;
        }

        if (j == i - 1) {
            array[i].isComparing = true;
            temp = array[i].value;
            j = i - 1;
            i++;
        }

        if (j >= 0 && array[j].value > temp) {
            array[j].isComparing = true;
            array[j].isSwapping = true;
            array[j + 1].isSwapping = true;
            stats.comparisons++;
            stats.currentStep++;
            array[j + 1] = array[j];
            j--;
        } else {
            array[j + 1].value = temp;
            j = i - 1;
        }

    }

    void InsertionSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
        reset();
        stats.reset();
        stats.isSorting = true;

        while (stats.isSorting && !stats.sortingComplete) {
            step(data, stats);
            std::this_thread::sleep_for(std::chrono::milliseconds(stats.speedFactor));
        }

        data.resetHighlighting();
        stats.sortingComplete = true;
        stats.isSorting = false;
    }

    bool InsertionSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void InsertionSort::reset() {
        i = 0;
        j = 0;
        temp = 0;
    }
}