#!/bin/sh

# Installs the cupidfetch config
# Public domain ~ Arthur Bacci 2024


if [ -n "${XDG_CONFIG_HOME}" ]
then
	config="${XDG_CONFIG_HOME}"
else
	if [ -n "${HOME}" ]
	then
		config="$HOME/.config"
	else
		echo 'Please set your $HOME'
		exit 1
	fi
fi

echo "Your config directory is $config"
echo "Press enter to continue"
read

if [ -f "${config}/cupidfetch" ]
then
	echo "Your ${config}/cupidfetch is a file! Please delete it to continue"
	exit 1
else
	if [ ! -d "${config}/cupidfetch" ]
	then
		echo "mkdir ${config}/cupidfetch......"
		mkdir ${config}/cupidfetch
	else
		echo "Your ${config}/cupidfetch already exists"
	fi
fi


if [ -d "${config}/cupidfetch/cupidfetch.ini" ]
then
	echo "Your ${config}/cupidfetch/cupidfetch.ini is a directory! Please delete it to continue"
	exit 1
else
	if [ -f "${config}/cupidfetch/cupidfetch.ini" ]
	then
		echo "Your ${config}/cupidfetch/cupidfetch.ini already exists. I won't touch it."
	else

		if [ -f "data/cupidfetch.ini" ]
		then
			echo "Creating ${config}/cupidfetch/cupidfetch.ini from the default config"
			cp data/cupidfetch.ini "${config}/cupidfetch/cupidfetch.ini"
		else
			echo "I don't see any data/cupidfetch.ini, please place one there"
			exit 1
		fi
	fi
fi

echo "Done! Please run cupidfetch and it will probably find your config."
echo "Feel free to tweak it, it's at ${config}/cupidfetch/cupidfetch.ini"
echo "Your log is probably at ${config}/cupidfetch/log.txt"

