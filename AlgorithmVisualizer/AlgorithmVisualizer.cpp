#include <iostream>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <imgui_impl_glfw.h>

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

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, World!");
		ImGui::Text("This is a simple algorithm visualizer.");
		ImGui::End();

		ImGui::Render();
		int display_w, display_h;

		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
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