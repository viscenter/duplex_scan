
PROJECT(EXTERNAL_PLATFORM)

#MESSAGE("Adding  ... ${PROJECT_NAME} include/library paths in  ${PROJECT_SOURCE_DIR}")
SET(extGphoto2Path "${PROJECT_SOURCE_DIR}/libghoto2.4.10/" CACHE STRING "*nix only: gphoto library path")
SET(extOpenCVPath "${PROJECT_SOURCE_DIR}/opencv2.1/" CACHE STRING "*nix only: opencv library no fffmpeg/dc1394 path")

SET(extOpenCV2.1_LIBS "cxcore cv highgui" CACHE STRING "*nix only: opencv libraries no fffmpeg/dc1394 path")
