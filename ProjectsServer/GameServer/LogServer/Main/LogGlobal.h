#pragma once

#ifndef LOGGLOBAL_H
#define LOGGLOBAL_H

class CLogGlobal;
extern CLogGlobal *					g_pLogGlobal;

class CLogGlobal : public CGlobalGameServer
{
public:
	CLogGlobal();
	~CLogGlobal();

	void VMemAlloc() override;

	bool InitServerSocket() override;				// ���ϰ��� �ʱ�ȭ �Լ�
	bool EndServerSocket() override;					// ���ϰ��� ���� �Լ�

	virtual BOOL LoadConfiguration(void);				// Server ������ ���� Configuration Load
	
	// 2010-06-01 by shcho, GLogDB ���� -
	char *	GetGamePublisherName(void);
	char	m_szGamePublisherName[SIZE_MAX_GAME_PUBLISHER_NAME];			// 2010-06-01 by shcho, GLogDB ���� -

	BOOL	m_bArenaFieldServerCheck;										// 2011-12-12 by hskim, GLog 2��
	BOOL	IsArenaServer(void)	{ return m_bArenaFieldServerCheck; }	// 2011-12-12 by hskim, GLog 2��
};

#endif
