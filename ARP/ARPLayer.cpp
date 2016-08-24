#include "stdafx.h"
#include "ARP.h"
#include "ARPLayer.h"
#include "EthernetLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CARPLayer::CARPLayer( char* pName)
: CBaseLayer( pName )
{
	ResetHeader();
	arp_table.clear();
}

CARPLayer::~CARPLayer()
{
}

void CARPLayer::ResetHeader()
{
	m_sHeader.arp_hardType = 0x0000;
	m_sHeader.arp_protocolType = 0x0000;
	m_sHeader.arp_hardLength = 0x06;
	m_sHeader.arp_protocolLength = 0x04;
	m_sHeader.arp_option = 0x0000;
	memset(m_sHeader.arp_srcEtherAddress.addrs, 0, 6);
	memset(m_sHeader.arp_srcIPAddress.addrs, 0, 4);
	memset(m_sHeader.arp_dstEtherAddress.addrs, 0, 6);
	memset(m_sHeader.arp_dstIPAddress.addrs, 0, 4);
}

BOOL CARPLayer::Send(unsigned char* ppayload, int nlength)
{
	/*
	unsigned char dstAddress[7];
    dstAddress[0] = 0xdc;
    dstAddress[1] = 0x0e;
    dstAddress[2] = 0xa1;
    dstAddress[3] = 0x71;
    dstAddress[4] = 0x04;
    dstAddress[5] = 0x3f;
	*/

	BOOL found = false;
	for(unsigned int i=0; i<arp_table.size(); i++) {
		if(get<1>(arp_table[i]) == GetDstIPAddress()) {
			SetEnetDstAddress(get<0>(arp_table[i]));
			found = true;
			break;
		}
	}

	BOOL bSuccess = FALSE;
	unsigned char ARP_BROADCAST[6] = {0xff, };
	if(!found) { // Send ARP Request
		tempPayload = make_pair(ppayload,nlength);

		// Cache table �� �߰�.
		arp_table.push_back(make_tuple( ARP_BROADCAST, GetDstIPAddress() , 3, FALSE));

		SetOption(ntohs(ARP_OPCODE_REQUEST));
		SetEnetDstAddress(ARP_BROADCAST);
		bSuccess = mp_UnderLayer->Send((unsigned char*)&m_sHeader, nlength);
	} else {
		((CEthernetLayer *)mp_UnderLayer)->SetEnetDstAddress(GetEnetDstAddress());
		bSuccess = mp_UnderLayer->Send(ppayload, nlength);
	}

    return true;
}

BOOL CARPLayer::Receive(unsigned char* ppayload)
{
	PARPLayer_HEADER pFrame =(PARPLayer_HEADER)ppayload;

	unsigned char ARP_BROADCAST[6];
	memset(ARP_BROADCAST, 0xff, 6);


	// �ڱⲨ�� update �ϰ�, reply message�� ���������.


	BOOL bSuccess;
	if( ntohs(pFrame->arp_option) == ARP_OPCODE_REQUEST &&
		memcmp((char *)(pFrame->arp_dstEtherAddress.addrs), ARP_BROADCAST,6) == 0 &&
		memcmp((char *)(pFrame->arp_dstIPAddress.addrs), m_sHeader.arp_srcIPAddress.addrs, 4) == 0)
	{
		// arp_cache table �ȿ� �����ϴ� ���� complete���� update����.
		


	}

	return bSuccess;
}

void CARPLayer::SetEnetSrcAddress(unsigned char *pAddress)
{
	memcpy(&m_sHeader.arp_srcEtherAddress.addrs, pAddress, 6);
}

void CARPLayer::SetEnetDstAddress(unsigned char *pAddress)
{
	memcpy(&m_sHeader.arp_dstEtherAddress.addrs, pAddress, 6);
}

void CARPLayer::SetSrcIPAddress(unsigned char* src_ip)
{
	memcpy(m_sHeader.arp_srcIPAddress.addrs, src_ip, 4);
}

void CARPLayer::SetDstIPAddress(unsigned char* dst_ip)
{
	memcpy(m_sHeader.arp_dstIPAddress.addrs, dst_ip, 4);
}

void CARPLayer::SetOption(unsigned int arp_option)
{
	m_sHeader.arp_option = arp_option;
}

unsigned char* CARPLayer::GetEnetSrcAddress()
{
	return m_sHeader.arp_srcEtherAddress.addrs;
}

unsigned char* CARPLayer::GetEnetDstAddress()
{
	return m_sHeader.arp_dstEtherAddress.addrs;
}

unsigned char* CARPLayer::GetSrcIPAddress()
{
	return m_sHeader.arp_srcIPAddress.addrs;
}

unsigned char* CARPLayer::GetDstIPAddress()
{
	return m_sHeader.arp_dstIPAddress.addrs;
}

unsigned int CARPLayer::GetOption()
{
	return m_sHeader.arp_option;
}