#include <iostream>
#include <fstream>
#include <string>
#include <opencv/cv.h>
#include "boost/filesystem.hpp" 

namespace viz
{

	IplImage* getIplImageFromPFM(const std::string& filename)
	{
		IplImage *im = 0;
		u_int size = boost::filesystem::file_size(filename);
		std::cout << size/(1024.0*1024.0) <<" MB";
		if(boost::filesystem::extension(filename) != ".pfm" || size < 1024)
		{
			std::cerr<<"\n Invalid file specified ";
			return 0;
		}

		int width, height, channels;
		float sf;
		std::ifstream fin(filename.c_str(), std::ios::binary);
		std::string header;

		getline(fin, header);
		if( header == "PF")
			channels = 3;
		else if( header != "Pf")
			channels = 1;
		else
		{
			std::cerr<<"\nBad header. ";
			return 0;
			fin.close();
		}
		getline(fin, header);
		std::stringstream ss(header);
		ss >> width >> height;

		getline(fin, header);
		ss.str(header);
		ss >> sf;

		im = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, channels);
		int toRead = width * height * channels * sizeof(float);
#if DEBUG
		std::cout <<"\nWidthStep: "<< im->widthStep << " row Length: "<<width*channels*sizeof(float)<< " toRead: " << toRead << " sf: "<< sf<< endl;
#endif
		fin.read(im->imageData, toRead);
		if( fin.gcount() != toRead)
		{
			cvReleaseImage(&im);
			im = 0;
			std::cerr<<"\nInvalid bytes read. Expected to read: " << toRead <<" bytes";
			return 0;
		}
#if DEBUG
		std::cout <<"\nimageDataRead[""] \"";
		for(int j=0; j < 32;++j)
			std::cout << im->imageData[j]; ;
		std::cout <<"\"";
#endif


		//SCALEFACTOR AND ENDIANS CHECK

		return im;
	}

	bool writePFM(const IplImage *im, const std::string& filename)
	{
		if(0 == im || filename.empty())
		{
			std::cerr<<"\ninvalid parameters";
		}
		std::ofstream fout(filename.c_str(), std::ios::binary| std::ios::trunc);
		if(!fout.good())
		{
			std::cerr<<"\nFailed to open output file: \""<<filename<<"\"";
			return false;
		}
		
		//assuming little endian and no scale;
		fout << (im->nChannels>1 ? "PF\n" : "Pf\n") 
			  << im->width << " " << im->height << "\n" 
			  << "-1.000000" << "\n";

#if DEBUG
		std::cout <<"\nimageDataWrite[""] \"";
		for(int j=0; j < 32;++j)
			std::cout << im->imageData[j]; ;
		std::cout <<"\"";
#endif

		fout.write(im->imageData, im->imageSize);
		if(fout.bad())
		{
			std::cerr<<"\nI/O error occured during write:";
			return false;
		}
		fout.close();
		return true;
	}
}
