#pragma once

///////////////////////////////////////////////////////////////////////////////
/*
Class		GLogGlobal
brief		GLog������Ʈ���� ���������� ���Ǵ� �κ� ���� Class
author		dhjin
date		2010-05-17 ~ 2010-05-17
warning		���� Class
*/
///////////////////////////////////////////////////////////////////////////////

class GLogGlobal
{
public:
	GLogGlobal(void);
	~GLogGlobal(void);

	CLogManager *	m_pGlobalLogManager;

public:
	// start 2011-12-12 by hskim, GLog 2��
	void SetConfigRootPath();
	CString m_strConfigRoot;
	// end 2011-12-12 by hskim, GLog 2��

private:
	void InitLogManager();

};

extern GLogGlobal *	g_pLogGlobal;