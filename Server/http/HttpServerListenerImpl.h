#pragma once
#include "socketinterface.h"
#include "Help.h"
#include "bufferptr.h"
//#include "helper.h"
#include "../Dispatcher.h"

//ʹ��help����������helper.h������Ϊ����ȫ��ʹ�ô��������

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

		//������onBody�н���post�������Ϣ��
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
	* ���ƣ���ɽ���֪ͨ
	* ������ÿ���������һ������ HTTP ���ģ�����������͸�֪ͨ
	*		
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	* ����ֵ��	HPR_OK		-- ����ִ��
	*			HPR_ERROR	-- ���� OnParserError() �� OnClose() �¼����ر�����
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
		////����ط�������----
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


		////��ͷ��Ϣֱ�ӷ���
		//CStringA strBody = GetHeaderSummary(pSender,dwConnID,"    ",0,FALSE);
		//TRACE("--------------------strBody2 == %s\n",strBody);
		//int iBodyLength = strBody.GetLength();
		//BOOL bSkipBody = FALSE;


		//TRACE("----------pSender->GetMethod-------\n");
		//if (strcmp(pSender->GetMethod(dwConnID),HTTP_METHOD_HEAD) == 0)
		//{
		//	bSkipBody  = TRUE;
		//}

		////���ݳ���
		//CStringA strContentLength;
		//strContentLength.Format("%u",iBodyLength);

		//DWORD dwSeq = 1;
		//LPCSTR lpszReqSequence = nullptr;

		////���cookie
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
		//////���cookie
		////if(pSender->GetCookie(dwConnID,"__reqSequence",&lpszReqSequence)){
		////	dwSeq += atoi(lpszReqSequence);
		////}
		////CStringA strSeqCookie;
		////strSeqCookie.Format("__reqSequence=%u; path=/",dwSeq);
		////THeader header[] = { {"Content-Type","text/plain"},{"Content-Length",strContentLength},{"Set-Cookie",strSeqCookie}};
		////int iHeaderCount = sizeof(header)/sizeof(header);

		
		//TRACE("----------pSender->SendResponse-------\n");
		////����Ϣ����
		//pSender->SendResponse(dwConnID,
		//	HSC_OK,
		//	"HP Http Server OK",
		//	header,iHeaderCount,
		//	(const BYTE*)(LPCSTR)strBody,
		//	strBody.GetLength());
	

		////������ͺ�pSenderΪnull,���ͷ���Դ
		//if (!pSender->IsKeepAlive(dwConnID))
		//{
		//	pSender->Release(dwConnID);
		//}

		//TRACE("----------return HPR_OK;-------\n");
	


////�����ļ�
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
	* ���ƣ���������֪ͨ
	* ������������ HTTP ���Ĵ���ʱ������������͸�֪ͨ
	*		
	* ������		pSender			-- �¼�Դ����
	*			dwConnID		-- ���� ID
	*			iErrorCode		-- �������
	*			lpszErrorDesc	-- ��������
	* ����ֵ��	HPR_OK			-- ����ִ��
	*			HPR_ERROR		-- ���� OnClose() �¼����ر�����
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

		//���ü�����ַ
		pSender->GetListenAddress(szAddress, iAddressLen, usPort);
		m_pHelp->PostOnPrepareListen(szAddress, usPort, m_strName);

		return HR_OK;
	}



	/*
	* ���ƣ�ͨ�Ŵ���֪ͨ
	* ������ͨ�ŷ��������Socket ���������յ���֪ͨ�����ر�����
	*		
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			enOperation	-- Socket ��������
	*			iErrorCode	-- �������
	* ����ֵ��	���Է���ֵ
	*/
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode){
		TRACE("-----CHttpServerListenerImpl------ OnClose------------- \n");

		iErrorCode  == SE_OK? m_pHelp->PostOnClose(dwConnID,m_strName)  :
			m_pHelp->PostOnError(dwConnID,enOperation,iErrorCode,m_strName);
			//��ȡ���������
		CBufferPtr* pBuffer = nullptr;
		CByteBufferPtr CBufferPtr;
		pSender->GetConnectionExtra(dwConnID,(PVOID*)&pBuffer);
		if (pBuffer)
		{
			delete pBuffer;
		}
		//ɾ�����ӵ�����
		return HR_OK;
	}


	CStringA GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep, int iSepCount, bool bWithContentLength);
	CString m_strName;
};

