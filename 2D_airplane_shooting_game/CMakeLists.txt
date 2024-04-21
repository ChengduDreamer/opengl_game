cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
project(game VERSION 1.0.0.1)
set(Version 1.0.0.1)
include(InstallRequiredSystemLibraries)
add_definitions("-DUNICODE" "-D_UNICODE" "-DWIN32_LEAN_AND_MEAN")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/element)
include_directories($ENV{GLFW_PATH}/include)
include_directories($ENV{STD_IMAGE_PATH}/include)
include_directories($ENV{GLM_PATH})

link_directories($ENV{GLFW_PATH}/lib)

message("CMAKE_TOOLCHAIN_FILE = " ${CMAKE_TOOLCHAIN_FILE})

set(game_SOURCES 
	${CMAKE_SOURCE_DIR}/glad/src/glad.c
	src/element/element_factory.cpp
	src/element/plane.cpp
	src/element/object.cpp
	src/element/main_plane.cpp
	src/element/missile.cpp
	src/element/enemy_plane.cpp
	src/element/background.cpp
    src/main.cpp
	src/setting.cpp
	src/game_context.cpp
	src/common/common.cpp
	src/common/time_util.cpp
)
add_executable(${PROJECT_NAME}
		${game_SOURCES}
)
# target_link_libraries(${PROJECT_NAME}  glfw3 opengl32 sfml-audio sfml-system sfml-network sfml-graphics sfml-window)

target_link_libraries(${PROJECT_NAME}  glfw3 opengl32 sfml-audio)



