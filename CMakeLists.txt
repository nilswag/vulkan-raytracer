cmake_minimum_required(VERSION 4.2)
project(vulkan-raytracer)

# ------------------------------
# language standard
# ------------------------------
set(CMAKE_CXX_STANDARD 			23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# ------------------------------
# dependencies
# ------------------------------
find_package(Vulkan REQUIRED)
add_subdirectory(third_party/glfw)

# ------------------------------
# sources
# ------------------------------
set(SOURCES
	src/main.cpp
	src/vulkan/vulkan_app.cpp
)

set(HEADERS
	src/util/log.h
	src/vulkan/vulkan_app.h
)

# ------------------------------
# target
# ------------------------------
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME}
	PRIVATE src
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE glfw
	PRIVATE Vulkan::Vulkan
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE $<$<CONFIG:Debug>:_DEBUG>
)