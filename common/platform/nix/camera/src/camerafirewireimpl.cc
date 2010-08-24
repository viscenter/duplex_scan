
#include <string>
#include <iostream>
#include <memory>

#include <common/camera/src/camerafirewireimpl.h>


using namespace viz;
using namespace std;


CameraFirewireImpl::CameraFirewireImpl() 
	: dcHandle(0), dcList(0), dcCamera(0)
{
#if DEBUG
	cout <<"\nCameraFirewireImpl()";
#endif
	
}

CameraFirewireImpl::~CameraFirewireImpl()
{
#if DEBUG
	cout <<"\n~CameraFirewireImpl()";
#endif
}

bool CameraFirewireImpl::initialize()
{
	dcHandle = dc1394_new();
	if((dcerr = dc1394_camera_enumerate(dcHandle, &dcList)) != DC1394_SUCCESS)
	{
		cerr<<"\n\tFailed to get a camera list";
		dc1394_free(dcHandle);
		dcHandle = 0;
		return false;
	}
#if DEBUG
	else
	{
		cout <<"\n\tGot camera list";
	}
#endif

	if(dcList->num == 0 )
	{
		cerr<<"\n\tNo cameras Found";
		dc1394_camera_free_list(dcList);
		dcList =0;
		dc1394_free(dcHandle);
		dcHandle = 0;
		return false;
	}
#if DEBUG
	else
	{
		cout <<"\n\tFound "<<dcList->num<< " cameras. Going to use the first one";
	}
#endif

	dcCamera = dc1394_camera_new(dcHandle, dcList->ids[0].guid);
	if( !dcCamera)
	{
		cerr<<"\n\tFailed to connect to camera";
		dc1394_camera_free_list(dcList);
		dcList = 0;
		dc1394_free(dcHandle);
		dcHandle = 0;
		return false;
	}
#if DEBUG
	else
	{
		cout <<"\n\tConnected to the camera.";
		dc1394_camera_print_info(dcCamera, stdout);
	}
#endif

	name = dcCamera->vendor;
	name += ":";
	name +=  dcCamera->model;

	return true;
}
bool CameraFirewireImpl::finalize()
{
	if(dcList)
	{
		dc1394_camera_free_list(dcList);
		dcList = 0;
	}

	if(dcCamera)
	{
		dc1394_video_set_transmission(dcCamera, DC1394_OFF);
		dc1394_capture_stop(dcCamera);
		dc1394_camera_free(dcCamera);
		dcCamera = 0;
	}

	if(dcHandle)
	{
		dc1394_free(dcHandle);
		dcHandle = 0;
	}

	return true;
}
bool CameraFirewireImpl::getImage(UserParams &p)
{
	return true;
}
bool CameraFirewireImpl::setShutterSpeed(UserParams &p)
{
	return true;
}
std::string  CameraFirewireImpl::getName() const
{
	return name;
}

ICamera::CameraType CameraFirewireImpl::getType() const
{
	if(dcCamera)
	{
		return ((dcCamera->bmode_capable == DC1394_TRUE)?FIREWIRE_A:FIREWIRE_B);
	}
	else
		return UNKNOWN;
}
