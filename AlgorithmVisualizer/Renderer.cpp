#include "Renderer.h"

namespace Rendering {

    Renderer::Renderer() {}

    bool Renderer::renderControls(Visualization::VisualizationData& data, Algorithms::SortingStats& stats, int& arraySize) {
        bool generateNewArray = false;

        ImGui::Text("Sorting Algorithm Controls");
        ImGui::Separator();

        // Array size slider
        if (!stats.isSorting && !stats.sortingComplete) {
            if (ImGui::SliderInt("Array Size", &arraySize, 10, 200)) {
                data.resize(arraySize);
				generateNewArray = true;
            }

            if (ImGui::Button("Generate New Array", ImVec2(200, 30))) {
				generateNewArray = true;
            }
        }
        
        // Speed control slider
        ImGui::Separator();
        ImGui::Text("Execution Speed");
        ImGui::SliderInt("Delay (ms)", &stats.speedFactor, 1, 500, "%d ms");

        // Start/Stop sorting button
        if (data.size() > 0) {
            if (!stats.isSorting && !stats.sortingComplete) {
                if (ImGui::Button("Start Bubble Sort", ImVec2(200, 30))) {
                    stats.isSorting = true;
                    stats.sortingComplete = false;
                    stats.comparisons = 0;
                    stats.swaps = 0;
                    stats.currentStep = 0;
                }
            }
            else if (stats.isSorting) {
                if (ImGui::Button("Stop Sorting", ImVec2(200, 30))) {
                    stats.isSorting = false;
                }
            }
            else if (stats.sortingComplete) {
                if (ImGui::Button("Reset", ImVec2(200, 30))) {
                    stats.reset();
                }
            }
        }

        return generateNewArray;
    }

    void Renderer::renderStatistics(const Algorithms::SortingStats& stats, int arraySize) {
        ImGui::Separator();
        ImGui::Text("Array Size: %d", arraySize);
        ImGui::Text("Comparisons: %d", stats.comparisons);
        ImGui::Text("Swaps: %d", stats.swaps);
        ImGui::Text("Current Step: %d", stats.currentStep);
        ImGui::Text("Status: %s", stats.isSorting ? "Sorting..." : (stats.sortingComplete ? "Sorting Complete" : "Ready"));
    }

    void Renderer::renderArrayVisualization(const Visualization::VisualizationData& data, const Algorithms::SortingStats& stats) {
        ImGui::Separator();
        ImGui::Text("Array Visualization");

        float windowWidth = ImGui::GetContentRegionAvail().x;

        const auto& array = data.getArray();

        if (!array.empty()) {
            float barWidth = (windowWidth - (array.size() * padding)) / array.size();

            for (int i = 0; i < array.size(); i++) {
                
                float barHeight = (array[i].value / 200.0f) * maxBarHeight;
                float barX = ImGui::GetCursorScreenPos().x + (i * (barWidth + padding));
                float barY = ImGui::GetCursorScreenPos().y + maxBarHeight - barHeight;

                ImU32 barColor = IM_COL32(200, 200, 200, 255); // Default color (gray)

                if (array[i].isSwapping) {
                    barColor = IM_COL32(255, 0, 0, 255); // Swapping (red)
                }
                else if (array[i].isComparing) {
                    barColor = IM_COL32(255, 165, 0, 255); // Comparing (orange)
                }
                else if (stats.sortingComplete) {
                    barColor = IM_COL32(0, 255, 0, 255); // Sorted (green)
                }

                ImGui::GetWindowDrawList()->AddRectFilled(
                    ImVec2(barX, barY),
                    ImVec2(barX + barWidth, barY + barHeight),
                    barColor
                );
            }

            ImGui::Dummy(ImVec2(windowWidth, maxBarHeight));
        }
    }

}