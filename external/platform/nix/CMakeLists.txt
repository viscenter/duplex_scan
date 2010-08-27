
PROJECT(EXTERNAL_PLATFORM)

#MESSAGE("Adding  ... ${PROJECT_NAME} include/library paths in  ${PROJECT_SOURCE_DIR}")
SET(extGphoto2Path "${PROJECT_SOURCE_DIR}/libghoto2.4.10/" CACHE STRING "*nix only: gphoto library path")
#photo
FIND_PATH( p "libgphoto2.so" "${extGphoto2Path}/lib" )
SET(gphoto2LibPath "${p}" CACHE STRING "*nix only: path to -lgphoto2") 
FIND_PATH( pp "libgphoto2_port.so" "${extGphoto2Path}/lib")
SET(gphoto2PortLibPath "${pp}" CACHE STRING "*nix only: path to -lgphoto2_port")
FIND_PATH( inc  "gphoto2/gphoto2-camera.h" "${extGphoto2Path}/include" )
SET(gphoto2IncPath "${inc}" CACHE STRING "*nix only: path to gphoto2 headers")

#opencv
SET(extOpenCVPath "${PROJECT_SOURCE_DIR}/opencv2.1" CACHE STRING "*nix only: opencv library no ffmpeg/dc1394 path")
SET(extOpenCV2_1_LIB_PATH "${extOpenCVPath}/lib" CACHE STRING "*nix only: opencv libraries path no ffmpeg/dc1394 path")
SET(extOpenCV2_1_INCLUDE_PATH "${extOpenCVPath}/include" CACHE STRING "*nix only: opencv include path no ffmpeg/dc1394 path")
