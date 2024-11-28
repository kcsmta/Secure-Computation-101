cmake_minimum_required(VERSION 3.10)
project(oblivious_transfer)

set(CMAKE_CXX_STANDARD 14)

# Specify the paths to the EMP toolkit headers and libraries
include_directories(/usr/local/include)  # Adjust if your EMP libraries are elsewhere
link_directories(/usr/local/lib)          # Adjust if your EMP libraries are elsewhere

# Find OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    message(STATUS "OpenSSL found at: ${OPENSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL not found!")
endif()

# Add the main executable
add_executable(simple-ot main.cpp)

# Link EMP libraries and OpenSSL
target_link_libraries(simple-ot 
    /usr/local/lib/libemp-tool.so
    OpenSSL::SSL 
    OpenSSL::Crypto
)