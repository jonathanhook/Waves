#pragma once

#include "Visuals/Visual.h"
#include "GridScrollMenu.h"
#include "FastDelegate.h"
#include "GridScrollMenu.h"
#include "Button.h"
#include <osc/OscPacketListener.h>
#include <ip/UdpSocket.h>

using namespace fastdelegate;
namespace JDHUtility { class OSCSender;  }

namespace Waves
{
	class Udpsocket;

	class ExternalVisualScrollMenu: 
		public GridScrollMenu, osc::OscPacketListener
	{
	public:
		typedef FastDelegate1<Visual *> VisualSelectedCallback;

		ExternalVisualScrollMenu(const Point2f &position, float width, float height, std::vector<Visual *> visuals);
		~ExternalVisualScrollMenu(void);

		static const unsigned int BUFFER_SIZE = 1024;
		static const char *PROFILEFORMAT;

		void setVisualSelectedCallback(VisualSelectedCallback visualSelected);
		void render(void);

	protected:
		void initInterfaceElements(void);

	private:
		std::vector<Visual *> visuals;
		VisualSelectedCallback visualSelected;
		Button *refreshButton;

		OSCSender *sender;
		UdpListeningReceiveSocket *s;

		void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);
		void refreshPressed (void);
		void gridScrollMenuItem_VisualSelected(Visual *visual);
		void addToVisual(void);
		void clearVisuals(void);

	};

	
}


