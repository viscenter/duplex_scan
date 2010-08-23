import sys
import cv
import highgui

filename="../resources/test.tiff"
print " Loading \"", filename, "\""
im = highgui.cvLoadImage(filename);
highgui.cvNamedWindow("Window", 1);
highgui.cvShowImage("Window", im);
highgui.cvWaitKey();

