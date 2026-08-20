cmake_minimum_required(VERSION 3.21)

project(DX12Study LANGUAGES CXX RC)

if(NOT WIN32)
    message(FATAL_ERROR "DX12Study is a Windows-only project.")
endif()

set(DX12STUDY_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Output")

# Engine is a static library.  Link this target from Client instead of using
# target_link_directories(): CMake then chooses the matching Debug/Release lib.
add_library(Engine STATIC
    Engine/EnginePch.cpp
    Engine/pch.cpp
    Engine/EnginePch.h
    Engine/pch.h
    Engine/d3dx12.h
)

target_compile_features(Engine PUBLIC cxx_std_20)
target_precompile_headers(Engine PRIVATE Engine/pch.h)
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Engine")
target_link_libraries(Engine PUBLIC d3d12 dxgi dxguid d3dcompiler)
set_target_properties(Engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${DX12STUDY_OUTPUT_DIR}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${DX12STUDY_OUTPUT_DIR}/$<CONFIG>"
)

add_executable(Client WIN32
    Client/Client.cpp
    Client/Game.cpp
    Client/pch.cpp
    Client/Client.rc
    Client/Client.h
    Client/framework.h
    Client/pch.h
    Client/Resource.h
    Client/targetver.h
    Client/Client.ico
    Client/small.ico
)

target_compile_features(Client PRIVATE cxx_std_20)
target_compile_definitions(Client PRIVATE UNICODE _UNICODE)
target_precompile_headers(Client PRIVATE Client/pch.h)
target_link_libraries(Client PRIVATE Engine)
set_target_properties(Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DX12STUDY_OUTPUT_DIR}/$<CONFIG>"
)

if(MSVC)
    target_compile_options(Engine PRIVATE /W3 /permissive-)
    target_compile_options(Client PRIVATE /W3 /permissive-)
endif()
