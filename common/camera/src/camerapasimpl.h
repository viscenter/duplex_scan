/**
 *	@file common/camera/src/camerapasimpl.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */
#pragma once

#include <string>
#include <common/camera/src/icamera.h>
#include <common/type/src/singleton.h>
#include <gphoto2/gphoto2-camera.h>

namespace viz
{

	/** 
	 * @brief Point and shoot camera implementation. 
	 *
	 * This is the point and shoot implementation of the camera interface
	 * <p> Supported platforms: 
	 		<ul>
				<li>Unix: via libghoto2-2-dev (v2.4.8)</li>
			</ul>
	 	</p>
	 */
	class CameraPASImpl : public virtual ICamera , public virtual Singleton<CameraPASImpl>
	{
		/**
		 * @brief so that singleton has access to the protected [con|de]structors
		 */
		friend class Singleton<CameraPASImpl>;
		public:
			virtual bool initialize();
			virtual bool finalize();
			virtual bool getImage(UserParams &p);
			virtual bool setShutterSpeed(UserParams &p);
			virtual std::string getName() const;
			virtual ICamera::CameraType getType() const;
			/**
			 * @brief virtual destructor 
			 */
			virtual ~CameraPASImpl();

			/**
			 * @brief gets the current value set for the specified config key
			 *
			 * @param[in] key config key
			 * @param[in,out] value of the config key
			 * @return if the key is valid
			 */
			bool getCameraConfigValue(const std::string& key,std::string &value);
			/**
			 * @brief sets the value for a config specified key
			 *
			 * @param[in] key config key 
			 * @param[in] value to set the config key 
			 * @return if the key is valid and it was successfull set
			 */
			bool setCameraConfigValue(const std::string& key,std::string value);

		protected:
			/**
			 * @brief protected constructor 
			 */
			CameraPASImpl();
			/**
			 * @brief protected copy constructor 
			 */
			CameraPASImpl(const CameraPASImpl&);
			/**
			 * @brief protected copy operator 
			 */
			CameraPASImpl& operator=(const CameraPASImpl&);
		private:
			Camera *gCamera;
			GPContext *gContext;
			std::string name;
	};


}
