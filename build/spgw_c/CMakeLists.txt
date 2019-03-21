cmake_minimum_required (VERSION 3.0.2)
# Override options for SPGW
set (  PACKAGE_NAME                    "S/P-GW-C" )
set (  STATIC_LINKING                   False )
#############################################
# Base directories, compatible with legacy OAI building
################################################
set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/build)
set (SRC_TOP_DIR     $ENV{OPENAIRCN_DIR}/src)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/oai_spgw_c/CMakeLists.txt)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../src/sgw_c ${CMAKE_CURRENT_BINARY_DIR}/sgw_c)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../src/pgw_c ${CMAKE_CURRENT_BINARY_DIR}/pgw_c)
