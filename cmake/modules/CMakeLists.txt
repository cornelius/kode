# install the cmake files

set(cmakeFiles FindKode.cmake KodeMacros.cmake)
install( FILES ${cmakeFiles} DESTINATION ${DATA_INSTALL_DIR}/cmake/modules )