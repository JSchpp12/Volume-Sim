cmake_minimum_required(VERSION 3.16)

project(VDBProcessor 
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenVDB CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)

set(SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VolumeFile.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VolumeProcessor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CmdLineArgHandler.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/DirectoryContentProcessor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Compressor.cpp"
)

add_executable(
    VDBProcessor
    "${SRC_FILES}"
)

target_include_directories(VDBProcessor
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(VDBProcessor
    PRIVATE
    OpenVDB::openvdb
    zstd::libzstd
)

target_compile_options(${PROJECT_NAME} PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/MP> 
    $<$<CXX_COMPILER_ID:MSVC>:/EHsc> 
    $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
)
