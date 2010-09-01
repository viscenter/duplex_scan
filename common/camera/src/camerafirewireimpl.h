/**
 *	@file common/camera/src/camerafirewireimpl.h
 * @author Danny S. Crasto
 * @date 08/2010
 * @version 1.0
 */
#pragma once

#include <string>

#include <dc1394/camera.h>

#include <common/camera/src/icamera.h>
#include <common/type/src/singleton.h>

namespace viz
{

	/** 
	 * @brief firewire camera implementation. 
	 *
	 * This is the firewire implementation of the camera interface
	 * <p> Supported platforms: 
	 		<ul>
				<li><b>N/A</b></li>
			</ul>
	 	</p>
	 * @warning <b><i>currently not supported under linux(ubuntu 10.04(lucid) because of dc1394/libghoto2-2 library conflict</i></b> 
	 */
	class CameraFirewireImpl : public virtual ICamera , public virtual Singleton<CameraFirewireImpl>
	{

		/**
		 * @brief so that singleton has access to the protected [con|de]structors
		 */
		friend class Singleton<CameraFirewireImpl>;
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
			virtual ~CameraFirewireImpl();

		protected:
			/**
			 * @brief protected constructor 
			 */
			CameraFirewireImpl();
			/**
			 * @brief protected copy constructor 
			 */
			CameraFirewireImpl(const CameraFirewireImpl&);
			/**
			 * @brief protected copy operator 
			 */
			CameraFirewireImpl& operator=(const CameraFirewireImpl&);
		private:
			std::string name;
			dc1394_t *dcHandle;
			dc1394camera_list_t *dcList;
			dc1394camera_t *dcCamera;
			dc1394error_t dcerr;
	};


}
