#pragma once
#include "http/HPSocket.h"

class CDispatcher
{
public:
	CDispatcher(void);
	~CDispatcher(void);

	static void DispatchCommend( CONNID dwConnID, const BYTE* pData, int iLength)
	{
		CStringA strData;
		ByteToChar(pData,iLength,strData);


#ifdef SHOW_TRACE
	//	TRACE(_T(strData));
#endif


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

