cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(kode)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include(CTest)

find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(SYSTEM
	${CMAKE_CURRENT_SOURCE_DIR}/libkode
	${CMAKE_CURRENT_SOURCE_DIR}/libkode/code_generation
)

add_subdirectory(libkode)
add_subdirectory(kxml_compiler)
#add_subdirectory(kxforms)

########### next target ###############

set(KODEBIN_SOURCES kodemain.cpp)

add_executable(kode_bin ${KODEBIN_SOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(kode_bin
	Qt5::Core
	Qt5::Network
	Qt5::Xml
	kode
)

set_target_properties(kode_bin PROPERTIES OUTPUT_NAME kode)

install( 
    TARGETS kode_bin
    RUNTIME DESTINATION bin
)
