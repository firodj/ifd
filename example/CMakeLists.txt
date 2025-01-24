
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