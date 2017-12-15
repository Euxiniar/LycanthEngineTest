#include "Vulkan_Engine.h"



Vulkan_Engine::Vulkan_Engine(const char* app_name)
{
	init_instance_extension_names();
	init_instance(app_name);
	
	init_device_extension_names();
	
	init_window_size(500, 500);
	init_window(app_name);

	enumerate_device();
	init_swapchain_extension();
	init_device();

	init_command_pool();
	init_command_buffer();

	execute_begin_command_buffer();

	init_device_queue();


}


Vulkan_Engine::~Vulkan_Engine()
{
	destroy_command_buffer();
	destroy_command_pool();
	destroy_device();
	destroy_window();
	destroy_instance();
}


/*************************************************************Vulkan Instance Part*******************************************************************************************/
void Vulkan_Engine::init_instance_extension_names()
{
	m_instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(_WIN32)
	m_instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
}

VkResult Vulkan_Engine::init_instance(char const *const app_short_name)
{
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = app_short_name;
	app_info.applicationVersion = 1;
	app_info.pEngineName = app_short_name;
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledLayerCount = 0;// instance_layer_names.size();
	inst_info.ppEnabledLayerNames = NULL;// instance_layer_names.size() ? instance_layer_names.data() : NULL;
	inst_info.enabledExtensionCount = m_instance_extension_names.size();
	inst_info.ppEnabledExtensionNames = m_instance_extension_names.data();

	VkResult res = vkCreateInstance(&inst_info, NULL, &m_inst);
	assert(res == VK_SUCCESS);

	return res;
}

void Vulkan_Engine::destroy_instance()
{
	vkDestroyInstance(m_inst, NULL);
}

/*************************************************************Device Part*******************************************************************************************/
void Vulkan_Engine::init_device_extension_names()
{
	m_device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VkResult Vulkan_Engine::enumerate_device(uint32_t gpu_count)
{
	uint32_t const req_count = gpu_count;
	VkResult res = vkEnumeratePhysicalDevices(m_inst, &gpu_count, NULL);
	assert(res == VK_SUCCESS);
	assert(gpu_count);

	m_gpus.resize(gpu_count);

	res = vkEnumeratePhysicalDevices(m_inst, &gpu_count, m_gpus.data());
	assert(!res && gpu_count >= req_count);

	vkGetPhysicalDeviceQueueFamilyProperties(m_gpus[0], &m_queue_family_count, NULL);
	assert(m_queue_family_count >= 1);

	m_queue_props.resize(m_queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(m_gpus[0], &m_queue_family_count, m_queue_props.data());
	assert(m_queue_family_count >= 1);

	/* This is as good a place as any to do this */
	vkGetPhysicalDeviceMemoryProperties(m_gpus[0], &m_memory_props);
	vkGetPhysicalDeviceProperties(m_gpus[0], &gpu_props);

	return res;
}

VkResult Vulkan_Engine::init_device()
{
	VkResult res;
	VkDeviceQueueCreateInfo queue_info = {};

	float queue_priorities[1] = { 0.0 };
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = NULL;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = queue_priorities;
	queue_info.queueFamilyIndex = m_graphics_queue_family_index;

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = NULL;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.enabledExtensionCount = m_device_extension_names.size();
	device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? m_device_extension_names.data() : NULL;
	device_info.pEnabledFeatures = NULL;

	res = vkCreateDevice(m_gpus[0], &device_info, NULL, &m_device);
	assert(res == VK_SUCCESS);

	return res;
}

void Vulkan_Engine::destroy_device()
{
	vkDeviceWaitIdle(m_device);
	vkDestroyDevice(m_device, NULL);
}

/*************************************************************Window Part*******************************************************************************************/
void Vulkan_Engine::init_window_size(int32_t default_width, int32_t default_height)
{
	m_width = default_width;
	m_height = default_height;
}

#ifdef _WIN32
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		break;
	default:
		break;
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void Vulkan_Engine::init_window(const char* app_name)
{
	WNDCLASSEX win_class;
	assert(m_width > 0);
	assert(m_height > 0);

	m_connection = GetModuleHandle(NULL);
	sprintf(m_name, app_name);

	// Initialize the window class structure:
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WndProc;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = m_connection;  // hInstance
	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = m_name;
	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	// Register window class:
	if (!RegisterClassEx(&win_class)) {
		// It didn't work, so try to give a useful error:
#if defined(_WIN32)
		MessageBox(m_window, "Unexpected error trying to start the application!", "Error", MB_ICONERROR | IDCANCEL);
#else
		std::cout << "Unexpected error trying to start the application!" << std::endl;
#endif
		exit(1);
	}
	// Create window with the registered class:
	RECT wr = { 0, 0, m_width, m_height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	m_window = CreateWindowEx(0,
		m_name,             // class name
		m_name,             // app name
		WS_OVERLAPPEDWINDOW |  // window style
		WS_VISIBLE | WS_SYSMENU,
		100, 100,            // x/y coords
		wr.right - wr.left,  // width
		wr.bottom - wr.top,  // height
		NULL,                // handle to parent
		NULL,                // handle to menu
		m_connection,     // hInstance
		NULL);               // no extra parameters
	if (!m_window) {
		// It didn't work, so try to give a useful error:
#if defined(_WIN32)
		MessageBox(m_window, "Cannot create a window in which to draw!", "Error", MB_ICONERROR | IDCANCEL);
#else
		std::cout << "Cannot create a window in which to draw!\n";
#endif
		exit(1);
	}
	SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)this);
}

void Vulkan_Engine::destroy_window() 
{
	vkDestroySurfaceKHR(m_inst, m_surface, NULL);
	DestroyWindow(m_window);
}
#endif

/*************************************************************Swapchain Part*******************************************************************************************/
void Vulkan_Engine::init_swapchain_extension()
{
	VkResult res;

#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = NULL;
	createInfo.hinstance = m_connection;
	createInfo.hwnd = m_window;
	res = vkCreateWin32SurfaceKHR(m_inst, &createInfo, NULL, &m_surface);
#endif
	assert(res == VK_SUCCESS);

	// Iterate over each queue to learn whether it supports presenting:
	std::vector<VkBool32> pSupportsPresent;
	pSupportsPresent.resize(m_queue_family_count);
	for (uint32_t i = 0; i < m_queue_family_count; i++) {
		vkGetPhysicalDeviceSurfaceSupportKHR(m_gpus[0], i, m_surface, &pSupportsPresent[i]);
	}

	// Search for a graphics and a present queue in the array of queue
	// families, try to find one that supports both
	m_graphics_queue_family_index = UINT32_MAX;
	m_present_queue_family_index = UINT32_MAX;
	for (uint32_t i = 0; i < m_queue_family_count; ++i) {
		if ((m_queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) 
		{
			if (m_graphics_queue_family_index == UINT32_MAX)
			{
				m_graphics_queue_family_index = i;
			}

			if (pSupportsPresent[i] == VK_TRUE) 
			{
				m_graphics_queue_family_index = i;
				m_present_queue_family_index = i;
				break;
			}
		}
	}

	if (m_present_queue_family_index == UINT32_MAX) {
		// If didn't find a queue that supports both graphics and present, then
		// find a separate present queue.
		for (size_t i = 0; i < m_queue_family_count; ++i)
			if (pSupportsPresent[i] == VK_TRUE) {
				m_present_queue_family_index = i;
				break;
			}
	}

	// Generate error if could not find queues that support graphics
	// and present
	if (m_graphics_queue_family_index == UINT32_MAX || m_present_queue_family_index == UINT32_MAX) 
	{
#if defined(_WIN32)
		MessageBox(m_window, "Could not find a queues for both graphics and present", "Error", MB_ICONERROR | IDCANCEL);
#else
		std::cout << "Could not find a queues for both graphics and present\n";
#endif
		exit(-1);
	}

	// Get the list of VkFormats that are supported
	uint32_t formatCount;
	res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_gpus[0], m_surface, &formatCount, NULL);
	assert(res == VK_SUCCESS);
	VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
	res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_gpus[0], m_surface, &formatCount, surfFormats);
	assert(res == VK_SUCCESS);

	if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) 
	{
		m_format = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else 
	{
		assert(formatCount >= 1);
		m_format = surfFormats[0].format;
	}
	free(surfFormats);
}

/*************************************************************Command Pool Part*******************************************************************************************/
void Vulkan_Engine::init_command_pool()
{
	/* DEPENDS on init_swapchain_extension() */
	VkResult res;

	VkCommandPoolCreateInfo cmd_pool_info = {};
	cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmd_pool_info.pNext = NULL;
	cmd_pool_info.queueFamilyIndex = m_graphics_queue_family_index;
	cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	res = vkCreateCommandPool(m_device, &cmd_pool_info, NULL, &m_cmd_pool);
	assert(res == VK_SUCCESS);
}

void Vulkan_Engine::destroy_command_pool()
{
	vkDestroyCommandPool(m_device, m_cmd_pool, NULL);
}

/*************************************************************Command Buffer Part*******************************************************************************************/
void Vulkan_Engine::init_command_buffer() 
{
	/* DEPENDS on init_swapchain_extension() and init_command_pool() */
	VkResult res;

	VkCommandBufferAllocateInfo cmd = {};
	cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd.pNext = NULL;
	cmd.commandPool = m_cmd_pool;
	cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd.commandBufferCount = 1;

	res = vkAllocateCommandBuffers(m_device, &cmd, &m_cmd);
	assert(res == VK_SUCCESS);
}

void Vulkan_Engine::destroy_command_buffer()
{
	VkCommandBuffer cmd_bufs[1] = { m_cmd };
	vkFreeCommandBuffers(m_device, m_cmd_pool, 1, cmd_bufs);
}

void Vulkan_Engine::execute_begin_command_buffer() 
{
	/* DEPENDS on init_command_buffer() */
	VkResult res;

	VkCommandBufferBeginInfo cmd_buf_info = {};
	cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmd_buf_info.pNext = NULL;
	cmd_buf_info.flags = 0;
	cmd_buf_info.pInheritanceInfo = NULL;

	res = vkBeginCommandBuffer(m_cmd, &cmd_buf_info);
	assert(res == VK_SUCCESS);
}

void Vulkan_Engine::init_device_queue() {
	/* DEPENDS on init_swapchain_extension() */
	vkGetDeviceQueue(m_device, m_graphics_queue_family_index, 0, &m_graphics_queue);
	if (m_graphics_queue_family_index == m_present_queue_family_index) 
	{
		m_present_queue = m_graphics_queue;
	}
	else 
	{
		vkGetDeviceQueue(m_device, m_present_queue_family_index, 0, &m_present_queue);
	}
}