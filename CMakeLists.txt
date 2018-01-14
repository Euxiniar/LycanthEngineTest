cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(VERSION 3.1)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME LycanthEngine)
set(DEPS_PATH "${CMAKE_SOURCE_DIR}/deps")
set(LIBS_PATH "${CMAKE_SOURCE_DIR}/libs")
project(${NAME})

if(WIN32)
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES")

	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -W4")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4201")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4201")
	
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
	 
include_directories("${DEPS_PATH}/vulkan")
link_directories(${LIBS_PATH})



if(WIN32)
add_executable(${PROJECT_NAME} ${SRC})
else()
add_executable(${PROJECT_NAME} ${SRC})
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${DEPS_PATH}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} vulkan-1)

