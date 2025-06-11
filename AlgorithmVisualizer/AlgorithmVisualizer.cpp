#include <iostream>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>

#include "Renderer.h"
#include "BubbleSort.h"
#include "InsertionSort.h"
#include "ArrayGenerator.h"

#include <thread>

static GLFWwindow* window = nullptr;

static void GLFWErrorCallback(int error, const char* description)
{
	std::cout << "GLFW Error: " << description << " code: " << error << "\n";
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

	// Initialize visualization components
	Visualization::VisualizationData visualizationData(100);
	Algorithms::SortingStats sortingStats;
	Rendering::Renderer renderer;
	Algorithms::BubbleSort bubbleSort;
	int arraySize = 100;

	// Generate initial random array
	Utils::ArrayGenerator::generateRandomArray(visualizationData);

	// Sorting thread
	std::thread sortingThread;

	// InsertionSort
	Visualization::VisualizationData visualizationData2(100);
	Algorithms::SortingStats sortingStats2;
	Rendering::Renderer renderer2;
	Algorithms::InsertionSort insertionSort;
	int arraySize2 = 100;

	Utils::ArrayGenerator::generateRandomArray(visualizationData2);

	std::thread sortingThread2;

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Setup
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// UI
		// Create a window
		ImGui::Begin("Sorting Algorithm Visualization", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		// Render controls and check if we need to generate a new array
		bool generateNewArray = renderer.renderControls(visualizationData, sortingStats, arraySize);

		if (generateNewArray) {
			Utils::ArrayGenerator::generateRandomArray(visualizationData);
			sortingStats.reset();
			bubbleSort.reset();
		}

		if (!sortingStats.isSorting && sortingThread.joinable()) {
			sortingThread.join();
		}

		if (sortingStats.isSorting && !sortingThread.joinable()) {
			sortingThread = std::thread(&Algorithms::BubbleSort::run, &bubbleSort,
				std::ref(visualizationData), std::ref(sortingStats));
		}

		if (sortingStats.isSorting && !sortingThread.joinable()) {
			bubbleSort.step(visualizationData, sortingStats);
			std::this_thread::sleep_for(std::chrono::milliseconds(sortingStats.speedFactor));
		}

		renderer.renderStatistics(sortingStats, visualizationData.size());
		renderer.renderArrayVisualization(visualizationData, sortingStats);

		ImGui::End();

		ImGui::Begin("InsertionSort", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		bool generateNewArray2 = renderer2.renderControls(visualizationData2, sortingStats2, arraySize2);

		if (generateNewArray2) {
			Utils::ArrayGenerator::generateRandomArray(visualizationData2);
			sortingStats2.reset();
			insertionSort.reset();
		}

		if (!sortingStats2.isSorting && sortingThread2.joinable()) {
			sortingThread2.join();
		}

		if (sortingStats2.isSorting && !sortingThread2.joinable()) {
			sortingThread2 = std::thread(&Algorithms::InsertionSort::run, &insertionSort,
				std::ref(visualizationData2), std::ref(sortingStats2));
		}

		if (sortingStats2.isSorting && !sortingThread2.joinable()) {
			insertionSort.step(visualizationData2, sortingStats2);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}

		renderer.renderStatistics(sortingStats2, visualizationData2.size());
		renderer.renderArrayVisualization(visualizationData2, sortingStats2);

		ImGui::End();

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
	
	if (sortingStats.isSorting) {
		sortingStats.isSorting = false;
		if (sortingThread.joinable()) {
			sortingThread.join();
		}
	}

	if (sortingStats2.isSorting) {
		sortingStats2.isSorting = false;
		if (sortingThread2.joinable()) {
			sortingThread2.join();
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