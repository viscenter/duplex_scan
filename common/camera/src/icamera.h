/**
 *	@file common/camera/src/icamera.h
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
			 * @see UserParamsGetImageImpl
			 */
			virtual bool getImage(UserParams &p) =0;

			/**
			 * @brief set the shutter speed of a camera
			 *
			 * pure virtual method to set the shutterspeed of a camera
			 *
			 * @param p user definiable paramaters to set the shutter 
			 * @return true if the shutter speed is successful set
			 * @see UserParamsSetShutterImpl
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
