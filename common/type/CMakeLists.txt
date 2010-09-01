PROJECT(Type)

INCLUDE_DIRECTORIES( ${extOpenCV2_1_INCLUDE_PATH} )
LINK_DIRECTORIES( ${extOpenCV2_1_LIB_PATH} )
add_library(userparams src/userparamsimpl.cc)

add_executable(typeTest tests/src/typeTest.cc) 
target_link_libraries( typeTest userparams cv highgui)

add_executable(singletonTest test/src/singletonTest.cc) 
