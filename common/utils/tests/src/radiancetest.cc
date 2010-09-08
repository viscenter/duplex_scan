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
using namespace boost::filesystem;

using namespace viz;

int main ( int argc, char **argv )
{

	string image1, image2;
	if(argc < 2)
	{
		cerr<<"\nUsage: "<<argv[0] <<" <back lit .pfm> [no back lit .pfm]";
		return EXIT_FAILURE;
	}
	image1 = argv[1];
	if(argc > 2 )
		image2 = argv[2];
	
	IplImage *im, *im2;
	im=0; im2=0;

	cout<<"\nLoading file \""<<image1<<"\" ...";
	im = getIplImageFromPFM(image1);
	if(0 == im)
	{
		cerr<<"\nFailed to load image file "<<endl;
		return -1;
	}
	cout  <<"["<<im->width<<"*"<<im->height<<" * "
			<<im->nChannels<<"] bpp:"<<im->depth<<endl;
#if TEST
	if(writePFM(im, "testout.pfm"))
	{
		cvReleaseImage(&im);
		im =0;
		im = getIplImageFromPFM("testout.pfm");
		if(0 == im)
		{
			cerr<<"\nFailed to load image file "<<endl;
			return -1;
		}
		cout  <<" Success read/write! ["<<im->width<<"*"<<im->height<<" * "
				<<im->nChannels<<"] bpp:"<<im->depth<<endl;
	}
	else
		cerr<<"\nFailed to write out file";
#endif

	if(!image2.empty())
	{
		cout<<"\nLoading file \""<<image2<<"\" ...";
		im2 = getIplImageFromPFM(image2);
		if(0 == im2)
		{
			cerr<<"\nFailed to load image file "<<endl;
			return -1;
		}
		cout  <<"["<<im2->width<<"*"<<im2->height<<" * "
				<<im2->nChannels<<"] bpp:"<<im2->depth<<endl;
	}

	if(im2->imageSize != im->imageSize)
	{
		cerr<<"\nImages are not of the same size";
	}
	else
	{
		IplImage *diffIm= cvCloneImage(im2);
		cvSub(im, im2, diffIm);
		if(!writePFM(diffIm, (image1+"diff"+image2+".pfm").c_str()))
		{
			cerr<<"\nfailed to write out file";
		}

		cvSub(im2, im, diffIm);
		if(!writePFM(diffIm, (image2+"diff"+image1+".pfm").c_str()))
		{
			cerr<<"\nfailed to write out file";
		}

		cvAbsDiff(im, im2, diffIm);
		if(!writePFM(diffIm, (image1+"ABSdiff"+image2+".pfm").c_str()))
		{
			cerr<<"\nfailed to write out file";
		}

		cvReleaseImage(&diffIm);
	}

	cvReleaseImage(&im);
	if(im2)
		cvReleaseImage(&im2);
	return EXIT_SUCCESS;
}

