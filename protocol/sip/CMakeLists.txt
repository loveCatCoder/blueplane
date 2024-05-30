
file(GLOB CPP_NAME_LIST ./*.cpp)
add_library(sip STATIC ${CPP_NAME_LIST})
