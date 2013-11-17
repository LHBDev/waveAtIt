waveAtIt 
========

WaveAtIt is an ongoing computer vision project application allowing for the control of your computer through your 
webcam with no additional hardware required. Development began during participation at HackTXAllows movement of 
mouse cursor, scroll, left/right click, using one's hands. Plans include to introduce gesture control as well 
as augmented reality HUD and full emulation of keyboard and numpad plus custom AR applications. 
If all goes well final product will also include voice command recognition.

**CURRENT BUILD = PRE-ALPHA**
**CURRENT SUPPORTED OSes = UBUNTU, MAC**
**ADDITIONAL DEPENDENCIES = OpenCV, Xlib**

HOW TO USE (Not working for current build)
-----------

**Alpha Build**
-Edit Source File to Calibrate
-For hand recognition
--uncomment createTrackBars() and imShow(..., threshold)
---run program and move sliders until only hand is white and visible on screen
-For colored tape recognition
--uncomment createTrackBarsColors(), imShow(...,threshold), imShow(..., bThreshold), imShow(..., rThreshold)
---run program and move green, blue, red sliders until only each color is visible in respective windows
-write down values from sliders after calibrating
--in source file edit values with numbers written down
---hand when using hand recognition
---green, blue, red when using colored tape recognition
---comment out methods you uncommented earlier.
-After calibration is done
--save source file
--open terminal and navigate to waveAtIt directory
---build using "make"
----will automatically run application
-----exit by pressing ESC

*Sorry for the mess still working out best way seamlessly automate this*

CURRENT WORKING FEATURES
-------------------------

**Hand Tracking**
-Tracks hand using:
-- Green for mouse cursor,
-- blue = left click, 
--red = right click
-Or tracks hand using skin color filter

**Mouse Control**
-Moves mouse across most of the screen
-Clicks function properly and have a 1 second & 10 frame cooldown time


CURRENT KNOWN BUGS
-------------------

**Hand Tracking**
-Have to recalibrate when environment lighting changes
-Using the skin color filters hand blends with forearm or face
--implementing face detection to remove from blending with hand

**Mouse Control**
-algorithm for reducing noise and jitteriness not 100%
--lighting conditions cause more noise
---causes mouse to be jittery
-Mapping webcam feed to screen cuts of screen edges
--mouse can't reach entire screen


PLANNED FUTURE ADDITIONS
-------------------------

**Gesture Control**
-Use certain gestures to control common aspects of OS interaction
--swipe for switching between workspaces in Linux/Mac
--run common apps such as browsers with unique gestures
--Shutdown, Reboot, Log-Off with specific gestures
--bring up HUD

**Augmented Reality HUD**
-Implement helpful HUD for access to utilities and hardware emulation
--bring up HUD with hand gestures
--incorporate useful apps such as Calculator, Terminal, and frequently used apps
--emulated keyboard and NumPad for typing through webcam
--custom desktop locking app with several unlock styles (swype, pattern, combination lock)
--run/close voice Command Recognition app

**Voice Command Recognition**
-Run certain commands through mic
--initialize AR Voice Command Recognition App to issue voice commands
-Open browsers using voice commands
--Saying "Run Chrome"/"Run Firefox"/"Run Internet Explorer"
-Lock Desktop