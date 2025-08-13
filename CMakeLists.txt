cmake_minimum_required(VERSION 3.20)
project(ChattingOn VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find vcpkg packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(portaudio CONFIG REQUIRED)
find_package(Opus CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

# Include directories (vcpkg handles most)
include_directories(src/common)

# Common library 
add_library(common STATIC
	src/common/audio_utils.cpp
	src/common/network_utils.cpp
	src/common/control_message.cpp
	#### ADD MORE
)

target_link_libraries(common PUBLIC
	Boost::system
	portaudio
	Opus::opus
	spdlog::spdlog
	fmt::fmt
	nlohmann_json::nlohmann_json
)

# Server executable
add_executable(server
	src/server/main.cpp
	src/server/server.cpp
	src/server/tcp_server.cpp
	src/server/udp_server.cpp
	src/server/room_manager.cpp
)
target_link_libraries(server PRIVATE common)

# Client executable
add_executable(client
	src/client/main.cpp
	src/client/client.cpp
	src/client/tcp_client.cpp
	src/client/udp_client.cpp
)
target_link_libraries(client PRIVATE common)

# Tests
enable_testing()
add_subdirectory(tests)
