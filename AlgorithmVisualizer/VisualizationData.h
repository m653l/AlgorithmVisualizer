#pragma once
#include <vector>

namespace Visualization {

    struct ArrayElement {
        int value;
        bool isComparing = false;
        bool isSwapping = false;
    };

    class VisualizationData {
    public:
        VisualizationData(int size = 100);

        std::vector<ArrayElement>& getArray() { return array; }
        const std::vector<ArrayElement>& getArray() const { return array; }

        void resetHighlighting();

        int size() const { return array.size(); }

        void resize(int newSize);

    private:
        std::vector<ArrayElement> array;
    };

}

