#  Lab Analysis Toolset "LabTools.h" -*- C++ -*- 
#  Author M. Sotgia 05/11/21
#  v0.0.2 alpha

cmake_minimum_required(VERSION 3.16)

project(LabTools VERSION 0.0.2)

find_package(ROOT)
find_library(ERR_A_LIB ErrorAnalysis)
find_path(ERR_A_PATH ErrorAnalysis.h PATHS /usr/local)

set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})

set(LIBSRC 
    src/LabTools.cpp
    src/LabPlot.cpp
    # src/LabPlotCore.cpp
    src/LabStat.cpp
    src/ATLASStyle.cpp
    src/Logger.cpp
)
set(LIBINC
    include/LabTools.h
    include/LabPlot.h
    # include/LabPlotCore.cpp
    include/LabStat.h
    include/ATLASStyle.h
    include/Logger.h
)

add_compile_options(-I${ROOT_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(LabTools SHARED ${LIBSRC} ${BODESRC})

target_link_libraries(LabTools ${ROOT_LIBRARIES} ${ERR_A_LIB})
target_include_directories(LabTools PUBLIC ${ERR_A_PATH})

install(TARGETS LabTools DESTINATION /usr/local/lib/)
install(FILES ${LIBINC} DESTINATION include)

