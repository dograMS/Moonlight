cmake_minimum_required(VERSION 3.6.0)

add_subdirectory("Networking" Net_Client)

project(Moon_Light)

set (CMAKE_CXX_FLAGS "-std=c++2a ${CMAKE_CXX_FLAGS}")

file(GLOB_RECURSE SRCS 
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(tui_light ${SRCS})

target_include_directories(tui_light PUBLIC
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

target_sources(tui_light PRIVATE ${SRCS})
target_link_libraries(tui_light PRIVATE  Sun_Client)

target_link_libraries(tui_light PRIVATE Sun_Net )
