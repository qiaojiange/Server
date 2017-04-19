#include "StdAfx.h"
#include "Dispatcher.h"


CDispatcher*  CDispatcher::m_pDispatcher = NULL;

CDispatcher::CDispatcher(void)
{
	TRACE("---CDispatcher::CDispatcher(void)-----\n");
	m_pCameraProxy = new CCameraProxy();	
}


CDispatcher::~CDispatcher(void)
{
	/*delete m_pCamera;*/
	TRACE("-------CDispatcher::~CDispatcher(void)-----\n");
	
	delete m_pCameraProxy;
	m_pDispatcher = NULL;
}

void CDispatcher::DispatchCommand(IHttpServer* pSend, CONNID dwConnID, const BYTE* pData, int iLength)
{
	CStringA strData;
	ByteToChar(pData,iLength,strData);
	TRACE("--CDispatcher--%s--\n",strData.GetBuffer(0));

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strData.GetBuffer(0),root))
	{
		TRACE("----parse ----true\n");
		std::string device = root["deviceName"].asString();
		if (0==strcmp(device.c_str(),"camera"))
		{
			m_pCameraProxy->handJsonData(pSend,dwConnID,strData);
		}else if(0==strcmp(device.c_str(),"lctf")){

		}else{

		}


	}
}

void CDispatcher::ByteToChar(const BYTE* pData,int iLength,CStringA& strA)
{
	CStringA strTemp;
	for (int i =0;i<iLength;i++)
	{
		strTemp.Format("%c",pData[i]);
		strA += strTemp;
	}
}
