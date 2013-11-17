waveAtIt 
========

WaveAtIt is an ongoing computer vision project application allowing for the control of your computer through your 
webcam with no additional hardware required. Development began during participation at HackTXAllows movement of 
mouse cursor, scroll, left/right click, using one's hands. Plans include to introduce gesture control as well 
as augmented reality HUD and full emulation of keyboard and numpad plus custom AR applications. 
If all goes well final product will also include voice command recognition.

**CURRENT BUILD = PRE-ALPHA**<br />
**CURRENT SUPPORTED OSes = UBUNTU, MAC**<br />
**ADDITIONAL DEPENDENCIES = OpenCV, Xlib**<br />

HOW TO USE (Not working for current build)
-----------

**Alpha Build**<br />
-Edit Source File to Calibrate<br />
-For hand recognition<br />
--uncomment createTrackBars() and imShow(..., threshold)<br />
---run program and move sliders until only hand is white and visible on screen<br />
-For colored tape recognition<br />
--uncomment createTrackBarsColors(), imShow(...,threshold), imShow(..., bThreshold), imShow(..., rThreshold)<br />
---run program and move green, blue, red sliders until only each color is visible in respective windows<br />
-write down values from sliders after calibrating<br />
--in source file edit values with numbers written down<br />
---hand when using hand recognition<br />
---green, blue, red when using colored tape recognition<br />
---comment out methods you uncommented earlier.<br />
-After calibration is done<br />
--save source file<br />
--open terminal and navigate to waveAtIt directory<br />
---build using "make"<br />
----will automatically run application<br />
-----exit by pressing ESC<br />

*Sorry for the mess still working out best way seamlessly automate this*

CURRENT WORKING FEATURES
-------------------------

**Hand Tracking**<br />
-Tracks hand using:<br />
-- Green for mouse cursor,<br />
-- blue = left click, <br />
--red = right click<br />
-Or tracks hand using skin color filter<br />

**Mouse Control**<br />
-Moves mouse across most of the screen<br />
-Clicks function properly and have a 1 second & 10 frame cooldown time<br />


CURRENT KNOWN BUGS
-------------------

**Hand Tracking**<br />
-Have to recalibrate when environment lighting changes<br />
-Using the skin color filters hand blends with forearm or face<br />
--implementing face detection to remove from blending with hand<br />

**Mouse Control**<br />
-algorithm for reducing noise and jitteriness not 100%<br />
--lighting conditions cause more noise<br />
---causes mouse to be jittery<br />
-Mapping webcam feed to screen cuts of screen edges<br />
--mouse can't reach entire screen<br />


PLANNED FUTURE ADDITIONS
-------------------------

**Gesture Control**<br />
-Use certain gestures to control common aspects of OS interaction<br />
--swipe for switching between workspaces in Linux/Mac<br />
--run common apps such as browsers with unique gestures<br />
--Shutdown, Reboot, Log-Off with specific gestures<br />
--bring up HUD<br />

**Augmented Reality HUD**<br />
-Implement helpful HUD for access to utilities and hardware emulation<br />
--bring up HUD with hand gestures<br />
--incorporate useful apps such as Calculator, Terminal, and frequently used apps<br />
--emulated keyboard and NumPad for typing through webcam<br />
--custom desktop locking app with several unlock styles (swype, pattern, combination lock)<br />
--run/close voice Command Recognition app<br />

**Voice Command Recognition**<br />
-Run certain commands through mic<br />
--initialize AR Voice Command Recognition App to issue voice commands<br />
-Open browsers using voice commands<br />
--Saying "Run Chrome"/"Run Firefox"/"Run Internet Explorer"<br />
-Lock Desktop<br />