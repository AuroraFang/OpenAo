#ifndef _CHATTAB_T_H_
#define _CHATTAB_T_H_

#include "AtumParam.h"

#define CHAT_BUFFER_NUMBER					200							// ä�� �ִ� ���� ���ۼ�


typedef struct ChatTab
{
	char *		pszChatString;											// �Ѷ����� ä�� ������ ����
	DWORD		dwColorType;											// �̶����� Color
	DWORD		dwType;													// ä���� ����(��ü, ����, �Ʒ���, ��Ƽ��)
	ChatTab()
	{
		pszChatString = NULL;
		dwColorType = 0;
		dwType = 0;
	}
} ChatLine_t;

class ChatTab_t
{
public:
	ChatTab_t(): m_nStoredLineCounts(0), m_nStoreStartIndex(0), m_nStoreEndIndex(0), m_nRenderStartIndex(0)
	{
	}       
	virtual ~ChatTab_t(); 

	BOOL IsValidChatLineIndex(int i_nIdx);
	void PushChatLine(const char *i_szChatLine, DWORD i_dwColorType, DWORD i_dwType = 0);
	int ChatLineLength(int i_nIdx);
	char* GetChatLine(int i_nIdx); 
	DWORD GetColorType(int i_nIdx);
	DWORD GetType(int i_nIdx);
	int IncreaseRenderStartIndex(void);
	int DecreaseRenderStartIndex(void);
	int SetRenderStartIndexWithScrollPositon(int i_nScrMoveRange, int i_nCurScrPosition);
	int GetScrollPositionWithRenderStartIndex(int i_nScrMoveRange, int i_nRanderLine);

	// 2008-02-27 by bhsohn �Ʒ��� �̵��� ä������ ����
	void GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER]);
	

	ChatLine_t		m_ArrChatLine[CHAT_BUFFER_NUMBER];
	int				m_nStoredLineCounts;								// ����� ä���� ���μ�
	int				m_nStoreStartIndex;									// ����� ä���� ���� �ε���
	int				m_nStoreEndIndex;									// ����� ä���� ������ �ε���
	int				m_nRenderStartIndex;								// ä�� â�� Render�ϱ� ���� ���� �ε���
};



#endif// _CHATTAB_T_H_endif