
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

//GLOBALS for win controls
IplImage *dim, *im, *tmp, *tmp2;
string win("Thresholding Window");
string lowTrack("lo threshold");
string highTrack("hi threshold");
int thresh1Int, thresh2Int;
double thresh1, thresh2, mi, mx;
double range;
bool interactive, tMet, pMet;

void update(int val);

int main ( int argc, char **argv )
{
	string filename, ofilename;
	int bpp;
	double scaleFactor;


	po::options_description desc("Allowed options");
	desc.add_options()
   ("help,h", "help message")
   ("input-file,f", po::value<string>(&filename),"input file")
   ("output-file,o", po::value<string>(&ofilename),"output file")
   ("bpp,b", po::value<int>(&bpp)->default_value(16),"bits per pixel for raw")
   ("low-threshold", po::value<double>(&thresh1)->default_value(0.25),"threshold value [0-1]")
   ("high-threshold", po::value<double>(&thresh2)->default_value(0.5),"threshold value [0-1]")
   ("scale-factor,s", po::value<double>(&scaleFactor)->default_value(1.0),"scale factor use to resize image [0-10]")
   ("interactive,i", "interactive-segmentation")
   ("threshold-segmentation,t", "bi-level thresholding for image segmentation")
   ("pyramid-segmentation,p", "pyramid segmentation");

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		 cout << desc << "\n";
		 return 0;
	}

	interactive = (vm.count("interactive") > 0);
	tMet = (vm.count("threshold-segmentation") > 0);
	pMet = (vm.count("pyramid-segmentation") > 0);

	if ( (vm.count("input-file") == 0)
		|| thresh1>thresh2 
		|| thresh1 < 0 
		|| thresh1 > 1
		|| thresh2 < 0 
		|| thresh2 > 1
		|| !(tMet^pMet) 
		|| scaleFactor < 0 
		|| scaleFactor > 10) 
	{
		 cout << desc << "\n";
		 return 0;
	}
	 


	//cerr<< "\nreg ";
	if(0 == (im=getIplImageFromRAW(filename.c_str(), true, bpp)))
	{
		//cerr<< "\npfm ";
		if(0 == (im=getIplImageFromPFM(filename.c_str())))
		{
			//cerr<< "\nraw ";
	if(0 == (im=cvLoadImage(filename.c_str(), -1)))
			{
				cerr<<"\nFailed to load image file \""<<filename<<"\"\n";
				return EXIT_FAILURE;
			}
		}
	}

	if(vm.count("scale-factor"))
	{
		cout<<"\nResizing image by factor of " <<scaleFactor;
		tmp = cvCreateImage(cvSize(im->width*scaleFactor, im->height*scaleFactor), im->depth, im->nChannels);
		cvResize(im, tmp, CV_INTER_CUBIC);
		cvReleaseImage(&im);
		im = cvCloneImage(tmp);
		cvReleaseImage(&tmp);
	}

	cout  << IplImageToString(im) <<endl;
	if(tMet)
		dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	else
	if(pMet)
		dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 3);

	tmp = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	tmp2 = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);

	cvZero(dim);
	if(interactive)
	{
		thresh1Int = (int)(thresh1*100);
		thresh2Int = (int)(thresh2*100);
		cvNamedWindow( filename.c_str(), 0);
		cvShowImage( filename.c_str(), im );
		cvResizeWindow(filename.c_str(), 640, 480);

		if(tMet)
		{
			cvNamedWindow( win.c_str(), 0);
			cvResizeWindow(win.c_str(), 1024, 768);
			cvCreateTrackbar(lowTrack.c_str(), win.c_str(), &thresh1Int, 100, update);
			cvCreateTrackbar(highTrack.c_str(), win.c_str(), &thresh2Int, 100, update);
			cvMinMaxLoc(im, &mi, &mx);
			range = mx - mi;
			//cout <<"\nrange: "<<range;
			update(0);
			cvShowImage( win.c_str(), dim );
		}
		else
		{

		}

		cvWaitKey();
		cvDestroyWindow(win.c_str());
		cvDestroyWindow(filename.c_str());


	}
	else
	{
		update(0); //get one update in with the default values
	}

	if(!ofilename.empty())
	{
		cvSaveImage(ofilename.c_str(), dim);
	}

	cvReleaseImage(&dim);
	cvReleaseImage(&tmp);
	cvReleaseImage(&tmp2);
	cvReleaseImage(&im);
	return EXIT_SUCCESS;

}

void update(int val)
{
	if(thresh1Int > thresh2Int)
	{
		thresh1Int = thresh2Int;
		cvSetTrackbarPos(lowTrack.c_str(), win.c_str(), thresh1Int);
		return;
	}
	else
	if(thresh2Int < thresh1Int)
	{
		thresh2Int = thresh1Int;
		cvSetTrackbarPos(highTrack.c_str(), win.c_str(), thresh2Int);
		return;
	}

	double lo = thresh1Int/100.0*range+mi;
	double hi = thresh2Int/100.0*range+mi;

	if(tMet)
	{

		cvCmpS(im, lo,tmp, CV_CMP_GE);
		cvCmpS(im, hi,tmp2, CV_CMP_LE);
		cvCmp(tmp, tmp2, dim, CV_CMP_EQ);
	}
	else
	if(pMet)
	{



	}

	cvShowImage( win.c_str(), dim );
}
