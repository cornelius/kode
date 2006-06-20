
set(kode_LIB_SRCS 
   code.cpp 
   enum.cpp 
   style.cpp 
   printer.cpp 
   license.cpp 
   file.cpp 
   class.cpp 
   function.cpp 
   variable.cpp 
   membervariable.cpp 
   typedef.cpp 
   statemachine.cpp 
   automakefile.cpp )

kde4_automoc(${kode_LIB_SRCS})

kde4_add_library(kode SHARED ${kode_LIB_SRCS})

target_link_libraries(kode  ${KDE4_KDECORE_LIBS} )

set_target_properties(kode PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kode )
