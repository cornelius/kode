# 
# KODE_XML_COMPILER_EXECUTABLE - the kxml_compiler executable
#
# The following macros are provided (from KodeMacros.cmake):
#
# kode_add_xml_parser (name schema1 ... schemaN)
#   Generate code to handle XML defined by the given schemata
#
# First let's see if kxml_compiler can be found
find_program( KODE_XML_COMPILER_EXECUTABLE kxml_compiler )

# If it wasn't found, try to use an alternate method
if ( NOT KODE_XML_COMPILER_EXECUTABLE )
  message( STATUS "The Kode XML Compiler wasn't found in your executable path" )

  if ( WIN32 )
    set( KODE_XML_COMPILER_EXECUTABLE ${kode_BINARY_DIR}/bin/${CMAKE_CFG_INTDIR}/kxml_compiler.exe.bat )
    message ( STATUS "Trying to use kxml_compiler.exe.bat instead" )
  else ( WIN32 )
     # On problems when building with XCode try adding ${CMAKE_CFG_INTDIR}, see also FindKDE4Internal.cmake
     set( KODE_XML_COMPILER_EXECUTABLE ${kode_BINARY_DIR}/kxml_compiler/kxml_compiler.shell )
  endif ( WIN32 )

  message ( STATUS "${KODE_XML_COMPILER_EXECUTABLE} will be used" )
  set( _KODE_XML_COMPILER_DEP kxml_compiler )
endif ( NOT KODE_XML_COMPILER_EXECUTABLE )

# macros to use the Kode tools
include( KodeMacros )
