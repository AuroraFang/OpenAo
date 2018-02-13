// PetManager.h: interface for the CPetdata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETDATA_H__05E6CAA0_471E_4B47_B691_1EFA0FD1002B__INCLUDED_)
#define AFX_PETDATA_H__05E6CAA0_471E_4B47_B691_1EFA0FD1002B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumData.h"

#define  PET_SKILL_NUMBER 8									   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
// 2010-06-15 by shcho&hslee ��ý��� - �� ���� Ŭ���� ����
class CWeaponItemInfo;
class CPetManager
{
public:
	CPetManager();
	virtual ~CPetManager();
	//������ �ֱ�
	BOOL InsertPetCurrentData(tPET_CURRENTINFO* i_PetCurrentInfo);

	//������ ������Ʈ
	BOOL UpdatePetCurrentData(tPET_CURRENTINFO* i_PetCurrentInfo);
	
	//������ ��������
	BOOL DeletePetCurrentData( UID64_t CreatedPetUID );
	BOOL DeleteAllPetCurrentData ( void );

	tPET_CURRENTINFO *GetPtr_PetCurrentData ( UID64_t PetUID );

	// ���� ��� SourceIndex�� ����.
	int GetSourceIndex_PetCurrentData ( UID64_t PetUID , bool bCharacter = true );

	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��
	CWeaponItemInfo*		GetWeaponItemInfo() { return m_pPetWeapon; }
	void					SetWeaponItemInfo(CWeaponItemInfo* i_WeaponItenInfo) { m_pPetWeapon = i_WeaponItenInfo; }
	void					InitAttackData( void );
// 	ATTACK_DATA*			GetPetAttack() { return m_pPetAttack; }
// 	void					SetPetAttack(ATTACK_DATA* i_PetAttack) { m_pPetAttack = i_PetAttack; }
	//end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��
	
	//void SetPetBaseData(MSG_FC_ITEM_PET_BASEDATA i_baseData);		// setting current baseData
	//void SetPetSkillData(MSG_FC_ITEM_PET_SKILLDATA i_skillData);	// setting current skillData
	//void SetPetSocketData(MSG_FC_ITEM_PET_SOCKETDATA i_socketData);	// setting current socketData	
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	void	SetSelectSocket(int Num){m_nSelectSockt = Num;}
	int		GetSelectSocket(){return m_nSelectSockt;}

	void	SetSocketNum(int Num){m_nSocktNum = Num;}
	int		GetSocketNum(){return m_nSocktNum;}

	void	SetSocketOnOff(BOOL Num){OnOFf = Num;}
	BOOL	GetSocketOnOff(){return OnOFf;}
	
	void	SetSocketUidNumber(UID64_t Num){m_SocketUidNumber = Num;}
	UID64_t	GetSocketUidNumber(){return m_SocketUidNumber;}

	BOOL	GetrenderOnOff(){return m_bRenderFlag;}
	void	SetrenderOnOff(BOOL OnOffFlag){m_bRenderFlag = OnOffFlag;}

	void	SetSocketSelectingNum(int Selectnum){m_nselecting_num = Selectnum;}
	int		GetSocketSelectingNum(){return m_nselecting_num;}

	void	SetSelectingCheck(int serching ,BOOL flag);
	int		GetSelectingCheck(int checking);


	void	SetSelectingCheckSocket(int serching ,BOOL flag);	
	BOOL	GetSelectingCheckSocket(int checking);

	void	SetWearPetSocketPosition(int wearposition);
	int		GetWearPetSocketPosition(){return m_bRenderOn;}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���


private:
	// ��� ������ ������ ����ü
	vecPetCurrentInfo		m_vectPetCurrentData;
	CWeaponItemInfo*		m_pPetWeapon;

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int						m_nselecting_num;
	int						m_nSelectSockt;
	int						m_nSocktNum;
	BOOL					OnOFf;
	BOOL					m_bRenderFlag;
	UID64_t					m_SocketUidNumber;
	BOOL					m_nSerching[3];
	BOOL					m_nSerchingSocket[3];
	BOOL					m_bRenderOn;
	
   //end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	//TACK_DATA*			m_pPetAttack;
};

#endif // !defined(AFX_PETDATA_H__05E6CAA0_471E_4B47_B691_1EFA0FD1002B__INCLUDED_)
