cmake_minimum_required(VERSION 3.21)

project(DX12Study LANGUAGES CXX RC)

if(NOT WIN32)
    message(FATAL_ERROR "DX12Study is a Windows-only project.")
endif()

add_executable(Client WIN32
    Client/Client.cpp
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

if(MSVC)
    target_compile_options(Client PRIVATE /W3 /permissive-)
endif()

# DX12 코드를 추가하면 즉시 사용할 Windows SDK 라이브러리.
target_link_libraries(Client PRIVATE d3d12 dxgi dxguid)
