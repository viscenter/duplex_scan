
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

#define PYR_SEG 0
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
bool interactive; 
#if PYR_SEG
bool tMet, pMet;
#endif
CvSeq *comp;
CvMemStorage *storage;
int level = 4;
CvRect ROI;
bool roiChanged = true;
	int tScale;


void update(int val);
void onMouse(int event, int x, int y, int flags,  void *param);

int main ( int argc, char **argv )
{
	string filename, ofilename, dfilename;
	int bpp;


	po::options_description desc("Allowed options");
	desc.add_options()
   ("help,h", "help message")
   ("input-file,f", po::value<string>(&filename),"input file")
   ("document,d", po::value<string>(&dfilename),"original document")
   ("output-file,o", po::value<string>(&ofilename),"output mask file")
   ("bpp,b", po::value<int>(&bpp)->default_value(16),"bits per pixel for raw")
   ("low-threshold", po::value<double>(&thresh1)->default_value(0.25),"threshold value [0-1]")
   ("high-threshold", po::value<double>(&thresh2)->default_value(0.5),"threshold value [0-1]")
   ("threshold-resolution,r", po::value<int>(&tScale)->default_value(100),"threshold resolution. Greater the number, smaller the change 100==1% 200==0.5% [0-500]")
   ("interactive,i", "interactive segmentation")
#if PYR_SEG
   ("threshold-segmentation,t", "bi-level thresholding for image segmentation")
   ("pyramid-segmentation,p", "pyramid segmentation");
#else
   ("threshold-segmentation,t", "bi-level thresholding for image segmentation");
#endif


	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
	po::notify(vm);
	
	if (vm.count("help")) {
		 cout << desc << "\n";
		 return EXIT_FAILURE;
	}

	interactive = (vm.count("interactive") > 0);
#if PYR_SEG
	tMet = (vm.count("threshold-segmentation") > 0);
	pMet = (vm.count("pyramid-segmentation") > 0);
#endif

	if ( (vm.count("input-file") == 0)
	 	|| (vm.count("document") == 0)
		|| thresh1>thresh2 
		|| thresh1 < 0 
		|| thresh1 > 1
		|| thresh2 < 0 
		|| thresh2 > 1
		|| tScale < 1 
		|| tScale > 500 
#if PYR_SEG
		|| !(tMet^pMet) 
#endif
		)
	{
		 cout << desc << "\n";
		 return EXIT_FAILURE;
	}

	if(thresh1 == 0.25 && thresh2 == 0.5) //defaults
		interactive = true;
#if PYR_SEG
	if(pMet)
	{
		lowTrack = "Link Threshold";
		highTrack = "Cluster Threshold";
	}
#endif
	 

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

	ROI.x = 0; ROI.y=0; ROI.width = doc->width; ROI.height = doc->height;
	roiChanged = true;
	map<string, pair<IplImage*, IplImage*> > winNames;
	

	cout  << IplImageToString(im) <<endl;
	dim = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	dim->origin = im->origin;
	cvZero(dim);
	tmp = cvCloneImage(dim);
	tmp2 = cvCloneImage(dim);

	thresh1Int = (int)(thresh1*tScale);
	thresh2Int = (int)(thresh2*tScale);
	if(interactive)
	{
		cvNamedWindow( filename.c_str(), 0);
		cvShowImage( filename.c_str(), im );
		cvResizeWindow(filename.c_str(), 800, 600);
		cvNamedWindow( dfilename.c_str(), 0);
		cvShowImage( dfilename.c_str(), doc );
		cvResizeWindow(dfilename.c_str(), 800, 600);
		winNames[filename] = make_pair(im, cvCloneImage(im));
		winNames[dfilename] = make_pair(doc, cvCloneImage(doc));
		winNames[win] = make_pair(dim, cvCloneImage(dim));
		cvSetMouseCallback(filename.c_str(), onMouse, &winNames);
		cvSetMouseCallback(dfilename.c_str(), onMouse, &winNames);


		cvNamedWindow( win.c_str(), 0);
		cvResizeWindow(win.c_str(), 1024, 768);
		cvCreateTrackbar(lowTrack.c_str(), win.c_str(), &thresh1Int, tScale, update);
		cvCreateTrackbar(highTrack.c_str(), win.c_str(), &thresh2Int, tScale, update);
#if PYR_SEG
		if(tMet)
		{
			cvMinMaxLoc(im, &mi, &mx);
			range = mx - mi;
			//cout <<"\nrange: "<<range;
		}
		else
		if(pMet)
		{
			storage = cvCreateMemStorage();
			if(im->depth > 8 )
			{
				scaled = cvCreateImage(cvSize(im->width, im->height), 
								IPL_DEPTH_8U, im->nChannels);
				scaled->origin = im->origin;
				cvConvertScaleAbs(im, scaled, 255.0);
				cout <<"\nScaled to 8bit";
			}
			else
			{
				scaled = cvCloneImage(im);
			}
		}
#else
		cvMinMaxLoc(im, &mi, &mx);
		range = mx - mi;
#endif

		update(0);
		cvShowImage( win.c_str(), dim );
		cvWaitKey();
		cvDestroyWindow(win.c_str());
		cvDestroyWindow(filename.c_str());
		cvDestroyWindow(dfilename.c_str());


	}
	else
	{
#if PYR_SSEG
		if(pMet)
		{
			storage = cvCreateMemStorage();
			if(im->depth > 8 )
			{
				scaled = cvCreateImage(cvSize(im->width, im->height), 
								IPL_DEPTH_8U, im->nChannels);
				scaled->origin = im->origin;
				cvConvertScaleAbs(im, scaled, 255.0);
				cout <<"\nScaled to 8bit";
			}
			else
			{
				scaled = cvCloneImage(im);
			}
		}
#else
		cvMinMaxLoc(im, &mi, &mx);
		range = mx - mi;
#endif

		update(0); //get one update in with values
	}



	//Morphological hackery

	IplConvKernel* k = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT);
	cvCopy(dim, tmp);
	cvCopy(dim, tmp2);
	IplImage * orig = cvCloneImage(dim);
	/*
	cvSaveImage("mop__nop.png",dim);
	cvMorphologyEx(dim, tmp, tmp2, k, CV_MOP_CLOSE);
	cvSaveImage("mop_close.png",tmp);
	cvMorphologyEx(dim, tmp, tmp2, k, CV_MOP_GRADIENT);
	cvSaveImage("mop_gradient.png",tmp);
	cvMorphologyEx(dim, tmp, tmp2, k, CV_MOP_TOPHAT);
	cvSaveImage("mop_topHat.png",tmp);
	cvMorphologyEx(dim, tmp, tmp2, k, CV_MOP_BLACKHAT);
	cvSaveImage("mop_bottomHat.png", tmp);
	*/

	cvMorphologyEx(dim, tmp, tmp2, k, CV_MOP_OPEN, 1);
	cvSaveImage("mop_open.png", tmp);

	cvMorphologyEx(tmp, tmp2, 0,  k, CV_MOP_CLOSE, 2);
	cvSaveImage("mop_close2.png", tmp2);
	cvMorphologyEx(tmp2, tmp, 0,  k, CV_MOP_OPEN, 1);
	cvSaveImage("mop_close2open.png", tmp);
	cvMorphologyEx(tmp, tmp2, 0,  k, CV_MOP_CLOSE, 2);
	cvSaveImage("mop_close2openclose2.png", tmp2);


	cvAnd(dim, tmp2, tmp);
	cvSaveImage("mop_openANDnop.png", tmp);
	cvCopy(orig, dim);
	cvReleaseImage(&orig);

	if(!ofilename.empty())
	{
		cvResetImageROI(im);
		cvResetImageROI(dim);
		cvResetImageROI(tmp);
		cvResetImageROI(tmp2);
		update(0);
		cvSaveImage(ofilename.c_str(), dim);
	}
#if PYR_SEG
	if(pMet)
	{
		cvReleaseMemStorage(&storage);
		cvReleaseImage(&scaled);
	}
#endif


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

	if(roiChanged)
	{
			cvSetImageROI(im, ROI);
			cvSetImageROI(tmp, ROI);
			cvSetImageROI(tmp2, ROI);
			cvSetImageROI(dim, ROI);

		roiChanged = false;
	}

#if PYR_SEG
	if(tMet)
	{
#endif
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
	

		double lo = thresh1Int/((double)tScale)*range+mi;
		double hi = thresh2Int/((double)tScale)*range+mi;


	//	cout <<"Updating .."<<thresh1Int <<":"<<thresh2Int << ".."<<lo <<":"<<hi <<"("<<range<<":"<<mi<<")"<<endl;
		cvCmpS(im, lo,tmp, CV_CMP_GE);
		cvCmpS(im, hi,tmp2, CV_CMP_LE);
		cvCmp(tmp, tmp2, dim, CV_CMP_EQ);
#if PYR_SEG
	}
	else
	if(pMet)
	{
	//cout <<"Updating .."<<thresh1Int <<":"<<thresh2Int <<endl;
		cvPyrSegmentation(scaled, dim, storage, &comp,
				                      level, 
											 (int)(thresh1Int/((double)tScale)*255), 
											 (int)(thresh2Int/((double)tScale)*255)); 

	}
#endif

	cvShowImage( win.c_str(), dim );
	//cout <<" done";
	computing = false;
}

void onMouse(int event, int x, int y, int flags,  void *param)
{
	static bool drag = false;
	static int sX, sY, eX, eY;
	map<string,pair< IplImage*, IplImage*> >*winNames = reinterpret_cast< map<string,pair<IplImage*,IplImage*> >* >(param); 
	if(!winNames)
	{
		cerr<<"\nfailed to get window names";
		return;
	}

	if(event == CV_EVENT_LBUTTONDOWN)
	{
		drag = true;
		sX = x ; sY = y ;
		//cout <<"Start "<<sX <<":"<<sY<<endl;
	}

	if(event == CV_EVENT_LBUTTONUP)
	{
		drag = false;
		eX = x; eY = y;
		//cout <<"End "<<eX <<":"<<eY<<endl;
		//cout << ROI.x <<"," <<ROI.y <<" "<<ROI.width<<"*"<<ROI.height<<endl;
	}

	if(event == CV_EVENT_MOUSEMOVE)
	{
		if(drag)
		{
			eX = x; eY = y;
			ROI.x = sX < eX ? sX:eX;
			ROI.y = sY < eY ? sY:eY;
			ROI.width = sX < eX ? eX-sX: sX-eX;
			ROI.height = sY < eY ? eY-sY: sY-eY;
			if(im->origin != 0)
			{
				ROI.y = im->height - ROI.y;
			}


			roiChanged = true;
		}
		else
			return;

	}

	if(event == CV_EVENT_RBUTTONUP)
	{
		cout <<"Resetting ROI"<<endl;
		ROI.x = 0; ROI.y =0; ROI.width = im->width; ROI.height = im->height;
		roiChanged = true;
	}



	if(!drag)
	{
		for(map<string, pair<IplImage*, IplImage*> >::iterator i=winNames->begin(); 
			 i != winNames->end() ; ++i)
		{
			cvResetImageROI(i->second.first);
			cvCopy(i->second.first, i->second.second);
				cvRectangle(i->second.second, cvPoint(ROI.x,ROI.y), cvPoint(ROI.x+ROI.width, ROI.y+ROI.height), CV_RGB(255, 0, 0), 8);
			cvShowImage(i->first.c_str(), i->second.second);
		}
	}

}
