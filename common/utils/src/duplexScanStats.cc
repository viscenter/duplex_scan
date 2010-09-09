
#include "common/utils/src/imageutils.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "boost/filesystem.hpp" 

#define TEST 0
using namespace std;
using namespace viz;
using namespace boost::filesystem;

int main ( int argc, char **argv )
{

	string backName, foreName, docName;
	int bpp=12;
	if(argc < 4)
	{
		cerr<<"\nUsage: "<<argv[0] <<" <back_lit.raw> <fore_lit.raw> <document.raw> [bpp]\n" ;
		return EXIT_FAILURE;
	}

	backName = argv[1];
	foreName = argv[2];
	docName = argv[3];

	if(argc  == 5)
	{
		stringstream ss(argv[4]);
		ss >> bpp;
		if(ss.bad())
		{
			cerr<<"\nFailure converting bpp";
			return EXIT_FAILURE;
		}
	}

	
	IplImage *back, *fore, *doc;
	back=0; fore=0; doc=0;

	cout<<"\nLoading file \""<<backName<<"\" ...";
	back = getIplImageFromRAW(backName, false, 12);
	if(!back)
	{
		cerr<<"\nFailed to load back lit image";
		return EXIT_FAILURE;
	}
	cout  <<"["<<back->width<<"*"<<back->height<<" * "
			<<back->nChannels<<"] bpp:"<<back->depth<<endl;

	cout<<"\nLoading file \""<<foreName<<"\" ...";
	fore = getIplImageFromRAW(foreName, false, 12);
	if(!fore)
	{
		cerr<<"\nFailed to load fore lit image";
		return EXIT_FAILURE;
	}
	cout  <<"["<<fore->width<<"*"<<fore->height<<" * "
			<<fore->nChannels<<"] bpp:"<<fore->depth<<endl;
	
	cout<<"\nLoading file \""<<docName<<"\" ...";
	doc = getIplImageFromRAW(docName, false, 12);
	if(!doc)
	{
		cerr<<"\nFailed to load doc lit image";
		return EXIT_FAILURE;
	}
	cout  <<"["<<doc->width<<"*"<<doc->height<<" * "
			<<doc->nChannels<<"] bpp:"<<doc->depth<<endl;

	if((back->imageSize != fore->imageSize) || (fore->imageSize !=  doc->imageSize))
	{
		cerr<<"\nImages are not of the same size";
	}
	else
	{
		IplImage *delta= cvCloneImage(fore);
		IplImage *deltaAbs= cvCloneImage(fore);
		cvSub(back, fore, delta);
		cvAbsDiff(back, fore, deltaAbs);
	
		IplImage *diffIm= cvCloneImage(doc);
		IplImage *diffImAbs= cvCloneImage(doc);
		cvSub(delta, doc, diffIm);
		cvAbsDiff(delta, doc, diffImAbs);

		if(!writePFM(diffIm, "back.pfm") || !writePFM(delta, "delta.pfm") ||
			!writePFM(diffImAbs, "backAbs.pfm") || !writePFM(deltaAbs, "deltaAbs.pfm"))
		{
			cerr<<"\nFailed to write back,delta pfm's";
		}

		IplImage *scaled = cvCreateImage(cvSize(back->width, back->height), IPL_DEPTH_8U, back->nChannels);
		cvConvertScaleAbs(diffIm, scaled, 255.0);
		cvSaveImage("scaledBack.ppm", scaled);
		cvConvertScaleAbs(diffImAbs, scaled, 255.0);
		cvSaveImage("scaledAbsBack.ppm", scaled);
		cvConvertScaleAbs(delta, scaled, 255.0);
		cvSaveImage("delta.ppm", scaled);
		cvConvertScaleAbs(deltaAbs, scaled, 255.0);
		cvSaveImage("deltaAbs.ppm", scaled);



		cvReleaseImage(&scaled);


		cvReleaseImage(&diffIm);
		cvReleaseImage(&diffImAbs);
		cvReleaseImage(&delta);
		cvReleaseImage(&deltaAbs);
	}

	cvReleaseImage(&back);
	cvReleaseImage(&fore);
	cvReleaseImage(&doc);
	return EXIT_SUCCESS;
}
