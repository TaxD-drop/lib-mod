cmake_minimum_required(VERSION 3.14)
project(libmod VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opção para build Android (ativada via script ou linha de comando)
option(ANDROID_BUILD "Cross-compile for Android" OFF)

if(ANDROID_BUILD)
    message(STATUS "Configurando para Android (${ANDROID_ABI})")
    add_library(mod SHARED
        src/main.cpp
        src/modmenu.cpp
        src/hooking.cpp
    )
else()
    # Build para PC (Linux/Windows/Mac) – apenas teste
    add_executable(loader
        src/main.cpp
        src/modmenu.cpp
        src/hooking.cpp
    )
    target_compile_definitions(loader PRIVATE DESKTOP_TEST)
endif()

target_include_directories(mod PRIVATE include external/imgui external/dobby)

# Link ImGui (você precisa ter os fontes em external/imgui)
file(GLOB IMGUI_SOURCES external/imgui/*.cpp)
target_sources(mod PRIVATE ${IMGUI_SOURCES})
target_include_directories(mod PRIVATE external/imgui/backends)

# Link Dobby (já compilado ou como source)
if(ANDROID_BUILD)
    find_library(log-lib log)
    target_link_libraries(mod ${log-lib})
    # Adicione Dobby compilado para Android (pré-compilado ou fonte)
    # Exemplo: target_link_libraries(mod ${CMAKE_SOURCE_DIR}/external/dobby/libdobby.a)
endif()

# Instalação da .so no diretório de saída
if(ANDROID_BUILD)
    install(TARGETS mod LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/app/src/main/jniLibs/${ANDROID_ABI})
endif()