PROJECT(Cameras)

MESSAGE( "ghoto2 Path at \"${extGphoto2Path}\"")
FIND_PATH( p "libgphoto2.so" "${extGphoto2Path}/lib" )
SET(gphoto2LibPath "${p}" CACHE STRING "*nix only: path to -lgphoto2") 

FIND_PATH( pp "libgphoto2_port.so" "${extGphoto2Path}/lib")
SET(gphoto2PortLibPath "${pp}" CACHE STRING "*nix only: path to -lgphoto2_port")

FIND_PATH( inc  "gphoto2/gphoto2-camera.h" "${extGphoto2Path}/include" )
SET(gphoto2IncPath "${inc}" CACHE STRING "*nix only: path to gphoto2 headers")

#MESSAGE( "ghoto2 lib at \"${gphoto2LibPath}\"")
#MESSAGE( "ghoto2_port lib at \"${gphoto2PortLibPath}\"")
#MESSAGE( "ghoto2.h at \"${gphoto2IncPath}\"")

INCLUDE_DIRECTORIES( ${gphoto2IncPath} )
LINK_DIRECTORIES( ${gphoto2LibPath} ${gphoto2PortLibPath} )

MESSAGE("Added ...  ${PROJECT_NAME} code in  ${PROJECT_SOURCE_DIR}")
#add_library(vizcamera src/camerapasimpl.cc src/camerafirewireimpl.cc)
add_library(vizcamera src/camerapasimpl.cc)
target_link_libraries(vizcamera gphoto2 gphoto2_port m )
