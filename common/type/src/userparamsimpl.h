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
	 * Implements @see UserParams
	 *
	 * Defines parameters to get a camera image
	 */
	class UserParamsGetImageImpl : public UserParams
	{

		public:
			/**
			 * constructor
			 *
			 *	If <code>!filename.empty()</code> capture image data will be saved to a file specified by user, or a generic name will be used 
			 *	If <code>image != 0</code> imageData will be saves to user supplied handle. image will only be valid if it is a non RAW image. Uses opencv to load data so only valid data types are jpg,png,pbm,pgm,ppm,tiff,bmp,dib,sr,ras
			 * @param[in] filename name to save the captured image to
			 * @param[in] image image data to save captured to
			 * @param[in] setImageData whether to set image data
			 */
			UserParamsGetImageImpl(std::string& filename, IplImage **image, bool setImageData=false);

			/**
			 * default virtual destrutor
			 */
			virtual ~UserParamsGetImageImpl();

			/**
			 * filename getter
			 * 
			 * @return filename used to save captured image to
			 */
			std::string getFilename() const;

			/**
			 * filename setter
			 * 
			 * @param[in] fn sets the filename of the captured image
			 */
			void setFilename(const std::string& fn) { filename = fn;}

			/**
			 * imageData getter
			 * 
			 * @return returns the imageData of the captured image
			 */
			IplImage** getImage();

			/**
			 * getImageData setter
			 * 
			 * @param[in] val sets whether the image should be caputured
			 */
			void setImageData(bool val) { getImageData = val;}

			/**
			 * getImageData getter
			 * 
			 * @return whether imageData is going to be set
			 */
			bool loadImageData() const { return getImageData; }

		
		private:
			std::string filename;
			IplImage **image;
			bool getImageData;
	};

	/**
	 * Implements @see UserParams
	 *
	 * Defines parameters to set camera shutter speed 
	 */
	class UserParamsSetShutterImpl : public UserParams
	{
		public:
			/**
			 * constructor
			 * 
			 * @param[in] value string representation of the shutter speed
			 */
			UserParamsSetShutterImpl(const std::string& value);
			/**
			 * virtual destructor
			 */
			virtual ~UserParamsSetShutterImpl();

			/**
			 * shutter value getter
			 * 
			 * @return value shutter is set to
			 */
			std::string getValue() const;

			/**
			 * shutter value setter
			 * 
			 * @param[in] val sets the shutter value
			 */
			void setValue(const std::string& val) {  value= val;}
		
		private:
			std::string value;
	};
}
