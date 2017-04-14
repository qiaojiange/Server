#pragma once
#include "http/HPSocket.h"
#include "json/json.h"

class CDispatcher
{
public:
	CDispatcher(void);
	~CDispatcher(void);

	static void DispatchCommand( CONNID dwConnID, const BYTE* pData, int iLength)
	{
		CStringA strData;
		ByteToChar(pData,iLength,strData);
		TRACE("--CDispatcher--%s--\n",strData.GetBuffer(0));

		Json::Reader reader;
		Json::Value root;
		if (reader.parse(strData.GetBuffer(0),root))
		{
			TRACE("----parse ----true\n");
			std::string device = root["device"].asString();
			/*TRACE(device)
			TRACE("\n");*/

		}


	}

	
private:
	static void ByteToChar(const BYTE* pData,int iLength,CStringA& strA){
		CStringA strTemp;
		for (int i =0;i<iLength;i++)
		{
			strTemp.Format("%c",pData[i]);
			strA += strTemp;
		}
	}

};

