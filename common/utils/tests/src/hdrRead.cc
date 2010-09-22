#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <iostream>

#include <common/utils/src/imageutils.h>

using namespace std;
using namespace viz;

int main ( int argc, char **argv )
{
	string filename;

	if(argc > 1)
		filename = argv[1];
	bool display  = (argc >2);

	cout<<"Loading file \""<<filename<<"\"" <<endl;
	IplImage *im = 0; 
	if(0 == (im=cvLoadImage(filename.c_str(), -1)) && 
		0 == (im=getIplImageFromPFM(filename.c_str())) && 
		0 == (im=getIplImageFromRAW(filename.c_str(), true, 16)))
	{
		cerr<<"\nFailed to load image file "<<endl;
		return -1;
	}

	cout  << IplImageToString(im) <<endl;

	if(display)
	{
		cvNamedWindow( "My Window", 1 );
		cvShowImage( "My Window", im );
		cvWaitKey();
		cvDestroyWindow("My Window");
	}
	cvReleaseImage(&im);
	return 0;
}
