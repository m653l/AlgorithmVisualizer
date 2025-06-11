#include "BubbleSort.h"
#include <thread>
#include <chrono>

namespace Algorithms {

    BubbleSort::BubbleSort() : Algorithm("Bubble Sort") {
        reset();
    }

    void BubbleSort::step(Visualization::VisualizationData& data, SortingStats& stats) {
        if (stats.sortingComplete || !stats.isSorting) {
            return;
        }

        auto& array = data.getArray();
        int n = array.size();

        data.resetHighlighting();

        if (i < n - 1) {
            if (j < n - i - 1) {
                array[j].isComparing = true;
                array[j + 1].isComparing = true;
                stats.comparisons++;
                stats.currentStep++;

                if (array[j].value > array[j + 1].value) {
                    array[j].isSwapping = true;
                    array[j + 1].isSwapping = true;
                    stats.swaps++;

                    std::swap(array[j], array[j + 1]);
                    swapped = true;
                }

                j++;
            }
            else {
                j = 0;
                i++;

                if (!swapped) {
                    stats.sortingComplete = true;
                    stats.isSorting = false;
                }
                swapped = false;
            }
        }
        else {
            stats.sortingComplete = true;
            stats.isSorting = false;
        }
    }

    void BubbleSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
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

    bool BubbleSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void BubbleSort::reset() {
        i = 0;
        j = 0;
        swapped = false;
    }
}