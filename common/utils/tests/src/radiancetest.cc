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

IplImage* getIplImageFromPFM(const string& filename);
bool writePFM(const IplImage *im, const string& filename);

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

IplImage* getIplImageFromPFM(const string& filename)
{
	IplImage *im = 0;
	u_int size = file_size(filename);
	cout << size/(1024.0*1024.0) <<" MB";
	if(extension(filename) != ".pfm" || size < 1024)
	{
		cerr<<"\n Invalid file specified ";
		return 0;
	}

	int width, height, channels;
	float sf;
	ifstream fin(filename.c_str(), ios::binary);
	string header;

	getline(fin, header);
	if( header == "PF")
		channels = 3;
	else if( header != "Pf")
		channels = 1;
	else
	{
		cerr<<"\nBad header. ";
		return 0;
		fin.close();
	}
	getline(fin, header);
	stringstream ss(header);
	ss >> width >> height;

	getline(fin, header);
	ss.str(header);
	ss >> sf;

	im = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, channels);
	int toRead = width * height * channels * sizeof(float);
#if TEST
	cout <<"\nWidthStep: "<< im->widthStep << " row Length: "<<width*channels*sizeof(float)<< " toRead: " << toRead << " sf: "<< sf<< endl;
#endif
	fin.read(im->imageData, toRead);
	if( fin.gcount() != toRead)
	{
		cvReleaseImage(&im);
		im = 0;
		cerr<<"\nInvalid bytes read. Expected to read: " << toRead <<" bytes";
		return 0;
	}
#if TEST
	cout <<"\nimageDataRead[""] \"";
	for(int j=0; j < 32;++j)
		cout << im->imageData[j]; ;
	cout <<"\"";
#endif


	//SCALEFACTOR AND ENDIANS CHECK

	return im;
}

bool writePFM(const IplImage *im, const string& filename)
{
	if(0 == im || filename.empty())
	{
		cerr<<"\ninvalid parameters";
	}
	ofstream fout(filename.c_str(), ios::binary| ios::trunc);
	if(!fout.good())
	{
		cerr<<"\nFailed to open output file: \""<<filename<<"\"";
		return false;
	}
	
	//assuming little endian and no scale;
	fout << (im->nChannels>1 ? "PF\n" : "Pf\n") 
		  << im->width << " " << im->height << "\n" 
		  << "-1.000000" << "\n";

#if TEST
	cout <<"\nimageDataWrite[""] \"";
	for(int j=0; j < 32;++j)
		cout << im->imageData[j]; ;
	cout <<"\"";
#endif

	fout.write(im->imageData, im->imageSize);
	if(fout.bad())
	{
		cerr<<"\nI/O error occured during write:";
		return false;
	}
	fout.close();
	return true;
}
