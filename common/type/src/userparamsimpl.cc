

#include <string>
#include <common/type/src/userparamsimpl.h>


using namespace std;
namespace viz
{

UserParamsGetImageImpl::UserParamsGetImageImpl(std::string& filename, IplImage **image)
	:
		filename(filename), image(*image)
{

}

UserParamsGetImageImpl::~UserParamsGetImageImpl()
{

}

string UserParamsGetImageImpl::getFilename() const
{
	return filename;
}

IplImage* UserParamsGetImageImpl::getImage() const
{
	return image;
}

UserParamsSetShutterImpl::UserParamsSetShutterImpl(double value)
	:
		value(value)
{
}

UserParamsSetShutterImpl::~UserParamsSetShutterImpl()
{
}

double UserParamsSetShutterImpl::getValue() const
{
	return value;
}


}//namespace viz
