#pragma once
#include "socketinterface.h"
#include "Help.h"
#include "bufferptr.h"
//#include "helper.h"
#include "../Dispatcher.h"

//使用help，而不是用helper.h，是因为我想全部使用纯面向对象

class CHttpServerListenerImpl :
	public CHttpServerListener
{
public:
	CHttpServerListenerImpl(void);
	CHttpServerListenerImpl(LPCTSTR lpszName):m_strName(lpszName){
			m_pHelp = CHelp::newInstance();
	}
	~CHttpServerListenerImpl(void);
private:
	CHelp* m_pHelp;

public:

	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength){
		TRACE("-------CHttpServerListenerImpl--------OnSend------------------\n");
		//m_pHelp->PostOnSend(dwConnID,pData,iLength,m_strName);
		return HR_OK;
	}

	virtual EnHandleResult  OnHandShake(ITcpServer* pSender, CONNID dwConnID){
		TRACE("-------CHttpServerListenerImpl--------OnHandShake------------------\n");
		//m_pHelp->PostOnHandShake(dwConnID, m_strName);
		return HR_OK;
	}

	virtual EnHttpParseResult OnHeader(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszName, LPCSTR lpszValue){
		TRACE("-------CHttpServerListenerImpl--------OnHeader------------------\n");
		//m_pHelp->PostOnHeader(dwConnID, lpszName, lpszValue, m_strName);
		

		return HPR_OK;
	}

	virtual EnHttpParseResult OnMessageBegin(IHttpServer* pSender, CONNID dwConnID){
		TRACE("-------CHttpServerListenerImpl--------OnMessageBegin------------------\n");
		//m_pHelp->PostOnMessageBegin(dwConnID, m_strName);

		return HPR_OK;
	}

	virtual EnHttpParseResult OnRequestLine(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszMethod, LPCSTR lpszUrl){
		TRACE("-------CHttpServerListenerImpl--------OnRequestLine------------------\n");

	//	m_pHelp->PostOnRequestLine(dwConnID, lpszMethod, pSender->GetUrlFieldSet(dwConnID), lpszUrl, m_strName);

		return HPR_OK;
	}


	//OnAccept()
	virtual	EnHttpParseResult OnHeadersComplete(IHttpServer* pSender, CONNID dwConnID){
		TRACE("-------CHttpServerListenerImpl--------OnHeadersComplete------------------\n");
		//CStringA strSummary = GetHeaderSummary(pSender,dwConnID,"  ",0,TRUE);

	//	TRACE("Http head = %s\n",strSummary);
	
		//TRACE("---------CHttpServerListenerImpl--------OnHeadersComplete-----%s--------\n",m_strName);

	//	m_pHelp->PostOnHeadersComplete(dwConnID,strSummary,m_strName);

	
		return HPR_OK;
	}


	virtual EnHttpParseResult OnBody(IHttpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength){
		TRACE("-------CHttpServerListenerImpl--------OnBody------------------\n");

		CDispatcher::DispatchCommand(pSender,dwConnID,pData,iLength);

		//就是在onBody中解析post请求的消息体
		//m_pHelp->PostOnBody(dwConnID,pData,iLength,m_strName);
		/*CStringA str;
		CStringA s;
		for(int i = 0;i<iLength;i++){
			str.Format("%c",pData[i]);
			s+=str;
		}*/

		//TRACE(s);



		return HPR_OK;
	}

	/*
	* 名称：完成解析通知
	* 描述：每当解析完成一个完整 HTTP 报文，向监听器发送该通知
	*		
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	* 返回值：	HPR_OK		-- 继续执行
	*			HPR_ERROR	-- 引发 OnParserError() 和 OnClose() 事件并关闭连接
	*/
	virtual EnHttpParseResult OnMessageComplete(IHttpServer* pSender, CONNID dwConnID){
		//pSender->Send(dwConnID,)
		TRACE("------CHttpServerListenerImpl----- OnMessageComplete------\n");

	
		//LPWSABUF pHead = new WSABUF();
		//CStringA strATemp = "image";
		//pHead->buf = new char[strATemp.GetLength()+1];
		//sprintf(pHead->buf,"%s",strATemp);

		//CStringA fileName = "e:/temp.jpg";
		////fileName.
		//pHead->len = GetFileSize(fileName.GetBuffer());
		//TRACE("------------len = %ul---\n",pHead->len);


		////SendSmallFile(CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr)
		////pSender->SendSmallFile(dwConnID,_T("e:/temp.jpg"),pHead);
		////这个地方有问题----
		////	pSender->SendSmallFile(dwConnID,_T("e:/temp.jpg"));


		//delete[] pHead->buf;
		//delete pHead;


		//m_pHelp->PostOnMessageComplete(dwConnID,m_strName);

		//if (pSender->IsUpgrade(dwConnID))
		//{
		//	return HPR_ERROR;
		//}
		//
		//LPCSTR contentType = pSender->GetContentType(dwConnID);
		//TRACE("---contentType=-%s-----\n",contentType);
		//ULONG  iContentLength= pSender->GetContentLength(dwConnID);
		//TRACE("--iContentLength=%d----\n",iContentLength);

		///*char* pData = new char[iContentLength+1];
		//memset(pData,0,sizeof(pData));
		//PVOID pVoid = (PVOID)pData;
		//pSender->GetConnectionExtra(dwConnID,&pVoid);*/

		///*CStringA str;
		//CStringA s;
		//for(int i = 0;i<iLength;i++){
		//str.Format("%c",pData[i]);
		//s += str;
		//}
		//s += "\n";
		//TRACE(s);*/
		////TRACE("-----content=%s---",pData);

		////delete[] pData;


		////将头信息直接返回
		//CStringA strBody = GetHeaderSummary(pSender,dwConnID,"    ",0,FALSE);
		//TRACE("--------------------strBody2 == %s\n",strBody);
		//int iBodyLength = strBody.GetLength();
		//BOOL bSkipBody = FALSE;


		//TRACE("----------pSender->GetMethod-------\n");
		//if (strcmp(pSender->GetMethod(dwConnID),HTTP_METHOD_HEAD) == 0)
		//{
		//	bSkipBody  = TRUE;
		//}

		////内容长度
		//CStringA strContentLength;
		//strContentLength.Format("%u",iBodyLength);

		//DWORD dwSeq = 1;
		//LPCSTR lpszReqSequence = nullptr;

		////获得cookie
		//if(pSender->GetCookie(dwConnID,"__reqSequence",&lpszReqSequence)){
		//	dwSeq += atoi(lpszReqSequence);
		//}

		//CStringA strSeqCookie;
		//strSeqCookie.Format("__reqSequence=%u; path=/",dwSeq);

		//THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength},{"Set-Cookie",strSeqCookie}};
		//int iHeaderCount = sizeof(header)/sizeof(header);

		//if (bSkipBody)
		//{
		//	strBody.Empty();
		//	iBodyLength = 0;
		//}


		////CStringA strBody;
		////strBody.Format("i am qjg ,fist connected----erewfe");

		////CStringA strContentLength;
		////strContentLength.Format("%u",strBody.GetLength());
		////
		////DWORD dwSeq = 1;
		////LPCSTR lpszReqSequence = nullptr;
		//////获得cookie
		////if(pSender->GetCookie(dwConnID,"__reqSequence",&lpszReqSequence)){
		////	dwSeq += atoi(lpszReqSequence);
		////}
		////CStringA strSeqCookie;
		////strSeqCookie.Format("__reqSequence=%u; path=/",dwSeq);
		////THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength},{"Set-Cookie",strSeqCookie}};
		////int iHeaderCount = sizeof(header)/sizeof(header);

		
		//TRACE("----------pSender->SendResponse-------\n");
		////将消息返回
		//pSender->SendResponse(dwConnID,
		//	HSC_OK,
		//	"HP Http Server OK",
		//	header,iHeaderCount,
		//	(const BYTE*)(LPCSTR)strBody,
		//	strBody.GetLength());
	

		////如果发送后，pSender为null,就释放资源
		//if (!pSender->IsKeepAlive(dwConnID))
		//{
		//	pSender->Release(dwConnID);
		//}

		//TRACE("----------return HPR_OK;-------\n");
	


////发送文件
//		CFile file(_T("e:/temp.jpg"),CFile::modeRead);
//		ULONGLONG length = file.GetLength();
//		BYTE* pData = new BYTE[length];
//		file.Read(pData,length);
//		CStringA strContentLength;
//		strContentLength.Format("%u",length);
//		THeader header[]  = {{"Content-Type","image"},{"Content-Length",strContentLength}};
//		int iHeaderCount = sizeof(header)/sizeof(THeader);
//		pSender->SendResponse(dwConnID,HSC_OK,"HP Http Server OK",header,iHeaderCount,pData,length);
//
//		
//		if (!pSender->IsKeepAlive(dwConnID))
//		{
//			pSender->Release(dwConnID);
//		}
//
//		delete[] pData;

		return HPR_OK;
	}	
	
	/*ULONG	GetFileSize(char* fileName){
	long size;
	FILE* fp;
	fp = fopen(fileName,"rb");
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fclose(fp);
	return size;
	}

	void getFileBytes(char* fileName )*/


	/*
	* 名称：解析错误通知
	* 描述：当解析 HTTP 报文错误时，向监听器发送该通知
	*		
	* 参数：		pSender			-- 事件源对象
	*			dwConnID		-- 连接 ID
	*			iErrorCode		-- 错误代码
	*			lpszErrorDesc	-- 错误描述
	* 返回值：	HPR_OK			-- 继续执行
	*			HPR_ERROR		-- 引发 OnClose() 事件并关闭连接
	*/
	virtual EnHttpParseResult OnParseError(IHttpServer* pSender, CONNID dwConnID, int iErrorCode, LPCSTR lpszErrorDesc){
		TRACE("-----CHttpServerListenerImpl------OnParseError---- \n");
		m_pHelp->PostOnParseError(dwConnID, iErrorCode, lpszErrorDesc, m_strName);
		return HPR_OK;

	}

	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen){
		TRACE("-------CHttpServerListenerImpl--------OnPrepareListen------------------\n");
		TCHAR szAddress[40];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		//设置监听地址
		pSender->GetListenAddress(szAddress, iAddressLen, usPort);
		m_pHelp->PostOnPrepareListen(szAddress, usPort, m_strName);

		return HR_OK;
	}



	/*
	* 名称：通信错误通知
	* 描述：通信发生错误后，Socket 监听器将收到该通知，并关闭连接
	*		
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			enOperation	-- Socket 操作类型
	*			iErrorCode	-- 错误代码
	* 返回值：	忽略返回值
	*/
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode){
		TRACE("-----CHttpServerListenerImpl------ OnClose------------- \n");

		iErrorCode  == SE_OK? m_pHelp->PostOnClose(dwConnID,m_strName)  :
			m_pHelp->PostOnError(dwConnID,enOperation,iErrorCode,m_strName);
			//获取额外的连接
		CBufferPtr* pBuffer = nullptr;
		CByteBufferPtr CBufferPtr;
		pSender->GetConnectionExtra(dwConnID,(PVOID*)&pBuffer);
		if (pBuffer)
		{
			delete pBuffer;
		}
		//删除连接的数据
		return HR_OK;
	}


	CStringA GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep, int iSepCount, bool bWithContentLength);
	CString m_strName;
};

