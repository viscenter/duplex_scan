#!/bin/bash

# Make sure only root can run our script
if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root" 1>&2
	exit 1
fi

home="/home/$1"
if [ -d "$home"  ] ; then
	echo -n "Updating home dir $home"
	pushd "$home"
	echo `pwd`
		if [ ! -e ".vimrc" ]; then
			wget http://dl.dropbox.com/u/441713/.vimrc >> /dev/null 2>&1
		else
			echo ".vimrc already exists"
		fi

		if [ ! -e ".vim/colors/xoria256.vim" ]; then
			mkdir -p .vim/colors/
			wget http://dl.dropbox.com/u/441713/xoria256.vim -O .vim/colors/xoria256.vim >> /dev/null 2>&1
		else
			echo ".vim/colors/xoria256.vim already exists"
		fi

		if [ -n `fgrep .bashrc xterm-256color`] 
			echo "export TERM=xterm-256color" >> .bashrc 
		else
			echo "TERM env already exists"
		fi

			chown -R $1:$1 .vimrc .vim .bashrc

	popd
	echo -n " done"
	echo " "
else
	echo "Not updating home dir as it was not specified/ or dir was invalid"
	home=""
fi
exit 1

apt-get update -qq -y >> /dev/null 2>&1 
if [ "$?" != "0" ]; then
   echo "package update failed" 1>&2
	exit 1
fi
apt-get install -qq -y subversion libboost-dev libboost-filesystem-dev libboost-progam-options-dev  g++ vim libghoto2-2-dev gphoto2 ctags ncurses-term libjpeg62-dev libtiff4-dev libjasper-dev libpng-dev libtbb2 libtbb-dev libgtk2.0-dev pkg-config
if [ "$?" != "0" ]; then
   echo "package install failed" 1>&2
	exit 1
fi
