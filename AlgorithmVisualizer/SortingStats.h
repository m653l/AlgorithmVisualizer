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
        
        // Variables for state restoration when stepping backward
        int lastRestoredI = 0;
        int lastRestoredJ = 0;
        bool lastRestoredSwapped = false;
        bool stateRestored = false;
        
        // History of algorithm states for stepping backward
        std::vector<AlgorithmState> history;
        
        void reset() {
            comparisons = 0;
            swaps = 0;
            currentStep = 0;
            isSorting = false;
            sortingComplete = false;
            steppingMode = false;
            lastRestoredI = 0;
            lastRestoredJ = 0;
            lastRestoredSwapped = false;
            stateRestored = false;
            history.clear();
        }
        
        // Save current state to history
        void saveState(const Visualization::VisualizationData& data, int i, int j, bool swapped) {
            // Limit history size to prevent excessive memory usage
            const size_t MAX_HISTORY_SIZE = 100;
            
            // If history is getting too large, remove oldest entries
            if (history.size() >= MAX_HISTORY_SIZE) {
                // Remove the oldest entry
                history.erase(history.begin());
            }
            
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