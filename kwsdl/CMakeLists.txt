
add_subdirectory( doc ) 
add_subdirectory( wsdl ) 
#add_subdirectory( wscl ) 
#add_subdirectory( kung ) 

include_directories( ${CMAKE_SOURCE_DIR}/kode  )


########### next target ###############

set(kwsdl_compiler_SRCS 
   compiler.cpp 
   converter.cpp 
   converter_attribute.cpp 
   converter_clientstub.cpp 
   converter_complextype.cpp 
   converter_element.cpp 
   converter_serverstub.cpp 
   converter_simpletype.cpp 
   converter_transport.cpp 
   converter_utils.cpp 
   creator.cpp 
   main.cpp 
   namemapper.cpp 
   settings.cpp 
   typemap.cpp )

kde4_automoc(${kwsdl_compiler_SRCS})

kde4_add_executable(kwsdl_compiler ${kwsdl_compiler_SRCS})

target_link_libraries(kwsdl_compiler  ${KDE4_KDECORE_LIBS} kode wsdl )

install_targets(/bin kwsdl_compiler )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kwsdl_compiler.desktop )
