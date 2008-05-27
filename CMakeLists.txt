project( kode )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/libkode  ${CMAKE_CURRENT_SOURCE_DIR}/common/ )

add_subdirectory( libkode ) 
add_subdirectory( common ) 
add_subdirectory( schema ) 
add_subdirectory( kxml_compiler ) 
add_subdirectory(kxforms)
add_subdirectory( kwsdl )

########### next target ###############

set(kode_bin_SRCS kodemain.cpp )


kde4_add_executable(kode_bin ${kode_bin_SRCS})
set_target_properties(kode_bin PROPERTIES OUTPUT_NAME kode)

target_link_libraries(kode_bin  ${KDE4_KDEUI_LIBS} kode ${KDE4_KABC_LIBS} )

install(TARGETS kode_bin  ${INSTALL_TARGETS_DEFAULT_ARGS} )
