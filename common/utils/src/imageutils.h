/**
 *	@file common/utils/src/imageutils.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */
#pragma once
 
#include <string>
#include <fstream>
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
	
	/**
	 * @brief constructs and IplImage from a raw image

	 * @param[in] filename  name of the file
	 * @param[in] inColor  will return the image in color
	 * @param[in] bpp  bpp so it can return a floating point image [0-1]
	 * @return if parsed correctly will return the appropriate IplImage or null otherwise
	 * @warning platform dependent. Uses ufraw-batch in nix
	 */
	IplImage* getIplImageFromRAW(const std::string& filename, bool inColor, int bpp);

	/**
	 * @brief writes an IplImage as a gnuplot file x y value
	 *
	 * @param[in] im IplImage  (only first channel used as value for multichannel image
	 * @param[in] filename to write data to
	 * @return the image is valid and the filesize is writable
	 */
	bool writePlot(const IplImage* im, const std::string filename);

	template<typename T>
	bool dumpData(const IplImage *im, const T *data, std::ofstream& fout);
}
