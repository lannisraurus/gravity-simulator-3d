# Planet-Simulator-CPP-SDL2
This was a summer project that arose from the questions: How would I build a very basic 3D renderer without previous knowledge? How could I simulate orbits numerically? This was my (naive and experimental) solution. Not all the planned functionalities are working because I have since decided to leave the project.

The 3D Renderer was built using concepts from analytical geometry (basic dot products and cartesian plane equations).
The program comes with a file manager to save and load files.
The orbits are calculated using Newton's Law of Gravitation and Euler's Method (Fancy name for just updating the force vectors for small dt).

The program might not work right away, You'll need to link SDL2main and SDL2image, as well as dll files.

The classes are all in .h files (I was experimenting to see if it'd be better than doing cpp and h (spoiler: it's not))

Enjoy!!
