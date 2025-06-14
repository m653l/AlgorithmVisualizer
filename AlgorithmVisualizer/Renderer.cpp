#define _CRT_SECURE_NO_WARNINGS
#include "Renderer.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

namespace Rendering {

    Renderer::Renderer() {}

    bool Renderer::renderControls(Visualization::VisualizationData& data, Algorithms::SortingStats& stats, int& arraySize, bool isStepsEnable) {
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
           
        
            ImGui::InputTextWithHint("##Num", "Enter numbers separated by spaces (e.g., 5 3 8 1 9)", data.buf, 256);
            if (ImGui::Button("Enter", ImVec2(200, 30))) {
                if (strlen(data.buf) > 0) {
                    data.array.clear();
                    
                    char bufCopy[256];
                    strcpy(bufCopy, data.buf);
                    
                    char* token = std::strtok(bufCopy, " ");

                    while (token != nullptr) {
                        Visualization::ArrayElement elem{ std::atoi(token), false, false };
                        data.array.push_back(elem);

                        token = std::strtok(NULL, " ");
                    }
                }
            }
        
            ImGui::InputTextWithHint("##Path", "Enter CSV file path (e.g., C:\\data\\numbers.csv)", data.buf1, 256);
            if (ImGui::Button("Input", ImVec2(200, 30))) {
                std::ifstream file(data.buf1);
                std::string line;

                if (file.is_open()) {
                    data.array.clear();
                
                    while (std::getline(file, line)) {
                        std::stringstream strstream(line);
                        std::string value;
                        while (std::getline(strstream, value, ' ')) {
                            const char* v1 = value.c_str();
                            Visualization::ArrayElement elem{ std::atoi(v1), false, false };
                            data.array.push_back(elem);
                        }
                    }
                    file.close();
                }
                else {
                    ImGui::OpenPopup("File Error");
                }
            }

            if (ImGui::BeginPopupModal("File Error", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Error: Could not open the file.\nPlease check the path and try again.");
                ImGui::Separator();

                if (ImGui::Button("OK", ImVec2(120, 0))) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();

            }
        }
        // Execution mode checkbox
        if (!stats.isSorting && !stats.sortingComplete && isStepsEnable) {
            // Store previous stepping mode state
            bool prevSteppingMode = stats.steppingMode;
            
            // Toggle stepping mode
            ImGui::Checkbox("Step-by-Step Mode", &stats.steppingMode);
            
            // If we're exiting stepping mode, make sure we preserve the algorithm state
            // but don't start sorting automatically
            if (prevSteppingMode && !stats.steppingMode) {
                stats.isSorting = false;
            }
            if (stats.steppingMode) {
                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                    ImGui::TextUnformatted("In step-by-step mode, you can control the algorithm execution manually using the Step Forward and Step Backward buttons.");
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
            }
        }

        // Start/Stop sorting button
        if (data.size() > 0) {
            if (stats.steppingMode == false && !stats.sortingComplete)
            {
                // Speed control slider
                ImGui::Separator();
                ImGui::Text("Execution Speed");
                ImGui::SliderInt("Delay (ms)", &stats.speedFactor, 1, 500, "%d ms");
            }

            if (!stats.isSorting && !stats.sortingComplete && stats.steppingMode == false) {
                // If we have a currentStep > 0, show Resume button instead of Start
                if (stats.currentStep > 0) {
                    if (ImGui::Button("Resume Sorting", ImVec2(200, 30))) {
                        stats.isSorting = true;
                        stats.sortingComplete = false;
                    }
                } else {
                    if (ImGui::Button("Start Sorting", ImVec2(200, 30))) {
                        stats.isSorting = true;
                        stats.sortingComplete = false;
                        
                        if (stats.currentStep == 0) {
                            stats.comparisons = 0;
                            stats.swaps = 0;
                            stats.history.clear();
                        }
                    }
                }
            }
            else if (stats.isSorting && stats.steppingMode == false) {
                if (ImGui::Button("Stop Sorting", ImVec2(200, 30))) {
                    stats.isSorting = false;
                }
            }
            else if (stats.sortingComplete) {
                if (ImGui::Button("Reset", ImVec2(200, 30))) {
                    stats.reset();
                }
            }
            
            // Step controls (only visible in stepping mode or when paused)
            if (stats.steppingMode && !stats.isSorting && !stats.sortingComplete) {
                ImGui::Separator();
                ImGui::Text("Stepping Controls");
                
                // Step forward button
                if (ImGui::Button("Step Forward", ImVec2(95, 30))) {
                    stats.isSorting = true; // This will trigger one step and then pause again
                }
                
                // Step backward button (only enabled if we have history)
                ImGui::SameLine();
                if (ImGui::Button("Step Backward", ImVec2(95, 30)) && stats.canStepBackward() && !stats.history.empty()) {
                    // Get the previous state
                    if (stats.currentStep > 0) {
                        stats.currentStep--;
                        
                        // Pop the last state from history
                        auto& prevState = stats.history.back();
                        
                        // Restore algorithm state
                        stats.comparisons = prevState.comparisons;
                        stats.swaps = prevState.swaps;
                        
                        // Restore array state with highlighting
                        auto& array = data.getArray();
                        array = prevState.array;
                        
                        // Explicitly set highlighting for the elements being compared
                        // This ensures the highlighting is visible when stepping backward
                        if (prevState.j < array.size() && prevState.j + 1 < array.size()) {
                            array[prevState.j].isComparing = true;
                            array[prevState.j + 1].isComparing = true;
                            
                            // If a swap occurred, also set the swapping flag
                            if (prevState.swapped) {
                                array[prevState.j].isSwapping = true;
                                array[prevState.j + 1].isSwapping = true;
                            }
                        }
                        
                        // Store algorithm state variables for BubbleSort to retrieve
                        stats.lastRestoredI = prevState.i;
                        stats.lastRestoredJ = prevState.j;
                        stats.lastRestoredSwapped = prevState.swapped;
                        stats.stateRestored = true;
                        
                        // Remove the state from history
                        stats.history.pop_back();
                    }
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