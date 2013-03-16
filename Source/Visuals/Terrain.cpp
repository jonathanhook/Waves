/**
 * Class:	Terrain
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <sstream>
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "Model.h"
#include "Visuals/Terrain.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string	Terrain::HEIGHT_OFFSET_NAME		= "Height Offset";
	const std::string	Terrain::MODEL_PATH				= "..\\Models\\Landscape.obj";
	const std::string	Terrain::ROTATION_SPEED_NAME	= "Rotation Speed";
	const float			Terrain::MAX_ROTATION_INCREMENT	= 0.005f;

	/* CONSTRUCTORS */
	Terrain::Terrain(void) :
		Visual("Terrain")
	{
		model		= Model::loadModel(MODEL_PATH);
		rotation	= 0.0f;
		
		unsigned vbkId = glutGet(GLUT_ELAPSED_TIME) + rand();	
		std::stringstream ss;
		ss << vbkId;
		vertexBufferKey = ss.str();

		// new params
		parameters[HEIGHT_OFFSET_NAME] = new Parameter(HEIGHT_OFFSET_NAME);
		parameters[ROTATION_SPEED_NAME] = new Parameter(ROTATION_SPEED_NAME, 0.1f);

		// defaults
		Parameter *rotXParam = parameters[ROTATION_X_NAME];
		assert(rotXParam != NULL);
		rotXParam->setDefaultValue(0.1f);

		Parameter *posYParam = parameters[POSITION_Y_NAME];
		assert(posYParam != NULL);
		posYParam->setDefaultValue(0.6f);

		// disable params;
		Parameter *hueP = parameters[HUE_NAME];
		assert(hueP != NULL);
		hueP->setDisabled(true);
		
		Parameter *lumaP = parameters[LUMINANCE_NAME];
		assert(lumaP != NULL);
		lumaP->setDisabled(true);

		Parameter *saturationP = parameters[SATURATION_NAME];
		assert(saturationP != NULL);
		saturationP->setDisabled(true);

		Parameter *rotXP = parameters[ROTATION_X_NAME];
		assert(rotXP != NULL);
		rotXP->setDisabled(true);

		Parameter *rotYP = parameters[ROTATION_Y_NAME];
		assert(rotYP != NULL);
		rotYP->setDisabled(true);

		Parameter *rotZP = parameters[ROTATION_Z_NAME];
		assert(rotZP != NULL);
		rotZP->setDisabled(true);

		Parameter *posXP = parameters[POSITION_X_NAME];
		assert(posXP != NULL);
		posXP->setDisabled(true);

		Parameter *posYP = parameters[POSITION_Y_NAME];
		assert(posYP != NULL);
		posYP->setDisabled(true);

		Parameter *posZP = parameters[POSITION_Z_NAME];
		assert(posZP != NULL);
		posZP->setDisabled(true);
	}

	Terrain::~Terrain(void)
	{		
		if(model != NULL)
		{
			delete model;
			model = NULL;
		}

		// TODO : possible leak on graphics card?
		//glDeleteBuffersARB(1, &vertexVbo);
		cgDestroyContext(cg_Context);
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void Terrain::draw(void)
	{
		assert(model != NULL);

		//
		// initialise VBO
		//
		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		int result = currContext->getVertexBuffer(vertexBufferKey);
		bool initialised = (result != -1); 

		unsigned int vertexVbo = NULL;
		if(!initialised)
		{
			vertexVbo = initVbo();
			currContext->setVertexBuffer(vertexBufferKey, vertexVbo);
		
			initShaders();
		}
		else
		{
			vertexVbo = (unsigned int)result;
		}

		//
		// then draw
		//

		// init vards
		unsigned int fvc = model->getFaceVertexCount();
		unsigned int vertexCount = model->getNumVertices();
		GLenum renderType = (fvc == 3) ? GL_TRIANGLES : GL_QUADS;
		
		// enable buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);
		glVertexPointer(fvc, GL_FLOAT, 64, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnable(GL_NORMALIZE);

		glPushAttrib(GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
		glEnable(GL_LINE_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glColor4f(1.0f, 1.0f, 1.0f, getOpacity());

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glRotatef(rotation * 360.0f, 0.0f, 1.0f, 0.0f);

		// enable shader
		setShaderConstants();
		cgGLEnableProfile(cg_VertexProfile);
		cgGLBindProgram(cg_Program);

		glDrawArrays(renderType, 0, vertexCount);

		// disable shader
		cgGLDisableProfile(cg_VertexProfile);

		glPopMatrix();
		glPopAttrib();
		
		// disable buffer
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
	}

	Visual *Terrain::getInstance(void) const
	{
		return new Terrain();
	}

	void Terrain::update(void)
	{
		unsigned int now = glutGet(GLUT_ELAPSED_TIME);
		unsigned int elapsed = now - last;

		if(elapsed >= UPDATE_RATE_MS)
		{
			Parameter *rotationP = parameters[ROTATION_SPEED_NAME];
			assert(rotationP != NULL);
			float rotVal = rotationP->getValue();

			rotation = fmodf(rotation + (MAX_ROTATION_INCREMENT * rotVal), 1.0f) ;
			last = now;
		}
	}


	/* PRIVATE MEMBER FUNCTIONS */
	unsigned int Terrain::initVbo(void)
	{
		assert(model != NULL);

		// vertices
		unsigned int vertexVbo = NULL;

		glGenBuffers(1, &vertexVbo);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVbo);

		unsigned int vertexCount = model->getNumVertices();
		vertexBufferSizeInBytes =  vertexCount * sizeof(GLVBOVertex);
		const GLVBOVertex *vertices = model->getVertices();

		glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeInBytes, vertices, GL_STATIC_DRAW);

		// unbind
		glBindBuffer(GL_ARRAY_BUFFER, NULL);

		return vertexVbo;
	}

	void Terrain::initShaders(void)
	{
		cg_Context = cgCreateContext();
		assert(cg_Context != NULL);

		cg_VertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
		assert(cg_VertexProfile != CG_PROFILE_UNKNOWN);

		cgGLSetOptimalOptions(cg_VertexProfile);

		cg_Program = cgCreateProgramFromFile(cg_Context, CG_SOURCE, "..\\Waves Visuals\\Shaders\\height.cg", cg_VertexProfile, "main", 0);
		assert(cg_Program != NULL);

		cgGLLoadProgram(cg_Program);

		// get parameter handles
		cgp_modelViewProj = cgGetNamedParameter(cg_Program, "modelViewProj");
		cgp_opacity = cgGetNamedParameter(cg_Program, "opacity");
		cgp_heightOffset = cgGetNamedParameter(cg_Program, "heightOffset");
	}

	void Terrain::setShaderConstants(void)
	{
		// transformation
		cgGLSetStateMatrixParameter(cgp_modelViewProj, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

		// opacity
		cgGLSetParameter1f(cgp_opacity, getOpacity());

		// height offset
		Parameter *hoP = parameters[HEIGHT_OFFSET_NAME];
		assert(hoP != NULL);

		float heightOffset = hoP->getValue();
		heightOffset = 1.0f - heightOffset;
		heightOffset = (heightOffset * 2.0f) - 1.0f;
		cgGLSetParameter1f(cgp_heightOffset, heightOffset);
	}
}
