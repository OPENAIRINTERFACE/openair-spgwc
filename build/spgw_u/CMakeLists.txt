cmake_minimum_required (VERSION 3.0.2)
# Override options for SPGW
set (  PACKAGE_NAME                    "S/P-GW-U" )
set (  STATIC_LINKING                   False )
#############################################
# Base directories, compatible with legacy OAI building
################################################
set (OPENAIRCN_DIR   $ENV{OPENAIRCN_DIR})
set (BUILD_TOP_DIR   ${OPENAIRCN_DIR}/build)
set (SRC_TOP_DIR     $ENV{OPENAIRCN_DIR}/src)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/oai_spgw_u/CMakeLists.txt)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../src/spgw_u ${CMAKE_CURRENT_BINARY_DIR}/spgw_u)
