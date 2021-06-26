CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT("Cunnyware")

SET(CMAKE_CXX_FLAGS "-std=c++2a -ldl -lpthread -fopenmp -Wall -fpic -O2 --no-gnu-unique -ftree-vectorize")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB SOURCES
	"src/*.cpp"
	"src/Hooks/*.cpp"
	"src/Hacks/*.cpp"
	"src/Util/*.cpp"
	"src/UI/*.cpp"
	"src/UI/Tabs/*.cpp"
	"src/ImGUI/imgui_draw.cpp"
	"src/ImGUI/imgui_freetype.cpp"
	"src/incbin/*.c")
list(APPEND SOURCES "src/ImGUI/examples/imgui_impl_opengles3.cpp")
list(APPEND SOURCES "src/ImGUI/examples/libs/gl3w/GL/gl3w.c")
list(APPEND SOURCES "src/incbin/incbin.c")

INCLUDE_DIRECTORIES("src/ImGUI/")
INCLUDE_DIRECTORIES("src/ImGUI/examples/")
INCLUDE_DIRECTORIES("src/ImGUI/examples/libs/")
INCLUDE_DIRECTORIES("src/ImGUI/examples/libs/gl3w/")
INCLUDE_DIRECTORIES("libs/freetype/include")

ADD_SUBDIRECTORY(libs/fmt)
ADD_CUSTOM_TARGET(
   libFreetype
   COMMAND make
   WORKING_DIRECTORY ../libs/freetype
)
ADD_LIBRARY(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} fmt dl SDL2 GL "${CMAKE_SOURCE_DIR}/libs/freetype/objs/.libs/libfreetype.so")
ADD_DEPENDENCIES(${CMAKE_PROJECT_NAME} libFreetype)
