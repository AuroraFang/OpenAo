// INFCharacterInfoExtend.cpp: implementation of the CINFCharacterInfoExtend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "QuestData.h"
#include "D3DHanFont.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "Interface.h"
#include "INFPilotFace.h"
#include "INFWindow.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "AtumSJ.h"
#include "Skill.h"
#include "WeaponItemInfo.h"
#include "StoreData.h"
#include "SkillInfo.h"
#include "INFScrollBar.h"
#include "INFCityBase.h"
#include "Chat.h"
#include "INFSkill.h"
#include "INFCommunity.h"
#include "INFCommunityGuild.h"
#include "INFSelect.h"
#include "dxutil.h"
#include "INFCityBazaar.h"
#include "INFImageBtn.h"		// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
#include "AtumSound.h"			// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
#include "INFItemInfo.h"
// 2009-02-13 by bhsohn ���� ��ŷ �ý���
#include "WorldRankManager.h"
// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

#include "INFCharacterInfoExtend.h"
#include "INFArenaScrollBar.h"	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
#include "PetManager.h"			// 2010-06-15 by shcho&hslee ��ý��� - �� �����͸� �������� �Լ�

#include "CustomOptimizer.h"
// 2011. 10. 10 by jskim UI�ý��� ����
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFImageList.h"			  
// end 2011. 10. 10 by jskim UI�ý��� ����

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
//#define INFO_HP_START_X		56
#define INFO_START_X		250
#define INFO_START_Y		69
#define INFO_START_INTERVAL_X		238


#define INFO_INTERVAL		18

#define CHAR_UTC_START_X		(nWindowPosX+36)
#define CHAR_UTC_START_Y		(nWindowPosY+212)
#define CHAR_UTC_ICON_SIZE		30
#define CHAR_UTC_INTERVAL_X		115
#define CHAR_UTC_INTERVAL_Y		37

#define UTC_SCROLL_BUTTON_START_X			(nWindowPosX+493)	// 2012-02-27 by mspark, ������� â�� ��ų �� ��ũ�� ��ġ �������� ���߱� - 485���� 493���� ����
#define UTC_SCROLL_BAR_SIZE_X				11
#define UTC_SCROLL_BAR_SIZE_Y				30
//#define UTC_MAX_SCROLL_NUMBER				max(max(g_pD3dApp->m_pShuttleChild->m_mapSkillAttack.size(),g_pD3dApp->m_pShuttleChild->m_mapSkillDefense.size()),max(g_pD3dApp->m_pShuttleChild->m_mapSkillSupport.size(),g_pD3dApp->m_pShuttleChild->m_mapSkillAttribute.size()))
#define UTC_MAX_SCROLL_NUMBER				GetUtcMaxScrollNumber()
#define UTC_SCROLL_BUTTON_UP_START_Y		71
#define UTC_SCROLL_BUTTON_DOWN_START_Y		247
#define UTC_SCROLL_BAR_START_Y				209		// 2012-02-27 by mspark, ������� â�� ��ų �� ��ũ�� ��ġ �������� ���߱� - 212���� 209�� ����
#define UTC_SCROLL_BAR_END_Y				538
#define UTC_SCROLL_INTERVAL					(UTC_MAX_SCROLL_NUMBER==0 ? 0:((UTC_SCROLL_BAR_END_Y-UTC_SCROLL_BAR_START_Y + 21.1f)/UTC_MAX_SCROLL_NUMBER))	// 2012-02-27 by mspark, ������� â�� ��ų �� ��ũ�� ��ġ �������� ���߱� - (+ 21.1f �߰�)

#define CHARACTER_FACE_START_X				17
#define CHARACTER_FACE_START_Y				171
#define CHARACTER_FACE_SIZE_X				66
#define CHARACTER_FACE_SIZE_Y				86

#define AMMO_INTERVAL						18
#define AMMO_DEFENSE_START_X				20

#define C_QUEST_START_X						(nWindowPosX+19)
#define C_QUEST_START_Y						70
#define C_QUEST_SIZE_X						171
#define C_QUEST_SIZE_Y						103
//#define C_QUEST_STATE_START_X				136
//#define C_QUEST_STATE_START_Y				70
#define C_QUEST_INTERVAL					17
#define C_QUEST_DESC_START_X				(nWindowPosX+20)
#define C_QUEST_DESC_START_Y				178
#define C_QUEST_DESC_INTERVAL				15
#define C_QUEST_DESC_SIZE_X					171
#define C_QUEST_DESC_SIZE_Y					78


#define QUEST_LIST_SCROLL_START_X		(nWindowPosX+198)
#define QUEST_LIST_SCROLL_START_Y		84
#define QUEST_LIST_SCROLL_LINE_LENGTH	76
#define QUEST_DESC_SCROLL_START_X		(nWindowPosX+198)
#define QUEST_DESC_SCROLL_START_Y		185
#define QUEST_DESC_SCROLL_LINE_LENGTH	61

#define PARTNER_SKILL_SHOP_X			210
#define PARTNER_SKILL_SHOP_Y			115

#define PARTERN_CHECK_SOCKET_X			430
#define PARTERN_CHECK_SOCKET_Y			180

//#define CHAR_SCROLL_BAR_SIZE_X					11
//#define CHAR_SCROLL_BAR_SIZE_Y					30

//#define CHAR_QUEST_LIST_SCROLL_BAR_START_X		192
//#define CHAR_QUEST_LIST_SCROLL_BAR_START_Y		83
//#define CHAR_QUEST_LIST_SCROLL_BAR_LENGTH		(76-CHAR_SCROLL_BAR_SIZE_Y)
//#define CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)		(count==0 ? 0:(CHAR_QUEST_LIST_SCROLL_BAR_LENGTH/count))

//#define CHAR_QUEST_DESC_SCROLL_BAR_START_X		192
//#define CHAR_QUEST_DESC_SCROLL_BAR_START_Y		185
//#define CHAR_QUEST_DESC_SCROLL_BAR_LENGTH		(61-30)
//#define CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count)		(count==0 ? 0:(CHAR_QUEST_DESC_SCROLL_BAR_LENGTH/count))
#define SHOP_START_X				CITY_BASE_NPC_BOX_START_X
#define SHOP_START_Y				(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)
// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
//#define SHOP_SKILL_BACK_START_X		(SHOP_START_X+26)
//#define SHOP_SKILL_BACK_START_Y		(SHOP_START_Y+34)
//#define SHOP_SKILL_START_X			(SHOP_SKILL_BACK_START_X+6)
//#define SHOP_SKILL_START_Y			(SHOP_SKILL_BACK_START_Y+29)
//#define SHOP_SKILL_INTERVAL_X		43
//#define SHOP_SKILL_INTERVAL_Y		32
//#define SHOP_SKILL_SPI_START_X		(SHOP_SKILL_BACK_START_X+26)
//#define SHOP_SKILL_SPI_START_Y		(SHOP_SKILL_BACK_START_Y+238)
//#define SHOP_SKILL_SCROLL_START_X	(SHOP_SKILL_BACK_START_X+174)
//#define SHOP_SKILL_SCROLL_START_Y	(SHOP_SKILL_BACK_START_Y+8)
//#define SHOP_SKILL_LENGTH			181
//#define SHOP_SKILL_BACK_SIZE_X		166
//#define SHOP_SKILL_BACK_SIZE_Y		181
#define SHOP_SKILL_BACK_START_X		(SHOP_START_X)
#define SHOP_SKILL_BACK_START_Y		(SHOP_START_Y)
#define SHOP_SKILL_START_X			(SHOP_SKILL_BACK_START_X+36)
#define SHOP_SKILL_START_Y			(SHOP_SKILL_BACK_START_Y+64)
#define SHOP_SKILL_INTERVAL_X		115
#define SHOP_SKILL_INTERVAL_Y		37
#define SHOP_SKILL_SPI_START_X		(SHOP_SKILL_BACK_START_X+26)
#define SHOP_SKILL_SPI_START_Y		(SHOP_SKILL_BACK_START_Y+238)
#define SHOP_SKILL_SCROLL_START_X	(SHOP_SKILL_BACK_START_X+489)
#define SHOP_SKILL_SCROLL_START_Y	(SHOP_SKILL_BACK_START_Y+58)
#define SHOP_SKILL_LENGTH			204		// 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ �������� ���߱� - ���� 181���� 204�� ����
#define SHOP_SKILL_BACK_SIZE_X		456
#define SHOP_SKILL_BACK_SIZE_Y		181
// end 2008-08-22 by bhsohn EP3 �κ��丮 ó��

#define SKILL_DROP_SIZE				22

// 29/03/2006 by ispark, �� ���� ��ġ ����
//#ifdef LANGUAGE_ENGLISH
//#define INFO_LEVEL_START_X			18//13
//#define INFO_LEVEL_START_Y			25//26
//#define INFO_ID_START_Y				24//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y						96
//#define GENERAL_START_Y						87
//#define PROPENSITY_START_Y					105
//
//#define AMMO_START_X				80//71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			55//50
//#define STATINFO_START_X1			160//155
//
//#define CHARACTER_AMMO_TEXT_X1		26
//#define CHARACTER_AMMO_TEXT_X2		61
//
//#define SHOWSTATINFO_Y1				186
//#define SHOWSTATINFO_Y2				205
//#define SHOWSTATINFO_Y3				223
//#define SHOWSTATINFO_Y4				241
//#endif
//
//#ifdef LANGUAGE_KOREA
//#define INFO_LEVEL_START_X			13
//#define INFO_LEVEL_START_Y			24//26
//#define INFO_ID_START_Y				25//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y				67//69
//#define GENERAL_START_Y				85//87
//#define PROPENSITY_START_Y			103//105
//#define AMMO_START_X				71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			50
//#define STATINFO_START_X1			155
//
//#define CHARACTER_AMMO_TEXT_X1		0
//#define CHARACTER_AMMO_TEXT_X2		51
//
//#define SHOWSTATINFO_Y1				186//187
//#define SHOWSTATINFO_Y2				205//206
//#define SHOWSTATINFO_Y3				223//224
//#define SHOWSTATINFO_Y4				241//243
//#endif
//
//#ifdef LANGUAGE_CHINA
//#define INFO_LEVEL_START_X			13
//#define INFO_LEVEL_START_Y			24//26
//#define INFO_ID_START_Y				25//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y				67//69
//#define GENERAL_START_Y				85//87
//#define PROPENSITY_START_Y			103//105
//#define AMMO_START_X				71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			50
//#define STATINFO_START_X1			155
//
//#define CHARACTER_AMMO_TEXT_X1		0
//#define CHARACTER_AMMO_TEXT_X2		51
//
//#define SHOWSTATINFO_Y1				186//187
//#define SHOWSTATINFO_Y2				205//206
//#define SHOWSTATINFO_Y3				223//224
//#define SHOWSTATINFO_Y4				241//243
//#endif
//
//#ifdef LANGUAGE_VIETNAM
//#define INFO_LEVEL_START_X			18//13
//#define INFO_LEVEL_START_Y			25//26
//#define INFO_ID_START_Y				24//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y						69
//#define GENERAL_START_Y						87
//#define PROPENSITY_START_Y					105
//
//#define AMMO_START_X				118//80//71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			55//50
//#define STATINFO_START_X1			160//155
//
//#define CHARACTER_AMMO_TEXT_X1		13//26
//#define CHARACTER_AMMO_TEXT_X2		99//61
//
//#define SHOWSTATINFO_Y1				186
//#define SHOWSTATINFO_Y2				205
//#define SHOWSTATINFO_Y3				223
//#define SHOWSTATINFO_Y4				241
//#endif
//// 2008-04-30 by bhsohn �±� ���� �߰�
//#ifdef LANGUAGE_THAI
//	#define INFO_LEVEL_START_X			18//13
//	#define INFO_LEVEL_START_Y			25//26
//	#define INFO_ID_START_Y				24//26
//	#define OTHER_INFO_START_Y			184//186
//
//	#define GUILD_START_Y						69
//	#define GENERAL_START_Y						87
//	#define PROPENSITY_START_Y					105
//
//	#define AMMO_START_X				80//71
//	#define AMMO_START_Y				68//70
//
//	#define STATINFO_START_X0			55//50
//	#define STATINFO_START_X1			160//155
//
//	#define CHARACTER_AMMO_TEXT_X1		26
//	#define CHARACTER_AMMO_TEXT_X2		61
//
//	#define SHOWSTATINFO_Y1				186
//	#define SHOWSTATINFO_Y2				205
//	#define SHOWSTATINFO_Y3				223
//	#define SHOWSTATINFO_Y4				241
//#endif
//// 2008-04-30 by bhsohn �±� ���� �߰�
#define GENERAL_START_X						248
#define GENERAL_START_Y						110

#define GUILD_START_X						248
#define GUILD_START_Y						92

#define GUILD_MARK_START_X						130
#define GUILD_MARK_START_Y						94

#define PROPENSITY_START_X					486
#define PROPENSITY_START_Y					56

#define PERSONAL_STAT_X						486
#define PERSONAL_STAT_Y						74

#define GUILDWAR_STAT_X						486
#define GUILDWAR_STAT_Y						92

#define INFO_LEVEL_START_X			248
#define INFO_LEVEL_START_Y			74

#define INFO_ID_START_X		100
#define INFO_ID_START_Y		56

// 2009-02-13 by bhsohn ���� ��ŷ �ý���
#define WR_NICKNAME_START_X						248
#define WR_NICKNAME_START_Y						128
// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

//////////////// ���� ��Ʈ��//////////////// 
#define STATINFO_START_X0			212
#define STATINFO_START_X1			443

#define SHOWSTATINFO_Y1				204
#define SHOWSTATINFO_Y2				222
#define SHOWSTATINFO_Y3				240
#define SHOWSTATINFO_Y4				259

//////////////// ���� ��ư////////////////    
#define STAT_TOOLTIP_LEFT_START_X			175
#define STAT_TOOLTIP_RIGHT_START_X			410

#define STAT_TOOLTIP_ATTACK_START_Y			207
#define STAT_TOOLTIP_ENDURANCE_START_Y		207
#define STAT_TOOLTIP_FUEL_START_Y			207
#define STAT_TOOLTIP_SOUL_START_Y			224
#define STAT_TOOLTIP_DODGE_START_Y			242
#define STAT_TOOLTIP_DEFENSE_START_Y		224
#define STAT_TOOLTIP_SIZE_X					64
#define STAT_TOOLTIP_SIZE_Y					16


#define OTHER_INFO_START_Y			413

// 2007-05-15 by bhsohn ��� ���� ���� ó��
// �������� �۾� ����
#define	OVER_STAT_CAP0_X			(13)
#define	OVER_STAT_CAP1_X			(24)
#define	OVER_STAT_CAPX				30

// 2007-06-07 by dgwoo �Ʒ��� ����â.
#define ARENA_GAP_H							18

#define ARENA_SCORE_X						(nWindowPosX + 248)
#define ARENA_SCORE_Y						(nWindowPosY + 472)

#define ARENA_SCORE_PERCENTAGE_X			(nWindowPosX + 250)
#define ARENA_SCORE_PERCENTAGE_Y			(ARENA_SCORE_Y + ARENA_GAP_H)

#define ARENA_COMPULSION_END_X				(nWindowPosX + 250)
#define ARENA_COMPULSION_END_Y				(ARENA_SCORE_PERCENTAGE_Y + ARENA_GAP_H)

// ���� ����Ʈ
#define ARENA_COMMULATION_POINT_X			(nWindowPosX + 250)
#define ARENA_COMMULATION_POINT_Y			(ARENA_COMPULSION_END_Y + ARENA_GAP_H + ARENA_GAP_H + 5)

// ���� ����Ʈ
#define ARENA_FUSIBLE_POINT_X				(nWindowPosX + 250)
#define ARENA_FUSIBLE_POINT_Y				(ARENA_COMMULATION_POINT_Y + ARENA_GAP_H)

#define AMMO_START_X				250
#define AMMO_START_Y				319
#define AMMO_START_INTERVAL_X		238

#define INVEN_WEIGHT_START_X	488
#define INVEN_WEIGHT_START_Y	(473)

#define AMMO_WEIGHT_START_X				488
#define AMMO_WEIGHT_START_Y				490

#define WEIGHT_FUEL_START_X				488
#define WEIGHT_FUEL_START_Y				508

#define WEIGHT_BOOSTER_START_X				488
#define WEIGHT_BOOSTER_START_Y				526

#define WEIGHT_SPEED_START_X				488
#define WEIGHT_SPEED_START_Y				544


#define INFO_TAB_POX_X			24
#define INFO_TAB_POX_Y			155

#define INFO_TAB_POX_W			m_pInfo->GetImgSize().x
#define INFO_TAB_POX_H			31

#define ATTACK_TOOLIP_W			150

// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
#define PARTNER_NAMECHANGE_BTN_X 457
#define PARTNER_NAMECHANGE_BTN_Y 207 

#define PARTNER_OKCANCEL_BTN_X  347
#define PARTNER_OKCANCEL_BTN_Y  557				//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#define PARTNER_INFO_X			 230 + 10
#define PARTNER_INFO_Y			 212
#define PARTNER_INFO_Y_GAP		 18 

#define PARTNER_DTAIL_INFO_X		455		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_DTAIL_INFO_Y		353		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_DTAIL_INFO_Y_GAP	40		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

// 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� �� ���� ����. - AtumParam.h �ű�.
/*
#define PARTNER_TYPE_ATT		 1
#define PARTNER_TYPE_DFN		 2
#define PARTNER_TYPE_BUF		 3
#define PARTNER_TYPE_STORE		 4
*/
// End 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� �� ���� ����.


#define PARTNER_EXP_POS_X		 187		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���		// 2012-03-29 by mspark, ������� -> ��Ʈ�� -> ����ġ �� ��ġ ���� - ���� 182���� 187�� ����
#define PARTNER_EXP_POS_Y		 355		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���		// 2012-03-29 by mspark, ������� -> ��Ʈ�� -> ����ġ �� ��ġ ���� - ���� 354���� 355�� ����

#define PARTNER_STAMENA_POS_X	 160
#define PARTNER_STAMENA_POS_Y	 359

#define PARTNER_EXP_SCROLL		 50
#define PARTNER_EXP_SCROLL_POS_X 183	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_EXP_SCROLL_POS_Y 392	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_EXP_SCROLL_POS_W 219
#define PARTNER_EXP_SCROLL_POS_H 0

//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

/////////////////////////////////////////////////////////////////////////

#define PARTNER_IMAGE_POS_X			40
#define PARTNER_IMAGE_POS_Y			209

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#define PARTNER_ITEM_SCROLL			100
#define PARTNER_ITEM_SCROLL_POS_X	152
#define PARTNER_ITEM_SCROLL_POS_Y	451
#define PARTNER_ITEM_SCROLL_POS_W	219
#define PARTNER_ITEM_SCROLL_POS_H	0

#define	PARTNER_SOCKET_SKILL_POS_X			119
#define	PARTNER_SOCKET_SKILL_POS_Y			466
#define	PARTNER_SOCKET_SKILL_GAP			10

#define PARTNER_SOCKET_ITEM_POS_X  63
#define PARTNER_SOCKET_ITEM_POS_Y  439
#define PARTNER_SOCKET_ITEM_POS_GAP  31

#define PARTNER_SOCKET_KIT_POS_X  107
#define PARTNER_SOCKET_KIT_POS_Y  436
#define PARTNER_SOCKET_KIT_POS_GAP  35


#define  PARTNER_SKILL_POPUPSHOP_X 512
#define  PARTNER_SKILL_POPUPSHOP_Y 420
#define  PARTNER_SKILL_POPUPSHOP_X_GAP 169
#define  PARTNER_SKILL_POPUPSHOP_Y_GAP 76


#define PARTNER_SOCKET_POS_X		62
#define PARTNER_SOCKET_POS_Y		428
#define PARTNER_SOCKET_GAP			1


#define  PARTNER_SKILL_POPUPSOCKETOK_X 50
#define  PARTNER_SKILL_POPUPSOCKETOK_Y 256
#define  PARTNER_SKILL_POPUPSOCKETOK_X_GAP 388
#define  PARTNER_SKILL_POPUPSOCKETOK_Y_GAP 172

#define  PARTNER_SOCKET_KIT_ITEM_POS_X		162
#define  PARTNER_SOCKET_KIT_ITEM_POS_Y		463

#define DIS_EFF_STEP_TIME							0.5
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#else 
//#define INFO_HP_START_X		56
#define INFO_START_X		248
#define INFO_START_Y		69
#define INFO_START_INTERVAL_X		240


#define INFO_INTERVAL		18

#define CHAR_UTC_START_X		(nWindowPosX+31)
#define CHAR_UTC_START_Y		(nWindowPosY+217)
#define CHAR_UTC_ICON_SIZE		30
#define CHAR_UTC_INTERVAL_X		113
#define CHAR_UTC_INTERVAL_Y		36

#define UTC_SCROLL_BUTTON_START_X			(nWindowPosX+485)
#define UTC_SCROLL_BAR_SIZE_X				11
#define UTC_SCROLL_BAR_SIZE_Y				30
//#define UTC_MAX_SCROLL_NUMBER				max(max(g_pD3dApp->m_pShuttleChild->m_mapSkillAttack.size(),g_pD3dApp->m_pShuttleChild->m_mapSkillDefense.size()),max(g_pD3dApp->m_pShuttleChild->m_mapSkillSupport.size(),g_pD3dApp->m_pShuttleChild->m_mapSkillAttribute.size()))
#define UTC_MAX_SCROLL_NUMBER				GetUtcMaxScrollNumber()
#define UTC_SCROLL_BUTTON_UP_START_Y		71
#define UTC_SCROLL_BUTTON_DOWN_START_Y		247
#define UTC_SCROLL_BAR_START_Y				212
#define UTC_SCROLL_BAR_END_Y				538
#define UTC_SCROLL_INTERVAL					(UTC_MAX_SCROLL_NUMBER==0 ? 0:((UTC_SCROLL_BAR_END_Y-UTC_SCROLL_BAR_START_Y)/UTC_MAX_SCROLL_NUMBER))

#define CHARACTER_FACE_START_X				17
#define CHARACTER_FACE_START_Y				171
#define CHARACTER_FACE_SIZE_X				66
#define CHARACTER_FACE_SIZE_Y				86

#define AMMO_INTERVAL						18
#define AMMO_DEFENSE_START_X				20

#define C_QUEST_START_X						(nWindowPosX+19)
#define C_QUEST_START_Y						70
#define C_QUEST_SIZE_X						171
#define C_QUEST_SIZE_Y						103
//#define C_QUEST_STATE_START_X				136
//#define C_QUEST_STATE_START_Y				70
#define C_QUEST_INTERVAL					17
#define C_QUEST_DESC_START_X				(nWindowPosX+20)
#define C_QUEST_DESC_START_Y				178
#define C_QUEST_DESC_INTERVAL				15
#define C_QUEST_DESC_SIZE_X					171
#define C_QUEST_DESC_SIZE_Y					78


#define QUEST_LIST_SCROLL_START_X		(nWindowPosX+198)
#define QUEST_LIST_SCROLL_START_Y		84
#define QUEST_LIST_SCROLL_LINE_LENGTH	76
#define QUEST_DESC_SCROLL_START_X		(nWindowPosX+198)
#define QUEST_DESC_SCROLL_START_Y		185
#define QUEST_DESC_SCROLL_LINE_LENGTH	61

//#define CHAR_SCROLL_BAR_SIZE_X					11
//#define CHAR_SCROLL_BAR_SIZE_Y					30

//#define CHAR_QUEST_LIST_SCROLL_BAR_START_X		192
//#define CHAR_QUEST_LIST_SCROLL_BAR_START_Y		83
//#define CHAR_QUEST_LIST_SCROLL_BAR_LENGTH		(76-CHAR_SCROLL_BAR_SIZE_Y)
//#define CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)		(count==0 ? 0:(CHAR_QUEST_LIST_SCROLL_BAR_LENGTH/count))

//#define CHAR_QUEST_DESC_SCROLL_BAR_START_X		192
//#define CHAR_QUEST_DESC_SCROLL_BAR_START_Y		185
//#define CHAR_QUEST_DESC_SCROLL_BAR_LENGTH		(61-30)
//#define CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count)		(count==0 ? 0:(CHAR_QUEST_DESC_SCROLL_BAR_LENGTH/count))
#define SHOP_START_X				CITY_BASE_NPC_BOX_START_X
#define SHOP_START_Y				(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)
// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
//#define SHOP_SKILL_BACK_START_X		(SHOP_START_X+26)
//#define SHOP_SKILL_BACK_START_Y		(SHOP_START_Y+34)
//#define SHOP_SKILL_START_X			(SHOP_SKILL_BACK_START_X+6)
//#define SHOP_SKILL_START_Y			(SHOP_SKILL_BACK_START_Y+29)
//#define SHOP_SKILL_INTERVAL_X		43
//#define SHOP_SKILL_INTERVAL_Y		32
//#define SHOP_SKILL_SPI_START_X		(SHOP_SKILL_BACK_START_X+26)
//#define SHOP_SKILL_SPI_START_Y		(SHOP_SKILL_BACK_START_Y+238)
//#define SHOP_SKILL_SCROLL_START_X	(SHOP_SKILL_BACK_START_X+174)
//#define SHOP_SKILL_SCROLL_START_Y	(SHOP_SKILL_BACK_START_Y+8)
//#define SHOP_SKILL_LENGTH			181
//#define SHOP_SKILL_BACK_SIZE_X		166
//#define SHOP_SKILL_BACK_SIZE_Y		181
#define SHOP_SKILL_BACK_START_X		(SHOP_START_X)
#define SHOP_SKILL_BACK_START_Y		(SHOP_START_Y)
#define SHOP_SKILL_START_X			(SHOP_SKILL_BACK_START_X+20)
#define SHOP_SKILL_START_Y			(SHOP_SKILL_BACK_START_Y+57)
#define SHOP_SKILL_INTERVAL_X		116
#define SHOP_SKILL_INTERVAL_Y		35
#define SHOP_SKILL_SPI_START_X		(SHOP_SKILL_BACK_START_X+26)
#define SHOP_SKILL_SPI_START_Y		(SHOP_SKILL_BACK_START_Y+238)
#define SHOP_SKILL_SCROLL_START_X	(SHOP_SKILL_BACK_START_X+489)
#define SHOP_SKILL_SCROLL_START_Y	(SHOP_SKILL_BACK_START_Y+58)
#define SHOP_SKILL_LENGTH			204		// 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ �������� ���߱� - ���� 181���� 204�� ����
#define SHOP_SKILL_BACK_SIZE_X		500
#define SHOP_SKILL_BACK_SIZE_Y		181
// end 2008-08-22 by bhsohn EP3 �κ��丮 ó��

#define SKILL_DROP_SIZE				22

// 29/03/2006 by ispark, �� ���� ��ġ ����
//#ifdef LANGUAGE_ENGLISH
//#define INFO_LEVEL_START_X			18//13
//#define INFO_LEVEL_START_Y			25//26
//#define INFO_ID_START_Y				24//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y						96
//#define GENERAL_START_Y						87
//#define PROPENSITY_START_Y					105
//
//#define AMMO_START_X				80//71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			55//50
//#define STATINFO_START_X1			160//155
//
//#define CHARACTER_AMMO_TEXT_X1		26
//#define CHARACTER_AMMO_TEXT_X2		61
//
//#define SHOWSTATINFO_Y1				186
//#define SHOWSTATINFO_Y2				205
//#define SHOWSTATINFO_Y3				223
//#define SHOWSTATINFO_Y4				241
//#endif
//
//#ifdef LANGUAGE_KOREA
//#define INFO_LEVEL_START_X			13
//#define INFO_LEVEL_START_Y			24//26
//#define INFO_ID_START_Y				25//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y				67//69
//#define GENERAL_START_Y				85//87
//#define PROPENSITY_START_Y			103//105
//#define AMMO_START_X				71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			50
//#define STATINFO_START_X1			155
//
//#define CHARACTER_AMMO_TEXT_X1		0
//#define CHARACTER_AMMO_TEXT_X2		51
//
//#define SHOWSTATINFO_Y1				186//187
//#define SHOWSTATINFO_Y2				205//206
//#define SHOWSTATINFO_Y3				223//224
//#define SHOWSTATINFO_Y4				241//243
//#endif
//
//#ifdef LANGUAGE_CHINA
//#define INFO_LEVEL_START_X			13
//#define INFO_LEVEL_START_Y			24//26
//#define INFO_ID_START_Y				25//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y				67//69
//#define GENERAL_START_Y				85//87
//#define PROPENSITY_START_Y			103//105
//#define AMMO_START_X				71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			50
//#define STATINFO_START_X1			155
//
//#define CHARACTER_AMMO_TEXT_X1		0
//#define CHARACTER_AMMO_TEXT_X2		51
//
//#define SHOWSTATINFO_Y1				186//187
//#define SHOWSTATINFO_Y2				205//206
//#define SHOWSTATINFO_Y3				223//224
//#define SHOWSTATINFO_Y4				241//243
//#endif
//
//#ifdef LANGUAGE_VIETNAM
//#define INFO_LEVEL_START_X			18//13
//#define INFO_LEVEL_START_Y			25//26
//#define INFO_ID_START_Y				24//26
//#define OTHER_INFO_START_Y			184//186
//
//#define GUILD_START_Y						69
//#define GENERAL_START_Y						87
//#define PROPENSITY_START_Y					105
//
//#define AMMO_START_X				118//80//71
//#define AMMO_START_Y				68//70
//
//#define STATINFO_START_X0			55//50
//#define STATINFO_START_X1			160//155
//
//#define CHARACTER_AMMO_TEXT_X1		13//26
//#define CHARACTER_AMMO_TEXT_X2		99//61
//
//#define SHOWSTATINFO_Y1				186
//#define SHOWSTATINFO_Y2				205
//#define SHOWSTATINFO_Y3				223
//#define SHOWSTATINFO_Y4				241
//#endif
//// 2008-04-30 by bhsohn �±� ���� �߰�
//#ifdef LANGUAGE_THAI
//	#define INFO_LEVEL_START_X			18//13
//	#define INFO_LEVEL_START_Y			25//26
//	#define INFO_ID_START_Y				24//26
//	#define OTHER_INFO_START_Y			184//186
//
//	#define GUILD_START_Y						69
//	#define GENERAL_START_Y						87
//	#define PROPENSITY_START_Y					105
//
//	#define AMMO_START_X				80//71
//	#define AMMO_START_Y				68//70
//
//	#define STATINFO_START_X0			55//50
//	#define STATINFO_START_X1			160//155
//
//	#define CHARACTER_AMMO_TEXT_X1		26
//	#define CHARACTER_AMMO_TEXT_X2		61
//
//	#define SHOWSTATINFO_Y1				186
//	#define SHOWSTATINFO_Y2				205
//	#define SHOWSTATINFO_Y3				223
//	#define SHOWSTATINFO_Y4				241
//#endif
//// 2008-04-30 by bhsohn �±� ���� �߰�
#define GENERAL_START_X						248
#define GENERAL_START_Y						110

#define GUILD_START_X						248
#define GUILD_START_Y						92

#define GUILD_MARK_START_X						130
#define GUILD_MARK_START_Y						94

#define PROPENSITY_START_X					486
#define PROPENSITY_START_Y					56

#define PERSONAL_STAT_X						486
#define PERSONAL_STAT_Y						74

#define GUILDWAR_STAT_X						486
#define GUILDWAR_STAT_Y						92

#define INFO_LEVEL_START_X			248
#define INFO_LEVEL_START_Y			74

#define INFO_ID_START_X		100
#define INFO_ID_START_Y		56

// 2009-02-13 by bhsohn ���� ��ŷ �ý���
#define WR_NICKNAME_START_X						248
#define WR_NICKNAME_START_Y						128
// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

//////////////// ���� ��Ʈ��//////////////// 
#define STATINFO_START_X0			209
#define STATINFO_START_X1			443

#define SHOWSTATINFO_Y1				196
#define SHOWSTATINFO_Y2				215
#define SHOWSTATINFO_Y3				233
#define SHOWSTATINFO_Y4				251

//////////////// ���� ��ư//////////////// 
#define STAT_TOOLTIP_LEFT_START_X			175
#define STAT_TOOLTIP_RIGHT_START_X			410

#define STAT_TOOLTIP_ATTACK_START_Y			198
#define STAT_TOOLTIP_ENDURANCE_START_Y		198
#define STAT_TOOLTIP_FUEL_START_Y			198
#define STAT_TOOLTIP_SOUL_START_Y			216
#define STAT_TOOLTIP_DODGE_START_Y			234
#define STAT_TOOLTIP_DEFENSE_START_Y		216
#define STAT_TOOLTIP_SIZE_X					64
#define STAT_TOOLTIP_SIZE_Y					13


#define OTHER_INFO_START_Y			405

// 2007-05-15 by bhsohn ��� ���� ���� ó��
// �������� �۾� ����
#define	OVER_STAT_CAP0_X			(13)
#define	OVER_STAT_CAP1_X			(24)
#define	OVER_STAT_CAPX				30

// 2007-06-07 by dgwoo �Ʒ��� ����â.
#define ARENA_GAP_H							18

#define ARENA_SCORE_X						(nWindowPosX + 248)
#define ARENA_SCORE_Y						(nWindowPosY + 463)

#define ARENA_SCORE_PERCENTAGE_X			(nWindowPosX + 248)
#define ARENA_SCORE_PERCENTAGE_Y			(ARENA_SCORE_Y + ARENA_GAP_H)

#define ARENA_COMPULSION_END_X				(nWindowPosX + 248)
#define ARENA_COMPULSION_END_Y				(ARENA_SCORE_PERCENTAGE_Y + ARENA_GAP_H)

// ���� ����Ʈ
#define ARENA_COMMULATION_POINT_X			(nWindowPosX + 248)
#define ARENA_COMMULATION_POINT_Y			(ARENA_COMPULSION_END_Y + ARENA_GAP_H + ARENA_GAP_H + 5)

// ���� ����Ʈ
#define ARENA_FUSIBLE_POINT_X				(nWindowPosX + 248)
#define ARENA_FUSIBLE_POINT_Y				(ARENA_COMMULATION_POINT_Y + ARENA_GAP_H)

#define AMMO_START_X				248
#define AMMO_START_Y				309
#define AMMO_START_INTERVAL_X		240

#define INVEN_WEIGHT_START_X	486
#define INVEN_WEIGHT_START_Y	(464)

#define AMMO_WEIGHT_START_X				486
#define AMMO_WEIGHT_START_Y				482

#define WEIGHT_FUEL_START_X				486
#define WEIGHT_FUEL_START_Y				500

#define WEIGHT_BOOSTER_START_X				486
#define WEIGHT_BOOSTER_START_Y				518

#define WEIGHT_SPEED_START_X				486
#define WEIGHT_SPEED_START_Y				536


#define INFO_TAB_POX_X			24
#define INFO_TAB_POX_Y			155

#define INFO_TAB_POX_W			80
#define INFO_TAB_POX_H			23

#define ATTACK_TOOLIP_W			150

// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
#define PARTNER_NAMECHANGE_BTN_X 432
#define PARTNER_NAMECHANGE_BTN_Y 207 

#define PARTNER_OKCANCEL_BTN_X  347
#define PARTNER_OKCANCEL_BTN_Y  557				//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#define PARTNER_INFO_X			 222
#define PARTNER_INFO_Y			 207
#define PARTNER_INFO_Y_GAP		 18 

#define PARTNER_DTAIL_INFO_X		455		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_DTAIL_INFO_Y		352		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_DTAIL_INFO_Y_GAP	29		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

// 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� �� ���� ����. - AtumParam.h �ű�.
/*
#define PARTNER_TYPE_ATT		 1
#define PARTNER_TYPE_DFN		 2
#define PARTNER_TYPE_BUF		 3
#define PARTNER_TYPE_STORE		 4
*/
// End 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� �� ���� ����.


#define PARTNER_EXP_POS_X		 187		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���		// 2012-03-29 by mspark, ������� -> ��Ʈ�� -> ����ġ �� ��ġ ���� - ���� 182���� 187�� ����
#define PARTNER_EXP_POS_Y		 355		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���		// 2012-03-29 by mspark, ������� -> ��Ʈ�� -> ����ġ �� ��ġ ���� - ���� 354���� 355�� ����

#define PARTNER_STAMENA_POS_X	 160
#define PARTNER_STAMENA_POS_Y	 359

#define PARTNER_EXP_SCROLL		 50
#define PARTNER_EXP_SCROLL_POS_X 180	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_EXP_SCROLL_POS_Y 379	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define PARTNER_EXP_SCROLL_POS_W 219
#define PARTNER_EXP_SCROLL_POS_H 0

//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

/////////////////////////////////////////////////////////////////////////

#define PARTNER_IMAGE_POS_X			40
#define PARTNER_IMAGE_POS_Y			209

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#define PARTNER_ITEM_SCROLL			100
#define PARTNER_ITEM_SCROLL_POS_X	152
#define PARTNER_ITEM_SCROLL_POS_Y	451
#define PARTNER_ITEM_SCROLL_POS_W	219
#define PARTNER_ITEM_SCROLL_POS_H	0

#define	PARTNER_SOCKET_SKILL_POS_X			119
#define	PARTNER_SOCKET_SKILL_POS_Y			466
#define	PARTNER_SOCKET_SKILL_GAP			10

#define PARTNER_SOCKET_ITEM_POS_X  48
#define PARTNER_SOCKET_ITEM_POS_Y  424
#define PARTNER_SOCKET_ITEM_POS_GAP  31

#define PARTNER_SOCKET_KIT_POS_X  107
#define PARTNER_SOCKET_KIT_POS_Y  436
#define PARTNER_SOCKET_KIT_POS_GAP  35


#define  PARTNER_SKILL_POPUPSHOP_X 512
#define  PARTNER_SKILL_POPUPSHOP_Y 420
#define  PARTNER_SKILL_POPUPSHOP_X_GAP 169
#define  PARTNER_SKILL_POPUPSHOP_Y_GAP 76


#define PARTNER_SOCKET_POS_X		58
#define PARTNER_SOCKET_POS_Y		423
#define PARTNER_SOCKET_GAP			1


#define  PARTNER_SKILL_POPUPSOCKETOK_X 50
#define  PARTNER_SKILL_POPUPSOCKETOK_Y 256
#define  PARTNER_SKILL_POPUPSOCKETOK_X_GAP 388
#define  PARTNER_SKILL_POPUPSOCKETOK_Y_GAP 172

#define  PARTNER_SOCKET_KIT_ITEM_POS_X		162
#define  PARTNER_SOCKET_KIT_ITEM_POS_Y		463

#define DIS_EFF_STEP_TIME							0.5
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#endif					

#define SCROLL_MOVE_X 8		// 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ �������� ���߱�

/////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCharacterInfoExtend::CINFCharacterInfoExtend(CAtumNode* pParent)
{
	FLOG( "CINFCharacterInfoExtend(CAtumNode* pParent)" );
	//m_pAmmo = NULL;
	//m_pStatImage = NULL;
	m_pInfo = NULL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInoBk = NULL;
#endif
	//m_pId = NULL;
	m_pUtc = NULL;
	m_pUtcScr = NULL;
//	m_pImgArenaTab = NULL;
//	m_pScrollBar = NULL;
	m_pQuestSelect = NULL;
	m_pSkillShopBack = NULL;
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int count;
	for (count = 0 ; count < 9 ; count++)
	{
		m_pPartnerSkillShop[count] = NULL;
	}

	for ( count = 0 ; count < 9 ; count++)
	{
		m_pOkSocket[count] = NULL;
	}
	
										  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	m_pParent = pParent;
	m_nCharacterInfoType = CHARACTER_INFO;
	m_nStatType = STAT_BUTTON_UP ;

	int i;
	for(i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i] = NULL ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i] = NULL ;
	}
	
	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	m_StatOverInfoFont = NULL;
	m_StatInfoBoldFont = NULL;

	
	m_SocketDescInfoFont = NULL;		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���


//	for(i=0 ; i<3 ; i++)
//	{
//		m_pStat[i] = NULL ;
//	}
	m_pFontName = NULL;
	m_pFontLevel = NULL;
	m_pFontGuild = NULL;
	m_pFontGeneral = NULL;
	m_pFontPropensity = NULL;
	m_pFontAutoStat = NULL;
	m_pFontStatDec = NULL;
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i] = NULL ;
	}

	m_nCurrentUTCScroll = 0;
	memset(m_pSkillShopSkillDisplayInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*CHAR_UTC_TYPE_NUMBER*MAX_CHAR_UTC_SLOT_NUMBER);
	memset(m_pCharacterSkillDisplayInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*CHAR_UTC_TYPE_NUMBER*MAX_CHAR_UTC_SLOT_NUMBER);
	memset(m_pPartnerSkilldisplay, 0x00, sizeof(INVEN_DISPLAY_INFO)*MAX_PARTNER_UTC_SLOT_NUMBER);  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	m_pSelectIcon = NULL;
	m_pSelectSkill = NULL;
	m_bUTCScrollLock = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;

	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i] = NULL;
//		m_pFontQuestState[i] = NULL;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i] = NULL;
	}
	for(i=0; i<MAX_CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i] = NULL;
	}
//	m_nCurrentQuestListScroll = 0;
//	m_nCurrentSelectQuestList = 0;
//	m_bQuestListLock = FALSE;
//	m_nCurrentQuestDescScroll = 0;
//	m_nCurrentSelectQuestDesc = 0;
//	m_bQuestDescLock = FALSE;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_nRenderMoveIconIntervalWidth  = 0;
	m_nRenderMoveIconIntervalHeight = 0;
	m_pQuestListScrollBar = NULL;
	m_pQuestDescScrollBar = NULL;
	m_pShopSkillScrollBar = NULL;
	m_nOldQuestSelectDataIndex = 0;

	m_pDeleteSkill = NULL;
	m_nMissionProCount = 0;
	m_bLbuttonDown = FALSE;	
//	m_bToolTipStat = FALSE;						// 2005-07-04 by ispark �ڵ� ���� �й� ����

	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	int nTmpCnt=0;
	for(nTmpCnt=0;nTmpCnt < MAX_STAT_SHUTTLEINFO;nTmpCnt++)
	{
		m_bStatShow[nTmpCnt] = TRUE;
		m_nShowOverStat[nTmpCnt] = 0;
		m_pBonusStatBtn[nTmpCnt] = NULL; // ���ʽ� ����
	}
	// end 2007-05-15 by bhsohn ��� ���� ���� ó��

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	m_pOpenBtn = NULL;			
	m_pCloseBtn = NULL;

	m_ptBkPos.x = m_ptBkPos.y = 0;
	m_ptSkillBkPos.x = m_ptSkillBkPos.y = 0;	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_bShowWnd = FALSE;

	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;
	m_ptSkillCommOpMouse.x = m_ptSkillCommOpMouse.y = 0;	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	m_pNickNameBtn = NULL;
	m_pFontNickName = NULL;
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pChangeNameBtn	= NULL;
	m_pPartnerInfoBk	= NULL;
	m_pPartnerName		= NULL;
	m_pPartnerLv		= NULL;
	m_pPartnerType		= NULL;
	m_pStamina			= NULL;
	m_pPartnerLvBar		= NULL;
	m_pPartnerStaminaBar= NULL;
	m_pPartnerGiveExp	= NULL;
	m_pExpScroll		= NULL;
	
	m_pPartnerItem		= NULL;		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	m_pHpItemScroll = NULL;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pDpItemScroll = NULL;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pSpItemScroll = NULL;


	m_bShowSoketSetting = FALSE;	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_bCheckSoketSetting = FALSE;	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	
	m_pPartnerSocet			= NULL;
	m_pPartnerSocetClose	= NULL;
	m_pPartnerSocetSelect	= NULL;
	
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItemLine		= NULL;

	m_pPartnerItemUseSelect	= NULL;
	m_pPartnerSkillSocket = NULL;
	m_pUseSkillSocket = NULL;
	m_pPartnerSkillshopSocket = NULL;

	m_pBigSlotSocket = NULL;

	for (i=0; i < 4; i++)
	{
		m_pItemSocketHP[i] = NULL;
		m_pItemSocketShield[i]  = NULL;
//		m_pItemSocketSp[i]  = NULL;
	}	
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
	for(i=0; i < 2; i++)
	{
		m_pPartnerImageLvBar[i] = NULL;
		m_pPartnerImageStaminaBar[i] = NULL;
	}

	m_pPartnerImage		= NULL;

	m_pPartnerSocketSettingOk = NULL;		 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���	

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for (i = 0 ; i < 2 ; i++)
	{
		m_pImgDissolutionTakeEffect[i] = NULL;
	}

	m_pWearPositionBlink = NULL;

	m_tBlinkTime.Set( 0.0f, 1.5f, 0.5f );	  
    //end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInformationBK = NULL;
#endif
}

CINFCharacterInfoExtend::~CINFCharacterInfoExtend()
{
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	util::del(m_pOpenBtn);	
	util::del(m_pCloseBtn);
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
	{
		m_pBonusStatBtn[nCnt] = NULL; // ���ʽ� ����
	}	
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	util::del(m_pNickNameBtn);
	util::del(m_pFontNickName);
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	
	FLOG( "~CINFCharacterInfoExtend()" );
	//util::del(m_pAmmo );
	//util::del(m_pStatImage ) ;
	util::del(m_pInfo);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del(m_pInoBk);
#endif
	//util::del(m_pId);
	util::del(m_pUtc);
	util::del(m_pUtcScr);
	util::del(m_pSkillShopBack);
//	util::del(m_pImgArenaTab);
//	util::del(m_pScrollBar);
	util::del(m_pQuestListScrollBar);
	util::del(m_pQuestDescScrollBar);
	util::del(m_pShopSkillScrollBar);
	util::del(m_pQuestSelect);
	util::del(m_pFontName);
	util::del(m_pFontLevel);
	util::del(m_pFontGuild);
	util::del(m_pFontGeneral);
	util::del(m_pFontPropensity);
	util::del(m_pFontAutoStat);
	util::del(m_pFontStatDec);
	int i;
	for(i=0 ; i<5 ; i++)
	{
		util::del(m_CharacInfoFont[i] ) ;
	}
	for(i=0 ; i<7 ; i++)
	{
		util::del(m_StatInfoFont[i] ) ;
	}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int count;
	for (count = 0 ; count < 9 ; count++)
	{
		util::del(m_pPartnerSkillShop[count]);
	}

	for ( count = 0 ; count < 9 ; count++)
	{
		util::del(m_pOkSocket[count]);
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	
	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	util::del(m_StatOverInfoFont);
	util::del(m_StatInfoBoldFont);	

	
	util::del(m_SocketDescInfoFont);			   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	
//	for(i=0 ; i<3 ; i++)
//	{
//		util::del(m_pStat[i] );
//	}
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		util::del(m_pFontQuestList[i]);
//		util::del(m_pFontQuestState[i]);
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		util::del(m_pFontQuestDesc[i]);
	}
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		util::del(m_pFontAmmoWeapon[i]);
	}
	for(i=0; i<MAX_CHAR_UTC_SLOT_NUMBER; i++)
	{
		util::del(m_pFontItemNum[i]);
	}
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	util::del(m_pChangeNameBtn);
	util::del(m_pPartnerInfoBk);	
	util::del(m_pPartnerName);
	util::del(m_pPartnerLv);
	util::del(m_pPartnerType);
	util::del(m_pStamina);
	


	util::del(m_pPartnerSocet	);
	util::del(m_pPartnerSocetClose);
	util::del(m_pPartnerSocetSelect);

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	util::del(m_pPartnerItemLine);

	util::del(m_pPartnerItemUseSelect);
	
	util::del(m_pPartnerSkillSocket);
	util::del(m_pPartnerSkillshopSocket);

	

	for(i=0; i < 4; i++)
	{
		util::del(m_pItemSocketHP[i]);		
		util::del(m_pItemSocketShield[i]);		
	//	util::del(m_pItemSocketSp[i]);			
	}
	
	util::del(m_pUseSkillSocket);

	util::del(m_pBigSlotSocket);
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
	util::del(m_pPartnerImage);
	
	util::del(m_pExpScroll);

	for(i=0; i < 2; i++)
	{
		util::del(m_pPartnerImageLvBar[i]);
		util::del(m_pPartnerImageStaminaBar[i]);
	}
	
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
	for (i = 0 ; i < 2 ; i++)
	{
		util::del(m_pImgDissolutionTakeEffect[i]);
	}

	util::del(m_pWearPositionBlink);	  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	util::del(m_pPartnerSocketSettingOk);		   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	util::del( m_pInformationBK );
    #endif
}

HRESULT CINFCharacterInfoExtend::InitDeviceObjects()
{
	FLOG( "CINFCharacterInfoExtend::InitDeviceObjects()" );
	//����
	DataHeader	* pDataHeader ;

//	m_pAmmo = new CINFImage;
//	pDataHeader = FindResource("ammo");
//	m_pAmmo->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;

//		m_pStatImage = new CINFImage;
//	pDataHeader = FindResource("stat");
//	m_pStatImage->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("INFORMATION");
	m_pInformationBK = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pInformationBK->InitDeviceObjects( g_pD3dApp->m_pImageList );		
#endif

	m_pInfo = new CINFImageEx;
	pDataHeader = FindResource("in_bk1");
	m_pInfo->InitDeviceObjects(pDataHeader) ;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
 	m_pInoBk = new CINFImageEx;
	pDataHeader = FindResource("in_bk0");
 	m_pInoBk->InitDeviceObjects(pDataHeader) ;													  
#endif

//		m_pId = new CINFImage;
//	pDataHeader = FindResource("ID");
//	m_pId->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;

	m_pUtc = new CINFImageEx;
	pDataHeader = FindResource("in_bk2");
	m_pUtc->InitDeviceObjects(pDataHeader) ;

	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pPartnerInfoBk = new CINFImageEx;
	pDataHeader = FindResource("in_bk3");
	m_pPartnerInfoBk->InitDeviceObjects(pDataHeader);

	m_pExpScroll = new CINFArenaScrollBar;
	m_pExpScroll->InitDeviceObjects(0, "scrl_b");

// 2012-06-15 by isshin ŰƮ �ڵ� ��� ���� ���� ��ũ�� ���� - c_scrlbt -> scrl_b
	char buf[SIZE_MAX_FILE_NAME];

#ifdef C_UI_REPAIR_ISSHIN
	wsprintf(buf,"scrl_b");
#else
	wsprintf(buf,"c_scrlbt");
#endif	
// end 2012-06-15 by isshin ŰƮ �ڵ� ��� ���� ���� ��ũ�� ���� - c_scrlbt -> scrl_b
	
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pHpItemScroll = new CINFArenaScrollBar;
	m_pHpItemScroll->InitDeviceObjects(0, buf);	// 2012-06-15 by isshin ŰƮ �ڵ� ��� ���� ���� ��ũ�� ���� - c_scrlbt -> scrl_b
	m_pDpItemScroll = new CINFArenaScrollBar;
	m_pDpItemScroll->InitDeviceObjects(0, buf);	// 2012-06-15 by isshin ŰƮ �ڵ� ��� ���� ���� ��ũ�� ���� - c_scrlbt -> scrl_b
	m_pSpItemScroll = new CINFArenaScrollBar;
	m_pSpItemScroll->InitDeviceObjects(0, buf);	// 2012-06-15 by isshin ŰƮ �ڵ� ��� ���� ���� ��ũ�� ���� - c_scrlbt -> scrl_b
    //end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	m_pChangeNameBtn = new CINFImageBtn;
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	
	//m_pChangeNameBtn->InitDeviceObjects("PN_change");
	m_pChangeNameBtn->InitDeviceObjects("PN_change","STRTOOLTIP75");
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	

	m_pPartnerName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerName->InitDeviceObjects(g_pD3dDev);

	m_pPartnerLv = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerLv->InitDeviceObjects(g_pD3dDev);

	m_pPartnerType = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerType->InitDeviceObjects(g_pD3dDev);

	m_pStamina = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pStamina->InitDeviceObjects(g_pD3dDev);

	m_pPartnerLvBar = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerLvBar->InitDeviceObjects(g_pD3dDev);

	m_pPartnerStaminaBar = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerStaminaBar->InitDeviceObjects(g_pD3dDev);

	m_pPartnerGiveExp = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerGiveExp->InitDeviceObjects(g_pD3dDev);

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItem  = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);
	m_pPartnerItem->InitDeviceObjects(g_pD3dDev);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	m_pPartnerImageLvBar[0] = new CINFImageEx;
	pDataHeader = FindResource("PN_exp");
	m_pPartnerImageLvBar[0]->InitDeviceObjects(pDataHeader);

	m_pPartnerImageLvBar[1] = new CINFImageEx;
	pDataHeader = FindResource("PN_exp1");
	m_pPartnerImageLvBar[1]->InitDeviceObjects(pDataHeader);
	


	m_pPartnerImageStaminaBar[0] = new CINFImageEx;
	pDataHeader = FindResource("PN_sta");
	m_pPartnerImageStaminaBar[0]->InitDeviceObjects(pDataHeader);

	m_pPartnerImageStaminaBar[1] = new CINFImageEx;
	pDataHeader = FindResource("PN_sta1");
	m_pPartnerImageStaminaBar[1]->InitDeviceObjects(pDataHeader);
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//��Ʈ�� �ý��� ���� ��Ʈ�� ����â ���Ϻκ�
	
	m_pPartnerImage = new CINFImageEx;
	pDataHeader = FindResource("PN_01");
	m_pPartnerImage->InitDeviceObjects(pDataHeader);
	//��Ʈ�� �̹����κ� 
	
	m_pPartnerSocet = new CINFImageEx;
	pDataHeader = FindResource("PN_closoc");		 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerSocet->InitDeviceObjects(pDataHeader);
	//��Ʈ�� ���� ��Ȱ��ȭ ����
	m_pPartnerSocetClose = new CINFImageEx;
	pDataHeader = FindResource("PN_soc02");
	m_pPartnerSocetClose->InitDeviceObjects(pDataHeader);
	//��Ʈ�� ���� ���ȭ ����
	m_pPartnerSocetSelect = new CINFImageEx;
	pDataHeader = FindResource("PN_selsoc");	 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerSocetSelect->InitDeviceObjects(pDataHeader);
	//��Ʈ�� ���� ���� ����	
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItemLine= new CINFImageEx;
	pDataHeader = FindResource("PN_item");
	m_pPartnerItemLine->InitDeviceObjects(pDataHeader);

	m_pPartnerItemUseSelect = new CINFImageEx;
	pDataHeader = FindResource("PN_iauto");
	m_pPartnerItemUseSelect->InitDeviceObjects(pDataHeader);

	m_pPartnerSkillshopSocket = new CINFImageEx;
	pDataHeader = FindResource("itemsl");
	m_pPartnerSkillshopSocket->InitDeviceObjects(pDataHeader) ;

	m_pPartnerSkillSocket = new CINFImageEx;
	pDataHeader = FindResource("PN_socket");
	m_pPartnerSkillSocket->InitDeviceObjects(pDataHeader) ;

	
	m_pUseSkillSocket = new CINFImageEx;
	pDataHeader = FindResource("PN_useitem");
	m_pUseSkillSocket->InitDeviceObjects(pDataHeader) ;


	m_pBigSlotSocket = new CINFImageEx;
	pDataHeader = FindResource("bigslot");
	m_pBigSlotSocket->InitDeviceObjects(pDataHeader) ;


	///////////////////////////////////////////////////////////////////////////////////
	m_pItemSocketHP[0] = new CINFImageEx;
	pDataHeader = FindResource("PN_usehpc");
	m_pItemSocketHP[0]->InitDeviceObjects(pDataHeader) ;

	m_pItemSocketHP[1] = new CINFImageEx;
	pDataHeader = FindResource("PN_usehpb");
	m_pItemSocketHP[1]->InitDeviceObjects(pDataHeader) ;

	m_pItemSocketHP[2] = new CINFImageEx;
	pDataHeader = FindResource("PN_usehpa");
	m_pItemSocketHP[2]->InitDeviceObjects(pDataHeader) ;

	m_pItemSocketHP[3] = new CINFImageEx;
	pDataHeader = FindResource("PN_usehps");
	m_pItemSocketHP[3]->InitDeviceObjects(pDataHeader) ;

	m_pItemSocketShield[0] = new CINFImageEx;
	pDataHeader = FindResource("PN_useshc");
	m_pItemSocketShield[0]->InitDeviceObjects(pDataHeader) ;
	
	m_pItemSocketShield[1] = new CINFImageEx;
	pDataHeader = FindResource("PN_useshb");
	m_pItemSocketShield[1]->InitDeviceObjects(pDataHeader) ;
	
	m_pItemSocketShield[2] = new CINFImageEx;
	pDataHeader = FindResource("PN_usesha");
	m_pItemSocketShield[2]->InitDeviceObjects(pDataHeader) ;
	
	m_pItemSocketShield[3] = new CINFImageEx;
	pDataHeader = FindResource("PN_useshs");
	m_pItemSocketShield[3]->InitDeviceObjects(pDataHeader) ;

	

	m_pItemSocketSp[0] = new CINFImageEx;
	pDataHeader = FindResource("PN_usespc");
	m_pItemSocketSp[0]->InitDeviceObjects(pDataHeader) ;
	
	m_pItemSocketSp[1] = new CINFImageEx;
	pDataHeader = FindResource("PN_usespb");
	m_pItemSocketSp[1]->InitDeviceObjects(pDataHeader) ;
	
	m_pItemSocketSp[2] = new CINFImageEx;
	pDataHeader = FindResource("PN_usespa");
	m_pItemSocketSp[2]->InitDeviceObjects(pDataHeader) ;
	
	m_pItemSocketSp[3] = new CINFImageEx;
	pDataHeader = FindResource("PN_usespa");
	m_pItemSocketSp[3]->InitDeviceObjects(pDataHeader) ;


	
	///////////////////////////////////////////////////////////////////////////////////




	char szSKillShop[9][30];
	int count = 0;

	for (int sNum = 0 ; sNum < 3 ; sNum++)
	{
		for (int sNum2 = 0 ; sNum2 < 3 ; sNum2++ )
		{
			wsprintf(szSKillShop[count], "box%d%d",sNum,sNum2);
			
			m_pPartnerSkillShop[count] = new CINFImageEx;
			pDataHeader = FindResource(szSKillShop[count]);
			m_pPartnerSkillShop[count]->InitDeviceObjects(pDataHeader) ;

			count++;
		}
	}

	char szOkSKillShop[9][30];
	int countOk = 0;
	
	for (int sOkNum = 0 ; sOkNum < 3 ; sOkNum++)
	{
		for (int sOkNum2 = 0 ; sOkNum2 < 3 ; sOkNum2++ )
		{
			wsprintf(szOkSKillShop[countOk], "bbox%d%d",sOkNum,sOkNum2);
			
			m_pOkSocket[countOk] = new CINFImageEx;
			pDataHeader = FindResource(szOkSKillShop[countOk]);
			m_pOkSocket[countOk]->InitDeviceObjects(pDataHeader) ;
			
			countOk++;
		}
	}

	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	
	//��Ʈ�� ��� ��ư ���� 

	
	//��Ʈ�� ���� ȭ�� 

	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_pUtcScr = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pUtcScr->InitDeviceObjects(pDataHeader) ;

		m_pSkillShopBack = new CINFImageEx;
	pDataHeader = FindResource("shskill");
	m_pSkillShopBack->InitDeviceObjects(pDataHeader) ;

//		m_pImgArenaTab = new CINFImage;
//	pDataHeader = FindResource("arena_t");
//	m_pImgArenaTab->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;

		m_pQuestSelect = new CINFImageEx;
	pDataHeader = FindResource("q_select");
	m_pQuestSelect->InitDeviceObjects(pDataHeader) ;

	int i;
	for(i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_CharacInfoFont[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  FALSE,256,32);
		m_StatInfoFont[i]->InitDeviceObjects(g_pD3dDev) ;
	}

	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	m_StatOverInfoFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_StatOverInfoFont->InitDeviceObjects(g_pD3dDev) ;	

	m_StatInfoBoldFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE|D3DFONT_BOLD,  FALSE,256,32);
	m_StatInfoBoldFont->InitDeviceObjects(g_pD3dDev) ;	
	

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_SocketDescInfoFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,1024,32);
	m_SocketDescInfoFont->InitDeviceObjects(g_pD3dDev) ;	
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	
	

//	for(i=0 ; i<3 ; i++)
//	{
//		m_pStat[i] = new CINFImage;
//		char Buf[30] ;
//		wsprintf(Buf, "stat0%d", i+1) ;
// 		pDataHeader = FindResource(Buf);
//		m_pStat[i]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//	}
	m_pFontName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontName->InitDeviceObjects(g_pD3dDev) ;

	m_pFontLevel = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),10, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontLevel->InitDeviceObjects(g_pD3dDev) ;

	m_pFontGuild = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontGuild->InitDeviceObjects(g_pD3dDev) ;
	m_pFontGeneral = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontGeneral->InitDeviceObjects(g_pD3dDev) ;
	m_pFontPropensity = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontPropensity->InitDeviceObjects(g_pD3dDev) ;
	m_pFontAutoStat = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontAutoStat->InitDeviceObjects(g_pD3dDev) ;
	m_pFontStatDec = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
	m_pFontStatDec->InitDeviceObjects(g_pD3dDev) ;

	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontQuestList[i]->InitDeviceObjects(g_pD3dDev) ;
//		m_pFontQuestState[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
//		m_pFontQuestState[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontQuestDesc[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontAmmoWeapon[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(i=0; i<MAX_CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum[i]->InitDeviceObjects(g_pD3dDev);
	}

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	
	m_pQuestListScrollBar = new CINFScrollBar(this,
								QUEST_LIST_SCROLL_START_X-3, 
								nWindowPosY+QUEST_LIST_SCROLL_START_Y+1, 
								QUEST_LIST_SCROLL_LINE_LENGTH,
								C_QUEST_LIST_NUM);
	m_pQuestListScrollBar->SetGameData( m_pGameData );
	m_pQuestListScrollBar->InitDeviceObjects();
	m_pQuestDescScrollBar = new CINFScrollBar(this,
								QUEST_DESC_SCROLL_START_X, 
								nWindowPosY+QUEST_DESC_SCROLL_START_Y, 
								QUEST_DESC_SCROLL_LINE_LENGTH,
								C_QUEST_DESC_NUM);
	m_pQuestDescScrollBar->SetGameData( m_pGameData );
	m_pQuestDescScrollBar->InitDeviceObjects();
	m_pShopSkillScrollBar = new CINFScrollBar(this,
								SHOP_SKILL_SCROLL_START_X, 
								SHOP_SKILL_SCROLL_START_Y, 
								SHOP_SKILL_LENGTH,
								CHAR_UTC_SLOT_NUMBER);
	m_pShopSkillScrollBar->SetGameData( m_pGameData );
	m_pShopSkillScrollBar->InitDeviceObjects();
	m_pShopSkillScrollBar->SetWheelRect(SHOP_SKILL_BACK_START_X, 
		SHOP_SKILL_BACK_START_Y,
		SHOP_SKILL_BACK_START_X+SHOP_SKILL_BACK_SIZE_X,
		SHOP_SKILL_BACK_START_Y+SHOP_SKILL_BACK_SIZE_Y);

	
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
#else
		wsprintf(szUpBtn, "Ook3");
		wsprintf(szDownBtn, "Ook1");
		wsprintf(szSelBtn, "Ook0");
		wsprintf(szDisBtn, "Ook2");
#endif
		if(NULL == m_pPartnerSocketSettingOk)
		{
			m_pPartnerSocketSettingOk = new CINFImageBtn;
		}
		m_pPartnerSocketSettingOk->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}			 

	for(i = 0; i < 2; i++)
	{
		char temp[256];
		m_pImgDissolutionTakeEffect[i] = new CINFImageEx;
		wsprintf(temp,"Dis_slot%d",i + 1);
		pDataHeader = FindResource(temp);
		m_pImgDissolutionTakeEffect[i]->InitDeviceObjects(pDataHeader);
	}
//2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	m_pImgPnUseHp= new CINFImageEx;
	pDataHeader = FindResource("PN_usehp");
	m_pImgPnUseHp->InitDeviceObjects(pDataHeader);

	m_pImgPnUseSh= new CINFImageEx;
	pDataHeader = FindResource("PN_usesh");
	m_pImgPnUseSh->InitDeviceObjects(pDataHeader);

	m_pImgPnUseSp= new CINFImageEx;
	pDataHeader = FindResource("PN_usesp");
	m_pImgPnUseSp->InitDeviceObjects(pDataHeader);
//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	
	if(NULL == m_pWearPositionBlink)
	{
		m_pWearPositionBlink = new CINFImageEx;
		pDataHeader = FindResource("iFocus");
		m_pWearPositionBlink->InitDeviceObjects(pDataHeader);	
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "uopbtn3");
		wsprintf(szDownBtn, "uopbtn1");
		wsprintf(szSelBtn, "uopbtn0");
		wsprintf(szDisBtn, "uopbtn2");
		if(NULL == m_pOpenBtn)
		{
			m_pOpenBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		//m_pOpenBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pOpenBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP15");
		// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	}
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	int nCnt = 0;	
	for(nCnt = 0;nCnt< MAX_STAT_SHUTTLEINFO;nCnt++)
	{		
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "stat03");
		wsprintf(szDownBtn, "stat01");
		wsprintf(szSelBtn, "stat00");
		wsprintf(szDisBtn, "stat02");
		if(NULL == m_pBonusStatBtn[nCnt])
		{
			m_pBonusStatBtn[nCnt] = new CINFImageBtn;
		}
		m_pBonusStatBtn[nCnt]->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		// �ݱ�
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");
#else
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();	
	if(pWorldRankManager->GetUseWorldRanking())
	{	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "PN_change3");
		wsprintf(szDownBtn, "PN_change1");
		wsprintf(szSelBtn, "PN_change0");
		wsprintf(szDisBtn, "PN_change2");
#else
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "op_j3");
		wsprintf(szDownBtn, "op_j1");
		wsprintf(szSelBtn, "op_j0");
		wsprintf(szDisBtn, "op_j2");
#endif
		if(NULL == m_pNickNameBtn)
		{
			m_pNickNameBtn = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
			//m_pNickNameBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pNickNameBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP109");
			// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
		}		
		if(NULL == m_pFontNickName)
		{
			m_pFontNickName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
			m_pFontNickName->InitDeviceObjects(g_pD3dDev) ;
		}
	}
	
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	return S_OK ;
}



HRESULT CINFCharacterInfoExtend::RestoreDeviceObjects()
{
	FLOG( "CINFCharacterInfoExtend::RestoreDeviceObjects()" );
	//m_pAmmo->RestoreDeviceObjects();
	//m_pStatImage->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInformationBK->RestoreDeviceObjects();													  
#else 
	m_pInoBk->RestoreDeviceObjects();
#endif
	m_pInfo->RestoreDeviceObjects();
	//m_pId->RestoreDeviceObjects();
	m_pUtc->RestoreDeviceObjects();
	m_pUtcScr->RestoreDeviceObjects();
	m_pSkillShopBack->RestoreDeviceObjects();
	//m_pImgArenaTab->RestoreDeviceObjects();
//	m_pScrollBar->RestoreDeviceObjects();
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pPartnerInfoBk->RestoreDeviceObjects();
	m_pChangeNameBtn->RestoreDeviceObjects();
	m_pPartnerName->RestoreDeviceObjects();
	m_pExpScroll->RestoreDeviceObjects();
	m_pPartnerLv->RestoreDeviceObjects();
	m_pPartnerType->RestoreDeviceObjects();
	m_pStamina->RestoreDeviceObjects();
	m_pPartnerLvBar->RestoreDeviceObjects();
	m_pPartnerStaminaBar->RestoreDeviceObjects();
	m_pPartnerGiveExp->RestoreDeviceObjects();
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItem->RestoreDeviceObjects();


	m_pHpItemScroll->RestoreDeviceObjects();
	m_pDpItemScroll->RestoreDeviceObjects();
	m_pSpItemScroll->RestoreDeviceObjects();

	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerImage->RestoreDeviceObjects();
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
// 	m_pPartnerBtnCancel->RestoreDeviceObjects();
// 	m_pPartnerBtnOK->RestoreDeviceObjects();
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	int i;
	for(i=0; i < 2; i++)
	{
		m_pPartnerImageLvBar[i]->RestoreDeviceObjects();
		m_pPartnerImageStaminaBar[i]->RestoreDeviceObjects();
	}
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pFontName->RestoreDeviceObjects();
	m_pFontLevel->RestoreDeviceObjects();
	m_pFontGuild->RestoreDeviceObjects();
	m_pFontGeneral->RestoreDeviceObjects();
	m_pFontPropensity->RestoreDeviceObjects();
	m_pFontAutoStat->RestoreDeviceObjects();
	m_pFontStatDec->RestoreDeviceObjects();
	m_pQuestListScrollBar->RestoreDeviceObjects();
	m_pQuestDescScrollBar->RestoreDeviceObjects();
	m_pQuestSelect->RestoreDeviceObjects();
	m_pShopSkillScrollBar->RestoreDeviceObjects();

	
	m_pPartnerSocet->RestoreDeviceObjects();
	m_pPartnerSocetClose->RestoreDeviceObjects();
	m_pPartnerSocetSelect->RestoreDeviceObjects();
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItemLine->RestoreDeviceObjects();

	m_pPartnerItemUseSelect->RestoreDeviceObjects();
	m_pPartnerSkillSocket->RestoreDeviceObjects();

	m_pPartnerSkillshopSocket->RestoreDeviceObjects();

	m_pUseSkillSocket->RestoreDeviceObjects();

	m_pBigSlotSocket->RestoreDeviceObjects();
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i]->RestoreDeviceObjects();
	}
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for(i=0 ; i<4 ; i++)
	{
		m_pItemSocketHP[i]->RestoreDeviceObjects();
		m_pItemSocketShield[i]->RestoreDeviceObjects();
		m_pItemSocketSp[i]->RestoreDeviceObjects();
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	for( i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i]->RestoreDeviceObjects() ;
	}
 	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int count;
	for (count = 0 ; count < 9 ; count++)
	{
		m_pPartnerSkillShop[count]->RestoreDeviceObjects();
	}
	for (count = 0 ; count < 9 ; count++)
	{
		m_pOkSocket[count]->RestoreDeviceObjects();
	}
    //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i]->RestoreDeviceObjects() ;
	}
	// 2007-05-15 by bhsohn ��� ���� ���� ó��	
	m_StatOverInfoFont->RestoreDeviceObjects() ;
	m_StatInfoBoldFont->RestoreDeviceObjects() ;


	m_SocketDescInfoFont->RestoreDeviceObjects();		   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	

//	for(i=0 ; i<3 ; i++)
//	{
//		m_pStat[i]->RestoreDeviceObjects() ;
//	}
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i]->RestoreDeviceObjects() ;
//		m_pFontQuestState[i]->RestoreDeviceObjects() ;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i]->RestoreDeviceObjects() ;
	}
	for(i=0; i<MAX_CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i]->RestoreDeviceObjects() ;
	}
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	if(m_pOpenBtn)
	{
		m_pOpenBtn->RestoreDeviceObjects();						
	}
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
    if(m_pPartnerSocketSettingOk)
	{
		m_pPartnerSocketSettingOk->RestoreDeviceObjects();						
	}

	for(i = 0; i < 2; i++)
	{
		m_pImgDissolutionTakeEffect[i]->RestoreDeviceObjects();
	}
// 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	m_pImgPnUseHp->RestoreDeviceObjects();
	m_pImgPnUseSh->RestoreDeviceObjects();
	m_pImgPnUseSp->RestoreDeviceObjects();
//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	m_pWearPositionBlink->RestoreDeviceObjects();

	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();						
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
	{
		m_pBonusStatBtn[nCnt]->RestoreDeviceObjects();
	}	
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	if(m_pNickNameBtn)
	{
		m_pNickNameBtn->RestoreDeviceObjects();
	}
	if(m_pFontNickName)
	{
		m_pFontNickName->RestoreDeviceObjects();
	}
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFCharacterInfoExtend::DeleteDeviceObjects()
{
	FLOG( "CINFCharacterInfoExtend::DeleteDeviceObjects()" );
//	m_pAmmo->DeleteDeviceObjects();
//	util::del(m_pAmmo );
//	m_pStatImage->DeleteDeviceObjects();
//	util::del(m_pStatImage );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInformationBK->DeleteDeviceObjects();
	util::del(m_pInformationBK);																  
#else
    m_pInoBk->DeleteDeviceObjects();
	util::del(m_pInoBk );
#endif

	m_pInfo->DeleteDeviceObjects();
	util::del(m_pInfo );
//	m_pId->DeleteDeviceObjects();
//	util::del(m_pId );
	m_pUtc->DeleteDeviceObjects();
	util::del(m_pUtc );
	m_pUtcScr->DeleteDeviceObjects();
	util::del(m_pUtcScr );
	m_pSkillShopBack->DeleteDeviceObjects();
	util::del(m_pSkillShopBack );
//	m_pImgArenaTab->DeleteDeviceObjects();
//	util::del(m_pImgArenaTab );
//	m_pScrollBar->DeleteDeviceObjects();
//	util::del(m_pScrollBar );
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pPartnerInfoBk->DeleteDeviceObjects();
	util::del(m_pPartnerInfoBk);
	m_pExpScroll->DeleteDeviceObjects();
	util::del(m_pExpScroll);
	m_pChangeNameBtn->DeleteDeviceObjects();
	util::del(m_pChangeNameBtn);
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pHpItemScroll->DeleteDeviceObjects();
	m_pDpItemScroll->DeleteDeviceObjects();
	m_pSpItemScroll->DeleteDeviceObjects();

	util::del(m_pHpItemScroll);
	util::del(m_pDpItemScroll);
	util::del(m_pSpItemScroll);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	m_pPartnerSocet->DeleteDeviceObjects();
	util::del(m_pPartnerSocet);
	m_pPartnerSocetClose->DeleteDeviceObjects();
	util::del(m_pPartnerSocetClose);
	m_pPartnerSocetSelect->DeleteDeviceObjects();
	util::del(m_pPartnerSocetSelect);

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerSkillshopSocket->DeleteDeviceObjects();
	util::del(m_pPartnerSkillshopSocket);

	m_pPartnerSkillSocket->DeleteDeviceObjects();
	util::del(m_pPartnerSkillSocket);
	
	m_pUseSkillSocket->DeleteDeviceObjects();
	util::del(m_pUseSkillSocket);


	m_pBigSlotSocket->DeleteDeviceObjects();
	util::del(m_pBigSlotSocket);

	int i;
	for(i=0 ; i<4 ; i++)
	{
		m_pItemSocketHP[i]->DeleteDeviceObjects();
		util::del(m_pItemSocketHP[i]);
		m_pItemSocketShield[i]->DeleteDeviceObjects();
		util::del(m_pItemSocketShield[i]);
		m_pItemSocketSp[i]->DeleteDeviceObjects();
		util::del(m_pItemSocketSp[i]);
		

	}


	m_pPartnerItemLine->DeleteDeviceObjects();
	util::del(m_pPartnerItemLine);



	m_pPartnerItemUseSelect->DeleteDeviceObjects();
	util::del(m_pPartnerItemUseSelect);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerImage->DeleteDeviceObjects();
	util::del(m_pPartnerImage);

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
// 	m_pPartnerBtnCancel->DeleteDeviceObjects();
// 	util::del(m_pPartnerBtnCancel);
// 	m_pPartnerBtnOK->DeleteDeviceObjects();
// 	util::del(m_pPartnerBtnOK);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	
	m_pPartnerName->DeleteDeviceObjects();
	util::del(m_pPartnerName);
	m_pPartnerLv->DeleteDeviceObjects();
	util::del(m_pPartnerLv);
	m_pPartnerType->DeleteDeviceObjects();
	util::del(m_pPartnerType);
	m_pStamina->DeleteDeviceObjects();
	util::del(m_pStamina);
	m_pPartnerLvBar->DeleteDeviceObjects();
	util::del(m_pPartnerLvBar);
	m_pPartnerStaminaBar->DeleteDeviceObjects();
	util::del(m_pPartnerStaminaBar);
	m_pPartnerGiveExp->DeleteDeviceObjects();
	util::del(m_pPartnerGiveExp);
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItem->DeleteDeviceObjects();
	util::del(m_pPartnerItem);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for( i=0; i<2; i++)
	{
		m_pPartnerImageLvBar[i]->DeleteDeviceObjects();
		util::del(m_pPartnerImageLvBar[i]);
		m_pPartnerImageStaminaBar[i]->DeleteDeviceObjects();
		util::del(m_pPartnerImageStaminaBar[i]);
	}
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pFontName->DeleteDeviceObjects();
	util::del(m_pFontName );
	m_pFontLevel->DeleteDeviceObjects();
	util::del(m_pFontLevel );
	m_pFontGuild->DeleteDeviceObjects();
	util::del(m_pFontGuild );
	m_pFontGeneral->DeleteDeviceObjects();
	util::del(m_pFontGeneral );
	m_pFontPropensity->DeleteDeviceObjects();
	util::del(m_pFontPropensity );
	m_pFontAutoStat->DeleteDeviceObjects();
	util::del(m_pFontAutoStat );
	m_pFontStatDec->DeleteDeviceObjects();
	util::del(m_pFontStatDec );
	m_pQuestListScrollBar->DeleteDeviceObjects();
	util::del(m_pQuestListScrollBar );
	m_pQuestDescScrollBar->DeleteDeviceObjects();
	util::del(m_pQuestDescScrollBar );
	m_pQuestSelect->DeleteDeviceObjects();
	util::del(m_pQuestSelect );
	m_pShopSkillScrollBar->DeleteDeviceObjects();
	util::del(m_pShopSkillScrollBar );
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i]->DeleteDeviceObjects();
		util::del(m_pFontAmmoWeapon[i] );
	}

	for( i=0 ; i<5 ; i++)
	{
		if(m_CharacInfoFont[i])
		{
			m_CharacInfoFont[i]->DeleteDeviceObjects() ;
			util::del(m_CharacInfoFont[i] );
		}
	}
	for(i=0 ; i<7 ; i++)
	{
		if(m_StatInfoFont[i])
		{
			m_StatInfoFont[i]->DeleteDeviceObjects() ;
			util::del(m_StatInfoFont[i] );
		}
	}
	// 2007-05-15 by bhsohn ��� ���� ���� ó��	
	if(m_StatOverInfoFont)
	{
		m_StatOverInfoFont->DeleteDeviceObjects() ;
		util::del(m_StatOverInfoFont );
	}		
	if(m_StatInfoBoldFont)
	{
		m_StatInfoBoldFont->DeleteDeviceObjects() ;
		util::del(m_StatInfoBoldFont );
	}	

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_SocketDescInfoFont)
	{
		m_SocketDescInfoFont->DeleteDeviceObjects() ;
		util::del(m_SocketDescInfoFont );
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	

//	for(i=0 ; i<3 ; i++)
//	{
//		m_pStat[i]->DeleteDeviceObjects() ;
//		util::del(m_pStat[i] );
//	}
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i]->DeleteDeviceObjects() ;
		util::del(m_pFontQuestList[i] );
//		m_pFontQuestState[i]->DeleteDeviceObjects() ;
//		util::del(m_pFontQuestState[i] );
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i]->DeleteDeviceObjects() ;
		util::del(m_pFontQuestDesc[i] );
	}
	for(i=0; i<MAX_CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i]->DeleteDeviceObjects() ;
		util::del(m_pFontItemNum[i] );
	}
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	if(m_pOpenBtn)
	{		
		m_pOpenBtn->DeleteDeviceObjects();
		util::del(m_pOpenBtn);
	}	
	 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pPartnerSocketSettingOk)
	{		
		m_pPartnerSocketSettingOk->DeleteDeviceObjects();
		util::del(m_pPartnerSocketSettingOk);
	}	
	
	for(i = 0; i < 2; i++)
	{
		if(m_pImgDissolutionTakeEffect[i])
		{
			m_pImgDissolutionTakeEffect[i]->DeleteDeviceObjects();
			util::del(m_pImgDissolutionTakeEffect[i]);
		}
	}
//2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	m_pImgPnUseHp->DeleteDeviceObjects();
	util::del(m_pImgPnUseHp);

	m_pImgPnUseSh->DeleteDeviceObjects();
	util::del(m_pImgPnUseSh);

	m_pImgPnUseSp->DeleteDeviceObjects();
	util::del(m_pImgPnUseSp);
//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	if(m_pWearPositionBlink)
	{
		m_pWearPositionBlink->DeleteDeviceObjects();
		util::del(m_pWearPositionBlink );
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
	{
		m_pBonusStatBtn[nCnt]->DeleteDeviceObjects();
		util::del(m_pBonusStatBtn[nCnt]);
	}	
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	if(m_pNickNameBtn)
	{
		m_pNickNameBtn->DeleteDeviceObjects();
		util::del(m_pNickNameBtn);
	}
	if(m_pFontNickName)
	{
		m_pFontNickName->DeleteDeviceObjects();
		util::del(m_pFontNickName);
	}
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int count = 0;
	for ( count = 0; count < 9 ; count++)
	{
		m_pPartnerSkillShop[count]->DeleteDeviceObjects();
		util::del(m_pPartnerSkillShop[count]);
	}
	for ( count = 0; count < 9 ; count++)
	{
		m_pOkSocket[count]->DeleteDeviceObjects();
		util::del(m_pOkSocket[count]);
	}
    //end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	m_bInvalidated = FALSE;

	return S_OK ;
}


HRESULT CINFCharacterInfoExtend::InvalidateDeviceObjects()
{
	FLOG( "CINFCharacterInfoExtend::InvalidateDeviceObjects()" );
	//m_pAmmo->InvalidateDeviceObjects();
	//m_pStatImage->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pInformationBK->InvalidateDeviceObjects();
#else 
	m_pInoBk->InvalidateDeviceObjects();
#endif

	m_pInfo->InvalidateDeviceObjects();
	//m_pId->InvalidateDeviceObjects();
	m_pUtc->InvalidateDeviceObjects();
	m_pUtcScr->InvalidateDeviceObjects();
	m_pSkillShopBack->InvalidateDeviceObjects();
//	m_pImgArenaTab->InvalidateDeviceObjects();
//	m_pScrollBar->InvalidateDeviceObjects();
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pPartnerInfoBk->InvalidateDeviceObjects();
	m_pPartnerName->InvalidateDeviceObjects();
	m_pChangeNameBtn->InvalidateDeviceObjects();
	m_pPartnerImage->InvalidateDeviceObjects();

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	//m_pPartnerBtnCancel->InvalidateDeviceObjects();
	//m_pPartnerBtnOK->InvalidateDeviceObjects();
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pExpScroll->InvalidateDeviceObjects();
	m_pPartnerLv->InvalidateDeviceObjects();
	m_pPartnerType->InvalidateDeviceObjects();
	m_pStamina->InvalidateDeviceObjects();
	m_pPartnerLvBar->InvalidateDeviceObjects();
	m_pPartnerStaminaBar->InvalidateDeviceObjects();
	m_pPartnerGiveExp->InvalidateDeviceObjects();

	m_pPartnerItem->InvalidateDeviceObjects();	 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	m_pHpItemScroll->InvalidateDeviceObjects();	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pDpItemScroll->InvalidateDeviceObjects();	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pSpItemScroll->InvalidateDeviceObjects();	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	m_pPartnerSocet->InvalidateDeviceObjects();
	m_pPartnerSocetClose->InvalidateDeviceObjects();
	m_pPartnerSocetSelect->InvalidateDeviceObjects();

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerItemLine->InvalidateDeviceObjects();

	m_pPartnerItemUseSelect->InvalidateDeviceObjects();
	m_pPartnerSkillSocket->InvalidateDeviceObjects();

	m_pPartnerSkillshopSocket->InvalidateDeviceObjects();

	m_pUseSkillSocket->InvalidateDeviceObjects();

	m_pBigSlotSocket->InvalidateDeviceObjects();

	int i;
	for(i=0; i< 4; i++)
	{
		m_pItemSocketHP[i]->InvalidateDeviceObjects();
		m_pItemSocketShield[i]->InvalidateDeviceObjects();
		m_pItemSocketSp[i]->InvalidateDeviceObjects();
		
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for(i=0; i< 2; i++)
	{
		m_pPartnerImageLvBar[i]->InvalidateDeviceObjects();
		m_pPartnerImageStaminaBar[i]->InvalidateDeviceObjects();
	}	
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	m_pQuestListScrollBar->InvalidateDeviceObjects();
	m_pQuestDescScrollBar->InvalidateDeviceObjects();
	m_pQuestSelect->InvalidateDeviceObjects();
	m_pShopSkillScrollBar->InvalidateDeviceObjects();

	for(i=0 ; i<5 ; i++)
	{
		m_CharacInfoFont[i]->InvalidateDeviceObjects() ;
	}
	for(i=0 ; i<7 ; i++)
	{
		m_StatInfoFont[i]->InvalidateDeviceObjects() ;
	}
	// 2007-05-15 by bhsohn ��� ���� ���� ó��		
	m_StatOverInfoFont->InvalidateDeviceObjects() ;
	m_StatInfoBoldFont->InvalidateDeviceObjects() ;

		
	m_SocketDescInfoFont->InvalidateDeviceObjects() ;           //2011-10-06 by jhahn ��Ʈ�� ������ �ý���


//	for(i=0 ; i<3 ; i++)
//	{
//		m_pStat[i]->InvalidateDeviceObjects() ;
//	}

	m_pFontName->InvalidateDeviceObjects();
	m_pFontLevel->InvalidateDeviceObjects();
	m_pFontGuild->InvalidateDeviceObjects();
	m_pFontGeneral->InvalidateDeviceObjects();
	m_pFontPropensity->InvalidateDeviceObjects();
	m_pFontAutoStat->InvalidateDeviceObjects();
	m_pFontStatDec->InvalidateDeviceObjects();
	for(i=0;i<C_QUEST_LIST_NUM;i++)
	{
		m_pFontQuestList[i]->InvalidateDeviceObjects() ;
//		m_pFontQuestState[i]->InvalidateDeviceObjects() ;
	}
	for(i=0;i<C_QUEST_DESC_NUM;i++)
	{
		m_pFontQuestDesc[i]->InvalidateDeviceObjects() ;
	}
	for(i=0 ; i<CHARACTER_INTERFACE_FONT_NUM ; i++)
	{
		m_pFontAmmoWeapon[i]->InvalidateDeviceObjects();
	}
	for(i=0; i<MAX_CHAR_UTC_SLOT_NUMBER; i++)
	{
		m_pFontItemNum[i]->InvalidateDeviceObjects() ;
	}
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	if(m_pOpenBtn)
	{		
		m_pOpenBtn->InvalidateDeviceObjects();		
	}
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pPartnerSocketSettingOk)
	{		
		m_pPartnerSocketSettingOk->InvalidateDeviceObjects();		
	}

	for(i = 0; i < 2; i++)
	{
		if(m_pImgDissolutionTakeEffect[i])
		{
			m_pImgDissolutionTakeEffect[i]->InvalidateDeviceObjects();
		}		
	}
// 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	m_pImgPnUseHp->InvalidateDeviceObjects();
	m_pImgPnUseSh->InvalidateDeviceObjects();
	m_pImgPnUseSp->InvalidateDeviceObjects();
//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 	

	m_pWearPositionBlink->InvalidateDeviceObjects();

	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();				
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
	{
		m_pBonusStatBtn[nCnt]->InvalidateDeviceObjects();		
	}	
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	if(m_pNickNameBtn)
	{
		m_pNickNameBtn->InvalidateDeviceObjects();		
	}
	if(m_pFontNickName)
	{
		m_pFontNickName->InvalidateDeviceObjects();		
	}
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int count;
	for (count = 0 ; count < 9 ; count++)
	{
		m_pPartnerSkillShop[count]->InvalidateDeviceObjects();

	}
	
	for (count = 0 ; count < 9 ; count++)
	{
		m_pOkSocket[count]->InvalidateDeviceObjects();
			
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	return S_OK ;
}


void CINFCharacterInfoExtend::Tick()
{
	FLOG( "CINFCharacterInfoExtend::Tick()" );

	if(!IsShowWnd())
	{
		return;
	}
//	if(m_nCharacterInfoType == CHARACTER_QUEST)
//	{
//		if(m_pQuestListScrollBar)
//		{
//			m_pQuestListScrollBar->SetNumberOfData( g_pQuestData->GetProgressQuestNum() + g_pQuestData->m_vecQuest.size());
//			m_pQuestListScrollBar->SetScrollLinePos( QUEST_LIST_SCROLL_START_X, g_pGameMain->m_nRightWindowY+QUEST_LIST_SCROLL_START_Y);
//			m_pQuestListScrollBar->SetWheelRect(C_QUEST_START_X, 
//				g_pGameMain->m_nRightWindowY+C_QUEST_START_Y,
//				C_QUEST_START_X+C_QUEST_SIZE_X,
//				g_pGameMain->m_nRightWindowY+C_QUEST_START_Y+C_QUEST_SIZE_Y);
//			if(m_nOldQuestSelectDataIndex != m_pQuestListScrollBar->GetCurrentSelectDataIndex())
//			{
//				m_nOldQuestSelectDataIndex = m_pQuestListScrollBar->GetCurrentSelectDataIndex();
//				SetQuestDesc(m_nOldQuestSelectDataIndex);
//			}
//		}
//		if(m_pQuestDescScrollBar)
//		{
//			//m_pQuestDescScrollBar->SetNumberOfData( m_vecQuestDescription.size() );
//			m_pQuestDescScrollBar->SetScrollLinePos( QUEST_DESC_SCROLL_START_X, g_pGameMain->m_nRightWindowY+QUEST_DESC_SCROLL_START_Y);
//			m_pQuestDescScrollBar->SetWheelRect(C_QUEST_DESC_START_X, 
//				g_pGameMain->m_nRightWindowY+C_QUEST_DESC_START_Y,
//				C_QUEST_DESC_START_X+C_QUEST_DESC_SIZE_X,
//				g_pGameMain->m_nRightWindowY+C_QUEST_DESC_START_Y+C_QUEST_DESC_SIZE_Y);
//		}
//	}

	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	// ���� ������ ����.
	GetStatShowInfo();
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	Patner3D();
	ExpScrollUpdate();
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

	ItemScrollUpdate();					   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	CheckSocketSetting();				   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���


//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_fTimeEffectBG += g_pD3dApp->GetElapsedTime();		
	
	if(m_fTimeEffectBG >= DIS_EFF_STEP_TIME )
	{
		m_fTimeEffectBG = NULL;
		m_nEffStep += 1;
		if(m_nEffStep >= 2)
		{
			m_nEffStep = NULL;
		}
	}

	m_tBlinkTime.Tick( g_pD3dApp->GetElapsedTime() );
	
	if( m_tBlinkTime.IsOverEnd() )
		m_tBlinkTime.Start();			  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

}

// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
void CINFCharacterInfoExtend::Patner3D()
{
	// ��Ʈ�� 3D ȭ��
}

void CINFCharacterInfoExtend::PatnerInfo()
{

	// 2010-06-15 by shcho&hslee ��ý��� - �� �������ͽ� â ó��.
	//INVEN_DISPLAY_INFO *pEquipPetInfo = NULL;
	tPET_CURRENTINFO *psPetCurrentInfo = NULL;
	tPET_BASEDATA *psPetBaseData = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;
	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	GetPtr_AttachmentedPetInfo ( NULL , &psPetCurrentInfo , &psPetBaseData , &psPetLevelData );
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if (psPetCurrentInfo)
	{
		psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData ( psPetCurrentInfo->PetIndex , psPetCurrentInfo->PetLevel-1 );
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	

	if ( psPetCurrentInfo && psPetBaseData && psPetLevelData )
	{
		// �⺻ ����
		/*
		char * tempName = "�Ƿη�";
		int	   tempLv	= 18;
		int	   tempType = PARTNER_TYPE_ATT;
		int tempMaxStamena = 100000;
		int tempStamena = 6000;
		*/
		
		
		int nWindowPosX = m_ptBkPos.x;
		int nWindowPosY = m_ptBkPos.y;

		char szTemp[256] = {0, };

		// �̸�
		//strcpy(szTemp, tempName);
		strcpy ( szTemp , psPetCurrentInfo->PetName );                 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		m_pPartnerName->DrawText(nWindowPosX + PARTNER_INFO_X, nWindowPosY + PARTNER_INFO_Y, GUI_FONT_COLOR, szTemp, 0L);
		// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
		//Ÿ��
		memset ( szTemp , 0x00 , sizeof( szTemp ) );
		strcpy ( szTemp , g_pInterface->GetString_PetType ( psPetBaseData->PetKind ) );
		m_pPartnerType->DrawText(nWindowPosX + PARTNER_INFO_X, nWindowPosY + PARTNER_INFO_Y + PARTNER_INFO_Y_GAP, GUI_FONT_COLOR, szTemp, 0L);


		//����
		memset(szTemp, 0x00, sizeof(szTemp));
		//wsprintf( szTemp, STRMSG_C_100608_0401, tempLv);
		wsprintf ( szTemp , STRMSG_C_100608_0401 , psPetCurrentInfo->PetLevel );
		m_pPartnerLv->DrawText(nWindowPosX + PARTNER_INFO_X, nWindowPosY + PARTNER_INFO_Y + PARTNER_INFO_Y_GAP * 2, GUI_FONT_COLOR, szTemp, 0L);
		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		DWORD NextLevel_Exp;
		if (psPetLevelDataPrev)				
			NextLevel_Exp = (DWORD)psPetLevelDataPrev->NeedExp;
		
		else
			NextLevel_Exp = 0;
 
		
		memset(szTemp, 0x00, sizeof(szTemp));
		//wsprintf( szTemp, STRMSG_C_100608_0403, tempStamena,tempMaxStamena);
		wsprintf ( szTemp , STRMSG_C_100608_0403 , (DWORD)psPetCurrentInfo->PetExp - NextLevel_Exp, (DWORD)psPetLevelData->NeedExp - NextLevel_Exp);
		m_pStamina->DrawText(nWindowPosX + PARTNER_INFO_X, nWindowPosY + PARTNER_INFO_Y + PARTNER_INFO_Y_GAP * 3, GUI_FONT_COLOR, szTemp, 0L);
		//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â	

		m_pPartnerImage->Move(m_ptBkPos.x + PARTNER_IMAGE_POS_X , m_ptBkPos.y + PARTNER_IMAGE_POS_Y);  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		m_pPartnerImage->Render();			 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���		   
		//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	}

	if ( !psPetCurrentInfo )
	{
		m_pChangeNameBtn->SetBtnState ( BTN_STATUS_DISABLE );
	}
	else 
	{
		if ( m_pChangeNameBtn->GetBtnState() == BTN_STATUS_DISABLE )
			m_pChangeNameBtn->SetBtnState ( BTN_STATUS_UP );
	}

	if( psPetBaseData && psPetBaseData->EnableName)
	{
	// �̸� ���� ��ư 
	m_pChangeNameBtn->SetBtnPosition( m_ptBkPos.x + PARTNER_NAMECHANGE_BTN_X, m_ptBkPos.y + PARTNER_NAMECHANGE_BTN_Y );
	m_pChangeNameBtn->Render();

	}
}	
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

void CINFCharacterInfoExtend::ItemRender()
{

	tPET_CURRENTINFO *pPetCurrentInfo = NULL;
	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;
	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );	

	if (!pInvenDisplayInfo)
		return;
	pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(pInvenDisplayInfo->pItem->UniqueNumber);
	if (!pPetCurrentInfo)
		return;
	psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData(pPetCurrentInfo->PetIndex, pPetCurrentInfo->PetLevel);
	if (!psPetLevelDataPrev)
		return;

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;	
	
	char buff[MAX_PATH];	
	SIZE szSizeTmp={0,0};
	memset(buff, 0x00, MAX_PATH);

	m_pPartnerItemUseSelect->Move( m_ptBkPos.x + 85 + 22,	m_ptBkPos.y + 436 );
	m_pPartnerItemUseSelect->Render();
	m_pHpItemScroll->SetOnlyMaxItem( PARTNER_ITEM_SCROLL, TRUE );
	m_pHpItemScroll->Render();	
	if (pPetCurrentInfo == FALSE)
		return;

	pPetCurrentInfo->PetKitHP.TriggerValue = m_pHpItemScroll->GetScrollStep();
	sprintf(buff, "%d%", m_pHpItemScroll->GetScrollStep());
	szSizeTmp = m_pFontGeneral->GetStringSize(buff);
	m_pPartnerItem->DrawText(nWindowPosX + 407 + 27 - szSizeTmp.cx, nWindowPosY + 448 , GUI_FONT_COLOR, buff, 0L);

	if (psPetLevelDataPrev->KitLevelShield)	
	{
		m_pPartnerItemLine->Move( m_ptBkPos.x + 95 + 22,	m_ptBkPos.y + 442 + 26 + 10 );
		m_pPartnerItemLine->Render();
		m_pDpItemScroll->SetOnlyMaxItem( PARTNER_ITEM_SCROLL, TRUE );
		m_pDpItemScroll->Render();	
		pPetCurrentInfo->PetKitShield.TriggerValue = m_pDpItemScroll->GetScrollStep();
		sprintf(buff, "%d%", m_pDpItemScroll->GetScrollStep());
		szSizeTmp = m_pFontGeneral->GetStringSize(buff);
		m_pPartnerItem->DrawText(nWindowPosX + 407 + 27 - szSizeTmp.cx, nWindowPosY + 448 + 35, GUI_FONT_COLOR, buff, 0L);
	}	
	else
	{
		m_pPartnerSocet->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3   + 1 * 35);		
		m_pPartnerSocet->Render();
	}

	if (psPetLevelDataPrev->KitLevelSP)	
	{
	
		m_pPartnerItemLine->Move( m_ptBkPos.x + 95 + 22,	m_ptBkPos.y + 442 + 60 + 10 );
		m_pPartnerItemLine->Render();
		m_pSpItemScroll->SetOnlyMaxItem( PARTNER_ITEM_SCROLL, TRUE );
		m_pSpItemScroll->Render();	
		pPetCurrentInfo->PetKitSP.TriggerValue = m_pSpItemScroll->GetScrollStep();
		sprintf(buff, "%d%", m_pSpItemScroll->GetScrollStep());
		szSizeTmp = m_pFontGeneral->GetStringSize(buff);
		m_pPartnerItem->DrawText(nWindowPosX + 407  + 27 - szSizeTmp.cx, nWindowPosY + 448  + 70 , GUI_FONT_COLOR, buff, 0L);
	}
	else
	{
		m_pPartnerSocet->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3   + 2 * 35);		
		m_pPartnerSocet->Render();
	}

	tPET_CURRENTINFO *psPetCurrentInfo = NULL;
	tPET_BASEDATA *psPetBaseData = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;
	
	GetPtr_AttachmentedPetInfo ( NULL , &psPetCurrentInfo , &psPetBaseData , &psPetLevelData );
		for (int j = 0 ; j < 3 ; j++)
		{

		int uniquNumber;
		switch(j)
		{
		case 0:
			{
				if (psPetLevelDataPrev->KitLevelHP)	
				{
					uniquNumber = psPetCurrentInfo->PetKitHP.ItemNum;
					CItemInfo *tempderch = g_pShuttleChild->m_pStoreData->FindItemInInventoryByItemNum(	uniquNumber);
					if (tempderch == NULL)
						psPetCurrentInfo->PetKitHP.ItemNum = NULL;
				}
				else
					uniquNumber = NULL;

			}
			break;
			
		case 1:
			{
				if (psPetLevelDataPrev->KitLevelShield)	
				{
					uniquNumber = psPetCurrentInfo->PetKitShield.ItemNum;
					CItemInfo *tempderch = g_pShuttleChild->m_pStoreData->FindItemInInventoryByItemNum(	uniquNumber);
					if (tempderch == NULL)
						psPetCurrentInfo->PetKitShield.ItemNum = NULL;
				}

				else
					uniquNumber = NULL;
			}
			break;
			
		case 2:
			{
				if (psPetLevelDataPrev->KitLevelSP)	
				{
					uniquNumber = psPetCurrentInfo->PetKitSP.ItemNum;
					CItemInfo *tempderch = g_pShuttleChild->m_pStoreData->FindItemInInventoryByItemNum(	uniquNumber);
					if (tempderch == NULL)
						psPetCurrentInfo->PetKitSP.ItemNum = NULL;					
				}
				else
					uniquNumber = NULL;
			}
			break;
		}

	
		if (g_pShuttleChild->GetPetManager()->GetSelectingCheck(j) &&  (uniquNumber == NULL))
		{
// 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
			if (0 ==j && m_nEffStep )
			{
				m_pImgPnUseHp->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3  );
				m_pImgPnUseHp->Render();
			}
			else if (1 == j && m_nEffStep)
			{
				m_pImgPnUseSh->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3   + 35);
				m_pImgPnUseSh->Render();
			}
			else if(m_nEffStep)
			{
				m_pImgPnUseSp->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3   + 70);
				m_pImgPnUseSp->Render();
				
			}
		}
	
// 		if (g_pShuttleChild->GetPetManager()->GetSelectingCheck(j) &&  (uniquNumber == NULL))
// 		{
// 			m_pImgDissolutionTakeEffect[m_nEffStep]->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3   + j * 35);			
// 			m_pImgDissolutionTakeEffect[m_nEffStep]->Render();
// 		}
//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
		
		
		CItemInfo *temp = g_pShuttleChild->m_pStoreData->FindItemInInventoryByItemNum(	uniquNumber);	
		RenderItemSocket(j,temp, FALSE);
	}
	int Gap = NULL;


	for (int i = 0 ; i < 4 ; i++)
	{
		if(psPetLevelDataPrev->KitLevelHP > i)
		{
			Gap = i - 1;
			if (Gap <= 0)
				Gap = 0;
			
			m_pItemSocketHP[i]->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_ITEM_POS_X + i * 14 - Gap , m_ptBkPos.y +PARTNER_SOCKET_KIT_ITEM_POS_Y );
			m_pItemSocketHP[i]->Render();
			
		}
		
		if(psPetLevelDataPrev->KitLevelShield > i)
		{
			Gap = i - 1;
			if (Gap <= 0)
				Gap = 0;
			
			m_pItemSocketShield[i]->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_ITEM_POS_X + i * 14 - Gap , m_ptBkPos.y +PARTNER_SOCKET_KIT_ITEM_POS_Y + 35 );
			m_pItemSocketShield[i]->Render();
			
		}
		
		if(psPetLevelDataPrev->KitLevelSP > i)
		{
			Gap = i - 1;
			if (Gap <= 0)
				Gap = 0;
			
			m_pItemSocketSp[i]->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_ITEM_POS_X + i * 14 - Gap , m_ptBkPos.y +PARTNER_SOCKET_KIT_ITEM_POS_Y + 69 );
			m_pItemSocketSp[i]->Render();
			
		}
	}
}  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void	CINFCharacterInfoExtend::SkillRender()
{

	tPET_CURRENTINFO *pPetCurrentInfo = NULL;
	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;
	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );	
	
	if (!pInvenDisplayInfo)
		return;
	pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData ( pInvenDisplayInfo->pItem->UniqueNumber );
	if (!pPetCurrentInfo)
		return;
	psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData ( pPetCurrentInfo->PetIndex , pPetCurrentInfo->PetLevel );
	if (!psPetLevelDataPrev)
		return;

	for (int i = 0 ; i < 6 ; i++)
	{
		m_pPartnerSkillSocket->Move( m_ptBkPos.x + PARTNER_SOCKET_SKILL_POS_X + i * (PARTNER_SOCKET_SKILL_GAP + m_pPartnerSkillSocket->GetImgSize().x) ,
			m_ptBkPos.y + PARTNER_SOCKET_SKILL_POS_Y );
		m_pPartnerSkillSocket->Render();	

	
		CSkillInfo* tempskill =  g_pShuttleChild->m_pSkill->FindItemSkill(pPetCurrentInfo->PetAutoSkill.ItemNum[i] );
		RenderSkillSocket(i,tempskill, FALSE);

		if (tempskill && g_pShuttleChild->GetPetManager()->GetSocketOnOff() && psPetLevelDataPrev->SlotCountSkill - 1 >= i)
		{
			m_pUseSkillSocket->Move(m_ptBkPos.x + PARTNER_SOCKET_SKILL_POS_X + i * (PARTNER_SOCKET_SKILL_GAP + m_pPartnerSkillSocket->GetImgSize().x) ,
				m_ptBkPos.y + PARTNER_SOCKET_SKILL_POS_Y);			
			m_pUseSkillSocket->Render();	
		}

		if (psPetLevelDataPrev->SlotCountSkill - 1 < i)
		{
			m_pPartnerSocet->Move( m_ptBkPos.x + PARTNER_SOCKET_SKILL_POS_X + i * (PARTNER_SOCKET_SKILL_GAP + m_pPartnerSkillSocket->GetImgSize().x) ,
				m_ptBkPos.y + PARTNER_SOCKET_SKILL_POS_Y);	  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			m_pPartnerSocet->Render();
		}
		
	}
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	g_pGameMain->m_pInfWindow->RenderCenterWindow(m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X, m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y,
												PARTNER_SKILL_SHOP_X, PARTNER_SKILL_SHOP_Y, FALSE);
#else 
	m_pPartnerSkillShop[1]->SetScale(PARTNER_SKILL_POPUPSHOP_X_GAP,1);
	
	m_pPartnerSkillShop[3]->SetScale(1,PARTNER_SKILL_POPUPSHOP_Y_GAP);
	m_pPartnerSkillShop[4]->SetScale(PARTNER_SKILL_POPUPSHOP_X_GAP,PARTNER_SKILL_POPUPSHOP_Y_GAP);
	m_pPartnerSkillShop[5]->SetScale(1,PARTNER_SKILL_POPUPSHOP_Y_GAP);

	m_pPartnerSkillShop[7]->SetScale(PARTNER_SKILL_POPUPSHOP_X_GAP,1);



	m_pPartnerSkillShop[0]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X,m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y);
	m_pPartnerSkillShop[1]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X+ m_pPartnerSkillShop[0]->GetImgSize().x ,m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y);
	m_pPartnerSkillShop[2]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X+ m_pPartnerSkillShop[0]->GetImgSize().x + PARTNER_SKILL_POPUPSHOP_X_GAP, m_ptSkillBkPos.y+ PARTNER_SKILL_POPUPSHOP_Y );

	m_pPartnerSkillShop[3]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X,m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y + m_pPartnerSkillShop[0]->GetImgSize().y);
	m_pPartnerSkillShop[4]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X+ m_pPartnerSkillShop[3]->GetImgSize().x, m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y+ m_pPartnerSkillShop[0]->GetImgSize().y);
	m_pPartnerSkillShop[5]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X+ m_pPartnerSkillShop[3]->GetImgSize().x + PARTNER_SKILL_POPUPSHOP_X_GAP, m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y+ m_pPartnerSkillShop[0]->GetImgSize().y);

	m_pPartnerSkillShop[6]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X,m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y+ m_pPartnerSkillShop[0]->GetImgSize().y + PARTNER_SKILL_POPUPSHOP_Y_GAP );
	m_pPartnerSkillShop[7]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X+ m_pPartnerSkillShop[6]->GetImgSize().x,m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y+ m_pPartnerSkillShop[0]->GetImgSize().y + PARTNER_SKILL_POPUPSHOP_Y_GAP );
	m_pPartnerSkillShop[8]->Move( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X+ m_pPartnerSkillShop[6]->GetImgSize().x + PARTNER_SKILL_POPUPSHOP_X_GAP, m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y+ m_pPartnerSkillShop[0]->GetImgSize().y + PARTNER_SKILL_POPUPSHOP_Y_GAP);

	for (int k = 0 ; k < 9 ; k++)
	{
		m_pPartnerSkillShop[k]->Render();
	}
#endif

	for (int j = 0 ; j < 2 ; j++)
	{
		for (int l = 0 ; l < 4 ;l++)
		{
			m_pPartnerSkillshopSocket->Move( m_ptSkillBkPos.x + (PARTNER_SKILL_POPUPSHOP_X + 19) + l * (PARTNER_SOCKET_SKILL_GAP + 6 + m_pPartnerSkillshopSocket->GetImgSize().x) ,
				m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y + 28 + j * (m_pPartnerSkillshopSocket->GetImgSize().y  + 8 ));		
			m_pPartnerSkillshopSocket->Render();	

			if(m_pPartnerSkilldisplay[(j * 4) + l].pItem)		
			{					
 				CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
 				char buf[20];
				// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
// 				wsprintf(buf, "%08d",SKILL_BASE_NUM(m_pPartnerSkilldisplay[(j * 4) + l].pItem->ItemInfo->SourceIndex));
				wsprintf(buf, "%08d",SKILL_BASE_SOURCEINDEX(m_pPartnerSkilldisplay[(j * 4) + l].pItem->ItemInfo->SourceIndex));
 				pIcon->SetIcon(buf, m_ptSkillBkPos.x + (PARTNER_SKILL_POPUPSHOP_X + 20) + l * (PARTNER_SOCKET_SKILL_GAP + 8 + pIcon->GetIconSize().x) ,
 					m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y + 29 + j * (pIcon->GetIconSize().y + 10) , 1.0f );
 				pIcon->Render();	
  
			}
		}
	}	
}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
											  
// 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void	CINFCharacterInfoExtend::ProductRender()
{

} 
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
// 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void CINFCharacterInfoExtend::SocketOkRender()
{

	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(m_uCheckSocketNum);
	
	if (pItemInfo == FALSE)
		return;
	
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
	char strBuf[256];
	memset(strBuf, 0x00,256);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
	
	
//	strcat( strBuf, pItemInfo->GetItemInfo()->Description );	
	if ( pItemInfo && pItemInfo->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOKIT )
	{
		// 2013-03-26 by bhsohn ��Ʈ�� ������ �޸� ���� ����
//		strcat( strBuf, STRMSG_C_111021_0001 );	
		strncpy( strBuf, STRMSG_C_111021_0001, 255);			
	}
	else if (pItemInfo && pItemInfo->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOSKILL )
	{
		// 2013-03-26 by bhsohn ��Ʈ�� ������ �޸� ���� ����
//		strcat( strBuf, STRMSG_C_111021_0002 );			
		strncpy( strBuf, STRMSG_C_111021_0002, 255);			
 	}
	else if (pItemInfo && pItemInfo->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_SPEED )
	{
		// 2013-03-26 by bhsohn ��Ʈ�� ������ �޸� ���� ����
//		strcat( strBuf, STRMSG_C_120712_0001 );			
		strncpy( strBuf, STRMSG_C_120712_0001, 255);			
 	}
	
	

	
	if(COMPARE_BIT_FLAG(pItemInfo->GetItemInfo()->ItemAttribute, ITEM_ATTR_CASH_ITEM))
	{
		for(int i=0; i<strlen(strBuf); i++)
		{
			if(strBuf[i]=='$')
				strBuf[i] = ' ';
		}
	}
	vector<string> vectempCulling;
#ifdef S_VIE_SERVER_SETTING_HSSON
	::StringCullingUserData(strBuf, 78, &vectempCulling);	
#else
	::StringCullingUserData(strBuf, 67, &vectempCulling);	

#endif
	
	vector<string>::iterator itL = vectempCulling.begin();		
	int LineStr = NULL;
	while(itL != vectempCulling.end())
	{
		char* pLString = (char*)(*itL).c_str();
		itL++;
		LineStr++;
	}
	

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	m_pOkSocket[1]->SetScale(PARTNER_SKILL_POPUPSOCKETOK_X_GAP,1);
	
	m_pOkSocket[3]->SetScale(1,PARTNER_SKILL_POPUPSOCKETOK_Y_GAP + (9 * (LineStr - 3)));
	m_pOkSocket[4]->SetScale(PARTNER_SKILL_POPUPSOCKETOK_X_GAP,PARTNER_SKILL_POPUPSOCKETOK_Y_GAP + (9 * (LineStr - 3)));
	m_pOkSocket[5]->SetScale(1,PARTNER_SKILL_POPUPSOCKETOK_Y_GAP + (9 * (LineStr - 3)));
	
	m_pOkSocket[7]->SetScale(PARTNER_SKILL_POPUPSOCKETOK_X_GAP,1);	
	
	
	m_pOkSocket[0]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X,
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y);
	m_pOkSocket[1]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X+ m_pOkSocket[0]->GetImgSize().x ,
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y);
	m_pOkSocket[2]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X+ m_pOkSocket[0]->GetImgSize().x + PARTNER_SKILL_POPUPSOCKETOK_X_GAP,
		m_ptBkPos.y+ PARTNER_SKILL_POPUPSOCKETOK_Y );
	
	m_pOkSocket[3]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X,
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + m_pOkSocket[0]->GetImgSize().y);
	m_pOkSocket[4]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X+ m_pOkSocket[3]->GetImgSize().x, 
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + m_pOkSocket[0]->GetImgSize().y);
	m_pOkSocket[5]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X+ m_pOkSocket[3]->GetImgSize().x + PARTNER_SKILL_POPUPSOCKETOK_X_GAP,
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + m_pOkSocket[0]->GetImgSize().y);
	
	m_pOkSocket[6]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X,
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y+ PARTNER_SKILL_POPUPSOCKETOK_Y_GAP  + (9 * (LineStr - 3)) );
	m_pOkSocket[7]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X+ m_pOkSocket[6]->GetImgSize().x,
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y+ PARTNER_SKILL_POPUPSOCKETOK_Y_GAP  + (9 * (LineStr - 3)) );
	m_pOkSocket[8]->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X+ m_pOkSocket[6]->GetImgSize().x + PARTNER_SKILL_POPUPSOCKETOK_X_GAP, 
		m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y+ PARTNER_SKILL_POPUPSOCKETOK_Y_GAP  + (9 * (LineStr - 3)) );
#endif
	
	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );	

	if (pInvenDisplayInfo && g_pShuttleChild->GetPetManager()->GetrenderOnOff() )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
		g_pGameMain->m_pInfWindow->RenderCenterWindow(m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X, m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y,
												PARTERN_CHECK_SOCKET_X, PARTERN_CHECK_SOCKET_Y  + (9 * (LineStr - 3)) , FALSE);
#else 
		for (int k = 0 ; k < 9 ; k++)
		{
			m_pOkSocket[k]->Render();
		}		
#endif
		
		m_pBigSlotSocket->Move( m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X + 172 ,m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + 25);
		m_pBigSlotSocket->Render();
	
		itL = vectempCulling.begin();
		LineStr = NULL;
		while(itL != vectempCulling.end())
		{	
			char* pLString = (char*)(*itL).c_str();
			m_SocketDescInfoFont->DrawText(m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X + 19 ,m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + 55 + 34+(LineStr*17),
				GUI_FONT_COLOR,pLString,0L );	
			
			LineStr++;
			itL++;
		}	
		
 		CGameData gameData;
 		gameData.SetFile( ".\\Res-Tex\\bigitem.tex", FALSE, NULL, 0, FALSE );
 		char szName[32];
 		wsprintf(szName, "%08d", pItemInfo->GetItemInfo()->SourceIndex);
 		DataHeader*		m_pDataHeader = gameData.FindFromFile(szName);

		// 2015-06-30 Future, avoided possible NULL-Pointer exception
		if (m_pDataHeader)
		{
			CINFImageEx* m_pBigSlotSocketRender = new CINFImageEx;
			m_pBigSlotSocketRender->InitDeviceObjects(m_pDataHeader);
			m_pBigSlotSocketRender->RestoreDeviceObjects();

			m_pBigSlotSocketRender->Move(m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X + 176, m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + 29);
			m_pBigSlotSocketRender->Render();

			if (m_pPartnerSocketSettingOk)
			{
				// 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
				m_pPartnerSocketSettingOk->SetBtnPosition(m_ptBkPos.x + PARTNER_SKILL_POPUPSOCKETOK_X + 342, m_ptBkPos.y + PARTNER_SKILL_POPUPSOCKETOK_Y + 145 + (13 * (LineStr - 3)));
				//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
				m_pPartnerSocketSettingOk->Render();
			}
			m_pBigSlotSocketRender->InvalidateDeviceObjects();
			m_pBigSlotSocketRender->DeleteDeviceObjects();
		}
	}
} 
 //end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

/****************************************************
**
**	�� ���� ���� ���.
**
**	Create Info :	??. ??. ??.
**	
*****************************************************/
void CINFCharacterInfoExtend::PatnerDtail()
{

	// 2010-06-15 by shcho&hslee ��ý��� - ���¹̳�, ����ġ �� ���� ó�� ����.
	tPET_CURRENTINFO *psPetCurInfo = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;

	GetPtr_AttachmentedPetInfo ( NULL , &psPetCurInfo , NULL , &psPetLevelData );

	if ( !psPetCurInfo || !psPetLevelData )
		return;

	tPET_LEVEL_DATA *psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData ( psPetCurInfo->PetIndex , psPetCurInfo->PetLevel-1 );
	tPET_LEVEL_DATA *psPetLevelDataNext = g_pDatabase->GetPtr_PetLevelData ( psPetCurInfo->PetIndex , psPetCurInfo->PetLevel );         //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	// ���� ����
	/*
	LevelExpBar();
	StaminaBar();
	*/

	Experience_t ExpNow = 0; 
	Experience_t ExpMax = 0;

	if ( psPetLevelDataPrev )
	{
		ExpNow = psPetCurInfo->PetExp - psPetLevelDataPrev->NeedExp;
		ExpMax = psPetLevelDataNext->NeedExp - psPetLevelDataPrev->NeedExp;		   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	}
	else
	{
		ExpNow = psPetCurInfo->PetExp;
		ExpMax = psPetLevelData->NeedExp;
	}

	LevelExpBar( ExpNow , ExpMax );

//	StaminaBar( psPetCurInfo->Stamina , psPetLevelData->Stamina );

	m_pExpScroll->SetOnlyMaxItem( PARTNER_EXP_SCROLL, TRUE );
	m_pExpScroll->Render();

	char szTemp[256] = {0, };
	/*
	char buf[256];
	int tempMaxStamena = 100000;
	int tempStamena = 6000;
	int tempLv = 18;
	float tempMaxExp = 2000.0f;
	float temExp = 240.0f;
	*/
	float fExpPer = .0f;

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;

	// Level & Exp(Per).
	SIZE szSize = {0,0};
	//memset(buf, 0x00, sizeof(buf));

	if ( psPetLevelData->NeedExp > .0f )
		fExpPer = ((float)ExpNow / (float)ExpMax ) * 100.0f;
	
	//sprintf( buf, STRMSG_C_100608_0402, tempLv,(float(temExp / tempMaxExp) * 100));
	//sprintf ( szTemp , STRMSG_C_100608_0402 , psPetCurInfo->PetLevel , fExpPer );
	sprintf ( szTemp , "%.2f%" , fExpPer );				  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	szSize = m_pFontGeneral->GetStringSize(szTemp);
	m_pPartnerLvBar->DrawText(nWindowPosX + PARTNER_DTAIL_INFO_X - szSize.cx, nWindowPosY + PARTNER_DTAIL_INFO_Y , GUI_FONT_COLOR, szTemp, 0L); //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	// Stamina
	/*
	memset(buf, 0x00, sizeof(buf));
	sprintf( buf, STRMSG_C_100608_0403, (int)tempStamena,(int)tempMaxStamena);
	*/
//	sprintf ( szTemp , STRMSG_C_100608_0403 , (DWORD)psPetCurInfo->Stamina , (DWORD)psPetLevelData->Stamina );
//	szSize = m_pFontGeneral->GetStringSize(szTemp);
//	m_pPartnerStaminaBar->DrawText(nWindowPosX + PARTNER_DTAIL_INFO_X - szSize.cx, nWindowPosY + PARTNER_DTAIL_INFO_Y + PARTNER_DTAIL_INFO_Y_GAP, GUI_FONT_COLOR, szTemp, 0L);

}

void CINFCharacterInfoExtend::SetBar(CINFImage* Image1, CINFImage* Image2, float MaxValue, float Value, float x, float y)
{
	// 2010-06-15 by shcho&hslee ��ý��� - ���� ó�� MaxValue == 0.0f ����.
	if ( MaxValue <= .0f )
		MaxValue = 0.1f; //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	RECT ImageRect1, ImageRect2;
	POINT ImageSize1, ImageSize2;
	int fTemp = 0.0f;
	float fValue = (Value / MaxValue) * 100.0f;
	
	ImageSize1 = Image1->GetImgSize();
	ImageSize2 = Image2->GetImgSize();

	if(100.0f > fValue && ImageSize2.x * 2 > ((float)ImageSize1.x * fValue / 100.0f))
	{
		fTemp = (ImageSize2.x * 2 - ((float)ImageSize1.x * (fValue / 100.0f))) / 2;
		ImageRect1.left = 0; 
		ImageRect1.top = 0;
		ImageRect1.right = ImageSize2.x - fTemp;
		ImageRect1.bottom = ImageSize1.y;

		ImageRect2.left = fTemp;
		ImageRect2.top = 0;
		ImageRect2.right = ImageSize2.x;
		ImageRect2.bottom = ImageSize2.y;
	}
	else
	{
		ImageSize1.x = ((float)ImageSize1.x * (fValue / 100.0f));
		ImageRect1.left = 0;
		ImageRect1.top = 0;
		ImageRect1.right = ImageSize1.x - ImageSize2.x;
		ImageRect1.bottom = ImageSize1.y;
		
		ImageRect2.left = 0;
		ImageRect2.top = 0;
		ImageRect2.right = ImageSize2.x;
		ImageRect2.bottom = ImageSize2.y;
	}

	Image1->SetRect(ImageRect1.left, ImageRect1.top, ImageRect1.right, ImageRect1.bottom);

	ImageRect2.left = fTemp;
	ImageRect2.top = 0;
	ImageRect2.right = ImageSize2.x;
	ImageRect2.bottom = ImageSize2.y;
		
	Image2->SetRect(ImageRect2.left, ImageRect2.top, ImageRect2.right, ImageRect2.bottom);

	Image1->Move(x , y);
	Image2->Move(x + ImageRect1.right, y);
}


// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ���� ������ ���� �Ѱ��ֱ�.
void CINFCharacterInfoExtend :: GetPtr_AttachmentedPetInfo ( INVEN_DISPLAY_INFO **o_ppInvenDisplayInfo , tPET_CURRENTINFO **o_ppPetCurInfo , tPET_BASEDATA **o_ppPetBaseData , tPET_LEVEL_DATA **o_ppPetLevelData )
{

	INVEN_DISPLAY_INFO *pInvenDisplayInfo = g_pGameMain->m_pInven->rtnPtr_AttachmentItemInfo ( POS_PET );

	if ( !pInvenDisplayInfo )
		return;

	if ( NULL != o_ppInvenDisplayInfo )
		*o_ppInvenDisplayInfo = pInvenDisplayInfo;

	tPET_CURRENTINFO *pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData ( pInvenDisplayInfo->pItem->UniqueNumber );

	if ( !pPetCurrentInfo )
		return;

	if ( NULL != o_ppPetCurInfo )
		*o_ppPetCurInfo = pPetCurrentInfo;

	tPET_BASE_ALL_DATA *pPetAllData = g_pDatabase->GetPtr_PetAllDataByIndex ( pPetCurrentInfo->PetIndex );

	if ( !pPetAllData )
		return;

	if ( NULL != o_ppPetBaseData )
		*o_ppPetBaseData = &pPetAllData->BaseData;

	if ( NULL != o_ppPetLevelData )
		*o_ppPetLevelData = pPetAllData->rtn_LevelData ( pPetCurrentInfo->PetLevel );

}


/****************************************************************************************
**
**	�� ���¹̳� �� ���.
**
**	Create Info : ??. ??. ??.
**
**	Update Info : 2010. 07. 09. by hsLee. ���¹̳����� ���ڷ� �ް� ����.
**
*****************************************************************************************/
void CINFCharacterInfoExtend::StaminaBar( const int a_StaminaNow , const int a_StaminaMax )
{
	// 2010-06-15 by shcho&hslee ��ý��� - ���¹̳��� ��� ����.
	/*
	int tempMaxStamena = 100000;
	int tempStamena = 6000;

	SetBar(m_pPartnerImageStaminaBar[0], m_pPartnerImageStaminaBar[1], tempMaxStamena, tempStamena, m_ptBkPos.x + PARTNER_STAMENA_POS_X, m_ptBkPos.y + PARTNER_STAMENA_POS_Y);
	*/
	SetBar(m_pPartnerImageStaminaBar[0], m_pPartnerImageStaminaBar[1], (float)a_StaminaMax, (float)a_StaminaNow, m_ptBkPos.x + PARTNER_STAMENA_POS_X, m_ptBkPos.y + PARTNER_STAMENA_POS_Y);
	

	m_pPartnerImageStaminaBar[0]->Render();	
	m_pPartnerImageStaminaBar[1]->Render();
}


/****************************************************************************************
**
**	�� ����ġ �� ���.
**
**	Create Info : ??. ??. ??.
**
**	Update Info : 2010. 07. 09. by hsLee. ����ġ���� ���ڷ� �ް� ����.
**
*****************************************************************************************/
void CINFCharacterInfoExtend::LevelExpBar( const Experience_t a_ExpNow , const Experience_t a_ExpMax )
{
	// 2010-06-15 by shcho&hslee ��ý��� - ����ġ�� ��� ����.
	/*
	float tempMaxExp = 2000.0f;
	float temExp = 240.0f;

	SetBar(m_pPartnerImageLvBar[0], m_pPartnerImageLvBar[1], tempMaxExp, temExp, m_ptBkPos.x + PARTNER_EXP_POS_X, m_ptBkPos.y + PARTNER_EXP_POS_Y);
	*/
	SetBar(m_pPartnerImageLvBar[0], m_pPartnerImageLvBar[1], (float)a_ExpMax, (float)a_ExpNow, m_ptBkPos.x + PARTNER_EXP_POS_X, m_ptBkPos.y + PARTNER_EXP_POS_Y);
	

	m_pPartnerImageLvBar[0]->Render();
//	m_pPartnerImageLvBar[1]->Render();
}

void CINFCharacterInfoExtend::ExpScrollUpdate()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	
	// end 2007-05-15 by bhsohn ��� ���� ���� ó��
	m_pExpScroll->SetPosition( nWindowPosX + PARTNER_EXP_SCROLL_POS_X, nWindowPosY + PARTNER_EXP_SCROLL_POS_Y, PARTNER_EXP_SCROLL_POS_W, PARTNER_EXP_SCROLL_POS_H);
	RECT rect = {
		nWindowPosX + PARTNER_EXP_SCROLL_POS_X,
			nWindowPosY + PARTNER_EXP_SCROLL_POS_Y,
			nWindowPosX + PARTNER_EXP_SCROLL_POS_X + PARTNER_EXP_SCROLL_POS_W,
			nWindowPosY + PARTNER_EXP_SCROLL_POS_Y + 10
	};
	m_pExpScroll->SetMouseBallRect( rect );
}
//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void  CINFCharacterInfoExtend::CheckSocketSetting()
{

}


void CINFCharacterInfoExtend::ItemScrollUpdate()
{

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;

	m_pHpItemScroll->SetPosition( nWindowPosX + PARTNER_ITEM_SCROLL_POS_X, nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y, PARTNER_ITEM_SCROLL_POS_W, PARTNER_ITEM_SCROLL_POS_H);
	RECT Hpitemrect = {
		nWindowPosX + PARTNER_ITEM_SCROLL_POS_X,
			nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y,
			nWindowPosX + PARTNER_ITEM_SCROLL_POS_X + PARTNER_ITEM_SCROLL_POS_W,
			nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 10
	};
	m_pHpItemScroll->SetMouseBallRect( Hpitemrect );

	
	m_pDpItemScroll->SetPosition( nWindowPosX + PARTNER_ITEM_SCROLL_POS_X, nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 34 , PARTNER_ITEM_SCROLL_POS_W, PARTNER_ITEM_SCROLL_POS_H);
	RECT Dpitemrect = {
		nWindowPosX + PARTNER_ITEM_SCROLL_POS_X,
			nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 34,
			nWindowPosX + PARTNER_ITEM_SCROLL_POS_X + PARTNER_ITEM_SCROLL_POS_W,
			nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 10
	};
	m_pDpItemScroll->SetMouseBallRect( Dpitemrect );

	m_pSpItemScroll->SetPosition( nWindowPosX + PARTNER_ITEM_SCROLL_POS_X, nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 34 * 2 , PARTNER_ITEM_SCROLL_POS_W, PARTNER_ITEM_SCROLL_POS_H);
	RECT Spitemrect = {
		nWindowPosX + PARTNER_ITEM_SCROLL_POS_X,
			nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 34 * 2,
			nWindowPosX + PARTNER_ITEM_SCROLL_POS_X + PARTNER_ITEM_SCROLL_POS_W,
			nWindowPosY + PARTNER_ITEM_SCROLL_POS_Y + 10
	};
	m_pSpItemScroll->SetMouseBallRect( Spitemrect );


}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCharacterInfoExtend::RenderSkillInShop()
/// \brief		�������� ���� â�� �� ��ų ������
/// \author		dhkwon
/// \date		2004-10-18 ~ 2004-10-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::RenderSkillInShop()
{
	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
//	if(m_pSkillShopBack )
//	{
//		m_pSkillShopBack->Move(SHOP_SKILL_BACK_START_X,SHOP_SKILL_BACK_START_Y);
//		m_pSkillShopBack->Render() ;
//	}
	// end 2008-08-22 by bhsohn EP3 �κ��丮 ó��
	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	for(int i=0;i<CHAR_UTC_TYPE_NUMBER;i++)
	{
		for(int j=0;j<CHAR_UTC_SLOT_NUMBER;j++)
		{
			if(m_pSkillShopSkillDisplayInfo[i][j].pItem)
			{
				// 2005-11-28 by ispark, SourceIndex�� ����, ��ų�� SKILL_BASE_NUM���� �Ѵ�.
				char buf[64];
				// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//				wsprintf(buf, "%08d", SKILL_BASE_NUM(m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
				wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
				pIcon->SetIcon(buf,
//					SHOP_SKILL_START_X+CHAR_UTC_INTERVAL_X*i+1, 
//					SHOP_SKILL_START_Y+CHAR_UTC_INTERVAL_Y*j+1, 
					SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+1, 
					SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+1, 
					1.0f);
				pIcon->Render();
				
				if(m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemInfo->SkillLevel > 0)
				{
					char buf[128];

					// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
//					wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21,
//										SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+14,
//						GUI_FONT_COLOR_Y,buf, 0L);
					
					{
						// ��ų����
						//"LV%d"
						wsprintf(buf, STRMSG_C_080922_0202, m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
						m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21+13,
#else
						m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21+20,
#endif
							SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j,
							GUI_FONT_COLOR,buf, 0L);
					}
					{
						// ��ų�̸�
						strncpy(buf, m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
						
						// 2008-10-23 by bhsohn ��ų�� �Ѿ�� ���� ó��
						g_pGameMain->TextReduce(m_pFontItemNum[i], 73, buf);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
						m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21+13,
#else			   
						m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21+20,
#endif
							SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+14,
							GUI_FONT_COLOR,buf, 0L);
					}

					// end 2008-08-22 by bhsohn EP3 �κ��丮 ó��

//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
//					wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21,
//										SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+14,
//						GUI_FONT_COLOR_Y,buf, 0L);
//#else
//					wsprintf(buf, "%d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+21,SHOP_SKILL_START_Y+CHAR_UTC_INTERVAL_Y*j+14,
//						GUI_FONT_COLOR_Y,buf, 0L);

//					wsprintf(buf, "%4.d",m_pUTCDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
//					m_pFontItemNum[i]->DrawText(SHOP_SKILL_START_X+CHAR_UTC_INTERVAL_X*i+2,SHOP_SKILL_START_Y+CHAR_UTC_INTERVAL_Y*j+15,
//						GUI_FONT_COLOR_Y,buf, 0L);
//#endif
				}
			}
		}
	}
	if(m_pSelectIcon)
	{
		POINT ptCursor;
		GetCursorPos( &ptCursor );
		ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
		CheckMouseReverse(&ptCursor);

		m_pSelectIcon->Move(ptCursor.x-m_nRenderMoveIconIntervalWidth, ptCursor.y-m_nRenderMoveIconIntervalHeight);
		m_pSelectIcon->Render();

	}
	//m_pShopSkillScrollBar->Render();
	// 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ �������� ���߱�
	m_pShopSkillScrollBar->Render(SCROLL_MOVE_X);
	// end 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ �������� ���߱�
}

void CINFCharacterInfoExtend::Render()
{
	FLOG( "CINFCharacterInfoExtend::Render()" );

	if(!IsShowWnd())
	{
		return;
	}

	// �������� ����
	RenderCommandInfo();
	
	switch(m_nCharacterInfoType)
	{
	case CHARACTER_INFO:
		{
			RenderCharacterInfo();			
		}
		break;

	case CHARACTER_AMMO:
		{
		}
		break;
	case CHARACTER_UTC:
		{
			RenderSkillInfo();
		}		
		break;

	case CHARACTER_ARENA:
		{
			
		}
		break;
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	case CHARACTER_PARTNER:
		{
			RenderPartnerInfo();
		}
		break;
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	}
}

void CINFCharacterInfoExtend::SetAllUTCInfo()
{
	FLOG( "CINFCharacterInfoExtend::SetAllUTCInfo()" );

	m_pShopSkillScrollBar->SetNumberOfData( GetUtcMaxScrollNumber() );
	char strIconName[64];
	int petSkill = NULL;					//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	memset(m_pSkillShopSkillDisplayInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*CHAR_UTC_TYPE_NUMBER*MAX_CHAR_UTC_SLOT_NUMBER);
	memset(m_pCharacterSkillDisplayInfo, 0x00, sizeof(INVEN_DISPLAY_INFO)*CHAR_UTC_TYPE_NUMBER*MAX_CHAR_UTC_SLOT_NUMBER);
	memset(m_pPartnerSkilldisplay, 0x00, sizeof(INVEN_DISPLAY_INFO)*MAX_PARTNER_UTC_SLOT_NUMBER);       //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	

//	map<int, ITEM_SKILL*>::iterator itSkill = g_pShuttleChild->m_pSkill->m_mapEnableSkill.begin();
	map<int, CSkillInfo*>::iterator itSkillinfo = g_pShuttleChild->m_pSkill->m_mapSkill.begin();
	while(itSkillinfo != g_pShuttleChild->m_pSkill->m_mapSkill.end() )
	{
		CSkillInfo* pSkillInfo = itSkillinfo->second;		
		// 2013-03-19 by bhsohn ��ų ��ȣ ���� ����
// 		int nTemp = pSkillInfo->ItemNum / 1000;
// 		int nWindowNum = (pSkillInfo->ItemNum  % nTemp) / 10 -1;
//		int nSkillType = pSkillInfo->ItemNum /1000 - (pSkillInfo->ItemNum /10000)*10;
		//#define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill ����(����: 0, ���: 1, ��Ʈ��: 2, �Ӽ�|����: 3)
		int nBaseNum = max(1, SKILL_BASE_NUM(pSkillInfo->ItemNum));
		int nTemp = max(1, nBaseNum / 1000);
		int nWindowNum = (nBaseNum  % nTemp) / 10 -1;
		int nSkillType = min((CHAR_UTC_TYPE_NUMBER-1), SKILL_KIND_INDEX(pSkillInfo->ItemNum));		
		// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����

		// ��ų���� ���� ����
		if(pSkillInfo->ItemInfo && nWindowNum >= m_nCurrentUTCScroll && nWindowNum < m_nCurrentUTCScroll+CHAR_UTC_SLOT_NUMBER)
		{
//			wsprintf(strIconName,"%08d",SKILL_BASE_NUM(pSkillInfo->ItemNum));	// 2005-08-23 by ispark
			// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//			wsprintf(strIconName,"%08d",SKILL_BASE_NUM(pSkillInfo->ItemInfo->SourceIndex));
			wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(pSkillInfo->ItemInfo->SourceIndex));
			strcpy(m_pSkillShopSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].IconName, strIconName);
			if(pSkillInfo)
			{
				strcpy(m_pSkillShopSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name, pSkillInfo->ItemInfo->ItemName);
			}
			else
			{
				memset(m_pSkillShopSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name,0x00, sizeof(m_pSkillShopSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name));
			}
			m_pSkillShopSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].pItem = pSkillInfo;
		}
		// ĳ������â 
		if(pSkillInfo->ItemInfo && nWindowNum >= m_nCurrentUTCScroll && nWindowNum < m_nCurrentUTCScroll+MAX_CHAR_UTC_SLOT_NUMBER)
		{
//			wsprintf(strIconName,"%08d",SKILL_BASE_NUM(pSkillInfo->ItemNum));	// 2005-08-23 by ispark
			// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//			wsprintf(strIconName,"%08d",SKILL_BASE_NUM(pSkillInfo->ItemInfo->SourceIndex));
			wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(pSkillInfo->ItemInfo->SourceIndex));
			strcpy(m_pCharacterSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].IconName, strIconName);
			if(pSkillInfo)
			{
				strcpy(m_pCharacterSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name, pSkillInfo->ItemInfo->ItemName);
			}
			else
			{
				memset(m_pCharacterSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name,0x00, sizeof(m_pCharacterSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].Name));
			}
			m_pCharacterSkillDisplayInfo[nSkillType][nWindowNum-m_nCurrentUTCScroll].pItem = pSkillInfo;
		}		
		itSkillinfo++;
	}

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	for (int i = 0 ; i< CHAR_UTC_TYPE_NUMBER ; i++)
	{	
		for (int j = 0 ; j< MAX_CHAR_UTC_SLOT_NUMBER ; j++)
		{
			if(IS_BT(g_pShuttleChild->m_myShuttleInfo.UnitKind))				
			{
				if (m_pCharacterSkillDisplayInfo[i][j].pItem &&
					m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ArrDestParameter[4] == DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR  &&
					petSkill < MAX_PARTNER_UTC_SLOT_NUMBER)
				{
					// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//					wsprintf(strIconName,"%08d",SKILL_BASE_NUM(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));					
					wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));					
					strcpy(m_pPartnerSkilldisplay[petSkill].IconName, strIconName);
					
					if(m_pCharacterSkillDisplayInfo[i][j].pItem)
					{
						
						strcpy(m_pPartnerSkilldisplay[petSkill].Name, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName);
					}
					else
					{
						memset(m_pPartnerSkilldisplay[petSkill].Name,0x00, sizeof(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName));
					}
					m_pPartnerSkilldisplay[petSkill].pItem = m_pCharacterSkillDisplayInfo[i][j].pItem;	
					petSkill++;
				}
				
			}
			else if(IS_OT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
			{
				if (m_pCharacterSkillDisplayInfo[i][j].pItem && 
					m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ArrDestParameter[4] == DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR &&
					petSkill < MAX_PARTNER_UTC_SLOT_NUMBER)
				{
					// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//					wsprintf(strIconName,"%08d",SKILL_BASE_NUM(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
					wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
					strcpy(m_pPartnerSkilldisplay[petSkill].IconName, strIconName);
					if(m_pCharacterSkillDisplayInfo[i][j].pItem)
					{
						
						strcpy(m_pPartnerSkilldisplay[petSkill].Name, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName);
					}
					else
					{
						memset(m_pPartnerSkilldisplay[petSkill].Name,0x00, sizeof(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName));
					}					
					m_pPartnerSkilldisplay[petSkill].pItem = m_pCharacterSkillDisplayInfo[i][j].pItem;	
					petSkill++;
				
				}
			}
			else if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
			{
				if (m_pCharacterSkillDisplayInfo[i][j].pItem && 
					m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ArrDestParameter[4] == DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR &&
					petSkill < MAX_PARTNER_UTC_SLOT_NUMBER)
				{
					// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//					wsprintf(strIconName,"%08d",SKILL_BASE_NUM(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
					wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
					strcpy(m_pPartnerSkilldisplay[petSkill].IconName, strIconName);
					if(m_pCharacterSkillDisplayInfo[i][j].pItem)
					{
						
						strcpy(m_pPartnerSkilldisplay[petSkill].Name, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName);
					}
					else
					{
						memset(m_pPartnerSkilldisplay[petSkill].Name,0x00, sizeof(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName));
					}
					m_pPartnerSkilldisplay[petSkill].pItem = m_pCharacterSkillDisplayInfo[i][j].pItem;		
					petSkill++;
					
				}
			}				
			else if(IS_ST(g_pShuttleChild->m_myShuttleInfo.UnitKind))
			{
				if (m_pCharacterSkillDisplayInfo[i][j].pItem && 
					m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ArrDestParameter[4] == DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR &&
					petSkill < MAX_PARTNER_UTC_SLOT_NUMBER)
				{
					// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//					wsprintf(strIconName,"%08d",SKILL_BASE_NUM(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
					wsprintf(strIconName,"%08d",SKILL_BASE_SOURCEINDEX(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
					strcpy(m_pPartnerSkilldisplay[petSkill].IconName, strIconName);
					if(m_pPartnerSkilldisplay[petSkill].pItem)
					{
						
						strcpy(m_pPartnerSkilldisplay[petSkill].Name, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName);
					}
					else
					{
						memset(m_pPartnerSkilldisplay[petSkill].Name,0x00, sizeof(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName));
					}
					m_pPartnerSkilldisplay[petSkill].pItem = m_pCharacterSkillDisplayInfo[i][j].pItem;	
					petSkill++;
				}
			}						
		}		
	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
}

int CINFCharacterInfoExtend::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfoExtend::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
//			POINT pt = MAKEPOINT(lParam);
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			m_pCloseBtn->OnMouseMove(pt);
			m_pOpenBtn->OnMouseMove(pt);	
			// 2009-02-13 by bhsohn ���� ��ŷ �ý���
			if(m_pNickNameBtn)
			{
				m_pNickNameBtn->OnMouseMove(pt);
			}
			// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

			// 2013-03-28 by bhsohn ���� �̸��� �ʹ� �� ��� ����â���� �ʵ� ����
			if( g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0 )
			{
				int nWindowPosX = m_ptBkPos.x;
				int nWindowPosY = m_ptBkPos.y;			
				
				if((nWindowPosX+GUILD_START_X-105 <= pt.x) && (pt.x <= nWindowPosX+GUILD_START_X))
				{
					if((nWindowPosY + GUILD_START_Y <= pt.y) && (pt.y <= nWindowPosY + GUILD_START_Y+20))
					{
						char buff[MAX_PATH] ={0,};
						wsprintf(buff, "%s", g_pShuttleChild->m_myShuttleInfo.GuildName );//"�Ҽ�:%s"											
						
						g_pGameMain->SetToolTip( pt.x, pt.y, buff);//"���ݽ� �������� �������� �ݴϴ�."
						return INF_MSGPROC_BREAK;
					}
				}				
			}
			// END 2013-03-28 by bhsohn ���� �̸��� �ʹ� �� ��� ����â���� �ʵ� ����

			if(m_bMove)
			{
				m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
				m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
				// UI���� ���� 
				UpdateBtnPos();
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;

			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				if(TRUE == m_pOpenBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			// 2009-02-13 by bhsohn ���� ��ŷ �ý���
			if(m_pNickNameBtn)
			{
				
				if(TRUE == m_pNickNameBtn->OnLButtonDown(pt))
				{
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}
			}
			// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
				m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
				m_bMove = TRUE;
				g_pInterface->SetWindowOrder(WNDCharacterWnd);
				return INF_MSGPROC_BREAK;
			}			

			if(pt.y>nWindowPosY + INFO_TAB_POX_Y && pt.y<nWindowPosY + INFO_TAB_POX_Y + INFO_TAB_POX_H)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
				if(pt.x>=nWindowPosX+INFO_TAB_POX_X &&
				   pt.x<nWindowPosX+INFO_TAB_POX_X + INFO_TAB_POX_W / 3)
				{
					m_nCharacterInfoType = CHARACTER_INFO;
				}
				else if(pt.x>nWindowPosX+INFO_TAB_POX_X + (INFO_TAB_POX_W / 3)&&
				   pt.x<nWindowPosX+INFO_TAB_POX_X + 2* (INFO_TAB_POX_W / 3) )
#else			
				if(pt.x>=nWindowPosX+INFO_TAB_POX_X &&
				   pt.x<nWindowPosX+INFO_TAB_POX_X + INFO_TAB_POX_W)
				{
					m_nCharacterInfoType = CHARACTER_INFO;
				}
				else if(pt.x>nWindowPosX+INFO_TAB_POX_X + INFO_TAB_POX_W&&
				   pt.x<nWindowPosX+INFO_TAB_POX_X + 2*INFO_TAB_POX_W)
#endif
				{
					m_nCharacterInfoType = CHARACTER_UTC;							
					
					int nTemp = m_nCurrentUTCScroll;
					// ��ũ�� �ʱ�ȭ
					m_nCurrentUTCScroll = 0;
					if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
					{
						m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
					}				
					
					if(nTemp != m_nCurrentUTCScroll)
					{
						SetAllUTCInfo();
					}
				}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
				else if(pt.x>nWindowPosX+INFO_TAB_POX_X + 2 * (INFO_TAB_POX_W / 3)&&
					pt.x<nWindowPosX+INFO_TAB_POX_X + 3 * (INFO_TAB_POX_W / 3))
#else 
				else if(pt.x>nWindowPosX+INFO_TAB_POX_X + INFO_TAB_POX_W&&
					pt.x<nWindowPosX+INFO_TAB_POX_X + 3*INFO_TAB_POX_W)
#endif
				{
#ifdef SC_GROWING_PARTNER_HSKIM_JHAHN
					m_nCharacterInfoType = CHARACTER_PARTNER;
#endif // SC_GROWING_PARTNER_HSKIM_JHAHN
				}				

			}

			// 2005-08-05 by ispark
			// AMMO â�ȿ� ���콺 Ŭ���� ��ȿ
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			POINT ptBkSize;
			ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
			ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else 
			POINT ptBkSize = m_pInoBk->GetImgSize();
#endif
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+ptBkSize.y&&
				pt.x>nWindowPosX &&
				pt.x<nWindowPosX + ptBkSize.x)
			{
				g_pInterface->SetWindowOrder(WNDCharacterWnd);				
			}		
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			{
				if(TRUE == m_pOpenBtn->OnLButtonUp(pt))
				{			
					// ��ư���� ���콺�� �ִ�.					
					g_pGameMain->m_pCommunity->OnPopupUserOpenSetup(TRUE);
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}

//			POINT pt = MAKEPOINT(lParam);
			{				
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{			
					g_pGameMain->RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
					// ��ư���� ���콺�� �ִ�.										
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			// 2009-02-13 by bhsohn ���� ��ŷ �ý���
			if(m_pNickNameBtn)
			{				
				if(TRUE == m_pNickNameBtn->OnLButtonUp(pt))
				{
					OnClickNickNameBtn();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
					// ��ư���� ���콺�� �ִ�.
					return  INF_MSGPROC_BREAK;
				}
			}
			// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

			if(m_bMove)
			{
				m_bMove = FALSE;
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	}
	switch(m_nCharacterInfoType)
	{
	case CHARACTER_INFO:
		{
			return InfoWndProc( uMsg, wParam, lParam);
		}
		break;
	case CHARACTER_AMMO:
		{
			return AmmoWndProc( uMsg, wParam, lParam);
			
		}
		break;
	case CHARACTER_UTC:
		{
			return UTCWndProc(uMsg, wParam, lParam);
		}
		break;
	case CHARACTER_ARENA:
		{
			return ArenaWndProc(uMsg,wParam,lParam);
		}
		break;
		// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	case CHARACTER_PARTNER:
		{
			return PartnerWndProc(uMsg,wParam,lParam);
		}
		break;
		//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	}
	if(WM_MOUSEMOVE == uMsg)
	{
		int nWindowPosX = m_ptBkPos.x;
		int nWindowPosY = m_ptBkPos.y;

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
		CheckMouseReverse(&pt);
		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		POINT ptBkSize;
		ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
		ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;			  
#else 
		POINT ptBkSize = m_pInoBk->GetImgSize();
#endif
		if(pt.y>nWindowPosY &&
			pt.y<nWindowPosY+ptBkSize.y&&
			pt.x>nWindowPosX &&
			pt.x<nWindowPosX + ptBkSize.x)
		{
			return INF_MSGPROC_BREAK;
		}		
	}

	return INF_MSGPROC_NORMAL;
}


int CINFCharacterInfoExtend::GetCharacterQuestCount()
{
	FLOG( "CINFCharacterInfoExtend::GetCharacterQuestCount()" );
	map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();
	int count = 0;
	while(it != g_pQuestData->m_mapCharacterQuest.end())
	{
		if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			count++;
		}
		it++;
	}
	return count;
}

int CINFCharacterInfoExtend::QuestWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfoExtend::QuestWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	if(m_pQuestListScrollBar)
	{
		if(m_pQuestListScrollBar->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	if(m_pQuestDescScrollBar)
	{
		if(m_pQuestDescScrollBar->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
//			POINT pt = MAKEPOINT(lParam);
//			if(m_bQuestListLock)
//			{
//				int nMove = pt.y - m_ptMouse.y;
//				int count = GetCharacterQuestCount();
//				if(count > 0 && abs(nMove) > CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count))
//				{
//					m_nCurrentQuestListScroll += nMove / CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count);
//					if(m_nCurrentQuestListScroll < 0)
//						m_nCurrentQuestListScroll = 0;
//					if(m_nCurrentQuestListScroll > count)
//						m_nCurrentQuestListScroll = count;
//					m_ptMouse = pt;
//				}
//			}
//			if(m_bQuestDescLock)////// �����ؾ� �Ѵ�.
//			{
//				int nMove = pt.y - m_ptMouse.y;
//				int count = 4;// line number
//				if(abs(nMove) > CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count))
//				{
//					m_nCurrentQuestDescScroll += nMove / CHAR_QUEST_DESC_SCROLL_BAR_INTERVAL(count);
//					if(m_nCurrentQuestDescScroll < 0)
//						m_nCurrentQuestDescScroll = 0;
//					if(m_nCurrentQuestDescScroll > count)
//						m_nCurrentQuestDescScroll = count;
//					m_ptMouse = pt;
//				}
//			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
//			POINT pt = MAKEPOINT(lParam);
//			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int count = GetCharacterQuestCount();
//			if( pt.x > nWindowPosX+CHAR_QUEST_LIST_SCROLL_BAR_START_X &&
//				pt.x < nWindowPosX+CHAR_QUEST_LIST_SCROLL_BAR_START_X+CHAR_SCROLL_BAR_SIZE_X &&
//				pt.y > nWindowPosY+CHAR_QUEST_LIST_SCROLL_BAR_START_Y+CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)*m_nCurrentQuestListScroll &&
//				pt.y < nWindowPosY+CHAR_QUEST_LIST_SCROLL_BAR_START_Y+CHAR_QUEST_LIST_SCROLL_BAR_INTERVAL(count)*m_nCurrentQuestListScroll+CHAR_SCROLL_BAR_SIZE_Y )
//			{
//				m_bQuestListLock = TRUE;
//				m_ptMouse = pt;
//			}
		}
		break;
	case WM_LBUTTONUP:
		{
//			m_bQuestListLock = FALSE;
		}
		break;
//#define CHAR_QUEST_LINE_SIZE_X			169
//#define CHAR_QUEST_LINE_SIZE_Y			16
//#define	CHAR_QUEST_LINE_START_X			19
//#define	CHAR_QUEST_LINE_START_Y			70

	case WM_LBUTTONDBLCLK:
		{
/*			POINT pt = MAKEPOINT(lParam);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
			if( g_pQuestData->m_mapCharacterQuest.size()>0 && 
				pt.x>nWindowPosX+CHAR_QUEST_LINE_START_X && 
				pt.x<nWindowPosX+CHAR_QUEST_LINE_START_X+CHAR_QUEST_LINE_SIZE_X)
			{
				int line = (pt.y-nWindowPosY-CHAR_QUEST_LINE_START_Y)/CHARACTER_QUEST_INTERVAL;
				if(line >=0 && line < 6 && pt.y>nWindowPosY+CHAR_QUEST_LINE_START_Y)
				{
					map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();
					int i = -1;
					while(it != g_pQuestData->m_mapCharacterQuest.end() )
					{
						if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
						{
							i++;
							if(line == i)
								break;
						}
						it++;
					}
					if(it == g_pQuestData->m_mapCharacterQuest.end() )
						return INF_MSGPROC_NORMAL;
					MEX_QUEST_INFO* pInfo = it->second;
					map<int, CQuest*>::iterator it2 = g_pQuestData->m_mapQuest.find(pInfo->QuestIndex);
					if(it2 != g_pQuestData->m_mapQuest.end())
					{
						if(it2->second->QuestEndType == QUEST_END_TYPE_IMMEDIATE)//Ŭ���� ��� ���� ������
						{
							MSG_FC_QUEST_ACCEPT_QUEST sMsg;
							memset(&sMsg,0x00,sizeof(sMsg));
							char buffer[SIZE_MAX_PACKET];
							sMsg.QuestIndex = pInfo->QuestIndex;
							int nType = T_FC_QUEST_ACCEPT_QUEST;
							memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
							memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
							g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
						}
						else if(it2->second->QuestEndType == QUEST_END_TYPE_REQESTED_BY_CLIENT)
						{// �������� Ŭ���̾�Ʈ ����Ʈ�� �߰� ��ȭ�� �����ش�.
							g_pGameMain->m_pMission->SetQuest(it2->second,pInfo);
						}
					}
				}
			}
*/		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFCharacterInfoExtend::WndProcSkillInShop(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int bReturn = m_pShopSkillScrollBar->WndProc( uMsg, wParam, lParam );
	if( m_nCurrentUTCScroll != m_pShopSkillScrollBar->GetCurrentScrollIndex())
	{
		m_nCurrentUTCScroll = m_pShopSkillScrollBar->GetCurrentScrollIndex();
		SetAllUTCInfo();
	}
	if( bReturn == INF_MSGPROC_BREAK )
	{
		return INF_MSGPROC_BREAK;
	}

	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.x>SHOP_SKILL_START_X && 
				pt.x<SHOP_SKILL_START_X+SHOP_SKILL_BACK_SIZE_X &&
				pt.y>SHOP_SKILL_START_Y && 
				pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_BACK_SIZE_Y)
			{
				int i = ( pt.x - SHOP_SKILL_START_X ) / SHOP_SKILL_INTERVAL_X;
				if( i>=0 && 
					i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - SHOP_SKILL_START_Y) / SHOP_SKILL_INTERVAL_Y;
					if( j >=0 && 
						j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE &&
						m_pSkillShopSkillDisplayInfo[i][j].pItem)
					{
						((CINFGameMain*)m_pParent)->SetItemInfo(m_pSkillShopSkillDisplayInfo[i][j].pItem->UniqueNumber, 
																m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemNum, pt.x, pt.y);
					}
					else
						((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
				}
				else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
			}
//			else
//			{
//				((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
//			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.x>SHOP_SKILL_START_X && 
				pt.x<SHOP_SKILL_START_X+SHOP_SKILL_BACK_SIZE_X &&
				pt.y>SHOP_SKILL_START_Y && 
				pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_BACK_SIZE_Y)
			{
				int i = ( pt.x - SHOP_SKILL_START_X ) / SHOP_SKILL_INTERVAL_X;
				if( i>=0 && 
					i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - SHOP_SKILL_START_Y) / SHOP_SKILL_INTERVAL_Y;
					if( j >=0 && 
						j<CHAR_UTC_SLOT_NUMBER &&
						pt.y<SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE )
					{

						m_nRenderMoveIconIntervalWidth =  pt.x - (SHOP_SKILL_START_X+SHOP_SKILL_INTERVAL_X*i);
						m_nRenderMoveIconIntervalHeight = pt.y - (SHOP_SKILL_START_Y+SHOP_SKILL_INTERVAL_Y*j);

						if(	m_pSkillShopSkillDisplayInfo[i][j].pItem &&
							m_pSkillShopSkillDisplayInfo[i][j].pItem->ItemInfo->SkillType != SKILLTYPE_PERMANENT)	
						{
							SetSelectUTC( &m_pSkillShopSkillDisplayInfo[i][j] );
						}
					}
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if( m_pSelectIcon &&
				m_pSelectSkill &&
				pt.x>SKILL_SHOP_SPI_START_X+1 && 
				pt.x<SKILL_SHOP_SPI_START_X+1+SKILL_DROP_SIZE &&
				pt.y>SKILL_SHOP_SPI_START_Y+1 && 
				pt.y<SKILL_SHOP_SPI_START_Y+1+SKILL_DROP_SIZE)
			{
				if( !((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE) &&
					!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_ITEM_DELETE_NUM))
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRERR_C_SKILL_0012,COLOR_SKILL_USE);//"��ų ������ ���� �������� �ʽ��ϴ�."
//					char buf[256];
//					ITEM *pITEM = g_pDatabase->GetServerItemInfo(m_pSelectSkill->ItemNum);
//					if(pITEM)
//					{
//						m_pDeleteSkill = m_pSelectSkill;
//						wsprintf(buf, "��ų %s ��(��)  �����Ͻðڽ��ϱ�?", pITEM->ItemName);
//						g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_ITEM_DELETE_SKILL);
//					}
				}
			}
			SetSelectUTC(NULL);
			((CINFGameMain*)m_pParent)->SetItemInfo(0,0, 0,0);
		}
		break;

	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCharacterInfoExtend::SendDeleteSelectItemSkill()
/// \brief		���� �۵����� ����
/// \author		dhkwon
/// \date		2004-10-18 ~ 2004-10-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::SendDeleteSelectItemSkill()
{
	ASSERT_ASSERT(m_pDeleteSkill);
	MSG_FC_ITEM_THROW_AWAY_ITEM sMsg;
	sMsg.Amount = 1;
	sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.ItemUniqueNumber = m_pDeleteSkill->UniqueNumber;
	g_pFieldWinSocket->SendMsg( T_FC_ITEM_THROW_AWAY_ITEM, (char*)&sMsg, sizeof(sMsg) );
	m_pDeleteSkill = NULL;
}

int CINFCharacterInfoExtend::UTCWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfoExtend::UTCWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient( g_pD3dApp->GetHwnd(), &pt );
			CheckMouseReverse(&pt);

			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;
			
			if( pt.x>CHAR_UTC_START_X && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*MAX_CHAR_UTC_SLOT_NUMBER)
			{

				int nTemp = m_nCurrentUTCScroll;

				if((int)wParam > 0)
				{
					m_nCurrentUTCScroll--;
					if(m_nCurrentUTCScroll<0)
						m_nCurrentUTCScroll = 0;
					else if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
						m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
				}
				else
				{
					m_nCurrentUTCScroll++;
					if(m_nCurrentUTCScroll<0)
						m_nCurrentUTCScroll = 0;
					else if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
						m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
				}

				if(nTemp != m_nCurrentUTCScroll)
						SetAllUTCInfo();
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_bLbuttonDown = FALSE;
			
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;

			if(m_bUTCScrollLock)
			{
				int nMove = (int)(pt.y-m_ptMouse.y);
				if(abs(nMove)>UTC_SCROLL_INTERVAL)
				{
					int nTemp = m_nCurrentUTCScroll;
					m_ptMouse = pt;
					int max = UTC_SCROLL_INTERVAL;
					if(max>0)
					{
						m_nCurrentUTCScroll += nMove/max;
						if(m_nCurrentUTCScroll<0)
							m_nCurrentUTCScroll = 0;
						else if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
							m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
						if(nTemp != m_nCurrentUTCScroll)
							SetAllUTCInfo();
					}
				}
			}
			if( pt.x>CHAR_UTC_START_X-20 && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER+20 &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*MAX_CHAR_UTC_SLOT_NUMBER+20)
			{
				int i = ( pt.x - CHAR_UTC_START_X ) / CHAR_UTC_INTERVAL_X;
				if( i>=0 && 
					i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - CHAR_UTC_START_Y) / CHAR_UTC_INTERVAL_Y;
					if( j >=0 && 
						j<MAX_CHAR_UTC_SLOT_NUMBER &&
						pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE &&
						m_pCharacterSkillDisplayInfo[i][j].pItem)
					{
						//((CINFGameMain*)m_pParent)->SetToolTip(pt.x, pt.y, m_pCharacterSkillDisplayInfo[i][j].Name );
						((CINFGameMain*)m_pParent)->SetItemInfo(m_pCharacterSkillDisplayInfo[i][j].pItem->UniqueNumber, 
																m_pCharacterSkillDisplayInfo[i][j].pItem->ItemNum, pt.x, pt.y);
					}
					else
						((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
						//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
				}
				else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
					//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			}
			else
			{
				((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
			}
				//((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);
			{					
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����		
				POINT ptBkSize;
				ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
				ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else 
				POINT ptBkSize = m_pInoBk->GetImgSize();
#endif
				if(pt.y>nWindowPosY &&
					pt.y<nWindowPosY+ptBkSize.y&&
					pt.x>nWindowPosX &&
					pt.x<nWindowPosX + ptBkSize.x)
				{
					return INF_MSGPROC_BREAK;
				}		
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;			

			if( pt.x>CHAR_UTC_START_X && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*MAX_CHAR_UTC_SLOT_NUMBER)
			{
				int i = ( pt.x - CHAR_UTC_START_X ) / CHAR_UTC_INTERVAL_X;
				if( i>=0 && i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - CHAR_UTC_START_Y) / CHAR_UTC_INTERVAL_Y;
					if( j >=0 && j<MAX_CHAR_UTC_SLOT_NUMBER &&
						pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE)
					{

						m_nRenderMoveIconIntervalWidth =  pt.x - (CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i);
						m_nRenderMoveIconIntervalHeight = pt.y - (CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j);

						if(	m_pCharacterSkillDisplayInfo[i][j].pItem &&
							m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SkillType != SKILLTYPE_PERMANENT)	
						{
							SetSelectUTC( &m_pCharacterSkillDisplayInfo[i][j] );
							m_bLbuttonDown = TRUE;
							//((CINFGameMain*)m_pParent)->SetItemInfo(m_pCharacterSkillDisplayInfo[i][j].pItem->UniqueNumber, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemNum, pt.x, pt.y);
							return INF_MSGPROC_BREAK;
						}
					}
				}
			}
			if( pt.x>UTC_SCROLL_BUTTON_START_X && pt.x<UTC_SCROLL_BUTTON_START_X+UTC_SCROLL_BAR_SIZE_X &&
				pt.y>nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL && pt.y<nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL+UTC_SCROLL_BAR_SIZE_Y)
			{
				m_bUTCScrollLock =	TRUE;
				m_ptMouse = pt;

			}

			// 2005-08-02 by ispark
			// â�ȿ� ���콺 Ŭ���� ��ȿ
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
			POINT ptBkSize;
			ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
			ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else 
			POINT ptBkSize = m_pInoBk->GetImgSize();
#endif
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+ptBkSize.y&&
				pt.x>nWindowPosX &&
				pt.x<nWindowPosX + ptBkSize.x)
			{
				return INF_MSGPROC_BREAK;
			}		

			// 2006-07-27 by ispark, ���� �������� ������ �ִٸ�
			if(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && 
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_SKILL_POS)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			//			POINT pt = MAKEPOINT(lParam);
			if(m_bLbuttonDown)
			{
				m_bLbuttonDown = FALSE;
			}
			else
			{
				// 2005-12-01 by ispark, �����Կ��� LBUTTONUP�� ��
				if(((CINFGameMain*)m_pParent)->m_pQuickSlot->LButtonUpQuickSlot(pt))
				{
					// �����Կ����� SelectItem ���� ����
					break;
				}
				else if(((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_SKILL_POS)
				{
					SetSelectUTC(NULL);
				}
			}

			((CINFGameMain*)m_pParent)->SetItemInfo(0,0, 0,0);

			m_bUTCScrollLock = FALSE;
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;
			
			if( pt.x>CHAR_UTC_START_X && pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*CHAR_UTC_TYPE_NUMBER &&
				pt.y>CHAR_UTC_START_Y && pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*MAX_CHAR_UTC_SLOT_NUMBER)
			{
				int i = ( pt.x - CHAR_UTC_START_X ) / CHAR_UTC_INTERVAL_X;
				if( i>=0 && i<CHAR_UTC_TYPE_NUMBER &&
					pt.x<CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+CHAR_UTC_ICON_SIZE)
				{
					int j = (pt.y - CHAR_UTC_START_Y) / CHAR_UTC_INTERVAL_Y;
					if( j >=0 && j<MAX_CHAR_UTC_SLOT_NUMBER &&
						pt.y<CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+CHAR_UTC_ICON_SIZE)
					{
						m_nRenderMoveIconIntervalWidth =  pt.x - (CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i);
						m_nRenderMoveIconIntervalHeight = pt.y - (CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j);

						if(	m_pCharacterSkillDisplayInfo[i][j].pItem &&
							m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SkillType != SKILLTYPE_PERMANENT)	
						{
							SendUseSkill(m_pCharacterSkillDisplayInfo[i][j].pItem);	
						}
					}
				}
			}
			if( pt.x>UTC_SCROLL_BUTTON_START_X && pt.x<UTC_SCROLL_BUTTON_START_X+UTC_SCROLL_BAR_SIZE_X &&
				pt.y>nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL && pt.y<nWindowPosY+UTC_SCROLL_BAR_START_Y+m_nCurrentUTCScroll*UTC_SCROLL_INTERVAL+UTC_SCROLL_BAR_SIZE_Y)
			{
//				m_bUTCScrollLock =	TRUE;
				m_ptMouse = pt;
			}

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
int CINFCharacterInfoExtend::ArenaWndProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			POINT ptBkSize;
			ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
			ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;		  
#else 
			POINT ptBkSize = m_pInoBk->GetImgSize();
#endif

			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+ptBkSize.y&&
				pt.x>nWindowPosX &&
				pt.x<nWindowPosX + ptBkSize.x)
			{
				return INF_MSGPROC_BREAK;
			}		
		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}

// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
int CINFCharacterInfoExtend::PartnerWndProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;

	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	tPET_CURRENTINFO *psPetCurrentInfo = NULL;
	tPET_BASEDATA *psPetBaseData = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;
	tPET_LEVEL_DATA *psPetLevelDataPrev = NULL;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	
	GetPtr_AttachmentedPetInfo ( NULL , &psPetCurrentInfo , &psPetBaseData , &psPetLevelData );

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			if(m_pChangeNameBtn && psPetBaseData && (psPetBaseData->EnableName))
				m_pChangeNameBtn->OnMouseMove(pt);
			if (m_pPartnerSocketSettingOk)					   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				m_pPartnerSocketSettingOk->OnMouseMove(pt);	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

			if( m_pExpScroll->GetMouseMoveMode() )
			{
				m_pExpScroll->SetScrollPos( pt, TRUE);
				return INF_MSGPROC_BREAK;
			}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if( m_pHpItemScroll->GetMouseMoveMode() )
			{
				m_pHpItemScroll->SetScrollPos( pt, TRUE);
				return INF_MSGPROC_BREAK;
			}
			if( m_pDpItemScroll->GetMouseMoveMode() )
			{
				m_pDpItemScroll->SetScrollPos( pt, TRUE);
				return INF_MSGPROC_BREAK;
			}
			if( m_pSpItemScroll->GetMouseMoveMode() )
			{
				m_pSpItemScroll->SetScrollPos( pt, TRUE);
				return INF_MSGPROC_BREAK;
			}				
			{		
				
		
				int k = (pt.x - ( m_ptSkillBkPos.x + (PARTNER_SKILL_POPUPSHOP_X + 20))) / (30 + PARTNER_SOCKET_SKILL_GAP - 10  + 20);
				int j = (pt.y - ( m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y + 29)) / (30 + 20);
				
				if((pt.x > m_ptSkillBkPos.x  + PARTNER_SKILL_POPUPSHOP_X + 10 + k * (30 + PARTNER_SOCKET_SKILL_GAP - 10 + 20 ) && 
					pt.x <= m_ptSkillBkPos.x  + PARTNER_SKILL_POPUPSHOP_X +10 +  30 + k * (30 + PARTNER_SOCKET_SKILL_GAP - 10 + 20) && 
					pt.y > m_ptSkillBkPos.y  + PARTNER_SKILL_POPUPSHOP_Y + 20 + j * (30 + 30 ) && pt.y < m_ptSkillBkPos.y  + PARTNER_SKILL_POPUPSHOP_Y +20 + 30 + j * (30 + 30  ) )	)
				{	
					tPET_CURRENTINFO *tempCurrentInfo = NULL;
					tPET_BASEDATA *psPetBaseData = NULL;
					tPET_LEVEL_DATA *psPetLevelData = NULL;
					
					GetPtr_AttachmentedPetInfo ( NULL , &tempCurrentInfo , &psPetBaseData , &psPetLevelData );

					if( (k >= 0) && (k < 4) && (j >= 0) && (j < 2) && tempCurrentInfo && m_pPartnerSkilldisplay[j * 4 + k].pItem && (g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_SKILL)  )
					{
						((CINFGameMain*)m_pParent)->SetItemInfo(m_pPartnerSkilldisplay[j * 4 + k].pItem->UniqueNumber, m_pPartnerSkilldisplay[j * 4 + k].pItem->ItemNum, pt.x, pt.y);
						return INF_MSGPROC_BREAK;
					}

					else
						((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);			
				}
				else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
			}

			{				
				int j = (pt.y - (nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y)) / 30;
				if(pt.x > nWindowPosX + PARTNER_SOCKET_ITEM_POS_X && pt.x < nWindowPosX + PARTNER_SOCKET_ITEM_POS_X + 30  &&
					pt.y > nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + j * PARTNER_SOCKET_ITEM_POS_GAP && pt.y < nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + 30 + j * PARTNER_SOCKET_ITEM_POS_GAP)
				{
					CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	
					if (pAllItem)
					{
						POINT temp;
						temp.x=temp.y=0;
						tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );			
						if (tempCurrentInfo)
						{
							CItemInfo* SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetSocketItemUID[j]);

							if (SoketItem)
							{
								char strIconName[64];
								sprintf(strIconName, "%08d", SoketItem->GetItemInfo()->SourceIndex);
								INVEN_DISPLAY_INFO* SocketItemDisplay = new INVEN_DISPLAY_INFO;
								strcpy(SocketItemDisplay->IconName, strIconName);
								strcpy(SocketItemDisplay->Name, SoketItem->GetItemInfo()->ItemName);
								SocketItemDisplay->pItem = SoketItem;

								((CINFGameMain*)m_pParent)->SetItemInfo(SocketItemDisplay->pItem->UniqueNumber, SocketItemDisplay->pItem->ItemNum, pt.x, pt.y);
								return INF_MSGPROC_BREAK;
							}
							else
								((CINFGameMain*)m_pParent)->SetItemInfo(0, 0, 0, 0);
						}
						else
						{
							DBGOUT("CRITICAL: Pet not found in the client! Store UID %d", pAllItem->UniqueNumber);
							((CINFGameMain*)m_pParent)->SetItemInfo(0, 0, 0, 0);
						}
					}	
					else
						((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);

				}

			}

			{				
				int j = (pt.y - ( nWindowPosY + PARTNER_SOCKET_KIT_POS_Y )) / 30;
				if((pt.x > nWindowPosX + PARTNER_SOCKET_KIT_POS_X && 
					pt.x <= nWindowPosX + PARTNER_SOCKET_KIT_POS_X + 30 && 
					pt.y > nWindowPosY + PARTNER_SOCKET_KIT_POS_Y + j * PARTNER_SOCKET_KIT_POS_GAP
					&& pt.y < nWindowPosY + PARTNER_SOCKET_KIT_POS_Y + 30 + j * PARTNER_SOCKET_KIT_POS_GAP ))
				{
					tPET_CURRENTINFO *tempCurrentInfo = NULL;
					tPET_BASEDATA *psPetBaseData = NULL;
					tPET_LEVEL_DATA *psPetLevelData = NULL;
					
					GetPtr_AttachmentedPetInfo ( NULL , &tempCurrentInfo , &psPetBaseData , &psPetLevelData );

					CItemInfo* SoketItem = NULL;
					if (j == 0 && tempCurrentInfo)
					{
						SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetKitHP.ItemNum ); 
					}
					else if (j == 1 && tempCurrentInfo)
					{
						SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetKitShield.ItemNum ); 
					}
					else if (j == 2 && tempCurrentInfo)
					{
						SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetKitHP.ItemNum ); 
					}
					
					if(tempCurrentInfo && SoketItem)
					{						
					
						char strIconName[64];	
						sprintf(strIconName,"%08d",SoketItem->GetItemInfo()->SourceIndex);						
						INVEN_DISPLAY_INFO* SocketKitDisplay = new INVEN_DISPLAY_INFO;
						strcpy(SocketKitDisplay->IconName, strIconName);
						strcpy(SocketKitDisplay->Name, SoketItem->GetItemInfo()->ItemName);						
						SocketKitDisplay->pItem = SoketItem;	
						((CINFGameMain*)m_pParent)->SetItemInfo(SocketKitDisplay->pItem->UniqueNumber, SocketKitDisplay->pItem->ItemNum, pt.x, pt.y);								
							return INF_MSGPROC_BREAK;
					
					}
					else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
				
				}
				else
					((CINFGameMain*)m_pParent)->SetItemInfo(0,0,0,0);
			}			

			if(m_bMoveSkill)
			{			
				m_ptSkillBkPos.x = pt.x - m_ptSkillCommOpMouse.x;
				m_ptSkillBkPos.y = pt.y - m_ptSkillCommOpMouse.y;
				// UI���� ���� 
				UpdateBtnPos();
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:		
		{				
			int j = (pt.y - (nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y)) / 30;
			if(pt.x > nWindowPosX + PARTNER_SOCKET_ITEM_POS_X && pt.x < nWindowPosX + PARTNER_SOCKET_ITEM_POS_X + 30  &&
				pt.y > nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + j * PARTNER_SOCKET_ITEM_POS_GAP && pt.y < nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + 30 + j * PARTNER_SOCKET_ITEM_POS_GAP)
			{
				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	
				if (pAllItem)
				{
					POINT temp;
					temp.x=temp.y=0;
					tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );

					if (tempCurrentInfo)
					{
						CItemInfo* SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetSocketItemUID[j]);

						if (SoketItem)
						{
							if (g_pShuttleChild->GetPetManager()->GetSocketOnOff())
							{
								MSG_FC_ITEM_CANCEL_PET_SOCKET_ITEM sMsg;
								sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
								sMsg.ItemUniqueNumber = SoketItem->UniqueNumber;
								g_pFieldWinSocket->SendMsg(T_FC_ITEM_CANCEL_PET_SOCKET_ITEM, (char*)&sMsg, sizeof(sMsg));
								g_pShuttleChild->GetPetManager()->SetSocketOnOff(FALSE);

							}
							else
							{
								MSG_FC_ITEM_USE_PET_SOCKET_ITEM sMsg;
								sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
								sMsg.ItemUniqueNumber = SoketItem->UniqueNumber;
								g_pFieldWinSocket->SendMsg(T_FC_ITEM_USE_PET_SOCKET_ITEM, (char*)&sMsg, sizeof(sMsg));
								g_pShuttleChild->GetPetManager()->SetSocketOnOff(TRUE);
							}
						}
					}		
				}					
			}			
		}
		break;	
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	case WM_LBUTTONDOWN:
		{
			m_bLbuttonDown = TRUE;					  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

			if(m_pChangeNameBtn && psPetBaseData && (psPetBaseData->EnableName))
				m_pChangeNameBtn->OnLButtonDown(pt);

			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if(TRUE == m_pPartnerSocketSettingOk->OnLButtonDown(pt))
			{					
				return  INF_MSGPROC_BREAK;
			}		
			//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

			if( m_pExpScroll->IsMouseBallPos( pt ) )
			{
				m_pExpScroll->SetMouseMoveMode( TRUE );
				return INF_MSGPROC_BREAK;
			}
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if( m_pHpItemScroll->IsMouseBallPos(pt) )
			{
				m_pHpItemScroll->SetMouseMoveMode( TRUE);
				return INF_MSGPROC_BREAK;
			}
			if( m_pDpItemScroll->IsMouseBallPos(pt) )
			{
				m_pDpItemScroll->SetMouseMoveMode( TRUE);
				return INF_MSGPROC_BREAK;
			}
			if( m_pSpItemScroll->IsMouseBallPos(pt) )
			{
				m_pSpItemScroll->SetMouseMoveMode( TRUE);
				return INF_MSGPROC_BREAK;
			}


			
			{
				//	pIcon->SetIcon(buf, 
				//		m_ptSkillBkPos.x + (PARTNER_SKILL_POPUPSHOP_X + 20) + l * (PARTNER_SOCKET_SKILL_GAP + 8 + pIcon->GetIconSize().x) ,
				//		m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y + 29 + j * (pIcon->GetIconSize().y + 10) , 1.0f );


				int k = (pt.x - ( m_ptSkillBkPos.x + PARTNER_SKILL_POPUPSHOP_X +20)) / (PARTNER_SOCKET_SKILL_GAP + 8 + 30);
				int j = (pt.y - ( m_ptSkillBkPos.y + PARTNER_SKILL_POPUPSHOP_Y +29)) / (30 + 10);

				if((pt.x > m_ptSkillBkPos.x  + PARTNER_SKILL_POPUPSHOP_X + 20 + k * (30 + PARTNER_SOCKET_SKILL_GAP + 8 ) && 
					pt.x <= m_ptSkillBkPos.x  + PARTNER_SKILL_POPUPSHOP_X +20 +  30 + k * (30 + PARTNER_SOCKET_SKILL_GAP + 8 ) && 
					pt.y > m_ptSkillBkPos.y  + PARTNER_SKILL_POPUPSHOP_Y + 29 + j * (30 + 10 ) &&
					pt.y < m_ptSkillBkPos.y  + PARTNER_SKILL_POPUPSHOP_Y +29 + 30 + j * (30 + 10  ) )&& 
					(g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_SKILL))
				{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
					if ((k >= 0) && (k < 4) && (j >= 0) && (j < 2) && &m_pPartnerSkilldisplay[j * 4 + k] && m_pPartnerSkilldisplay[j * 4 + k].pItem)
#else 
					if ((k >= 0) && (k < 4) && (j >= 0) && (j < 2) )
#endif
					{
						POINT temp;
						temp.x=temp.y=0;
						((CINFGameMain*)m_pParent)->SetSelectItem(&m_pPartnerSkilldisplay[j * 4 + k], temp, ITEM_SKILL_POS);
						m_bLbuttonDown = TRUE;
						return INF_MSGPROC_BREAK;
					}
					
				}
				
			}
			// 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����
			{
				int k = (pt.x - ( nWindowPosX + PARTNER_SOCKET_SKILL_POS_X)) / (30 + PARTNER_SOCKET_SKILL_GAP + 20);
				if((pt.x > nWindowPosX + PARTNER_SOCKET_SKILL_POS_X + k * (30 + PARTNER_SOCKET_SKILL_GAP + 20 ) && 
					pt.x <= nWindowPosX + PARTNER_SOCKET_SKILL_POS_X + 30 + k * (30 + PARTNER_SOCKET_SKILL_GAP + 20) && 
					pt.y > nWindowPosY + PARTNER_SOCKET_SKILL_POS_Y  + 6 && 
					pt.y < nWindowPosY + PARTNER_SOCKET_SKILL_POS_Y + 6 + 30 )&& (g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_SKILL))
				{
					POINT temp;
					temp.x=temp.y=0;

					CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 		
					if(pAllItem)
					{
						tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );
						if(tempCurrentInfo)
						{
							CSkillInfo* tempskill =  g_pShuttleChild->m_pSkill->FindItemSkill(tempCurrentInfo->PetAutoSkill.ItemNum[k] );
							if(tempskill)
							{
								INVEN_DISPLAY_INFO*	ptempItemDisplayInfo = NULL;
								ITEM_BASE* pItem = tempskill;
								for(int i = 0 ; i < MAX_PARTNER_UTC_SLOT_NUMBER ; i++)
								{
									if(pItem == m_pPartnerSkilldisplay[i].pItem)
									{
										ptempItemDisplayInfo = &m_pPartnerSkilldisplay[i];
										break;
									}
								}
								((CINFGameMain*)m_pParent)->SetSelectItem(ptempItemDisplayInfo, temp, ITEM_SKILL_POS);
							}
							m_bLbuttonDown = TRUE;
							SetSkillSoket(k,NULL,0);				
							return INF_MSGPROC_BREAK;
							}
						}
					}
			}
			// end 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����
			
			{
				int j = (pt.y - (nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y)) / 30;
				if(pt.x > nWindowPosX + PARTNER_SOCKET_ITEM_POS_X &&
					pt.x < nWindowPosX + PARTNER_SOCKET_ITEM_POS_X + 30  &&
					pt.y > nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + j * PARTNER_SOCKET_ITEM_POS_GAP &&
					pt.y < nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + 30 + j * PARTNER_SOCKET_ITEM_POS_GAP)
				{
					CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 	
					if (pAllItem)
					{
						POINT ptIconPos;
						
						ptIconPos.x = pt.x - (nWindowPosX + PARTNER_SOCKET_ITEM_POS_X );
						ptIconPos.y = pt.y - (nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + (PARTNER_SOCKET_ITEM_POS_GAP*j));

						tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );	
						if (tempCurrentInfo)
						{
							CItemInfo* SoketItem = g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetSocketItemUID[j]);

							if (SoketItem)
							{
								char strIconName[64];
								sprintf(strIconName, "%08d", SoketItem->GetItemInfo()->SourceIndex);
								INVEN_DISPLAY_INFO* SocketItemDisplay = new INVEN_DISPLAY_INFO;
								strcpy(SocketItemDisplay->IconName, strIconName);
								strcpy(SocketItemDisplay->Name, SoketItem->GetItemInfo()->ItemName);
								SocketItemDisplay->pItem = SoketItem;

								((CINFGameMain*)m_pParent)->SetSelectItem(SocketItemDisplay, ptIconPos, ITEM_SKILL_POS);
							}
						}
					}					
				}
			}	
			
			if(IsMouseCaps2(pt))
			{

				m_ptSkillCommOpMouse.x = pt.x - m_ptSkillBkPos.x;
				m_ptSkillCommOpMouse.y = pt.y - m_ptSkillBkPos.y;  	
				m_bMoveSkill = TRUE;
				return INF_MSGPROC_BREAK;
			}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			POINT ptBkSize;
			ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
			ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else 
			POINT ptBkSize = m_pInoBk->GetImgSize();
#endif
			
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+ptBkSize.y&&
				pt.x>nWindowPosX &&
				pt.x<nWindowPosX + ptBkSize.x)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	case WM_LBUTTONUP:
		{
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if(m_bLbuttonDown)
			{		
				m_bLbuttonDown = FALSE;
			}

			if(m_bMoveSkill)
			{
				m_bMoveSkill = FALSE;
				return INF_MSGPROC_BREAK;
			}
			//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if(m_pChangeNameBtn->OnLButtonUp(pt) && psPetBaseData && (psPetBaseData->EnableName))
			{
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_100608_0408, _Q_PARTNER_NEW_NAME_CHANGE );//"��Ʈ�� �̸��� �Է��ϼ���"
			}
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if(TRUE == m_pPartnerSocketSettingOk->OnLButtonUp(pt))
			{	
				tPET_CURRENTINFO *tempCurrentInfo = NULL;
				tPET_BASEDATA *psPetBaseData = NULL;	
				tPET_LEVEL_DATA *psPetLevelData = NULL;

				m_bShowSoketSetting = FALSE;
				m_bCheckSoketSetting = FALSE;
				g_pShuttleChild->GetPetManager()->SetrenderOnOff(FALSE);	
			
				GetPtr_AttachmentedPetInfo ( NULL , &tempCurrentInfo , &psPetBaseData , &psPetLevelData );	
				CItemInfo* temp =  g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetSocketItemUID[g_pShuttleChild->GetPetManager()->GetSocketNum()]);
							
				if ( temp && temp->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOKIT )
				{
					g_pShuttleChild->GetPetManager()->SetSelectSocket(SOKET_TYPE_ITEM);
					
				}
				else if ( temp && temp->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOSKILL )
				{
					g_pShuttleChild->GetPetManager()->SetSelectSocket(SOKET_TYPE_SKILL);
					
				}			
				
				g_pShuttleChild->GetPetManager()->SetSocketSelectingNum(g_pShuttleChild->GetPetManager()->GetSocketNum());
				
				return  INF_MSGPROC_BREAK;				
			}
			//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if(m_pExpScroll->GetMouseMoveMode())
			{
				m_pExpScroll->SetMouseMoveMode( FALSE );
				//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				MSG_FC_ITEM_PET_SET_EXP_RATIO msg;					
				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET );		
			
				msg.ItemUniqueNumber = pAllItem->UniqueNumber;	
				msg.ExpRatio = 	m_pExpScroll->GetScrollStep();
				g_pFieldWinSocket->SendMsg( T_FC_ITEM_PET_SET_EXP_RATIO, (char*)(&msg), sizeof(MSG_FC_ITEM_PET_SET_EXP_RATIO) );			
				//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				return INF_MSGPROC_BREAK;
			}	  
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if( m_pHpItemScroll->GetMouseMoveMode() )
			{
				m_pHpItemScroll->SetMouseMoveMode( FALSE );
				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET );
				if (pAllItem)
				{
					tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(pAllItem->UniqueNumber);

					if (tempCurrentInfo)
					{
						MSG_FC_ITEM_PET_SET_KIT_SLOT msg;
						msg.ItemUniqueNumber = pAllItem->UniqueNumber;
						msg.SocketIndex = g_pShuttleChild->GetPetManager()->GetSocketNum();
						msg.PetSocketItemUID = tempCurrentInfo->PetSocketItemUID[g_pShuttleChild->GetPetManager()->GetSocketNum()];

						msg.PetKitHP.ItemNum = tempCurrentInfo->PetKitHP.ItemNum;
						msg.PetKitHP.TriggerValue = tempCurrentInfo->PetKitHP.TriggerValue;

						msg.PetKitShield.ItemNum = tempCurrentInfo->PetKitShield.ItemNum;
						msg.PetKitShield.TriggerValue = tempCurrentInfo->PetKitShield.TriggerValue;

						msg.PetKitSP.ItemNum = tempCurrentInfo->PetKitSP.ItemNum;
						msg.PetKitSP.TriggerValue = tempCurrentInfo->PetKitSP.TriggerValue;

						g_pFieldWinSocket->SendMsg(T_FC_ITEM_PET_SET_KIT_SLOT, (char*)(&msg), sizeof(MSG_FC_ITEM_PET_SET_KIT_SLOT));
					}
				}
				
				return INF_MSGPROC_BREAK;
			}
			if( m_pDpItemScroll->GetMouseMoveMode() )
			{
				m_pDpItemScroll->SetMouseMoveMode( FALSE );
				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET );
				if (pAllItem)
				{
					tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(pAllItem->UniqueNumber);

					if (tempCurrentInfo)
					{
						MSG_FC_ITEM_PET_SET_KIT_SLOT msg;
						msg.ItemUniqueNumber = pAllItem->UniqueNumber;
						msg.SocketIndex = g_pShuttleChild->GetPetManager()->GetSocketNum();
						msg.PetSocketItemUID = tempCurrentInfo->PetSocketItemUID[g_pShuttleChild->GetPetManager()->GetSocketNum()];

						msg.PetKitHP.ItemNum = tempCurrentInfo->PetKitHP.ItemNum;
						msg.PetKitHP.TriggerValue = tempCurrentInfo->PetKitHP.TriggerValue;

						msg.PetKitShield.ItemNum = tempCurrentInfo->PetKitShield.ItemNum;
						msg.PetKitShield.TriggerValue = tempCurrentInfo->PetKitShield.TriggerValue;

						msg.PetKitSP.ItemNum = tempCurrentInfo->PetKitSP.ItemNum;
						msg.PetKitSP.TriggerValue = tempCurrentInfo->PetKitSP.TriggerValue;

						g_pFieldWinSocket->SendMsg(T_FC_ITEM_PET_SET_KIT_SLOT, (char*)(&msg), sizeof(MSG_FC_ITEM_PET_SET_KIT_SLOT));
					}
				}
				return INF_MSGPROC_BREAK;
			}
			if( m_pSpItemScroll->GetMouseMoveMode() )
			{
				m_pSpItemScroll->SetMouseMoveMode( FALSE );
				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 		

				if (pAllItem)
				{
					tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(pAllItem->UniqueNumber);

					if (tempCurrentInfo)
					{
						MSG_FC_ITEM_PET_SET_KIT_SLOT msg;
						msg.ItemUniqueNumber = pAllItem->UniqueNumber;
						msg.SocketIndex = g_pShuttleChild->GetPetManager()->GetSocketNum();
						msg.PetSocketItemUID = tempCurrentInfo->PetSocketItemUID[g_pShuttleChild->GetPetManager()->GetSocketNum()];

						msg.PetKitHP.ItemNum = tempCurrentInfo->PetKitHP.ItemNum;
						msg.PetKitHP.TriggerValue = tempCurrentInfo->PetKitHP.TriggerValue;

						msg.PetKitShield.ItemNum = tempCurrentInfo->PetKitShield.ItemNum;
						msg.PetKitShield.TriggerValue = tempCurrentInfo->PetKitShield.TriggerValue;

						msg.PetKitSP.ItemNum = tempCurrentInfo->PetKitSP.ItemNum;
						msg.PetKitSP.TriggerValue = tempCurrentInfo->PetKitSP.TriggerValue;

						g_pFieldWinSocket->SendMsg(T_FC_ITEM_PET_SET_KIT_SLOT, (char*)(&msg), sizeof(MSG_FC_ITEM_PET_SET_KIT_SLOT));
					}
				}
				
				return INF_MSGPROC_BREAK;
			}
		
			
			{
				int j = (pt.y - (nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y + 3)) / 30;
				if(pt.x > nWindowPosX + PARTNER_SOCKET_ITEM_POS_X + 6 && 
					pt.x < nWindowPosX + PARTNER_SOCKET_ITEM_POS_X + 6 + 30  &&
					pt.y > nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y +j * PARTNER_SOCKET_ITEM_POS_GAP  && 
					pt.y < nWindowPosY + nWindowPosY + PARTNER_SOCKET_ITEM_POS_Y +j * PARTNER_SOCKET_ITEM_POS_GAP  + 20)
				{
					tPET_CURRENTINFO *tempCurrentInfo = NULL;
					tPET_BASEDATA *psPetBaseData = NULL;
					tPET_LEVEL_DATA *psPetLevelData = NULL;
					
					GetPtr_AttachmentedPetInfo ( NULL , &tempCurrentInfo , &psPetBaseData , &psPetLevelData );

					if(tempCurrentInfo && tempCurrentInfo->PetSocketItemUID[j])
					{
						g_pShuttleChild->GetPetManager()->SetSelectSocket(CheckSocket(j));
						g_pShuttleChild->GetPetManager()->SetSocketNum(j);
						g_pShuttleChild->GetPetManager()->SetSocketSelectingNum(j);
 					}				
					else if( tempCurrentInfo && ((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && ((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
					{
						if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )						
						{
								return INF_MSGPROC_BREAK;
						}

						CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->UniqueNumber);						
						tempCurrentInfo->pItemPetSocket[j] = pItemInfo->GetItemInfo();						
						SetSockeitem(j,pItemInfo,((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->UniqueNumber);						
						g_pShuttleChild->GetPetManager()->SetSelectSocket(CheckSocket(j));
						g_pShuttleChild->GetPetManager()->SetSocketNum(j);
						
					}				
				}
			}			

			if(	g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_ITEM)
			{
				int j = (pt.y - ( nWindowPosY + PARTNER_SOCKET_KIT_POS_Y )) / 35;
				if((pt.x > nWindowPosX + PARTNER_SOCKET_KIT_POS_X && 
					pt.x <= nWindowPosX + PARTNER_SOCKET_KIT_POS_X + 35 && 
					pt.y > nWindowPosY + PARTNER_SOCKET_KIT_POS_Y + j * PARTNER_SOCKET_KIT_POS_GAP && 
					pt.y < nWindowPosY + PARTNER_SOCKET_KIT_POS_Y + 35 + j * PARTNER_SOCKET_KIT_POS_GAP ) &&
					(g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_ITEM))
				{
					if( ((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && ((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
					{
						
						CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->UniqueNumber);						
						SetItemSoket(j,pItemInfo,((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->UniqueNumber);							
					}
				}
			}
			else if(SOKET_TYPE_SKILL == g_pShuttleChild->GetPetManager()->GetSelectSocket())
			{

				int k = (pt.x - ( nWindowPosX + PARTNER_SOCKET_SKILL_POS_X)) / (30 + PARTNER_SOCKET_SKILL_GAP + 20);
				if((pt.x > nWindowPosX + PARTNER_SOCKET_SKILL_POS_X + k * (30 + PARTNER_SOCKET_SKILL_GAP + 20 ) && 
					pt.x <= nWindowPosX + PARTNER_SOCKET_SKILL_POS_X + 30 + k * (30 + PARTNER_SOCKET_SKILL_GAP + 20) && 
					pt.y > nWindowPosY + PARTNER_SOCKET_SKILL_POS_Y  + 6 && 
					pt.y < nWindowPosY + PARTNER_SOCKET_SKILL_POS_Y + 6 + 30 )&& (g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_SKILL))
				{
					if( ((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem && ((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem)
					{
					
						CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->ItemNum);							
						CSkillInfo* tempskill =  g_pShuttleChild->m_pSkill->FindItemSkill(((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->ItemNum);						
						SetSkillSoket(k,tempskill,((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem->UniqueNumber);				
					}
				}  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			}
		}
		break;
		// 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����
		case WM_RBUTTONUP:
			{
				int k = (pt.x - ( nWindowPosX + PARTNER_SOCKET_SKILL_POS_X)) / (30 + PARTNER_SOCKET_SKILL_GAP + 20);
				if((pt.x > nWindowPosX + PARTNER_SOCKET_SKILL_POS_X + k * (30 + PARTNER_SOCKET_SKILL_GAP + 20 ) && 
					pt.x <= nWindowPosX + PARTNER_SOCKET_SKILL_POS_X + 30 + k * (30 + PARTNER_SOCKET_SKILL_GAP + 20) && 
					pt.y > nWindowPosY + PARTNER_SOCKET_SKILL_POS_Y  + 6 && 
					pt.y < nWindowPosY + PARTNER_SOCKET_SKILL_POS_Y + 6 + 30 )&& (g_pShuttleChild->GetPetManager()->GetSelectSocket() == SOKET_TYPE_SKILL))
				{
					int n = 0;
					SetSkillSoket(k,NULL,0);				
				}
			}
			break;
		// 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����
	}
	return INF_MSGPROC_NORMAL;

}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
int CINFCharacterInfoExtend::CheckSocket(int num)
{
	tPET_CURRENTINFO *tempCurrentInfo = NULL;
	tPET_BASEDATA *psPetBaseData = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;
	
	GetPtr_AttachmentedPetInfo ( NULL , &tempCurrentInfo , &psPetBaseData , &psPetLevelData );

	CItemInfo* temp =  g_pStoreData->FindItemInInventoryByUniqueNumber(tempCurrentInfo->PetSocketItemUID[num]);
	int selectSockt;
 	if ( temp && temp->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOKIT )
 	{
 		selectSockt = SOKET_TYPE_ITEM;
 		return selectSockt;
 	}
 	else if ( temp && temp->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_AUTOSKILL )
 	{
 		selectSockt = SOKET_TYPE_SKILL;
 		return selectSockt;
 	}
	else if ( temp && temp->GetItemInfo()->ArrDestParameter[0] == DES_PET_SOCKET_ITEM_SPEED )
	{
		selectSockt = SOKET_TYPE_SPEED;
		return selectSockt;
 	}

	return SOKET_TYPE_NONE;
}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

void CINFCharacterInfoExtend::SetSelectUTC( INVEN_DISPLAY_INFO* pDisplayInfo )
{
	FLOG( "CINFCharacterInfoExtend::SetSelectUTC( INVEN_DISPLAY_INFO* pDisplayInfo )" );
	POINT ptIcon;
	ptIcon.x = m_nRenderMoveIconIntervalWidth;
	ptIcon.y = m_nRenderMoveIconIntervalHeight;

	((CINFGameMain*)m_pParent)->SetSelectItem(pDisplayInfo, ptIcon, ITEM_SKILL_POS);

//	if(pDisplayInfo)
//	{
//		m_pSelectIcon = ((CINFGameMain*)m_pParent)->m_pIcon->FindIcon(pDisplayInfo->IconName);
//		m_pSelectSkill = (ITEM_SKILL*)pDisplayInfo->pItem;
//		g_pGameMain->m_pQuickSlot->m_pSelectItem = (ITEM_BASE*)m_pSelectSkill;
//	}
//	else
//	{
//		m_pSelectIcon = NULL;
//		m_pSelectSkill = NULL;
//	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			SendUseSkill( ITEM_BASE* pSkill )
/// \brief		��ų�� ����Ѵ�.
/// \author		jschoi
/// \date		2004-10-04 ~ 2004-10-04
/// \warning	����� ��ų�� ������� ��� ��Ŭ �����ϴ� �κ��� �ִ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::SendUseSkill( ITEM_BASE* pSkill )
{
	FLOG( "CINFCharacterInfoExtend::SendUseSkill( ITEM_BASE* pSkill )" );
	// 2006-07-28 by ispark, ĳ���Ϳ� ��ų
	if(g_pD3dApp->m_bCharacter &&
		!IS_BAZAAR_SKILL(pSkill->ItemInfo))
	{
		// ĳ���Ϳ� ��ų �ƴ�
		return;
	}

	// 2006-07-30 by ispark, ��ų ����� �޽��� �ڽ��� �㰡 �ؾ��ϴ� ��ų�� üũ�Ѵ�.
	if(!CheckUseSkillApprovalEnd(pSkill))
	{
		return;
	}

	if( pSkill->ItemInfo->SkillType == SKILLTYPE_PERMANENT )
	{
		ASSERT_ASSERT(pSkill->ItemInfo->SkillType != SKILLTYPE_PERMANENT);
		return;
	}

	BOOL bUse = TRUE;
	if(	pSkill->ItemInfo->SkillType == SKILLTYPE_TOGGLE )
	{
		CSkillInfo* pSkillInfo = g_pShuttleChild->m_pSkill->FindUsingSkillInfo(pSkill->ItemNum);
		if(pSkillInfo != NULL)
		{
//			if(	pSkillInfo->GetSkillState() == SKILL_STATE_WAIT_REATTACK ||
//				pSkillInfo->GetSkillState() == SKILL_STATE_RELEASE )
//			{
//				return;
//			}
			pSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
			g_pGameMain->m_pInfSkill->DeleteSkillInfo(pSkillInfo);
			g_pShuttleChild->m_pSkill->EraseUsingSkill(pSkillInfo);
			bUse = FALSE;
		}
	}
	
	// 2008-12-16 by bhsohn A��� ������Ƽ�� ��ų ����� ���� ��ų ���� ���������� ��ų ���
	const auto& ShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if(IS_DT(ShuttleInfo.UnitKind))
	{
		// A�� �ش����
		RefreshAGearSkillInfo(pSkill->ItemNum);
	}
	// end 2008-12-16 by bhsohn A��� ������Ƽ�� ��ų ����� ���� ��ų ���� ���������� ��ų ���

	if(bUse)
	{
		g_pShuttleChild->m_pSkill->UseSkill( (CSkillInfo*)pSkill );
	}
	else
	{
//		g_pShuttleChild->m_pSkill->ReleaseAllUsingToggleSkill();
		
//		(*itSkillInfo)->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
//		g_pGameMain->m_pInfSkill->DeleteSkillInfo(*itSkillInfo);
//		SafeDeleteSkillInfo(*itSkillInfo);
//		m_vecUsingSkill.erase(itSkillInfo);	


//		MSG_FC_SKILL_CANCEL_SKILL sMsg;
//		memset(&sMsg,0x00,sizeof(sMsg));
//		char buffer[SIZE_MAX_PACKET];
//		int nType = T_FC_SKILL_CANCEL_SKILL;
//		sMsg.SkillItemUniqueNumber = pSkill->UniqueNumber;
//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		}

//	if(g_pD3dApp->m_pShuttleChild->m_bIsAir == TRUE && pSkill->ItemInfo->SkillType == 3)		// ���߿��� �����带 ����Ҷ� ������]
//	{
//	}
//	else if( pSkill->ItemInfo->SkillType == 3 ||				// ���� ��� �̰ų�
//		(g_pShuttleChild->m_dwState == _NORMAL					// �Ϲݻ��׿���.. 
//		&& g_pShuttleChild->m_nEventType != EVENT_GAME_END) )	// ��ų ��� ���� - jschoi 
//	{
//		if(bUse)
//		{
//			g_pShuttleChild->m_pSkill->UseSkill( (CSkillInfo*)pSkill );
//			//		MSG_FC_SKILL_USE_SKILL sMsg;
//			//		memset(&sMsg,0x00,sizeof(sMsg));
//			//		char buffer[SIZE_MAX_PACKET];
//			//		int nType = T_FC_SKILL_USE_SKILL;
//			//		sMsg.SkillItemUniqueNumber = pSkill->UniqueNumber;
//			//		// target ???
//			//		sMsg.AttackIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
//			//		sMsg.TargetIndex = 0;
//			//		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//			//		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//			//		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		}
//		else
//		{
//			MSG_FC_SKILL_CANCEL_SKILL sMsg;
//			memset(&sMsg,0x00,sizeof(sMsg));
//			char buffer[SIZE_MAX_PACKET];
//			int nType = T_FC_SKILL_CANCEL_SKILL;
//			sMsg.SkillItemUniqueNumber = pSkill->UniqueNumber;
//			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//			g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
//		}
//	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-12-16 by bhsohn A��� ������Ƽ�� ��ų ����� ���� ��ų ���� ���������� ��ų ���
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::RefreshAGearSkillInfo(int i_nSkillItemNum)
{	
	switch(SKILL_BASE_NUM(i_nSkillItemNum))
	{
	case AGEAR_SKILL_BASENUM_SIEGEMODE:
	case AGEAR_SKILL_BASENUM_SIEGEDEFENSEMODE:
		{
		}
		break;
	default:
		{
			return;
		}
		break;
	}	

	CSkillInfo *pGroundAccelSkillInfo = 
		g_pShuttleChild->m_pSkill->FindUsingSkillInfoByBaseNum(AGEAR_SKILL_BASENUM_GROUNDACCELERATOR);
	if(NULL == pGroundAccelSkillInfo)
	{
		// �׶��� ����
		return;
	}

	pGroundAccelSkillInfo->ChangeSkillState(SKILL_STATE_WAIT_REATTACK);
	g_pGameMain->m_pInfSkill->DeleteSkillInfo(pGroundAccelSkillInfo);
	g_pShuttleChild->m_pSkill->EraseUsingSkill(pGroundAccelSkillInfo);
}



int CINFCharacterInfoExtend::InfoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCharacterInfoExtend::InfoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;
//			m_bToolTipStat = FALSE;					// 2005-07-04 by ispark �ڵ� ���� �й� ����

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			((CINFGameMain*)m_pParent)->SetToolTip(0,0,NULL);

			// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
			
	
			int nCnt = 0;
			for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
			{
				m_pBonusStatBtn[nCnt]->OnMouseMove(pt);	
			}	
			// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
			
			// 2007-05-15 by bhsohn ��� ���� ���� ó��
			BOOL bOverStat[MAX_STAT_SHUTTLEINFO];			
			for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO; nCnt++)
			{
				bOverStat[nCnt] = (m_nShowOverStat[nCnt] > 0);
			}			
				

			if( pt.y > nWindowPosY+STAT_TOOLTIP_ATTACK_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_ATTACK_START_Y+STAT_TOOLTIP_SIZE_Y)
			{				
				if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0001);//"���ݽ� �������� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn ��� ���� ���� ó��
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X-OVER_STAT_CAP0_X
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0001);//"���ݽ� �������� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_ATTACK])
					&& (m_bStatShow[STAT_SHUTTLEINFO_ATTACK]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );
					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0002, pStat->GearStat1.AttackPart);//"\\y���� ���� ������: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				// 2007-05-15 by bhsohn ��� ���� ���� ó��
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_ATTACK]))
				{				
					// �ʰ��� ����
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y�ʰ��� ����"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
				if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0005);//"������ ���� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn ��� ���� ���� ó��
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X -OVER_STAT_CAP0_X
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0005);//"������ ���� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_FUEL])
					&&(m_bStatShow[STAT_SHUTTLEINFO_FUEL]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0006, pStat->GearStat1.FuelPart);//"\\y���� ���� ������: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_FUEL]))
				{
					// �ʰ��� ����
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y�ʰ��� ����"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}

				
			}
			else if( pt.y > nWindowPosY+STAT_TOOLTIP_DEFENSE_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_DEFENSE_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
				if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0003);//"�´� �������� ���ҽ��� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X- OVER_STAT_CAP1_X
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0003);//"�´� �������� ���ҽ��� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DEFENCE])
					&&(m_bStatShow[STAT_SHUTTLEINFO_DEFENCE]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );

					// 2007-05-15 by bhsohn ��� ���� ���� ó��
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0004, pStat->GearStat1.DefensePart);//����->���//"\\y��� ���� ������: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DEFENCE]))
				{
					// �ʰ��� ����
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y�ʰ��� ����"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}

				
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X 
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0007);//"��ų�� ����� �� �ִ� ��ų ����Ʈ�� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn ��� ���� ���� ó��
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X -OVER_STAT_CAP1_X
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0007);//"��ų�� ����� �� �ִ� ��ų ����Ʈ�� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SOUL])
				&&(m_bStatShow[STAT_SHUTTLEINFO_SOUL]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );

					// 2007-05-15 by bhsohn ��� ���� ���� ó��
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0008, pStat->GearStat1.SoulPart);//"\\y���� ���� ������: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SOUL]))
				{
					// �ʰ��� ����
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y�ʰ��� ����"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
			}
			else if( pt.y > nWindowPosY+STAT_TOOLTIP_DODGE_START_Y &&
				pt.y < nWindowPosY+STAT_TOOLTIP_DODGE_START_Y+STAT_TOOLTIP_SIZE_Y)
			{
				if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0009);//"���ݿ� ���� ȸ�Ǹ� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				// 2007-05-15 by bhsohn ��� ���� ���� ó��
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X 
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0009);//"���ݿ� ���� ȸ�Ǹ� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (FALSE == bOverStat[STAT_SHUTTLEINFO_DODGE])
					&& (m_bStatShow[STAT_SHUTTLEINFO_DODGE]))
				{ 
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );
					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0010, pStat->GearStat1.DodgePart);//"\\yȸ�� ���� ������: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}				
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP0_X &&
					pt.x < nWindowPosX+STAT_TOOLTIP_LEFT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y
					&& (TRUE == bOverStat[STAT_SHUTTLEINFO_DODGE]))
				{
					// �ʰ��� ����
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y�ʰ��� ����"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X 
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0011);//"������ ���� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, STRMSG_C_TOOLTIP_0011);//"������ ���� �������� �ݴϴ�."
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X &&
				pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (FALSE == bOverStat[STAT_SHUTTLEINFO_SHIELD])
				&& (m_bStatShow[STAT_SHUTTLEINFO_SHIELD]))
				{
					char buf[64];
					const GEAR_STAT_CHANGE_VALUE* pStat = CAtumSJ::GetGearStatChangeValueStruct( g_pShuttleChild->m_myShuttleInfo.UnitKind );
					
					if(pStat && (g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0))
					{
						sprintf( buf, STRMSG_C_TOOLTIP_0012, pStat->GearStat1.ShieldPart); //���->����//"\\y���� ���� ������: %d"
						g_pGameMain->SetToolTip( pt.x, pt.y, buf);
						return INF_MSGPROC_BREAK;
					}					
				}
				else if( pt.x > nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X - OVER_STAT_CAP1_X&&
				pt.x < nWindowPosX+STAT_TOOLTIP_RIGHT_START_X+STAT_TOOLTIP_SIZE_X+STAT_TOOLTIP_SIZE_Y 
				&& (TRUE == bOverStat[STAT_SHUTTLEINFO_SHIELD]))
				{
					// �ʰ��� ����
					char buf[64];
					sprintf( buf, STRMSG_C_070516_0201);//"\\y�ʰ��� ����"
					g_pGameMain->SetToolTip( pt.x, pt.y, buf);
					return INF_MSGPROC_BREAK;
				}
			}
			// 2008-09-22 by bhsohn EP3 ĳ���� â
			else if( pt.y > nWindowPosY + (AMMO_START_Y+(AMMO_INTERVAL)) &&
				pt.y < nWindowPosY + (AMMO_START_Y+(AMMO_INTERVAL))+STAT_TOOLTIP_SIZE_Y)
			{
				if( pt.x > (nWindowPosX+AMMO_START_X -ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+AMMO_START_X))					
				{
					char chToolTip[MAX_PATH];				
					memset(chToolTip, 0x00, MAX_PATH);

					if(g_pShuttleChild->m_pPrimaryWeapon)
					{							
						char chTmp1[128], chTmp2[128],chTmp3[128];
						//"�Ǿ : %.2f"
						float fStatPierce = g_pShuttleChild->m_pPrimaryWeapon->GetStatPierce();
						float fEnchantPierce = g_pShuttleChild->m_pPrimaryWeapon->GetPrimaryEnchantPierce();
						
						float fItemPierce = g_pShuttleChild->GetShuttleParamFactor()->pfm_PIERCE_UP_01;						
						// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
						//fItemPierce = ((float)fItemPierce / (float)PROB256_MAX_VALUE *100.0f);

						// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
						float fPreSufPierce = g_pShuttleChild->m_pPrimaryWeapon->GetPreSufFixItemInfoRate(DES_PIERCE_UP_01);
						// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��

						// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���																	
						fStatPierce = FloatSecRangeSharp(fStatPierce);
						fEnchantPierce = FloatSecRangeSharp(fEnchantPierce);
						fItemPierce = FloatSecRangeSharp(fItemPierce);
						
						// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
						fItemPierce += fPreSufPierce;
						// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
						
						sprintf( chTmp1, STRMSG_C_080923_0200, fStatPierce);
						sprintf( chTmp2, STRMSG_C_080923_0200, fEnchantPierce);
						sprintf( chTmp3, STRMSG_C_080923_0200, fItemPierce);
						
						if(fEnchantPierce > 0)
						{
							if(fItemPierce > 0)
							{
								sprintf( chToolTip, "%s\\e[+%s]\\e\\g[+%s]\\g", chTmp1, chTmp2, chTmp3);
							}
							else
							{
								sprintf( chToolTip, "%s\\e[+%s]\\e", chTmp1, chTmp2);
							}							
						}
						else
						{
							if(fItemPierce > 0)
							{
								sprintf( chToolTip, "%s\\g[+%s]\\g", chTmp1, chTmp3);
							}
							else
							{
								sprintf( chToolTip, "%s", chTmp1);								
							}
							
						}							
						g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//						
						return INF_MSGPROC_BREAK;
					}					
					else
					{
						char chTmp1[128], chTmp2[128];
						float fStatPierce = GetDefaultPierce(TRUE);			
						float fItemPierce = g_pShuttleChild->GetShuttleParamFactor()->pfm_PIERCE_UP_01;						
						// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
						//fItemPierce = ((float)fItemPierce / (float)PROB256_MAX_VALUE *100.0f);

						// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
						fStatPierce = FloatSecRangeSharp(fStatPierce);
						fItemPierce = FloatSecRangeSharp(fItemPierce);

						sprintf( chTmp1, STRMSG_C_080923_0200, fStatPierce);			
						sprintf( chTmp2, STRMSG_C_080923_0200, fItemPierce);			
						if(fItemPierce > 0)
						{
							sprintf( chToolTip, "%s\\g[+%s]\\g", chTmp1, chTmp2);
						}
						else
						{
							sprintf( chToolTip, "%s", chTmp1);								
						}						

						g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//						

						return INF_MSGPROC_BREAK;
					}
				}
				else if( pt.x > (nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X-ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X))					
				{
					char chToolTip[MAX_PATH];				
					memset(chToolTip, 0x00, MAX_PATH);

					if(g_pShuttleChild->m_pSecondaryWeapon)
					{
						char chTmp1[128], chTmp2[128], chTmp3[128];
						//"�Ǿ : %.2f"
						float fStatPierce = g_pShuttleChild->m_pSecondaryWeapon->GetStatPierce();
						float fEnchantPierce = g_pShuttleChild->m_pSecondaryWeapon->GetSecondaryEnchantPierce();
						float fItemPierce = g_pShuttleChild->GetShuttleParamFactor()->pfm_PIERCE_UP_02;						
						// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
						//fItemPierce = ((float)fItemPierce / (float)PROB256_MAX_VALUE *100.0f);

						// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
						float fPreSufPierce = g_pShuttleChild->m_pSecondaryWeapon->GetPreSufFixItemInfoRate(DES_PIERCE_UP_02);
						// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��

						// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
						fStatPierce = FloatSecRangeSharp(fStatPierce);
						fEnchantPierce = FloatSecRangeSharp(fEnchantPierce);
						fItemPierce = FloatSecRangeSharp(fItemPierce);

						// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
						fItemPierce += fPreSufPierce;
						// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
						
						sprintf( chTmp1, STRMSG_C_080923_0200, fStatPierce);
						sprintf( chTmp2, STRMSG_C_080923_0200, fEnchantPierce);
						sprintf( chTmp3, STRMSG_C_080923_0200, fItemPierce);

						if(fEnchantPierce > 0)
						{
							if(fItemPierce > 0)
							{
								sprintf( chToolTip, "%s\\e[+%s]\\e\\g[+%s]\\g", chTmp1, chTmp2, chTmp3);
							}
							else
							{
								sprintf( chToolTip, "%s\\e[+%s]\\e", chTmp1, chTmp2);
							}							
						}
						else
						{
							if(fItemPierce > 0)
							{
								sprintf( chToolTip, "%s\\g[+%s]\\g", chTmp1, chTmp3);
							}
							else
							{
								sprintf( chToolTip, "%s", chTmp1);								
							}
						}							

						g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//

						return INF_MSGPROC_BREAK;
					}
					else
					{
						char chTmp1[128], chTmp2[128];
						float fStatPierce = GetDefaultPierce(FALSE);			
						float fItemPierce = g_pShuttleChild->GetShuttleParamFactor()->pfm_PIERCE_UP_02;						
						// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
						//fItemPierce = ((float)fItemPierce / (float)PROB256_MAX_VALUE *100.0f);

						// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���						
						fStatPierce = FloatSecRangeSharp(fStatPierce);
						fItemPierce = FloatSecRangeSharp(fItemPierce);

						sprintf( chTmp1, STRMSG_C_080923_0200, fStatPierce);			
						sprintf( chTmp2, STRMSG_C_080923_0200, fItemPierce);			
						if(fItemPierce > 0)
						{
							sprintf( chToolTip, "%s\\g[+%s]\\g", chTmp1, chTmp2);
						}
						else
						{
							sprintf( chToolTip, "%s", chTmp1);								
						}						

						g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//						

						return INF_MSGPROC_BREAK;
					}
				}

			}
			else if( pt.y > nWindowPosY + (AMMO_START_Y+(AMMO_INTERVAL*2)) &&
				pt.y < nWindowPosY + (AMMO_START_Y+(AMMO_INTERVAL*2))+STAT_TOOLTIP_SIZE_Y)
			{
				if( pt.x > (nWindowPosX+AMMO_START_X -ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+AMMO_START_X))					
				{
					if(g_pShuttleChild->m_pPrimaryWeapon)
					{
						CItemInfo *	pItemInfo = g_pShuttleChild->m_pPrimaryWeapon->GetClassItemInfo();
						char chToolTip[MAX_PATH];				
						memset(chToolTip, 0x00, MAX_PATH);
						GetItemAttackInfo(pItemInfo, chToolTip);		
						
						g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//"���ݿ� ���� ȸ�Ǹ� �������� �ݴϴ�."
						return INF_MSGPROC_BREAK;
					}
					
				}
				else if( pt.x > (nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X-ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X))					
				{
					if(g_pShuttleChild->m_pSecondaryWeapon)
					{
						CItemInfo *	pItemInfo = g_pShuttleChild->m_pSecondaryWeapon->GetClassItemInfo();
						char chToolTip[MAX_PATH];				
						memset(chToolTip, 0x00, MAX_PATH);
						GetItemAttackInfo(pItemInfo, chToolTip);		
						
						g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//"���ݿ� ���� ȸ�Ǹ� �������� �ݴϴ�."
						return INF_MSGPROC_BREAK;
					}
					
				}

			}
			else if( pt.y > nWindowPosY + (OTHER_INFO_START_Y) &&
				pt.y < nWindowPosY + (OTHER_INFO_START_Y+STAT_TOOLTIP_SIZE_Y))
			{
				if( pt.x > (nWindowPosX+INFO_START_X -ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+INFO_START_X))					
				{
					// �⺻���� ����� 
//					float fAllDefence = 
//						((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, 
//						&g_pShuttleChild->m_paramFactor, TRUE ) / PROB256_MAX_VALUE)*100.0f;	
					char pDefaultRate[128],pArmorRate[128], pPetRate[128];					
					char chToolTip[MAX_PATH];									
					
					memset(pDefaultRate, 0x00, 128);	
					memset(pArmorRate, 0x00, 128);	
					memset(pPetRate, 0x00, 128);	

					memset(chToolTip, 0x00, MAX_PATH);	
					
					GetDefenceRateString(TRUE, pDefaultRate,pArmorRate, pPetRate);
					
					sprintf(chToolTip, "%s", pDefaultRate);
					if(strlen(pArmorRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pArmorRate);
					}					
					
					if(strlen(pPetRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pPetRate);
					}
					
					g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > (nWindowPosX+INFO_START_X+INFO_START_INTERVAL_X  -ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+INFO_START_X+INFO_START_INTERVAL_X ))					
				{

					char pDefaultRate[128],pArmorRate[128], pPetRate[128];					
					char chToolTip[MAX_PATH];									
					
					memset(pDefaultRate, 0x00, 128);	
					memset(pArmorRate, 0x00, 128);	
					memset(pPetRate, 0x00, 128);	

					memset(chToolTip, 0x00, MAX_PATH);	
										

					GetDodgeRateString(TRUE, pDefaultRate, pArmorRate, pPetRate);

					sprintf(chToolTip, "%s", pDefaultRate);
					
					if(strlen(pArmorRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pArmorRate);
					}										
					if(strlen(pPetRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pPetRate);
					}
					g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//"���ݿ� ���� ȸ�Ǹ� �������� �ݴϴ�."

					return INF_MSGPROC_BREAK;
				}		
				
				
			}
			else if( pt.y > nWindowPosY + (OTHER_INFO_START_Y+INFO_INTERVAL) &&
				pt.y < nWindowPosY + (OTHER_INFO_START_Y+INFO_INTERVAL+STAT_TOOLTIP_SIZE_Y))
			{
				
				if( pt.x > (nWindowPosX+INFO_START_X -ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+INFO_START_X))					
				{
					// ��޹��� ����� 
//					float fAllDefence = 
//						((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, 
//						&g_pShuttleChild->m_paramFactor, TRUE ) / PROB256_MAX_VALUE)*100.0f;	
					char pDefaultRate[128],pArmorRate[128], pPetRate[128];					
					char chToolTip[MAX_PATH];									
					
					memset(pDefaultRate, 0x00, 128);	
					memset(pArmorRate, 0x00, 128);	
					memset(pPetRate, 0x00, 128);	

					memset(chToolTip, 0x00, MAX_PATH);	
					
					GetDefenceRateString(FALSE, pDefaultRate,pArmorRate, pPetRate);
					
					sprintf(chToolTip, "%s", pDefaultRate);
					if(strlen(pArmorRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pArmorRate);
					}					
					
					if(strlen(pPetRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pPetRate);
					}
					
					g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);
					return INF_MSGPROC_BREAK;
				}
				else if( pt.x > (nWindowPosX+INFO_START_X+INFO_START_INTERVAL_X -ATTACK_TOOLIP_W)
					&&pt.x < (nWindowPosX+INFO_START_X+INFO_START_INTERVAL_X))					
				{
					char pDefaultRate[128],pArmorRate[128], pPetRate[128];					
					char chToolTip[MAX_PATH];									
					
					memset(pDefaultRate, 0x00, 128);	
					memset(pArmorRate, 0x00, 128);	
					memset(pPetRate, 0x00, 128);	

					memset(chToolTip, 0x00, MAX_PATH);	
										

					GetDodgeRateString(FALSE, pDefaultRate, pArmorRate, pPetRate);

					sprintf(chToolTip, "%s", pDefaultRate);
					
					if(strlen(pArmorRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pArmorRate);
					}										
					if(strlen(pPetRate) > 0)
					{
						sprintf(chToolTip, "%s%s", chToolTip, pPetRate);
					}
					g_pGameMain->SetToolTip( pt.x, pt.y, chToolTip);//"���ݿ� ���� ȸ�Ǹ� �������� �ݴϴ�."					
					return INF_MSGPROC_BREAK;

				}

			}
			{								
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����							
				POINT ptBkSize;
				ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
				ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else 
				POINT ptBkSize = m_pInoBk->GetImgSize();
#endif
				
				if(pt.y>nWindowPosY &&
					pt.y<nWindowPosY+ptBkSize.y&&
					pt.x>nWindowPosX &&
					pt.x<nWindowPosX + ptBkSize.x)
				{
					return INF_MSGPROC_BREAK;
				}		
			}
			// end 2008-09-22 by bhsohn EP3 ĳ���� â			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;
						
			// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��									
			{
				int nCnt = 0;
				for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
				{					
					if(TRUE == m_pBonusStatBtn[nCnt]->OnLButtonDown(pt))
					{
						OnClickStat(nCnt);
						return  INF_MSGPROC_BREAK;
					}
				}	
			}
			// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��

			// 2005-08-02 by ispark
			// â�ȿ� ���콺 Ŭ���� ��ȿ
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			POINT ptBkSize;
			ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
			ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;	  
#else 
			POINT ptBkSize = m_pInoBk->GetImgSize();
#endif

			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+ptBkSize.y&&
				pt.x>nWindowPosX &&
				pt.x<nWindowPosX + ptBkSize.x)
			{
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	case WM_LBUTTONUP:
		{
//			POINT pt = MAKEPOINT(lParam);
			m_nButtonState = STAT_BUTTON_UP ;
			m_nStatType = STAT_NONE ;

			// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
						
			{
				int nCnt = 0;
				for(nCnt = 0;nCnt < MAX_STAT_SHUTTLEINFO;nCnt++)
				{
					if(TRUE == m_pBonusStatBtn[nCnt]->OnLButtonUp(pt))
					{
						//g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
						return  INF_MSGPROC_BREAK;
					}					
				}	
			}
			// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCharacterInfoExtend::RenderShowStatInfo()
{
	FLOG( "CINFCharacterInfoExtend::ShowStatInfo()" );
	
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;


	char	buff[30] ;
	CShuttleChild* pShuttle = g_pD3dApp->m_pShuttleChild;

	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	if(pShuttle)
	{
		// X��ǥ
		FLOAT fAllPosX[MAX_STAT_SHUTTLEINFO] =
		{
				nWindowPosX+STATINFO_START_X0,		// ����
				nWindowPosX+STATINFO_START_X0,		// ���
				nWindowPosX+STATINFO_START_X1,		// ����
				nWindowPosX+STATINFO_START_X1,		// ����
				nWindowPosX+STATINFO_START_X0,		// ȸ��
				nWindowPosX+STATINFO_START_X1// ����
		};
		// Y��ǥ
		FLOAT fAllPosY[MAX_STAT_SHUTTLEINFO] =
		{
				nWindowPosY+ SHOWSTATINFO_Y1,				// ����
				nWindowPosY+ SHOWSTATINFO_Y2,				// ���	
				nWindowPosY+ SHOWSTATINFO_Y1,				// ����
				nWindowPosY+ SHOWSTATINFO_Y2,				// ����
				nWindowPosY+ SHOWSTATINFO_Y3,				// ȸ��
				nWindowPosY+ SHOWSTATINFO_Y3				// ����
		};	

		FLOAT fOverStatPosX[MAX_STAT_SHUTTLEINFO] =
		{
				nWindowPosX+STATINFO_START_X0-OVER_STAT_CAP0_X,		// ����
				nWindowPosX+STATINFO_START_X0-OVER_STAT_CAP0_X,		// ���
				nWindowPosX+STATINFO_START_X1-OVER_STAT_CAP1_X,		// ����
				nWindowPosX+STATINFO_START_X1-OVER_STAT_CAP1_X,		// ����
				nWindowPosX+STATINFO_START_X0-OVER_STAT_CAP0_X,		// ȸ��
				nWindowPosX+STATINFO_START_X1-OVER_STAT_CAP1_X		// ����
		};
		FLOAT fPosX, fPosY;		
		char szOvrbuff[256] ;
		wsprintf(szOvrbuff, "");

		fPosX = fPosY = 0;
		
		// ���� 
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_ATTACK];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			fPosY = fAllPosY[STAT_SHUTTLEINFO_ATTACK] + 1;										  
#else					 
			fPosY = fAllPosY[STAT_SHUTTLEINFO_ATTACK];		
#endif
			if(m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_ATTACK];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK]);
				if(m_StatOverInfoFont)
				{				
					// ���������� �׸���. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}

				// �β��� �۾��� �׷��ش�.
				wsprintf(buff, "\\e%4d", (int)pShuttle->m_myShuttleInfo.TotalGearStat.AttackPart);//+(int)pShuttle->m_paramFactor.pfp_ATTACK_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
				
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK] == 0)
			{
				// �β��� �۾��� �׷��ش�.
				wsprintf(buff, "\\e%4d", (int)pShuttle->m_myShuttleInfo.TotalGearStat.AttackPart);//+(int)pShuttle->m_paramFactor.pfp_ATTACK_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
				
			}
			else
			{
				wsprintf(buff, "%4d", (int)pShuttle->m_myShuttleInfo.TotalGearStat.AttackPart);//+(int)pShuttle->m_paramFactor.pfp_ATTACK_PART) ;
				if(m_StatInfoFont[0])
				{
					m_StatInfoFont[0]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}

		// ���
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_DEFENCE];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			fPosY = fAllPosY[STAT_SHUTTLEINFO_DEFENCE] + 1;		
#else 
			fPosY = fAllPosY[STAT_SHUTTLEINFO_DEFENCE];		

#endif
			if(m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_DEFENCE];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE]);
				if(m_StatOverInfoFont)
				{				
					// ���������� �׸���. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart);//+(int)pShuttle->m_paramFactor.pfp_DEFENSE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart);//+(int)pShuttle->m_paramFactor.pfp_DEFENSE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart);//+(int)pShuttle->m_paramFactor.pfp_DEFENSE_PART) ;
				if(m_StatInfoFont[1])
				{
					m_StatInfoFont[1]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}

		// ����
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_FUEL];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			fPosY = fAllPosY[STAT_SHUTTLEINFO_FUEL] + 1;		
#else 
			fPosY = fAllPosY[STAT_SHUTTLEINFO_FUEL];		
#endif
			if(m_nShowOverStat[STAT_SHUTTLEINFO_FUEL] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_FUEL];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_FUEL]);
				if(m_StatOverInfoFont)
				{				
					// ���������� �׸���. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart);//+(int)pShuttle->m_paramFactor.pfp_FUEL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_FUEL] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart);//+(int)pShuttle->m_paramFactor.pfp_FUEL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart);//+(int)pShuttle->m_paramFactor.pfp_FUEL_PART) ;
				if(m_StatInfoFont[2])
				{
					m_StatInfoFont[2]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			
		}

		// ����
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_SOUL];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			fPosY = fAllPosY[STAT_SHUTTLEINFO_SOUL] + 1;										  
#else 
			fPosY = fAllPosY[STAT_SHUTTLEINFO_SOUL];		
#endif
			if(m_nShowOverStat[STAT_SHUTTLEINFO_SOUL] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_SOUL];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_SOUL]);
				if(m_StatOverInfoFont)
				{				
					// ���������� �׸���. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart);//+(int)pShuttle->m_paramFactor.pfp_SOUL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_SOUL] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart);//+(int)pShuttle->m_paramFactor.pfp_SOUL_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}

			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart);//+(int)pShuttle->m_paramFactor.pfp_SOUL_PART) ;
				if(m_StatInfoFont[3])
				{
					m_StatInfoFont[3]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}

		// ȸ��
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_DODGE];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			fPosY = fAllPosY[STAT_SHUTTLEINFO_DODGE] + 1;
#else					   
			fPosY = fAllPosY[STAT_SHUTTLEINFO_DODGE];		
#endif		
			if(m_nShowOverStat[STAT_SHUTTLEINFO_DODGE] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_DODGE];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_DODGE]);
				if(m_StatOverInfoFont)
				{				
					// ���������� �׸���. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart);//+(int)pShuttle->m_paramFactor.pfp_DODGE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_DODGE] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart);//+(int)pShuttle->m_paramFactor.pfp_DODGE_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart);//+(int)pShuttle->m_paramFactor.pfp_DODGE_PART) ;
				if(m_StatInfoFont[4])
				{
					m_StatInfoFont[4]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			
		}

		// ����
		{			
			fPosX = fAllPosX[STAT_SHUTTLEINFO_SHIELD];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			fPosY = fAllPosY[STAT_SHUTTLEINFO_SHIELD] + 1;		
#else	   
			fPosY = fAllPosY[STAT_SHUTTLEINFO_SHIELD];		
#endif
			if(m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD] > 0)
			{
				fPosX = fOverStatPosX[STAT_SHUTTLEINFO_SHIELD];
				wsprintf(szOvrbuff, "(+%d)", m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD]);
				if(m_StatOverInfoFont)
				{				
					// ���������� �׸���. 
					m_StatOverInfoFont->DrawText(fPosX+OVER_STAT_CAPX, fPosY, GUI_FONT_COLOR_R, szOvrbuff, 0 );
				}
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart);//+(int)pShuttle->m_paramFactor.pfp_SHIELD_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else if(m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD] == 0)
			{
				wsprintf(buff, "\\e%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart);//+(int)pShuttle->m_paramFactor.pfp_SHIELD_PART) ;
				if(m_StatInfoBoldFont)
				{
					m_StatInfoBoldFont->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}
			else
			{
				wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart);//+(int)pShuttle->m_paramFactor.pfp_SHIELD_PART) ;
				if(m_StatInfoFont[5])
				{
					m_StatInfoFont[5]->DrawText(fPosX, fPosY,GUI_FONT_COLOR, buff, 0 );
				}
			}			
		}
		
		wsprintf(buff, "%4d", (int)g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat) ;
		if(m_StatInfoFont[6])
			m_StatInfoFont[6]->DrawText(nWindowPosX+STATINFO_START_X0-115, nWindowPosY+ SHOWSTATINFO_Y4,
			GUI_FONT_COLOR, buff, 0 );	
		
	}	
	// end 2007-05-15 by bhsohn ��� ���� ���� ó��
	
	if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0)
	{
		// X��ǥ
		int  nPosX[MAX_STAT_SHUTTLEINFO] =
		{
				nWindowPosX+STATINFO_START_X0+30,		// ����
				nWindowPosX+STATINFO_START_X0+30,		// ���
				nWindowPosX+STATINFO_START_X1+30,		// ����
				nWindowPosX+STATINFO_START_X1+30,		// ����
				nWindowPosX+STATINFO_START_X0+30,		// ȸ��
				nWindowPosX+STATINFO_START_X1+30		// ����
		};
		// Y��ǥ
		int nPosY[MAX_STAT_SHUTTLEINFO] =
		{
				nWindowPosY+ SHOWSTATINFO_Y1+3,				// ����
				nWindowPosY+ SHOWSTATINFO_Y2+3,				// ���	
				nWindowPosY+ SHOWSTATINFO_Y1+3,				// ����
				nWindowPosY+ SHOWSTATINFO_Y2+3,				// ����
				nWindowPosY+ SHOWSTATINFO_Y3+3,				// ȸ��
				nWindowPosY+ SHOWSTATINFO_Y3+3				// ����
		};	

		
		int nPos = 0;
				
		for(nPos = 0; nPos<MAX_STAT_SHUTTLEINFO ; nPos++)
		{
			m_pBonusStatBtn[nPos]->SetBtnPosition(nPosX[nPos], nPosY[nPos]);	
			m_pBonusStatBtn[nPos]->Render();
		}
	}
}

int CINFCharacterInfoExtend::GetUtcMaxScrollNumber()
{
	int Utc[4]={0,0,0,0};
//	map<int,ITEM_SKILL*>::iterator itSkill = g_pShuttleChild->m_pSkill->m_mapSkill.begin();
	map<int,CSkillInfo*>::iterator itSkillInfo = g_pShuttleChild->m_pSkill->m_mapSkill.begin();
	while(itSkillInfo != g_pShuttleChild->m_pSkill->m_mapSkill.end())
	{
		// 2013-03-19 by bhsohn ��ų ��ȣ ���� ����
// 		int nTemp = itSkillInfo->second->ItemNum / 1000;
// 		int nWindowNum = (itSkillInfo->second->ItemNum  % nTemp) / 10 -1;
		int nBaseNum = max(1, SKILL_BASE_NUM(itSkillInfo->second->ItemNum));
		int nTemp = max(1, nBaseNum / 1000);
		int nWindowNum = (nBaseNum  % nTemp) / 10 -1;		
		// END 2013-03-19 by bhsohn ��ų ��ȣ ���� ����
		// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//		Utc[SKILL_KIND(itSkillInfo->second->ItemNum)] = max(Utc[SKILL_KIND(itSkillInfo->second->ItemNum)], nWindowNum) ;
		Utc[SKILL_KIND_INDEX(itSkillInfo->second->ItemNum)] = max(Utc[SKILL_KIND_INDEX(itSkillInfo->second->ItemNum)], nWindowNum) ;
		itSkillInfo++;
	}
	return max(max(Utc[0],Utc[1]),max(Utc[2],Utc[3]));
}

#define C_QUEST_DESC_TEXT_NUM		28
void CINFCharacterInfoExtend::SetQuestDesc(int nQuestWindowDataIndex)
{
	if(nQuestWindowDataIndex < 0)
	{
		m_pQuestListScrollBar->Reset();
		m_pQuestDescScrollBar->Reset();
		m_vecQuestDescription.clear();
		return;
	}
	m_pQuestDescScrollBar->Reset();
	m_vecQuestDescription.clear();
	int index =0;
	map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();
	while(it != g_pQuestData->m_mapCharacterQuest.end() )
	{
		if(it->second->QuestState == QUEST_STATE_IN_PROGRESS)
		{
			if(index == nQuestWindowDataIndex)
			{
				break;
			}
			index ++;
		}
		it++;
	}
	if(it == g_pQuestData->m_mapCharacterQuest.end() )
	{
		return;
	}
	CQuest* pQuest = g_pQuestData->FindQuest( it->second->QuestIndex );
	if(pQuest == NULL)
	{
		DBGOUT("CINFCharacterInfoExtend::SetQuestDesc(%d) Can't Find Quest.\n", nQuestWindowDataIndex);
		return;
	}
	if(strlen(pQuest->QuestDescription) == 0)
	{
		DBGOUT("CINFCharacterInfoExtend::SetQuestDesc(%d) Quest (%d) description is empty.\n", nQuestWindowDataIndex, pQuest->QuestIndex);
		return;
	}
	int i = 0;
	int nPoint = 0;
	int nCheckPoint = 0;
	int nBreakPoint = 0;
	int nLine = 0;
	int nLineLength = C_QUEST_DESC_TEXT_NUM;
//	if(strlen(strNPCTalk) > CITY_NPCTALK_SIZE+CITY_NPCTALK_SIZE/2 ||
//		strlen(strNPCTalk) < CITY_NPCTALK_SIZE/3*2)
//	{
//		nLineLength = CITY_NPCTALK_SIZE-1;
//	}
//	else 
//	{
//		nLineLength = strlen(strNPCTalk)/3 * 2 - 1;
//	}
	char szBuffer[MAX_PATH];
	memset( szBuffer, 0x00, MAX_PATH );
	while(TRUE)
	{
		if(pQuest->QuestDescription[i] == ' ' || 
			pQuest->QuestDescription[i] == '.' || 
			pQuest->QuestDescription[i] == '!' || 
			pQuest->QuestDescription[i] == NULL)
		{
			if(nPoint >= nLineLength)
			{
				memcpy( szBuffer, pQuest->QuestDescription + nCheckPoint, nBreakPoint+1);
				m_vecQuestDescription.push_back( (string)szBuffer );
				memset( szBuffer, 0x00, MAX_PATH );
				
				nPoint -= nBreakPoint;
				nCheckPoint += nBreakPoint+1;
				nBreakPoint = nPoint-1;
				nLine ++;
				i++;
				continue;
			}
			if(pQuest->QuestDescription[i] == NULL)
			{
				memcpy( szBuffer, pQuest->QuestDescription + nCheckPoint, nPoint);
				m_vecQuestDescription.push_back( (string)szBuffer );
				memset( szBuffer, 0x00, MAX_PATH );
				break;
			}
			nBreakPoint = nPoint;
		}
		i++;
		nPoint++;
	}
	if (pQuest->IsCityWar)
	{
		//wsprintf( szBuffer, STRMSG_C_QUEST_0009, it->second->szCityWarServerGroupName );//"����:%s"
		//m_vecQuestDescription.push_back( (string)szBuffer );
	}
	m_pQuestDescScrollBar->SetNumberOfData( m_vecQuestDescription.size() );
}
void CINFCharacterInfoExtend::RenderNonQuest(int nWindowPosY, int nLine)
{
	int nWindowPosX = m_ptBkPos.x;
	nWindowPosY = m_ptBkPos.y;

	// ����Ʈ ���� ������Ʈ
	g_pQuestData->NotifyStartQuest();
	int mScrollIndex	= m_pQuestListScrollBar->GetCurrentScrollIndex() - m_nMissionProCount;
	
	vector<Quest_Data*>::iterator it = g_pQuestData->m_vecQuest.begin();
	while(it != g_pQuestData->m_vecQuest.end())
	{		
		Quest_Data* pInfo = (*it);
		if(mScrollIndex <= 0 && nLine < C_QUEST_LIST_NUM)
		{
			m_pFontQuestList[nLine]->DrawText(C_QUEST_START_X,
				nWindowPosY+C_QUEST_START_Y+C_QUEST_INTERVAL*nLine+1,
				RGB(128,128,255),
				pInfo->szMissionName);
			/////////////////////////////////////////////////////////////////////////////////////////////
			// ����
			if(m_pQuestListScrollBar->GetCurrentSelectWindowIndex() == nLine)
			{
				char buffChar[1024];
				memset(buffChar, 0x00, 1024);
				
				m_pFontQuestDesc[0]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+0*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR_Y,
					STRMSG_C_QUEST_0010 );//"�̼�����"

				CQuest* pQuest = g_pQuestData->FindQuest( pInfo->nIndex );
				if(pQuest->ReqLevel.Max > 100)
				{
					wsprintf(buffChar, STRMSG_C_QUEST_0011, pQuest->ReqLevel.Min);//"���ѷ���: %d ~"
				}
				else
				{
					wsprintf(buffChar, STRMSG_C_QUEST_0012,pQuest->ReqLevel.Min, pQuest->ReqLevel.Max);//"���ѷ���: %d ~ %d "
				}				
				m_pFontQuestDesc[2]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+2*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR,
					buffChar );

				memset(buffChar, 0x00, 1024);
				wsprintf(buffChar, STRMSG_C_QUEST_0013, pInfo->szMAPName);//"��:%s"
				m_pFontQuestDesc[3]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+3*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR,
					buffChar );

				memset(buffChar, 0x00, 1024);
				wsprintf(buffChar, "NPC: %s", pInfo->szNPCName);
				m_pFontQuestDesc[4]->DrawText(C_QUEST_DESC_START_X+1,
					nWindowPosY+C_QUEST_DESC_START_Y+4*C_QUEST_DESC_INTERVAL+1,
					GUI_FONT_COLOR,
					buffChar );				
			}
			//
			///////////////////////////////////////////////////////////////////////////////////////////////	

			nLine++;			
		}		
		mScrollIndex--;
		it++;
	}
}	
		
void CINFCharacterInfoExtend::RenderStatDescToolTip(int nType, int x, int y)
{
	char buff[512];
	ZERO_MEMORY(buff);
	
	switch(nType) {
	case AUTOSTAT_TYPE_FREESTYLE:			wsprintf(buff, STRMSG_C_TOOLTIP_0021);	break;// "���ϴ� ���·� ���������� ���� ����"
	case AUTOSTAT_TYPE_BGEAR_ATTACK:		wsprintf(buff, STRMSG_C_TOOLTIP_0022);	break;// "���ݷ��� ��ȭ�� ���� ����"
	case AUTOSTAT_TYPE_BGEAR_MULTI:			wsprintf(buff, STRMSG_C_TOOLTIP_0023);	break;// "�پ��� ��Ƽ��� ���� ����"
	case AUTOSTAT_TYPE_IGEAR_ATTACK:		wsprintf(buff, STRMSG_C_TOOLTIP_0022);	break;// "���ݷ��� ��ȭ�� ���� ����"
	case AUTOSTAT_TYPE_IGEAR_DODGE:			wsprintf(buff, STRMSG_C_TOOLTIP_0024);	break;// "ȸ�Ƿ��� ��ȭ�� ���� ����"
	case AUTOSTAT_TYPE_AGEAR_ATTACK:		wsprintf(buff, STRMSG_C_TOOLTIP_0022);	break;// "���ݷ��� ��ȭ�� ���� ����"
	case AUTOSTAT_TYPE_AGEAR_SHIELD:		wsprintf(buff, STRMSG_C_TOOLTIP_0025);	break;// "�������� ��ȭ�� ���� ����"
	case AUTOSTAT_TYPE_MGEAR_DEFENSE:		wsprintf(buff, STRMSG_C_TOOLTIP_0026);	break;// "������ ��ȭ�� ���� ����"
	case AUTOSTAT_TYPE_MGEAR_SUPPORT:		wsprintf(buff, STRMSG_C_TOOLTIP_0027);	break;// "����� �����ɷ��� ��ȭ�� ���� ����"
	}

	g_pGameMain->RenderPopUpWindowImage(x, y, strlen(buff)*6.5, 1);
	m_pFontStatDec->DrawText(x, y, GUI_FONT_COLOR,buff);
}

int CINFCharacterInfoExtend::AmmoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosX = m_ptBkPos.x;
			int nWindowPosY = m_ptBkPos.y;


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
			POINT ptBkSize;
			ptBkSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
			ptBkSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else					  
			POINT ptBkSize = m_pInoBk->GetImgSize();
#endif																								  

			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+ptBkSize.y&&
				pt.x>nWindowPosX &&
				pt.x<nWindowPosX + ptBkSize.x)
			{
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFCharacterInfoExtend::CheckUseSkillApprovalEnd(ITEM_BASE* pSkill)
/// \brief		������� ��ų ���� �㰡 üũ
/// \author		ispark
/// \date		2006-07-30 ~ 2006-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCharacterInfoExtend::CheckUseSkillApprovalEnd(ITEM_BASE* pSkill)
{
	CSkillInfo* pSkillInfo = g_pShuttleChild->m_pSkill->FindUsingSkillInfo(pSkill->ItemNum);
	if(pSkillInfo && pSkillInfo->GetbApprovalEnd() == FALSE)
	{
		// ��ų�� ����� ���� ������
		if(g_pInterface->m_pBazaarShop &&
			pSkill->UniqueNumber == pSkillInfo->UniqueNumber &&
			IS_BAZAAR_SKILL(pSkillInfo->ItemInfo))
		{
			// 2006-08-01 by ispark, ������ �����̸�
			if(g_pInterface->m_pBazaarShop->GetbBazaarShopStart())
			{
				char msgBox[256] = {0,};
				if(g_pInterface->m_pBazaarShop->GetbBazaarShopType() == 1)
				{
					wsprintf(msgBox, STRMSG_C_060730_0000, STRMSG_C_060730_0002);		// "%s ������ �����Ͻðڽ��ϱ�?"
				}	
				else if(g_pInterface->m_pBazaarShop->GetbBazaarShopType() == 2)
				{
					wsprintf(msgBox, STRMSG_C_060730_0000, STRMSG_C_060730_0003);		// "%s ������ �����Ͻðڽ��ϱ�?"
				}
				g_pGameMain->m_pInfWindow->AddMsgBox(msgBox, _Q_BAZAAR_OPEN_SHOP_END, (DWORD)pSkillInfo);

				return FALSE;
			}
		}
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		������ ������ ���� ����
/// \author		// 2007-05-15 by bhsohn ��� ���� ���� ó��
/// \date		2007-05-15 ~ 2007-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::GetStatShowInfo()
{	
	//(m_bStatShow, m_nShowOverStat)
	// ��ü ���� 
	GEAR_STAT stMyShuttleFullStatInfo = g_pD3dApp->m_pShuttleChild->GetMyShuttleFullStatInfo();	

	// 2009. 12. 29 by ckPark ���� 100���� ĳ���ʹ� �ִ뽺�� 300, ���� 100�ʰ� ĳ���ʹ� �ִ뽺�� 340 ����

// 	// ����
// 	m_bStatShow[STAT_SHUTTLEINFO_ATTACK] = TRUE;
// 	m_nShowOverStat[STAT_SHUTTLEINFO_ATTACK] = stMyShuttleFullStatInfo.AttackPart - COUNT_MAX_STAT_POINT;		
// 	if(stMyShuttleFullStatInfo.AttackPart >= COUNT_MAX_STAT_POINT)
// 	{
// 		m_bStatShow[STAT_SHUTTLEINFO_ATTACK] = FALSE;				
// 	}	
// 	m_pBonusStatBtn[STAT_SHUTTLEINFO_ATTACK]->ShowWindow(m_bStatShow[STAT_SHUTTLEINFO_ATTACK]);
// 
// 	// ���
// 	m_bStatShow[STAT_SHUTTLEINFO_DEFENCE] = TRUE;
// 	m_nShowOverStat[STAT_SHUTTLEINFO_DEFENCE] = stMyShuttleFullStatInfo.DefensePart - COUNT_MAX_STAT_POINT;		
// 	if(stMyShuttleFullStatInfo.DefensePart >= COUNT_MAX_STAT_POINT)
// 	{
// 		m_bStatShow[STAT_SHUTTLEINFO_DEFENCE] = FALSE;		
// 	}	
// 	m_pBonusStatBtn[STAT_SHUTTLEINFO_DEFENCE]->ShowWindow(m_bStatShow[STAT_SHUTTLEINFO_DEFENCE]);
// 
// 	// ����
// 	m_bStatShow[STAT_SHUTTLEINFO_FUEL] = TRUE;
// 	m_nShowOverStat[STAT_SHUTTLEINFO_FUEL] = stMyShuttleFullStatInfo.FuelPart - COUNT_MAX_STAT_POINT;		
// 	if(stMyShuttleFullStatInfo.FuelPart >= COUNT_MAX_STAT_POINT)
// 	{
// 		m_bStatShow[STAT_SHUTTLEINFO_FUEL] = FALSE;		
// 	}	
// 	m_pBonusStatBtn[STAT_SHUTTLEINFO_FUEL]->ShowWindow(m_bStatShow[STAT_SHUTTLEINFO_FUEL]);
// 
// 	// ����
// 	m_bStatShow[STAT_SHUTTLEINFO_SOUL] = TRUE;
// 	m_nShowOverStat[STAT_SHUTTLEINFO_SOUL] = stMyShuttleFullStatInfo.SoulPart - COUNT_MAX_STAT_POINT;		
// 	if(stMyShuttleFullStatInfo.SoulPart >= COUNT_MAX_STAT_POINT)
// 	{
// 		m_bStatShow[STAT_SHUTTLEINFO_SOUL] = FALSE;	
// 	}	
// 	m_pBonusStatBtn[STAT_SHUTTLEINFO_SOUL]->ShowWindow(m_bStatShow[STAT_SHUTTLEINFO_SOUL]);
// 
// 	// ȸ��
// 	m_bStatShow[STAT_SHUTTLEINFO_DODGE] = TRUE;
// 	m_nShowOverStat[STAT_SHUTTLEINFO_DODGE] = stMyShuttleFullStatInfo.DodgePart - COUNT_MAX_STAT_POINT;
// 	if(stMyShuttleFullStatInfo.DodgePart >= COUNT_MAX_STAT_POINT)
// 	{
// 		m_bStatShow[STAT_SHUTTLEINFO_DODGE] = FALSE;		
// 	}	
// 	m_pBonusStatBtn[STAT_SHUTTLEINFO_DODGE]->ShowWindow(m_bStatShow[STAT_SHUTTLEINFO_DODGE]);
// 	
// 	// ����
// 	m_bStatShow[STAT_SHUTTLEINFO_SHIELD] = TRUE;
// 	m_nShowOverStat[STAT_SHUTTLEINFO_SHIELD] = stMyShuttleFullStatInfo.ShieldPart - COUNT_MAX_STAT_POINT;		
// 	if(stMyShuttleFullStatInfo.ShieldPart >= COUNT_MAX_STAT_POINT)
// 	{
// 		m_bStatShow[STAT_SHUTTLEINFO_SHIELD] = FALSE;		
// 	}		
// 	m_pBonusStatBtn[STAT_SHUTTLEINFO_SHIELD]->ShowWindow(m_bStatShow[STAT_SHUTTLEINFO_SHIELD]);

	int nLv = g_pD3dApp->m_pShuttleChild->GetMyShuttleInfo().Level;
	for( int i=0; i<MAX_STAT_SHUTTLEINFO; ++i )
	{
		int nStat = 0;
		switch( i )
		{
		case STAT_SHUTTLEINFO_ATTACK:
			nStat = stMyShuttleFullStatInfo.AttackPart;
			break;
		case STAT_SHUTTLEINFO_DEFENCE:
			nStat = stMyShuttleFullStatInfo.DefensePart;
			break;
		case STAT_SHUTTLEINFO_FUEL:
			nStat = stMyShuttleFullStatInfo.FuelPart;
			break;
		case STAT_SHUTTLEINFO_SOUL:
			nStat = stMyShuttleFullStatInfo.SoulPart;
			break;
		case STAT_SHUTTLEINFO_DODGE:
			nStat = stMyShuttleFullStatInfo.DodgePart;
			break;
		case STAT_SHUTTLEINFO_SHIELD:
			nStat = stMyShuttleFullStatInfo.ShieldPart;
			break;
		}

		m_bStatShow[i] = TRUE;
		if( (nLv <= CHARACTER_LEVEL_100 && nStat >= CHARACTER_LEVEL_100_MAX_STAT_POINT)
			|| (nLv > CHARACTER_LEVEL_100 && nStat >= CHARACTER_LEVEL_110_MAX_STAT_POINT) )
		{
			m_bStatShow[i] = FALSE;
		}
		m_pBonusStatBtn[i]->ShowWindow(m_bStatShow[i]);

		if( nLv <= CHARACTER_LEVEL_100 )
			m_nShowOverStat[i] = nStat - CHARACTER_LEVEL_100_MAX_STAT_POINT;
		else if( nLv > CHARACTER_LEVEL_100 )
			m_nShowOverStat[i] = nStat - CHARACTER_LEVEL_110_MAX_STAT_POINT;
	}

	// end 2009. 12. 29 by ckPark ���� 100���� ĳ���ʹ� �ִ뽺�� 300, ���� 100�ʰ� ĳ���ʹ� �ִ뽺�� 340 ����
}

void CINFCharacterInfoExtend::SetCharacterInfoType(int nCharacterInfoType)
{
	m_nCharacterInfoType = nCharacterInfoType;
}

void CINFCharacterInfoExtend::ShowCharacterInfoWnd(BOOL bShowWnd, POINT* pPos/*=NULL*/)
{
	m_bShowWnd = bShowWnd;
	if(pPos)
	{
		m_ptBkPos = (*pPos);
		m_ptSkillBkPos = (*pPos);		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	}
	else
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		POINT ptSize;
		ptSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
		ptSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;			  
#else 
		POINT ptSize = m_pInoBk->GetImgSize();
#endif
		
		m_ptBkPos.x = (g_pD3dApp->GetBackBufferDesc().Width-ptSize.x)/2;
		m_ptBkPos.y = (g_pD3dApp->GetBackBufferDesc().Height-ptSize.y)/2;
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		POINT ptSkillSize;
		ptSkillSize.x = ptSkillSize.y = 0;
		for (int i = 0 ; i <  3 ; i++)
		{
			ptSkillSize.x += m_pPartnerSkillShop[i]->GetImgSize().x;
			ptSkillSize.y += m_pPartnerSkillShop[i*3]->GetImgSize().y;
		}
		ptSkillSize.x += PARTNER_SKILL_POPUPSHOP_X_GAP;	
		ptSkillSize.y += PARTNER_SKILL_POPUPSHOP_Y_GAP;


		m_ptSkillBkPos.x = (g_pD3dApp->GetBackBufferDesc().Width-ptSkillSize.x)/9;
		m_ptSkillBkPos.y = (g_pD3dApp->GetBackBufferDesc().Height-ptSkillSize.y)/10;
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	}	
	if(bShowWnd)
	{
		int nTemp = m_nCurrentUTCScroll;
		// ��ũ�� �ʱ�ȭ
		m_nCurrentUTCScroll = 0;
		if(m_nCurrentUTCScroll>GetUtcMaxScrollNumber())
		{
			m_nCurrentUTCScroll = GetUtcMaxScrollNumber();
		}				
					
		if(nTemp != m_nCurrentUTCScroll)
		{
			SetAllUTCInfo();
		}

		m_bMove = FALSE;
		m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;
		m_ptSkillCommOpMouse.x = m_ptSkillCommOpMouse.y = 0;              //2011-10-06 by jhahn ��Ʈ�� ������ �ý���


		g_pInterface->SetWindowOrder(WNDCharacterWnd);		

		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
		if(m_pNickNameBtn)
		{
			const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
			if(0 == strlen(myShuttleInfo.NickName))
			{
				// ������ ����.
				m_pNickNameBtn->ShowWindow(TRUE);
			}
			else
			{
				m_pNickNameBtn->ShowWindow(FALSE);
			}
			if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
			{
				// �Ʒ��� ���� ����
				m_pNickNameBtn->ShowWindow(FALSE);
			}

		}
		// end 2009-02-13 by bhsohn ���� ��ŷ �ý���
	}

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	tPET_CURRENTINFO *psPetCurInfo = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;
	
	GetPtr_AttachmentedPetInfo ( NULL , &psPetCurInfo , NULL , &psPetLevelData );
	
	if(psPetCurInfo	)
	{
		m_pExpScroll->SetScrollStep(psPetCurInfo->PetExpRatio);
		m_pHpItemScroll->SetScrollStep(psPetCurInfo->PetKitHP.TriggerValue );
		m_pDpItemScroll->SetScrollStep(psPetCurInfo->PetKitShield.TriggerValue );
		m_pSpItemScroll->SetScrollStep(psPetCurInfo->PetKitSP.TriggerValue );

	}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
}

BOOL CINFCharacterInfoExtend::IsShowWnd()
{
	return m_bShowWnd;
}

void CINFCharacterInfoExtend::RenderCommandInfo()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;	
	char buff[MAX_PATH];
	memset(buff, 0x00, MAX_PATH);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����

	if(m_pInformationBK)
	{
		m_pInformationBK->Move(nWindowPosX, nWindowPosY);
		m_pInformationBK->Render();
	}
#else 
	if(m_pInoBk )
	{
		m_pInoBk->Move(nWindowPosX, nWindowPosY);
		m_pInoBk->Render() ;
	}
#endif																								  

	SIZE szSize = {0,0};

		
	wsprintf(buff, "\\m%d\\m", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Level);	
	szSize = m_pFontLevel->GetStringSize(buff);
	m_pFontLevel->DrawText(	nWindowPosX + INFO_LEVEL_START_X - szSize.cx,
							nWindowPosY + INFO_LEVEL_START_Y, 
							GUI_FONT_COLOR, 
							buff, 0 );

	char szCharacterName[SIZE_MAX_ARENA_FULL_NAME];
	memset(szCharacterName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
	strncpy(szCharacterName, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
	g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharacterName);	
	m_pFontName->DrawText(nWindowPosX+INFO_ID_START_X ,
							nWindowPosY+ INFO_ID_START_Y, 
							GUI_FONT_COLOR, szCharacterName, 0 );	
	
	//
	CINFImage* pFace = ((CInterface*)m_pParent->m_pParent)->m_pPilotFace->FindPilotImage(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PilotFace);
	if(pFace)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		pFace->Move(nWindowPosX+30,nWindowPosY + 57);											  
#else																							  
		pFace->Move(nWindowPosX+26,nWindowPosY + 57);
#endif
		pFace->Render();
	}	
	if( g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0 )
	{
		wsprintf(buff, "\\m%s\\m", g_pShuttleChild->m_myShuttleInfo.GuildName );//"�Ҽ�:%s"					
		g_pGameMain->TextReduce(m_pFontGuild, 100, buff); // 2013-03-28 by bhsohn ���� �̸��� �ʹ� �� ��� ����â���� �ʵ� ����

		szSize = m_pFontGuild->GetStringSize(buff);
		m_pFontGuild->DrawText( nWindowPosX+GUILD_START_X-szSize.cx, 
			nWindowPosY + GUILD_START_Y, 
			GUI_FONT_COLOR, buff, 0 );

		CINFImage* pMark = g_pDatabase->GetGuildMark(g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber);
		if(pMark)
		{
			pMark->Move(nWindowPosX+GUILD_START_X-szSize.cx-30, nWindowPosY + GUILD_MARK_START_Y);
			pMark->Render();
		}		
	}
	
	wsprintf(buff, "\\m%s\\m", (LPTSTR)CAtumSJ::GetStatusByLevel(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Level) );//"�ź�:%s"	
	szSize = m_pFontGeneral->GetStringSize(buff);
	m_pFontGeneral->DrawText( nWindowPosX + GENERAL_START_X - szSize.cx,
		nWindowPosY + GENERAL_START_Y, 
								GUI_FONT_COLOR, buff,0 );
	
	wsprintf(buff,"\\m%d\\m",g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Propensity);//"��:%d"	
	szSize = m_pFontPropensity->GetStringSize(buff);
	m_pFontPropensity->DrawText( nWindowPosX+PROPENSITY_START_X - szSize.cx,
		nWindowPosY + PROPENSITY_START_Y, 
		GUI_FONT_COLOR, buff,0 );
	
	wsprintf(buff, STRMSG_C_080923_0203, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PKWinPoint,g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.PKLossPoint);//"������:%3d�� %3d��"	
	szSize = m_pFontGuild->GetStringSize(buff);
	m_pFontGuild->DrawText( nWindowPosX + PERSONAL_STAT_X - szSize.cx,
								nWindowPosY + PERSONAL_STAT_Y, 
								GUI_FONT_COLOR, buff, 0 );
	
	// ��� ��
	if( g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0 )
	{
		MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo = g_pGameMain->m_pCommunity->GetGuild()->GetGuildInfo();
		if(pInfo)
		{
			wsprintf(buff, STRMSG_C_080923_0203, pInfo->WarWinPoint, pInfo->WarLossPoint);//"�����:%3d�� %3d��"			
			szSize = m_pFontGuild->GetStringSize(buff);
			m_pFontGuild->DrawText( nWindowPosX + GUILDWAR_STAT_X - szSize.cx, 
				nWindowPosY + GUILDWAR_STAT_Y, 
				GUI_FONT_COLOR, buff, 0 );
		}
	}
		
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	if(m_pOpenBtn)
	{				
		int nPosX = nWindowPosX + 475;
		int	nPosY = nWindowPosY + 133;
		
		m_pOpenBtn->SetBtnPosition(nPosX, nPosY);
		m_pOpenBtn->Render();			
	}	
	if(m_pCloseBtn)
	{				
		int nPosX = nWindowPosX + 515;
		int	nPosY = nWindowPosY + 6;
		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
		m_pCloseBtn->Render();			
	}
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
#else	 
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	if(m_pOpenBtn)
	{				
		int nPosX = nWindowPosX + 415;
		int	nPosY = nWindowPosY + 149;
		
		m_pOpenBtn->SetBtnPosition(nPosX, nPosY);	
		m_pOpenBtn->Render();			
	}	
	if(m_pCloseBtn)
	{				
		int nPosX = nWindowPosX + 490;
		int	nPosY = nWindowPosY + 7;
		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
		m_pCloseBtn->Render();			
	}
	// end 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
#endif

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	if(m_pNickNameBtn)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		int nPosX = nWindowPosX + 225;
		int	nPosY = nWindowPosY + 125;
#else
		int nPosX = nWindowPosX + 203;
		int	nPosY = nWindowPosY + 128;
#endif

		const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
		if(0 == strlen(myShuttleInfo.NickName) && !m_pNickNameBtn->IsShowWindow())
		{
			// ������ ����.
			m_pNickNameBtn->ShowWindow(TRUE);
		}
		else if((strlen(myShuttleInfo.NickName) > 0) && m_pNickNameBtn->IsShowWindow())
		{
			m_pNickNameBtn->ShowWindow(FALSE);
		}
		if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING && m_pNickNameBtn->IsShowWindow())
		{
			// �Ʒ��� ���� ����
			m_pNickNameBtn->ShowWindow(FALSE);
		}
		
		if(m_pNickNameBtn->IsShowWindow())
		{
			m_pNickNameBtn->SetBtnPosition(nPosX, nPosY);	
			m_pNickNameBtn->Render();
		}
		else
		{
			wsprintf(buff, "\\c%s\\c", myShuttleInfo.NickName);
			szSize = m_pFontNickName->GetStringSize(buff);
			m_pFontNickName->DrawText( nWindowPosX + WR_NICKNAME_START_X - szSize.cx, 
										nWindowPosY + WR_NICKNAME_START_Y, 
										GUI_FONT_COLOR, buff, 0 );			

		}
	}
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	
}

void CINFCharacterInfoExtend::RenderCharacterInfo()
{

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	char buff[MAX_PATH];	
	SIZE szSizeTmp={0,0};
	SIZE szSize = {0,0};
	memset(buff, 0x00, MAX_PATH);

	if(m_pInfo )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pInfo->Move(nWindowPosX,nWindowPosY + 153);
#else 
		m_pInfo->Move(nWindowPosX+22,nWindowPosY + 153);
#endif
		m_pInfo->Render() ;
	}

	RenderShowStatInfo() ;
	
	
	// �Ƹ� ����
	
	{
//		if(m_pAmmo )
//		{
//			m_pAmmo->Move(nWindowPosX+12,nWindowPosY + 48);
//			m_pAmmo->Render() ;
//		}
		
		CItemInfo * pArmorItemInfo = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER );

		CParamFactor paramCharctorParam = (*g_pShuttleChild->GetShuttleParamFactor());
		
		if(pArmorItemInfo)
		{
			CParamFactor * pParamArmor = pArmorItemInfo->GetEnchantParamFactor();
			if(pParamArmor)
			{
				paramCharctorParam += (*pParamArmor);
			}
		}		
		
		{
			sprintf( buff, STRMSG_C_080923_0200, ((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, 
							// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
							//&paramCharctorParam, TRUE ) / PROB256_MAX_VALUE)*100.0f );//"���� �⺻���� : %.2f%%"
							&paramCharctorParam, TRUE )));//"���� �⺻���� : %.2f%%"
			szSize = m_CharacInfoFont[0]->GetStringSize(buff);
			m_CharacInfoFont[0]->DrawText(nWindowPosX+INFO_START_X - szSize.cx, 
				nWindowPosY+OTHER_INFO_START_Y,
				GUI_FONT_COLOR, buff, 0 );
			
			sprintf( buff, STRMSG_C_080923_0200, ((float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo, 
							// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
							//&paramCharctorParam, FALSE ) / PROB256_MAX_VALUE)*100.0f );//"       ��޹��� : %.2f%%"
							&paramCharctorParam, FALSE )));//"       ��޹��� : %.2f%%"
			szSize = m_CharacInfoFont[0]->GetStringSize(buff);
			m_CharacInfoFont[1]->DrawText(nWindowPosX+INFO_START_X- szSize.cx, 
				nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL,
				GUI_FONT_COLOR, buff, 0 );

			sprintf( buff, STRMSG_C_080923_0200, //"ȸ�Ƿ� �⺻���� : %.2f%%"
				(CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, 
				// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
				//(pArmorItemInfo==NULL?NULL:pArmorItemInfo->GetRealItemInfo()), &paramCharctorParam, TRUE ) / PROB256_MAX_VALUE)*100.0f );
				(pArmorItemInfo==NULL?NULL:pArmorItemInfo->GetRealItemInfo()), &paramCharctorParam, TRUE )));
			szSize = m_CharacInfoFont[3]->GetStringSize(buff);
			m_CharacInfoFont[3]->DrawText(nWindowPosX+INFO_START_X+INFO_START_INTERVAL_X - szSize.cx, 
										nWindowPosY+OTHER_INFO_START_Y,
										GUI_FONT_COLOR, buff, 0 );
		
			sprintf( buff, STRMSG_C_080923_0200, //"       ��޹��� : %.2f%%"
				(CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, 
				// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
				//(pArmorItemInfo==NULL?NULL:pArmorItemInfo->GetRealItemInfo()), &paramCharctorParam, FALSE ) / PROB256_MAX_VALUE)*100.0f );
				(pArmorItemInfo==NULL?NULL:pArmorItemInfo->GetRealItemInfo()), &paramCharctorParam, FALSE )));
			szSize = m_CharacInfoFont[4]->GetStringSize(buff);
			m_CharacInfoFont[4]->DrawText(nWindowPosX+INFO_START_X+INFO_START_INTERVAL_X - szSize.cx, 
										nWindowPosY+OTHER_INFO_START_Y+INFO_INTERVAL,
										GUI_FONT_COLOR, buff, 0 );
		}
		
		
		
		
		
		if(g_pShuttleChild->m_pPrimaryWeapon)
		{
			CItemInfo *	pPrimaryItemInfo = g_pShuttleChild->m_pPrimaryWeapon->GetClassItemInfo();			
			CParamFactor paramPrimaryParam = (*g_pShuttleChild->GetShuttleParamFactor());		

			// 2008-11-11 by bhsohn Ȯ�� ���� �߸� ���� ���� ����
//			if(pPrimaryItemInfo)
//			{
//				//CParamFactor * pParamPrimaryWeapon = pPrimaryItemInfo->GetEnchantParamFactor();
//				CParamFactor * pParamPrimaryWeapon = pPrimaryItemInfo->GetParamFactor();
//				if(pParamPrimaryWeapon)
//				{
//					paramPrimaryParam += (*pParamPrimaryWeapon);
//				}
//			}		
			// end 2008-11-11 by bhsohn Ȯ�� ���� �߸� ���� ���� ����

			
			{
				// 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����

// 				sprintf( buff, STRMSG_C_080923_0200,//"����Ȯ�� : %.2f"
// 					(CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), 
// 					&paramPrimaryParam, 
// 					TRUE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart )*100)/255);	// ĳ������ ���� Ȯ��

				sprintf( buff, STRMSG_C_080923_0200,//"����Ȯ�� : %.2f"
						// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
						 //(CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(),&paramPrimaryParam,
						 //TRUE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level )*100)/255);	// ĳ������ ���� Ȯ��
						 (CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(),&paramPrimaryParam,
						 TRUE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level )));	// ĳ������ ���� Ȯ��

				// end 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����

				szSize = m_pFontAmmoWeapon[0]->GetStringSize(buff);
				m_pFontAmmoWeapon[0]->DrawText( nWindowPosX+AMMO_START_X-szSize.cx, 
												nWindowPosY + AMMO_START_Y, GUI_FONT_COLOR, buff, 0L );
			}
			{			
				//"�Ǿ : %.2f"
				float fTotalPierce = g_pShuttleChild->m_pPrimaryWeapon->GetTotalPierce(TRUE);					
				sprintf( buff, STRMSG_C_080923_0200, fTotalPierce);
				
				szSize = m_pFontAmmoWeapon[0]->GetStringSize(buff);
				m_pFontAmmoWeapon[0]->DrawText( nWindowPosX+AMMO_START_X-szSize.cx, 
												nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL), GUI_FONT_COLOR, buff, 0L );
			}

			{
				//"���ݷ� : %.2f ~ %.2f"
				sprintf( buff, STRMSG_C_080923_0201, 
					CAtumSJ::GetMinAttackC(&g_pShuttleChild->m_myShuttleInfo, g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &paramPrimaryParam, TRUE ), 
					CAtumSJ::GetMaxAttackC(&g_pShuttleChild->m_myShuttleInfo, g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &paramPrimaryParam, TRUE ) );
				szSize = m_pFontAmmoWeapon[0]->GetStringSize(buff);
				m_pFontAmmoWeapon[0]->DrawText( nWindowPosX+AMMO_START_X-szSize.cx, 
												nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL*2), GUI_FONT_COLOR, buff, 0L );
			}
			{
				sprintf( buff, STRMSG_C_080923_0201, CAtumSJ::GetMinAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, //"�ʴ���ݷ� : %.2f ~ %.2f"
					g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &paramPrimaryParam, TRUE ), 
					CAtumSJ::GetMaxAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, 
					g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo(), &paramPrimaryParam, TRUE ) );
				szSize = m_pFontAmmoWeapon[1]->GetStringSize(buff);
				m_pFontAmmoWeapon[1]->DrawText( nWindowPosX+AMMO_START_X-szSize.cx, nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL*3), GUI_FONT_COLOR, buff, 0L );				
				
			}			
		}
		else
		{
			float fStatPierce = GetDefaultPierce(TRUE);			
			float fItemPierce = g_pShuttleChild->GetShuttleParamFactor()->pfm_PIERCE_UP_01;						
			// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
			//fItemPierce = ((float)fItemPierce / (float)PROB256_MAX_VALUE *100.0f);

			// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
			fStatPierce = FloatSecRangeSharp(fStatPierce);
			fItemPierce = FloatSecRangeSharp(fItemPierce);

			sprintf( buff, STRMSG_C_080923_0200, (fStatPierce+fItemPierce));			

			szSize = m_pFontAmmoWeapon[0]->GetStringSize(buff);
			m_pFontAmmoWeapon[0]->DrawText( nWindowPosX+AMMO_START_X-szSize.cx, 
												nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL), GUI_FONT_COLOR, buff, 0L );
		}
		if(g_pShuttleChild->m_pSecondaryWeapon)
		{
			CItemInfo *	pSecondItemInfo = g_pShuttleChild->m_pSecondaryWeapon->GetClassItemInfo();			
			CParamFactor paramSecondParam = (*g_pShuttleChild->GetShuttleParamFactor());		

			// 2008-11-11 by bhsohn Ȯ�� ���� �߸� ���� ���� ����
//			if(pSecondItemInfo)
//			{
//				CParamFactor * pParamSecondWeapon = pSecondItemInfo->GetEnchantParamFactor();
//				if(pParamSecondWeapon)
//				{
//					paramSecondParam += (*pParamSecondWeapon);
//				}
//			}
			// end 2008-11-11 by bhsohn Ȯ�� ���� �߸� ���� ���� ����
			
			{
				// 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����

// 				sprintf( buff, STRMSG_C_080923_0200,//"����Ȯ�� : %.2f"
// 					(CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(),
// 					&paramSecondParam, FALSE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart)*100)/255 );	// ĳ������ ���� Ȯ��

				sprintf( buff, STRMSG_C_080923_0200,//"����Ȯ�� : %.2f"
						// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
						// (CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(),
						// &paramSecondParam, FALSE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level)*100)/255 );	// ĳ������ ���� Ȯ��
						(CAtumSJ::GetAttackProbabilityC(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(),
						 &paramSecondParam, FALSE, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, g_pShuttleChild->m_myShuttleInfo.Level)));	// ĳ������ ���� Ȯ��

				// end 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����

				szSize = m_pFontAmmoWeapon[2]->GetStringSize(buff);
				m_pFontAmmoWeapon[2]->DrawText( nWindowPosX + AMMO_START_X+AMMO_START_INTERVAL_X-szSize.cx, 
					nWindowPosY + AMMO_START_Y, GUI_FONT_COLOR, buff, 0L );
				
			}

			{				
				//"�Ǿ : %.2f"
				float fTotalPierce = g_pShuttleChild->m_pSecondaryWeapon->GetTotalPierce(FALSE);					
				sprintf( buff, STRMSG_C_080923_0200, fTotalPierce);				
				
				szSize = m_pFontAmmoWeapon[2]->GetStringSize(buff);
				m_pFontAmmoWeapon[2]->DrawText( nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X-szSize.cx, 
												nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL), GUI_FONT_COLOR, buff, 0L );
			}

			{
				//"���ݷ� : %.2f ~ %.2f"
				sprintf( buff, STRMSG_C_080923_0201, 
					CAtumSJ::GetMinAttackC(&g_pShuttleChild->m_myShuttleInfo,g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(), &paramSecondParam, FALSE ), 
					CAtumSJ::GetMaxAttackC(&g_pShuttleChild->m_myShuttleInfo, g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(), &paramSecondParam, FALSE ));				
				szSize = m_pFontAmmoWeapon[2]->GetStringSize(buff);
				m_pFontAmmoWeapon[2]->DrawText( nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X-szSize.cx, 
								nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL*2), GUI_FONT_COLOR, buff, 0L );
			}
			
			{
				sprintf( buff, STRMSG_C_080923_0201, CAtumSJ::GetMinAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, 
					g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(), &paramSecondParam, FALSE ), 
					CAtumSJ::GetMaxAttackPerSecondC(&g_pShuttleChild->m_myShuttleInfo, 
					g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo(), &paramSecondParam, FALSE ) );
				szSize = m_pFontAmmoWeapon[3]->GetStringSize(buff);
				m_pFontAmmoWeapon[3]->DrawText( nWindowPosX + AMMO_START_X+AMMO_START_INTERVAL_X-szSize.cx, 
					nWindowPosY + AMMO_START_Y+AMMO_INTERVAL*3, GUI_FONT_COLOR, buff, 0L );				
			}
		}
		else
		{			
			//"�Ǿ : %.2f"
			float fStatPierce = GetDefaultPierce(FALSE);	
			float fItemPierce = g_pShuttleChild->GetShuttleParamFactor()->pfm_PIERCE_UP_02;						
			// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
			//fItemPierce = ((float)fItemPierce / (float)PROB256_MAX_VALUE *100.0f);

			// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
			fStatPierce = FloatSecRangeSharp(fStatPierce);
			fItemPierce = FloatSecRangeSharp(fItemPierce);

			sprintf( buff, STRMSG_C_080923_0200, (fStatPierce+fItemPierce));			
			
			szSize = m_pFontAmmoWeapon[2]->GetStringSize(buff);
			m_pFontAmmoWeapon[2]->DrawText( nWindowPosX+AMMO_START_X+AMMO_START_INTERVAL_X-szSize.cx, 
				nWindowPosY + AMMO_START_Y+(AMMO_INTERVAL), GUI_FONT_COLOR, buff, 0L );					
		}
		
		
		wsprintf(buff, STRMSG_C_080923_0202, //"�� �� �� : %d%%(%dKg/%dKg)"
			(int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f ), 
			(int) g_pStoreData->GetTotalWeight(), (int)CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) );
		szSize = m_pFontAmmoWeapon[4]->GetStringSize(buff);
		m_pFontAmmoWeapon[4]->DrawText( nWindowPosX+AMMO_WEIGHT_START_X-szSize.cx, 
			nWindowPosY + AMMO_WEIGHT_START_Y, 
			GUI_FONT_COLOR, buff, 0L );
		
		// 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
		//wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), 			
		//CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1);	// -1�� ���� �������� �����ϴ� ���̴�.
		// 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) ����� 
		//CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
		//	wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), 			
		//	CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1);	// -1�� ���� �������� �����ϴ� ���̴�.
			wsprintf(buff, "%d/%d", (int)(g_pStoreData->GetTotalUseInven()), 			
			CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1);	// -1�� ���� �������� �����ϴ� ���̴�.
		//end 2009. 12. 17 by jskim ĳ��(�κ�/â�� Ȯ��) �����
		//end 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
		szSize = m_pFontAmmoWeapon[5]->GetStringSize(buff);
		m_pFontAmmoWeapon[5]->DrawText(nWindowPosX+INVEN_WEIGHT_START_X-szSize.cx,
								nWindowPosY + INVEN_WEIGHT_START_Y, GUI_FONT_COLOR, 
								buff, 0L);
		
	}
	
	{
//		m_pImgArenaTab->Move(nWindowPosX+12,nWindowPosY + 48);
//		m_pImgArenaTab->Render();
		
		const auto& ShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
		wsprintf(buff,STRMSG_C_070607_0108,ShuttleInfo.ArenaWin,ShuttleInfo.ArenaLose);		
		szSize = m_pFontGeneral->GetStringSize(buff);
		m_pFontGeneral->DrawText(ARENA_SCORE_X-szSize.cx,ARENA_SCORE_Y,GUI_FONT_COLOR,buff);
		UINT nPer = 0;
		if((ShuttleInfo.ArenaWin+ShuttleInfo.ArenaLose) > 0)
		{
			FLOAT fPer = (FLOAT)ShuttleInfo.ArenaWin/(FLOAT)(ShuttleInfo.ArenaWin+ShuttleInfo.ArenaLose);
			nPer = fPer*100;
		}
		wsprintf(buff,"\\c%d%%\\c",nPer);		
		szSize = m_pFontGeneral->GetStringSize(buff);
		m_pFontGeneral->DrawText(ARENA_SCORE_PERCENTAGE_X-szSize.cx,ARENA_SCORE_PERCENTAGE_Y,GUI_FONT_COLOR,buff);
		
		wsprintf(buff,STRMSG_C_070607_0107,ShuttleInfo.ArenaDisConnect);		
		szSize = m_pFontGeneral->GetStringSize(buff);
		m_pFontGeneral->DrawText(ARENA_COMPULSION_END_X-szSize.cx,ARENA_COMPULSION_END_Y,GUI_FONT_COLOR,buff);
		
		char buf[MAX_PATH];
		wsprintf(buf,"%d",ShuttleInfo.CumulativeWarPoint);
		MakeCurrencySeparator(buff,buf,3,',');		
		szSize = m_pFontGeneral->GetStringSize(buff);
		m_pFontGeneral->DrawText(ARENA_COMMULATION_POINT_X-szSize.cx,ARENA_COMMULATION_POINT_Y,GUI_FONT_COLOR,buff);
		
		wsprintf(buf,"%d",ShuttleInfo.WarPoint);
		MakeCurrencySeparator(buff,buf,3,',');		
		szSize = m_pFontGeneral->GetStringSize(buff);
		m_pFontGeneral->DrawText(ARENA_FUSIBLE_POINT_X-szSize.cx,ARENA_FUSIBLE_POINT_Y,GUI_FONT_COLOR,buff);
		
	}
	
	// ���緮�� ���� ���� �Ҹ��, �ν��� �ð�, �̵� �ӵ� ǥ��
	RenderWeightStats();	

}

void CINFCharacterInfoExtend::RenderSkillInfo()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	char buff[MAX_PATH];	
	SIZE szSizeTmp={0,0};
	memset(buff, 0x00, MAX_PATH);
	
	if(m_pUtc )
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pUtc->Move(nWindowPosX,nWindowPosY + 153);											  
#else							 
		m_pUtc->Move(nWindowPosX+22,nWindowPosY + 153);
#endif
		m_pUtc->Render() ;
	}
	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	for(int i=0;i<CHAR_UTC_TYPE_NUMBER;i++)
	{
		for(int j=0;j<MAX_CHAR_UTC_SLOT_NUMBER;j++)
		{
			if(m_pCharacterSkillDisplayInfo[i][j].pItem)
			{
				// 2005-11-28 by ispark, SourceIndex�� ����, ��ų�� SKILL_BASE_NUM���� �Ѵ�.
				char buf[64];
				// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//				wsprintf(buf, "%08d", SKILL_BASE_NUM(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
				wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SourceIndex));
				pIcon->SetIcon(buf,
					CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+1, CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+1, 1.0f);
				pIcon->Render();
				
				//						if( IS_COUNTABLE_ITEM(m_pCharacterSkillDisplayInfo[i][j].pItem->Kind) )
				//						{
				//							CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( 
				//								m_pCharacterSkillDisplayInfo[i][j].pItem->UniqueNumber );
				//							if( pItemInfo->CurrentCount > 1 )
				//							{
				// ������ �����ش�.
				if(m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SkillLevel > 0)
				{
					char buf[256];
					{
						// ��ų����
						//"LV%d"
						wsprintf(buf, STRMSG_C_080922_0202, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->SkillLevel);
						m_pFontItemNum[i]->DrawText(CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+33,
							CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j,
							GUI_FONT_COLOR,buf, 0L);
					}
					{
						// ��ų�̸�
						strncpy(buf, m_pCharacterSkillDisplayInfo[i][j].pItem->ItemInfo->ItemName, SIZE_MAX_ITEM_NAME);

						// 2008-10-23 by bhsohn ��ų�� �Ѿ�� ���� ó��
						g_pGameMain->TextReduce(m_pFontItemNum[i], 73, buf);

						m_pFontItemNum[i]->DrawText(CHAR_UTC_START_X+CHAR_UTC_INTERVAL_X*i+33,
							CHAR_UTC_START_Y+CHAR_UTC_INTERVAL_Y*j+14,
							GUI_FONT_COLOR,buf, 0L);
					}
				}
				//							}
				//						}
			}
		}
	}
	// 2006-07-27 by ispark
	//			if(m_pSelectIcon)
	//			{
	//				POINT ptCursor;
	//				GetCursorPos( &ptCursor );
	//				ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
	//				CheckMouseReverse(&ptCursor);				
	//
	//				m_pSelectIcon->Move(ptCursor.x-m_nRenderMoveIconIntervalWidth, ptCursor.y-m_nRenderMoveIconIntervalHeight);
	//				m_pSelectIcon->Render();
	//
	//			}
	m_pUtcScr->Move(UTC_SCROLL_BUTTON_START_X,
		nWindowPosY+UTC_SCROLL_BAR_START_Y+UTC_SCROLL_INTERVAL*m_nCurrentUTCScroll);
	m_pUtcScr->Render() ;
	
}

// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
void CINFCharacterInfoExtend::RenderPartnerInfo()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	char buff[MAX_PATH];	
	SIZE szSizeTmp={0,0};
	memset(buff, 0x00, MAX_PATH);
	
	if(m_pPartnerInfoBk)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		m_pPartnerInfoBk->Move(nWindowPosX,nWindowPosY + 153);
#else														  
		m_pPartnerInfoBk->Move(nWindowPosX + 22,nWindowPosY + 153);
#endif
		m_pPartnerInfoBk->Render();
	}

	PatnerInfo();
	PatnerDtail();

	sprintf(buff, "%d%", m_pExpScroll->GetScrollStep());	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	szSizeTmp = m_pFontGeneral->GetStringSize(buff);
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_pPartnerGiveExp->DrawText(nWindowPosX + PARTNER_DTAIL_INFO_X - szSizeTmp.cx, nWindowPosY + PARTNER_DTAIL_INFO_Y + PARTNER_DTAIL_INFO_Y_GAP, GUI_FONT_COLOR, buff, 0L);

	tPET_CURRENTINFO *psPetCurrentInfo = NULL;
	tPET_BASEDATA *psPetBaseData = NULL;
	tPET_LEVEL_DATA *psPetLevelData = NULL;
		
	GetPtr_AttachmentedPetInfo ( NULL , &psPetCurrentInfo , &psPetBaseData , &psPetLevelData );
	
	if(psPetLevelData && psPetCurrentInfo )
	{
		for (int i = 0 ; i < psPetLevelData->SlotCountSocket  ; i++)		
		{
			CItemInfo* temp  = g_pStoreData->FindItemInInventoryByUniqueNumber(psPetCurrentInfo->PetSocketItemUID[i]);
			if(temp)
			{
				RenderSocket(i,temp->ItemNum);
			}
			else if(g_pShuttleChild->GetPetManager()->GetSelectingCheckSocket(i))
			{
				m_pImgDissolutionTakeEffect[m_nEffStep]->Move(m_ptBkPos.x + PARTNER_SOCKET_POS_X - 10 ,	
					m_ptBkPos.y + PARTNER_SOCKET_POS_Y  + 2 + i * 35);
				m_pImgDissolutionTakeEffect[m_nEffStep]->Render();

			}	
			
			if (g_pShuttleChild->GetPetManager()->GetWearPetSocketPosition() && m_tBlinkTime.IsUnderMiddle() &&  ( psPetCurrentInfo->PetEnableSocketCount > i ) )
			{		
				
				m_pWearPositionBlink->Move(m_ptBkPos.x + PARTNER_SOCKET_POS_X , m_ptBkPos.y + PARTNER_SOCKET_POS_Y  + 12 + i * 35);
				
				DWORD dwColor = 0x00FFFFFF;
				DWORD alpha = 255* m_tBlinkTime.GetPositionRate();
				alpha = alpha << 24;
				m_pWearPositionBlink->SetColor( alpha | dwColor );
				m_pWearPositionBlink->Render();
				
		}
		}
	}
	

	switch( g_pShuttleChild->GetPetManager()->GetSelectSocket() )
	{
	case SOKET_TYPE_SKILL:
		{
			SkillRender();
		}
		break;
		
	case SOKET_TYPE_ITEM:
		{
			ItemRender();
		}
		break;
		
	case SOKET_TYPE_PRODUC:
		{
			ProductRender();
		}
		break;
	case SOKET_TYPE_SPEED:
		{
			//�ӵ� ���� üũ
		}
		break;
	}

	SocketOkRender();
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���	

}
//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void CINFCharacterInfoExtend::RenderSocket(int num,int nSelectItem)
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;

	if(nSelectItem)
	{
		CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
		char buf[20];
		wsprintf(buf, "%08d",SKILL_BASE_NUM(nSelectItem));
		pIcon->SetIcon( buf, m_ptBkPos.x + PARTNER_SOCKET_POS_X ,	m_ptBkPos.y + PARTNER_SOCKET_POS_Y  + 12 + num * 35, 1.0f );
 		pIcon->Render();

		if (g_pShuttleChild->GetPetManager()->GetSocketSelectingNum() >= 0)
		{
			m_pPartnerSocetSelect->Move( m_ptBkPos.x + PARTNER_SOCKET_POS_X - 10 ,	m_ptBkPos.y + PARTNER_SOCKET_POS_Y  + 2 + g_pShuttleChild->GetPetManager()->GetSocketSelectingNum() * 35);		
			m_pPartnerSocetSelect->Render();
			
		}
		
  	}
}
// end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
//2010-06-07 by jhahn ��Ʈ�� ������ �ý��� ����
void CINFCharacterInfoExtend::RenderItemSocket(int num,CItemInfo* nSelectItem,BOOL OnOff)
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	

	if(nSelectItem != NULL && nSelectItem->ItemInfo->SourceIndex != NULL)
	{
		CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
		char buf[20];
		wsprintf(buf, "%08d",nSelectItem->ItemInfo->SourceIndex);
		pIcon->SetIcon( buf, m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 11, m_ptBkPos.y + PARTNER_SOCKET_KIT_POS_Y + 7   + num * 35  , 1.0f );
		pIcon->Render();	

		if (g_pShuttleChild->GetPetManager()->GetSocketOnOff())
		{
			m_pUseSkillSocket->Move(m_ptBkPos.x + PARTNER_SOCKET_KIT_POS_X + 1, m_ptBkPos.y +PARTNER_SOCKET_KIT_POS_Y - 3   + num * 35);
			m_pUseSkillSocket->Render();

			
			
		}
  	}
}
//end 2010-06-07 by jhahn ��Ʈ�� ������ �ý��� ����

//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void CINFCharacterInfoExtend::RenderSkillSocket(int num,CSkillInfo* nSelectItem,BOOL OnOff)
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;
	

	if(nSelectItem != NULL && nSelectItem->ItemInfo->SourceIndex != NULL)
	{
		CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
		char buf[20];
		// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//		wsprintf(buf, "%08d",SKILL_BASE_NUM(nSelectItem->ItemInfo->SourceIndex));
		wsprintf(buf, "%08d",SKILL_BASE_SOURCEINDEX(nSelectItem->ItemInfo->SourceIndex));
	//	wsprintf(buf, "%08d",nSelectItem->ItemNum);
		pIcon->SetIcon( buf, 	m_ptBkPos.x + PARTNER_SOCKET_SKILL_POS_X + 10 + num * (PARTNER_SOCKET_SKILL_GAP + m_pPartnerSkillSocket->GetImgSize().x) ,
			m_ptBkPos.y + PARTNER_SOCKET_SKILL_POS_Y + 10, 1.0f );
		pIcon->Render();			
  	}

}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

void CINFCharacterInfoExtend::OnClickStat(int nPos)
{
	if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat>0)
	{
		// 2007-05-15 by bhsohn ��� ���� ���� ó��
		int nShowBtn = -1;
		
		MSG_FC_CHARACTER_USE_BONUSSTAT sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
		
		switch(nPos)
		{
		case STAT_ATTACK:
			{
				sMsg.KindOfStat = STAT_ATTACK_PART;
				nShowBtn= STAT_SHUTTLEINFO_ATTACK;
			}
			break;
		case STAT_DURABILITY:
			{
				sMsg.KindOfStat = STAT_DEFENSE_PART;
				nShowBtn= STAT_SHUTTLEINFO_DEFENCE;
			}
			break;
		case STAT_FUEL:
			{
				sMsg.KindOfStat = STAT_FUEL_PART;
				nShowBtn= STAT_SHUTTLEINFO_FUEL;
			}
			break;
		case STAT_INDUCTION:
			{
				sMsg.KindOfStat = STAT_SOUL_PART;
				nShowBtn= STAT_SHUTTLEINFO_SOUL;
			}
			break;
		case STAT_AVOID:
			{
				sMsg.KindOfStat = STAT_DODGE_PART;
				nShowBtn= STAT_SHUTTLEINFO_DODGE;
			}
			break;
		case STAT_KALISMA:
			{
				sMsg.KindOfStat = STAT_SHIELD_PART;
				nShowBtn= STAT_SHUTTLEINFO_SHIELD;
			}
			break;
		}		
		
		if(nShowBtn >= 0)
		{
			if(m_bStatShow[nShowBtn])
			{
				int nType = T_FC_CHARACTER_USE_BONUSSTAT;
				memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
				memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
				
#ifdef CUSTOM_OPTIMIZER_HSSON
				if( g_cCustomOptimizer.m_bStatGoGo )
				{
					for( int i = 0; i < 19; ++i )
						g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
				}
#endif // CUSTOM_OPTIMIZER_HSSON

				g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
				g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-09-18 by ispark, �޼��� ������ ��ٸ���.				
				
			}									
		}		
	}		
}
BOOL CINFCharacterInfoExtend::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT ptSize;
	ptSize.x = m_pInformationBK->GetMaxPos().x - m_pInformationBK->GetMinPos().x;
	ptSize.y = m_pInformationBK->GetMaxPos().y - m_pInformationBK->GetMinPos().y;
#else  
	POINT ptSize = m_pInoBk->GetImgSize();
#endif
	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+ptSize.x))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+20)))
	{
		return TRUE;
	}
	return FALSE;

}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
BOOL CINFCharacterInfoExtend::IsMouseCaps2(POINT ptPos)
{

	POINT ptSkillBakPos = m_ptSkillBkPos ;
	POINT ptSkillSize;
	ptSkillSize.x = ptSkillSize.y = 0;

	for (int i = 0 ; i <  3 ; i++)
	{
		ptSkillSize.x += m_pPartnerSkillShop[i]->GetImgSize().x;
		ptSkillSize.y += m_pPartnerSkillShop[i*3]->GetImgSize().y;
	}
	ptSkillSize.x += PARTNER_SKILL_POPUPSHOP_X_GAP;
	ptSkillSize.y += PARTNER_SKILL_POPUPSHOP_Y_GAP;
	if ((ptPos.x  >= ptSkillBakPos.x + PARTNER_SKILL_POPUPSHOP_X && (ptPos.x <= ptSkillBakPos.x+ptSkillSize.x + PARTNER_SKILL_POPUPSHOP_X))
		&& (ptPos.y >= ptSkillBakPos.y + PARTNER_SKILL_POPUPSHOP_Y && (ptPos.y <= ptSkillBakPos.y+20 + PARTNER_SKILL_POPUPSHOP_Y)))
	{
		return TRUE;
	}
	return FALSE;
}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void CINFCharacterInfoExtend::UpdateBtnPos()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		������ ���ݷ��� ���´�.
/// \author		// 2008-09-22 by bhsohn EP3 ĳ���� â
/// \date		2008-09-24 ~ 2008-09-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::GetItemAttackInfo(CItemInfo *pItemInfo, char* o_pDstBuff)
{
	CItemInfo*	pRefItemInfo = pItemInfo;
	ITEM		*pRefITEM = pItemInfo->GetItemInfo();
	RARE_ITEM_INFO *pRefPrefixRareInfo = pRefItemInfo->GetPrefixRareInfo();
	RARE_ITEM_INFO *pRefSuffixRareInfo = pRefItemInfo->GetSuffixRareInfo();
	CParamFactor *pRefEnchant = pRefItemInfo->GetEnchantParamFactor();
	BOOL bEnableItem = IsEnableItem( pRefItemInfo->GetRealItemInfo() );

	float fEnchantAttackMin = pRefEnchant ? pRefEnchant->pfm_MINATTACK_01+ pRefEnchant->pfm_MINATTACK_02 : 0;
	float fEnchantAttackMax = pRefEnchant ? pRefEnchant->pfm_MAXATTACK_01+ pRefEnchant->pfm_MAXATTACK_02 : 0;
	float fRareInfoAttackMin = 0;
	float fRareInfoAttackMax = 0;
	BYTE nEnchantShotNum = pRefEnchant ? pRefEnchant->pfp_SHOTNUM_01 : 0;
	BYTE nEnchantMultiNum = pRefEnchant ? pRefEnchant->pfp_MULTINUM_02 : 0;
	BYTE nRareInfoShotNum = 0;
	BYTE nRareInfoMultiNum = 0;
	if(pRefPrefixRareInfo)
	{
		for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if( pRefPrefixRareInfo->DesParameter[i] == DES_MINATTACK_01 || 
				pRefPrefixRareInfo->DesParameter[i] == DES_MINATTACK_02)
			{
				fRareInfoAttackMin += pRefPrefixRareInfo->ParameterValue[i];
			}
			else if(pRefPrefixRareInfo->DesParameter[i] == DES_MAXATTACK_01 ||
				pRefPrefixRareInfo->DesParameter[i] == DES_MAXATTACK_02)
			{
				fRareInfoAttackMax += pRefPrefixRareInfo->ParameterValue[i];
			}
			else if(pRefPrefixRareInfo->DesParameter[i] == DES_SHOTNUM_01  ||
				pRefPrefixRareInfo->DesParameter[i] == DES_SHOTNUM_02)
			{
				nRareInfoShotNum += (BYTE)pRefPrefixRareInfo->ParameterValue[i];
			}
			else if(//pRefPrefixRareInfo->DesParameter[i] == DES_MULTINUM_01 ||
				pRefPrefixRareInfo->DesParameter[i] == DES_MULTINUM_02)
			{
				nRareInfoMultiNum += (BYTE)pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if(pRefSuffixRareInfo)
	{
		for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if( pRefSuffixRareInfo->DesParameter[i] == DES_MINATTACK_01 || 
				pRefSuffixRareInfo->DesParameter[i] == DES_MINATTACK_02)
			{
				fRareInfoAttackMin += pRefSuffixRareInfo->ParameterValue[i];
			}
			else if(pRefSuffixRareInfo->DesParameter[i] == DES_MAXATTACK_01 ||
				pRefSuffixRareInfo->DesParameter[i] == DES_MAXATTACK_02)
			{
				fRareInfoAttackMax += pRefSuffixRareInfo->ParameterValue[i];
			}
			else if(pRefSuffixRareInfo->DesParameter[i] == DES_SHOTNUM_01 ||
				pRefSuffixRareInfo->DesParameter[i] == DES_SHOTNUM_02)
			{
				nRareInfoShotNum += (BYTE)pRefSuffixRareInfo->ParameterValue[i];
			}
			else if(//pRefSuffixRareInfo->DesParameter[i] == DES_MULTINUM_01 ||
				pRefSuffixRareInfo->DesParameter[i] == DES_MULTINUM_02)
			{
				nRareInfoMultiNum += (BYTE)pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}

	sprintf( o_pDstBuff, STRMSG_C_080923_0204, pRefITEM->AbilityMin );//"���� : (%.0f"
	if(fEnchantAttackMin > 0)
		sprintf( o_pDstBuff, "%s\\e[+%.0f%%]\\e", o_pDstBuff, fEnchantAttackMin*100.0f );
	else if(fEnchantAttackMin < 0) 
		sprintf( o_pDstBuff, "%s\\e[%.0f%%]\\e", o_pDstBuff, fEnchantAttackMin*100.0f );
	if(fRareInfoAttackMin > 0)
		sprintf( o_pDstBuff, "%s\\g[+%.0f%%]\\g", o_pDstBuff, fRareInfoAttackMin*100.0f );
	else if(fRareInfoAttackMin < 0) 
		sprintf( o_pDstBuff, "%s\\g[%.0f%%]\\g", o_pDstBuff, fRareInfoAttackMin*100.0f );

	sprintf( o_pDstBuff, "%s ~ %.0f", o_pDstBuff, pRefITEM->AbilityMax );
	if(fEnchantAttackMax > 0)
		sprintf( o_pDstBuff, "%s\\e[+%.0f%%]\\e", o_pDstBuff, fEnchantAttackMax*100.0f );
	else if(fEnchantAttackMin < 0) 
		sprintf( o_pDstBuff, "%s\\e[%.0f%%]\\e", o_pDstBuff, fEnchantAttackMax*100.0f );
	if(fRareInfoAttackMax > 0)
		sprintf( o_pDstBuff, "%s\\g[+%.0f%%]\\g", o_pDstBuff, fRareInfoAttackMax*100.0f );
	else if(fRareInfoAttackMin < 0) 
		sprintf( o_pDstBuff, "%s\\g[%.0f%%]\\g", o_pDstBuff, fRareInfoAttackMax*100.0f );
	sprintf( o_pDstBuff, "%s) X (%d",o_pDstBuff,pRefITEM->ShotNum);

	if(nEnchantShotNum>0)
		sprintf( o_pDstBuff, "%s\\e[+%d]\\e", o_pDstBuff, nEnchantShotNum );
	else if(nEnchantShotNum<0)
		sprintf( o_pDstBuff, "%s\\e[%d]\\e", o_pDstBuff, nEnchantShotNum );
	if(nRareInfoShotNum>0)
		sprintf( o_pDstBuff, "%s\\g[+%d]\\g", o_pDstBuff, nRareInfoShotNum );
	else if(nRareInfoShotNum<0)
		sprintf( o_pDstBuff, "%s\\g[%d]\\g", o_pDstBuff, nRareInfoShotNum );
	sprintf( o_pDstBuff, "%s X %d",o_pDstBuff,pRefITEM->MultiNum);

	if(nEnchantMultiNum>0)
		sprintf( o_pDstBuff, "%s\\e[+%d]\\e", o_pDstBuff, nEnchantMultiNum );
	else if(nEnchantMultiNum<0)
		sprintf( o_pDstBuff, "%s\\e[%d]\\e", o_pDstBuff, nEnchantMultiNum );
	if(nRareInfoMultiNum>0)
		sprintf( o_pDstBuff, "%s\\g[+%d]\\g", o_pDstBuff, nRareInfoMultiNum );
	else if(nRareInfoMultiNum<0)
		sprintf( o_pDstBuff, "%s\\g[%d]\\g", o_pDstBuff, nRareInfoMultiNum );
	strcat(o_pDstBuff, ")");
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-12 ~ 2004-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCharacterInfoExtend::IsEnableItem(ITEM* pITEM)
{
	if( pITEM->ReqGearStat.AttackPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart &&
		pITEM->ReqGearStat.ShieldPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart &&
		pITEM->ReqGearStat.DodgePart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart &&
		pITEM->ReqGearStat.DefensePart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart &&
		pITEM->ReqGearStat.FuelPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart &&
		pITEM->ReqMinLevel <= g_pShuttleChild->m_myShuttleInfo.Level &&
		pITEM->ReqMaterial <= g_pShuttleChild->m_myShuttleInfo.Material &&
		CompareBitFlag( pITEM->ReqRace , g_pShuttleChild->m_myShuttleInfo.Race ) &&
		pITEM->ReqGearStat.SoulPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart &&
		pITEM->ReqSP <= g_pShuttleChild->m_myShuttleInfo.SP &&
		CompareBitFlag( pITEM->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind ) )
	{
		if(	pITEM->ReqMaxLevel == 0 ||
			pITEM->ReqMaxLevel >= g_pShuttleChild->m_myShuttleInfo.Level)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}		
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-12 ~ 2004-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFCharacterInfoExtend::GetDefaultPierce(BOOL bIsPrimaryAttack)
{	
	int nAttackPart = g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart;
	float LevelPierce = CAtumSJ::GetLevelStatValue(min(COUNT_MAX_STAT_POINT,nAttackPart))->AttackPart_PierceAttackProbability;	

	CParamFactor paramCharctorParam = (*g_pShuttleChild->GetShuttleParamFactor());	
	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)	
	//LevelPierce = ((float)LevelPierce / (float)PROB256_MAX_VALUE *100.0f);

	return LevelPierce;
	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		
/// \author		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::GetDefenceRateString(BOOL bIsPrimaryAttack, char* pDefaultRate, char* pArmorRate, char* pPetRate)
{
	CItemInfo* pArmorItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER ); 
	CItemInfo* pPetItem = g_pStoreData->FindItemInInventoryByWindowPos(POS_PET);	// �ð��� �Ǽ��縮
	
	CParamFactor paramCharctorParam = (*g_pShuttleChild->GetShuttleParamFactor());	
	float fArmorDefence = 0.0f;
	float fArmorDefenceRate = 0.0f;

	if(pArmorItem)
	{
		CParamFactor *pArmorParamFacotor = pArmorItem->GetEnchantParamFactor();
		if(pArmorParamFacotor)
		{
			paramCharctorParam += (*pArmorParamFacotor);
			fArmorDefence = (bIsPrimaryAttack?pArmorParamFacotor->pfp_DEFENSE_01:pArmorParamFacotor->pfp_DEFENSE_02);		
			// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
			//fArmorDefenceRate = (fArmorDefence /PROB256_MAX_VALUE) * 100.0f;
			fArmorDefenceRate = fArmorDefence;
		}
	}
	
	float fAllDefence = 0.0f;

	fAllDefence = (float)CAtumSJ::GetDefenseC( &g_pShuttleChild->m_myShuttleInfo,  
			&paramCharctorParam, bIsPrimaryAttack );
	
	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)	
//	float fAllDefenceRate = (fAllDefence /PROB256_MAX_VALUE) * 100.0f;					
	float fAllDefenceRate = fAllDefence;					
	
	// 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����

// 	float fDefaultDefence = 
// 		(float)CAtumSJ::GetDefenseC_DecreaseRate(g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart)*PROB256_MAX_VALUE;
	float fDefaultDefence = 
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%) ���Ȱ��õȰ� �׳� 255�� 
		(float)CAtumSJ::GetDefenseC_DecreaseRate(g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart, g_pShuttleChild->m_myShuttleInfo.Level)*255.0f;
		//(float)CAtumSJ::GetDefenseC_DecreaseRate(g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart, g_pShuttleChild->m_myShuttleInfo.Level)*PROB100_MAX_VALUE;

	// end 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����
	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%) ���Ȱ��õȰ� �׳� 255�� 
	float fDefaultDefenceRate = fDefaultDefence / 2.55f;
	//float fDefaultDefenceRate = (fDefaultDefence/PROB100_MAX_VALUE) * 100.0f;
	
	// 2011-07-04 by hsson ����â ���� ���콺�� ���� �� ������ ��ġ ���� ����
	float fPetEnchant = fAllDefence - fDefaultDefenceRate - fArmorDefence;
	// end 2011-07-04 by hsson ����â ���� ���콺�� ���� �� ������ ��ġ ���� ����

	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
	//float fPetEnchantRate = (fPetEnchant /PROB256_MAX_VALUE) * 100.0f;
	float fPetEnchantRate = fPetEnchant;

	char chTmp[128];
	memset(chTmp, 0x00, 128);	

	// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
	fAllDefenceRate = FloatSecRangeSharp(fAllDefenceRate);
	fDefaultDefenceRate = FloatSecRangeSharp(fDefaultDefenceRate);
	fPetEnchantRate = FloatSecRangeSharp(fPetEnchantRate);
	
	if(fDefaultDefenceRate > 0)
	{
		sprintf( pDefaultRate, STRMSG_C_080923_0200, fDefaultDefenceRate);//"���� �⺻���� : %.2f%%"
	}
	if(fArmorDefenceRate > 0)
	{
		sprintf(chTmp, STRMSG_C_080923_0200, fArmorDefenceRate);
		sprintf(pArmorRate, "\\e[+%s]\\e", chTmp);
	}
	
	if(fPetEnchant > 0)
	{
		sprintf(chTmp, STRMSG_C_080923_0200, fPetEnchantRate);
		sprintf(pPetRate, "\\g[+%s]\\g", chTmp);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		
/// \author		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::GetDodgeRateString(BOOL bIsPrimaryAttack, char* pDefaultRate, char* pArmorRate, char* pPetRate)
{
	CItemInfo* pArmorItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_CENTER ); 
	CItemInfo* pPetItem = g_pStoreData->FindItemInInventoryByWindowPos(POS_PET);	// �ð��� �Ǽ��縮
	CParamFactor paramCharctorParam = (*g_pShuttleChild->GetShuttleParamFactor());	
	float fArmorDefence = 0.0f;
	float fArmorDefenceRate = 0.0f;
	if(pArmorItem)
	{
		CParamFactor *pArmorParamFacotor = pArmorItem->GetEnchantParamFactor();
		if(pArmorParamFacotor)
		{
			paramCharctorParam += (*pArmorParamFacotor);
			
			fArmorDefence = (bIsPrimaryAttack?pArmorParamFacotor->pfp_DEFENSEPROBABILITY_01:pArmorParamFacotor->pfp_DEFENSEPROBABILITY_02);		
			// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
			//fArmorDefenceRate = (fArmorDefence /PROB256_MAX_VALUE) * 100.0f;
			fArmorDefenceRate = fArmorDefence;
		}
	}
	
	float fAllDefence = 0.0f;

	fAllDefence = CAtumSJ::GetDefenseProbabilityC( &g_pShuttleChild->m_myShuttleInfo, 
			(pArmorItem==NULL?NULL:pArmorItem->GetRealItemInfo()), 
			&paramCharctorParam, bIsPrimaryAttack );

	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
	//float fAllDefenceRate = (fAllDefence /PROB256_MAX_VALUE) * 100.0f;
	float fAllDefenceRate = fAllDefence;
	
	// 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����

// 	float fDefaultDefence = 		
// 		((float)CAtumSJ::GetDefenseProbabilityC_Rate(g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart)) 
// 		* PROB256_MAX_VALUE;

	float fDefaultDefence = CAtumSJ::GetDefenseProbabilityC_Rate(
		g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart, g_pShuttleChild->m_myShuttleInfo.Level) * 255.0;

	// end 2010. 02. 19 by ckPark 100�����̸��Ͻ� 300�����ʰ����� ����Ǵ� ���� ����
	
	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%) ���ȿ� ���� ���� 255�� ��.
	float fDefaultDefenceRate = fDefaultDefence / 2.55f;						
	//float fDefaultDefenceRate = (fDefaultDefence/PROB100_MAX_VALUE) * 100;

	// 2011-07-04 by hsson ����â ���� ���콺�� ���� �� ������ ��ġ ���� ����	
	float fPetEnchant = fAllDefence - fDefaultDefenceRate - fArmorDefence;
	// end 2011-07-04 by hsson ����â ���� ���콺�� ���� �� ������ ��ġ ���� ����

	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
	//float fPetEnchantRate = (fPetEnchant /PROB256_MAX_VALUE) * 100.0f;
	float fPetEnchantRate = fPetEnchant;

	// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���	
	fAllDefenceRate = FloatSecRangeSharp(fAllDefenceRate);
	fDefaultDefenceRate = FloatSecRangeSharp(fDefaultDefenceRate);
	fPetEnchantRate = FloatSecRangeSharp(fPetEnchantRate);

	char chTmp[128];
	memset(chTmp, 0x00, 128);	
	
	if(fDefaultDefenceRate > 0)
	{
		sprintf( pDefaultRate, STRMSG_C_080923_0200, fDefaultDefenceRate);//"���� �⺻���� : %.2f%%"
	}
	if(fArmorDefence > 0)
	{
		sprintf(chTmp, STRMSG_C_080923_0200, fArmorDefenceRate);
		sprintf(pArmorRate, "\\e[+%s]\\e", chTmp);
	}
	
	if(fPetEnchant > 0)
	{
		sprintf(chTmp, STRMSG_C_080923_0200, fPetEnchantRate);
		sprintf(pPetRate, "\\g[+%s]\\g", chTmp);
	}
	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		// ���緮�� ���� ���� �Ҹ��, �ν��� �ð�, �̵� �ӵ� ǥ��
/// \author		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::RenderWeightStats()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;

	char chFuel[256],chBooster[256],chSpeed[256];
	
	memset(chFuel, 0x00, 256);
	memset(chBooster, 0x00, 256);
	memset(chSpeed, 0x00, 256);

	int nWeight = (int) (g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo) *100.0f );
	if(nWeight < 70)
	{
		wsprintf(chFuel, STRMSG_C_081014_0200);		// "����"
		wsprintf(chBooster, STRMSG_C_081014_0200);	// "����"
		wsprintf(chSpeed, STRMSG_C_081014_0200);	// "����"
	}
	else if((nWeight >= 70) && (nWeight < 80))
	{
		wsprintf(chFuel, STRMSG_C_081014_0201);		// "���� �Һ� 2�� ����"
		wsprintf(chBooster, STRMSG_C_081014_0200);	// "����"
		wsprintf(chSpeed, STRMSG_C_081014_0200);	// "����"
	}
	else if((nWeight >= 80) && (nWeight < 90))
	{
		wsprintf(chFuel, STRMSG_C_081014_0201);		// "���� �Һ� 2�� ����"
		wsprintf(chBooster, STRMSG_C_081014_0202);	// "�ν��� ��� �Ұ�"
		wsprintf(chSpeed, STRMSG_C_081014_0200);	// "����"
	}
	else if(nWeight >= 90)
	{
		wsprintf(chFuel, STRMSG_C_081014_0201);		// "���� �Һ� 2�� ����"
		wsprintf(chBooster, STRMSG_C_081014_0202);	// "�ν��� ��� �Ұ�"
		wsprintf(chSpeed, STRMSG_C_081014_0203);	// "���� �ӵ� �̵�"
	}	
	
	SIZE szSize = {0,0};
	{		
		// ���� �Ҹ�
		szSize = m_pFontGeneral->GetStringSize(chFuel);
		m_pFontGeneral->DrawText(nWindowPosX+WEIGHT_FUEL_START_X-szSize.cx,
								nWindowPosY + WEIGHT_FUEL_START_Y, GUI_FONT_COLOR, 
								chFuel, 0L);
	}

	{		
		// �ν��� �ð�
		szSize = m_pFontGeneral->GetStringSize(chBooster);
		m_pFontGeneral->DrawText(nWindowPosX+WEIGHT_BOOSTER_START_X-szSize.cx,
								nWindowPosY + WEIGHT_BOOSTER_START_Y, GUI_FONT_COLOR, 
								chBooster, 0L);
	}
	{		
		// �̵� �ӵ�
		szSize = m_pFontGeneral->GetStringSize(chSpeed);
		m_pFontGeneral->DrawText(nWindowPosX+WEIGHT_SPEED_START_X-szSize.cx,
								nWindowPosY + WEIGHT_SPEED_START_Y, GUI_FONT_COLOR, 
								chSpeed, 0L);
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn ���� ��ŷ �ý���
/// \date		2009-02-24 ~ 2009-02-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCharacterInfoExtend::OnClickNickNameBtn()
{
	char chTmp[512];
	ZERO_MEMORY(chTmp);

	wsprintf(chTmp, STRMSG_C_090224_0201);
	
	// �г��� ����â
	g_pGameMain->m_pInfWindow->AddMsgBox(chTmp, _Q_CREATE_NICKNAME_OKCANCEL);

}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

void CINFCharacterInfoExtend::SetSockeitem(int Sockenum,CItemInfo* pItemInfo,UID64_t UniqueNum)
{	
	if (m_bShowSoketSetting == FALSE)
	{
		m_bShowSoketSetting = TRUE;	
		m_bCheckSoketSetting = TRUE;
		m_nSockenum = Sockenum;
		m_uCheckSocketNum = UniqueNum;
		g_pShuttleChild->GetPetManager()->SetSocketUidNumber(UniqueNum);

		char chMsg[256];
		wsprintf(chMsg, STRMSG_C_110927_0001, pItemInfo->ItemInfo->ItemName); 


		g_pGameMain->m_pInfWindow->AddMsgBox(  chMsg, _Q_SETTING_SOCKET_OKCANCLE );
	}
	else
	{
		m_bShowSoketSetting = FALSE;	
		m_bCheckSoketSetting = FALSE;
	}

	
					
	return;
}
void CINFCharacterInfoExtend::SetSkillSoket(int Sockenum,CSkillInfo* pItemInfo,UID64_t UniqueNum)
{
	CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 		
	if (!pAllItem)
		return;
	tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );
	if (!tempCurrentInfo)
		return;
	tPET_LEVEL_DATA* tempLevelData = g_pDatabase->GetPtr_PetLevelData( tempCurrentInfo->PetIndex,tempCurrentInfo->PetLevel );
	if (!tempLevelData)
		return;
	tPET_BASE_ALL_DATA* tempBaseData = g_pDatabase->GetPtr_PetAllDataByIndex(tempCurrentInfo->PetIndex);
	if (!tempBaseData)
		return;
	BOOL setting = FALSE;

	for (int i = 0; i <6 ; i++)
	{
		// 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����
		//if( pItemInfo == FALSE || tempCurrentInfo->PetAutoSkill.ItemNum[i] == pItemInfo->ItemNum)
		if( pItemInfo && tempCurrentInfo->PetAutoSkill.ItemNum[i] == pItemInfo->ItemNum)
			return;
		else
			setting = TRUE;
		// end 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����

	}
	

	if (setting)
	{
		MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT msg;					
		msg.ItemUniqueNumber = pAllItem->UniqueNumber;
		
		msg.SocketIndex = g_pShuttleChild->GetPetManager()->GetSocketNum(); 
		
		memcpy(&msg.PetAutoSkill,&tempCurrentInfo->PetAutoSkill,sizeof(SPET_AUTOSKILL_SLOT_DATA));
		
		// 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����
		if(pItemInfo)
		msg.PetAutoSkill.ItemNum[Sockenum] = pItemInfo->ItemNum;
		else
			msg.PetAutoSkill.ItemNum[Sockenum] = 0;
		// end 2012-12-27 by jhjang ��Ʈ�� ��ų ���� �Ҽ� �ְ� ����

		msg.PetSocketItemUID = tempCurrentInfo->PetSocketItemUID[msg.SocketIndex];
	g_pFieldWinSocket->SendMsg( T_FC_ITEM_PET_SET_AUTOSKILL_SLOT, (char*)(&msg), sizeof(MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT) );	
	}
	

}
void CINFCharacterInfoExtend::SetItemSoket(int Sockenum,CItemInfo* pItemInfo,UID64_t UniqueNum)
{
	CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 		
	if (!pAllItem)
		return;
	tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );	
	if (!tempCurrentInfo)
		return;
	tPET_LEVEL_DATA *psPetLevelData = g_pDatabase->GetPtr_PetLevelData ( tempCurrentInfo->PetIndex , tempCurrentInfo->PetLevel );
	if (!psPetLevelData)
		return;
	
	INT		HpItemNum = tempCurrentInfo->PetKitHP.ItemNum ;
	INT		ShieldItemNum = tempCurrentInfo->PetKitShield.ItemNum;
	INT		SpItemNum = tempCurrentInfo->PetKitSP.ItemNum;

	BOOL SendMsgTrue = FALSE;
	if ((Sockenum == 0 ) && (pItemInfo->ItemInfo->ArrDestParameter[1] == DES_PET_SLOT_ITEM_AUTOKIT_HP ) )
	{
		if (psPetLevelData->KitLevelHP >= pItemInfo->ItemInfo->ArrParameterValue[1])
		{	
			HpItemNum	= pItemInfo->ItemNum;
			SendMsgTrue = TRUE;
		}	
		else 
		{
			SendMsgTrue = FALSE;
		}
	}
	
	else if ((Sockenum == 1 ) && (pItemInfo->ItemInfo->ArrDestParameter[1] == DES_PET_SLOT_ITEM_AUTOKIT_SHIELD ) )
	{
		if (psPetLevelData->KitLevelShield >= pItemInfo->ItemInfo->ArrParameterValue[1])
		{			
			ShieldItemNum	= pItemInfo->ItemNum;			
			SendMsgTrue = TRUE;
		}		
		else 
		{
			SendMsgTrue = FALSE;
		}
	}
	
	else if ((Sockenum == 2 ) && (pItemInfo->ItemInfo->ArrDestParameter[1] == DES_PET_SLOT_ITEM_AUTOKIT_SP ) )
	{
		if (psPetLevelData->KitLevelSP >= pItemInfo->ItemInfo->ArrParameterValue[1])
		{
			SpItemNum	= pItemInfo->ItemNum;			
			SendMsgTrue = TRUE;
		}	
		else 
		{
			SendMsgTrue = FALSE;
		}
	}

	if (SendMsgTrue)
	{
		MSG_FC_ITEM_PET_SET_KIT_SLOT msg;					
		msg.ItemUniqueNumber = pAllItem->UniqueNumber;
		
		msg.PetSocketItemUID = tempCurrentInfo->PetSocketItemUID[g_pShuttleChild->GetPetManager()->GetSocketNum()];
		msg.SocketIndex = g_pShuttleChild->GetPetManager()->GetSocketNum(); 
		msg.PetKitHP.ItemNum = HpItemNum;
		msg.PetKitHP.TriggerValue = tempCurrentInfo->PetKitHP.TriggerValue;
		
		msg.PetKitShield.ItemNum = ShieldItemNum;
		msg.PetKitShield.TriggerValue = tempCurrentInfo->PetKitShield.TriggerValue;
		
		msg.PetKitSP.ItemNum = SpItemNum;
		msg.PetKitSP.TriggerValue = tempCurrentInfo->PetKitSP.TriggerValue;	
		
		g_pFieldWinSocket->SendMsg( T_FC_ITEM_PET_SET_KIT_SLOT, (char*)(&msg), sizeof(MSG_FC_ITEM_PET_SET_KIT_SLOT) );	
	


	}

	return ;	
}	 
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

