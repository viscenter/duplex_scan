#include <common/type/src/userparamsimpl.h>
#include <string>
#include <iostream>
#include <opencv/cv.h>

using namespace std;
using namespace viz;

int main(int argc, char ** argv)
{

	IplImage *im = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	string filename("testFile"); 

	UserParamsGetImageImpl gip(filename, &im);
	if( filename != gip.getFilename() && im != gip.getImage() )
	{
		cerr	<<"\nUserParamsGetImageImpl accessor failure " 
				<<__FILE__<<":"<<__LINE__;
		cvReleaseImage(&im);
		return EXIT_FAILURE;
	}
	cvReleaseImage(&im);

	string val("50");
	UserParamsSetShutterImpl sip(val);
	if(  val != sip.getValue() )
	{
		cerr	<<"\nUserParamsGetShutterImpl accessor failure " 
				<<__FILE__<<":"<<__LINE__;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
