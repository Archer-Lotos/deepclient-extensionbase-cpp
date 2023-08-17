cmake_minimum_required(VERSION 3.12)
project(deep_client_php_extension)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package(PythonLibs REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(
        ${CMAKE_SOURCE_DIR}/php_headers
        ${CMAKE_SOURCE_DIR}/ext_php_headers
        ${CMAKE_SOURCE_DIR}/zend_headers
        ${CMAKE_SOURCE_DIR}/zend_headers/Optimizer
        ${CMAKE_SOURCE_DIR}/zend
        ${CMAKE_SOURCE_DIR}/include
)

# Find Boost package
find_package(Boost REQUIRED COMPONENTS python)

# Add the Boost include directory
include_directories(${Boost_INCLUDE_DIRS})

# Define your source files
set(SOURCES
        phpcpp.h
        # DeepClient.cpp
        # BoostPythonBindings.cpp
        deep_client_php_wrapper.cpp
)

# Create the Python/PHP module
add_library(deep_client_php_extension MODULE ${SOURCES})

# Link against Boost libraries
target_link_libraries(deep_client_php_extension ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
set_target_properties(deep_client_php_extension PROPERTIES PREFIX "")