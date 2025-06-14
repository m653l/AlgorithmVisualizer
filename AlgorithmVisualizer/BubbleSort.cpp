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

        // Check if we need to restore algorithm state after stepping backward
        if (stats.stateRestored) {
            i = stats.lastRestoredI;
            j = stats.lastRestoredJ;
            swapped = stats.lastRestoredSwapped;
            stats.stateRestored = false;
            
            // Don't reset highlighting or save state when restoring
            // The highlighting is already set in Renderer.cpp when stepping backward
        } else {
            // Only reset highlighting if we're not restoring a state
            data.resetHighlighting();
            
            // Save current state before making changes (for step backward functionality)
            stats.saveState(data, i, j, swapped);
        }

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
        
        // If in stepping mode, pause after each step
        if (stats.steppingMode) {
            stats.isSorting = false;
        }
    }

    void BubbleSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
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

    bool BubbleSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void BubbleSort::reset() {
        i = 0;
        j = 0;
        swapped = false;
    }
}