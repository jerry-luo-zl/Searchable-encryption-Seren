cmake_minimum_required(VERSION 3.16)

project(Seren)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(OpenSSL REQUIRED)
set(Boost_REQUIRED_VERSION "1.84.0")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

# build Thrift dependencies
set(THRIFT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Thrift/Server.thrift)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

exec_program("thrift --gen cpp -o ${CMAKE_CURRENT_SOURCE_DIR} ${THRIFT_FILE}")

add_library(gen-cpp-thrift STATIC
        gen-cpp/SSEService.cpp
        gen-cpp/Server_constants.cpp
        gen-cpp/Server_types.cpp)

target_link_libraries(gen-cpp-thrift libthrift.a)

# load Aura header
INCLUDE_DIRECTORIES(. BF BF/Hash GGM Util gen-cpp)

# set executable outputs
#ADD_EXECUTABLE(AESTest Test/AESTest.cpp Util/CommonUtil.c)
#ADD_EXECUTABLE(BloomFilterTest Test/BloomFilterTest.cpp BF/Hash/SpookyV2.cpp)
#ADD_EXECUTABLE(GGMTest Test/GGMTest.cpp GGM/GGMTree.cpp Util/CommonUtil.c)
#ADD_EXECUTABLE(SSETest Test/SSETest.cpp Core/SSEClientHandler.cpp Core/SSEServerHandler.cpp GGM/GGMTree.cpp BF/Hash/SpookyV2.cpp Util/CommonUtil.c)
ADD_EXECUTABLE(SerenServer Core/SSEServer.cpp Core/SSEServerHandler.cpp GGM/GGMTree.cpp BF/Hash/SpookyV2.cpp Util/CommonUtil.c gen-cpp)
ADD_EXECUTABLE(SerenClient main.cpp Core/SSEClient.cpp GGM/GGMTree.cpp BF/Hash/SpookyV2.cpp Util/CommonUtil.c gen-cpp)

# link
#TARGET_LINK_LIBRARIES(AESTest OpenSSL::Crypto)
#TARGET_LINK_LIBRARIES(GGMTest OpenSSL::Crypto)
#TARGET_LINK_LIBRARIES(SSETest OpenSSL::Crypto gen-cpp-thrift librocksdb.a)
TARGET_LINK_LIBRARIES(SerenServer OpenSSL::Crypto gen-cpp-thrift librocksdb.a)
TARGET_LINK_LIBRARIES(SerenClient OpenSSL::Crypto gen-cpp-thrift librocksdb.a)
