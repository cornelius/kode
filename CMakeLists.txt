project( kode )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(KDE4 REQUIRED)
find_package(KdepimLibs REQUIRED)
include(KDE4Defaults)

add_definitions(-DKDE_DEFAULT_DEBUG_AREA=5295)

include_directories(
    ${KDE4_INCLUDES}
    ${KDEPIMLIBS_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/libkode
    ${CMAKE_CURRENT_SOURCE_DIR}/kwsdl
)

add_subdirectory( libkode ) 
add_subdirectory( common ) 
add_subdirectory( schema ) 
add_subdirectory( kxml_compiler ) 
add_subdirectory( kxforms )
add_subdirectory( kwsdl )
add_subdirectory( cmake )

########### next target ###############

set(kode_bin_SRCS kodemain.cpp )


kde4_add_executable(kode_bin ${kode_bin_SRCS})
set_target_properties(kode_bin PROPERTIES OUTPUT_NAME kode)

target_link_libraries(kode_bin  ${KDE4_KDEUI_LIBS} kode ${KDE4_KABC_LIBS} )

install(TARGETS kode_bin  ${INSTALL_TARGETS_DEFAULT_ARGS} )
