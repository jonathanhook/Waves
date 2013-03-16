/**
 * Class:	OSCReciever
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#pragma once
#include <stdio.h>
#include <osc/OscPacketListener.h>

namespace JDHUtility
{
	class OSCReciever
	{
	public:
		OSCReciever(unsigned long address, unsigned int port, bool autoStart = true);
		~OSCReciever(void);

		void start(void);

	private:
		static const char *STARTUP_MESSAGE;

		unsigned long	address;
		unsigned int	port;
	};

	class Listener : 
		public osc::OscPacketListener
	{
	public:
		 virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint)
		 {
			 printf("%s ", m.AddressPattern());

			 osc::ReceivedMessageArgumentIterator it = m.ArgumentsBegin();
			 for( ; it != m.ArgumentsEnd(); it++)
			 {
				 if((*it).IsBool())
				 {
					 bool b = (*it).AsBool();
					 printf("%d ", b); 
				 }
				 else if((*it).IsChar())
				 {
					 char c = (*it).AsChar();
					 printf("%c ", c);
				 }
				 else if((*it).IsDouble())
				 {
					 double d = (*it).AsDouble();
					 printf("%f ", d);
				 }
				 else if((*it).IsFloat())
				 {
					 float f = (*it).AsFloat();
					 printf("%f ", f);
				 }
				 else if((*it).IsInt32())
				 {
					 int i = (*it).AsInt32();
					 printf("%i ", i);
				 }
				 else if((*it).IsString())
				 {
					const char *s = (*it).AsString();
					printf("%s ", s);
				 }
			 }

			 printf("\n");
		 }
	};
}

