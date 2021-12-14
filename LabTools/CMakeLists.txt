#  Lab Analysis Toolset "LabTools.h" -*- C++ -*- 
#  Author M. Sotgia 05/11/21
#  v0.0.2 alpha

cmake_minimum_required(VERSION 3.16)

project(LabTools VERSION 0.0.2)

find_package(ROOT)

set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})

add_compile_options(-I${ROOT_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(LabTools SHARED src/LabTools.cpp)

target_link_libraries(LabTools ${ROOT_LIBRARIES})

install(TARGETS LabTools DESTINATION /usr/local/lib/)
install(FILES include/LabTools.h DESTINATION include)