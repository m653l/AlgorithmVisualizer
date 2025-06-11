#pragma once
#include <vector>
#include "VisualizationData.h"

namespace Algorithms {
    // Structure to store the state of the algorithm at each step
    struct AlgorithmState {
        int i = 0;
        int j = 0;
        bool swapped = false;
        int comparisons = 0;
        int swaps = 0;
        std::vector<Visualization::ArrayElement> array;
    };

    struct SortingStats {
        int comparisons = 0;
        int swaps = 0;
        int currentStep = 0;
        bool isSorting = false;
        bool sortingComplete = false;
        int speedFactor = 50;
        bool steppingMode = false;
        
        // History of algorithm states for stepping backward
        std::vector<AlgorithmState> history;
        
        void reset() {
            comparisons = 0;
            swaps = 0;
            currentStep = 0;
            isSorting = false;
            sortingComplete = false;
            steppingMode = false;
            history.clear();
        }
        
        // Save current state to history
        void saveState(const Visualization::VisualizationData& data, int i, int j, bool swapped) {
            AlgorithmState state;
            state.i = i;
            state.j = j;
            state.swapped = swapped;
            state.comparisons = comparisons;
            state.swaps = swaps;
            state.array = data.getArray();
            history.push_back(state);
        }
        
        // Can we step backward?
        bool canStepBackward() const {
            return currentStep > 0 && !history.empty();
        }
    };
}