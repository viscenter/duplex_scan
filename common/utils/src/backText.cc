
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
	string bmfilename, dfilename, fmfilename, ofilename;
	int bpp;
	int flip;
	bool noFlip =false;

	po::options_description desc("Allowed options");
	desc.add_options()
   ("help,h", "help message")
   ("document,d", po::value<string>(&dfilename),"backlit document")
   ("background-mask,b", po::value<string>(&bmfilename),"background mask file")
   ("foreground-mask,f", po::value<string>(&fmfilename),"[foreground mask file]")
   ("output-file,o", po::value<string>(&ofilename),"output filename (don't include extension)")
   ("bpp", po::value<int>(&bpp)->default_value(16),"bits per pixel for raw")
   ("flip", po::value<int>(&flip)->default_value(0),
	 "flip the final result:\n0 - x-axis\n> 0 - y-axis\n< 0 x/y-axis")
   ("no-flip", "do not flip the result");

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		 cout << desc << endl;
		 return EXIT_FAILURE;
	}
	
	if(vm.count("background-mask") == 0)
	{
		 cout <<"\nYou have to specify a background mask\n"<< desc << endl;
		 return EXIT_FAILURE;
	}
	
	if(vm.count("output-file") == 0)
	{
		 cout <<"\nYou have to specify an output file \n"<< desc << endl;
		 return EXIT_FAILURE;
	}
	
	noFlip =  (vm.count("no-flip") > 0);
		


	IplImage *doc=0, *fmask=0, *bmask=0, *result, *tmp;

	if(0 == (doc=getIplImageFromRAW(dfilename, true, bpp)))
	{
		if(0 == (doc=getIplImageFromPFM(dfilename)))
		{
			if(0 == (doc=cvLoadImage(dfilename.c_str(), -1)))
			{
				cerr<<"\nFailed to load image file \""<<dfilename<<"\" as the document\n";
				return EXIT_FAILURE;
			}
		}
	}

	if(0 == (bmask=cvLoadImage(bmfilename.c_str(), -1)))
	{
		cerr<<"\nFailed to load image file \""<<bmfilename<<"\" as the background mask\n";
		return EXIT_FAILURE;
	}

	if(vm.count("foreground-mask") != 0)
	{
		if(0 == (fmask=cvLoadImage(fmfilename.c_str(), -1)))
		{
			cerr<<"\nFailed to load image file \""<<fmfilename<<"\" as the foreground mask\n";
			return EXIT_FAILURE;
		}
	}

	result = cvCloneImage(doc);
	cvZero(result);
	tmp = cvCloneImage(bmask);
	cvSaveImage("_001.png", bmask);
	cvSaveImage("_002.png", fmask);
	if(fmask)
	{
		IplConvKernel* k =cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);
		IplImage *morph = cvCloneImage(fmask);
		cvZero(morph);
		IplImage *dia = cvCloneImage(fmask);
		IplImage *tmp2 = cvCloneImage(fmask);
		cvMorphologyEx(fmask, morph, 0, k, CV_MOP_CLOSE, 10);
		//cvSaveImage("foremopClose.png", morph);
		cvSaveImage("_003.png", morph);
		cvDilate(morph, dia, 0, 13); 

		//only foregorund
		cvOr(dia, fmask, tmp2);
		//cvSaveImage("foreEx.png", tmp2);
		//cvSaveImage("_002.png", tmp2);

		//foreground in background mask
		cvZero(tmp);
		cvCopy(bmask, tmp, tmp2);
		//cvSaveImage("foreExInBack.png", tmp);
		cvSaveImage("_004.png", tmp);


		//remove foreground from back
		cvZero(morph);
		cvXor(bmask, tmp, morph);
		//cvSaveImage("backSubForeExInBack.png", morph);
		cvSaveImage("_005.png", morph);

		//morph results
		cvZero(tmp);
		cvMorphologyEx(morph, tmp, 0, k, CV_MOP_CLOSE, 2);
		//cvSaveImage("backSubForeExInBackOPENED.png", tmp);
		cvSaveImage("_006.png", tmp);
		cvCopy(tmp, morph);


		cvZero(result);
		cvCopy(doc, result, morph);
		
	}
	else
		cvCopy(doc, result, tmp);

	if(!noFlip)
		cvFlip(result, 0, flip);

	tmp = cvCloneImage(doc);

	if(result->depth > 8 )
	{
		writePFM(result, (ofilename+".pfm").c_str());
		IplImage *scaled = cvCreateImage(cvSize(doc->width, doc->height), IPL_DEPTH_8U, doc->nChannels);
		scaled->origin = doc->origin;
		cvConvertScaleAbs(result, scaled, 255.0);
		//cvSaveImage("resultDisplay.png", scaled);
		cvConvertScaleAbs(doc, scaled, 255.0);
		//cvSaveImage("docDisplay.png", scaled);
		cout<<"\nResult saved as \""<<(ofilename+".pfm")<<"\""<<endl;
	}
	else
	{
		cvSaveImage((ofilename+".png").c_str(), result);
		cout<<"\nResult saved as \""<<(ofilename+".png")<<"\""<<endl;
	}


	cvReleaseImage(&result);
	cvReleaseImage(&doc);
	cvReleaseImage(&tmp);
	cvReleaseImage(&fmask);
	cvReleaseImage(&bmask);

	return EXIT_SUCCESS;
}
