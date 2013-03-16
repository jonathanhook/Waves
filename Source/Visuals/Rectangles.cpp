/**
 * Class:	Rectangles
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <sstream>
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "Model.h"
#include "Visuals/Rectangles.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string	Rectangles::AXIS_NAME				= "Axis";
	const std::string	Rectangles::CENTRE_OFFSET_NAME		= "Centre Offset";
	const std::string	Rectangles::INTENSITY_NAME			= "Intensity";
	const float			Rectangles::MAX_RECT_DEPTH			= 1.0f;
	const float			Rectangles::MAX_RECT_HEIGHT			= 1.0f;
	const float			Rectangles::MAX_RECT_WIDTH			= 1.0f;
	const float			Rectangles::MAX_ROTATION_INCREMENT	= 0.005f;
	const float			Rectangles::MAX_X_OFFSET			= 1.0f;
	const float			Rectangles::MAX_Y_OFFSET			= 1.0f;
	const float			Rectangles::MAX_Z_OFFSET			= 1.0f;
	const float			Rectangles::MAX_ZOOM				= 0.005f;
	const std::string	Rectangles::MODEL_PATH				= "..\\Models\\RectangleHorizontal.obj";
	const float			Rectangles::NEW_THRESHOLD			= 0.25f;
	const std::string	Rectangles::ROTATION_SPEED_NAME		= "Rotation Speed";
	const std::string	Rectangles::RECT_DEPTH_NAME			= "Rect Depth";
	const std::string	Rectangles::RECT_WIDTH_NAME			= "Rect Width";
	const std::string	Rectangles::RECT_HEIGHT_NAME		= "Rect Height";
	const std::string	Rectangles::ZOOM_SPEED_NAME			= "Zoom Speed";

	/* CONSTRUCTORS */
	Rectangles::Rectangles(unsigned int maxRects) :
		Visual("Rectangles")
	{
		this->maxRects = maxRects;

		model		= Model::loadModel(MODEL_PATH);
		rects		= new RectData[maxRects];
		rectPtr		= 0;
		rotationVal	= 0.0f;
		zoom		= 0.0f;
		zoomSign	= 1.0f;

		for(unsigned int i = 0; i < maxRects; i++)
		{
			RectData *rd	= &rects[i];
			assert(rd != NULL);

			rd->enabled		= false;
		}

		unsigned vbkId = glutGet(GLUT_ELAPSED_TIME) + rand();	
		std::stringstream ss;
		ss << vbkId;
		vertexBufferKey = ss.str();

		// new params
		parameters[AXIS_NAME]			= new Parameter(AXIS_NAME);
		parameters[CENTRE_OFFSET_NAME]	= new Parameter(CENTRE_OFFSET_NAME);
		parameters[INTENSITY_NAME]		= new Parameter(INTENSITY_NAME);
		parameters[ROTATION_SPEED_NAME] = new Parameter(ROTATION_SPEED_NAME, 0.1f);
		parameters[RECT_WIDTH_NAME]		= new Parameter(RECT_WIDTH_NAME);
		parameters[RECT_HEIGHT_NAME]	= new Parameter(RECT_HEIGHT_NAME);
		parameters[RECT_DEPTH_NAME]		= new Parameter(RECT_DEPTH_NAME);
		parameters[ZOOM_SPEED_NAME]		= new Parameter(ZOOM_SPEED_NAME, 0.0f);

		// disable params
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

	Rectangles::~Rectangles(void)
	{
		if(model != NULL)
		{
			delete model;
			model = NULL;
		}

		if(rects != NULL)
		{
			delete [] rects;
			rects = NULL;
		}

		// TODO : some how delete buffer

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void Rectangles::draw(void)
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		float zoomVal = 1.0f + zoom;
		glScalef(zoomVal, zoomVal, zoomVal);
		glRotatef(360.0f * rotationVal, 0.0f, 1.0f, 0.0f);
		
		unsigned int now = glutGet(GLUT_ELAPSED_TIME);
		for(unsigned int i = 0; i < maxRects; i++)
		{
			RectData *rd = &rects[i];
			assert(rd != NULL);

			if(rd->enabled)
			{
				// opacity
				float opacity = 1.0f - ((float)(now - rd->createdTime) / (float)LIFETIME_MS);
				if(opacity <= 0.0f)
				{
					rd->enabled = false;
				}

				glColor4f(1.0f, 1.0f, 1.0f, getOpacity() * opacity);
				
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();

				float xScale = rd->width;
				float yScale = rd->height;
				float zScale = rd->depth;
				glScalef(xScale, yScale, zScale);

				Point3f *offset = &rd->offset;
				assert(offset != NULL);

				float xo = offset->getX();
				float yo = offset->getY();
				float zo = offset->getZ();
				
				glTranslatef(xo - (xo / 2.0f), yo - (yo / 2.0f), zo - (zo / 2.0f));

				if(rd->axis == 1)
				{
					glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
				}
				else if(rd->axis == 2)
				{
					glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				}

				glDrawArrays(renderType, 0, vertexCount);

				glPopMatrix();
			}
		}

		glPopMatrix();

		glPopAttrib();

		// disable buffer
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);

	}

	Visual *Rectangles::getInstance(void) const
	{
		return new Rectangles();
	}
	
	void Rectangles::update(void)
	{		
		unsigned int now = glutGet(GLUT_ELAPSED_TIME);
		unsigned int elapsed = now - last;

		if(elapsed >= UPDATE_RATE_MS)
		{
			// new
			Parameter *intensityParam = parameters[INTENSITY_NAME];
			assert(intensityParam != NULL);

			float intensity = intensityParam->getValue();
			float random = getRandom();
			float chance = intensity * NEW_THRESHOLD;

			if(random > (1.0f - chance))
			{
				newRect();
			}			

			// rotation
			Parameter *rotParam = parameters[ROTATION_SPEED_NAME];
			assert(rotParam);
			float rotationSpeed = rotParam->getValue();

			rotationVal = fmodf(rotationVal + (MAX_ROTATION_INCREMENT * rotationSpeed), 1.0f);

			// zoom
			Parameter *zoomParam = parameters[ZOOM_SPEED_NAME];
			assert(zoomParam != NULL);
			float zoomSpeed = zoomParam->getValue();
			float zoomIncrement = (zoomSpeed * zoomSign) * MAX_ZOOM;
			zoom+= zoomIncrement;

			if(zoom < 0.0f)
			{
				zoomSign = 1.0f;
			}
			else if(zoom > 1.0f)
			{
				zoomSign = -1.0f;
			}

			last = now;
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	unsigned int Rectangles::initVbo(void)
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

	void Rectangles::newRect(void)
	{
		Parameter *axisP	= parameters[AXIS_NAME];
		assert(axisP != NULL);
		float axisVal			= axisP->getValue();

		Parameter *rectWidthP	= parameters[RECT_WIDTH_NAME];
		assert(rectWidthP != NULL);
		float rectWidth			= rectWidthP->getValue();

		Parameter *rectHeightP	= parameters[RECT_HEIGHT_NAME];
		assert(rectHeightP != NULL);
		float rectHeight		= rectHeightP->getValue();

		Parameter *rectDepthP	= parameters[RECT_DEPTH_NAME];
		assert(rectDepthP != NULL);
		float rectDepth			= rectDepthP->getValue();

		Parameter *centreOffsetP	= parameters[CENTRE_OFFSET_NAME];
		assert(centreOffsetP != NULL);
		float centreOffset			= centreOffsetP->getValue();

		int axis		= (unsigned int)((getRandom () * axisVal) * 3.0f);
		float width		= getRandom() * (MAX_RECT_WIDTH * rectWidth);
		float height	= getRandom() * (MAX_RECT_HEIGHT * rectHeight);
		float depth		= getRandom() * (MAX_RECT_DEPTH * rectDepth);
		float px		= getRandom() * (MAX_X_OFFSET * centreOffset);
		float py		= getRandom() * (MAX_Y_OFFSET * centreOffset);
		float pz		= getRandom() * (MAX_Z_OFFSET * centreOffset);

		RectData *rd	= &rects[rectPtr];
		assert(rd != NULL);

		rd->axis		= axis;
		rd->createdTime = glutGet(GLUT_ELAPSED_TIME);
		rd->width		= width;
		rd->height		= height;
		rd->depth		= depth;
		rd->enabled		= true;
		
		rd->offset.setX(px);
		rd->offset.setY(py);
		rd->offset.setZ(pz);

		rectPtr = (rectPtr + 1) % maxRects;
	}
}
