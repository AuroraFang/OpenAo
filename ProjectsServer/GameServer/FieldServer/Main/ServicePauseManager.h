// ServicePauseManager.h: interface for the CServicePauseManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ATUM_SERVICE_PAUSE_MANAGER_H_
#define _ATUM_SERVICE_PAUSE_MANAGER_H_

class CServicePauseManager  
{
public:
	CServicePauseManager();

	void SetQuestServiceState(BOOL i_bQuestService);	// ����Ʈ ���� ������
	BOOL GetQuestServiceState();						// ����Ʈ ���� ������

private:
	BOOL m_bQuestService; // ����Ʈ ���� ������
};

#endif // _ATUM_SERVICE_PAUSE_MANAGER_H_
