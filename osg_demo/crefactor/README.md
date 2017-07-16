osg
========

Test work to integrate OpenSceneGraph as the new graphics layer in Concordia

# Build Instructions
* Build OpenSceneGraph with GL3, enable fixed-function pipeline, make sure core profile functionality is used
* Run ldpath.sh
* Run build.sh
* Run ./a.out

# Objectives
* SFGUI works with osg test. If not, I'm willing to punt on SFML and SFGUI since this engine is very important to the future of the game. In this case we'll check out librocket, imgui, or probably nanogui. We'll need to do *something* for OS X since Apple disabled compatibility profile because they just enjoy being a pain in the ass, and SFML can't render the GUI in the core profile.
