// CommuPartyOp.h: interface for the CINFCommuPartyOp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUPARTYOP_H__77B79AB8_C2DD_4782_9683_39D57698D4D3__INCLUDED_)
#define AFX_COMMUPARTYOP_H__77B79AB8_C2DD_4782_9683_39D57698D4D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
class CINFListBox;

class CINFCommuPartyOp  : public CINFBase
{
public:
	CINFCommuPartyOp(CAtumNode* pParent);
	virtual ~CINFCommuPartyOp();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	// ���� ���� ��ų ����
	UID32_t GetPartyBanCharacterUniqueNumber();
	void SetPartyBanCharacterUniqueNumber(UID32_t nPartyBanCharacterUniqueNumber);

	// ������ ����
	UID32_t GetPartyTransferMasterCharacterUniqueNumber();
	void SetPartyTransferMasterCharacterUniqueNumber(UID32_t nPartyTransferMasterCharacterUniqueNumber);
	
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	void RenderPartyInfo();
	DWORD GetParyInfoColor(int nIdx, PARTY_TYPE PartyType, char* pUserName);

	void PartyQuestionBanUser();		//�߹��� �����.
	void PartyQuestionTransferMaster();	// ����

	void RefreshPartyInfo();
	void ChagePartyFormation(int nFormation);// ��� ���� ����

	void InitComboInfo();	// �޺� ��ư �ʱ�ȭ

private:
	CINFImageEx*	m_pFormat;	

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;

	CINFImageBtn*			m_pJoinBtn;			// �ʴ�
	CINFImageBtn*			m_pLeaveBtn;			// Ż��
	CINFImageBtn*			m_pPurgeBtn;			// �߹�
	CINFImageBtn*			m_pChargeBtn;			// ����

	CINFImageBtn*			m_pCloseBtn;			// �ݱ�
	CINFImageBtn*			m_pOkBtn;				// Ȯ��

	CD3DHanFont*			m_pPartyList;			// ��Ƽ�� ���

	CINFImageEx*				m_pSelImage;			// ��Ƽ�������̹���

	int						m_nPartySel;

	UID32_t					m_nPartyBanCharacterUniqueNumber;				// �߹��� ����� ���� �ѹ�
	UID32_t					m_nPartyTransferMasterCharacterUniqueNumber;	// ������ �ɸ���

	PARTY_TYPE				m_bPartyTypeBtn;
	PARTYINFO				m_partyInfoTmp;

	CINFListBox *			m_pComboForMation;

};

#endif // !defined(AFX_COMMUPARTYOP_H__77B79AB8_C2DD_4782_9683_39D57698D4D3__INCLUDED_)
