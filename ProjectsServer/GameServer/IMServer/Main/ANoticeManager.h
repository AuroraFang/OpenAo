// ANoticeManager.h: interface for the CANoticeManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANOTICEMANAGER_H__21294739_F7BC_48A1_A878_A8846B0840D7__INCLUDED_)
#define AFX_ANOTICEMANAGER_H__21294739_F7BC_48A1_A878_A8846B0840D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
/// \class		CANoticeManager
///
/// \brief		// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 
/// \author		cmkwon
/// \version	
/// \date		2009-01-19 ~ 2009-01-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CIMIOCP;
class CANoticeManager	// AdminNoticeManager
{
public:
	CANoticeManager(CIMIOCP *i_pIOCP);
	virtual ~CANoticeManager();


	void ResetANoticeManager(void);
	BOOL InitANoticeManager(SANoticeInfo *i_pNoticeInfo, mtvectSANoticeString *i_pmtvectNoticeStringList, BOOL i_bInstantlyStart=FALSE);
	BOOL IsValidAdminAutoNoticeNoLock(void);
	BOOL GetNoticeStringforSending(SANoticeString *o_pANoticeStr, ATUM_DATE_TIME *i_pDateTime);

	void OnDoSecondlyWorkCANoticeManager(ATUM_DATE_TIME *i_pDateTime);
protected:
	SANoticeInfo			m_ANoticeInfo;				
	mtvectSANoticeString	m_mtvectANoticeStringList;	
	int						m_nSizemtvectANoticeStringList;
	int						m_nNextNoticeStringVectIndex;		// m_mtvectANoticeStringList ������ �ε���
	ATUM_DATE_TIME			m_atLastSentTime;					// ������ ���� ���۵� �ð�.

	CIMIOCP *				m_pIMIOCP6;
};

#endif // !defined(AFX_ANOTICEMANAGER_H__21294739_F7BC_48A1_A878_A8846B0840D7__INCLUDED_)
