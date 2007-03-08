
if(KDE4_BUILD_TESTS)

include_directories( ${CMAKE_SOURCE_DIR}/kode  )

########### next target ###############

set(formcreatortest_SRCS 
   formcreatortest.cpp 
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
   ../section.cpp )

kde4_add_kcfg_files(formcreatortest_SRCS ../prefs.kcfgc )

kde4_automoc(${formcreatortest_SRCS})

kde4_add_executable(formcreatortest NOGUI RUN_UNINSTALLED ${formcreatortest_SRCS})

target_link_libraries(formcreatortest kschema  ${QT_QTTEST_LIBRARY} ${KDE4_KDE3SUPPORT_LIBS}  )

endif(KDE4_BUILD_TESTS)

########### install files ###############
