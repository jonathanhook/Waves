/**
 * Class:	InterfaceElement
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"
#include "WavesGlobalState.h"

using namespace JDHUtility;

namespace Waves
{
	/* PROTECTED CONSTANTS */
	const float InterfaceElement::BAR_DIM = 0.0075f;
	const float InterfaceElement::BORDER_DIM = 0.001f;
	const float InterfaceElement::ICON_LINE_WIDTH = 0.75f;
	const float InterfaceElement::LINE_WIDTH = 0.25f;
	const float InterfaceElement::LINE_OPACITY = 0.6f;
	const float InterfaceElement::MASK_OPACITY = 0.75f;
	const float InterfaceElement::SPLINE_LINE_WIDTH = 3.0f;
	const float InterfaceElement::TEXT_LINE_WIDTH = 0.05f;

	/* CONSTRUCTORS */
	InterfaceElement::InterfaceElement(const Point2f &position, float width, float height)
	{
		this->position = position;
		this->width = width;
		this->height = height;

		dirty = true;
		displayList = glGenLists(1);
		id = rand();	
		markedForDelete = false;
		modelview = new GLMatrixf();
		transformSaved = false;
	}

	InterfaceElement::~InterfaceElement(void)
	{
		if(interfaceElements.size() > 0)
		{
			for(std::map<int, InterfaceElement *>::iterator it = interfaceElements.begin(); it != interfaceElements.end(); it++)
			{
				if((*it).second != NULL) delete (*it).second;
				(*it).second = NULL;

				WavesGlobalState *wgs = WavesGlobalState::getInstance();
				assert(wgs != NULL);

				wgs->decrementInterfaceElementCount();
			}
			interfaceElements.clear();
		}

		if(modelview != NULL) delete modelview;
		modelview = NULL;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool InterfaceElement::contains(const FingerEventArgs &e) const
	{
		assert(modelview != NULL);

		Point2f p =	e.getPosition();
		modelview->unTranslatePoint(p);

		return p.getX() >= 0.0f &&
			p.getX() <= (width * modelview->getXScale()) &&
			p.getY() >= 0.0f &&
			p.getY() <= (height * modelview->getYScale());
	}

	void InterfaceElement::deleteInterfaceElement(InterfaceElement *e)
	{
		assert(e != NULL);

		unregisterEventHandler(e);
		e->markedForDelete = true;
	}

	std::map<int, InterfaceElement *> InterfaceElement::getInterfaceElements(void) const
	{
		return interfaceElements;
	}	

	float InterfaceElement::getHeight() const
	{
		return height;
	}

	unsigned int InterfaceElement::getId() const
	{
		return id;
	}
	
	float InterfaceElement::getWidth() const
	{
		return width;
	}

	Point2f &InterfaceElement::getPosition(void)
	{
		return position;	
	}

	bool InterfaceElement::isMarkedForDelete(void) const
	{
		return markedForDelete;
	}

	bool InterfaceElement::isTransformSaved(void) const
	{
		return transformSaved;
	}

	void InterfaceElement::registerInterfaceElement(InterfaceElement *e)
	{
		assert(e != NULL);

		registerEventHandler(e);
		interfaceElements[e->getId()] = e;

		WavesGlobalState::getInstance()->incrementInterfaceElementCount();
	}

	void InterfaceElement::setDirty(bool dirty)
	{
		this->dirty = dirty;
	}

	void InterfaceElement::setHeight(float height)
	{
		this->height = height;
	}

	void InterfaceElement::setId(unsigned int id)
	{
		this->id = id;
	}

	void InterfaceElement::setMarkedForDelete(bool markedForDelete)
	{
		this->markedForDelete = markedForDelete;
	}

	void InterfaceElement::setPosition(const Point2f &position)
	{
		this->position = position;
	}

	void InterfaceElement::setWidth(float width)
	{
		this->width = width;
	}

	void InterfaceElement::update(void)
	{
		updateInterfaceElements();
	}

	/* PROTECTED MEMBER FUNCTIONS */
	float InterfaceElement::getLineWidth(float width) const
	{
		assert(modelview != NULL);

		return width * modelview->getXScale();
	}

	void InterfaceElement::initInterfaceElements(void)
	{

	}

	void InterfaceElement::renderInterfaceElements(void)
	{
		for(std::map<int, InterfaceElement*>::iterator it = interfaceElements.begin(); it != interfaceElements.end(); it++)
		{
			InterfaceElement *e = (*it).second;
			assert(e != NULL);

			if(e->isEnabled())
			{
				e->render();
			}
		}
	}

	void InterfaceElement::renderBackgroundMask(void)
	{
		glPushAttrib(GL_CURRENT_BIT);

		glColor4f(0.0f, 0.0f, 0.0f, MASK_OPACITY);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(width, 0.0f, 0.0f);
			glVertex3f(width, height, 0.0f);
			glVertex3f(0.0f, height, 0.0f);
		glEnd();

		glPopAttrib();
	}

	void InterfaceElement::saveTransform()
	{
		assert(modelview != NULL);

		glGetFloatv(GL_MODELVIEW_MATRIX, modelview->getMatrix());
		transformSaved = true;
	}

	void InterfaceElement::updateInterfaceElements(void)
	{
		for(std::map<int, InterfaceElement *>::iterator it = interfaceElements.begin(); it != interfaceElements.end(); )
		{
			assert((*it).second != NULL);

			if((*it).second->isMarkedForDelete())
			{
				if((*it).second != NULL) delete (*it).second;
				(*it).second = NULL;

				it = interfaceElements.erase(it);
				
				WavesGlobalState *wgs = WavesGlobalState::getInstance();
				assert(wgs != NULL);

				wgs->decrementInterfaceElementCount();
			}
			else
			{
				if((*it).second->isEnabled()) (*it).second->update();
				it++;
			}
		}
	}
}
