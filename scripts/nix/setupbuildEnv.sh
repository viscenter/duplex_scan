#!/bin/bash

# Make sure only root can run our script
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
	exit 1
fi
apt-get update -qq -y >> /dev/null 2>&1 
if [ "$?" != "0" ]; then
   echo "package update failed" 1>&2
	exit 1
fi
apt-get install -qq -y subversion libboost-dev libboost-filesystem-dev g++ vim libghoto2-2-dev gphoto2
if [ "$?" != "0" ]; then
   echo "package install failed" 1>&2
	exit 1
fi
