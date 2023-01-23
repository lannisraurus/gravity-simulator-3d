# Overview
This was a summer project that arose from the questions:

* How would I build a very basic 3D renderer without previous knowledge?
* How could I simulate orbits numerically? 

This was my (naïve and experimental) solution. Not all the planned functionalities are working (settings and creating worlds).

# The Renderer

The 3D Renderer was built using the usual dot product in R^3 and some geometric trickery, as seen in the image below.

![This is an image](renderAlgorithm.png)

# Worlds

The program comes with a `file manager` to save and load files, as well as a simple script for the world save files.
The scripts are contained in a world.dat file inside the world folders, which the program detects and considers valid.
To make new worlds simply copy an existing preset worlds and feel free to edit the world.dat!
Here are the world 'scripts':

Camera Center Position (x,y,z)
```
camera_center x y z
```
Camera Angles xAngle->\[-pi,pi\] yAngle->\[-pi/2,pi/2\]:
```
camera_angles xAngle yAngle
```
Planet informations (the planet texture must be 0 because not all functionalities were added, as previously mentioned):
```
planet_colour R G B
planet_position X Y Z
planet_velocity Vx Vy Vz
planet_mass M
planet_radius R
planet_texture 0
```
Create a planet (with previously stated information about the planet):
Note: You can create multiple copies of a planet by 'spamming' create_planet.
```
create_planet
```

I have also implemented a `python` script to generate a random world. Just run `rand_world.py` and the world information will be added to 

The orbits are calculated using Newton's Law of Gravitation and Euler's Method (Fancy name for just updating the force vectors for small dt).

The program might not work right away, You'll need to link SDL2main and SDL2image, as well as dll files.

The classes are all in .h files (I was experimenting to see if it'd be better than doing cpp and h (spoiler: it's not))

Enjoy!!

