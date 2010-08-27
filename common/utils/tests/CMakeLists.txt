PROJECT(Utilities)
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)

INCLUDE_DIRECTORIES( ${extOpenCV2_1_INCLUDE_PATH} )
LINK_DIRECTORIES( ${extOpenCV2_1_LIB_PATH} )

add_executable(hdrRead src/hdrRead.cc)
target_link_libraries( hdrRead ${extOpenCV2_1_LIBS})

add_executable(helloWorld src/helloWorld.cc)
target_link_libraries( helloWorld ${extOpenCV2_1_LIBS})

add_executable(radiancetest src/radiancetest.cc)
target_link_libraries( radiancetest ${extOpenCV2_1_LIBS} ${Boost_LIBRARIES} ${boost_filesystem_LIBRARY})
