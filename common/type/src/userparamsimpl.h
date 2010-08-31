/**
 *	@file common/type/src/userparamsimpl.h
 * Concrete classes for User defined parameters
 * 
 * These concrete classes define types used by implementing camera interface classes
	@see UserParams
 *
 */

#pragma once

#include <string>

#include <opencv/cv.h>

#include <common/type/src/userparams.h>

/**
 * viz namespace
 */
namespace viz
{
	
	/**
	 * Implement
	 */
	class UserParamsGetImageImpl : public UserParams
	{
		public:
			UserParamsGetImageImpl(std::string& filename, IplImage **image, bool imageData=false);
			virtual ~UserParamsGetImageImpl();
			std::string getFilename() const;
			void setFilename(const std::string& fn) { filename = fn;}
			IplImage** getImage();
			bool loadImageData() const { return getImageData; }

		
		private:
			std::string filename;
			IplImage **image;
			bool getImageData;
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
