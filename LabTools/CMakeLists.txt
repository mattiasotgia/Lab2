#  Lab Analysis Toolset "LabTools.h" -*- C++ -*- 
#  Author M. Sotgia 05/11/21
#  v0.0.2 alpha

cmake_minimum_required(VERSION 3.16)

project(LabTools VERSION 0.0.2)

find_package(ROOT)

set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})
set(LIBSRC 
    src/LabTools.cpp
    src/RooPlot.cpp
    src/Logger.cpp
    src/StaTools.cpp
    src/ATLASStyle.cpp
)
set(LIBINC
    include/LabTools.h
    include/RooPlot.h
    include/Logger.h
    include/StaTools.h
    include/ATLASStyle.h
)

add_compile_options(-I${ROOT_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(LabTools SHARED ${LIBSRC})

target_link_libraries(LabTools ${ROOT_LIBRARIES})

install(TARGETS LabTools DESTINATION /usr/local/lib/)
install(FILES ${LIBINC} DESTINATION include)

