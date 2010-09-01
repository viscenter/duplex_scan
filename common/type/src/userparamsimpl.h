/**
 *	@file common/type/src/userparamsimpl.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */

#pragma once

#include <string>

#include <opencv/cv.h>

#include <common/type/src/userparams.h>

namespace viz
{
	
	/**
	 * @brief Implements getImage parameters 
	 *
	 * Defines parameters to get a camera image
	 * @see UserParams
	 * @see ICamera
	 */
	class UserParamsGetImageImpl : public UserParams
	{

		public:
			/**
			 * @brief constructor
			 *
			   <p>
			 *	<code>if(!filename.empty())</code> capture image data will be saved to a file specified by user, or a generic name will be used 
			   </p>
			   <p>
			 *	<code>if(image != 0)</code> imageData will be saves to user supplied handle. Image will only be valid if it is a non RAW image (support later). Uses<b> OpenCV's</b> to load data so only valid data types are <i>jpg,png,pbm,pgm,ppm,tiff,bmp,dib,sr,ras</i>
			   </p>
			 * @param[in] filename name to save the captured image to
			 * @param[in] image image data to save captured to
			 * @param[in] setImageData whether to set image data
			 */
			UserParamsGetImageImpl(std::string& filename, IplImage **image, bool setImageData=false);

			/**
			 * @brief default virtual destrutor
			 */
			virtual ~UserParamsGetImageImpl();

			/**
			 * @brief filename getter
			 * 
			 * @return filename used to save captured image to
			 */
			std::string getFilename() const;

			/**
			 * @brief filename setter
			 * 
			 * @param[in] fn sets the filename of the captured image
			 */
			void setFilename(const std::string& fn) { filename = fn;}

			/**
			 * @brief imageData getter
			 * 
			 * @return returns the imageData of the captured image
			 */
			IplImage** getImage();

			/**
			 * @brief getImageData setter
			 * 
			 * @param[in] val sets whether the image should be caputured
			 */
			void setImageData(bool val) { getImageData = val;}

			/**
			 * @brief getImageData getter
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
	 * @brief Implements setShutterSpeed parameters
	 *
	 * Defines parameters to set camera shutter speed 
	 * @see UserParams
	 * @see ICamera
	 */
	class UserParamsSetShutterImpl : public UserParams
	{
		public:
			/**
			 * @brief constructor
			 * 
			 * @param[in] value string representation of the shutter speed
			 */
			UserParamsSetShutterImpl(const std::string& value);
			/**
			 * @brief virtual destructor
			 */
			virtual ~UserParamsSetShutterImpl();

			/**
			 * @brief shutter value getter
			 * 
			 * @return value shutter is set to
			 */
			std::string getValue() const;

			/**
			 * @brief shutter value setter
			 * 
			 * @param[in] val sets the shutter value
			 */
			void setValue(const std::string& val) {  value= val;}
		
		private:
			std::string value;
	};
}
