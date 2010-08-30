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
			IplImage** getImage();

		
		private:
			std::string filename;
			IplImage *image;
	};

	class UserParamsSetShutterImpl : public UserParams
	{
		public:
			UserParamsSetShutterImpl(const std::string& value);
			virtual ~UserParamsSetShutterImpl();
			std::string getValue() const;
		
		private:
			std::string value;
	};
}
