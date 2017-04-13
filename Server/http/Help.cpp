#include "StdAfx.h"
#include "Help.h"

CHelp* CHelp::m_pHelp=nullptr;

CHelp::CHelp(void)
{
}


CHelp::~CHelp(void)
{
	if (m_pHelp!=nullptr)
	{
		delete m_pHelp;
	}
	//delete m_pHelp;
}
