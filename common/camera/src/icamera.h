#pragma once

#include <string>
#include <common/type/src/userparams.h>

namespace viz
{
	class ICamera
	{
		public:
			enum CameraType {POINT_AND_SHOOT, FIREWIRE_A, FIREWIRE_B, UNKNOWN};

			virtual bool initialize() =0;
			virtual bool finalize() =0;
			virtual bool getImage(UserParams &p) =0;
			virtual bool setShutterSpeed(UserParams &p) =0;
			virtual std::string getName() const =0;
			virtual CameraType getType() const =0;
			virtual ~ICamera(){}

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
				return "";
			}

	};
}
