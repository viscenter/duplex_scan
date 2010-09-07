
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "boost/filesystem.hpp" 

#define TEST 0
using namespace std;
using namespace boost::filesystem;

int main ( int argc, char **argv )
{

	string backName, foreName;
	if(argc < 3)
	{
		cerr<<"\nUsage: "<<argv[0] <<" <back_lit.raw> <fore_lit.raw>";
		return EXIT_FAILURE;
	}
	backName = argv[1];
	foreName = argv[2];
	
	IplImage *back, *fore;
	back=0; fore=0;

	cout<<"\nLoading file \""<<backName<<"\" ...";
	cout<<"\nLoading file \""<<foreName<<"\" ...";
	if(back->imageSize != fore->imageSize)
	{
		cerr<<"\nImages are not of the same size";
	}
	else
	{
		IplImage *diffIm= cvCloneImage(fore);
		cvSub(back, fore, diffIm);


		cvReleaseImage(&diffIm);
	}

	cvReleaseImage(&back);
	cvReleaseImage(&fore);
	return EXIT_SUCCESS;
}
