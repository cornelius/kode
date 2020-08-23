MACRO(KODE_ADD_LOCAL_XML_PARSER _sources _current_file)
	get_filename_component(_schema ${_current_file} ABSOLUTE)
	get_filename_component(_basename ${_schema} NAME_WE)

	set(_source_cpp ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.cpp)
	set(_source_h ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h)

	add_custom_command(
		OUTPUT ${_source_cpp} ${_source_h}
		COMMAND "${CMAKE_CURRENT_BINARY_DIR}/../kxml_compiler"
		ARGS ${_schema} ${ARGN}
		MAIN_DEPENDENCY ${_schema}
		DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/../kxml_compiler"
	)

	set(${_sources} ${${_sources}} ${_source_cpp})
ENDMACRO(KODE_ADD_LOCAL_XML_PARSER)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt5Test REQUIRED)


# testfeatures
# FIXME BROKEN

set(testfeatures_SRCS testfeatures.cpp)
kode_add_local_xml_parser(testfeatures_SRCS data/kde-features.rng)
#add_executable(testfeatures ${testfeatures_SRCS})
#target_link_libraries(testfeatures Qt5::Core Qt5::Network	Qt5::Xml)


# testholidays
# FIXME BROKEN

set(testholidays_SRCS testholidays.cpp)
kode_add_local_xml_parser(testholidays_SRCS data/kde-holidays.rng)
#add_executable(testholidays ${testholidays_SRCS})
#target_link_libraries(testholidays	Qt5::Core	Qt5::Network Qt5::Xml)


# parserxsdtest

set(parsertest_SRCS parserxsdtest.h parserxsdtest.cpp)
add_executable(parserxsdtest ${parsertest_SRCS})
target_link_libraries(parserxsdtest Qt5::Core Qt5::Network Qt5::Test Qt5::Xml)
target_link_libraries(parserxsdtest	kode libkxml_compiler	xmlschema	xmlcommon)


# testaccounts
# FIXME BROKEN

set(testaccounts_SRCS testaccounts.cpp)
kode_add_local_xml_parser(testaccounts_SRCS data/account.xml
	--create-crud-functions)
#add_executable(testaccounts ${testaccounts_SRCS})
#target_link_libraries(testaccounts Qt5::Core Qt5::Network	Qt5::Xml)


# testparser
# FIXME BROKEN

set(testparser_SRCS testparser.h testparser.cpp)
#add_executable(testparser ${testparser_SRCS})
#target_link_libraries(testparser Qt5::Core Qt5::Network Qt5::Test Qt5::Xml)

# tests

add_test(RunKXmlCompiler ${EXECUTABLE_OUTPUT_PATH}/../kxml_compiler --help)
add_test(RunParserxsdtest ${EXECUTABLE_OUTPUT_PATH}/parserxsdtest)
#add_test(RunTestFeatures ${EXECUTABLE_OUTPUT_PATH}/testfeatures)
#add_test(RunTestHolidays ${EXECUTABLE_OUTPUT_PATH}/testholidays)
#add_test(RunTestAccount ${EXECUTABLE_OUTPUT_PATH}/testaccounts
#	${CMAKE_CURRENT_SOURCE_DIR}/data/account.xml)
#add_test(RunTestAccount2 ${EXECUTABLE_OUTPUT_PATH}/testaccounts
#	${CMAKE_CURRENT_SOURCE_DIR}/data/account2.xml
#	${CMAKE_CURRENT_SOURCE_DIR}/data/out_account2.xml)
#add_test(RunTestParser ${EXECUTABLE_OUTPUT_PATH}/testparser)
