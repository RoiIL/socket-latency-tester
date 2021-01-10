
// SocketLatencyTesterDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CSocketLatencyTesterDlg dialog
class CSocketLatencyTesterDlg : public CDialogEx
{
// Construction
public:
	CSocketLatencyTesterDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOCKETLATENCYTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int m_nMsgCnt;
	double m_dTickMicro;
	double GetTicksTimeInMicro();
	CAsyncSocket *m_pSendSock;
public:
	afx_msg void OnBnClickedSend();
	UINT m_nLocalPort;
	CString m_sLocalIP;
	CString m_sRemoteIP;
	UINT m_nRemotePort;
	afx_msg void OnBnClickedButtonRcv();
	double m_dDiffTime;
	CString m_sRcvrLocalIP;
	UINT m_nLastRcvMsgCnt;
	UINT m_nLatencyMax;
	UINT m_nLatencyAvg;
	UINT n_nNumberOfSends;
	afx_msg void OnBnClickedButton3();
};
