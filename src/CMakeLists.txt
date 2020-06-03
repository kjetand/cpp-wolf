add_library(wolf3d STATIC
    crt.cpp
    fmopl.cpp
    id_ca.cpp
    id_in.cpp
    id_pm.cpp
    id_sd.cpp
    id_us_1.cpp
    id_vh.cpp
    id_vl.cpp
    signon.cpp
    wl_act1.cpp
    wl_act2.cpp
    wl_agent.cpp
    wl_debug.cpp
    wl_draw.cpp
    wl_game.cpp
    wl_inter.cpp
    wl_main.cpp
    wl_menu.cpp
    wl_play.cpp
    wl_state.cpp
    wl_text.cpp)

target_include_directories(wolf3d PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL_INCLUDE_DIR}
    ${SDLMIXER_INCLUDE_DIR})

target_link_libraries(wolf3d PUBLIC
    ${OPENGL_LIBRARIES}
    ${SDL_LIBRARY}
    ${SDLMIXER_LIBRARY})

add_executable(cpp-wolf main.cpp)
target_include_directories(cpp-wolf PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cpp-wolf PRIVATE wolf3d)
