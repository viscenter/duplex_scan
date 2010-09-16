PROJECT(Utilities)

file(COPY resources DESTINATION .)

FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Boost COMPONENTS filesystem REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)

INCLUDE_DIRECTORIES( ${extOpenCV2_1_INCLUDE_PATH} )
LINK_DIRECTORIES( ${extOpenCV2_1_LIB_PATH} )


add_executable(hdrRead tests/src/hdrRead.cc)
target_link_libraries( hdrRead cv highgui imageutils ${Boost_LIBRARIES} ${boost_filesystem_LIBRARY})

add_executable(helloWorld tests/src/helloWorld.cc)
target_link_libraries( helloWorld cv highgui)

add_executable(radiancetest tests/src/radiancetest.cc)
target_link_libraries( radiancetest cv highgui imageutils ${Boost_LIBRARIES} ${boost_filesystem_LIBRARY})

add_executable(duplexScanStats src/duplexScanStats.cc)
target_link_libraries( duplexScanStats cv highgui vizcamera ${Boost_LIBRARIES} ${boost_filesystem_LIBRARY} ${boost_program_options_LIBRARY})
