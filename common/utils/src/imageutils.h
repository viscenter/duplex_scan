/**
 *	@file common/utils/src/imageutils.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */
#pragma once
 
#include <string>
#include <opencv/cv.h>

namespace viz
{
	
	/**
	 * @brief constructs and IplImage from a pfm file eith 1,3 channel 32bits floating point per sample
	 *
	 * @param[in] filename  name of the file
	 * @return if parsed correctly with number of bytes matching the hearder  will return the appropriate IplImage  or null otherwise
	 * @todo check for endianess and scale. Assumes little & 1.0
	 */
	IplImage* getIplImageFromPFM(const std::string& filename);

	/**
	 * @brief writes an IplImage to a pfm file
	 *
	 * @param[in] im IplImage
	 * @param[in] filename of the config key
	 * @return the image is valid and the number of bytes written is valid, returns true, otherwise returns false
	 * @note assumes 1.0 scale factor 
	 */
	bool writePFM(const IplImage *im, const std::string& filename);
}
