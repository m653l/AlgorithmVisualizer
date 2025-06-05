#include "ArrayGenerator.h"
#include <random>

namespace Utils {

    void ArrayGenerator::generateRandomArray(Visualization::VisualizationData& data, int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);

        auto& array = data.getArray();
        for (int i = 0; i < data.size(); i++) {
            array[i].value = distrib(gen);
            array[i].isComparing = false;
            array[i].isSwapping = false;
        }
    }

}