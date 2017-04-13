#include "StdAfx.h"
#include "HttpServerListenerImpl.h"
#include <memory>
#include "Help.h"

CHttpServerListenerImpl::CHttpServerListenerImpl(void)
{
	m_pHelp = CHelp::newInstance();
}


CHttpServerListenerImpl::~CHttpServerListenerImpl(void)
{

}


CStringA CHttpServerListenerImpl::GetHeaderSummary(IHttpServer* pSender, CONNID dwConnID, LPCSTR lpszSep, int iSepCount, bool bWithContentLength)
{
	//TRACE("-----CHttpServerListenerImpl------CHttpServerListenerImpl::GetHeaderSummary \n");
	CStringA SEP1;
	for(int i = 0;i< iSepCount;i++){
		SEP1 += lpszSep;
	}

	CStringA SEP2(SEP1);
	SEP2 += lpszSep;

	CStringA strResult;

	//获取url字段集
	USHORT usUrlFieldSet = pSender->GetUrlFieldSet(dwConnID);

	strResult.AppendFormat("%s[URL Fields]%s",SEP1,CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"SCHEMA",pSender->GetUrlField(dwConnID,HUF_SCHEMA),CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"HOST",pSender->GetUrlField(dwConnID,HUF_HOST),CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"PORT",pSender->GetUrlField(dwConnID,HUF_PORT),CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"PATH ",pSender->GetUrlField(dwConnID,HUF_PATH),CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"QUERY",pSender->GetUrlField(dwConnID,HUF_QUERY),CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"FRAGMENT",pSender->GetUrlField(dwConnID,HUF_FRAGMENT),CRLF);
	strResult.AppendFormat("%s%8s: %s%s",SEP2,"USERINFO",pSender->GetUrlField(dwConnID,HUF_USERINFO),CRLF);

	//http协议头
	DWORD dwHeaderCount = 0;
	pSender->GetAllHeaders(dwConnID,nullptr,dwHeaderCount);

	strResult.AppendFormat("%s[Request Headers]%s",SEP1,CRLF);

	if (dwHeaderCount == 0)
	{
		strResult.AppendFormat("%s(no header)%s",SEP2,CRLF);
	}else{
		std::unique_ptr<THeader[]> headers(new THeader[dwHeaderCount]);
		VERIFY(pSender->GetAllHeaders(dwConnID,headers.get(),dwHeaderCount));

		for (DWORD i = 0;i< dwHeaderCount;i++)
		{
			strResult.AppendFormat("%s%s: %s%s",SEP2,headers[i].name,headers[i].value,CRLF);
		}
	}

	//打印cookie
	DWORD dwCookieCount = 0;
	pSender->GetAllCookies(dwConnID,nullptr,dwCookieCount);

	strResult.AppendFormat("%s[Cookies]%s", SEP1, CRLF);
	if (dwCookieCount == 0)
	{
		strResult.AppendFormat("%s(no cookie)%s", SEP2, CRLF);
	}else{

		std::unique_ptr<TCookie[]> cookies(new TCookie[dwCookieCount]);
		VERIFY( pSender->GetAllCookies(dwConnID,cookies.get(),dwCookieCount) );
		for (DWORD i = 0;i< dwCookieCount;i++)
		{
			strResult.AppendFormat("%s%s: %s%s",SEP2,cookies[i].name,cookies[i].value,CRLF);
		}

	}

	//打印版本号
	CStringA strVersion;
	m_pHelp->HttpVersionToString((EnHttpVersion)pSender->GetVersion(dwConnID), strVersion);

	//是否更新
	EnHttpUpgradeType enUpdataType = pSender->GetUpgradeType(dwConnID);
	LPCSTR lpszUpgrade             = enUpdataType != HUT_NONE ? "true" : "false";
	LPCSTR lpszKeepAlive           = pSender->IsKeepAlive(dwConnID) ? "true" : "false";


	strResult.AppendFormat("%s[Basic Info]%s",SEP1,CRLF);
	strResult.AppendFormat("%s%13s: %s%s",SEP2,"Version",strVersion,CRLF);
	strResult.AppendFormat("%s%13s: %s%s",SEP2,"Method",pSender->GetMethod(dwConnID),CRLF);
	strResult.AppendFormat("%s%13s: %s%s",SEP2,"IsUpgrade",lpszUpgrade,CRLF);

	//更新类型
	if(enUpdataType != HUT_NONE){
		strResult.AppendFormat("%s%13s: %d%s",SEP2,"UpgradeType",enUpdataType,CRLF);
	}

	strResult.AppendFormat("%s%13s: %s%s",SEP2,"IsKeepAlive",lpszKeepAlive,CRLF);

	//内容长度
	if (bWithContentLength)
	{
		strResult.AppendFormat("%s%13s: %lld%s",SEP2,"ContentLength",pSender->GetContentLength(dwConnID),CRLF);
	}

	//内容类型
	strResult.AppendFormat("%s%13s: %s%s",SEP2,"ContentType",pSender->GetContentType(dwConnID),CRLF);

	/*PVOID* 
	pSender->GetConnectionExtra(dwConnID,)*/
	return strResult;
}
