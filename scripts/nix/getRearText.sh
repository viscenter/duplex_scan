#!/bin/bash

GRABBER="scripts/grabImages.sh"
DUPLEXER="common/utils/duplexScanStats"
SEGMENTER="common/utils/segment"

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

echo "Enter a directory to use/copy imaegs to"
read DIR

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

echo "Run segmentation on executable to manually find out what thresholds to use "
echo " done"
