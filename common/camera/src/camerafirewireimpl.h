#pragma once

#include <string>

#include <dc1394/camera.h>

#include <common/camera/src/icamera.h>
#include <common/type/src/singleton.h>

namespace viz
{

	class CameraFirewireImpl : public virtual ICamera , public virtual Singleton<CameraFirewireImpl>
	{
		friend class Singleton<CameraFirewireImpl>;
		public:
				virtual bool initialize();
				virtual bool finalize();
			virtual bool getImage(UserParams &p);
			virtual bool setShutterSpeed(UserParams &p);
			virtual std::string getName() const;
			virtual ICamera::CameraType getType() const;
			virtual ~CameraFirewireImpl();

		protected:
			CameraFirewireImpl();
			CameraFirewireImpl(const CameraFirewireImpl&);
			CameraFirewireImpl& operator=(const CameraFirewireImpl&);
		private:
			std::string name;
			dc1394_t *dcHandle;
			dc1394camera_list_t *dcList;
			dc1394camera_t *dcCamera;
			dc1394error_t dcerr;
	};


}
