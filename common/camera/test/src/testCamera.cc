
#include <common/camera/src/camerapasimpl.h>
#include <common/type/src/userparamsimpl.h>
//#include <common/camera/src/camerafirewireimpl.h>
#include <string>
#include <iostream>
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace viz;

int main(int argc, char ** argv)
{

	bool failure = false;
	vector<ICamera*> cameras;
	//cameras.push_back( CameraFirewireImpl::getInstance());
	cameras.push_back( CameraPASImpl::getInstance());
/*
	IplImage *im2 = cvLoadImage("test.jpg", -1); 
	cvNamedWindow("display", CV_WINDOW_AUTOSIZE);
	cvShowImage("display", im2);
	cvWaitKey();
	cvDestroyWindow("display");
	cvReleaseImage(&im2);
*/
	for(vector<ICamera*>::iterator it = cameras.begin(); it != cameras.end();
			++it)
	{
		ICamera::CameraType ct = (*it)->getType();
		string ctn = ICamera::getTypeName(ct);
		if(!(*it)->initialize())
		{
			cerr<<"\nFailed to initialize "<<ctn<<" camera";
			failure = true;
			continue;
		}
		else
			cout <<"\nInitialized a camera type "<< ctn 
				  << "with name \""<<(*it)->getName()<<"\"";

		UserParamsSetShutterImpl upss("1/10"); 
		if(!(*it)->setShutterSpeed(upss))
		{
			cerr<<"\nFailed to setShutter speed choice ("
				 <<upss.getValue()<<") " <<ctn<<" camera";
			failure = true;
		}

		IplImage *im = 0;
		string fname("test.jpg");
		UserParamsGetImageImpl upgi(fname, &im, true ); 
		if(!(*it)->getImage(upgi))
		{
			cerr<<"\nFailed to getImage "<<ctn<<" camera";
			failure = true;
		}
		else
		{
			im = *upgi.getImage();
			cvNamedWindow("display", CV_WINDOW_AUTOSIZE);
			cvShowImage("display", im);
			cvWaitKey();
			cvDestroyWindow("display");
			cvReleaseImage(&im);
		}

		if(!(*it)->finalize())
		{
			cerr<<"\nFailed to cleanup "<< ctn <<" camera";
			failure = true;
		}
		else
			cout<<"\nCleaned up "<< ctn <<" camera";;

	}

	return failure;
	return EXIT_SUCCESS;
}
