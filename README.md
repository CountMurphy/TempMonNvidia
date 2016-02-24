TempMon
=======


Contents:
=========

1.  About
2.  Install
3.  Usage
4.  FAQ
5.  Legal


About
=====
TempMon is a simple Linux Daemon that polls your system for Processor (and GPU if running TempMonNvidia) temperature readings.  TempMon then sends that data to a Serial connected USB reader (source code for reader is included).  All of the settings for the daemon are stored in /etc/tempMon.config.  If you are running tempMon, your screen can display CPU tempurature and fan RPM.  If you are running tempMonNvidia, your screen will display CPU temperature and GPU temperature.

Install
=======
just run make, then make-install.  Thats it.  
Note: 
    *  You must have libsensors ("sudo apt-get install lmsensors" if you running ubuntu), libserial (can find in your repositories) and libnvidia-ml if using tempMonNvidia (automatically installed if you are using the offical closed source driver)
	* The daemon wont function properly until you edit the config file
	* if the make file fails, try g++ *.cpp /usr/lib/libsensors.so  /usr/lib/libnvidia-ml.so /usr/lib/libserial.so -o tempMon
	
Usage
=====
The first thing you need to do is type "tempMon -list".  This will present you with the required information to set up your config file.  To start the daemon just type "tempMon -start" into your terminal.  If your system uses Systemd, the startup script it included

FAQ
===
The alarm on the display went off because the temperature went over its threshhold.  How do I shut it off?
    Toggle the switch on the side of the unit, or unplug it for a second or so.
	
Legal 
==========
Copyright 2012 

Licensed under the GPLv3: http://www.gnu.org/licenses/gpl-3.0.html.
