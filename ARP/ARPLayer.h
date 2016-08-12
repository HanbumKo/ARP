#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseLayer.h"

class CARPLayer
	: public CBaseLayer
{
private:
	inline void		ResetHeader( );


	// Cache table data structure


public:
	CARPLayer( char* pName );
	virtual ~CARPLayer();

	void SetSrcIPAddress(unsigned char* src_ip);
	void SetDstIPAddress(unsigned char* dst_ip);
	void SetEnetSrcAddress(unsigned char* pAddress);
	void SetEnetDstAddress(unsigned char* pAddress);
	void SetOption(unsigned int arp_option);

	unsigned char* GetSrcIPAddress();
	unsigned char* GetDstIPAddress();
	unsigned char* GetEnetSrcAddress();
	unsigned char* GetEnetDstAddress();
	unsigned int GetOption();

	BOOL Send(unsigned char* ppayload, int nlength);
	BOOL Receive(unsigned char* ppayload);

	typedef struct _ARPLayer_HEADER {
		unsigned short arp_hardType;
		unsigned short arp_protocolType;
		unsigned char arp_hardLength;
		unsigned char arp_protocolLength;
		unsigned short arp_option;
		ETHERNET_ADDR arp_srcHardAddress;
		IP_ADDR arp_srcProtocolAddress;
		ETHERNET_ADDR arp_dstHardAddress;
		IP_ADDR arp_dstProtocolAddress;
	} ARPLayer_HEADER, *PARPLayer_HEADER;

protected:
	ARPLayer_HEADER m_sHeader;
};