
#include <common/camera/src/camerapasimpl.h>
#include <common/type/src/userparamsimpl.h>
#include <string>
#include <iostream>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace viz;

int main(int argc, char ** argv)
{

	if(argc < 2 )
	{
		cerr<<"\nUsage: "<<argv[0]<<" <outputImage> [<shutterSpeed>]\n";
		return -1;
	}


	CameraPASImpl * camera =  CameraPASImpl::getInstance();
	if(!camera->initialize())
	{
		cerr<<"\nFailed to initialize camera";
	}
	cout <<"\nInitialized a camera type "<<camera->getType()
				  << " named \""<<camera->getName()<<"\"";
	
	if(argc > 2)
	{
		UserParamsSetShutterImpl upss(argv[2]); 
		if(!camera->setShutterSpeed(upss))
		{
			cerr<<"\nFailed to setShutter speed ("
				 <<upss.getValue()<<")";
			camera->finalize();
			return -1;
		}
	}
	else
	{
		cout <<"\nNo shutterspeed specified";
	}

	IplImage *im = 0;
	string fname(argv[1]);
	UserParamsGetImageImpl upgi(fname, &im, false ); 
	if(!camera->getImage(upgi))
	{
		cerr<<"\nFailed to getImage \""<<argv[1]<<" camera";
	}
	else
	{
		cout<<"\nImage \""<<argv[1]<<" saved";
	}

	if(!camera->finalize())
	{
		cerr<<"\nFailed to cleanup  camera";
	}

	return EXIT_SUCCESS;
}
