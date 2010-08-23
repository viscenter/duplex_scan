PROJECT(Type)
FIND_PACKAGE(OpenCV REQUIRED)

add_library(userparams src/userparamsimpl.cc)

add_executable(typeTest tests/src/typeTest.cc) 
target_link_libraries( typeTest userparams ${OpenCV_LIBRARIES})

add_executable(singletonTest tests/src/singletonTest.cc) 
