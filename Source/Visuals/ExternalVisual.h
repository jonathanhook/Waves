/**
 * Class:	ExternalVisual
 * Author:	Nichloas Mion
 * Email:	
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#pragma once
#include "Visuals/Visual.h"
#include <osc/OscPacketListener.h>
#include <ip/UdpSocket.h>

namespace JDHUtility { class OSCSender;  }

namespace WavesVisuals
{
	class Udpsocket;

	class ExternalVisual : 
		public Visual 
	{
	public:
		static const unsigned int BUFFER_SIZE = 1024;
		static const char *PROFILEFORMAT;

		ExternalVisual(std::string name, std::vector<std::string> vals = std::vector<std::string>());
		~ExternalVisual(void);

		void draw(void);
		Visual *getInstance(void) const;
		void update(void);

	private:
		OSCSender *sender;
		UdpListeningReceiveSocket *s;
		std::vector<std::string> vals;

		void initParameters(bool combinedScale);
		void render(void);
		
	};
}

