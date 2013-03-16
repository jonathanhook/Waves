/**
 * Class:	Model
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "JDHUtility/GlutInclude.h"
#include "Model.h"
#include "kixor\objLoader.h"

namespace WavesVisuals
{
	/* PUBLIC STATIC FUNCTIONS */
	Model *Model::loadModel(std::string path)
	{
		objLoader *objData = new objLoader();
		objData->load((char *)path.c_str());

		obj_vector **vertex_list = objData->vertexList;
		obj_vector **vertex_normal_list = objData->normalList;
		obj_vector **vertex_texture_list = objData->textureList;
		obj_face **face_list = objData->faceList;

		unsigned int textureCount = objData->textureCount;
		unsigned int fvc = face_list[0]->vertex_count;
		unsigned int faceCount = objData->faceCount;
		unsigned int vertexCount = faceCount * fvc;

		GLVBOVertex *vertices = new GLVBOVertex[vertexCount];
		for(unsigned int i = 0; i < faceCount; i++)
		{
			obj_face *face = face_list[i];
			for(unsigned int j = 0; j < fvc; j++)
			{
				GLVBOVertex *vboVertex = &vertices[(i * fvc) + j];

				// vertices
				unsigned int vertexId = face->vertex_index[j];
				obj_vector *vertex = vertex_list[vertexId];
				vboVertex->x = vertex->e[0];
				vboVertex->y = vertex->e[1];	
				vboVertex->z = vertex->e[2];

				// normals
				unsigned int normalId = face->normal_index[j];
				obj_vector *normal = vertex_normal_list[normalId];
				vboVertex->nx = normal->e[0];
				vboVertex->ny = normal->e[1];
				vboVertex->nz = normal->e[2];

				// texture coords
				unsigned int textureId = face->texture_index[j];
				obj_vector *texture = vertex_texture_list[textureId];
				vboVertex->u = texture->e[0];
				vboVertex->v = texture->e[1];
				vboVertex->w = texture->e[2];

				// other texture coords (NULL)
				vboVertex->r = 1.0f;
				vboVertex->g = 1.0f;
				vboVertex->b = 1.0f;
			}
			
		}

		normaliseVertexArray(vertices, vertexCount);

		Model *m = new Model(path, vertices, vertexCount, fvc);
		return m;
	}

	/* PRIVATE STATIC FUNCTIONS */
	void Model::normaliseVertexArray(GLVBOVertex *vertexArray, unsigned int numElements)
	{
		// 1) find largest
		float largest = 0.0f;
		float smallest = 9999.0f;

		for(unsigned int i = 0; i < numElements; i++)
		{
			GLVBOVertex *vboVertex = &vertexArray[i];
			assert(vboVertex != NULL);

			float x = vboVertex->x;
			float y = vboVertex->y;
			float z = vboVertex->z;

			// x
			if(x > largest)
			{
				largest = x;
			}
			else if(x < smallest)
			{
				smallest = x;
			}

			// y
			if(y > largest)
			{
				largest = y;

			}
			else if(y < smallest)
			{
				smallest = y;
			}
			
			// z
			if(z > largest)
			{
				largest = z;

			}
			else if(z < smallest)
			{
				smallest = z;
			}
		}

		float span = largest - smallest;
		for(unsigned int i = 0; i < numElements; i++)
		{
			GLVBOVertex *vboVertex = &vertexArray[i];
			
			// x
			float xVal = vboVertex->x;
			float xNVal = xVal / span;
			vboVertex->x = xNVal;

			// y
			float yVal = vboVertex->y;
			float yNVal = yVal / span;
			vboVertex->y = yNVal;

			// z
			float zVal = vboVertex->z;
			float zNVal = zVal / span;
			vboVertex->z = zNVal;
		}
	
	}

	/* CONSTRUCTORS */
	Model::Model(std::string name, GLVBOVertex *vertices, unsigned int numVertices, unsigned int faceVertexCount)
	{
		this->name = name;
		this->vertices = vertices;
		this->numVertices = numVertices;
		this->faceVertexCount = faceVertexCount;

		if(faceVertexCount == 3)
		{
			renderType = GL_TRIANGLES;
		}
		else
		{
			renderType = GL_QUADS;
		}
	}

	Model::~Model(void)
	{
		if(vertices != NULL)
		{
			delete [] vertices;
			vertices = NULL;
		}
	}

	/* PUBLIC MEMBER FUNCTIONS */
	const unsigned int Model::getFaceVertexCount(void) const
	{
		return faceVertexCount;
	}

	const std::string &Model::getName(void) const
	{
		return name;
	}

	const unsigned int Model::getNumVertices(void) const
	{
		return numVertices;
	}

	const GLenum &Model::getRenderType(void) const
	{
		return renderType;
	}

	const GLVBOVertex *Model::getVertices(void) const
	{
		return vertices;
	}
}
