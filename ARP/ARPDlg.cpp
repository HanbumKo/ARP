
// ARPDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ARP.h"
#include "ARPDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CARPDlg ��ȭ ����




CARPDlg::CARPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARPDlg::IDD, pParent),
	CBaseLayer("ARPDlg")
{
	mEdit_GARPEther = _T("");
	mEdit_MyEther = _T("");

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_LayerMgr.AddLayer(this);
	m_LayerMgr.AddLayer(new CTCPLayer("TCP"));
	m_LayerMgr.AddLayer(new CIPLayer("IP"));
	m_LayerMgr.AddLayer(new CARPLayer("ARP"));
	m_LayerMgr.AddLayer(new CEthernetLayer("ETHER"));
	m_LayerMgr.AddLayer(new CNILayer("NI"));

	m_LayerMgr.ConnectLayers("NI ( *Ethernet ( *ARP ( +IP ( *TCP ( *ARPDlg ) ) ) -IP ) )"); 

	m_TCP = (CTCPLayer *)m_LayerMgr.GetLayer("TCP");
	m_IP = (CIPLayer *)m_LayerMgr.GetLayer("IP");
	m_ARP = (CARPLayer *)m_LayerMgr.GetLayer("ARP");
	m_Ether = (CEthernetLayer *)m_LayerMgr.GetLayer("ETHER");
	m_NI = (CNILayer *)m_LayerMgr.GetLayer("NI");
	
}

void CARPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CARPDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CARPDlg �޽��� ó����

BOOL CARPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CARPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CARPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CARPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CARPDlg::SetDlgState(int state)
{
	UpdateData( TRUE ) ;

	CString		device_description;

	CButton*	pMYDEV_SelectButton = (CButton*) GetDlgItem(IDC_MYDEV_SELECT);

	CButton*	pARP_ItemDeleteButton = (CButton*) GetDlgItem(IDC_ARP_ITEM_DELETE);
	CButton*	pARP_AllDeleteButton = (CButton*) GetDlgItem(IDC_ARP_ALL_DELETE);
	CButton*	pARP_RequestButton = (CButton*) GetDlgItem(IDC_ARP_REQUEST);

	CButton*	pPARP_AddButton = (CButton*) GetDlgItem(IDC_PARP_ADD);
	CButton*	pPARP_DeleteButton = (CButton*) GetDlgItem(IDC_PARP_DELETE);

	CButton*	pGARP_AddButton = (CButton*) GetDlgItem(IDC_GARP_REQUEST);

	CEdit*		pGARP_EtherEdit = (CEdit*) GetDlgItem(IDC_GARP_ETHERNET);
	CEdit*		pMYDEV_EtherEdit = (CEdit*) GetDlgItem(IDC_MYDEV_ETHERNET);

	CComboBox*	pMYDEV_EtherComboBox = (CComboBox*) GetDlgItem(IDC_MYDEV_LIST);

	CIPAddressCtrl* pMYDEV_IPAddrCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_MYDEV_IP);
	CIPAddressCtrl* pARP_IPAddrCtrl = (CIPAddressCtrl*) GetDlgItem(IDC_ARP_DSTIP);

	switch(state)
	{
	case ARP_INITIALIZING:
		break;

	case ARP_OPERATING:
		break;

	case ARP_ENDPROCESSING:
		break;

	case ARP_DEVICE:
		break;
	}
}