/**
 * Class:	OSCSender
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#pragma once
#include <ip\IpEndpointName.h>
#include <ip\UdpSocket.h>
#include <osc\OscOutboundPacketStream.h>

namespace JDHUtility
{
	class OSCSender
	{
	public:
		OSCSender	(const char *address, unsigned int port);
		~OSCSender	(void);

		void send(osc::OutboundPacketStream p) const;

	private:
		const char			*address;
		unsigned int		port;
		UdpTransmitSocket	*transmitSocket;
	};
}

