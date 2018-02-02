include_directories( ${kode_SOURCE_DIR} )

set( nestedclasses_test_SRCS nestedclasses_test.cpp )

add_executable( nestedclasses_test ${nestedclasses_test_SRCS} )
qt5_use_modules(nestedclasses_test Core)
qt5_use_modules(nestedclasses_test Xml)
qt5_use_modules(nestedclasses_test Network)

target_link_libraries( nestedclasses_test ${QT_QTCORE_LIBRARY} kode )
