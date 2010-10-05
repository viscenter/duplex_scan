#!/bin/bash

GRABBER="scripts/grabImages.sh"
DUPLEXER="common/utils/duplexScanStats"
SEGMENTER="common/utils/segment"
BACKTEXTER="common/utils/backText"

echo "Generating images for rear text retreval"

if [ ! -e "$GRABBER" ] ; then
	"Can't find image grabber script \"$GRABBER\""
	exit -1
fi

if [ ! -e "$DUPLEXER" ] ; then
	"Can't find duplexer executable \"$DUPLEXER\""
	exit -1
fi

if [ ! -e "$SEGMENTER" ] ; then
	"Can't find segementation executable \"$SEGMENTER\""
	exit -1
fi

if [ ! -e "$BACKTEXTER" ] ; then
	"Can't find backText executable \"$BACKTEXTER\""
	exit -1
fi

DIR=$1
if [ "$DIR" == "" ] ; then
	echo "Enter a directory to use/copy images to"
	read DIR
fi

if [[ -e "$DIR/backlight.raw"  &&  -e "$DIR/backlitDoc.raw"  &&  -e "$DIR/doc.raw" ]]; then
	echo "Overwrite images(y/n)?"
	read YN
	if [[ "$YN" == "y" || "$YN" == "Y" ]]; then
		$GRABBER $DIR
	else
		echo "Using previously captured images "
   fi
else
	$GRABBER $DIR
fi

if [ "$?" != "0" ]; then
	echo "Image capture failed"
	exit -1
fi

$DUPLEXER -b $DIR/backlight.raw -d $DIR/backlitDoc.raw  -n $DIR/doc.raw -v -s
if [ "$?" != "0" ]; then
	echo "Failed to compute duplex scan stats from image files in \"$DIR\""
	exit -1
fi

if [ "$?" != "0" ]; then
	echo "Computing stats failed"
	exit -1
fi

echo "Specify values that isolate only the back text"
$SEGMENTER -f backTextEnhanced.pfm  -d $DIR/backlitDoc.raw -o backgroundthreshold.png 

if [ "$?" != "0" ]; then
	echo "Failed to run segmentation code for background threshold"
	exit -1
fi

echo "Specify values that isolate only the foreground text"
$SEGMENTER -f backTextEnhanced.pfm  -d $DIR/backlitDoc.raw -o foregroundthreshold.png 

if [ "$?" != "0" ]; then
	echo "Failed to run segmentation code for foreground threshold"
	exit -1
fi

$BACKTEXTER -d $DIR/backlitDoc.raw -f foregroundthreshold.png -b backgroundthreshold.png -o extractedBackText

if [ "$?" != "0" ]; then
	echo "Failed to extract back text"
	exit -1
fi
