#include<iostream>
#include<string>
#include <gphoto2/gphoto2-camera.h>

using namespace std;

int main(int argv, char **argc)
{
	Camera *gCamera;
	GPContext *gContext = gp_context_new (); /* see context.c */
	gp_camera_new (&gCamera);
	int err;
	if( (err= gp_camera_init(gCamera, gContext))  != GP_OK )
	{
		cerr<<"\nFailed to initialize camera ("<<err<<")\n";
		gp_camera_free (gCamera);
		return 1;
	}

	/* Simple query the camera summary text */
	CameraText	text;
	if(gp_camera_get_summary (gCamera, &text, gContext) < GP_OK)
	{
		cerr<<"Camera failed retrieving summary.("<<err<<")\n";
		gp_camera_free (gCamera);
		return 1;
	}
	string sum = text.text;
	cout <<"\nCamera Summary:" <<endl <<sum<<endl;

	/* Simple query of a string configuration variable. */
	if(0 != gCamera && 0 != gContext)
	{
		gp_camera_exit (gCamera, gContext);
		gp_camera_free (gCamera);
	}
	else
	{
		cerr<<"\nFailed finalization. Camera not initialized correctly.("<<err<<")\n";
		return 1;
	}

	return 0;
}
