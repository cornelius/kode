#
# Macros for the 'kode' code generatation tools
#
# see FindKode.cmake for documentation
#

MACRO (KODE_ADD_XML_PARSER _sources)
  foreach (_current_file ${ARGN})
    get_filename_component( _schema ${_current_file} ABSOLUTE )
    get_filename_component( _basename ${_schema} NAME_WE )

    set( _source_cpp ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cpp )
    set( _source_h   ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h )
    set( _parser_cpp ${CMAKE_CURRENT_BINARY_DIR}/${_basename}_parser.cpp )
    set( _parser_h   ${CMAKE_CURRENT_BINARY_DIR}/${_basename}_parser.h )

    add_custom_command(
      OUTPUT ${_source_cpp} ${_source_h} ${_parser_cpp} ${_parser_h}
      COMMAND ${KODE_XML_COMPILER_EXECUTABLE}
      ARGS --external-parser ${_schema}
      MAIN_DEPENDENCY ${_schema}
      DEPENDS ${_KODE_XML_COMPILER_DEP}
    )

    set( ${_sources} ${${_sources}} ${_source_cpp} ${_parser_cpp} )
  endforeach (_current_file)
ENDMACRO (KODE_ADD_XML_PARSER)
