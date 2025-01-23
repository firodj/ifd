cmake_minimum_required(VERSION 3.20)
project(ifd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ifd_BUILD_EXAMPLES "Enable building examples." ON)

find_package(OpenGL REQUIRED)
include(FetchContent)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.6-docking
    GIT_SHALLOW TRUE         # Equivalent to --depth=1
)
FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.3
    GIT_SHALLOW TRUE         # Equivalent to --depth=1
)
FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.6
    GIT_SHALLOW TRUE         # Equivalent to --depth=1
	SOURCE_SUBDIR cmake
)

if (NOT TARGET glad_gl_core_33)
	FetchContent_MakeAvailable(glad2)
	#set(GLAD_SOURCES_DIR "${glad2_SOURCE_DIR}")
	#add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
	glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)
endif()

if (NOT TARGET imgui)
	FetchContent_MakeAvailable(imgui)

	set(IMGUI_DIR "${imgui_SOURCE_DIR}")

	include_directories(SYSTEM
		${IMGUI_DIR}
	)

	set(imgui_SOURCES
		${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_demo.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
	)

	add_library(imgui
		${imgui_SOURCES}
	)

	if (BUILD_SHARED_LIBS)
		target_compile_definitions(imgui INTERFACE imgui_IMPORTS)
	endif()
endif()

# ImFileDialog (ifd)
list(APPEND ifd_SOURCES
	src/ImFileDialog.cpp
	src/ImFileDialog.hpp
	src/ImFileDialogIconInfo.cpp
	src/ImFileDialogIconInfo.hpp
	src/DirectoryIterator.hpp
	src/DirectoryIterator.cpp
)

if (APPLE)
	list(APPEND ifd_SOURCES
		src/ImFileDialog_osx.mm
		src/ImFileDialog_osx.hpp
	)
endif()
if (WIN32)
	list(APPEND ifd_SOURCES
		src/ImFileDialog_win32.cpp
		src/ImFileDialog_win32.hpp
	)
endif()

add_library(ifd ${ifd_SOURCES})

if (APPLE)
	# Cocoa
	find_library(COCOA_LIBRARY Cocoa)
	find_library(APPKIT_LIB AppKit)
	target_link_libraries(ifd PUBLIC ${COCOA_LIBRARY} ${APPKIT_LIB})
endif()

target_link_libraries(ifd PUBLIC imgui)
target_include_directories(ifd PUBLIC
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/example/lib/stb
)
if (ifd_BUILD_EXAMPLES)
	set(SDL_TEST OFF)
	set(SDL2_DISABLE_SDL2MAIN ON)
	FetchContent_MakeAvailable(sdl2)

	file(REAL_PATH "${IMGUI_DIR}"
		IMGUI_DIR_ABS
		BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

	message("IMGUI_DIR_ABS = ${IMGUI_DIR_ABS}")

	add_executable(ImFileDialogExample
		example/ImFileDialogExample.cpp
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
		${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
	)
	target_include_directories(ImFileDialogExample PRIVATE
		${OPENGL_INCLUDE_DIR}
		${SDL2_SOURCE_DIR}/include
	)
	target_compile_definitions(ImFileDialogExample PRIVATE
		IMGUI_DIR="${IMGUI_DIR_ABS}"
		IMGUI_USER_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/example/imconfig.h"
	)
	target_link_libraries(ImFileDialogExample
		${OPENGL_LIBRARIES}
		SDL2-static
		glad_gl_core_33
		imgui
		ifd
	)
endif()