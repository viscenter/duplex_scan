
#include <string>
#include <iostream>
#include <memory>
#include <typeinfo>
#include <fcntl.h>

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
		if( *param.getImage() !=0 )
			cout <<"\n\t as an IplImage";
#endif
		if(param.getFilename().empty() && *param.getImage()  ) 
		{
			cerr <<"\nGetImage(), invalid parameter. Specify name or image ";
			return false;
		}

		string cFormat;
		if(!getCameraConfigValue("imageformat", cFormat))
		{
			cerr <<"\nGetImage(), failed to get image format ";
			return false;
		}
#if DEBUG
		cout <<"\nCurrent image format :" <<cFormat;
#endif
		bool cameraRAWFormat = false;
		if(cFormat.find_last_of("RAW") == string::npos ||
			cFormat.find_last_of("RAW") == string::npos)
			cameraRAWFormat = true;

		bool saveRawFormat = false;
		if(param.getFilename().find_last_of(".CR2") == string::npos ||
			param.getFilename().find_last_of(".cr2") == string::npos ||
			param.getFilename().find_last_of(".RAW") == string::npos ||
			param.getFilename().find_last_of(".raw") == string::npos)
		{
			saveRawFormat = true;
			cout <<"\nSaverawformat";
		}
		else
			cout <<"\n!Saverawformat";


		
		string cameraSaveFile("foo.jpg");
		if(saveRawFormat && *param.getImage() != 0)
		{
			cvReleaseImage(param.getImage());
			*param.getImage() = 0;
			cameraSaveFile = "foo.raw";
		}

		if(saveRawFormat && param.getFilename().empty())
		{
			cerr <<"\nGetImage():If saving raw image format, specify a filename";
			return false;
		}

		CameraFile *canonfile;
		CameraFilePath camera_file_path;
			
		cout <<"Capturing.\n";

		/* NOP: This gets overridden in the library to /capt0000.jpg */
		strcpy(camera_file_path.folder, "/");
		strcpy(camera_file_path.name, cameraSaveFile.c_str());

		int retval = gp_camera_capture(gCamera, GP_CAPTURE_IMAGE, &camera_file_path, gContext);
		
		int fd = open(param.getFilename().c_str(), O_CREAT | O_WRONLY, 0644);
		retval = gp_file_new_from_fd(&canonfile, fd);
		retval = gp_camera_file_get(gCamera, camera_file_path.folder, camera_file_path.name,
				  GP_FILE_TYPE_NORMAL, canonfile, gContext);
			
		cerr<<"\nDeleting.\n";
		retval = gp_camera_file_delete(gCamera, camera_file_path.folder, camera_file_path.name,
				gContext);
			
		gp_file_free(canonfile);
		
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
