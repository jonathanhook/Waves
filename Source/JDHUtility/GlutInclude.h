#ifndef GLUT_INCLUDED
#define GLUT_INCLUDED
// stdio.h must be included before glut on all occasions
#	include <stdio.h>
#	include <stdlib.h>
#	include <GL/glew.h>
#	include <cg\cg.h>
#	include <cg\cggl.h>
#	ifdef __APPLE__
#		include <GLUT/glut.h>
#	else
#		include <GL/glut.h>
#	endif
#	pragma comment(lib, "cg.lib")
#	pragma comment(lib, "cggl.lib")
#endif

#ifndef GlutInclude_H
#define GlutInclude_H

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace JDHUtility
{
	typedef struct
	{
		float x, y, z;        // Vertex
		float nx, ny, nz;     // Normal
		float u, v, w; 		  // Texcoord
		float r, g, b;		  // Colour
		float padding[4];
	} GLVBOVertex;
}

#endif