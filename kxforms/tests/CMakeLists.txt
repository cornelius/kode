
if(KDE4_BUILD_TESTS)

include_directories( ${CMAKE_SOURCE_DIR}/kode  )

########### next target ###############

set(kxformstest_SRCS 
   kxformstest.cpp 
   ../manager.cpp 
   ../form.cpp 
   ../formcreator.cpp
   ../hints.cpp
   ../list.cpp 
   ../listmodel.cpp
   ../input.cpp 
   ../formgui.cpp 
   ../remotefile.cpp
   ../xformscommon.cpp 
   ../xmlbuilder.cpp
   ../guielement.cpp 
   ../kresult.cpp 
   ../guihandler.cpp 
   ../guihandlerflat.cpp
   ../guihandlerdialogs.cpp
   ../reference.cpp 
   ../textarea.cpp 
   ../select1.cpp
   ../section.cpp 
   ../dispatcher.cpp
   ../select.cpp 
   ../listproxymodel.cpp
   ../layout.cpp
   ../editor/editorwidget.cpp
   ../editor/editor.cpp 
   ../editor/editoraction.cpp 
   ../editor/changelabelaction.cpp )

kde4_add_kcfg_files(kxformstest_SRCS ../prefs.kcfgc )

kde4_automoc(${kxformstest_SRCS})

kde4_add_executable(kxformstest NOGUI RUN_UNINSTALLED ${kxformstest_SRCS})

target_link_libraries(kxformstest kschema  ${QT_QTTEST_LIBRARY} ${KDE4_KDE3SUPPORT_LIBS}  )

endif(KDE4_BUILD_TESTS)


########### install files ###############
