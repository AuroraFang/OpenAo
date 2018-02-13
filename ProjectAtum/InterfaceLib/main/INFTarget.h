// INFTarget.h: interface for the CINFTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFTARGET_H__1E2FD209_BB49_4CB7_ADDA_5BE689875601__INCLUDED_)
#define AFX_INFTARGET_H__1E2FD209_BB49_4CB7_ADDA_5BE689875601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"


#define VEL_ATTACK_MOUSE_ROTATION			11
#define VEL_NORMAL_MOUSE_ROTATION			2
#define TARGET_MOUSE_SCALE		((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
#define CENTER_START_X				(g_pD3dApp->GetBackBufferDesc().Width/2 - 15*TARGET_MOUSE_SCALE)
#define CENTER_START_Y				(g_pD3dApp->GetBackBufferDesc().Height/2 - 15*TARGET_MOUSE_SCALE)
#define MOUSE_HALF_SIZE				(25*TARGET_MOUSE_SCALE)
#define MOUSE_START_POS_X			(x - MOUSE_HALF_SIZE)
#define MOUSE_START_POS_Y			(y - MOUSE_HALF_SIZE)
#define SECOND_TARGET_HALF_SIZE_X	(56/2*TARGET_MOUSE_SCALE)
#define SECOND_TARGET_HALF_SIZE_Y	(54/2*TARGET_MOUSE_SCALE)
#define AUTO_TARGET_HALF_SIZE		(42*TARGET_MOUSE_SCALE)
#define HP_START_X_FROM_MOUSE		(AUTO_TARGET_HALF_SIZE - (11*TARGET_MOUSE_SCALE))
#define HP_START_Y_FROM_MOUSE		(AUTO_TARGET_HALF_SIZE + (1*TARGET_MOUSE_SCALE) - 2)				// 2006-11-27 by ispark
#define AUTO_TARGET_HP_SIZE_X		61//*TARGET_MOUSE_SCALE)
#define AUTO_TARGET_HP_SIZE_Y		5//*TARGET_MOUSE_SCALE)
#define TARGET_ARROW_HALF_SIZE		(14*TARGET_MOUSE_SCALE)//28/2
#define NAME_FROM_MOUSE_CENTER_Y	(AUTO_TARGET_HALF_SIZE + 14) // 12 : text height
#define DISTANCE_FROM_MOUSE_CENTER_Y (AUTO_TARGET_HALF_SIZE - 6) // 12 : text height
#define ID_GUILD_MARK_START_X		12
#define ID_GUILD_MARK_START_Y		(AUTO_TARGET_HALF_SIZE+25)
#define TARGET_DIR_ARROW_HALF_SIZE	(19*TARGET_MOUSE_SCALE)//28/2

#define FIRE_ANGLE_START_X			(g_pD3dApp->GetBackBufferDesc().Width/2 - 45*TARGET_MOUSE_SCALE)
#define FIRE_ANGLE_START_Y			(g_pD3dApp->GetBackBufferDesc().Height/2 - 45*TARGET_MOUSE_SCALE)

#define MOUSE_STATE_NORMAL			0
#define MOUSE_STATE_UP				1
#define MOUSE_WISPERCHAT_IMAGE_GAB  10

#define CENTER_NORMAL				0
#define CENTER_LOCKON				1
#define TARGET_TICK_MOVE_POS_X		1

class CINFImage;
class CAppEffectData;
class CD3DHanFont;
class CINFGameMainOtherInfo;
class CAtumData;
class CUnitData;
class CINFImageEx;										  // 2011. 10. 10 by jskim UI�ý��� ����
// 2005-08-12 by ispark
struct stSecondTarget
{
	int		nTargetIndexNum;
	int		nTargetAnimationTick;
	BOOL	bUse;
	stSecondTarget()
	{
		nTargetIndexNum = 0;
		nTargetAnimationTick = 0;
		bUse = FALSE;
	}
};

class CINFTarget : public CINFBase  
{
public:
	CINFTarget();
	virtual ~CINFTarget();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderMouse(int x, int y, int type);
	void Render2stTarget(int x, int y, int nTargetAnimationTick);
	void RenderAutoTarget(BOOL bAutoTarget, BOOL bShowAutoTargetHP, CAtumData* pUnit);
	void RenderHP(int x, int y, float fRate);

	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetMouseType(int nMouseType);
	int GetMouseType() { return m_nMouseType; }
	void RenderAttackMeMonster();
	void RenderAutoTargetDrow();
	void RenderAttackMeObject();
	void SetTargetAnimation(){m_bTargetAnimation = TRUE;}
	BOOL GetShowTargetCodition();
	void RenderAttackMePKEnemy();
	void RenderOtherTargetMonster();						// 2006-11-09 by ispark, ���Ͱ� �ٸ� ��� Ÿ�� ǥ��
	void RenderInfluenceMonster(CMonsterData * pMon);		// 2006-11-24 by ispark, ���� ���� ���Ϳ� ���� ǥ��

protected:
	void RenderGameMainMonsterInfo(CMonsterData* pUnit, int x, int y, float fHPCurrent, float fHPMax);
	void RenderGameMainEnemyInfo(CEnemyData* pUnit, int x, int y);
	// 2007-05-22 by bhsohn Ÿ�� ǥ�� ������ ó��
	//void RenderTargetArrow(D3DXVECTOR3 vPos );
	POINT RenderTargetArrow(D3DXVECTOR3 vPos );
	void RenderOutSideMonster(CMonsterData* pMonster);		// ���þ߿��� ��� ���� �����ϴ� ���� ���� ǥ��
	void RenderOutSideEnemy(CEnemyData* pEnemy);
	void RenderOutSideObject(D3DXVECTOR3 vPos);

	// 2007-05-22 by bhsohn Ÿ�� ǥ�� ������ ó��
	void RenderDistance(CAtumData* pUnit, POINT ptArrow);
	BOOL IsTargetToParty(ClientIndex_t i_nTargetIndex);

public:
	CINFImageEx*		m_pCenter[2];				// "t_center0, t_center1", ��� ǥ��
	CINFImageEx*		m_pRedMouse;				// "t_m_red", ���콺 ������ Ÿ��
	CINFImageEx*		m_pArrowMouse;				// "t_m_whi", ���콺 �Ͼ�� ��������
	CINFImageEx*		m_pBlueMouse;				// "t_m_blue", ���콺 �Ķ��� Ÿ��
	CINFImageEx*		m_p2stLockOn;				// "t_2st_l", 2�� ���� ����
	CINFImageEx*		m_pTextLockOn;				// "t_text_l", "LOCKON!"�ؽ�Ʈ
	CINFImageEx*		m_pAutoTarget;				// "t_ototar", �ڵ� Ÿ�� ��
	CINFImageEx*		m_pAutoTargetLong;			// "t_oto", �ڵ� Ÿ�� (�����Ÿ� �ۿ� �ִ� ���
	CINFImageEx*		m_pSkillTarget;				// "skTarget", Ÿ���� �ִ� ��ų ���� Ÿ��
	CINFImageEx*		m_pHPBox;					// "t_hpbox", HP�ڽ�
	CINFImageEx*		m_pHP;						// "t_hp", HP����
	CINFImageEx*		m_pNormalMouse[2];			// "t_m_red", ���콺 ������ Ÿ��
	CINFImageEx*		m_pFireAngle[2];			// "t_angle", ȭ�� (ȭ���̹���,A���� �ȳ���)	

	CINFImageEx*		m_pWisperBoxLeftTRightB;	// �Ӹ� ä��â ���콺 �̹���
	CINFImageEx*		m_pWisperBoxLeftBRightT;	// �Ӹ� ä��â ���콺 �̹���
	CINFImageEx*		m_pWisperBoxLeftRight;		// �Ӹ� ä��â ���콺 �̹���
	CINFImageEx*		m_pWisperBoxTopBottom;		// �Ӹ� ä��â ���콺 �̹���
	CINFImageEx*		m_pWisperBoxCross;			// �Ӹ� ä��â ���콺 �̹���

	CINFImageEx*		m_pMonTargetStateImg;		// ������ ���ݴ�� ���� ǥ���ϴ� �̹���(������ �̸� ����)
	CINFImageEx*		m_pImgTargetMe[2];			// ���� Ÿ���� ���� ����

//	CAppEffectData		* m_pTargetDirArrow;	// ȭ�鿡 ����� Ÿ�� ���� ȭ��ǥ
	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	//CINFImage*		m_pTargetDirArrow[3];		// ȭ�鿡 ����� Ÿ�� ���� ȭ��ǥ
	CINFImageEx*		m_pTargetDirArrow[4];		// ȭ�鿡 ����� Ÿ�� ���� ȭ��ǥ
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	CINFImageEx*		m_pImgArrowHelp;			// ���¼��� ������ ȭ��ǥ�� ������ ���ش�.
	float			m_fTargetDirVel;
	float			m_fMouseRotationtAngle;		// ���콺 ȸ�� ����
	int				m_nMouseType;				// �Ϲ�, ���Ÿ��, ����Ÿ��, ȸ�����, ȸ������
//	float			m_fAutoTargetHPRate;		// �ڵ� Ÿ�� ������(��.��������) HP ��
//	BOOL			m_bAutoTargetShow;			// �ڵ� Ÿ�� ������(��.��������) HP �����ִ��� ����
	CD3DHanFont*	m_pFontAutoTargetName;
	CD3DHanFont*	m_pFontAutoTargetDistance;
	CD3DHanFont*	m_pFontTargetArrow;
	int				m_nMouseState;
	BOOL			m_bRestored;
	BOOL			m_bWisperChatBoxMouseFlag;	// �Ӹ� ä��â ���콺 ����Ʈ Ȱ��ȭ ��Ȱ��ȭ 
	CINFGameMainOtherInfo* m_pOtherInfo;

	float			m_fTickCount;
	int				m_nAttackerTargetX;
	int				m_nTargetColor;
	BOOL			m_bReverseTargetMove;

protected:
	CINFImageEx*	m_pImgTargetAnimation[4];
	CINFImageEx*	m_pImgTargetHelper[2];				// 2007-07-27 by dgwoo ������ ���� ���� [0] 1��, [1] 2��.
	CINFImageEx*	m_pImgSelectTarget[2];
	CINFImageEx*	m_pImgPartyTarget[2];				// 2008-06-17 by dgwoo ��Ƽ���� Ÿ������ ������ ǥ��.

	list<stSecondTarget>	m_listTargetOld;		// 2005-08-12 by ispark
	D3DXVECTOR3		m_vecTargetTemp;				// ȭ�� ������ ���� �ӽ� ����
	BOOL			m_bTargetAnimation;
	BOOL			m_bTargetDistance;
	BOOL			m_bTargetDis;					// ���콺 �����͸� ������ �ʰ� �ϴ��ڵ�
//	int				m_nTargetAnimationTick;			// 2005-08-12 by ispark
	int				m_nOrderTargetX;
	int				m_nOrderTargetY;
	float 			m_fTimeDelay;	

	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	CINFImageEx*		m_pImgFriendlyTarget[2];
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
};

#endif // !defined(AFX_INFTARGET_H__1E2FD209_BB49_4CB7_ADDA_5BE689875601__INCLUDED_)
