
// DMBSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDMBSystemApp: 
// �йش����ʵ�֣������ DMBSystem.cpp
//

class CDMBSystemApp : public CWinApp
{
public:
	CDMBSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDMBSystemApp theApp;