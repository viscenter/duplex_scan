PROJECT(Type)

add_library(userparams src/userparamsimpl.cc)

add_executable(typeTest tests/src/typeTest.cc) 
target_link_libraries( typeTest userparams ${extOpenCV2.1_LIBS})

add_executable(singletonTest tests/src/singletonTest.cc) 
