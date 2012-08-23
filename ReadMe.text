***************************************
___________                      _____                 
\__    ___/___   _____ ______   /     \   ____   ____  
  |    |_/ __ \ /     \\____ \ /  \ /  \ /  _ \ /    \ 
  |    |\  ___/|  Y Y  \  |_> >    Y    (  <_> )   |  \
  |____| \___  >__|_|  /   __/\____|__  /\____/|___|  /
             \/      \/|__|           \/            \/
***************************************

Contents:

1.  About
2.  Install
3.  Usage
4.  FAQ
5.  Legal Crap


**************
About
**************
TempMon is a simple Linux Daemon that polls your system for Processor (and GPU if running TempMonNvidia) for temperature readings.  TempMon then sends that data to a Serial connected USB reader (source code for reader is included).  All of the settings for the daemon are stored in /etc/tempMon.config.  If you are running tempMon, your screen can display CPU tempurature and fan RPM.  If you are running tempMonNvidia, your screen will display CPU temperature and GPU temperature.

**************
Install
**************

just run make, then make-install.  Thats it.  
Note: 
    *  You must have libsensors ("sudo apt-get install lmsensors" if you running ubuntu), libserial (can find in your repositories) and libnvidia-ml if using tempMonNvidia (automatically installed if you are using the offical closed source driver)
	*  If running a 32-bit system the initial compile might fail.  This is because the make file is looking in the wrong spot for the required libs.  If it complanes that libsensors cannot be found, type "locate libsensors" into your terminal.  It should give you the correct path to the library.  Edit the make file accordingly.
	
****************
Usage
****************

The first thing you need to do is type "tempMon -list".  This will present you with the required information to set up your config file.  At present, tempMon isn't started like a regular daemon.  This is mostly because I am lazy, and its the first "beta" release.  To start the daemon just type "tempMon -start" into your terminal.  Eventually I'll figure out how to install the daemon properly.

****************
FAQ
****************

The alarm on the display went off because the temperature went over its threshhold.  How do I shut it off?
    Toggle the switch on the side of the unit, or unplug the damn thing for a second or so.
	
****************
Legal Crap
****************

Copyright 2012 

Licensed under the GPLv3: http://www.gnu.org/licenses/gpl-3.0.html.
