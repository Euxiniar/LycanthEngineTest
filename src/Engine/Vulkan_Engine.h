#pragma once
#include <vector>
#include <vulkan\vulkan.h>
#include <cassert>
#include <memory>
#include <iostream>
#include <glm\matrix.hpp>
#include <glm\gtc\matrix_transform.hpp>

typedef struct _swapchain_buffers 
{
	VkImage image;
	VkImageView view;
} swapchain_buffer;

class Vulkan_Engine
{
public:
	Vulkan_Engine(const char * app_name);
	~Vulkan_Engine();


private:
	/*****************************************Functions***************************************************************/
	void init_instance_extension_names();
	VkResult init_instance(char const *const app_short_name);
	void destroy_instance();

	void init_device_extension_names();
	VkResult enumerate_device(uint32_t gpu_count = 1);
	VkResult init_device();
	void destroy_device();

	void init_window_size(int32_t default_width, int32_t default_height);
	void init_window(const char * app_name);
	void destroy_window();

	void init_swapchain_extension();

	void init_command_pool();
	void destroy_command_pool();

	void init_command_buffer();
	void destroy_command_buffer();

	void execute_begin_command_buffer();

	void init_device_queue();

	void init_swapchain(VkImageUsageFlags usageFlags = 17);
	void destroy_swapchain();

	void init_depth_buffer();
	bool memory_type_from_properties(uint32_t typeBits, VkFlags requirements_mask, uint32_t * typeIndex);
	void destroy_depth_buffer();

	void init_uniform_buffer();
	void destroy_uniform_buffer();



	/*****************************************Arguments***************************************************************/
	VkInstance m_inst;
	VkDevice m_device;

	std::vector<const char*> m_instance_extension_names;
	std::vector<const char*> m_device_extension_names;

	std::vector<VkPhysicalDevice> m_gpus;
	VkPhysicalDeviceMemoryProperties m_memory_props;
	VkPhysicalDeviceProperties gpu_props;
	uint32_t m_queue_family_count;
	std::vector<VkQueueFamilyProperties> m_queue_props;

	uint32_t m_graphics_queue_family_index;
	uint32_t m_present_queue_family_index;
	
	int m_height;
	int m_width;

#ifdef _WIN32
#define APP_NAME_STR_LEN 80
	HINSTANCE m_connection;        // hInstance - Windows Instance
	char m_name[APP_NAME_STR_LEN]; // Name to put on the window/icon
	HWND m_window;                 // hWnd - window handle
#endif

	VkSurfaceKHR m_surface;
	VkFormat m_format = VK_FORMAT_UNDEFINED;

	VkCommandPool m_cmd_pool;
	VkCommandBuffer m_cmd;

	VkQueue m_graphics_queue;
	VkQueue m_present_queue;

	VkSwapchainKHR m_swapchain;
	uint32_t m_swapchainImageCount;

	std::vector<swapchain_buffer> m_buffers;
	uint32_t m_current_buffer;

	struct {
		VkFormat format = VK_FORMAT_UNDEFINED;

		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} m_depth;

	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_Model;
	glm::mat4 m_Clip;
	glm::mat4 m_MVP;

	struct {
		VkBuffer buf;
		VkDeviceMemory mem;
		VkDescriptorBufferInfo buffer_info;
	} m_uniform_data;
};