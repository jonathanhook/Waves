/**
 * Class:	OSCSender
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "Ndelete.h"
#include "OSCSender.h"

namespace JDHUtility
{
	/* Constructors */
	OSCSender::OSCSender(const char *address, unsigned int port)
	{
		this->address	= address;
		this->port		= port;
		
		transmitSocket = new UdpTransmitSocket(IpEndpointName(address, port));
	}

	OSCSender::~OSCSender(void)
	{
		NDELETE(transmitSocket);
	}

	/* Public Member Functions */
	void OSCSender::send(osc::OutboundPacketStream p) const
	{
		assert(transmitSocket);
		transmitSocket->Send(p.Data(), p.Size() );	
	}
}