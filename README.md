Swapnil Paratey
Summer 2015 - CSCI 4229/5229 - Computer Graphics
Final Project Submission - Sandbox
--------------------------------------

Extract contents of zip file
Run "make"
Use './final' to run the executable.
Optimized for GLUT on Ubuntu 14.04 LTS
You need freeglut3-dev package to run this on Ubuntu 14.04

No orthogonal/perspective modes. Only Strafe Forward/Backward mode.
Start moving around the sandbox when the program starts
Use 'H' to switch on the helicopter and helicopter spotlight

Key bindings for the program
----------------------------
ungraceful exit		            ESC
Reset viewing angle                 0 (zero)
Dinosaur's perspective              1
Another viewpoint                   2
Building top view point             3
Toggle lighting                     l/L
Toggle Helicopter and spotlight     h/H
Helicopter spotlight scope          r/R
Stop sunlight source movement       o/O
Increase/Decrease Field of View     +/-
Spotlight movement along X          </>
Spotlight movement along Z          ,/.
Toggle Axes                         q/Q
Raise/Lower strafe height           PgUp/PgDn
Increase/decrease ambient levels    A/a
Increase/decrease specular light    S/s
Increase/decrease diffusion         D/d
Increase/decrease shininess         n/N
Increase/decrease emission light    e/E
Straight forward/backward           Up/Down ARROW KEYS
Toggle Flat and Smooth shading      F1
Toggle Two-sided lighting           F3

Things achieved
- Sandbox movement over the 3D scene
- Terrain based on a height bitmap image 
[http://www.videotutorialsrock.com/opengl_tutorial/terrain/home.php]
- Understanding how to use objects and mtl files, used for trees
- Lighting over the terrain/objects/buildings
- Spotlight based lighting through the helicopter

Things not achieved and reasons
- Graphic Equalizer in the sky with SDL
SDL stores loaded audio in an opaque data format which cannot be processed upon.
Other libraries for media loading and processing and FFT calculations were 
too taxing of the processor and could not be integrated with this project.

Credits to free objects downloaded from http://tf3dm.com/
