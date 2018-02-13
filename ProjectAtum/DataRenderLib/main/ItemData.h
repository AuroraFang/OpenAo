// ItemData.h: interface for the CItemData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDATA_H__42443EB1_ABF9_469B_929A_53D8BC2F14A0__INCLUDED_)
#define AFX_ITEMDATA_H__42443EB1_ABF9_469B_929A_53D8BC2F14A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CAtumData;
class CChatMoveData;
class CCharacterInfo;
class CItemData : public CAtumNode  
{
public:
	CItemData(MSG_FC_ITEM_SHOW_ITEM* pMsg);
// 2007-06-21 by dgwoo ü�� ������ �Ʒ��� ���� ����.
//	CItemData(CAtumNode * pParent,MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg);
	CItemData::CItemData(CAtumNode * pParent,INT nItemNum, UINT nFieldIndex);
	
	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	//CItemData(CAtumNode * pParent, float fCheckTime);
	CItemData(CAtumNode * pParent, float fCheckTime, UINT uObjId, DWORD dwPartType, 
							D3DXVECTOR3	vVel, D3DXVECTOR3	vUp,
							D3DXVECTOR3	*i_pPos = NULL);
	// end 2007-04-19 by bhsohn ��ġ���� ������ �߰�

	CItemData(CAtumNode * pParent,CAtumData * pAttacker,MSG_FC_BATTLE_DROP_FIXER_OK* pMsg);
	CItemData(int nItemNum);
	virtual ~CItemData();
	virtual void Tick();

	void RenderItemName();
	void ChaffPos(int nCount,float fGap);
	void SendItemGetItem( const int& _nMyInventory, const int& nMaxMyInventory );

	
	DWORD					m_dwState;
	CAtumData	*			m_pTarget;
	D3DXVECTOR3				m_vPos;
	D3DXVECTOR3				m_vVel;
	D3DXVECTOR3				m_vUp;
	D3DXVECTOR3				m_vTargetPos;
	D3DXMATRIX				m_mFireMatrix;
	D3DXMATRIX				m_mMatrix;
	FLOAT					m_fCheckPacketTime;					// Client �� �������� �����Ҷ� ��Ŷ�� �����ϱ� ���� ����
	FLOAT					m_fSpeed;
	INT						m_nItemIndex;
	INT						m_nItemNum;
	BOOL					m_bIsRender;
	INT						m_nObjScreenX;			// ��ũ���� X ��ǥ
	INT						m_nObjScreenY;			// ��ũ���� Y ��ǥ
	INT						m_nObjScreenW;			// ��ũ���� W ��ǥ
	FLOAT					m_fObjectSize;
	int						m_nCase;				// ������ ȸ���� ���(����)
	BOOL					m_bTurnCheck;
	FLOAT					m_fItemCheckTime;
	UID32_t					m_nFirstCharacterUID;			// ������ ���� ������ ù��° ĳ����
	BYTE					m_byKind;				// �������� ����.

	CChatMoveData	*		m_pMoveChat;

	BodyCond_t				m_bodyCondition;
	CCharacterInfo*			m_pCharacterInfo;
	void LoadCharacterEffect(char* strFileName);

	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	BOOL					m_bInfinityEffect;
};

#endif // !defined(AFX_ITEMDATA_H__42443EB1_ABF9_469B_929A_53D8BC2F14A0__INCLUDED_)
