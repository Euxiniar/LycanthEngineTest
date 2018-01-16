//#pragma once
//#include <vulkan\vulkan.h>
//#include <GLFW\glfw3.h>
//
//#include <iostream>
//#include <set>
//#include <vector>
//#include <algorithm>
//#include <fstream>
//
//#ifdef NDEBUG
//const bool enableValidationLayers = false;
//#else
//const bool enableValidationLayers = true;
//#endif
//
//const std::vector<const char*> validationLayers = {
//	"VK_LAYER_LUNARG_standard_validation"
//};
//const std::vector<const char*> deviceExtensions = {
//	VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};
//
//VkResult CreateDebugReportCallbackEXT(VkInstance instance, 
//	const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, 
//	const VkAllocationCallbacks* pAllocator, 
//	VkDebugReportCallbackEXT* pCallback) 
//{
//	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
//	if (func != nullptr) 
//	{
//		return func(instance, pCreateInfo, pAllocator, pCallback);
//	}
//	else 
//	{
//		return VK_ERROR_EXTENSION_NOT_PRESENT;
//	}
//}
//
//void DestroyDebugReportCallbackEXT(VkInstance instance, 
//	VkDebugReportCallbackEXT callback, 
//	const VkAllocationCallbacks* pAllocator) 
//{
//	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
//	if (func != nullptr) 
//	{
//		func(instance, callback, pAllocator);
//	}
//}
//
//struct QueueFamilyIndices 
//{
//	int graphicsFamily = -1;
//	int presentFamily = -1;
//
//	bool isComplete() 
//	{
//		return graphicsFamily >= 0 && presentFamily >= 0;
//	}
//};
//
//struct SwapChainSupportDetails 
//{
//	VkSurfaceCapabilitiesKHR capabilities;
//	std::vector<VkSurfaceFormatKHR> formats;
//	std::vector<VkPresentModeKHR> presentModes;
//};
//
//class Vulkan_Kernel
//{
//public:
//	Vulkan_Kernel();
//	void initVulkan();
//	
//private:
//	GLFWwindow * window;
//
//	VkInstance instance;
//	VkDebugReportCallbackEXT callback;
//	VkSurfaceKHR surface;
//
//	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//	VkDevice device;
//
//	VkQueue graphicsQueue;
//	VkQueue presentQueue;
//
//	VkSwapchainKHR swapChain;
//	std::vector<VkImage> swapChainImages;
//	VkFormat swapChainImageFormat;
//	VkExtent2D swapChainExtent;
//	std::vector<VkImageView> swapChainImageViews;
//	std::vector<VkFramebuffer> swapChainFramebuffers;
//
//	VkRenderPass renderPass;
//	VkPipelineLayout pipelineLayout;
//	VkPipeline graphicsPipeline;
//
//	VkCommandPool commandPool;
//	std::vector<VkCommandBuffer> commandBuffers;
//
//	VkSemaphore imageAvailableSemaphore;
//	VkSemaphore renderFinishedSemaphore;
//
//	void createInstance();
//	bool checkValidationLayerSupport();
//	std::vector<const char*> getRequiredExtensions();
//
//	void setupDebugCallback();
//	void createSurface();
//	void pickPhysicalDevice();
//	bool isDeviceSuitable(VkPhysicalDevice device);
//	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
//	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
//	void createLogicalDevice();
//	void createSwapChain();
//	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
//	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
//	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities);
//	void createImageViews();
//	void createRenderPass();
//	void createGraphicsPipeline();
//	VkShaderModule createShaderModule(const std::vector<char>& code);
//	void createFramebuffers();
//	void createCommandPool();
//	void createCommandBuffers();
//	void createSemaphores();
//	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
//};