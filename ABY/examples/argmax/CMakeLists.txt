cmake_minimum_required(VERSION 3.10)
project(garble_circuit)

set(CMAKE_CXX_STANDARD 14)

# Specify the paths to the EMP toolkit headers and libraries
include_directories(/usr/local/include)  # Adjust if your ABY libraries are elsewhere
link_directories(/usr/local/lib)          # Adjust if your ABY libraries are elsewhere

find_package(ABY REQUIRED)
if (ABY_FOUND)
    message(STATUS "ABY found at: ${ABY_LIBRARIES}")
else()
    message(FATAL_ERROR "ABY not found!")
endif()

find_package(ENCRYPTO_utils REQUIRED)
if (ENCRYPTO_utils_FOUND)
    message(STATUS "ENCRYPTO_utils found at: ${ENCRYPTO_utils_LIBRARIES}")
else()
    message(FATAL_ERROR "ENCRYPTO_utils not found!")
endif()

# Add the main executable
# add_executable(main main.cpp)
add_executable(main_large main_large.cpp)

# Link ABY library
target_link_libraries(main_large
    ABY::aby
    ENCRYPTO_utils::encrypto_utils
)
