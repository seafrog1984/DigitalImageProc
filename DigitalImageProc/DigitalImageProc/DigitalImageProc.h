
// DigitalImageProc.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDigitalImageProcApp: 
// �йش����ʵ�֣������ DigitalImageProc.cpp
//

class CDigitalImageProcApp : public CWinApp
{
public:
	CDigitalImageProcApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDigitalImageProcApp theApp;