#include <conio.h>
#include "Engine\Vulkan_Engine.h"

#define _DEBUG_MODE true

#if defined(_WIN32)
int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, INT)
{
#if _DEBUG_MODE
	AllocConsole();
	freopen("CONOUT$", "w+", stdout);
#endif
	Vulkan_Engine engine("LycanthEngine");
	_getch();
	return 0;
}
#endif
