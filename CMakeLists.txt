cmake_minimum_required(VERSION 3.12)
project(deep_client_php_extension)

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package(PythonLibs REQUIRED)

set(PHP_INCLUDE_DIR php_headers)
set(EXT_PHP_INCLUDE_DIR ext_php_headers)
set(ZEND_INCLUDE_DIR zend_headers)

include_directories(${PHP_INCLUDE_DIR})
include_directories(${EXT_PHP_INCLUDE_DIR})
include_directories(${ZEND_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})

# Find Boost package
find_package(Boost REQUIRED COMPONENTS python)

# Add the Boost include directory
include_directories(${Boost_INCLUDE_DIRS})

# Define your source files
set(SOURCES
        deep_client_php_wrapper.cpp
)

# Create the Python/PHP module
add_library(deep_client_php_extension MODULE ${SOURCES})

# Link against Boost libraries
target_link_libraries(deep_client_php_extension ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
set_target_properties(deep_client_php_extension PROPERTIES PREFIX "")