#pragma once

class CGLogDataBase;
typedef vector<CGLogDataBase*>				GLogDataList;
typedef vector<CGLogDataBase*>::iterator	GLogDataListIterater;

///////////////////////////////////////////////////////////////////////////////
/*
Class		CGLogManager
brief		GLog ���� Ŭ����
author		dhjin
date		2010-05-17 ~ 2010-05-17
warning	
*/
///////////////////////////////////////////////////////////////////////////////

class CGLogDataAccount;


class CGLogManager
{
public:
	CGLogManager(void);
	~CGLogManager(void);

private:
	CGLogSourceDB * m_pGLogSourceDB;		// GLog�� �Էµ� ���� DB
	CGLogDestDB	*	m_pGLogDestDB;			// GLog DB

	GLogDataList	m_DataList;				// GLog�� �Էµ� �� ���̺� ����Ÿ�� ���� ����Ʈ

	DWORD	m_dwUpdatedTime;				// GLog DB�� ������Ʈ�� �ð�

	bool	m_bIsSourceDBCnn;
	bool	m_bIsDestDBCnn;

	CString	m_strSourceDBID;
	CString m_strSourceDBPW;
	CString m_strSourceDBIP;
	CString m_strSourceDBPort;

public:
	bool GLogDBCnn();						// DB���� ó��
	void GLogDBDisCnn();					// DB���� ���� ó��
	void GLogProcessing();					// DB������Ʈ ���� ó��
		
private:
	void Init();
	bool CheckUpdateTime();					// DB������Ʈ ó�� �ð� üũ GLOG_UPDATE_TIME_MSEC �ð� ��ŭ ���� �ش�

	bool GLogSourceDBCnn();
	bool GLogDestDBCnn();

	// For ACEOnline
	bool GetConfigFile();					// GLog�� �Էµ� ���� DB�� ����(ID, PW, IP, Port) global.cfg���Ͽ��� ������ 

private:
	// start 2011-12-12 by hskim, GLog 2��
	BOOL InitLockProcess();
	HANDLE	m_hMutexMonoInstance;		
	// end 2011-12-12 by hskim, GLog 2��
};
