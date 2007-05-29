
add_subdirectory( tests )

include_directories( ${QT_INCLUDES} ${CMAKE_SOURCE_DIR}/kode ${KDE4_INCLUDE_DIR} )
########### next target ###############

set(kxforms_SRCS 
   main.cpp 
   mainwindow.cpp 
   manager.cpp 
   form.cpp 
   formcreator.cpp
   hints.cpp
   list.cpp 
   listmodel.cpp
   input.cpp 
   formgui.cpp 
   remotefile.cpp
   xformscommon.cpp 
   xmlbuilder.cpp
   guielement.cpp 
   kresult.cpp 
   guihandler.cpp 
   guihandlerflat.cpp
   guihandlerdialogs.cpp
   reference.cpp 
   textarea.cpp 
   select1.cpp
   section.cpp 
   dispatcher.cpp
   select.cpp 
   listproxymodel.cpp
   layout.cpp
   editor/editorwidget.cpp
   editor/editor.cpp 
   editor/editoraction.cpp
   editor/appearanceaction.cpp 
   editor/listaction.cpp
   editor/positionaction.cpp
   editor/layoutstyleaction.cpp
   editor/groupaction.cpp
   editor/changelabelaction.cpp 
   editor/globalsettingsdlg.cpp )

kde4_automoc(${kxforms_SRCS})

kde4_add_kcfg_files(kxforms_SRCS prefs.kcfgc )

kde4_add_executable(kxforms ${kxforms_SRCS})

target_link_libraries(kxforms  ${KDE4_KIO_LIBS} kschema ${KDE4_KDE3SUPPORT_LIBS} )

install(TARGETS kxforms  DESTINATION ${BIN_INSTALL_DIR} )

########### next target ###############

if(KDE4_BUILD_TESTS)
set(testreferences_SRCS testreferences.cpp reference.cpp )

kde4_automoc(${testreferences_SRCS})

kde4_add_executable(testreferences ${testreferences_SRCS})

target_link_libraries(testreferences  ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

endif(KDE4_BUILD_TESTS)

########### install files ###############

install( FILES  kxformsui.rc  DESTINATION ${DATA_INSTALL_DIR}/kxforms)

install( FILES kxforms.kcfg  DESTINATION ${KCFG_INSTALL_DIR})
