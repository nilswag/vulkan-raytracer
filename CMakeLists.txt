cmake_minimum_required(VERSION 4.2)
project(vulkan-raytracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(Vulkan REQUIRED)

add_subdirectory(third_party/glfw)

file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
	PRIVATE src
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE glfw
	PRIVATE Vulkan::Vulkan
)

target_compile_definitions(${PROJECT_NAME}
	# PRIVATE $<$<CONFIG:RelWithDebInfo>:_DEBUG>
	PRIVATE $<$<CONFIG:Debug>:_DEBUG>
)