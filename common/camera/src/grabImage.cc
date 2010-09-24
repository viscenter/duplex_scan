
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <boost/program_options.hpp>
#include <common/camera/src/camerapasimpl.h>
#include <common/type/src/userparamsimpl.h>
using namespace std;
using namespace viz;
namespace po = boost::program_options;

int main(int argc, char ** argv)
{
	std::string baseName,extension;
	bool saveColor;
	int bpp;
	vector<string> shutterSpeeds;
	po::options_description desc("Allowed options");
	 desc.add_options()
    ("help,h", "help message")
    ("outputname,o", po::value<string>(&baseName)->default_value("grab"), 
	 "output file base name")
    ("outputextension,e", po::value<string>(&extension)->default_value("jpg"), 
	 "output file extension")
    ("saveColor,c", po::value<bool>(&saveColor)->default_value(false), 
	 "save the captured image in color")
    ("bits-per-pixel,b", po::value<int>(&bpp)->default_value(16), 
	 "bpp for a rawImage")
    ("shutterspeeds,s", po::value< vector<string> >(&shutterSpeeds), "shutter speeds");

	po::positional_options_description p;
	p.add("shutterspeeds", -1);
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);    

	if (vm.count("help")) {
		 cout << desc << "\n";
		 return 0;
	}
	if (vm.count("shutterspeeds")) {
		cout <<"\nSave file format \""<<baseName<<"_"<<"%d."<<extension<<"\"";
		cout << "\nShutter speeds are: " ;
		for(int i=0;i<(int)shutterSpeeds.size(); ++i)
			cout << shutterSpeeds[i] << " ";
		cout <<endl;

	} else {
		cout <<"\nSave file \""<<baseName<<"."<<extension<<"\"";
		 cout << "\nNot varying shutter speeds";
	}

	
	IplImage *im = 0;
	string fname;
	CameraPASImpl * camera =  CameraPASImpl::getInstance();
	if(!camera->initialize())
	{
		cerr<<"\nFailed to initialize camera";
	}
	cout <<"\nInitialized a camera type "<<camera->getType()
				  << " named \""<<camera->getName()<<"\"";
	
	if(!shutterSpeeds.empty())
	{
		for(int i =0;i<(int)shutterSpeeds.size();++i)
		{
			stringstream ss;
			ss << baseName <<"_"<< setfill('0') << setw(4)<<i<<"."<<extension;
			fname = ss.str();
			cout <<"\nSaving "<<fname;
			cout <<".";
			UserParamsSetShutterImpl upss(shutterSpeeds[i]); 
			if(!camera->setShutterSpeed(upss))
			{
				cerr<<"\nFailed to setShutter speed ("
					 <<upss.getValue()<<")";
				camera->finalize();
				return -1;
			}
			cout <<".";
			UserParamsGetImageImpl upgi(fname, &im, false, saveColor, bpp ); 
			if(!camera->getImage(upgi))
			{
				cerr<<". FAILED!";
			}
			else
			{
				cerr<<". done";
			}
		}
	}
	else
	{
		fname = baseName + "." + extension;
		cout <<"\nSaving "<<fname ;
		cout <<".";
		UserParamsGetImageImpl upgi(fname, &im, false, saveColor, bpp ); 
		cout <<".";
		if(!camera->getImage(upgi))
		{
			cerr<<". FAILED!";
		}
		else
		{
			cerr<<". done";
		}
	}


	if(!camera->finalize())
	{
		cerr<<"\nFailed to cleanup  camera";
	}

	return EXIT_SUCCESS;
}
