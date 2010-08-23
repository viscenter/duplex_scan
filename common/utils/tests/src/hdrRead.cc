#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <iostream>

using namespace std;

int main ( int argc, char **argv )
{
	string filename("../resources/test.tiff");

	if(argc == 2)
		filename = argv[1];

	cout<<"Loading file \""<<filename<<"\"" <<endl;
	IplImage *im = cvLoadImage(filename.c_str(), -1);
	if(0 == im)
	{
		cerr<<"\nFailed to load image file "<<endl;
		return -1;
	}
	cout  <<"["<<im->width<<"*"<<im->height<<" * "
			<<im->nChannels<<"] bpp:"<<im->depth<<endl;








   cvNamedWindow( "My Window", 1 );
	cvShowImage( "My Window", im );
	cvWaitKey();
	cvDestroyWindow("My Window");
	cvReleaseImage(&im);
	return 0;
}
