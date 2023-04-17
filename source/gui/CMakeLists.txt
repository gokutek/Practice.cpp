SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/third_party/)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/WTL/Include)

add_executable(hello_wtl WIN32
    hello_wtl.cpp
    )
set_property(TARGET hello_wtl PROPERTY FOLDER "gui")
