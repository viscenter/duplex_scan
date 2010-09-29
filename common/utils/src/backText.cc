
#include "common/utils/src/imageutils.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <boost/filesystem.hpp> 
#include <boost/program_options.hpp> 

using namespace std;
using namespace viz;
using namespace boost::filesystem;
namespace po = boost::program_options;


int main(int argc, char** argv)
{
	if(argc != 4 )
	{
		cerr<<"\nUsage: "<<argv[0] <<"<document> <mask> <outputfile>";
		return EXIT_FAILURE;
	}

	IplImage *doc=0, *mask=0, *result, *tmp;

	//cerr<< "\nreg ";
	if(0 == (doc=getIplImageFromRAW(argv[1], true, 16)))
	{
		//cerr<< "\npfm ";
		if(0 == (doc=getIplImageFromPFM(argv[1])))
		{
			//cerr<< "\nraw ";
			if(0 == (doc=cvLoadImage(argv[1], -1)))
			{
				cerr<<"\nFailed to load image file \""<<argv[1]<<"\"\n";
				return EXIT_FAILURE;
			}
		}
	}

	if(0 == (mask=cvLoadImage(argv[2], -1)))
	{
		cerr<<"\nFailed to load image file \""<<argv[2]<<"\"\n";
		return EXIT_FAILURE;
	}

	result = cvCloneImage(doc);
	cvZero(result);
	cvCopy(doc, result, mask);
	cvFlip(result, 0, 0);

	tmp = cvCloneImage(doc);

	if(result->depth > 8 )
	{
		writePFM(result, (string(argv[3])+".pfm").c_str());
		IplImage *scaled = cvCreateImage(cvSize(doc->width, doc->height), IPL_DEPTH_8U, doc->nChannels);
		scaled->origin = doc->origin;
		cvConvertScaleAbs(result, scaled, 255.0);
		cvSaveImage("resultDisplay.png", scaled);
		cvConvertScaleAbs(doc, scaled, 255.0);
		cvSaveImage("docDisplay.png", scaled);
	}
	else
		cvSaveImage(argv[3], result);
}
