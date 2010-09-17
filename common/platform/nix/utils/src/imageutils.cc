#include <common/utils/src/imageutils.h>

#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
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
		std::cout <<"\nWidthStep: "<< im->widthStep << " row Length: "<<width*channels*sizeof(float)<< " toRead: " << toRead << " sf: "<< sf<< std::endl;
#endif
		fin.read(im->imageData, toRead);
		if( fin.gcount() != toRead)
		{
			cvReleaseImage(&im);
			im = 0;
			std::cerr<<"\nInvalid bytes read. Expected to read: " << toRead <<" bytes";
			return 0;
		}

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

		fout.write(im->imageData, im->imageSize);
		if(fout.bad())
		{
			std::cerr<<"\nI/O error occured during write:";
			return false;
		}
		fout.close();
		return true;
	}


	IplImage* getIplImageFromRAW(const std::string& filename, bool inColor, int bpp)
	{
		IplImage *im = 0;
		std::stringstream cmd;
		std::string  tmpname;
		double dom = 1.0/((1<<bpp) -1);


		cmd << UFRAW_BATCH_EXE;
		if(inColor)
		{
			cmd << " --grayscale=none  ";
			tmpname = "_tmp.ppm";
		}
		else
		{
			cmd << " --grayscale=value ";
			tmpname = "_tmp.pgm";
		}

		cmd <<" --silent --overwrite --wb=camera --out-depth=16 --output=" << tmpname;
		cmd << " " <<filename;

#if DEBUG
		std::cout <<"\nExecuting \""<<cmd.str()<<"\" scale factor:" << dom 
					 <<std::endl; 
#endif

		if(system(cmd.str().c_str()) != 0)
		{
			std::cerr<<"\nFailed to run ufraw-batch on raw image";
		}
		else
		{
			im = cvLoadImage(tmpname.c_str(), -1);
			if(!im)
			{
				im = 0;
				std::cerr<<"\nFailed convert to IplImage";
			}
			else
			{
#if DEBUG
			std::cout  <<"loaded ["<<im->width<<"*"<<im->height<<" * "
					<<im->nChannels<<"] bpp:"<<im->depth<<std::endl;
#endif
			
				IplImage *im2 = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_32F, im->nChannels);
				cvConvertScale(im, im2, dom);
				cvReleaseImage(&im);
				im = im2;
			}
			boost::filesystem::remove(tmpname);
		}

		return im;
	}

	bool writePlot(const IplImage* im, const std::string filename)
	{
		if(0 == im || filename.empty())
		{
			std::cerr<<"\ninvalid parameters";
			return false;
		}
		std::ofstream fout(filename.c_str(), std::ios::trunc);
		if(!fout.good())
		{
			std::cerr<<"\nFailed to open output file: \""<<filename<<"\"";
			return false;
		}
		if(im->nChannels > 1)
			std::cerr<<"\nImage plot to \""<<filename<<"\" only using first channel";

#if DEBUG
			std::cout  <<"loaded ["<<im->width<<"*"<<im->height<<" * "
					<<im->nChannels<<"] bpp:"<<im->depth<<std::endl;
#endif

		switch(im->depth)
		{
			case IPL_DEPTH_8U:
			{
#if DEBUG
			std::cout<<"8U\n";
#endif

				const u_char *data = reinterpret_cast<const u_char*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}

				dumpData<u_char>(im, data, fout);
			}
			break;

			case IPL_DEPTH_8S:
			{
#if DEBUG
			std::cout<<"8S\n";
#endif
				const char *data = reinterpret_cast<const char*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}
				dumpData<char>(im, data, fout);

			}
			break;


			case IPL_DEPTH_16U:
			{
#if DEBUG
			std::cout<<"16U\n";
#endif
				const u_short *data = reinterpret_cast<const u_short*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}

				dumpData<u_short>(im, data, fout);
			}
			break;


			case IPL_DEPTH_16S:
			{
#if DEBUG
			std::cout<<"16S\n";
#endif
				const short *data = reinterpret_cast<const short*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}

				dumpData<short>(im, data, fout);
			}
			break;


			case IPL_DEPTH_32S:
			{
#if DEBUG
			std::cout<<"32S\n";
#endif
				const int *data = reinterpret_cast<const int*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}
				dumpData<int>(im, data, fout);

			}
			break;



			case IPL_DEPTH_32F:
			{
#if DEBUG
			std::cout<<"32F\n";
#endif
				const float *data = reinterpret_cast<const float*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}
				dumpData<float>(im, data, fout);
			}
			break;

			case IPL_DEPTH_64F:
			{
#if DEBUG
			std::cout<<"64F\n";
#endif
				const double *data = reinterpret_cast<const double*>(im->imageData);
				if(!data)
				{
					std::cerr<<"\nCast failed";
					return false;
				}

				dumpData<double>(im, data, fout);

			}
			break;


			default:
			{
				std::cerr<<"\nInvalid image depth";
				return false;
			}
			break;

		}
		fout.close();

		return true;
	}

	template<typename T>
	bool dumpData(const IplImage *im, const T *data, std::ofstream& fout)
	{

		for(int y=0;y< im->height;++y)
			for(int x=0; x<im->width;++x)
				fout << x << " " << y << " " 
						<< data[x*im->nChannels + im->widthStep*y] <<std::endl;

		return true;
	}
}
