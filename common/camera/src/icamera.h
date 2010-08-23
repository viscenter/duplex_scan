#pragma once

#include <string>
#include <common/type/src/userparams.h>

namespace viz
{
	class ICamera
	{
		public:
			enum CameraType {POINT_AND_SHOOT, FIREWIRE_A, FIREWIRE_B, CUSTOM};

			virtual bool initialize() =0;
			virtual bool finalize() =0;
			virtual bool getImage(UserParams &p) =0;
			virtual bool setShutterSpeed(UserParams &p) =0;
			virtual std::string getName() const =0;
			virtual CameraType getType() const =0;
			virtual ~ICamera(){}

	};
}
