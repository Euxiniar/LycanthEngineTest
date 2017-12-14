cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME LycanthEngine)
set(LIBS_PATH "${CMAKE_SOURCE_DIR}/libs")
set(INCLUDES_PATH "${CMAKE_SOURCE_DIR}/includes")

project(${NAME})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR -DWIN32_LEAN_AND_MEAN)
	message("Vulkan Api definition OK")
endif()

if(WIN32)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")

	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -W4")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4267")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267")
	
	elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wall -Wundef -Wno-unused-parameter -Wno-missing-field-initializers")

	endif()
else()
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")

endif()

message("Compiler is " ${CMAKE_CXX_COMPILER_ID})

file(GLOB_RECURSE SRC
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
)

if(EXISTS ${LIBS_PATH})
	link_libraries("${LIBS_PATH}/vulkan-1.lib")
	message("Libs linked")
else()
	message("Libs not linked !")
endif()

if(EXISTS ${INCLUDES_PATH})
	include_directories("${INCLUDES_PATH}")
	message("Includes included")
else()
	message("Includes not included !")
endif()

add_executable(${PROJECT_NAME} ${SRC})
add_library(LycanthEngine-1 STATIC src/Engine/Vulkan_Engine.cpp)

if (NOT UNIX)
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    CXX_STANDARD 14
	    CXX_STANDARD_REQUIRED ON
	    CXX_EXTENSIONS OFF
	)
else()
	add_definitions(-std=c++14)
endif()
