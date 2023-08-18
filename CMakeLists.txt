cmake_minimum_required(VERSION 3.12)
project(deep_client_php_extension)

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

INCLUDE_DIRECTORIES(
  "${CMAKE_SOURCE_DIR}/php_headers"
  "${CMAKE_SOURCE_DIR}/ext_php_headers"
  "${CMAKE_SOURCE_DIR}/TSRM"
  "${CMAKE_SOURCE_DIR}/zend_headers"
  "${CMAKE_SOURCE_DIR}/python"
)


SET(PHPCPP_COMMON_SOURCES
  common/modifiers.cpp
  common/streambuf.cpp
)

SET(PHPCPP_COMMON_HEADERS
  common/extensionbase.h
  common/includes.h
  common/streambuf.h
)

ADD_LIBRARY(phpcpp-common STATIC
  ${PHPCPP_COMMON_HEADERS}
  ${PHPCPP_COMMON_SOURCES}
)

set_target_properties(phpcpp-common
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

LINK_DIRECTORIES(
    ${PHP_LIB_PATH}
    include
    zend
)

ADD_LIBRARY(deep_client_php_extension MODULE
	phpcpp.h
	deep_client_php_wrapper.cpp
)

# Find Boost package
find_package(Boost REQUIRED COMPONENTS python)

# Add the Boost include directory
include_directories(${Boost_INCLUDE_DIRS})

find_package(PythonLibs REQUIRED)

# Link against Boost libraries
TARGET_LINK_LIBRARIES(deep_client_php_extension phpcpp-common ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

set_target_properties(deep_client_php_extension
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
  PREFIX ""
)