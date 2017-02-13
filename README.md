# Hilbert

![Header] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_header.PNG)

This program draws a three-dimensional Hilbert curve based on its L-system\* representation and uses OpenGL and the GLUT library to render it.

The [Hilbert curve](https://en.wikipedia.org/wiki/Hilbert_curve) is a fractal space-filling curve, usually rendered in two dimensions.  It never intersects with itself, but it is mathematically able to reach every point within a square!  Given how compact the curve is, it is able to preserve locality while mapping from 1D to 2D space and finds multiple applications in computer science.

The inspiration for this project came from an [excellent paper I found online](http://algorithmicbotany.org/papers/abop/abop-ch1.pdf).  If you have any interest in fractals and/or modeling with computer graphics, give it a read!  You'll soon see that, despite their simplicity, L-systems are surprisingly powerful and expressive.

\*If you're wondering what an L-system is or want to see other interesting fractals, check out my previous project ["LSystem"](https://github.com/kbhadury/LSystem).

### How it works
Currently the program is hardcoded to draw the L-system for the Hilbert curve.  I plan to add support for user input, but I wanted to get a working model out first.

This program uses 8 constants to control movement in space:
* F : move forward *(this  moves the pen forward 2 units so that we get spaces between the lines)*
* + : turn left
* - : turn right
* & : pitch down
* ^ : pitch up
* \\ : roll left
* / : roll right
* | : turn around

All of the directional constants change the pen's direction by 90 degrees.

When we encounter a constant in our input string, we perform its action.  If we encounter a variable (A,B,C,D), we replace it with another string according to the following rules:
* A = B-F+CFC+F-D&F^D-F+&&CFC+F+B//
* B = A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//
* C = |D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//
* D = |CFB-F+B|FA&F^A&&FB-F+B|FC//

We start with the string "A" and specify a recursion level, which tells us how many times to replace variables with their corresponding strings.  Below are renderings of the Hilbert curve for various levels of recursion:

#### Level 1
![Level 1] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb1.PNG)

#### Level 2
![Level 2] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb2.PNG)

#### Level 3
![Level 3] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb3.PNG)

### Gallery
Here are some more renderings.  These are all level 3 curves.

Edge view:

![Edge] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_edge.PNG)

Side view:

![Side] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_side.PNG)

Close-up view:

![Close up] (https://github.com/kbhadury/Hilbert/blob/master/Screenshots/hb_inside.PNG)
