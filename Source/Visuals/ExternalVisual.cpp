#include "Visuals/ExternalVisual.h"
#include <assert.h>
#include <osc\OscOutboundPacketStream.h>
#include <osc\OscPacketListener.h>
#include "JDHUtility/OSCReciever.h"
#include "JDHUtility/OSCSender.h"
#include <ip/UdpSocket.h>
#include "JDHUtility/Ndelete.h"
#include "Waves/WavesGlobalState.h"

using namespace JDHUtility;
using namespace Waves;

namespace WavesVisuals
{

	const char *ExternalVisual::PROFILEFORMAT = "/waves/%s";
	ExternalVisual::ExternalVisual(std::string name,std::vector<std::string> vals) :	//Added 14th
		Visual(name, true)
	{
		this->vals = vals;
		sender = new OSCSender(WavesGlobalState::externalIpAddress, 3334);
		initParameters(false);
	}


	ExternalVisual::~ExternalVisual(void)
	{
		NDELETE(sender);
		NDELETE(s);
	}

	void ExternalVisual::draw(void)
	{
		
	//HACK: PROBLEMS WILL ARISE WITH THUMBNAIL RENDERING

	}

	Visual *ExternalVisual::getInstance(void) const
	{
		return new ExternalVisual(name,vals);
	}

	void ExternalVisual::render(void)
	{
		printf("render");
	}

	void ExternalVisual::update(void)
	{
		printf("Start of draw Loop.");
		
		char msgBuffer[BUFFER_SIZE];
		osc::OutboundPacketStream p(msgBuffer, BUFFER_SIZE);

		char profileBuffer[BUFFER_SIZE];
		sprintf(profileBuffer, PROFILEFORMAT, name.c_str());

		float stuff [2];
		stuff[0] = 0;
		stuff[1] = 1;

		std::string temp = "BundleTest";

		p << osc::BeginMessage(profileBuffer);
		for(std::map<std::string, Parameter *>::iterator it = parameters.begin(); it != parameters.end(); it++)
		{
				printf("In Param loop");
				Parameter *a = (*it).second;
				assert(a);

				std::string name = a->getName();
				float val = a->getValue();

				p << name.c_str();
				p << val;
		}
			
		p << osc::EndMessage;

		sender->send(p);
		
		
	}


	void ExternalVisual::initParameters(bool combinedScale)
	{
		//Create Iterator
		std::vector<std::string>::iterator itr;
		
		//For all incoming values
		for(itr = vals.begin();itr < vals.end(); itr++)
		{

			//Create Temp value then create parameter
			std::string temp = "";
			temp = *itr;
			parameters[temp] = new Parameter(temp, 0.5f);


		}
	}
}
