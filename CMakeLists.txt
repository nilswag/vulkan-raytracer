cmake_minimum_required(VERSION 4.2)
project(vulkan-leren)

find_package(Vulkan REQUIRED)

add_subdirectory(third_party/glfw)

file(GLOB_RECURSE SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
	PRIVATE src
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE glfw
	PRIVATE Vulkan::Vulkan
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE $<$<CONFIG:RelWithDebInfo>:_DEBUG>
	PRIVATE $<$<CONFIG:Debug>:_DEBUG>
)