#pragma once
#include <vulkan/vulkan.h>
#include <functional>

void check_vk_result(VkResult err);

struct GLFWwindow;
class MainApp
{
public:
	MainApp(int wWidth=1280,int wHeight=720);
	~MainApp();
	void Init();
	void InitImGui();
	void Render();
	static MainApp& Get();
	static VkPhysicalDevice& GetPhysicalDevice();
	static VkDevice& GetDevice();
	static VkCommandBuffer& GetCommandBuffer(bool begin);
	static void FlushCommandBuffer(VkCommandBuffer commandBuffer);
	static void SubmitResourceFree(std::function<void()>&& func);
	void ShutDown();
private:
	GLFWwindow* window;
	VkSurfaceKHR surface;
	int windowWidth, windowHeight;
};

