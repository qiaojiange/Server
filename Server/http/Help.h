#pragma once

#include "HPTypeDef.h"

#define USER_INFO_MSG			(WM_USER + 100)
#define MAX_LOG_RECORD_LENGTH	1000

#define EVT_ON_SEND				_T("OnSend")
#define EVT_ON_RECEIVE			_T("OnReceive")
#define EVT_ON_CLOSE			_T("OnClose")
#define EVT_ON_ERROR			_T("OnError")
#define EVT_ON_PREPARE_CONNECT	_T("OnPrepareConnect")
#define EVT_ON_PREPARE_LISTEN	_T("OnPrepareListen")
#define EVT_ON_ACCEPT			_T("OnAccept")
#define EVT_ON_CONNECT			_T("OnConnect")
#define EVT_ON_HAND_SHAKE		_T("OnHandShake")
#define EVT_ON_SHUTDOWN			_T("OnShutdown")
#define EVT_ON_END_TEST			_T("END TEST")

#define EVT_ON_MESSAGE_BEGIN	_T("OnMessageBegin")
#define EVT_ON_REQUEST_LINE		_T("OnRequestLine")
#define EVT_ON_STATUS_LINE		_T("OnStatusLine")
#define EVT_ON_HEADER			_T("OnHeader")
#define EVT_ON_HEADERS_COMPLETE	_T("OnHeadersComplete")
#define EVT_ON_BODY				_T("OnBody")
#define EVT_ON_CHUNK_HEADER		_T("OnChunkHeader")
#define EVT_ON_CHUNK_COMPLETE	_T("OnChunkComplete")
#define EVT_ON_MESSAGE_COMPLETE	_T("OnMessageComplete")
#define EVT_ON_UPGRADE			_T("OnUpgrade")
#define EVT_ON_PARSE_ERROR		_T("OnParseError")

#define EVT_ON_WS_MSG_HEADER	_T("OnWSMessageHeader")
#define EVT_ON_WS_MSG_BODY		_T("OnWSMessageBody")
#define EVT_ON_WS_MSG_COMPLETE	_T("OnWSMessageComplete")

#define EVT_ON_UNCOMPRESS_BODY		_T("Uncompress Body")
#define EVT_ON_UNCOMPRESS_BODY_FAIL	_T("Uncompress Body Fail")


#define HTTP_NAME					_T("http")
#define HTTPS_NAME					_T("https")

#define CRLF						"\r\n"
#define NV_SEPARATOR_CHAR			'='
#define HEADER_SEPARATOR			": "
#define COOKIE_TOKENIZE				"; "
#define STR_HTTP_1_0				"HTTP/1.0"
#define STR_HTTP_1_1				"HTTP/1.1"
#define HOST_HEADER					"Host"
#define COOKIE_HEADER				"Cookie"
#define SET_COOKIE_HEADER			"Set-Cookie"
#define CONTENT_TYPE_HEADER			"Content-Type"
#define CONTENT_LENGTH_HEADER		"Content-Length"
#define TRANSFER_ENCODING_HEADER	"Transfer-Encoding"
#define UPGRADE_HEADER				"Upgrade"
#define WEB_SOCKET_HEADER_VALUE		"WebSocket"

#define HTTP_METHOD_POST			"POST"
#define HTTP_METHOD_PUT				"PUT"
#define HTTP_METHOD_PATCH			"PATCH"
#define HTTP_METHOD_GET				"GET"
#define HTTP_METHOD_DELETE			"DELETE"
#define HTTP_METHOD_HEAD			"HEAD"
#define HTTP_METHOD_TRACE			"TRACE"
#define HTTP_METHOD_OPTIONS			"OPTIONS"
#define HTTP_METHOD_CONNECT			"CONNECT"

#define HTTP_WEB_SOCKET_SEC_SALT	"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"


enum EnAppState
{
	ST_STARTING, ST_STARTED, ST_CONNECTING, ST_CONNECTED, ST_STOPPING, ST_STOPPED
};


struct info_msg
{
	LPCTSTR name;
	CONNID connID;
	LPCTSTR evt;
	int contentLength;
	LPCTSTR content;

	static info_msg* Construct(CONNID dwConnID, LPCTSTR lpszEvent, int iContentLength = 0, LPCTSTR lpszContent = nullptr, LPCTSTR lpszName = nullptr){
		return new info_msg(dwConnID,lpszEvent,iContentLength,lpszContent,lpszName);
	}
	static void Destruct(info_msg* pMsg){
		delete pMsg;
	}

private:
	info_msg(CONNID dwConnID, LPCTSTR lpszEvent, int iContentLength = 0, LPCTSTR lpszContent = nullptr, LPCTSTR lpszName = nullptr)
	:connID(dwConnID),evt(lpszEvent),contentLength(iContentLength),content(lpszContent),name(nullptr){
		
	/*	TRACE("-----info_msg-----info_msg-\n");
		TRACE("-----info_msg---dwConnID=%d-----lpszEvent=%s-------iContentLength=%d-----lpszContent=%s----\n",dwConnID,lpszEvent,iContentLength,lpszContent);*/

		if (lpszName)
		{
			int len  = lstrlen(lpszName);
			if (len > 0)
			{
				name = new TCHAR[len+1];
				memcpy((LPSTR)name,lpszName,(len+1)*sizeof(TCHAR));
			}
		}
	}

	~info_msg(){
		if (name)
		{
			delete[] name;
		}

		if (contentLength> 0 )
		{
			delete[] content;
		}
	}
};


struct tpkgbody 
{
	char name[30];
	short age;
	char desc[1];
};

//struct TPkgInfo
//{
//	bool is_header;
//	int length;
//
//	TPkgInfo(bool header = true, int len = sizeof(TPkgHeader)) : is_header(header), length(len) {}
//	void Reset() {is_header = true, length = sizeof(TPkgHeader);}
//	~TPkgInfo() {}
//};


class CHelp
{

public:
	//单例设计模式
	static CHelp*  newInstance(){
		if (m_pHelp == nullptr)
		{
			m_pHelp = new CHelp();
			return m_pHelp;
		}
		return m_pHelp;
	}


	static void  destory(){
		delete m_pHelp;	
	}




	void PostOnMessageComplete(CONNID dwConnID, LPCTSTR lpszName /* = nullptr */){
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_MESSAGE_COMPLETE, 0, nullptr, lpszName);

		PostInfoMsg(msg);
	}


	void PostOnHandShake(CONNID dwConnID, LPCTSTR lpszName)
	{
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_HAND_SHAKE, 0, nullptr, lpszName);

		PostInfoMsg(msg);
	}

	void PostOnPrepareListen(LPCTSTR lpszAddress, USHORT usPort, LPCTSTR lpszName)
	{
		LPTSTR lpszContent = new TCHAR[100];
		wsprintf(lpszContent, _T("bind address: %s:%d"), lpszAddress, usPort);
		int content_len = lstrlen(lpszContent);
		info_msg* msg = info_msg::Construct(0, EVT_ON_PREPARE_LISTEN, content_len, lpszContent, lpszName);

		LogInfoMsg(msg);
	}

	void PostOnRequestLine(CONNID dwConnID, LPCSTR lpszMethod, USHORT usUrlFieldSet, LPCSTR lpszUrl, LPCTSTR lpszName)
	{
		USES_CONVERSION;

		int content_len		= (int)(strlen(lpszMethod) + strlen(lpszUrl) + 20);
		LPTSTR lpszContent	= new TCHAR[content_len];

		wsprintf(lpszContent, _T("[%s/0x%02X] : %s"), A2T(lpszMethod), usUrlFieldSet, A2T(lpszUrl));
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_REQUEST_LINE, content_len, lpszContent, lpszName);

		PostInfoMsg(msg);
	}

	void PostOnHeadersComplete(CONNID dwConnID, LPCSTR lpszSummary, LPCTSTR lpszName)
	{
		USES_CONVERSION;

		static LPCTSTR PREFIX = _T("* * * * * * * * * Summary * * * * * * * * *\r\n");
		static int PREFIX_LEN = lstrlen(PREFIX);

		//将Ansi码转成Unicode
		LPCTSTR lpszSummaryT = A2CT(lpszSummary);


		int content_len		= lstrlen(lpszSummaryT) + PREFIX_LEN + 1;
		LPTSTR lpszContent	= new TCHAR[content_len];


		memcpy(lpszContent, PREFIX, PREFIX_LEN * sizeof(TCHAR));
		memcpy(lpszContent + PREFIX_LEN, lpszSummaryT, (content_len - PREFIX_LEN) * sizeof(TCHAR));


		TRACE("--------------PostOnHeadersComplete-----------%s---------\n",lpszContent);
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_HEADERS_COMPLETE, content_len, lpszContent, lpszName);

		PostInfoMsg(msg);

	}

	//销毁消息
	void PostInfoMsg(info_msg* msg)
	{
		//TRACE("--------------------PostInfoMsg(msg)-------\n");
		if(	g_pMainWnd == nullptr								||
			g_pMainWnd->GetSafeHwnd() == nullptr				||
			!g_pMainWnd->PostMessage(USER_INFO_MSG, (WPARAM)msg	)){
				info_msg::Destruct(msg);
		}
	}


	void PostOnMessageBegin(CONNID dwConnID, LPCTSTR lpszName /* = nullptr */){
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_MESSAGE_BEGIN, 0, nullptr, lpszName);

		PostInfoMsg(msg);
	}

	void PostOnHeader(CONNID dwConnID, LPCSTR lpszHeaderName, LPCSTR lpszHeaderValue, LPCTSTR lpszName /* = nullptr */){
		USES_CONVERSION;
		int content_len = (int)(strlen(lpszHeaderName) + strlen(lpszHeaderValue) +10);
		LPTSTR lpszContent = new TCHAR[content_len];

		wsprintf(lpszContent,_T("%s: %s"),A2T(lpszHeaderName), A2T(lpszHeaderValue));
		info_msg* msg = info_msg::Construct(dwConnID,EVT_ON_HEADER,content_len,lpszContent,lpszName);

		PostInfoMsg(msg);
	}

	void PostOnStatusLine(CONNID dwConnID, USHORT usStatusCode, LPCSTR lpszDesc, LPCTSTR lpszName)
	{
		USES_CONVERSION;

		int content_len		= (int)(strlen(lpszDesc) + 10);
		LPTSTR lpszContent	= new TCHAR[content_len];

		wsprintf(lpszContent, _T("(%u) : %s"), usStatusCode, A2T(lpszDesc));
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_STATUS_LINE, content_len, lpszContent, lpszName);

		PostInfoMsg(msg);
	}

	void PostOnClose(CONNID dwConnID, LPCTSTR lpszName /* = nullptr */){
		info_msg* msg = info_msg::Construct(dwConnID,EVT_ON_CLOSE,0,nullptr,lpszName);
		PostInfoMsg(msg);
	}
	

	void PostOnError(CONNID dwConnID, int enOperation, int iErrorCode, LPCTSTR lpszName /* = nullptr */){
		LPTSTR lpszContent = new TCHAR[100];
		wsprintf(lpszContent,_T("OP: %d,CODE: %d"),enOperation,iErrorCode);

		int content_len = lstrlen(lpszContent);
		info_msg* msg = info_msg::Construct(dwConnID,EVT_ON_ERROR,content_len,lpszContent,lpszName);

		PostInfoMsg(msg);
	}
	

	void PostOnParseError(CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc, LPCTSTR lpszName /* = nullptr */){
		USER_INFO_MSG;

		int content_len = (int)(strlen(lpszErrorDesc)+10);
		LPTSTR lpszContent = new TCHAR[content_len];
	
		//这个地方可能有错误，得小心---转成unicode
		wsprintf(lpszContent,_T("(%i) : %s"),iErrorCode,  lpszErrorDesc );
		info_msg * msg = info_msg::Construct(dwConnID,EVT_ON_PARSE_ERROR,content_len,lpszContent,lpszName);

		PostInfoMsg(msg);
	}

	void PostOnBody(CONNID dwConnID, const BYTE* pData, int iLength, LPCTSTR lpszName /* = nullptr */){
		LPTSTR lpszContent = new TCHAR[20];
		wsprintf(lpszContent,_T("(%d bytes)"),iLength);
		int content_len = lstrlen(lpszContent);
		info_msg* msg = info_msg::Construct(dwConnID,EVT_ON_BODY,content_len,lpszContent,lpszName);
		
		PostInfoMsg(msg);

	}
	//服务器开始日志
	void LogServerStart(LPCTSTR lpszAddress, USHORT port, LPCTSTR lpszName)
	{
		CString msg;
		msg.Format(_T("$ %sServer Start OK --> (%s : %d)"), SafeString(lpszName), lpszAddress, port);

		LogMsg(msg);
	}

	CStringA& HttpVersionToString(EnHttpVersion enVersion,CStringA& strResult){
		strResult.Format("HTTP/%d.%d",LOBYTE(enVersion),HIBYTE(enVersion));
		return strResult;
	}


	void LogServerStop(LPCTSTR lpszName){
		CString msg;
		msg.Format(_T("$ %sServer Stop"), SafeString(lpszName));

		LogMsg(msg);
	}

	void LogDisconnect(CONNID dwConnID,LPCTSTR lpszName){
		CString msg;
		msg.Format(_T("$ %s(%Iu) Disconnect OK"),SafeString2(lpszName),dwConnID);

		LogMsg(msg);
	}

	//打印消息结构体中的内容
	void LogInfoMsg(info_msg* pInfoMsg){
		CString msg;

		if(pInfoMsg->name)
		{
			if(pInfoMsg->connID > 0)
			{
				if(pInfoMsg->contentLength > 0)
					msg.Format(_T("  > [ %s #%Iu, %s ] -> %s"), pInfoMsg->name, pInfoMsg->connID, pInfoMsg->evt, pInfoMsg->content);
				else
					msg.Format(_T("  > [ %s #%Iu, %s ]"), pInfoMsg->name, pInfoMsg->connID, pInfoMsg->evt);
			}
			else
			{
				if(pInfoMsg->contentLength > 0)
					msg.Format(_T("  > [ %s - %s ] -> %s"), pInfoMsg->name, pInfoMsg->evt, pInfoMsg->content);
				else
					msg.Format(_T("  > [ %s - %s ]"), pInfoMsg->name, pInfoMsg->evt);
			}
		}
		else
		{
			if(pInfoMsg->connID > 0)
			{
				if(pInfoMsg->contentLength > 0)
					msg.Format(_T("  > [ %Iu, %s ] -> %s"), pInfoMsg->connID, pInfoMsg->evt, pInfoMsg->content);
				else
					msg.Format(_T("  > [ %Iu, %s ]"), pInfoMsg->connID, pInfoMsg->evt);
			}
			else
			{
				if(pInfoMsg->contentLength > 0)
					msg.Format(_T("  > [ %s ] -> %s"), pInfoMsg->evt, pInfoMsg->content);
				else
					msg.Format(_T("  > [ %s ]"), pInfoMsg->evt);
			}
		}
		LogMsg(msg);
		info_msg::Destruct(pInfoMsg);
	}

	void LogDisconnectFail(CONNID dwConnID,LPCTSTR lpszName){
		CString msg;
		msg.Format(_T("$ %s(%Iu) Disconnect Fail"),SafeString2(lpszName),dwConnID);
		LogMsg(msg);
	}

	inline CString SafeString2(LPCTSTR lpszName){
		CString str(lpszName);
		if(lpszName) str.Append(_T(" #"));
		return str;
	}

	void PostOnSend(CONNID dwConnID, const BYTE* pData, int iLength, LPCTSTR lpszName /* = nullptr */){
		LPTSTR lpszContent = new TCHAR[20];
		wsprintf(lpszContent, _T("(%d bytes)"), iLength);
		int content_len = lstrlen(lpszContent);
		info_msg* msg = info_msg::Construct(dwConnID, EVT_ON_SEND, content_len, lpszContent, lpszName);
		TRACE("---Help------PostOnSend-----\n");
		PostInfoMsg(msg);
	}

	void LogMsg(const CString& msg)
	{
		USES_CONVERSION;
		if (!g_pInfoList || !g_pInfoList->GetSafeHwnd())
		{
			return;
		}
		int iStart = 0;
		while(true)
		{
			CString item = msg.Tokenize(_T("\r\n"), iStart);
			if (-1 == iStart)
			{
				break;
			}
			int countIndex = g_pInfoList->GetCount();
			g_pInfoList->InsertString(countIndex,item);
		}
		g_pInfoList->SetRedraw(TRUE);
		//再写下


		/*TRACE("-----CHelp----LogMsg----msg---%s\n",T2A(msg));
		if(!g_pInfoList || !g_pInfoList->GetSafeHwnd())
			return;

		g_pInfoList->SetRedraw(FALSE);

		int iCurIndex	= g_pInfoList->GetCurSel();

		BOOL bFirst		= TRUE;
		int iStart		= 0;
		int iAdd		= 0;

		while(true)
		{
			CString item = msg.Tokenize(_T("\r\n"), iStart);

			if(iStart == -1)
				break;

			if(bFirst)
				bFirst = FALSE;
			else
				item.Insert(0, _T("      | "));

			TRACE("--------LogMsg---------item= %s \n",T2A(item));

			g_pInfoList->AddString(item);
			++iAdd;
		}

		int iCount		= g_pInfoList->GetCount();
		BOOL bCurLast	= (iCurIndex == LB_ERR || iCurIndex + iAdd == iCount - 1);

		int iDec = 0;
		while(g_pInfoList->GetCount() > MAX_LOG_RECORD_LENGTH)
		{
			g_pInfoList->DeleteString(0);
			++iDec;
		}

		if(bCurLast)
		{
			iCurIndex = iCount - iDec - 1;
			g_pInfoList->SetCurSel(iCurIndex);
		}
		else if(iCurIndex < iDec)
		{
			iCurIndex = 0;
			g_pInfoList->SetCurSel(iCurIndex);
		}
		else
			iCurIndex = g_pInfoList->GetCurSel();

		g_pInfoList->SetAnchorIndex(iCurIndex);
		g_pInfoList->SetRedraw(TRUE);*/


	}
	

	void LogServerStartFail(DWORD code, LPCTSTR lpszDesc, LPCTSTR lpszName = nullptr){
		CString msg;
		msg.Format(_T("$ %sServer Start Fail ---> %s (%d)"),SafeString(lpszName),lpszDesc,code);
		LogMsg(msg);
	}

	inline CString SafeString(LPCTSTR lpszName){
		CString str(lpszName);
		if (lpszName)
		{
			str.AppendChar(' ');
		}
		return str;
	}

	//设置显示列表
	void setInfoList(CListBox* pListBox){
		this->g_pInfoList = pListBox;
	}

	//设置窗口
	void setMainWnd(CWnd* pWnd){
		this->g_pMainWnd = pWnd;
	}


private:
	static CHelp* m_pHelp;

public:
	~CHelp(void);

private:
	CHelp(void);
	CListBox* g_pInfoList;
	CWnd* g_pMainWnd;

	
};

