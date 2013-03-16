/**
 * Class:	AndrewTest
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "AndrewTest.h"
#include "JDHUtility/GlutInclude.h"
#include "Visuals/Parameter.h"

namespace WavesVisuals
{
	const std::string AndrewTest::RADIUS_NAME = "Radius";

	/* CONSTRUCTORS */
	AndrewTest::AndrewTest(void) : 
		Visual("Andrew Test")
	{
		parameters["Red"] = new Parameter("Red", 1.0f);
		parameters[RADIUS_NAME] = new Parameter(RADIUS_NAME, 1.0f);
	}

	AndrewTest::~AndrewTest(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void AndrewTest::draw(void)
	{
		float red = getParameter("Red")->getValue(); 
		float size = 0.5f;
		float radius = getParameter(RADIUS_NAME)->getValue();

		//glColor3f(red, 0.0f, 0.0f);
		glBegin(GL_LINES);
			glLineWidth(5.0f);
			glVertex3f(-0.25f, 0.0f, 0.0f);
			glVertex3f(0.25f, 0.0f, 0.0f);
		glEnd();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(-0.25f, 0.0f, 0.0f);
		glutSolidSphere(radius / 10.0f, 10, 10);
		glutWireSphere(radius / 10.0f, 10, 10);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(0.25f, 0.0f, 0.0f);
		glutSolidSphere(radius / 10.0f, 10, 10);
		glutWireSphere(radius / 10.0f, 10, 10);
		glPopMatrix();

	}
}
