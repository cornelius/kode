cmake_minimum_required(VERSION 3.5)

project( kode )

find_package(Qt5Core REQUIRED
    Xml
    Network
)

add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )

include_directories(
    ${Qt5Core_INCLUDES}
    ${Qt5Xml_INCLUDES}
    ${Qt5Network_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/libkode
    ${CMAKE_CURRENT_SOURCE_DIR}/kwsdl
)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_subdirectory( libkode )
add_subdirectory( common )
add_subdirectory( schema )
add_subdirectory( kxml_compiler )
#add_subdirectory( kxforms )
#add_subdirectory( kwsdl )

########### next target ###############



set( kode_bin_SRCS kodemain.cpp )

add_executable(kode_bin kodemain.cpp)
qt5_use_modules(kode_bin Core)
qt5_use_modules(kode_bin Xml)
qt5_use_modules(kode_bin Network)

# Use the Widgets module from Qt 5.
target_link_libraries(kode_bin Qt5::Core)
target_link_libraries(kode_bin Qt5::Xml)
target_link_libraries(kode_bin Qt5::Network)
target_link_libraries( kode_bin kode  )

set_target_properties( kode_bin PROPERTIES OUTPUT_NAME kode )

install( 
    TARGETS kode_bin  ${INSTALL_TARGETS_DEFAULT_ARGS} 
    RUNTIME DESTINATION bin
)
