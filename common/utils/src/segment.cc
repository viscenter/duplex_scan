
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
IplImage *dim=0, *im=0, *tmp=0, *tmp2=0, *scaled=0, *imOrig=0, *doc;
string win("Thresholding Window");
string lowTrack("lo threshold");
string highTrack("hi threshold");
int thresh1Int, thresh2Int;
double thresh1, thresh2, mi, mx;
double range;
bool interactive, tMet, pMet;
CvSeq *comp;
CvMemStorage *storage;
int level = 4;

void update(int val);

int main ( int argc, char **argv )
{
	string filename, ofilename, dfilename;
	int bpp;
	double scaleFactor;


	po::options_description desc("Allowed options");
	desc.add_options()
   ("help,h", "help message")
   ("input-file,f", po::value<string>(&filename),"input file")
   ("document,d", po::value<string>(&dfilename),"original document")
   ("output-file,o", po::value<string>(&ofilename),"output file")
   ("bpp,b", po::value<int>(&bpp)->default_value(16),"bits per pixel for raw")
   ("low-threshold", po::value<double>(&thresh1)->default_value(0.25),"threshold value [0-1]")
   ("high-threshold", po::value<double>(&thresh2)->default_value(0.5),"threshold value [0-1]")
   ("scale-factor,s", po::value<double>(&scaleFactor)->default_value(1.0),"scale factor use to resize image [0-10]")
   ("interactive,i", "interactive segmentation")
   ("threshold-segmentation,t", "bi-level thresholding for image segmentation")
   ("pyramid-segmentation,p", "pyramid segmentation");

	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		 cout << desc << "\n";
		 return EXIT_FAILURE;
	}

	interactive = (vm.count("interactive") > 0);
	tMet = (vm.count("threshold-segmentation") > 0);
	pMet = (vm.count("pyramid-segmentation") > 0);

	if ( (vm.count("input-file") == 0)
	 	|| (vm.count("document") == 0)
		|| thresh1>thresh2 
		|| thresh1 < 0 
		|| thresh1 > 1
		|| thresh2 < 0 
		|| thresh2 > 1
		|| !(tMet^pMet) 
		|| scaleFactor < 0 
		|| scaleFactor > 4) 
	{
		 cout << desc << "\n";
		 return EXIT_FAILURE;
	}

	if(thresh1 == 0.25 && thresh2 == 0.5) //defaults
		interactive = true;

	if(pMet)
	{
		lowTrack = "Link Threshold";
		highTrack = "Cluster Threshold";
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

	//cerr<< "\nreg ";
	if(0 == (doc=getIplImageFromRAW(dfilename.c_str(), true, bpp)))
	{
		//cerr<< "\npfm ";
		if(0 == (doc=getIplImageFromPFM(dfilename.c_str())))
		{
			//cerr<< "\nraw ";
			if(0 == (doc=cvLoadImage(dfilename.c_str(), -1)))
			{
				cerr<<"\nFailed to load image file \""<<dfilename<<"\"\n";
				return EXIT_FAILURE;
			}
		}
	}

	if(scaleFactor != 1.0)
	{
		cout<<"\nResizing image by factor of " <<scaleFactor;
		imOrig = cvCloneImage(im);
		tmp = cvCreateImage(cvSize(im->width*scaleFactor, im->height*scaleFactor), im->depth, im->nChannels);
		cvResize(im, tmp, CV_INTER_CUBIC);
		cvReleaseImage(&im);
		im = cvCloneImage(tmp);
		cvReleaseImage(&tmp);
	}

	cout  << IplImageToString(im) <<endl;
	/*(
	if(tMet)
		dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	else
	if(pMet)
		dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 3);
	*/
	dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	tmp = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	tmp2 = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);

	cvZero(dim);
	thresh1Int = (int)(thresh1*100);
	thresh2Int = (int)(thresh2*100);
	if(interactive)
	{
		cvNamedWindow( filename.c_str(), 0);
		cvShowImage( filename.c_str(), im );
		cvResizeWindow(filename.c_str(), 800, 600);
		cvNamedWindow( dfilename.c_str(), 0);
		cvShowImage( dfilename.c_str(), doc );
		cvResizeWindow(dfilename.c_str(), 800, 600);

		cvNamedWindow( win.c_str(), 0);
		cvResizeWindow(win.c_str(), 1024, 768);
		cvCreateTrackbar(lowTrack.c_str(), win.c_str(), &thresh1Int, 100, update);
		cvCreateTrackbar(highTrack.c_str(), win.c_str(), &thresh2Int, 100, update);

		if(tMet)
		{
			cvMinMaxLoc(im, &mi, &mx);
			range = mx - mi;
			//cout <<"\nrange: "<<range;
		}
		else
		if(pMet)
		{
			storage = cvCreateMemStorage ( 1000 );
			if(im->depth > 8 )
			{
				scaled = cvCreateImage(cvSize(im->width, im->height), 
								IPL_DEPTH_8U, im->nChannels);
				cvConvertScaleAbs(im, scaled, 255.0);
			}
			else
			{
				scaled = cvCloneImage(im);
			}
		}
		update(0);
		cvShowImage( win.c_str(), dim );
		cvWaitKey();
		cvDestroyWindow(win.c_str());
		cvDestroyWindow(filename.c_str());
		cvDestroyWindow(dfilename.c_str());


	}
	else
	{
		update(0); //get one update in with values
	}


	if(!ofilename.empty())
	{
		//revert to the orignal size and threshold and save that.
		if(scaleFactor != 1.0)
		{
			cvReleaseImage(&dim);
			cvReleaseImage(&tmp);
			cvReleaseImage(&tmp2);
			cvReleaseImage(&im);

			im = cvCloneImage(imOrig);
			dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
			tmp = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
			tmp2 = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
			cvZero(dim);
			if(tMet)
			{
			cvMinMaxLoc(im, &mi, &mx);
			range = mx - mi;
			}
			else
			if(pMet)
			{
				if(im->depth > 8 )
				{
					scaled = cvCreateImage(cvSize(im->width, im->height), 
									IPL_DEPTH_8U, im->nChannels);
					cvConvertScaleAbs(im, scaled, 255.0);
				}
				else
				{
					scaled = cvCloneImage(im);
				}
			}
			update(0);
			cvReleaseImage(&imOrig);
		}

		cvSaveImage(ofilename.c_str(), dim);
	}

	if(pMet)
	{
		cvReleaseMemStorage(&storage);
		cvReleaseImage(&scaled);
	}


	cvReleaseImage(&dim);
	cvReleaseImage(&tmp);
	cvReleaseImage(&tmp2);
	cvReleaseImage(&im);
	return EXIT_SUCCESS;

}

void update(int val)
{
	static bool computing = false;
	
	if(computing )
	{
		//cout <<"\nComputing previous";
		return;
	}
	else
		computing = true;

	//cout <<"Updating .."<<thresh1Int <<":"<<thresh2Int <<endl;
	if(tMet)
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


		cvCmpS(im, lo,tmp, CV_CMP_GE);
		cvCmpS(im, hi,tmp2, CV_CMP_LE);
		cvCmp(tmp, tmp2, dim, CV_CMP_EQ);
	}
	else
	if(pMet)
	{
		cvPyrSegmentation(scaled, dim, storage, &comp,
				                      level, 
											 (int)thresh1Int/100.0*255, 
											 (int)thresh2Int/100.0*255); 

	}
	cvShowImage( win.c_str(), dim );
	//cout <<" done";
	computing = false;
}
