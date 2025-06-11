#pragma once

namespace Algorithms {
    struct SortingStats {
        int comparisons = 0;
        int swaps = 0;
        int currentStep = 0;
        bool isSorting = false;
        bool sortingComplete = false;
        int speedFactor = 50;

        void reset() {
            comparisons = 0;
            swaps = 0;
            currentStep = 0;
            isSorting = false;
            sortingComplete = false;
        }
    };
}