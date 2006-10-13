
if(KDE4_BUILD_TESTS)

########### next target ###############

set(testfeatures_SRCS testfeatures.cpp)
kode_add_xml_parser(testfeatures_SRCS kde-features.rng)

kde4_automoc(testfeatures ${testfeatures_SRCS})

kde4_add_executable(testfeatures ${testfeatures_SRCS})

target_link_libraries(testfeatures  ${KDE4_KDECORE_LIBS} )

########### next target ###############

set(testholidays_SRCS testholidays.cpp)
kode_add_xml_parser(testholidays_SRCS kde-holidays.rng)

kde4_automoc(testholidays ${testholidays_SRCS})

kde4_add_executable(testholidays ${testholidays_SRCS})

target_link_libraries(testholidays  ${KDE4_KDECORE_LIBS} )

endif(KDE4_BUILD_TESTS)
