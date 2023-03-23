# ==============================================================================
# 公共头文件
# ==============================================================================
SET(THIRD_PARTY_ROOT_PATH ${CMAKE_HOME_DIRECTORY}/third_party/)

INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH})
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/lua)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/sqlite)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/rapidxml)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/jsoncpp/include)
INCLUDE_DIRECTORIES(${THIRD_PARTY_ROOT_PATH}/ratas/src)

# ==============================================================================
# 一些单文件项目
# ==============================================================================
add_executable(fight_system fight_system.cpp)
