PROJECT(Cameras)

MESSAGE("Added ...  ${PROJECT_NAME} code in  ${PROJECT_SOURCE_DIR}")
add_library(vizcamera src/camerapasimpl.cc)
target_link_libraries(vizcamera gphoto2 gphoto2_port m)
