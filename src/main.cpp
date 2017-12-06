#include <vulkan\vulkan.h>
#include <iostream>
#include <cassert>
#include <conio.h>
#include <vector>

#define APP_NAME "LycanthEngine"
#define ENGINE_NAME "LycanthEngine";


typedef struct 
{
	VkLayerProperties properties;
	std::vector<VkExtensionProperties> extensions;
} layer_properties;

struct lycanthEngine_info 
{
#ifdef _WIN32
#define APP_NAME_STR_LEN 80
	HINSTANCE connection;        // hInstance - Windows Instance
	char name[APP_NAME_STR_LEN]; // Name to put on the window/icon
	HWND window;                 // hWnd - window handle
#endif
	VkSurfaceKHR surface;
	bool prepared;
	bool use_staging_buffer;
	bool save_images;

	std::vector<layer_properties> instance_layer_properties;
	std::vector<const char *> instance_extension_names;
	std::vector<const char *> instance_layer_names;

	VkInstance inst;

	std::vector<const char *> device_extension_names;
	std::vector<VkPhysicalDevice> gpus;
	VkDevice device;
	VkQueue graphics_queue;
	uint32_t graphics_queue_family_index;

	VkPhysicalDeviceProperties gpu_props;
	std::vector<VkQueueFamilyProperties> queue_props;
	VkPhysicalDeviceMemoryProperties memory_properties;

	int width, height;

	VkCommandPool cmd_pool;
	VkCommandBuffer cmd;

	uint32_t queue_family_count;
};


VkResult init_global_extension_properties(layer_properties &layer_props) 
{
	VkExtensionProperties *instance_extensions;
	uint32_t instance_extension_count;
	VkResult res;
	char *layer_name = NULL;

	layer_name = layer_props.properties.layerName;

	do {
		res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, NULL);
		if (res) return res;

		if (instance_extension_count == 0) {
			return VK_SUCCESS;
		}

		layer_props.extensions.resize(instance_extension_count);
		instance_extensions = layer_props.extensions.data();
		res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, instance_extensions);
	} while (res == VK_INCOMPLETE);

	return res;
}

VkResult init_global_layer_properties(struct lycanthEngine_info &info) 
{
	uint32_t instance_layer_count;
	VkLayerProperties *vk_props = NULL;
	VkResult res;

	do {
		res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
		if (res) return res;

		if (instance_layer_count == 0) {
			return VK_SUCCESS;
		}

		vk_props = (VkLayerProperties *)realloc(vk_props, instance_layer_count * sizeof(VkLayerProperties));

		res = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_props);
	} while (res == VK_INCOMPLETE);

	/*
	* Now gather the extension list for each instance layer.
	*/
	for (uint32_t i = 0; i < instance_layer_count; i++) {
		layer_properties layer_props;
		layer_props.properties = vk_props[i];
		res = init_global_extension_properties(layer_props);
		if (res) return res;
		info.instance_layer_properties.push_back(layer_props);
	}
	free(vk_props);

	return res;
}
void init_instance_extension_names(struct lycanthEngine_info &info) 
{
	info.instance_extension_names.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef _WIN32
	info.instance_extension_names.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
}

void init_device_extension_names(struct lycanthEngine_info &info) {
	info.device_extension_names.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VkResult init_instance(struct lycanthEngine_info &info, char const *const app_short_name) {
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
	inst_info.enabledLayerCount = info.instance_layer_names.size();
	inst_info.ppEnabledLayerNames = info.instance_layer_names.size() ? info.instance_layer_names.data() : NULL;
	inst_info.enabledExtensionCount = info.instance_extension_names.size();
	inst_info.ppEnabledExtensionNames = info.instance_extension_names.data();

	VkResult res = vkCreateInstance(&inst_info, NULL, &info.inst);
	assert(res == VK_SUCCESS);

	return res;
}

VkResult init_enumerate_device(struct lycanthEngine_info &info, uint32_t gpu_count = 1) {
	uint32_t const req_count = gpu_count;
	VkResult res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, NULL);
	assert(gpu_count);
	info.gpus.resize(gpu_count);

	res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, info.gpus.data());
	assert(!res && gpu_count >= req_count);

	vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, NULL);
	assert(info.queue_family_count >= 1);

	info.queue_props.resize(info.queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(info.gpus[0], &info.queue_family_count, info.queue_props.data());
	assert(info.queue_family_count >= 1);

	/* This is as good a place as any to do this */
	vkGetPhysicalDeviceMemoryProperties(info.gpus[0], &info.memory_properties);
	vkGetPhysicalDeviceProperties(info.gpus[0], &info.gpu_props);

	return res;
}

void init_window_size(struct lycanthEngine_info &info, int32_t default_width, int32_t default_height)
{
#ifdef _WIN32
	info.width = default_width;
	info.height = default_height;
#endif
}

#ifdef _WIN32
static void run(struct lycanthEngine_info *info) 
{ /* Placeholder for samples that want to show dynamic content */ }

// MS-Windows event handling function:
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	struct lycanthEngine_info *info = reinterpret_cast<struct lycanthEngine_info *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (uMsg) 
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		run(info);
		return 0;
	default:
		break;
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void init_window(struct lycanthEngine_info &info) 
{
	WNDCLASSEX win_class;
	assert(info.width > 0);
	assert(info.height > 0);

	info.connection = GetModuleHandle(NULL);
	sprintf(info.name, "Sample");

	// Initialize the window class structure:
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WndProc;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = info.connection;  // hInstance
	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = info.name;
	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	// Register window class:
	if (!RegisterClassEx(&win_class)) {
		// It didn't work, so try to give a useful error:
		printf("Unexpected error trying to start the application!\n");
		fflush(stdout);
		exit(1);
	}
	// Create window with the registered class:
	RECT wr = { 0, 0, info.width, info.height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
	info.window = CreateWindowEx(0,
		info.name,             // class name
		info.name,             // app name
		WS_OVERLAPPEDWINDOW |  // window style
		WS_VISIBLE | WS_SYSMENU,
		100, 100,            // x/y coords
		wr.right - wr.left,  // width
		wr.bottom - wr.top,  // height
		NULL,                // handle to parent
		NULL,                // handle to menu
		info.connection,     // hInstance
		NULL);               // no extra parameters
	if (!info.window) {
		// It didn't work, so try to give a useful error:
		printf("Cannot create a window in which to draw!\n");
		fflush(stdout);
		exit(1);
	}
	SetWindowLongPtr(info.window, GWLP_USERDATA, (LONG_PTR)&info);
}

void destroy_window(struct lycanthEngine_info &info) 
{
	vkDestroySurfaceKHR(info.inst, info.surface, NULL);
	DestroyWindow(info.window);
}
#endif

VkResult init_device(struct lycanthEngine_info &info) {
	VkResult res;
	VkDeviceQueueCreateInfo queue_info = {};

	float queue_priorities[1] = { 0.0 };
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = NULL;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = queue_priorities;
	queue_info.queueFamilyIndex = info.graphics_queue_family_index;

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = NULL;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.enabledExtensionCount = info.device_extension_names.size();
	device_info.ppEnabledExtensionNames = device_info.enabledExtensionCount ? info.device_extension_names.data() : NULL;
	device_info.pEnabledFeatures = NULL;

	res = vkCreateDevice(info.gpus[0], &device_info, NULL, &info.device);
	assert(res == VK_SUCCESS);

	return res;
}

void destroy_device(struct lycanthEngine_info &info) {
	vkDeviceWaitIdle(info.device);
	vkDestroyDevice(info.device, NULL);
}

void destroy_instance(struct lycanthEngine_info &info) 
{ 
	vkDestroyInstance(info.inst, NULL); 
}

int main()
{
	VkResult res;
	struct lycanthEngine_info info = {};
	char lycanthEngine_title[] = "LycanthEngine";
	init_global_layer_properties(info);
	init_instance_extension_names(info);
	init_device_extension_names(info);
	init_instance(info, lycanthEngine_title);
	init_enumerate_device(info);
	init_window_size(info, 1000, 1000);
	init_window(info);
	init_device(info);

	_getch();
	destroy_device(info);
	destroy_window(info);
	destroy_instance(info);
	return 0;
}
