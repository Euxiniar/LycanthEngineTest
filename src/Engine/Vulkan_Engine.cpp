//#include "Vulkan_Engine.h"
//
//
//
//Vulkan_Engine::Vulkan_Engine(const char* app_name)
//{
//	init_instance_extension_names();
//	init_instance(app_name);
//	
//	init_device_extension_names();
//	
//	init_window_size(500, 500);
//	init_window(app_name);
//
//	enumerate_device();
//	init_swapchain_extension();
//	init_device();
//
//	init_command_pool();
//	init_command_buffer();
//
//	execute_begin_command_buffer();
//
//	init_device_queue();
//
//	init_swapchain();
//
//	init_depth_buffer();
//
//	init_uniform_buffer();
//}
//
//
//Vulkan_Engine::~Vulkan_Engine()
//{
//	destroy_uniform_buffer();
//	destroy_depth_buffer();
//	destroy_swapchain();
//	destroy_command_buffer();
//	destroy_command_pool();
//	destroy_device();
//	destroy_window();
//	destroy_instance();
//}
//
//
///*************************************************************Vulkan Instance Part*******************************************************************************************/
//void Vulkan_Engine::init_instance_extension_names()
//{
//	m_instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
//
//#if defined(_WIN32)
//	m_instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
//#endif
//}
//
//VkResult Vulkan_Engine::init_instance(char const *const app_short_name)
//{
//	VkApplicationInfo app_info = {};
//	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//	app_info.pNext = NULL;
//	app_info.pApplicationName = app_short_name;
//	app_info.applicationVersion = 1;
//	app_info.pEngineName = app_short_name;
//	app_info.engineVersion = 1;
//	app_info.apiVersion = VK_API_VERSION_1_0;
//
//	VkInstanceCreateInfo inst_info = {};
//	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//	inst_info.pNext = NULL;
//	inst_info.flags = 0;
//	inst_info.pApplicationInfo = &app_info;
//	inst_info.enabledLayerCount = 0;// instance_layer_names.size();
//	inst_info.ppEnabledLayerNames = NULL;// instance_layer_names.size() ? instance_layer_names.data() : NULL;
//	inst_info.enabledExtensionCount = (uint32_t)m_instance_extension_names.size();
//	inst_info.ppEnabledExtensionNames = m_instance_extension_names.data();
//
//	VkResult res = vkCreateInstance(&inst_info, NULL, &m_inst);
//	assert(res == VK_SUCCESS);
//
//	return res;
//}
//
//void Vulkan_Engine::destroy_instance()
//{
//	vkDestroyInstance(m_inst, NULL);
//}
//
///*************************************************************Device Part*******************************************************************************************/
//void Vulkan_Engine::init_device_extension_names()
//{
//	m_device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
//}
//
//VkResult Vulkan_Engine::enumerate_device(uint32_t gpu_count)
//{
//	uint32_t const req_count = gpu_count;
//	VkResult res = vkEnumeratePhysicalDevices(m_inst, &gpu_count, NULL);
//	assert(res == VK_SUCCESS);
//	assert(gpu_count);
//
//	m_gpus.resize(gpu_count);
//
//	res = vkEnumeratePhysicalDevices(m_inst, &gpu_count, m_gpus.data());
//	assert(!res && gpu_count >= req_count);
//
//	vkGetPhysicalDeviceQueueFamilyProperties(m_gpus[0], &m_queue_family_count, NULL);
//	assert(m_queue_family_count >= 1);
//
//	m_queue_props.resize(m_queue_family_count);
//	vkGetPhysicalDeviceQueueFamilyProperties(m_gpus[0], &m_queue_family_count, m_queue_props.data());
//	assert(m_queue_family_count >= 1);
//
//	/* This is as good a place as any to do this */
//	vkGetPhysicalDeviceMemoryProperties(m_gpus[0], &m_memory_props);
//	vkGetPhysicalDeviceProperties(m_gpus[0], &gpu_props);
//
//	return res;
//}
//
//VkResult Vulkan_Engine::init_device()
//{
//	VkResult res;
//	VkDeviceQueueCreateInfo queue_info = {};
//
//	float queue_priorities[1] = { 0.0 };
//	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//	queue_info.pNext = NULL;
//	queue_info.queueCount = 1;
//	queue_info.pQueuePriorities = queue_priorities;
//	queue_info.queueFamilyIndex = m_graphics_queue_family_index;
//
//	VkDeviceCreateInfo device_info = {};
//	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//	device_info.pNext = NULL;
//	device_info.queueCreateInfoCount = 1;
//	device_info.pQueueCreateInfos = &queue_info;
//	device_info.enabledExtensionCount = (uint32_t)m_device_extension_names.size();
//	device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? m_device_extension_names.data() : NULL;
//	device_info.pEnabledFeatures = NULL;
//
//	res = vkCreateDevice(m_gpus[0], &device_info, NULL, &m_device);
//	assert(res == VK_SUCCESS);
//
//	return res;
//}
//
//void Vulkan_Engine::destroy_device()
//{
//	vkDeviceWaitIdle(m_device);
//	vkDestroyDevice(m_device, NULL);
//}
//
///*************************************************************Window Part*******************************************************************************************/
//void Vulkan_Engine::init_window_size(int32_t default_width, int32_t default_height)
//{
//	m_width = default_width;
//	m_height = default_height;
//}
//
//#ifdef _WIN32
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//	
//	switch (uMsg) {
//	case WM_CLOSE:
//		PostQuitMessage(0);
//		break;
//	case WM_PAINT:
//		break;
//	default:
//		break;
//	}
//	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
//}
//
//void Vulkan_Engine::init_window(const char* app_name)
//{
//	WNDCLASSEX win_class;
//	assert(m_width > 0);
//	assert(m_height > 0);
//
//	m_connection = GetModuleHandle(NULL);
//	sprintf(m_name, app_name);
//
//	// Initialize the window class structure:
//	win_class.cbSize = sizeof(WNDCLASSEX);
//	win_class.style = CS_HREDRAW | CS_VREDRAW;
//	win_class.lpfnWndProc = WndProc;
//	win_class.cbClsExtra = 0;
//	win_class.cbWndExtra = 0;
//	win_class.hInstance = m_connection;  // hInstance
//	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
//	win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	win_class.lpszMenuName = NULL;
//	win_class.lpszClassName = m_name;
//	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
//	// Register window class:
//	if (!RegisterClassEx(&win_class)) {
//		// It didn't work, so try to give a useful error:
//#if defined(_WIN32)
//		MessageBox(m_window, "Unexpected error trying to start the application!", "Error", MB_ICONERROR | IDCANCEL);
//#else
//		std::cout << "Unexpected error trying to start the application!" << std::endl;
//#endif
//		exit(1);
//	}
//	// Create window with the registered class:
//	RECT wr = { 0, 0, m_width, m_height };
//	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
//	m_window = CreateWindowEx(0,
//		m_name,             // class name
//		m_name,             // app name
//		WS_OVERLAPPEDWINDOW |  // window style
//		WS_VISIBLE | WS_SYSMENU,
//		100, 100,            // x/y coords
//		wr.right - wr.left,  // width
//		wr.bottom - wr.top,  // height
//		NULL,                // handle to parent
//		NULL,                // handle to menu
//		m_connection,     // hInstance
//		NULL);               // no extra parameters
//	if (!m_window) {
//		// It didn't work, so try to give a useful error:
//#if defined(_WIN32)
//		MessageBox(m_window, "Cannot create a window in which to draw!", "Error", MB_ICONERROR | IDCANCEL);
//#else
//		std::cout << "Cannot create a window in which to draw!\n";
//#endif
//		exit(1);
//	}
//	SetWindowLongPtr(m_window, GWLP_USERDATA, (LONG_PTR)this);
//}
//
//void Vulkan_Engine::destroy_window() 
//{
//	vkDestroySurfaceKHR(m_inst, m_surface, NULL);
//	DestroyWindow(m_window);
//}
//#endif
//
///*************************************************************Swapchain Extension Part*******************************************************************************************/
//void Vulkan_Engine::init_swapchain_extension()
//{
//	VkResult res;
//
//#ifdef _WIN32
//	VkWin32SurfaceCreateInfoKHR createInfo = {};
//	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
//	createInfo.pNext = NULL;
//	createInfo.hinstance = m_connection;
//	createInfo.hwnd = m_window;
//	res = vkCreateWin32SurfaceKHR(m_inst, &createInfo, NULL, &m_surface);
//#endif
//	assert(res == VK_SUCCESS);
//
//	// Iterate over each queue to learn whether it supports presenting:
//	std::vector<VkBool32> pSupportsPresent;
//	pSupportsPresent.resize(m_queue_family_count);
//	for (uint32_t i = 0; i < m_queue_family_count; i++) {
//		vkGetPhysicalDeviceSurfaceSupportKHR(m_gpus[0], i, m_surface, &pSupportsPresent[i]);
//	}
//
//	// Search for a graphics and a present queue in the array of queue
//	// families, try to find one that supports both
//	m_graphics_queue_family_index = UINT32_MAX;
//	m_present_queue_family_index = UINT32_MAX;
//	for (uint32_t i = 0; i < m_queue_family_count; ++i) {
//		if ((m_queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) 
//		{
//			if (m_graphics_queue_family_index == UINT32_MAX)
//			{
//				m_graphics_queue_family_index = i;
//			}
//
//			if (pSupportsPresent[i] == VK_TRUE) 
//			{
//				m_graphics_queue_family_index = i;
//				m_present_queue_family_index = i;
//				break;
//			}
//		}
//	}
//
//	if (m_present_queue_family_index == UINT32_MAX) {
//		// If didn't find a queue that supports both graphics and present, then
//		// find a separate present queue.
//		for (size_t i = 0; i < m_queue_family_count; ++i)
//			if (pSupportsPresent[i] == VK_TRUE) {
//				m_present_queue_family_index = (uint32_t)i;
//				break;
//			}
//	}
//
//	// Generate error if could not find queues that support graphics
//	// and present
//	if (m_graphics_queue_family_index == UINT32_MAX || m_present_queue_family_index == UINT32_MAX) 
//	{
//#if defined(_WIN32)
//		MessageBox(m_window, "Could not find a queues for both graphics and present", "Error", MB_ICONERROR | IDCANCEL);
//#else
//		std::cout << "Could not find a queues for both graphics and present\n";
//#endif
//		exit(-1);
//	}
//
//	// Get the list of VkFormats that are supported
//	uint32_t formatCount;
//	res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_gpus[0], m_surface, &formatCount, NULL);
//	assert(res == VK_SUCCESS);
//	VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
//	res = vkGetPhysicalDeviceSurfaceFormatsKHR(m_gpus[0], m_surface, &formatCount, surfFormats);
//	assert(res == VK_SUCCESS);
//
//	if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) 
//	{
//		m_format = VK_FORMAT_B8G8R8A8_UNORM;
//	}
//	else 
//	{
//		assert(formatCount >= 1);
//		m_format = surfFormats[0].format;
//	}
//	free(surfFormats);
//}
//
///*************************************************************Command Pool Part*******************************************************************************************/
//void Vulkan_Engine::init_command_pool()
//{
//	/* DEPENDS on init_swapchain_extension() */
//	VkResult res;
//
//	VkCommandPoolCreateInfo cmd_pool_info = {};
//	cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//	cmd_pool_info.pNext = NULL;
//	cmd_pool_info.queueFamilyIndex = m_graphics_queue_family_index;
//	cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//
//	res = vkCreateCommandPool(m_device, &cmd_pool_info, NULL, &m_cmd_pool);
//	assert(res == VK_SUCCESS);
//}
//
//void Vulkan_Engine::destroy_command_pool()
//{
//	vkDestroyCommandPool(m_device, m_cmd_pool, NULL);
//}
//
///*************************************************************Command Buffer Part*******************************************************************************************/
//void Vulkan_Engine::init_command_buffer() 
//{
//	/* DEPENDS on init_swapchain_extension() and init_command_pool() */
//	VkResult res;
//
//	VkCommandBufferAllocateInfo cmd = {};
//	cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//	cmd.pNext = NULL;
//	cmd.commandPool = m_cmd_pool;
//	cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//	cmd.commandBufferCount = 1;
//
//	res = vkAllocateCommandBuffers(m_device, &cmd, &m_cmd);
//	assert(res == VK_SUCCESS);
//}
//
//void Vulkan_Engine::destroy_command_buffer()
//{
//	VkCommandBuffer cmd_bufs[1] = { m_cmd };
//	vkFreeCommandBuffers(m_device, m_cmd_pool, 1, cmd_bufs);
//}
//
//void Vulkan_Engine::execute_begin_command_buffer() 
//{
//	/* DEPENDS on init_command_buffer() */
//	VkResult res;
//
//	VkCommandBufferBeginInfo cmd_buf_info = {};
//	cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//	cmd_buf_info.pNext = NULL;
//	cmd_buf_info.flags = 0;
//	cmd_buf_info.pInheritanceInfo = NULL;
//
//	res = vkBeginCommandBuffer(m_cmd, &cmd_buf_info);
//	assert(res == VK_SUCCESS);
//}
//
//void Vulkan_Engine::init_device_queue() {
//	/* DEPENDS on init_swapchain_extension() */
//	vkGetDeviceQueue(m_device, m_graphics_queue_family_index, 0, &m_graphics_queue);
//	if (m_graphics_queue_family_index == m_present_queue_family_index) 
//	{
//		m_present_queue = m_graphics_queue;
//	}
//	else 
//	{
//		vkGetDeviceQueue(m_device, m_present_queue_family_index, 0, &m_present_queue);
//	}
//}
//
///*************************************************************Swapchain Part*******************************************************************************************/
//void Vulkan_Engine::init_swapchain(VkImageUsageFlags usageFlags) {
//	/* DEPENDS on m_cmd and m_queues initialized */
//
//	VkResult res;
//	VkSurfaceCapabilitiesKHR surfCapabilities;
//
//	res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_gpus[0], m_surface, &surfCapabilities);
//	assert(res == VK_SUCCESS);
//
//	uint32_t presentModeCount;
//	res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_gpus[0], m_surface, &presentModeCount, NULL);
//	assert(res == VK_SUCCESS);
//	VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
//	assert(presentModes);
//	res = vkGetPhysicalDeviceSurfacePresentModesKHR(m_gpus[0], m_surface, &presentModeCount, presentModes);
//	assert(res == VK_SUCCESS);
//
//	VkExtent2D swapchainExtent;
//	// width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
//	if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
//		// If the surface size is undefined, the size is set to
//		// the size of the images requested.
//		swapchainExtent.width = m_width;
//		swapchainExtent.height = m_height;
//		if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {
//			swapchainExtent.width = surfCapabilities.minImageExtent.width;
//		}
//		else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) {
//			swapchainExtent.width = surfCapabilities.maxImageExtent.width;
//		}
//
//		if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {
//			swapchainExtent.height = surfCapabilities.minImageExtent.height;
//		}
//		else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) {
//			swapchainExtent.height = surfCapabilities.maxImageExtent.height;
//		}
//	}
//	else {
//		// If the surface size is defined, the swap chain size must match
//		swapchainExtent = surfCapabilities.currentExtent;
//	}
//
//	VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
//	for (int i = 0; i <= (sizeof(presentModes) / sizeof(VkPresentModeKHR)); i++)
//	{
//		// The FIFO present mode is guaranteed by the spec to be supported
//		// Also note that current Android driver only supports FIFO
//		if (presentModes[i] & VK_PRESENT_MODE_FIFO_KHR)
//		{
//			swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
//			break;
//		}
//	}
//	if (!(swapchainPresentMode & VK_PRESENT_MODE_FIFO_KHR))
//	{
//#if defined(_WIN32)
//		MessageBox(m_window, "FIFO mode not supported !", "Error", MB_ICONERROR | IDCANCEL);
//#else
//		std::wcout << "FIFO mode not supported !";
//#endif
//		exit(-1);
//	}
//	// Determine the number of VkImage's to use in the swap chain.
//	// We need to acquire only 1 presentable image at at time.
//	// Asking for minImageCount images ensures that we can acquire
//	// 1 presentable image as long as we present it before attempting
//	// to acquire another.
//	uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;
//
//	VkSurfaceTransformFlagBitsKHR preTransform;
//	if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) 
//	{
//		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
//	}
//	else {
//		preTransform = surfCapabilities.currentTransform;
//	}
//
//	// Find a supported composite alpha mode - one of these is guaranteed to be set
//	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//	VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
//		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
//		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
//		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
//		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
//	};
//	for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); i++) {
//		if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
//			compositeAlpha = compositeAlphaFlags[i];
//			break;
//		}
//	}
//
//	VkSwapchainCreateInfoKHR swapchain_ci = {};
//	swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//	swapchain_ci.pNext = NULL;
//	swapchain_ci.surface = m_surface;
//	swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
//	swapchain_ci.imageFormat = m_format;
//	swapchain_ci.imageExtent.width = swapchainExtent.width;
//	swapchain_ci.imageExtent.height = swapchainExtent.height;
//	swapchain_ci.preTransform = preTransform;
//	swapchain_ci.compositeAlpha = compositeAlpha;
//	swapchain_ci.imageArrayLayers = 1;
//	swapchain_ci.presentMode = swapchainPresentMode;
//	swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
//#if defined(_WIN32)
//	swapchain_ci.clipped = true;
//#endif
//	swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
//	swapchain_ci.imageUsage = usageFlags;
//	swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//	swapchain_ci.queueFamilyIndexCount = 0;
//	swapchain_ci.pQueueFamilyIndices = NULL;
//
//	uint32_t queueFamilyIndices[2] = { (uint32_t)m_graphics_queue_family_index, (uint32_t)m_present_queue_family_index };
//	if (m_graphics_queue_family_index != m_present_queue_family_index) {
//		// If the graphics and present queues are from different queue families,
//		// we either have to explicitly transfer ownership of images between the
//		// queues, or we have to create the swapchain with imageSharingMode
//		// as VK_SHARING_MODE_CONCURRENT
//		swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//		swapchain_ci.queueFamilyIndexCount = 2;
//		swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
//	}
//
//	res = vkCreateSwapchainKHR(m_device, &swapchain_ci, NULL, &m_swapchain);
//	assert(res == VK_SUCCESS);
//
//	res = vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_swapchainImageCount, NULL);
//	assert(res == VK_SUCCESS);
//
//	VkImage *swapchainImages = (VkImage *)malloc(m_swapchainImageCount * sizeof(VkImage));
//	assert(swapchainImages);
//	res = vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_swapchainImageCount, swapchainImages);
//	assert(res == VK_SUCCESS);
//
//	for (uint32_t i = 0; i < m_swapchainImageCount; i++) 
//	{
//		swapchain_buffer sc_buffer;
//
//		VkImageViewCreateInfo color_image_view = {};
//		color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//		color_image_view.pNext = NULL;
//		color_image_view.format = m_format;
//		color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
//		color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
//		color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
//		color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;
//		color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//		color_image_view.subresourceRange.baseMipLevel = 0;
//		color_image_view.subresourceRange.levelCount = 1;
//		color_image_view.subresourceRange.baseArrayLayer = 0;
//		color_image_view.subresourceRange.layerCount = 1;
//		color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
//		color_image_view.flags = 0;
//
//		sc_buffer.image = swapchainImages[i];
//
//		color_image_view.image = sc_buffer.image;
//
//		res = vkCreateImageView(m_device, &color_image_view, NULL, &sc_buffer.view);
//		m_buffers.push_back(sc_buffer);
//		assert(res == VK_SUCCESS);
//	}
//	free(swapchainImages);
//	m_current_buffer = 0;
//
//	/*if (NULL != presentModes) 
//	{
//		free(presentModes);
//	}*/
//}
//
//void Vulkan_Engine::destroy_swapchain()
//{
//	for (uint32_t i = 0; i < m_swapchainImageCount; i++) {
//		vkDestroyImageView(m_device, m_buffers[i].view, NULL);
//	}
//	vkDestroySwapchainKHR(m_device, m_swapchain, NULL);
//}
//
///**********************************************************Depth Buffer Part*****************************************************************************************/
//void Vulkan_Engine::init_depth_buffer() 
//{
//	VkResult res;
//	bool pass;
//	VkImageCreateInfo image_info = {};
//
//	/* allow custom depth formats */
//#if defined(_WIN32)
//	if (m_depth.format == VK_FORMAT_UNDEFINED)
//	{
//		m_depth.format = VK_FORMAT_D16_UNORM;
//	}
//#endif
//
//	const VkFormat depth_format = m_depth.format;
//	VkFormatProperties props;
//	vkGetPhysicalDeviceFormatProperties(m_gpus[0], depth_format, &props);
//	if (props.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) 
//	{
//		image_info.tiling = VK_IMAGE_TILING_LINEAR;
//	}
//	else if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) 
//	{
//		image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
//	}
//	else 
//	{
//		/* Try other depth formats? */
//		std::cout << "depth_format " << depth_format << " Unsupported.\n";
//		exit(-1);
//	}
//
//	image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//	image_info.pNext = NULL;
//	image_info.imageType = VK_IMAGE_TYPE_2D;
//	image_info.format = depth_format;
//	image_info.extent.width = m_width;
//	image_info.extent.height = m_height;
//	image_info.extent.depth = 1;
//	image_info.mipLevels = 1;
//	image_info.arrayLayers = 1;
//	image_info.samples = VK_SAMPLE_COUNT_1_BIT;
//	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//	image_info.queueFamilyIndexCount = 0;
//	image_info.pQueueFamilyIndices = NULL;
//	image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//	image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
//	image_info.flags = 0;
//
//	VkMemoryAllocateInfo mem_alloc = {};
//	mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//	mem_alloc.pNext = NULL;
//	mem_alloc.allocationSize = 0;
//	mem_alloc.memoryTypeIndex = 0;
//
//	VkImageViewCreateInfo view_info = {};
//	view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//	view_info.pNext = NULL;
//	view_info.image = VK_NULL_HANDLE;
//	view_info.format = depth_format;
//	view_info.components.r = VK_COMPONENT_SWIZZLE_R;
//	view_info.components.g = VK_COMPONENT_SWIZZLE_G;
//	view_info.components.b = VK_COMPONENT_SWIZZLE_B;
//	view_info.components.a = VK_COMPONENT_SWIZZLE_A;
//	view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
//	view_info.subresourceRange.baseMipLevel = 0;
//	view_info.subresourceRange.levelCount = 1;
//	view_info.subresourceRange.baseArrayLayer = 0;
//	view_info.subresourceRange.layerCount = 1;
//	view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
//	view_info.flags = 0;
//
//	if (depth_format == VK_FORMAT_D16_UNORM_S8_UINT || depth_format == VK_FORMAT_D24_UNORM_S8_UINT ||
//		depth_format == VK_FORMAT_D32_SFLOAT_S8_UINT) 
//	{
//		view_info.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
//	}
//
//	VkMemoryRequirements mem_reqs;
//
//	/* Create image */
//	res = vkCreateImage(m_device, &image_info, NULL, &m_depth.image);
//	assert(res == VK_SUCCESS);
//
//	vkGetImageMemoryRequirements(m_device, m_depth.image, &mem_reqs);
//
//	mem_alloc.allocationSize = mem_reqs.size;
//	/* Use the memory properties to determine the type of memory required */
//	pass =
//		memory_type_from_properties(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mem_alloc.memoryTypeIndex);
//	assert(pass);
//
//	/* Allocate memory */
//	res = vkAllocateMemory(m_device, &mem_alloc, NULL, &m_depth.mem);
//	assert(res == VK_SUCCESS);
//
//	/* Bind memory */
//	res = vkBindImageMemory(m_device, m_depth.image, m_depth.mem, 0);
//	assert(res == VK_SUCCESS);
//
//	/* Create image view */
//	view_info.image = m_depth.image;
//	res = vkCreateImageView(m_device, &view_info, NULL, &m_depth.view);
//	assert(res == VK_SUCCESS);
//}
//
//bool Vulkan_Engine::memory_type_from_properties(uint32_t typeBits, VkFlags requirements_mask, uint32_t *typeIndex) 
//{
//	// Search memtypes to find first index with those properties
//	for (uint32_t i = 0; i < m_memory_props.memoryTypeCount; i++) 
//	{
//		if ((typeBits & 1) == 1) 
//		{
//			// Type is available, does it match user properties?
//			if ((m_memory_props.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) 
//			{
//				*typeIndex = i;
//				return true;
//			}
//		}
//		typeBits >>= 1;
//	}
//	// No memory types matched, return failure
//	return false;
//}
//
//void Vulkan_Engine::destroy_depth_buffer() 
//{
//	vkDestroyImageView(m_device, m_depth.view, NULL);
//	vkDestroyImage(m_device, m_depth.image, NULL);
//	vkFreeMemory(m_device, m_depth.mem, NULL);
//}
//
///**********************************************************Uniform Buffer Part*****************************************************************************************/
//void Vulkan_Engine::init_uniform_buffer()
//{
//	VkResult res;
//	bool pass;
//	float fov = glm::radians(45.0f);
//	if (m_width > m_height) {
//		fov *= static_cast<float>(m_height) / static_cast<float>(m_width);
//	}
//	m_Projection = glm::perspective(fov, static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 100.0f);
//	m_View = glm::lookAt(
//		glm::vec3(-2, 0, 0),  // Camera position in the space
//		glm::vec3(0, 0,	0),     // camera looking at
//		glm::vec3(0, 0, 0)     // camera tilt
//	);
//	m_Model = glm::mat4(1.0f);
//	// Vulkan clip space has inverted Y and half Z.
//	m_Clip = glm::mat4(
//		1.0f, 0.0f, 0.0f, 0.0f, 
//		0.0f, -1.0f, 0.0f, 0.0f, 
//		0.0f, 0.0f, 0.5f, 0.0f, 
//		0.0f, 0.0f, 0.5f, 1.0f);
//
//	m_MVP = m_Clip * m_Projection * m_View * m_Model;
//
//	VkBufferCreateInfo buf_info = {};
//	buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//	buf_info.pNext = NULL;
//	buf_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
//	buf_info.size = sizeof(m_MVP);
//	buf_info.queueFamilyIndexCount = 0;
//	buf_info.pQueueFamilyIndices = NULL;
//	buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//	buf_info.flags = 0;
//	res = vkCreateBuffer(m_device, &buf_info, NULL, &m_uniform_data.buf);
//	assert(res == VK_SUCCESS);
//
//	VkMemoryRequirements mem_reqs;
//	vkGetBufferMemoryRequirements(m_device, m_uniform_data.buf, &mem_reqs);
//
//	VkMemoryAllocateInfo alloc_info = {};
//	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//	alloc_info.pNext = NULL;
//	alloc_info.memoryTypeIndex = 0;
//
//	alloc_info.allocationSize = mem_reqs.size;
//	pass = memory_type_from_properties(mem_reqs.memoryTypeBits,
//		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//		&alloc_info.memoryTypeIndex);
//	assert(pass && "No mappable, coherent memory");
//
//	res = vkAllocateMemory(m_device, &alloc_info, NULL, &(m_uniform_data.mem));
//	assert(res == VK_SUCCESS);
//
//	uint8_t *pData;
//	res = vkMapMemory(m_device, m_uniform_data.mem, 0, mem_reqs.size, 0, (void **)&pData);
//	assert(res == VK_SUCCESS);
//
//	memcpy(pData, &m_MVP, sizeof(m_MVP));
//
//	vkUnmapMemory(m_device, m_uniform_data.mem);
//
//	res = vkBindBufferMemory(m_device, m_uniform_data.buf, m_uniform_data.mem, 0);
//	assert(res == VK_SUCCESS);
//
//	m_uniform_data.buffer_info.buffer = m_uniform_data.buf;
//	m_uniform_data.buffer_info.offset = 0;
//	m_uniform_data.buffer_info.range = sizeof(m_MVP);
//}
//
//void Vulkan_Engine::destroy_uniform_buffer()
//{
//	vkDestroyBuffer(m_device, m_uniform_data.buf, NULL);
//	vkFreeMemory(m_device, m_uniform_data.mem, NULL);
//}