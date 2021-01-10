
// SocketLatencyTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketLatencyTester.h"
#include "SocketLatencyTesterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CSocketLatencyTesterDlg dialog




CSocketLatencyTesterDlg::CSocketLatencyTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSocketLatencyTesterDlg::IDD, pParent)
	, m_nLocalPort(9091)
	, m_sLocalIP(_T("127.0.0.1"))
	, m_sRemoteIP(_T("127.0.0.1"))
	, m_nRemotePort(9091)
	, m_nMsgCnt(0)
	, m_dDiffTime(0)
	, m_dTickMicro(0)
	, m_pSendSock(NULL)
	, m_sRcvrLocalIP(_T("127.0.0.1"))
	, m_nLastRcvMsgCnt(0)
	, m_nLatencyMax(0)
	, m_nLatencyAvg(0)
	, n_nNumberOfSends(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	LARGE_INTEGER Frequency;
	const BOOL bRv = QueryPerformanceFrequency(&Frequency);
	assert(TRUE == bRv);
	m_dTickMicro = (1.0e6 / Frequency.QuadPart); //[MicroSec]
}

void CSocketLatencyTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nLocalPort);
	DDV_MinMaxUInt(pDX, m_nLocalPort, 0, 99999);
	DDX_Text(pDX, IDC_EDIT3, m_sLocalIP);
	DDV_MaxChars(pDX, m_sLocalIP, 16);
	DDX_Text(pDX, IDC_EDIT5, m_sRemoteIP);
	DDV_MaxChars(pDX, m_sRemoteIP, 16);
	DDX_Text(pDX, IDC_EDIT2, m_nRemotePort);
	DDV_MinMaxUInt(pDX, m_nRemotePort, 0, 99999);
	DDX_Text(pDX, IDC_STATIC_LATENCY, m_dDiffTime);
	DDX_Text(pDX, IDC_EDIT4, m_sRcvrLocalIP);
	DDX_Text(pDX, IDC_STATIC_LASTMSGCNT, m_nLastRcvMsgCnt);
	DDX_Text(pDX, IDC_STATIC_LATENCY_MAX, m_nLatencyMax);
	DDX_Text(pDX, IDC_STATIC_LATENCY_AVG, m_nLatencyAvg);
	DDX_Text(pDX, IDC_EDIT6, n_nNumberOfSends);
}

BEGIN_MESSAGE_MAP(CSocketLatencyTesterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSocketLatencyTesterDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BUTTON2, &CSocketLatencyTesterDlg::OnBnClickedButtonRcv)
	ON_BN_CLICKED(IDC_BUTTON3, &CSocketLatencyTesterDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSocketLatencyTesterDlg message handlers

BOOL CSocketLatencyTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketLatencyTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketLatencyTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketLatencyTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

struct MsgFormat
{
	unsigned long m_nMsgCnt;
	char m_buf[1024];
};

void CSocketLatencyTesterDlg::OnBnClickedSend()
{
	double dStart = 0;
	UpdateData();

	for (size_t nIteration = 0; nIteration < n_nNumberOfSends; nIteration++)
	{
		m_nMsgCnt++;
		if (NULL == m_pSendSock)
		{
			char *pLocalAddr =  ((m_sLocalIP == "127.0.0.1") ||(m_sLocalIP == "")) ? NULL : m_sLocalIP.GetBuffer();
			m_pSendSock = new CAsyncSocket();
			if (m_pSendSock->Create(0, SOCK_DGRAM , 0, pLocalAddr))
			{
				unsigned long nNonBlocking = 0;
				m_pSendSock->IOCtl(FIONBIO, &nNonBlocking);
			}
			else
			{
				CString sErr;
				sErr.Format("Err = %d", GetLastError());
				MessageBox(sErr, "Create fail");
				delete m_pSendSock ;
				m_pSendSock = NULL;
			}
		}

		if (NULL != m_pSendSock )
		{
			MsgFormat cMsg;
			cMsg.m_nMsgCnt = m_nMsgCnt;

			dStart = GetTicksTimeInMicro();
			if (m_pSendSock->SendTo(&cMsg, sizeof(MsgFormat), m_nRemotePort, m_sRemoteIP) != sizeof(MsgFormat))
			{
				CString sErr;
				sErr.Format("Err = %d", GetLastError());
				MessageBox(sErr, "Send sock fail");
			}

			CString sRcvAddr;
			UINT nRcvPort = 0;
			if (m_pSendSock->ReceiveFrom(&cMsg, sizeof(cMsg), sRcvAddr, nRcvPort) != sizeof(MsgFormat))
			{
				CString sErr;
				sErr.Format("Err = %d", GetLastError());
				MessageBox(sErr, "Rcv From Failed");
			}
			double dEnd = GetTicksTimeInMicro();

			if (cMsg.m_nMsgCnt != m_nMsgCnt)
			{
				MessageBox("Unexpected MsgCnt", "Rcv From Failed");
			}

			m_dDiffTime = int(dEnd - dStart);

			if (m_dDiffTime > m_nLatencyMax)
			{
				m_nLatencyMax = (UINT)m_dDiffTime;
			}

			if (0 == m_nLatencyAvg)
			{
				m_nLatencyAvg = (UINT)m_dDiffTime;
			}
			else
			{
				m_nLatencyAvg = (UINT)(((m_nLatencyAvg * (m_nMsgCnt - 1)) + m_dDiffTime)/ (m_nMsgCnt));
			}


		}
		UpdateData(false);
		::Sleep(50);
	}
}


void CSocketLatencyTesterDlg::OnBnClickedButtonRcv()
{
	while(1)
	{
		MSG msgCur;
		::PeekMessage(&msgCur, NULL, NULL, NULL, PM_REMOVE);

		CAsyncSocket cSendSock;
		CAsyncSocket cRcvSock;
		MsgFormat cMsg;
		char *pLocalAddr =  ((m_sRcvrLocalIP == "127.0.0.1") ||(m_sLocalIP == "")) ? NULL : m_sRcvrLocalIP.GetBuffer();
		if (!cSendSock.Create(0, SOCK_DGRAM , 0, pLocalAddr))
		{
			CString sErr;
			sErr.Format("Err = %d", GetLastError());
			MessageBox(sErr, "Create fail");
		}
		else
		{
			if (!cRcvSock.Create(m_nLocalPort, SOCK_DGRAM , 0, pLocalAddr))
			{
				CString sErr;
				sErr.Format("Err = %d", GetLastError());
				MessageBox(sErr, "Create fail");
			}
			else
			{
				unsigned long nNonBlocking = 0;
				cRcvSock.IOCtl(FIONBIO, &nNonBlocking);
				CString sRcvAddr;
				UINT nRcvPort;
				if (cRcvSock.ReceiveFrom(&cMsg, sizeof(cMsg), sRcvAddr, nRcvPort) == sizeof(MsgFormat))
				{
					m_nLastRcvMsgCnt = cMsg.m_nMsgCnt;
					UpdateData(FALSE);
					if (cSendSock.SendTo(&cMsg, sizeof(MsgFormat), nRcvPort, sRcvAddr) != sizeof(MsgFormat)) // send back
					{
						CString sErr;
						sErr.Format("Err = %d", GetLastError());
						MessageBox(sErr, "Send sock fail");
					}
				}
				else
				{
					CString sErr;
					sErr.Format("Err = %d", GetLastError());
					MessageBox(sErr, "Rcv From Failed");
				}
			}
		}
	}
}

double CSocketLatencyTesterDlg::GetTicksTimeInMicro()
{
	LARGE_INTEGER PerfCounter;
	const BOOL bRv = QueryPerformanceCounter(&PerfCounter);
	assert(TRUE == bRv);
	double dTimeMicro = PerfCounter.QuadPart * m_dTickMicro;
	return dTimeMicro;
}


void CSocketLatencyTesterDlg::OnBnClickedButton3()
{
	UpdateData(true);
	m_dDiffTime = 0.;
	m_nLatencyMax = (UINT)0;
	m_nLatencyAvg = (UINT)0;
	m_nMsgCnt = 0;
	UpdateData(false);
}
