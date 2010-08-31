
#include <string>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <fcntl.h>


#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <common/camera/src/camerapasimpl.h>
#include <common/type/src/userparamsimpl.h>


using namespace viz;
using namespace std;


CameraPASImpl::CameraPASImpl() 
	: gCamera(0), gContext(0)
{
#if DEBUG
	cout <<"\nCameraPASImpl()";
#endif
	
}

CameraPASImpl::~CameraPASImpl()
{
#if DEBUG
	cout <<"\n!CameraPASImpl()";
#endif
}
extern "C" 
{
bool CameraPASImpl::initialize()
{
	gContext = gp_context_new (); /* see context.c */
	gp_camera_new (&gCamera);
	if( gp_camera_init(gCamera, gContext)  < GP_OK )
	{
		cerr<<"\nFailed to initialize camera ";
		gp_camera_free (gCamera);
		return false;
	}

	/* Simple query the camera summary text */
	CameraText	text;
	if(gp_camera_get_summary (gCamera, &text, gContext) < GP_OK)
	{
		cerr<<"Camera failed retrieving summary.\n";
		gp_camera_free (gCamera);
		return false;
	}
#if DEBUG
	cout <<"\nCamera summary "<<endl << text.text<<endl;
#endif

	if(!getCameraConfigValue("model", name))
	{
		cerr<<"\nFailed to getCameraName()\n";
		gp_camera_free (gCamera);
		return false;
	}


	/* Simple query of a string configuration variable. */

	return true;
}
bool CameraPASImpl::finalize()
{
	if(0 != gCamera && 0 != gContext)
	{
		gp_camera_exit (gCamera, gContext);
		gp_camera_free (gCamera);
		return true;
	}
	else
	{
		cerr<<"\nFailed finalization. Camera not initialized correctely.";
		return false;
	}
}
}
bool CameraPASImpl::getImage(UserParams &p)
{
	try
	{
		UserParamsGetImageImpl &param = dynamic_cast<UserParamsGetImageImpl&>(p);
#if DEBUG
		cout <<"\nGetting image from camera:";
		if(!param.getFilename().empty())
			cout <<"\n\tas a saved file \"" <<param.getFilename() <<"\"";
		if( param.loadImageData() )
			cout <<"\n\t as an IplImage as well";
#endif
		if(param.getFilename().empty() && !param.loadImageData()  ) 
		{
			cerr <<"\nGetImage(), invalid parameter. Specify name or getImageData ==true ";
			return false;
		}

		string cFormat;
		if(!getCameraConfigValue("imageformat", cFormat))
		{
			cerr <<"\nGetImage(), failed to get camera save image format ";
			return false;
		}
#if DEBUG
		cout <<"\nCurrent image format :" <<cFormat;
#endif

		bool cameraRawFormat = false;
		if(cFormat.find("RAW") != string::npos ||
			cFormat.find("raw") != string::npos)
		{
			cameraRawFormat = true;
#if DEBUG
			cout <<"\nRAW Camera format";
#endif
		}
#if DEBUG
		else
			cout <<"\n!RAW Camera format";
#endif

		bool saveRawFormat = false;
		if(param.getFilename().empty())
		{
			saveRawFormat = cameraRawFormat;
			param.setFilename("foo.jpg");
			if(saveRawFormat)
				param.setFilename( "foo.CR2");
			cout <<"\nNo savefile name: using \""<<param.getFilename()<<"\"";
		}
		else
		{
			if(param.getFilename().rfind(".CR2") != string::npos ||
				param.getFilename().rfind(".cr2") != string::npos ||
				param.getFilename().rfind(".RAW") != string::npos ||
				param.getFilename().rfind(".raw") != string::npos)
			{
				saveRawFormat = true;
#if DEBUG
				cout <<"\nRAW Save format";
#endif
			}
#if DEBUG
			else
				cout <<"\n!RAW Save format";
#endif
		 }

		//cout << endl << cameraRawFormat << " == "<< saveRawFormat;


		if(cameraRawFormat != saveRawFormat)
		{
			cerr<<"\nFormat mismatch. Save name and current camera save mode not the same.";
			cerr<<"\nEither change filename extension (raw/cr2 or jpg/tiff/ppm) or save mode on the camera to match";
			return false;
		}
		
		string cameraSaveFile("foo.jpg");
		if(saveRawFormat)
			cameraSaveFile = "foo.CR2";

		if(saveRawFormat && param.getFilename().empty())
		{
			cerr <<"\nGetImage():If saving raw image format, specify a filename";
			return false;
		}

		CameraFile *canonfile;
		CameraFilePath camera_file_path;
			
#if DEBUG
		cout <<"Capturing.\n";
#endif

		/* NOP: This gets overridden in the library to /capt0000.jpg */
		strcpy(camera_file_path.folder, "/");
		strcpy(camera_file_path.name, cameraSaveFile.c_str());

		int retval = gp_camera_capture(gCamera, GP_CAPTURE_IMAGE, &camera_file_path, gContext);
		int fd = open(param.getFilename().c_str(), O_CREAT | O_WRONLY, 0644);
		if(fd < 0)
		{
			cerr <<"\nGetImage(): failed to open file \""<<param.getFilename()
				  << "\" for saving image";
			return false;
		}
		
		retval = gp_file_new_from_fd(&canonfile, fd);
		retval = gp_camera_file_get(gCamera, camera_file_path.folder, camera_file_path.name,
				  GP_FILE_TYPE_NORMAL, canonfile, gContext);
			
#if DEBUG
		cout<<"\nDeleting image ... off the camera\n";
#endif
		retval = gp_camera_file_delete(gCamera, camera_file_path.folder, camera_file_path.name,
				gContext);
			
		gp_file_free(canonfile);
		close(fd);
		if(param.loadImageData() && !cameraRawFormat)
		{
#if DEBUG
			cout <<"\nloading image file \""<<param.getFilename()<<"\"";
#endif
			*(param.getImage()) = cvLoadImage(param.getFilename().c_str(), CV_LOAD_IMAGE_UNCHANGED);
			if(*param.getImage() == 0)
			{
				cerr <<"\nFailed to load image data";
				return false;
			}
		}
		else
		{
			cerr
				<<"\nNot loading image data as raw format not currently supported"
				<<"\nimage file \""<<param.getFilename()<<"\" saved though";
			return false;
		}
			
	}
	catch(bad_cast& b)
	{
		cerr<<"\nFailed to dynamic_cast UserParam& to UserParamsGetImageImpl&";
		return false;
	}


	return true;
}

bool CameraPASImpl::getCameraConfigValue(const string& key, string &value)
{
	value = "";
	if(key.empty())
	{
		cout <<"\nGetCameraConfigValue(), invalid config key value. ";
		return false;
	}

	CameraWidget      *widget = NULL, *child = NULL;
   CameraWidgetType  type;
   int         ret;
   char        *val;

   ret = gp_camera_get_config(gCamera, &widget, gContext);
   if (ret < GP_OK) {
      cerr<<"\ngetCameraConfigValue failed("<< ret <<")";
      return false;
   }

	ret = gp_widget_get_child_by_name (widget, key.c_str(), &child);
   if (ret < GP_OK)
	{
      ret = gp_widget_get_child_by_label (widget, key.c_str(), &child);
	}

   if (ret < GP_OK) {
      cerr<<"\ngetCameraConfigValue failed lookup("<< ret <<")";
		return false;
   }

   /* This type check is optional, if you know what type the label
    * has already. If you are not sure, better check. */
   ret = gp_widget_get_type (child, &type);
   if (ret < GP_OK) {
      cerr<<"\ngetCameraConfigValue failed get widget type("<< ret <<")";
		return false;
   }
   switch (type) {
        case GP_WIDGET_MENU:
        case GP_WIDGET_RADIO:
        case GP_WIDGET_TEXT:
      break;
   default:
      ret = GP_ERROR_BAD_PARAMETERS;
      cerr<<"\ngetCameraConfigValue bad widget type("<< ret <<")";
		return false;
   }

   /* This is the actual query call. Note that we just
    * a pointer reference to the string, not a copy... */
   ret = gp_widget_get_value (child, &val);
   if (ret < GP_OK)
	{
      cerr<<"\ngetCameraConfigValue failed to query widget value ("<< ret <<")";
		return false;
	}

	value = val;
	return true;
}


bool CameraPASImpl::setShutterSpeed(UserParams &p)
{
	try
	{
		UserParamsSetShutterImpl &param = 
			dynamic_cast<UserParamsSetShutterImpl&>(p);
		string key("shutterspeed"), cur;
#if DEBUG
		cout <<"\nSetting shutter to :"<<param.getValue();
		getCameraConfigValue(key, cur);
		cout << " current value :"<<cur <<"\"";
#endif
		if(param.getValue() == cur)
		{
			cerr<<"\nNot changing shutterspeed. New value is same as old";
			return true;
		}

		if( !setCameraConfigValue(key, param.getValue()))
		{
			cerr<<"\nError occured changing shutter value ";
			return false;
		}

		if(param.getValue() == cur )
		{
			//cerr<<"\n\""<<param.getValue()<<"\" == \""<<cur<<"\"";
			cerr<<"\nfailed to change shutterspeed";
			return false;
		}

		cout <<"\nShutter speed changed"<<param.getValue();
		return true;
	}
	catch(bad_cast& b)
	{
		cerr<<"\nFailed to dynamic_cast UserParam& to UserParamsSetShutterImpl&";
		return false;
	}

	
}
bool CameraPASImpl::setCameraConfigValue(const string& key, string value)
{
	if(key.empty())
	{
		cout <<"\nsetCameraConfigValue(), invalid config key value. ";
		return false;
	}
	CameraWidget      *widget = NULL, *child = NULL;
   CameraWidgetType  type;
   int         ret;

   ret = gp_camera_get_config (gCamera, &widget, gContext);
   if (ret < GP_OK) {
      cerr<< "\ncamera_get_config failed:("<<ret<<")";;
      return false;
   }
	
	ret = gp_widget_get_child_by_name (widget, key.c_str(), &child);
   if (ret < GP_OK)
	{
      ret = gp_widget_get_child_by_label (widget, key.c_str(), &child);
	}

   if (ret < GP_OK) {
      cerr<<"\ngetCameraConfigValue failed lookup("<< ret <<")";
		return false;
   }

   /* This type check is optional, if you know what type the label
    * has already. If you are not sure, better check. */
   ret = gp_widget_get_type (child, &type);
   if (ret < GP_OK) {
      cerr<<"\nwidget get type failed:("<<ret<<")";
	   gp_widget_free (widget);
		return false;
   }
   switch (type) {
        case GP_WIDGET_MENU:
        case GP_WIDGET_RADIO:
        case GP_WIDGET_TEXT:
      break;
   default:
      cerr<<"\nwidget has bad type("<<type<<")";
	   gp_widget_free (widget);
		return false;
   }

   /* This is the actual set call. Note that we keep
    * ownership of the string and have to free it if necessary.
    */
   ret = gp_widget_set_value (child, value.c_str());
   if (ret < GP_OK) {
      cerr<<"\ncould not set widget value ("<<ret<<")";
	   gp_widget_free (widget);
		return false;
   }
   /* This stores it on the camera again */
   ret = gp_camera_set_config (gCamera, widget, gContext);
   if (ret < GP_OK) {
      cerr<<"\ncamera set config failed ("<<ret<<")";
	   gp_widget_free (widget);
		return false;
   }

	return true ;
}

std::string  CameraPASImpl::getName() const
{
	return name;
}

ICamera::CameraType CameraPASImpl::getType() const
{
	return POINT_AND_SHOOT;
}
