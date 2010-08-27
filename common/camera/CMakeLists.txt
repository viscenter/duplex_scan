PROJECT(VizCamera)

#MESSAGE("${gphoto2LibPath} -> gphoto2LibPath")
#MESSAGE("${gphoto2PortLibPath} -> gphoto2PortLibPath")
#MESSAGE("${gphoto2IncPath} -> gphoto2IncPath")
#SET(extdir "${CMAKE_SOURCE_DIR}/external/platform/nix/libghoto2.4.10/")
INCLUDE_DIRECTORIES( ${extdir}/include )
LINK_DIRECTORIES( ${extdir}/lib )

INCLUDE_DIRECTORIES( ${gphoto2IncPath} )
LINK_DIRECTORIES( ${gphoto2LibPath} ${gphoto2PortLibPath} )

INCLUDE_DIRECTORIES( ${extOpenCV2_1_INCLUDE_PATH} )
LINK_DIRECTORIES( ${extOpenCV2_1_LIB_PATH} )

add_executable(cameraTest test/src/testCamera.cc) 
target_link_libraries( cameraTest userparams vizcamera ${extOpenCV2_1_LIBS})

add_executable(testgphoto test/src/testgphoto.cc) 
target_link_libraries( testgphoto userparams vizcamera )
