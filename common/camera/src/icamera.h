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
		<li>Platform independent - the source tree is split by platform where the porting layer is clearly separate from cross platform code The build system that is in place takes care of the   </li>
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
			<li>opencv2.1 -(included with source in external/platform/nix/opencv2.1)</li> 
		</ul>
		<br />Run:
		<br />
		<code>
			<br />mkdir bin; cd bin; cmake (location of source)
			<br /><i> cmake options: -DDEBUG (debug build)</i>
		</code>
		<br /><br />And your done.
	  </p>
	</p>
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
