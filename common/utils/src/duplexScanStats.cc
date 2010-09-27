
#include "common/utils/src/imageutils.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp> 
#include <boost/program_options.hpp> 

#define TEST 0
using namespace std;
using namespace viz;
using namespace boost::filesystem;
namespace po = boost::program_options;

/**
 * @todo ROI information
 */

int main ( int argc, char **argv )
{
	string backName, foreName, docName;
	int bpp;
	bool verb, saveInter, plots;
	po::options_description desc("Allowed options");
	desc.add_options()
   ("help,h", "help message")
   ("backlight-only,b", po::value<string>(&backName),"backlight only")
   ("doc-backlit,d", po::value<string>(&foreName),"document with backlight")
   ("doc-no-backlight,n", po::value<string>(&docName),"document without backlight")
   ("bits-per-pixel", po::value<int>(&bpp)->default_value(16),
	 "bpp for a rawImage")
   //("verbose,v", po::value<bool>(&verb)->default_value(false),"verbose output")
   ("verbose,v", "verbose output")
   ("generate-plots,p", "generate gnu plot files of images backText & variance")
   ("save-intermediates,s", "save intermediate images");

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		 cout << desc << "\n";
		 return EXIT_FAILURE;
	}

	if (!vm.count("back")||!vm.count("fore")||!vm.count("doc")) {
		 cout << desc << "\n";
		 return EXIT_FAILURE;
	}

	verb = (vm.count("verbose") > 0);
	plots = (vm.count("generate-plots") > 0);
	saveInter = (vm.count("save-intermediates") > 0);

	IplImage *back, *fore, *doc;
	back=0; fore=0; doc=0;

	if(verb)
		cout<<"\nLoading file \""<<backName<<"\" ... as the backLight only image";
	back = getIplImageFromRAW(backName, false, bpp);

	if(!back)
	{
		cerr<<"\nFailed to load back lit image";
		return EXIT_FAILURE;
	}

	if(verb)
	{
		cout<<IplImageToString(back) <<endl;
	}

	if(verb)
		cout<<"\nLoading file \""<<foreName<<"\" ... as the documentWith backlight";
	fore = getIplImageFromRAW(foreName, false, bpp);
	if(!fore)
	{
		cerr<<"\nFailed to load fore lit image";
		return EXIT_FAILURE;
	}

	if(verb)
	{
		cout<<IplImageToString(fore) <<endl;
	}
	
	if(verb)
		cout<<"\nLoading file \""<<docName<<"\" ... as the document without backlight";
	doc = getIplImageFromRAW(docName, false, bpp);
	if(!doc)
	{
		cerr<<"\nFailed to load doc lit image";
		return EXIT_FAILURE;
	}

	if(verb)
	{
		cout<<IplImageToString(doc) <<endl;
	}

	if((back->imageSize != fore->imageSize) || (fore->imageSize !=  doc->imageSize))
	{
		cerr<<"\nImages are not of the same size";
	}
	else
	{

		IplImage *variance= cvCloneImage(fore);
		IplImage *varianceAbs= cvCloneImage(fore);

		cvSub(back, fore, variance);
		if(verb)
		{
			cout<<"Variance Image: "<<IplImageToString(doc) <<endl;
		}
		/********/
		cvAbsDiff(back, fore, varianceAbs);
		if(verb)
		{
			cout<<"VarianceABS Image: "<<IplImageToString(doc) <<endl;
		}
		/********/

		IplImage *diffIm= cvCloneImage(doc);
		IplImage *diffImAbs= cvCloneImage(doc);

		cvSub(variance, doc, diffIm);
		if(verb)
		{
			cout<<"Backside Image: "<<IplImageToString(doc) <<endl;
		}
		/********/
		cvAbsDiff(variance, doc, diffImAbs);
		if(verb)
		{
			cout<<"BacksideABS Image: "<<IplImageToString(doc) <<endl;
		}
		/********/

		if(!writePFM(diffIm, "backText.pfm") || 
			!writePFM(variance, "variance.pfm") ||
			!writePFM(diffImAbs, "backTextAbs.pfm") || 
			!writePFM(varianceAbs, "varianceAbs.pfm") )
		{
			cerr<<"\nFailed to saw backText images";
			return EXIT_FAILURE;	
		}
		
		if(saveInter)
		{
				if(!writePFM(doc, "doc-no-backlight.pfm") ||
				!writePFM(back, "backlight-only.pfm") ||
				!writePFM(fore, "doc-backlit.pfm"))
			{
				cerr<<"\nFailed to dump raw pfm's to disk";
			}

			/*
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
			*/
		}

		if(plots)
		{
			if(!writePlot(diffIm, "backText.dat") || 
				!writePlot(variance, "variance.dat"))
			{
				 cerr<<"\nFailed to write plot files";
			}
		}

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
