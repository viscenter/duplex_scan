#pragma once

#include <string>

#include <opencv/cv.h>

#include <common/type/src/userparams.h>

namespace viz
{
	class UserParamsGetImageImpl : public UserParams
	{
		public:
			UserParamsGetImageImpl(std::string& filename, IplImage **image);
			virtual ~UserParamsGetImageImpl();
			std::string getFilename() const;
			IplImage* getImage() const;

		
		private:
			std::string filename;
			IplImage *image;
	};

	class UserParamsSetShutterImpl : public UserParams
	{
		public:
			UserParamsSetShutterImpl(double value);
			virtual ~UserParamsSetShutterImpl();
			double getValue() const;
		
		private:
			double value;
	};
}
