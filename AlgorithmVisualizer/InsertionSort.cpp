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

        // Check if we need to restore algorithm state after stepping backward
        if (stats.stateRestored) {
            i = stats.lastRestoredI;
            j = stats.lastRestoredJ;
            temp = array[j + 1].value; // Restore temp value from the array
            stats.stateRestored = false;
            
            // Don't reset highlighting or save state when restoring
            // The highlighting is already set in Renderer.cpp when stepping backward
        } else {
            // Only reset highlighting if we're not restoring a state
            data.resetHighlighting();
            
            // Save current state before making changes (for step backward functionality)
            stats.saveState(data, i, j, false); // InsertionSort doesn't use swapped flag
        }

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
            stats.swaps++;
            stats.currentStep++;
            array[j + 1] = array[j];
            j--;
        } else {
            if (j + 1 != i - 1) {
                stats.swaps++;
            }
            array[j + 1].value = temp;
            j = i - 1;
        }

    }

    void InsertionSort::run(Visualization::VisualizationData& data, SortingStats& stats) {
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

    bool InsertionSort::isComplete(const SortingStats& stats) const {
        return stats.sortingComplete;
    }

    void InsertionSort::reset() {
        i = 0;
        j = 0;
        temp = 0;
    }
}