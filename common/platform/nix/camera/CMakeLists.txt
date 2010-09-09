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
add_library(vizcamera src/camerapasimpl.cc)
target_link_libraries(vizcamera gphoto2 userparams gphoto2_port m imageutils cv highgui )
