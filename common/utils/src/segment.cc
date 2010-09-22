
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

IplImage *dim, *im,* im8U;
string win("Thresh Window");
int thresh1Int, thresh2Int;
	double thresh1, thresh2;

void update(int val);

int main ( int argc, char **argv )
{
	string filename, ofilename;
	int bpp;
	bool interactive;


	po::options_description desc("Allowed options");
	desc.add_options()
   ("help,h", "help message")
   ("input-file,f", po::value<string>(&filename),"input file")
   ("output-file,o", po::value<string>(&ofilename),"output file")
   ("bpp,b", po::value<int>(&bpp)->default_value(16),"bits per pixel for raw")
   ("low-threshold", po::value<double>(&thresh1)->default_value(0.25),"threshold value [0-1]")
   ("high-threshold", po::value<double>(&thresh2)->default_value(0.5),"threshold value [0-1]")
   ("interactive,i", "interactive-segmentation");

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		 cout << desc << "\n";
		 return 0;
	}

	if (!vm.count("input-file") && thresh1<=thresh2) 
	{
		 cout << desc << "\n";
		 return 0;
	}
	 

	interactive = (vm.count("interactive") > 0);

	//cerr<< "\nreg ";
	if(0 == (im=cvLoadImage(filename.c_str(), -1)))
	{
		//cerr<< "\npfm ";
		if(0 == (im=getIplImageFromPFM(filename.c_str())))
		{
			//cerr<< "\nraw ";
			if(0 == (im=getIplImageFromRAW(filename.c_str(), true, bpp)))
			{
				cerr<<"\nFailed to load image file \""<<filename<<"\"\n";
				return EXIT_FAILURE;
			}
		}
	}

	cout  << IplImageToString(im) <<endl;

	if(interactive)
	{
		thresh1Int = (int)(thresh1*100);
		thresh2Int = (int)(thresh2*100);
		cvNamedWindow( win.c_str(), -1 );
		cvCreateTrackbar("low thresh", win.c_str(), &thresh1Int, 100, update);
		cvCreateTrackbar("high thresh", win.c_str(), &thresh2Int, 100, update);
		dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
		im8U = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);

		cvConvert(im, im8U);
		cout  << IplImageToString(dim) <<endl;
		cout  << IplImageToString(im8U) <<endl;

		cvShowImage( win.c_str(), dim );
		cvWaitKey();
		cvDestroyWindow(win.c_str());


		if(!ofilename.empty())
		{
			cvSaveImage(ofilename.c_str(), dim);
		}

		cvReleaseImage(&dim);
		cvReleaseImage(&im8U);

	}

	cvReleaseImage(&im);
	return EXIT_SUCCESS;

}

void update(int val)
{
	//cvThreshold(im8U, dim, (int)(thresh1Int/100.0*255),255, CV_THRESH_TOZERO);
	//cvThreshold(dim, dim, (int)(thresh2Int/100.0*255),255,CV_THRESH_TOZERO_INV);
	cvShowImage( win.c_str(), dim );
}
