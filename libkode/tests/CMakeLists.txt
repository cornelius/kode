include_directories( ${CMAKE_SOURCE_DIR} )

set(nestedclasses_test_SRCS nestedclasses_test.cpp )


kde4_add_executable(nestedclasses_test RUN_UNINSTALLED ${nestedclasses_test_SRCS})

target_link_libraries(nestedclasses_test ${QT_QTCORE_LIBRARY} kode )
