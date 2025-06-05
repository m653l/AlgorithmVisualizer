#pragma once
#include "VisualizationData.h"

namespace Utils {
    class ArrayGenerator {
    public:
        static void generateRandomArray(Visualization::VisualizationData& data, int min = 5, int max = 200);
    };
}

