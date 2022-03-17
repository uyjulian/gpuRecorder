======================================================================
                     GPU Recorder v2.0           (October, 12th 2003.)

                      by Matesic Darko        
======================================================================
                                              e-mail: darkman@eunet.yu

1) About
---------
	This is a PSEMU Pro graphic Plugin that acts as a wrapper
for other plugins while saving information that are send to plugin
into a file. Later that file can be used to replay that information
again using any of available gpu plugins. Plugin is intended to be
used for debuging gpu plugins.

2) Usage
---------
	Copy gpuRecorder.dll into plugins directory of psx emulator
and configure it through emulator. In configuration dialog you only
have to select another gpu plugin for display. Recorder files are
automaticaly recorder when emulation starts and finished when
gpu is closed (not emulation just gpu). Files are saved in Record
subdirectory of an emulator and are named RecordXXXX.rec (or some
other directory if specified in options). After recording you can 
play them with GPU Player.
	Recorder files are usualy huge for the amount of recorded
time. You can choose to compress it internally while recording or
you can rebuild it with gpuPlayer with compressions or you can
just compress it manualy with some compression tool (if you
compress it with another tool you will have to decompress it
before playback).

3) Options
-----------
	- Plugin: select your plugin as you would normaly select
		it in an emulator
	- Recording Start:
		- gpu open: recording starts immediately when
			emulation starts
		- load state: recording starts when you load a
			state for the first time
		- key press: recording starts when you press
			selected key
	- Recording Stop:
		- gpu close: recording stops when gpu is closed
		- key press: recording stops when you press
			selected key
        - Output:
		- use custom directory: check this and enter a
			directory you want your recorded files to be
	- Status Display:
		- title bar: recording status is displayed in
			window title bar
		- send to gpu: if gpu has functions GPUdisplayText
			it can be used to display status (if you
			use this you won't be able to see gpu 
			options and status like FPS)
		- draw manually: status will be drawn manually at
			the bottom of the screen.
	- Options:
		- record sound: if checked and spuRecorder is
			properly configured sound will be recorded
		- compress data: just check this so recorded file
			will be smaller in size (uses default
			zlib compression)
		- Demo Maker Recording: with this option on while
			recording everything between last save state
			and load state will be discarded; (eg. I made
			a demo of me playing '4 survivor' scenarion in
			Resident Evil 2 where demo shows how I run
			through all rooms without a scratch although
			I really used save state in every room and
			had had many tries to pass them until I've
			done it right)

4) Known problems / still to do
-------------------------------
	- context menus perhaps ?
	- file size checks ? what happens on 4GB (never tested it)
	- total frames check ? (About half million frames could be recorded)
	- dma chains do not handle memory transfers so if a game uses
		it there will be truouble; it is posible to resolve
		this but plugin needs some additional checking
		(never gonna do it)
	- memory reads are recorder; they could be avoided but again
		plugin then needs additional checking 
		(i don't want to do it)

5) Version history
-------------------
Octover, 12th 2003. Version 2.0
--------------------------------
	- sound recording
	- compression
August, 4th 2002. Version 1.1
----------------------------------
	- added more options: recording start/stop, "Demo Maker"

July, 29th 2002. Version 1.0
----------------------------------
	- first public release

====================================================================
   URL: http://mrdario.tripod.com
e-mail: darkman@eunet.yu
