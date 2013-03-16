#include "ExternalVisualScrollMenu.h"
#include "GridScrollMenuItem.h"
#include "button.h"
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/OSCReciever.h"
#include "JDHUtility/OSCSender.h"
#include <osc\OscPacketListener.h>
#include "JDHUtility/Ndelete.h"
#include <ip/UdpSocket.h>
#include <iostream>
#include "Visuals/ExternalVisual.h"
#include "WavesGlobalState.h"

namespace Waves
{
	const char *ExternalVisualScrollMenu::PROFILEFORMAT = "/waves/%s";
	ExternalVisualScrollMenu::ExternalVisualScrollMenu(const Point2f &position, float width, float height, std::vector<Visual *> visuals) 
		: GridScrollMenu(position, width, height)
	{
		sender = new OSCSender(WavesGlobalState::externalIpAddress,3335);

		this->visuals = visuals;
		initInterfaceElements();

		s = new UdpListeningReceiveSocket(IpEndpointName(IpEndpointName::ANY_ADDRESS, 3337), this);

	}


	ExternalVisualScrollMenu::~ExternalVisualScrollMenu(void)
	{
		NDELETE(s);
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void ExternalVisualScrollMenu::setVisualSelectedCallback(VisualSelectedCallback visualSelected)
	{
		this->visualSelected = visualSelected;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void ExternalVisualScrollMenu::gridScrollMenuItem_VisualSelected(Visual *visual)
	{
		if(visualSelected != NULL)
		{
			visualSelected(visual);
		}
	}

	void ExternalVisualScrollMenu::initInterfaceElements(void)
	{

		float borderDim = BORDER_DIM * 5.0f;

		float buttonWidth = ((width * 0.6) / 2.0f) - borderDim; 
		float buttonHeight = (height * (1.0f - 0.8)) - (borderDim * 2.0f);
		Button *refreshButton = new Button(Point2f((borderDim * 2.0f) + buttonWidth, borderDim + (height * 0.8)), buttonWidth, buttonHeight);
		refreshButton->setText("Refresh");
		refreshButton->setClickedCallback(MakeDelegate(this, &ExternalVisualScrollMenu::refreshPressed));
		registerInterfaceElement(refreshButton);


		float miWidth = itemWidth;

		for(unsigned int i = 0; i < visuals.size(); i++)
		{
			float xPos = miWidth * (float)(i % GRID_DIM);
			float yPos = itemHeight * (float)(i / GRID_DIM);

			GridScrollMenuItem *mi = new GridScrollMenuItem(Point2f(xPos, yPos), miWidth, itemHeight, visuals[i]);
			mi->setVisualSelected(MakeDelegate(this, &ExternalVisualScrollMenu::gridScrollMenuItem_VisualSelected));
			registerInterfaceElement(mi);
		}
	}

	void ExternalVisualScrollMenu::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		if(displayList)
		{
			glNewList(displayList, GL_COMPILE);

			glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
			glLineWidth(1.0f);
			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);

			glBegin(GL_LINE_LOOP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(width, 0.0f, 0.0f);
			glVertex3f(width, height, 0.0f);
			glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glPopAttrib();

			glEndList();

			dirty = false;
		}
		glCallList(displayList);

		renderInterfaceElements();

		glPopMatrix();	
	}

	void ExternalVisualScrollMenu::addToVisual(void)
	{


	}

	void ExternalVisualScrollMenu::refreshPressed(void)
	{

		string name = "updateRequest";

		char msgBuffer[BUFFER_SIZE];
		osc::OutboundPacketStream p(msgBuffer, BUFFER_SIZE);

		char profileBuffer[BUFFER_SIZE];
		sprintf(profileBuffer, PROFILEFORMAT, name.c_str());

		float stuff [2];
		stuff[0] = 0;
		stuff[1] = 1;
		p << osc::BeginMessage(profileBuffer);
		p << name.c_str();
		p << osc::EndMessage;


		sender->send(p);
		//Send the message
		s->Run();

		/*
		Process message will create new visuals, adding them to the visuals list
		When the break command is sent after a specific amount of time.
		*/


		//Recreate the Visuals list
		float miWidth = itemWidth;
		
		for(unsigned int i = 0; i < visuals.size(); i++)
		{
			float xPos = miWidth * (float)(i % GRID_DIM);
			float yPos = itemHeight * (float)(i / GRID_DIM);

			GridScrollMenuItem *mi = new GridScrollMenuItem(Point2f(xPos, yPos), miWidth, itemHeight, visuals[i]);
			mi->setVisualSelected(MakeDelegate(this, &ExternalVisualScrollMenu::gridScrollMenuItem_VisualSelected));
			registerInterfaceElement(mi);
		}

		printf("\nEnd of Menu Addition");//Works

	}	

	void ExternalVisualScrollMenu::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint)
	{

		printf("\nTesting process listening "); //Works

		//Take in package and break down into arguments


		std::vector<std::string> vals;
		/*Go through arguements. 
		0 = /Reply
		1 = Name
		2 = All arguments

		*/

		string name = "";
		bool aBreak = true;
		int i = 0;
		m;
		//EDITED
		vals.clear();
		for(osc::ReceivedMessageArgumentIterator it = m.ArgumentsBegin(); it != m.ArgumentsEnd(); it++, i++)
		{
			if (it->IsString())
			{
				//Convert the message into a usable string format
				std::string wub = ""; 
				wub = std::string (it->AsString());
				//If it is the first element
				if (i == 0)
				{
					//Check Break
					if (wub == "break")
					{
							printf("\nAccessed");
							s->AsynchronousBreak();
							aBreak = false;
					//Otherwise set the name
					}else
					{
						//Assign the string as the EV name
						name = wub;
					}
				//If not frst element
				}else
				{
					//Push into the parameters array
					vals.push_back(wub);
				}

				printf("THIS IS ONE STATEMENT");


			}else
			{
				printf("THIS IS ANOTHER STATMENT");

			}
			i++;

		}

		//Call External Visual Constructor
		//if( i >= 0)
		if( aBreak)
		{
			ExternalVisual* newVisual = new ExternalVisual (name,vals);
			//Add to the visuals list
			visuals.push_back(newVisual);
		}
	}

}