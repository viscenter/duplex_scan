PROJECT(Cameras)

#MESSAGE( "ghoto2 Path at \"${extGphoto2Path}\"")
#MESSAGE( "ghoto2 lib at \"${gphoto2LibPath}\"")
#MESSAGE( "ghoto2_port lib at \"${gphoto2PortLibPath}\"")
#MESSAGE( "ghoto2.h at \"${gphoto2IncPath}\"")

INCLUDE_DIRECTORIES( ${gphoto2IncPath} )
LINK_DIRECTORIES( ${gphoto2LibPath} ${gphoto2PortLibPath} )
INCLUDE_DIRECTORIES( ${extOpenCV2_1_INCLUDE_PATH} )
LINK_DIRECTORIES( ${extOpenCV2_1_LIB_PATH} )

#MESSAGE("Added ...  ${PROJECT_NAME} code in  ${PROJECT_SOURCE_DIR}")
#add_library(vizcamera src/camerapasimpl.cc src/camerafirewireimpl.cc)

FIND_FILE( UFRAW_BATCH_EXE  "ufraw-batch" "/usr/bin /bin" )
#SET(Path "${inc}" CACHE STRING "*nix only: path to gphoto2 headers")

add_library(imageutils src/imageutils.cc)
target_link_libraries( imageutils cv highgui ${Boost_LIBRARIES} ${boost_filesystem_LIBRARY} ${boost_progam-options_LIBRARY})
IF(UFRAW_BATCH_EXE)
MESSAGE("found  ${UFRAW_BATCH_EXE}")
SET_SOURCE_FILES_PROPERTIES( src/imageutils.cc COMPILE_FLAGS -D'UFRAW_BATCH_EXE="${UFRAW_BATCH_EXE}"')
ELSE(UFRAW_BATCH_EXE)
MESSAGE("ufraw-batch executable not found. Please apt-get install ufraw ")
ENDIF(UFRAW_BATCH_EXE)
