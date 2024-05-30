cmake_minimum_required(VERSION 3.0.0)
project(blueplane VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_DIR_LIST 
        "connector/*.cpp"
        "connector/producer/*.cpp"
        "muduo/net/*.cpp"
        "muduo/net/poller/*.cpp"
        "muduo/base/*.cpp" 
        "protocol/onvif/*.cpp" 
        "protocol/rtcp/*.cpp" 
        "protocol/rtp/*.cpp" 
        "protocol/rtsp/*.cpp" 
        "protocol/sdp/*.cpp" 
        "protocol/sip/*.cpp" 
)



include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/connector
        ${CMAKE_CURRENT_SOURCE_DIR}/muduo
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol/onvif
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol/sdp
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol/rtp
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol/rtsp
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol/sip
        ${CMAKE_CURRENT_SOURCE_DIR}/protocol/rtcp)

# add the executable
add_executable(${PROJECT_NAME} main.cpp ${SRC_DIR_LIST} )


target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_OPENSSL WITH_DOM)
target_link_libraries(${PROJECT_NAME} PUBLIC  pthread ssl crypto)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
