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

find_package(RELIC REQUIRED)

if (RELIC_FOUND)
    message(STATUS "RELIC found at: ${RELIC_INCLUDE_DIRS}")
    include_directories(${RELIC_INCLUDE_DIRS})
    link_directories(${RELIC_LIBRARIES})
else()
    message(FATAL_ERROR "RELIC not found!")
endif()

find_package(Boost 1.69.0 COMPONENTS system thread filesystem REQUIRED)

# Add the main executable
add_executable(simple-aby main.cpp)

# Link ABY library
target_link_libraries(simple-aby
	# /usr/local/lib/libaby.a OR
    ABY::aby
    ENCRYPTO_utils::encrypto_utils
    RELIC::relic
)
