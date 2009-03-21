set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${CMAKE_SOURCE_DIR} )
find_package (Kode REQUIRED)

########### next target ###############

set(testfeatures_SRCS testfeatures.cpp)
kode_add_xml_parser(testfeatures_SRCS kde-features.rng)


kde4_add_executable(testfeatures TEST ${testfeatures_SRCS})

target_link_libraries(testfeatures  ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

########### next target ###############

set(testholidays_SRCS testholidays.cpp)
kode_add_xml_parser(testholidays_SRCS kde-holidays.rng)


kde4_add_executable(testholidays TEST ${testholidays_SRCS})

target_link_libraries(testholidays  ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )


########### next target ###############

set(parsertest_SRCS parserxsdtest.cpp )


kde4_add_executable(parserxsdtest TEST  ${parsertest_SRCS})

target_link_libraries(parserxsdtest kschema  ${QT_QTTEST_LIBRARY} ${QT_QTCORE_LIBRARY} )

########### install files ###############
