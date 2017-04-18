#include "StdAfx.h"
#include "Dispatcher.h"

CCameraProxy* CDispatcher::m_pCameraProxy = new CCameraProxy();

CDispatcher::CDispatcher(void)
{

}


CDispatcher::~CDispatcher(void)
{
	/*delete m_pCamera;*/
	delete m_pCameraProxy;
}
