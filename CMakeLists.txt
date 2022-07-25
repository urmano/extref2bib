cmake_minimum_required(VERSION 3.16)
project(extref2bib VERSION 1.0.0 DESCRIPTION "extract references to bib")

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "BUILD_SHARED_LIBS: ${BUILD_SHARED_LIBS}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++2a")

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 871ed52d350214a034f6ef8a3b8f51c5ce1bd400)
FetchContent_MakeAvailable(cpr)

include(ExternalProject)

ExternalProject_Add(
    dependency.json_spirit
    URL ${CMAKE_CURRENT_SOURCE_DIR}/deps/json_spirit
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/json_spirit
    CMAKE_COMMAND ${CMAKE_COMMAND}
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    CMAKE_ARGS ${BOOST_CMAKE_ARGS} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG> --target json
    INSTALL_COMMAND "")

ExternalProject_Get_Property(dependency.json_spirit SOURCE_DIR BINARY_DIR)
set(INSTALL_DIR ${BINARY_DIR}/${CMAKE_CFG_INTDIR})

add_library(json_spirit STATIC IMPORTED)

set(libjson_name "${CMAKE_STATIC_LIBRARY_PREFIX}json${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_property(
TARGET json_spirit
PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/${libjson_name})

set_property(
TARGET json_spirit
PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR})

add_dependencies(json_spirit dependency.json_spirit)

# FILE(GLOB SOURCES src/*.cc)
# FILE(GLOB HEADERS include/${PROJECT_NAME}/*.hh)
# include_directories(include)
# add_library(${PROJECT_NAME} SHARED ${SOURCES})
# set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
# set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/${PROJECT_NAME}/${PROJECT_NAME}.hxx)
# target_include_directories(${PROJECT_NAME} PRIVATE include/)
# target_include_directories(${PROJECT_NAME} PRIVATE src/)
# install(TARGETS ${PROJECT_NAME})
  # LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib/
# PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include/cea/)
# install(FILES ${HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})
# configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
# install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_PREFIX})
# target_link_libraries(${PROJECT_NAME} cpr::cpr json_spirit)
# target_link_libraries(${PROJECT_NAME})

set(EXE extref2bibexe)
add_executable(${EXE} main.cxx)
# add_dependencies(${EXE} ${PROJECT_NAME})
# target_link_libraries(${EXE} ${PROJECT_NAME})
target_link_libraries(${EXE} cpr::cpr json_spirit)

install(TARGETS ${EXE} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)

# FILE(GLOB TESTS tests/*.cc)
# foreach(test ${TESTS})
#   string(REGEX REPLACE "\\.[^.]*$" "" test_mid ${test})
#   string(REGEX REPLACE ".*tests/" "" test_exe ${test_mid})
#   add_executable(${test_exe} ${test})
#   add_dependencies(${test_exe} ${PROJECT_NAME})
#   target_link_libraries(${test_exe} ${PROJECT_NAME})
  # install(TARGETS ${test_exe} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/tests)
# endforeach()
