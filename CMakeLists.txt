cmake_minimum_required(VERSION 3.20)
project(ifd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ifd_BUILD_EXAMPLES "Enable building examples." ON)

find_package(OpenGL REQUIRED)
set(SDL_TEST OFF)
set(SDL2_DISABLE_SDL2MAIN ON)
set(SDL2_DIR "example/lib/sdl2")
add_subdirectory("${SDL2_DIR}")

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/example/lib/glad2")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

include_directories(SYSTEM
	${OPENGL_INCLUDE_DIR}
	${SDL2_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/example/lib/stb
	${PROJECT_SOURCE_DIR}/src
)

if (NOT TARGET imgui)
	set(IMGUI_DIR "example/lib/imgui-docking")

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

if (ifd_BUILD_EXAMPLES)
	file(REAL_PATH "${IMGUI_DIR}"
		IMGUI_DIR_ABS
		BASE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

	message("IMGUI_DIR_ABS = ${IMGUI_DIR_ABS}")

	add_executable(demo
		example/demo.cpp
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
		${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
	)
	target_compile_definitions(demo PRIVATE
		IMGUI_DIR="${IMGUI_DIR_ABS}"
		IMGUI_USER_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/example/imconfig.h"
	)
	target_link_libraries(demo
		${OPENGL_LIBRARIES}
		SDL2-static
		glad_gl_core_33
		imgui
		ifd
	)
endif()