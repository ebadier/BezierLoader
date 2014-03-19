BezierLoader
============
An application which can load an OBJ format file, containing one or more Bezier curves, into some internal format. 
The application should then render the curves.
Notes:
- The application is written in C++.
- The loader handles any valid Bezier curve in the OBJ format.
- The loader report an error if non-Bezier primitives are found in the file.
- This project use Ogre3D for the rendering part.

==> Repository structure
bin : the last released.
doc : documentation about the BezierLoader project.
externals : external librairies used in this project.
media : some assets used to render objects.
src : the source code.
vcproj : a solution to compile the project with Visual C++ 2010.
	-BezierLoader.vcxproj generate a static library
	-BezierLoaderTest.vcxproj generate an exe to test the functionalities provided by BezierLoader static library.
	
==> Test Application:
You can launch the last release by double-clicking on the "test.bat" file.
Check the "test.bat" file to see how to load your own .obj file.
	
	==> The application load the curves in the .obj file.
	Each curve is rendered with a particular random colour.
	Control points are rendered as small spheres.
	The curves are rendered as more or less broken lines, depending on the resolution. 
	You can switch and focus on each curve pressing space bar.
	
	==> Controls:
	W,S,A,D + mouse to move the camera.
	Left_Shift : speed up translation.		
	Space : switch to the next curve (the camera focus automatically on the next curve).
	Enter : show all curves.
	Esc : Exit.
