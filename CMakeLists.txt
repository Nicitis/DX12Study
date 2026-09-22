cmake_minimum_required(VERSION 3.21)

project(DX12Study LANGUAGES CXX RC)

if(NOT WIN32)
    message(FATAL_ERROR "DX12Study is a Windows-only project.")
endif()

set(DX12STUDY_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/Output")

# Engine is a static library.  Client links the target itself (not a bare file
# name), so CMake always picks the lib matching the configuration being built.
add_library(Engine STATIC
    Engine/CommandQueue.cpp
    Engine/ConstantBuffer.cpp
    Engine/Device.cpp
    Engine/Engine.cpp
    Engine/EnginePch.cpp
    Engine/Mesh.cpp
    Engine/pch.cpp
    Engine/RootSignature.cpp
    Engine/Shader.cpp
    Engine/SwapChain.cpp
    Engine/CommandQueue.h
    Engine/ConstantBuffer.h
    Engine/d3dx12.h
    Engine/Device.h
    Engine/Engine.h
    Engine/EnginePch.h
    Engine/Mesh.h
    Engine/pch.h
    Engine/RootSignature.h
    Engine/Shader.h
    Engine/SwapChain.h
)

target_compile_features(Engine PUBLIC cxx_std_20)
# EnginePch.h does `using namespace std;` before pulling in the Windows headers,
# which makes `byte` ambiguous between std::byte and rpcndr.h's typedef.
target_compile_definitions(Engine PUBLIC UNICODE _UNICODE _HAS_STD_BYTE=0)
target_precompile_headers(Engine PRIVATE Engine/pch.h)
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Engine")
target_link_libraries(Engine PUBLIC d3d12 dxgi dxguid d3dcompiler)

add_executable(Client WIN32
    Client/Client.cpp
    Client/Game.cpp
    Client/pch.cpp
    Client/Client.rc
    Client/Client.h
    Client/framework.h
    Client/Game.h
    Client/pch.h
    Client/Resource.h
    Client/targetver.h
    Client/Client.ico
    Client/small.ico
    Resources/Shader/default.hlsli
)

# Icons are pulled in by Client.rc, and the shader is compiled at runtime by
# D3DCompileFromFile: list them so they show up in the IDE, but never build them.
set_source_files_properties(
    Client/Client.ico
    Client/small.ico
    Resources/Shader/default.hlsli
    PROPERTIES HEADER_FILE_ONLY TRUE
)

target_compile_features(Client PRIVATE cxx_std_20)
target_precompile_headers(Client PRIVATE Client/pch.h)
target_link_libraries(Client PRIVATE Engine)

# Client/pch.h carries `#pragma comment(lib, "Engine.lib")`, so the linker also
# resolves Engine.lib by name and needs its directory on the library path.
target_link_directories(Client PRIVATE "$<TARGET_FILE_DIR:Engine>")

# Game.cpp loads the shader as "..\Resources\Shader\default.hlsli", i.e. relative
# to the working directory.  Keep the binaries directly in Output/ (no per-config
# subfolder) and debug from there so that ".." resolves to the project root.
set_target_properties(Engine PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${DX12STUDY_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${DX12STUDY_OUTPUT_DIR}"
)
set_target_properties(Client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DX12STUDY_OUTPUT_DIR}"
    VS_DEBUGGER_WORKING_DIRECTORY "${DX12STUDY_OUTPUT_DIR}"
)
foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${config}" config_upper)
    set_target_properties(Engine PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${config_upper} "${DX12STUDY_OUTPUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY_${config_upper} "${DX12STUDY_OUTPUT_DIR}"
    )
    set_target_properties(Client PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${config_upper} "${DX12STUDY_OUTPUT_DIR}"
    )
endforeach()

if(MSVC)
    target_compile_options(Engine PRIVATE /W3 /permissive- /MP)
    target_compile_options(Client PRIVATE /W3 /permissive- /MP)

    # F5 in a generated .sln should start Client, not ALL_BUILD.
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT Client)
endif()

# Mirror the on-disk layout in the Solution Explorer.
get_target_property(ENGINE_SOURCES Engine SOURCES)
get_target_property(CLIENT_SOURCES Client SOURCES)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ENGINE_SOURCES} ${CLIENT_SOURCES})
