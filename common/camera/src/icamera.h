/**
 *	@file common/camera/src/icamera.h
 * Camera Interface
 * 
 * This describes the main camera interface which is (intially) limited to 
 * <ul>
 	<ol>initializing the camera</ol>
 	<ol>setting the shutter speed</ol>
 	<ol>grabbing an image</ol>
	</ul>
	@see CameraPASImpl for the point and shoot implementation and @see CaemrafirewireImpl firewire implementation
 *
 */

#pragma once

#include <string>
#include <common/type/src/userparams.h>

/**
 * viz namespace
 */
namespace viz
{
	/**
	 * Abstract class describe the camera interface
	 */
	class ICamera
	{
		/**
		 * types of cameras supported
		 */
		public:
			enum CameraType {POINT_AND_SHOOT, FIREWIRE_A, FIREWIRE_B, UNKNOWN};
			
			/**
			 * pure virtual method to intialize a camera
			 *
			 * All initialization connection code should go in here
			 * @return true if a camera connection is established.
			 */
			virtual bool initialize() =0;

			/**
			 * pure virtual method to disconnect from a camera
			 *
			 * All disconnect code should go in here 
			 * @return true if a camera has been disconnected.
			 */
			virtual bool finalize() =0;

			/**
			 * pure virtual method to get an image from the camera
			 *
			 * @param p user definiable paramaters to get an image
			 * @return true if on a successful image grab
			 * @see UserParamsGetImageImpl
			 */
			virtual bool getImage(UserParams &p) =0;

			/**
			 * pure virtual method to set the shutterspeed of a camera
			 *
			 * @param p user definiable paramaters to set the shutter 
			 * @return true if the shutter speed is successful set
			 * @see UserParamsSetShutterImpl
			 */
			virtual bool setShutterSpeed(UserParams &p) =0;

			/**
			 * pure virtual method to get the name of the camera
			 *
			 * @return the camera's name
			 */
			virtual std::string getName() const =0;

			/**
			 * pure virtual method to get type of camera 
			 *
			 * @return the type of camera
			 */
			virtual CameraType getType() const =0;

			/**
			 * virtual destructor
			 */
			virtual ~ICamera(){}

			/**
			 * help function to translate the @see CameraType to a string
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
