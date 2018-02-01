include_directories( ${kode_SOURCE_DIR} )

set( nestedclasses_test_SRCS nestedclasses_test.cpp )


add_executable( nestedclasses_test ${nestedclasses_test_SRCS} )

target_link_libraries( nestedclasses_test ${QT_QTCORE_LIBRARY} kode )
