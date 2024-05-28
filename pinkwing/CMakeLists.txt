cmake_minimum_required(VERSION 3.0.0)
project(pinkwing VERSION 0.1.0 LANGUAGES C  CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_DIR_LIST 
        "onvif/*.cpp"
)

set(CMAKE_BUILD_TYPE Debug)

add_library(${PROJECT_NAME} STATIC ${SRC_DIR_LIST})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/onvif)

target_compile_definitions(pinkwing PUBLIC WITH_OPENSSL WITH_DOM)
target_link_libraries(pinkwing PUBLIC ssl crypto)


# add the executable
add_executable(example example.cpp  )
target_compile_definitions(example PUBLIC WITH_OPENSSL WITH_DOM)
target_link_libraries(example PUBLIC pinkwing pthread ssl crypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
