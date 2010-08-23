
#include <string>
#include <iostream>
#include <memory>

#include <common/camera/src/camerapasimpl.h>


using namespace viz;
using namespace std;


CameraPASImpl::CameraPASImpl() 
	: gCamera(0), gContext(0)
{
#if DEBUG
	cout <<"\nCameraPASImpl()";
#endif
	
}

CameraPASImpl::~CameraPASImpl()
{
#if DEBUG
	cout <<"\n!CameraPASImpl()";
#endif
}
extern "C" 
{
bool CameraPASImpl::initialize()
{
	gContext = gp_context_new (); /* see context.c */
	gp_camera_new (&gCamera);
	if( gp_camera_init(gCamera, gContext)  < GP_OK )
	{
		cerr<<"\nFailed to initialize camera ";
		gp_camera_free (gCamera);
		return false;
	}

	/* Simple query the camera summary text */
	CameraText	text;
	if(gp_camera_get_summary (gCamera, &text, gContext) < GP_OK)
	{
		cerr<<"Camera failed retrieving summary.\n";
		gp_camera_free (gCamera);
		return false;
	}
	name = text.text;

	/* Simple query of a string configuration variable. */

	return true;
}
bool CameraPASImpl::finalize()
{
	if(0 != gCamera && 0 != gContext)
	{
		gp_camera_exit (gCamera, gContext);
		gp_camera_free (gCamera);
		return true;
	}
	else
	{
		cerr<<"\nFailed finalization. Camera not initialized correctely.";
		return false;
	}
}
}
bool CameraPASImpl::getImage(UserParams &p)
{
	return true;
}
bool CameraPASImpl::setShutterSpeed(UserParams &p)
{
	return true;
}
std::string  CameraPASImpl::getName() const
{
	return name;
}

ICamera::CameraType CameraPASImpl::getType() const
{
	return POINT_AND_SHOOT;
}
