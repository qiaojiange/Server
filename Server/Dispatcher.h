#pragma once
#include "http/HPSocket.h"
#include "json/json.h"
#include "CameraProxy.h"


class CDispatcher
{
private:
	CDispatcher(void);
public:
	~CDispatcher(void);

	void DispatchCommand(IHttpServer* pSend, CONNID dwConnID, const BYTE* pData, int iLength);

	static CDispatcher* newInstance(){
		if (m_pDispatcher == NULL)
		{
			m_pDispatcher = new CDispatcher();
		}
		return m_pDispatcher;
	}

private:
	void ByteToChar(const BYTE* pData,int iLength,CStringA& strA);

	 CCameraProxy* m_pCameraProxy;
static CDispatcher* m_pDispatcher;
};

