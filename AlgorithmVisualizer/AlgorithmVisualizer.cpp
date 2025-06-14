#include <iostream>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>

#include "Renderer.h"
#include "BubbleSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "BucketSort.h"
#include "ArrayGenerator.h"

#include <thread>
#include <vector>
#include <memory>

static GLFWwindow* window = nullptr;

static void GLFWErrorCallback(int error, const char* description)
{
	std::cout << "GLFW Error: " << description << " code: " << error << "\n";
}

// Structure to hold all components for a single algorithm visualization
struct AlgorithmContext {
    std::string name;
    Visualization::VisualizationData visualizationData;
    Algorithms::SortingStats sortingStats;
    Rendering::Renderer renderer;
    std::unique_ptr<Algorithms::Algorithm> algorithm;
    std::thread sortingThread;
    int arraySize;
    bool prevSteppingMode;
    bool isStepsEnable;

    AlgorithmContext(const std::string& name, int size, std::unique_ptr<Algorithms::Algorithm> algo, bool isStepsEnable)
        : name(name),
          visualizationData(size),
          algorithm(std::move(algo)),
          arraySize(size),
          isStepsEnable(isStepsEnable),
          prevSteppingMode(false) {
        // Generate initial random array
        Utils::ArrayGenerator::generateRandomArray(visualizationData);
    }
};

// Function to handle algorithm execution logic
void handleAlgorithmExecution(AlgorithmContext& context) {
    // Render controls and check if we need to generate a new array
    bool generateNewArray = context.renderer.renderControls(context.visualizationData, context.sortingStats, context.arraySize, context.isStepsEnable);

    // Handle mode switching while sorting is in progress
    // If we just switched to stepping mode while sorting was in progress,
    // we need to stop the sorting thread and continue in stepping mode
    if (context.sortingStats.steppingMode && !context.prevSteppingMode && 
        context.sortingStats.isSorting && context.sortingThread.joinable()) {
        context.sortingStats.isSorting = false;
        context.sortingThread.join();
        context.sortingStats.isSorting = true;
    }
    // If we just switched from stepping mode to continuous mode,
    // we don't automatically start sorting - user needs to click Resume
    else if (!context.sortingStats.steppingMode && context.prevSteppingMode && context.sortingStats.currentStep > 0) {
        context.sortingStats.isSorting = false;
    }
    
    // Update previous stepping mode for next frame
    context.prevSteppingMode = context.sortingStats.steppingMode;

    if (generateNewArray) {
        Utils::ArrayGenerator::generateRandomArray(context.visualizationData);
        context.sortingStats.reset();
        context.algorithm->reset();
    }

    if (!context.sortingStats.isSorting && context.sortingThread.joinable()) {
        context.sortingThread.join();
    }

    // Handle continuous sorting (non-stepping mode)
    if (context.sortingStats.isSorting && !context.sortingStats.steppingMode && 
        !context.sortingThread.joinable()) {
        // If we have a current step, continue from where we left off rather than starting a new run
        if (context.sortingStats.currentStep > 0) {
            context.sortingThread = std::thread([&context]() {
                // Continue sorting from current state
                while (context.sortingStats.isSorting && !context.sortingStats.sortingComplete) {
                    context.algorithm->step(context.visualizationData, context.sortingStats);
                    std::this_thread::sleep_for(std::chrono::milliseconds(context.sortingStats.speedFactor));
                }
                
                context.visualizationData.resetHighlighting();
                context.sortingStats.isSorting = false;
            });
        } else {
            context.sortingThread = std::thread(&Algorithms::Algorithm::run, context.algorithm.get(),
                std::ref(context.visualizationData), std::ref(context.sortingStats));
        }
    }

    // Handle step-by-step execution
    if (context.sortingStats.isSorting && context.sortingStats.steppingMode) {
        bool previousSteppingMode = context.sortingStats.steppingMode;
        context.sortingStats.steppingMode = true;
        context.algorithm->step(context.visualizationData, context.sortingStats);
        context.sortingStats.steppingMode = previousSteppingMode;
        
        // Ensure we stop after one step in stepping mode
        context.sortingStats.isSorting = false;
    }

    context.renderer.renderStatistics(context.sortingStats, context.visualizationData.size());
    context.renderer.renderArrayVisualization(context.visualizationData, context.sortingStats);
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}

	glfwSetErrorCallback(GLFWErrorCallback);

	window = glfwCreateWindow(1280, 720, "Algorithm Visualizer", nullptr, nullptr);
	
	glfwMakeContextCurrent(window);

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Create algorithm contexts
    std::vector<AlgorithmContext> algorithms;
    
    // Add BubbleSort
    algorithms.emplace_back("Bubble Sort", 100, std::make_unique<Algorithms::BubbleSort>(), true);
    
    // Add InsertionSort
    algorithms.emplace_back("Insertion Sort", 100, std::make_unique<Algorithms::InsertionSort>(), true);

    // Add MergeSort
    algorithms.emplace_back("Merge Sort", 100, std::make_unique<Algorithms::MergeSort>(), false);

    // Add QuickSort
    algorithms.emplace_back("Quick Sort", 100, std::make_unique<Algorithms::QuickSort>(), true);

    // Add BucketSort
    algorithms.emplace_back("Bucket Sort", 100, std::make_unique<Algorithms::BucketSort>(), true);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Setup
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        // Process each algorithm
        for (auto& context : algorithms) {
            // Create a window for this algorithm
            ImGui::Begin(context.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            
            // Handle algorithm execution
            handleAlgorithmExecution(context);
            
            ImGui::End();
        }

		// Rendering
		ImGui::Render();
		int display_w, display_h;

		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	
    // Clean up all algorithm threads
    for (auto& context : algorithms) {
        if (context.sortingStats.isSorting) {
            context.sortingStats.isSorting = false;
            if (context.sortingThread.joinable()) {
                context.sortingThread.join();
            }
        }
    }

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (window)
	{
		glfwDestroyWindow(window);
	}

	glfwTerminate();
}