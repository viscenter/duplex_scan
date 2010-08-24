PROJECT(Cameras)

FIND_LIBRARY( GPHOTO2_LIBRARY gphoto2 )
FIND_LIBRARY( GPHOTO2_PORT_LIBRARY gphoto2_port )
FIND_FILE( GPHOTO2_HEADERS  "gphoto2-camera.h" /usr/include )
FIND_LIBRARY( DC1394_LIBRARY dc1394 )
FIND_FILE( DC1394_HEADERS  "dc1394.h" /usr/include  )

MESSAGE( "ghoto2 lib at \"${GPHOTO2_LIBRARY}\"")
MESSAGE( "ghoto2_port lib at \"${GPHOTO2_PORT_LIBRARY}\"")
MESSAGE( "ghoto2.h at \"${GPHOTO2_HEADERS}\"")
MESSAGE( "dc1394 lib at \"${DC1394_LIBRARY}\"")
MESSAGE( "dc1394.h at \"${DC1394_HEADERS}\"")


MESSAGE("Added ...  ${PROJECT_NAME} code in  ${PROJECT_SOURCE_DIR}")
add_library(vizcamera src/camerapasimpl.cc src/camerafirewireimpl.cc)
target_link_libraries(vizcamera gphoto2 gphoto2_port m dc1394)
