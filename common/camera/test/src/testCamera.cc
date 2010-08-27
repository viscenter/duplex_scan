
#include <common/camera/src/camerapasimpl.h>
//#include <common/camera/src/camerafirewireimpl.h>
#include <string>
#include <iostream>
#include <vector>
#include <opencv/cv.h>

using namespace std;
using namespace viz;

int main(int argc, char ** argv)
{

	bool failure = false;
	vector<ICamera*> cameras;
	//cameras.push_back( CameraFirewireImpl::getInstance());
	cameras.push_back( CameraPASImpl::getInstance());

	for(vector<ICamera*>::iterator it = cameras.begin(); it != cameras.end();
			++it)
	{
		ICamera::CameraType ct = (*it)->getType();
		string ctn = ICamera::getTypeName(ct);
		if(!(*it)->initialize())
		{
			cerr<<"\nFailed to initialize "<<ctn<<" camera";
			failure = true;
			continue;
		}
		else
			cout <<"\nInitialized a camera type "<< ctn 
				  << "with name \""<<(*it)->getName()<<"\"";

		if(!(*it)->finalize())
		{
			cerr<<"\nFailed to cleanup "<< ctn <<" camera";
			failure = true;
		}
		else
			cout<<"\nCleaned up "<< ctn <<" camera";;

	}

	return failure;
	return EXIT_SUCCESS;
}
