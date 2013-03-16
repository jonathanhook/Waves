/**
 * Class:	OSCReciever
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <stdio.h>
#include <ip/UdpSocket.h>
#include "OSCReciever.h"

namespace JDHUtility
{
	/* Private Constants */
	const char *OSCReciever::STARTUP_MESSAGE = "Listening for OSC messages on port: %d\n";

	/* Constructors */
	OSCReciever::OSCReciever(unsigned long address, unsigned int port, bool autoStart)
	{
		this->address	= address;
		this->port		= port;

		if(autoStart)
		{
			start();
		}
	}

	OSCReciever::~OSCReciever(void)
	{
	}

	/* Public Member Functions */
	void OSCReciever::start(void)
	{
		printf(STARTUP_MESSAGE, port);

		Listener listener;
		UdpListeningReceiveSocket s(IpEndpointName(address, port), &listener);
		s.RunUntilSigInt();
	}
}
