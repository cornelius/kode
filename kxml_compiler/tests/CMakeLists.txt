MACRO (KODE_ADD_LOCAL_XML_PARSER _sources)
  foreach (_current_file ${ARGN})
    get_filename_component( _schema ${_current_file} ABSOLUTE )
    get_filename_component( _basename ${_schema} NAME_WE )

    set( _source_cpp ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cpp )
    set( _source_h   ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h )

    add_custom_command(
      OUTPUT ${_source_cpp} ${_source_h}
      COMMAND "${CMAKE_CURRENT_BINARY_DIR}/../kxml_compiler"
      ARGS ${_schema}
      MAIN_DEPENDENCY ${_schema}
      DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/../kxml_compiler"
    )

    set( ${_sources} ${${_sources}} ${_source_cpp} )
  endforeach (_current_file)
ENDMACRO (KODE_ADD_LOCAL_XML_PARSER)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

########### next target ###############

set( testfeatures_SRCS testfeatures.cpp )
kode_add_xml_parser( testfeatures_SRCS kde-features.rng )


kde4_add_executable( testfeatures TEST ${testfeatures_SRCS} )

target_link_libraries( testfeatures  ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

########### next target ###############

set( testholidays_SRCS testholidays.cpp )
kode_add_xml_parser( testholidays_SRCS kde-holidays.rng )

kde4_add_executable( testholidays TEST ${testholidays_SRCS} )

target_link_libraries( testholidays ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )


########### next target ###############

set( parsertest_SRCS parserxsdtest.cpp )

kde4_add_executable( parserxsdtest TEST ${parsertest_SRCS} )

target_link_libraries( parserxsdtest kschema ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} )

# next target

set( testaccounts_SRCS testaccounts.cpp )
kode_add_local_xml_parser( testaccounts_SRCS account.xml )
kde4_add_executable( testaccounts TEST ${testaccounts_SRCS} )
target_link_libraries( testaccounts ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

# tests
set( testparser_SRCS testparser.cpp )
kde4_add_executable( testparser TEST ${testparser_SRCS} )
target_link_libraries( testparser ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
  ${QT_QTXML_LIBRARY} )

# tests

ADD_TEST(RunKXmlCompiler ${EXECUTABLE_OUTPUT_PATH}/../kxml_compiler --help)
ADD_TEST(RunTestFeatures ${EXECUTABLE_OUTPUT_PATH}/testfeatures)
ADD_TEST(RunTestHolidays ${EXECUTABLE_OUTPUT_PATH}/testholidays)
ADD_TEST(RunTestAccount ${EXECUTABLE_OUTPUT_PATH}/testaccounts
  ${CMAKE_CURRENT_SOURCE_DIR}/account.xml)
ADD_TEST(RunTestParser ${EXECUTABLE_OUTPUT_PATH}/testparser)
