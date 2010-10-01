#!/bin/bash

DIR=$1
EXE="common/camera/grabImage"

if [ ! -e "$EXE" ] ; then
	"Can't find image grabber executable \"$EXE\""
	exit -1
fi

if [ "$DIR" == "" ] ; then
	DIR="./"
elif [ ! -d "$DIR" ] ; then
	mkdir -p $DIR
fi

echo "This will grab 3 images."
echo "1) backlitDoc.raw - an image of the backlight ON behind the document"
echo "2) doc.raw - an image of the backlight OFF behind the document "
echo "3) backlight.raw - an image of the backlight only"
echo "Saving images to $DIR"
echo
echo "Connect the camera via USB and turn it on to capture mode"
echo "Make sure the camera is:"
echo " - not mounted by a system automount"
echo " - communication mode is no PTP"
echo " - set to capture in raw mode"
echo " - set to capture in manual mode"
echo " - set to capture with auto white balance"
echo " - the camera is not in auto focus and correctly focused"
echo " - the camera is shutter/aperture is set so that it's not too bright [brightness meter should be centered when the backlight is in view]"
echo "Hit Enter to continue"
read IN

echo "Grabbing backlit document image .. \"$DIR/backlitDoc.raw\"" 
echo "Place the backlight behind the document and switch it on"
echo "Hit Enter to capture image"
read IN
$EXE -o $DIR/backlitDoc -e raw 
if [ "$?" != "0" ]; then
   echo "Failed to grab image" 1>&2
	exit -1
fi
echo "done"
echo ""

echo "Grabbing document image without backlight .. \"$DIR/doc.raw\"" 
echo "Switch the backlight off"
echo "Hit Enter to capture image"
read IN
$EXE -o "$DIR"/doc -e raw
if [ "$?" != "0" ]; then
   echo "Failed to grab image" 1>&2
	exit -1
fi
echo "done"
echo ""

echo "Grabbing image of the backlight only .. \"$DIR/backlight.raw\"" 
echo "Carefully remove the backlight from behind the document"
echo "Place it in the camera view and switch it on"
echo "Hit Enter to capture image"
read IN
$EXE -o $DIR/backlight -e raw
if [ "$?" != "0" ]; then
   echo "Failed to grab image" 1>&2
	exit -1
fi
echo "Done grabbing images"
echo ""

exit 0
