/**
 *	@file common/camera/src/icamera.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */

/**
 * @mainpage
 * @section intro_sec Introduction
 * <p>
 	<p>This documentation describes the code that is use to preform non invasive, duplex scanning of documents and is part of the <i> EDUCE : Enhanced Digital Unwrapping for Conservation and Exploration</i> project</p>
   <p>
	The goals of this code are
	<ul>
		<li>Let it be easily extensible - uses interfaces allowing the user to define specialization for types, cameras, etc </li>
		<li>Platform independent - the source tree is split by platform where the porting layer is clearly separated from cross platform code The build system that is in place takes care of the   </li>
	</ul>
  </p>
   <p> Supported platforms: 
 		<ul>
			<li>UNIX</li> 
			<li>OSX - TBA</li> 
			<li>WIN - TBA</li> 
		</ul>
	 </p>
  </p>

  @section build_sec Building from Source
   <p>
	  You will need <a href="www.cmake.org">CMAKE</a> which is a cross platform build utility and the appropriate platform independent/dependent libraries
	 <p>
 		<br /> Platform independent library
 		<ul>
			<li>libboost-dev v1.40 (incude filesystem program-options)</li> 
		</ul>
 		<br /> Platform dependent library (dev platform (ubuntu x86 10.04 [lucid]) 
 		<ul>
			<li>g++-4.4</li> 
			<li>libghoto2-2-dev v2.4.8</li> 
			<li>dcraw  0.86-1</li> 
			<li>ufraw  0.16-1</li> 
			<li>opencv2.1 -(included with source in external/platform/nix/opencv2.1)</li> 
		</ul>
		<br />Run:
		<br />
		<code>
			<br />mkdir bin; cd bin; cmake (location of source)
			<br /><i> cmake options: -DDEBUG (debug build)</i>
			<br /><i> to get the backtext of a document run scripts/getRearText.sh</i>
		</code>
		<br /><br />And your done.
	  </p>
	</p>
  @section camera_sec Camera setup 
   <p>
		Camera Setup (tested on Canon 20D) connected via USB on linux
 		<ul>
			<li>Camera mounted on a tripod where the view is set on <i>whole</i> area of the document that has backtext that needs to be extracted.</li> 
			<li>Shooting mode - Manual</li> 
			<li>Communication mode - Normal</li> 
			<li>White Balance - Auto</li> 
			<li>Quality - RAW</li> 
			<li>Focus - Manual</li>
			<li>Manually set the shutter and aperture it's not too bright [brightness meter should be centered when the backlight is in view]</li>
		</ul>
	</p>

  @section run_sec Extracting back text 
  Each part of the process is split up in to programs. However there is a script 'scripts/getRearText.sh' that automates the process
	<br />
  @subsection aquistion_subsec Aquisition 
  This process requires 3 raw images which uses grabImage 
  <code>
	<br />
	  <br />common/camera/grabImage -h 
	  <br />Allowed options:
	  <br />-h [ --help ]                            help message
	  <br />-o [ --outputname ] arg (=capturedImage) output file base name
	  <br />-e [ --outputextension ] arg (=raw)      output file extension
	  <br />-c [ --saveColor ] arg (=0)              save the captured image in color
	  <br />-b [ --bits-per-pixel ] arg (=16)        bpp for a rawImage
	  <br />-s [ --shutterspeeds ] arg               shutter speeds
  </code>
  	<p>
	<i>
     <br /> This program will also allow one to capture a series of images varying shutter speeds using values similar to what <code>gphoto2 --get-config=shutterspeed</code> returns
	  <br /> If the camera is is raw mode the -e option should either be raw or cr2
	</i>
	</p>

  @subsection stat_subsec Statistic Computation 
  This process will use the images from the previous step to extract the back text as backText.pfm variance.pfm
  <code>
	<br />
	<br />common/utils/duplexScanStats -h
	<br />Allowed options:
  	<br />-h [ --help ]                 help message
   <br />-b [ --backlight-only ] arg   backlight only
   <br />-d [ --doc-backlit ] arg      document with backlight
   <br />-n [ --doc-no-backlight ] arg document without backlight
   <br />--bits-per-pixel arg (=16)    bpp for a rawImage
   <br />-v [ --verbose ]              verbose output
   <br />-p [ --generate-plots ]       generate gnu plot files of images backText & variance 
   <br />-s [ --save-intermediates ]   save intermediate images
  </code>
  	<p>
	<i>
	<code>
	<br /> This will output the following files:
	<ul>
	<li>variance(Abs).pfm  - is the variation of the opacity of the backlit document and the back light (floating point image) (Abs is the absolute difference)</li>
	<li>backTextEnhanced(Abs).pfm  - is the variation without the fore lit document which should contain the rear text(floating point image) (Abs is the absolute difference)</li>
	</ul>
	</code>
	</i>
	</p>

  @subsection stat_seg Segmentation 
  This process will use the images from the previous step to allow the user to specify thresholds using different thresholding methods (pyramid segmentation is commented out because of memory issue will large files) to cleanly extract the back text producing a mask to be used with the original document to specify the back text
  <code>
  <br />common/utils/segment -h
  <br />Allowed options:
  <br />-h [ --help ]                            help message
  <br />-f [ --input-file ] arg                  input file
  <br />-d [ --document ] arg                    original document
  <br />-o [ --output-file ] arg                 output mask file
  <br />-b [ --bpp ] arg (=16)                   bits per pixel for raw
  <br />--low-threshold arg (=0.25)              threshold value [0-1]
  <br />--high-threshold arg (=0.5)              threshold value [0-1]
  <br />-r [ --threshold-resolution ] arg (=100) threshold resolution. Greater the 
                                           number, smaller the change 100==1% 
                                           200==0.5% [0-500]
  <br />-i [ --interactive ]                     interactive segmentation
  <br />-t [ --threshold-segmentation ]          bi-level thresholding for image 
                                           segmentation
  </code>

	<br /> You can specify ROI's on the doc and input-file images  
	<br /> To skip specifing values interactively, provide both low/high thresholds with respect to the threshold resolution
  @subsection stat_backText Back text extraction 
  This process applies the mask that was generated from the previous code specifying the parts of the document that are back text only. The mask(grey level image) can be hand tweaked to get better results)
  <code>
  <br />common/utils/backText
  <br />backText [document] [mask] [outputfile]
  <br /> [document]    backlit document 
  <br /> [mask]        mask file 
  <br /> [outputfile]  output file 
  </code>
	
*/


#pragma once

#include <string>
#include <common/type/src/userparams.h>

/**
 * @brief viz center namespace 
 */
namespace viz
{
	/** 
	 * @brief abstract camera interface. 
	 *
	 * This describes the main camera interface which is (intially) limited to 
	 * <ul>
			<li>initializing the camera</li>
			<li>setting the shutter speed</li>
			<li>grabbing an image</li>
		</ul>
		@see CameraPASImpl for the point and shoot implementation 
		@see CameraFirewireImpl firewire implementation (currently not supported)
	 */
	class ICamera
	{
		/**
		 * @brief types of cameras supported
		 */
		public:
			enum CameraType {
								  ///dslr, point and shoot
								  POINT_AND_SHOOT, 
								  ///IIDC firewire A cameras
								  FIREWIRE_A, 
								  ///IIDC firewire B cameras
								  FIREWIRE_B, 
								  ///unknown camera
								  UNKNOWN}; 
			
			/**
			 * @brief camera initialization
			 *
			 * pure virtual method to intialize a camera
			 * All initialization connection code should go in here
			 * @return true if a camera connection is established.
			 */
			virtual bool initialize() =0;

			/**
			 * @brief camera de-initialization
			 *
			 * pure virtual method to disconnect from a camera
			 * All disconnect code should go in here 
			 * @return true if a camera has been disconnected.
			 */
			virtual bool finalize() =0;

			/**
			 * @brief get an image from the camera
			 *
			 * pure virtual method to get an image from the camera
			 *
			 * @param p user definiable paramaters to get an image
			 * @return true if on a successful image grab
			 */
			virtual bool getImage(UserParams &p) =0;

			/**
			 * @brief set the shutter speed of a camera
			 *
			 * pure virtual method to set the shutterspeed of a camera
			 *
			 * @param p user definiable paramaters to set the shutter 
			 * @return true if the shutter speed is successful set
			 */
			virtual bool setShutterSpeed(UserParams &p) =0;

			/**
			 * @brief get the name of the camera
			 *
			 * pure virtual method to get the name of the camera
			 *
			 * @return the camera's name
			 */
			virtual std::string getName() const =0;

			/**
			 * @brief get the camera type
			 *
			 * pure virtual method to get type of camera 
			 *
			 * @return the type of camera
			 */
			virtual CameraType getType() const =0;

			/**
			 * @brief virtual destructor
			 */
			virtual ~ICamera(){}

			/**
			 * @brief get camera type as a string
			 *
			 * helper function to translate the CameraType to a string
			 * 
			 * @return camera type as a string
			 */
			static std::string getTypeName(const CameraType& ct)
			{
				switch (static_cast<int>(ct))
				{
					case POINT_AND_SHOOT:
						return "Point and shoot";
					break;

					case FIREWIRE_A:
						return "Firewire A";
					break;

					case FIREWIRE_B:
						return "Firewire B";
					break;
				
					case UNKNOWN:
					default:
						return "Unknown";
					break;

				}
				return "Unknown";
			}

	};
}
