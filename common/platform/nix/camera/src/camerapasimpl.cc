
#include <string>
#include <iostream>
#include <memory>
#include <typeinfo>

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
		if(param.getFilename().empty())
			cout <<"\n\tas a saved file \"" <<param.getFilename() <<"\"";
		else if( param.getImage() !=0 )
			cout <<"\n\t as an IplImage";
#endif
		if(param.getFilename().empty() && param.getImage() ) 
		{
			cout <<"\nGetImage(), invalid parameter. Specify name or image ";
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
		cout <<"\nSetting shutter to :"<<param.getValue() <<"\"";
		getCameraConfigValue(key, cur);
		cout << " current value :"<<cur <<"\"";
#endif
		cur = param.getValue();
		return setCameraConfigValue(key,cur);
		
	}
	catch(bad_cast& b)
	{
		cerr<<"\nFailed to dynamic_cast UserParam& to UserParamsSetShutterImpl&";
		return false;
	}

	
}
bool CameraPASImpl::setCameraConfigValue(const string& key, string &value)
{
	if(key.empty())
	{
		cout <<"\nsetCameraConfigValue(), invalid config key value. ";
		return false;
	}

	CameraWidget      *widget = NULL, *child = NULL;
   CameraWidgetType  type;
   int         ret;
   char*        val;

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
		gp_widget_free(widget);
		return false;
   }

   /* This type check is optional, if you know what type the label
    * has already. If you are not sure, better check. */
   ret = gp_widget_get_type (child, &type);
   if (ret < GP_OK) {
      cerr<<"\nwidgetGetType failed get widget type("<< ret <<")";
		gp_widget_free(widget);
		return false;
   }
   switch (type) {
        case GP_WIDGET_MENU:
#if DEBUG
			cout <<endl << "menu type";
#endif
      break;
        case GP_WIDGET_RADIO:
#if DEBUG
			cout <<endl << "Radio type";
#endif
      break;
        case GP_WIDGET_TEXT:
#if DEBUG
			cout <<endl << "text type";
#endif
      break;
   default:
      ret = GP_ERROR_BAD_PARAMETERS;
      cerr<<"\nwidgetGetType bad widget type("<< ret <<")";
		gp_widget_free(widget);
		return false;
   }

	const char* choiceName;
   ret = gp_widget_get_name (child, &choiceName);
	cerr<<"\nName "<<choiceName;
	/*
   ret = gp_widget_get_choice (child, choice, &choiceName);
   if (ret < GP_OK) {
      cerr<<"\nwidgetGetChoice failed get choice ("<< ret <<")";
		return false;
   }
	*/
   ret = gp_widget_get_value (child, &val);
   if (ret < GP_OK) {
      cerr<<"\nwidgetgettValue failed to get widget value ("<< ret <<")";
	   gp_widget_free(widget);
		return false;
   }
	cout <<"\n\""<<choiceName<<"\"-> "<<val;
	const char* setVal =value.c_str();
	//cout <<"\n\""<<choiceName<<"\"-> "<<choice;
	//val =  static_cast<int>(param.getValue());
   ret = gp_widget_set_value (child, &setVal);
   if (ret < GP_OK) {
      cerr<<"\nwidgetSetValue failed to set widget value ("<< ret <<")";
		gp_widget_free(widget);
		return false;
   }

   ret = gp_camera_set_config(gCamera, widget, gContext);
   if (ret < GP_OK) {
      cerr<<"\ngetSetConfig failed to save widget value ("<< ret <<")";
	   gp_widget_free(widget);
		return false;
   }

	//cout <<"\n\""<<choiceName<<"\"-> "<<choice;
   ret = gp_widget_get_value (child, &val);
   if (ret < GP_OK) {
      cerr<<"\nwidgetgettValue failed to get widget value ("<< ret <<")";
	   gp_widget_free(widget);
		return false;
   }
	cout <<"\n\""<<choiceName<<"\"-> "<<val;
	gp_widget_free(widget);
	return (value == val) ;
}

std::string  CameraPASImpl::getName() const
{
	return name;
}

ICamera::CameraType CameraPASImpl::getType() const
{
	return POINT_AND_SHOOT;
}
