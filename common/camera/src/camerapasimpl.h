#pragma once

#include <string>
#include <common/camera/src/icamera.h>
#include <common/type/src/singleton.h>
#include <gphoto2/gphoto2-camera.h>

namespace viz
{

	class CameraPASImpl : public virtual ICamera , public virtual Singleton<CameraPASImpl>
	{
		friend class Singleton<CameraPASImpl>;
		public:
				virtual bool initialize();
				virtual bool finalize();
			virtual bool getImage(UserParams &p);
			virtual bool setShutterSpeed(UserParams &p);
			virtual std::string getName() const;
			virtual ICamera::CameraType getType() const;
			virtual ~CameraPASImpl();
			bool getCameraConfigValue(const std::string& key,std::string &value);
			bool setCameraConfigValue(const std::string& key,std::string &value);

		protected:
			CameraPASImpl();
			CameraPASImpl(const CameraPASImpl&);
			CameraPASImpl& operator=(const CameraPASImpl&);
		private:
			Camera *gCamera;
			GPContext *gContext;
			std::string name;
	};


}
