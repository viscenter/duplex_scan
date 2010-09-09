

#include <string>
#include <common/type/src/userparamsimpl.h>


using namespace std;
namespace viz
{

UserParamsGetImageImpl::UserParamsGetImageImpl(std::string& filename, IplImage **image, bool getImageData, bool saveColor, int bpp)
	:
		bpp(bpp),
		getImageData(getImageData), 
		saveColor(saveColor), 
		image(image), 
		filename(filename)
{
}

UserParamsGetImageImpl::~UserParamsGetImageImpl()
{
}

string UserParamsGetImageImpl::getFilename() const
{
	return filename;
}

IplImage** UserParamsGetImageImpl::getImage() 
{
	return image;
}

UserParamsSetShutterImpl::UserParamsSetShutterImpl(const std::string& val)
	:
		value(val)
{
}

UserParamsSetShutterImpl::~UserParamsSetShutterImpl()
{
}

std::string UserParamsSetShutterImpl::getValue() const
{
	return value;
}


}//namespace viz
