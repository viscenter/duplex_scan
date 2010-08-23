PROJECT(VizCamera)
FIND_PACKAGE(OpenCV REQUIRED)

add_executable(cameraTest test/src/testCamera.cc) 
target_link_libraries( cameraTest userparams vizcamera ${OpenCV_LIBRARIES})

