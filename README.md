# Hilbert

![Header] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_header.PNG)

This program draws three-dimensional fractals based on their L-system\* representations and uses OpenGL and the GLUT library to render them.  Check out the [Gallery](https://github.com/kbhadury/Hilbert#gallery) for a few samples!

_Update_: overhauled the original program to use object-oriented design principles.  The user only has to interact with the Driver, which encapsulates most of the inner workings of the Hilbert class.  The program is now able to map user-provided variables to their corresponding rules, so it's no longer restricted to the Hilbert curve.  It also supports push and pop operations, which means it can parse most L-systems (more pen options will be coming soon!).  Also added a debug option.

_Update #2_: added cylinders!  Pyramids were previously an option but there was no way to select them without going into the hilbet.cpp file.  Now you can choose whichever shape you want to use from the driver.

_Future plans_: add more pen options (i.e. change size, specify color), give the user more customization options, work on plant models.  Stochastic L-systems are definitely a possibility.

### About this project
This program was originally designed to draw a three-dimensional Hilbert curve.  The [Hilbert curve](https://en.wikipedia.org/wiki/Hilbert_curve) is a fractal space-filling curve, usually rendered in two dimensions.  It never intersects with itself, but it is mathematically able to reach every point within a square!  Given how compact the curve is, it is able to preserve locality while mapping from 1D to 2D space and finds multiple applications in computer science.  

With the latest update, the program can theoretically parse any L-system which uses the constants listed below.  3D plants are especially fun to draw using L-systems.

The inspiration for this project came from an [excellent paper I found online](http://algorithmicbotany.org/papers/abop/abop-ch1.pdf).  If you have any interest in fractals and/or modeling with computer graphics, give it a read!  You'll soon see that, despite their simplicity, L-systems are surprisingly powerful and expressive.

\*If you're wondering what an L-system is or want to see other interesting fractals, check out my previous project ["LSystem"](https://github.com/kbhadury/LSystem).

### How it works
This program uses 10 constants to control movement in space:
* F : move forward *(this  moves the pen forward 2 units so that we get spaces between the lines)*
* + : turn left
* - : turn right
* & : pitch down
* ^ : pitch up
* \\ : roll left
* / : roll right
* | : turn around
* [ : push the current position and direction data to the stack
* ] : pop position and direction data from the stack

The rotation angle is specified in the constructor, along with a set of variables and their rules.  When we encounter a constant in our input string, we perform its action.  If we encounter a variable, we replace it with another string according to its corresponding rule.  For instance, to draw the Hilbert curve we specify the variables A, B, C, and D, and map them to the following rules:
* A = B-F+CFC+F-D&F^D-F+&&CFC+F+B//
* B = A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//
* C = |D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//
* D = |CFB-F+B|FA&F^A&&FB-F+B|FC//

In this case, we would start with the string "A".  We then specify a recursion level, which tells us how many times to replace variables with their corresponding strings.

Below are renderings of the Hilbert curve for various levels of recursion:

#### Level 1
![Level 1] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb1.PNG)

#### Level 2
![Level 2] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb2.PNG)

#### Level 3
![Level 3] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb3.PNG)

### Gallery
#### Here are some more renderings of the Hilbert curve:

Edge view:

![Edge] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_edge.PNG)

Side view:

![Side] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_side.PNG)

Close-up view:

![Close up] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_inside.PNG)

#### This a fractal plant (you can find the L-system rules in the hbDriver.cpp file)

A lush green plant (_I know it doesn't look very realistic at the moment.  I'm working on it!_):

![Plant1] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/plant1.PNG)

This rendering highlights the branching points:

![Plant2] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/plant2.PNG)

I added an option to draw with pyramids instead of cubes:

![PyraPlant] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/pyraplant.PNG)

Broccoli is supposed to be good for you

![BroccoliPlant] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/broccoli.PNG)
