
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
		CvScalar mean, std;
		double min, max;

		IplImage *delta= cvCloneImage(fore);
		IplImage *deltaAbs= cvCloneImage(fore);

		cvSub(back, fore, delta);
		cvAvgSdv(delta, &mean, &std); cvMinMaxLoc(delta, &min, &max);
		cout <<"\ndelta min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
		/********/
		cvAbsDiff(back, fore, deltaAbs);
		cvAvgSdv(deltaAbs, &mean, &std); cvMinMaxLoc(deltaAbs, &min, &max);
		cout <<"\ndeltaAbs min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
		/********/

		IplImage *diffIm= cvCloneImage(doc);
		IplImage *diffImAbs= cvCloneImage(doc);

		cvSub(delta, doc, diffIm);
		cvAvgSdv(diffIm, &mean, &std); cvMinMaxLoc(diffIm, &min, &max);
		cout <<"\ndiffIm min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
		/********/
		cvAbsDiff(delta, doc, diffImAbs);
		cvAvgSdv(diffImAbs, &mean, &std); cvMinMaxLoc(diffImAbs, &min, &max);
		cout <<"\ndiffImAbs min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0]<<endl;
		/********/
		if(!writePFM(diffIm, "back.pfm") || !writePFM(delta, "delta.pfm") ||
			!writePFM(diffImAbs, "backAbs.pfm") || !writePFM(deltaAbs, "deltaAbs.pfm"))
		{
			cerr<<"\nFailed to write back,delta pfm's";
		}

		IplImage *scaled = cvCreateImage(cvSize(back->width, back->height), IPL_DEPTH_8U, back->nChannels);
		cvConvertScaleAbs(diffIm, scaled, 255.0);
		cvSaveImage("dpScaledBack.ppm", scaled);
		cvConvertScaleAbs(diffImAbs, scaled, 255.0);
		cvSaveImage("dpScaledAbsBack.ppm", scaled);
		cvConvertScaleAbs(delta, scaled, 255.0);
		cvSaveImage("dpDelta.ppm", scaled);
		cvConvertScaleAbs(deltaAbs, scaled, 255.0);
		cvSaveImage("dpDeltaAbs.ppm", scaled);

		cvConvertScaleAbs(back, scaled, 255.0);
		cvSaveImage("dpBack.ppm", scaled);
		cvConvertScaleAbs(fore, scaled, 255.0);
		cvSaveImage("dpFore.ppm", scaled);
		cvConvertScaleAbs(doc, scaled, 255.0);
		cvSaveImage("dpDoc.ppm", scaled);


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
