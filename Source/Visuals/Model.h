/**
 * Class:	Model
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Model_H
#define Model_H

#include <vector>
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/Point3f.h"

using namespace JDHUtility;

namespace WavesVisuals
{
	class Model
	{
	public:
		typedef struct
		{
			std::vector<unsigned int> indices;
		} Face;

		static Model *loadModel(std::string path);

		Model(std::string name, GLVBOVertex *vertices, unsigned int numVertices, unsigned int faceVertexCount);
		~Model(void);

		const unsigned int getFaceVertexCount(void) const;
		const std::string &getName(void) const;
		const unsigned int getNumVertices(void) const;
		const GLenum &getRenderType(void) const;
		const GLVBOVertex *getVertices(void) const;

	private:
		static void normaliseVertexArray(GLVBOVertex *vertexArray, unsigned int numElements);

		GLenum renderType; 
		unsigned int faceVertexCount;
		std::string name;
		unsigned int numVertices;
		GLVBOVertex *vertices;
	};
}
#endif

