#include "StdAfx.h"
#include "CameraProxy.h"



CCameraProxy::CCameraProxy(void)
{
	m_pCamera= CCamera::getInstance();
}



CCameraProxy::~CCameraProxy(void)
{
	//delete m_pCamera;
}
