#include "VisualizationData.h"

namespace Visualization {

    VisualizationData::VisualizationData(int size) {
        resize(size);
    }

    void VisualizationData::resetHighlighting() {
        for (auto& element : array) {
            element.isComparing = false;
            element.isSwapping = false;
        }
    }

    void VisualizationData::resize(int newSize) {
        array.resize(newSize);
        resetHighlighting();
    }

}