PROJECT(Utilities)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)

add_executable(hdrRead src/hdrRead.cc)
target_link_libraries( hdrRead ${OpenCV_LIBRARIES})

add_executable(helloWorld src/helloWorld.cc)
target_link_libraries( helloWorld ${OpenCV_LIBRARIES})

add_executable(radiancetest src/radiancetest.cc)
target_link_libraries( radiancetest ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} ${boost_filesystem_LIBRARY})
