
#include <common/camera/src/camerapasimpl.h>
#include <string>
#include <iostream>
#include <opencv/cv.h>

using namespace std;
using namespace viz;

int main(int argc, char ** argv)
{

	CameraPASImpl *camera =  CameraPASImpl::getInstance();
	if(!camera->initialize())
	{
		return EXIT_FAILURE;
	}
	cout <<"\nCreate a camera with name \""<<camera->getName()<<"\"";

	if(!camera->finalize())
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
