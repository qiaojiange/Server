
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "json/json.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CServerDlg 对话框

 const LPCTSTR CServerDlg::ADDRESS = _T("127.0.0.1");
 const USHORT CServerDlg::HTTP_PORT = 8888;

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Server_Info, m_ServerInfo);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Server_Start, &CServerDlg::OnBnClickedButtonServerStart)
	ON_WM_DESTROY()
	
	ON_BN_CLICKED(IDC_BUTTON_Sever_Stop, &CServerDlg::OnClickedButtonSeverStop)
	ON_BN_CLICKED(IDC_BUTTON_Test, &CServerDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码


	m_pHelp = CHelp::newInstance();

	//初始化接口
	m_pServerListener = new CHttpServerListenerImpl(HTTP_NAME);

	m_pServerPtr = new CHttpServerPtr(m_pServerListener);
	m_pServer = *m_pServerPtr;


	m_pHelp->setMainWnd(this);
	m_pHelp->setInfoList(&m_ServerInfo);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedButtonServerStart()
{
#ifdef SHOW_TRACE
	TRACE("click --CServerDlg::OnBnClickedStart-\n");
#endif

	if (m_pServer->Start(ADDRESS,HTTP_PORT))
	{
#ifdef SHOW_TRACE
		TRACE("server start success!!!\n");
#endif
		
		m_pHelp->LogServerStart(ADDRESS,HTTP_PORT,HTTP_NAME);
		setServerState(ST_STARTED);

	}else{

#ifdef SHOW_TRACE
			TRACE("server start failse!!!\n");
#endif
		m_pHelp->LogServerStartFail(m_pServer->GetLastError(),m_pServer->GetLastErrorDesc(),HTTP_NAME);
	//	SetAppState(ST_STOPPED);

	}


}


void CServerDlg::OnDestroy()
{

	CHelp::destory();
	if (m_pServerPtr!=NULL)
	{
			delete m_pServerPtr;
	}

	if (NULL != m_pServer)
	{
		delete m_pServer;
	}
	
	if (NULL != m_pServerListener)
	{
		delete m_pServerListener;
	}


	CDialogEx::OnDestroy();
}


//void CServerDlg::OnBnClickedButtonStop()
//{
//	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
//#ifdef SHOW_TRACE
//		TRACE(_T("click --CServerDlg::OnBnClickedStop-\n"));
//#endif
//	
//	if(!m_pServer->Stop()){
//		ASSERT(FALSE);
//	}
//
//
//}


// 设置状态


void CServerDlg::setServerState(EnAppState state)
{

	if (this->GetSafeHwnd() ==nullptr)
	{
		return ;
	}

	GetDlgItem(IDC_BUTTON_Server_Start)->EnableWindow(state == ST_STOPPED);
	GetDlgItem(IDC_BUTTON_Sever_Stop)->EnableWindow(state == ST_STARTED);
	//GetDlgItem(idc_)
}


void CServerDlg::OnClickedButtonSeverStop()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	
#ifdef SHOW_TRACE
	TRACE(_T("click --CServerDlg::OnBnClickedStop-\n"));
#endif

	if(!m_pServer->Stop()){
		ASSERT(FALSE);
	}
	m_pHelp->LogServerStop(HTTP_NAME);
	setServerState(ST_STOPPED);
}


void CServerDlg::OnBnClickedButtonTest()
{
	
	//char* str =  "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
	//Json::Reader reader;
	//Json::Value root;
	//if (reader.parse(str,root))
	//{
	//	std::string uploadid = root["uploadid"].asString();
	//	CStringA strUploadid;
	//	strUploadid.Format("%s",uploadid.c_str());
	//	TRACE(strUploadid);
	//	//TRACE(_T("-----%s"),uploadid.c_str());
	//	//TRACE(_T(uploadid));
	//	int code = root["code"].asInt();
	//	TRACE(_T("---code = %d"),code);
	//}

	Json::Value root;
	Json::Value device;
	device["device"]="camera";
	Json::Value operate;
	operate["operate"] = "connect";

	Json::Value parameter;
	parameter["parameter"] = "parameter";

	root["command"].append(device);
	root["command"].append(operate);
	root["command"].append(parameter);
	//有格式输出
	std::string strTemp = root.toStyledString();
	CStringA strTempA;
	strTempA.Format("%s",strTemp.c_str());
	
	//输出无格式json字符串
	Json::FastWriter writer;
	strTemp = writer.write(root);
	strTempA.Format("%s",strTemp.c_str());

	TRACE(strTempA);




}
