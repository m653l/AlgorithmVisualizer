#pragma once
#include "VisualizationData.h"
#include "SortingStats.h"
#include "imgui.h"

namespace Rendering {

    class Renderer {
    public:
        Renderer();

        bool renderControls(Visualization::VisualizationData& data, Algorithms::SortingStats& stats, int& arraySize);

        void renderArrayVisualization(const Visualization::VisualizationData& data, const Algorithms::SortingStats& stats);

        void renderStatistics(const Algorithms::SortingStats& stats, int arraySize);

    private:
        float maxBarHeight = 300.0f;
        float padding = 1.0f;
    };

}
