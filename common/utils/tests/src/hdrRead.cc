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
	//cerr<< "\nreg ";
	if(0 == (im=getIplImageFromRAW(filename.c_str(), true, 16)))
	{
		//cerr<< "\npfm ";
		if(0 == (im=getIplImageFromPFM(filename.c_str())))
		{
			if(0 == (im=cvLoadImage(filename.c_str(), -1)))
			//cerr<< "\nraw ";
			{
				cerr<<"\nFailed to load image file \""<<filename<<"\"\n";
				return EXIT_FAILURE;
			}
		}
	}

	cout  << IplImageToString(im) <<endl;

	if(display)
	{
		cvNamedWindow( "My Window", 0 );
		cvShowImage( "My Window", im );
		cvWaitKey();
		cvDestroyWindow("My Window");
	}
	cvReleaseImage(&im);
	return 0;
}
