cmake_minimum_required(VERSION 3.10)
project(emp_ot_playaround)

set(CMAKE_CXX_STANDARD 14)

# Add the required flags for AES-NI support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -msse4.1")

find_package(OpenSSL REQUIRED)

#Testing macro
macro (add_test_executable_with_lib _name libs)
	add_executable(test_${_name} "${_name}.cpp")
	target_link_libraries(test_${_name} ${EMP-OT_LIBRARIES} OpenSSL::Crypto)
endmacro()

macro (add_test_case _name)
	add_test_executable_with_lib(${_name} "")
  	add_test(NAME ${_name} COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${_name}" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")
endmacro()

macro (add_test_case_with_run _name)
	add_test_executable_with_lib(${_name} "")
	add_test(NAME ${_name} COMMAND "./run" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${_name}" WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/")

endmacro()

# Test cases
add_test_case_with_run(bit)
add_test_case_with_run(int)
IF(${ENABLE_FLOAT})
add_test_case_with_run(float)
ENDIF(${ENABLE_FLOAT})
add_test_case_with_run(circuit_file)
add_test_case_with_run(example)
add_test_case_with_run(repeat)
