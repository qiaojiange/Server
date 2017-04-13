
// ServerDlg.h : 头文件
//

#pragma once

#include "http/Help.h"
#include "http/HPSocket.h"
#include "http/HttpServerListenerImpl.h"
// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonServerStart();


private :
	static const USHORT HTTP_PORT;
	static const LPCTSTR ADDRESS;

	CHelp* m_pHelp;
	CHttpServerListenerImpl* m_pServerListener;
	CHttpServerPtr* m_pServerPtr;
	IHttpServer* m_pServer;
public:
	afx_msg void OnDestroy();
//	afx_msg void OnBnClickedButtonStop();
	CListBox m_ServerInfo;
//	void SetServerState(void);
	void setServerState(EnAppState state);
	afx_msg void OnClickedButtonSeverStop();
	afx_msg void OnBnClickedButtonTest();
};
