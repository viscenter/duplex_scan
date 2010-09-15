
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

/**
 * @todo use boost_program options
 * @todo ROI information
 * @todo silent option
 * @todo dump intermediates option
 * @bug flip after pfm converstion
 */

int main ( int argc, char **argv )
{

	string backName, foreName, docName;
	int bpp=16;
	if(argc < 4)
	{
		cerr<<"\nUsage: "<<argv[0] <<" <backLightOnlyNoDoc.raw> <docBacklightON.raw> <docBacklightOFF.raw> [bpp]\n" ;
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
	CvScalar mean, std;
	double min, max;

	cout<<"\nLoading file \""<<backName<<"\" ... as the backLight only image";
	back = getIplImageFromRAW(backName, false, bpp);
	if(!back)
	{
		cerr<<"\nFailed to load back lit image";
		return EXIT_FAILURE;
	}
	cout  <<"["<<back->width<<"*"<<back->height<<" * "
			<<back->nChannels<<"] bpp:"<<back->depth<<endl;
	cvAvgSdv(back, &mean, &std); cvMinMaxLoc(back, &min, &max);
	cout <<"\nbacklight min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];

	cout<<"\nLoading file \""<<foreName<<"\" ... as the documentWith backlight";
	fore = getIplImageFromRAW(foreName, false, bpp);
	if(!fore)
	{
		cerr<<"\nFailed to load fore lit image";
		return EXIT_FAILURE;
	}
	cout  <<"["<<fore->width<<"*"<<fore->height<<" * "
			<<fore->nChannels<<"] bpp:"<<fore->depth<<endl;
	cvAvgSdv(fore, &mean, &std); cvMinMaxLoc(fore, &min, &max);
	cout <<"\ndocbacklight min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
	
	cout<<"\nLoading file \""<<docName<<"\" ... as the document without backlight";
	doc = getIplImageFromRAW(docName, false, bpp);
	if(!doc)
	{
		cerr<<"\nFailed to load doc lit image";
		return EXIT_FAILURE;
	}
	cout  <<"["<<doc->width<<"*"<<doc->height<<" * "
			<<doc->nChannels<<"] bpp:"<<doc->depth<<endl;
	cvAvgSdv(doc, &mean, &std); cvMinMaxLoc(doc, &min, &max);
	cout <<"\ndoc only min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];

	if((back->imageSize != fore->imageSize) || (fore->imageSize !=  doc->imageSize))
	{
		cerr<<"\nImages are not of the same size";
	}
	else
	{

		IplImage *variance= cvCloneImage(fore);
		IplImage *varianceAbs= cvCloneImage(fore);

		cvSub(back, fore, variance);
		cvAvgSdv(variance, &mean, &std); cvMinMaxLoc(variance, &min, &max);
		cout <<"\nvariance min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
		/********/
		cvAbsDiff(back, fore, varianceAbs);
		cvAvgSdv(varianceAbs, &mean, &std); cvMinMaxLoc(varianceAbs, &min, &max);
		cout <<"\nvarianceAbs min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
		/********/

		IplImage *diffIm= cvCloneImage(doc);
		IplImage *diffImAbs= cvCloneImage(doc);

		cvSub(variance, doc, diffIm);
		cvAvgSdv(diffIm, &mean, &std); cvMinMaxLoc(diffIm, &min, &max);
		cout <<"\ndiffIm min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0];
		/********/
		cvAbsDiff(variance, doc, diffImAbs);
		cvAvgSdv(diffImAbs, &mean, &std); cvMinMaxLoc(diffImAbs, &min, &max);
		cout <<"\ndiffImAbs min:"<<min <<" max:"<<max<<" mean:"<<mean.val[0]<<" std:"<<std.val[0]<<endl;
		/********/
		if(!writePFM(diffIm, "back.pfm") || !writePFM(variance, "variance.pfm") ||
			!writePFM(diffImAbs, "backAbs.pfm") || 
			!writePFM(varianceAbs, "varianceAbs.pfm") || 
			!writePFM(doc, "doc.pfm") ||
			!writePFM(back, "back.pfm") ||
			!writePFM(fore, "fore.pfm"))
		{
			cerr<<"\nFailed to dump raw pfm's to disk";
		}

		IplImage *scaled = cvCreateImage(cvSize(back->width, back->height), IPL_DEPTH_8U, back->nChannels);
		cvConvertScaleAbs(diffIm, scaled, 255.0);
		cvSaveImage("dpScaledBack.ppm", scaled);
		cvConvertScaleAbs(diffImAbs, scaled, 255.0);
		cvSaveImage("dpScaledAbsBack.ppm", scaled);
		cvConvertScaleAbs(variance, scaled, 255.0);
		cvSaveImage("dpDeltaScaled.ppm", scaled);
		cvConvertScaleAbs(varianceAbs, scaled, 255.0);
		cvSaveImage("dpDeltaAbsScaled.ppm", scaled);

		cvSaveImage("variance.ppm", variance);
		cvSaveImage("doc.ppm", doc);

		cvConvertScaleAbs(back, scaled, 255.0);
		cvSaveImage("dpBack.ppm", scaled);
		cvConvertScaleAbs(fore, scaled, 255.0);
		cvSaveImage("dpFore.ppm", scaled);
		cvConvertScaleAbs(doc, scaled, 255.0);
		cvSaveImage("dpDoc.ppm", scaled);


		cvReleaseImage(&scaled);


		cvReleaseImage(&diffIm);
		cvReleaseImage(&diffImAbs);
		cvReleaseImage(&variance);
		cvReleaseImage(&varianceAbs);
	}

	cvReleaseImage(&back);
	cvReleaseImage(&fore);
	cvReleaseImage(&doc);
	return EXIT_SUCCESS;
}
