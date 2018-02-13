#if !defined(ATUMGLOBAL_H__017A08F0_77F1_4553_AF1A_FDFE85643262__INCLUDED_)
#define ATUMGLOBAL_H__017A08F0_77F1_4553_AF1A_FDFE85643262__INCLUDED_

#include "stdafx.h"
#include "math.h"
//#include "AtumParam.h"
//#include "AtumDataLoader.h"
#include <FieldWinSocket.h>
// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
#include <FieldWinSocketManager.h>

class CParamFactor;
class CD3DHanFont;
// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
//extern CFieldWinSocket			*g_pFieldWinSocket;
extern CFieldWinSocketManager	*g_pFieldWinSocket;

// ���� ���ϱ� ( (x1,y1,z1) * (x2,y2,z2) = (x1*x2, y1*y2, z1*z2)
#define MULTI_VECTOR(X,Y)		(D3DXVECTOR3(X.x*Y.x, X.y*Y.y, X.z*Y.z)) 

inline DWORD FtoDW( FLOAT f )	{ return *((DWORD*)&f); }

void	FAR PASCAL BMP_To_JPG(char *Source, char *Target);// AtumJPG.LIB

void SetParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
float GetParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam);
void ReleaseParamFactor_DesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
void ParamFactor_plus_ParamFactor( CParamFactor &destParamFactor, CParamFactor &srcParamFactor );
void SetItem_ParamFactor( ITEM & item, CParamFactor &paramFactor );
BOOL CompareIntRange( INT_RANGE &range, INT nValue );
BOOL CompareBitFlag( USHORT nSource, USHORT nValue );
BOOL CompareItemCount( ITEM_W_COUNT &itemCount);
unsigned short GGetUnitUpgradeLevel(unsigned short i_nUnitKind);
unsigned short GGetUnitKindHexToDeimal(unsigned short i_nUnitKind);
int GetUnitTypeDecimal(unsigned short nUnitKind); 
float Random(float max, float min);
int GetCreateUnitWeapon11(int nUnitKind);
int GetCreateUnitWeapon12(int nUnitkind);	// 2006-01-02 by ispark
int GetCreateUnitEngine(int nUnitKind);
char* GetGuildRankName( BYTE nGuildRank );
int GetGearTrace(UINT nUnitKind);
void MakeCurrencySeparator(char *pDest, char *pSrc, int nSepInterval, char cSepChar);

// nDeegree is level of detail, nDefenseItemNum is SourceIndex
UINT GetUnitNum(int nDegree, int nDefenseItemNum, int nUnitKind, BOOL bCharacter);

char* GetWaterTypeAtMap( USHORT nMapIndex );
BOOL IsPkEnableMap( USHORT nMapIndex);
BOOL IsTileMapRenderEnable( USHORT nMapIndex );
// 2007-04-19 by bhsohn �浹 ó�� ����
BOOL IsTileMapTickEnable(USHORT nMapIndex );
BOOL IsSkyRenderEnable( USHORT nMapIndex );
BOOL IsSunRenderEnable( USHORT nMapIndex );
BOOL IsFogEnableMap(USHORT nMapIndex);
int GetQuestIndexForWarp(USHORT nMapIndex);
void SetFogLevel(USHORT nMapIndex,BOOL bDay);
void SetOptionFogDistance(USHORT nMapIndex,BOOL bDay, float fFogStart, float fFogEnd);
DWORD GetFogColor(USHORT nMapIndex, BOOL bDay);
D3DXVECTOR3 GetMapDirection(USHORT nMapIndex,BOOL bDay);
void CheckMouseReverse(POINT* pt);

// 2007-04-13 by bhsohn A��� ���빮��
float GetRadianVectorBetweenVector(D3DXVECTOR3 i_vecDir, D3DXVECTOR3 i_vecUp);
D3DXVECTOR3 GetRadianVector(D3DXVECTOR3 i_vecOld, D3DXVECTOR3 i_vecUp, FLOAT i_fRadianMove);

// 2005-07-29 by ispark
float GetCharacterHeight(int nPilotNum);
float GetCharacterSteps(int nPilotNum);
BOOL GetCharactorName(char* o_szCharatorName,INT i_nPilotNum);
void GetBaseChange3D(POINT pt, D3DXVECTOR3 *vOutPos, D3DXVECTOR3 *vOutDir);
// 2005-11-30 by ispark
int GetPositionInvenScreenMode();
BOOL EnableRevivalMap(USHORT nInfluenceType, USHORT nMapIndex);									// 2006-01-02 by ispark
BOOL IsNotInfluenceSameMap(USHORT nInfluenceType, USHORT nMapIndex);							// 2006-01-02 by ispark
#ifdef FREEWAR_
BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType = 1, UINT WarZone = 0);
#else
BOOL IsInfluenceCharacter(USHORT nMyInfluenceType, USHORT nEnemyInfuenceType, BYTE byType = 1);	// 2006-01-07 by ispark
#endif
int GetMapItemVisibleDistance(USHORT nMapIndex);												// 2006-01-13 by ispark
int GetInvenUseEngineEffectNum(int nUnitKind);													// 2006-01-23 by ispark
int GetInvenUseEngineObjectNum(int nUnitKind);													// 2006-01-24 by ispark
int GetMusicVolume(int nRate);																	// 2006-03-22 by ispark
DWORD GetFontColor( char chr );																	// 2006-04-17 by ispark
DWORD GetFontColor(char* str);																	// 2015-03-22 by killburne
BOOL ReSetRemoveString(int nType, char *pChar);													// 2006-04-17 by ispark
float GetCollReflect(D3DXVECTOR3 *pvOut, D3DXVECTOR3 *pvVel, D3DXVECTOR3 *pvNor, BOOL	bRight = TRUE);	// 2006-05-09 by ispark
void GetInfluenceString(char* pOutStr, USHORT nMyInfluenceType);								// 2006-05-24 by ispark

// 2006-08-09 by ispark, ��Ʈ��
void StringCullingUserData(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont = NULL);	// ��ü ��Ʈ�� ���ܳ���
// 2007-11-15 by bhsohn ����Ʈ�����ҋ��� \n�ǰ� ����
//BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);		// 2006-08-07 by ispark
//BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont, BOOL bChRt = FALSE);		// 2006-08-07 by ispark
BOOL StringCullingUserDataEx(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont, BOOL bChRt = FALSE, BOOL bShop = FALSE);		// 2013-04-05 by ssjung �� �϶� ������ �̸� ���� �� �Ѿ� ���� ó�� ����

//BOOL StringCullingUserData2( char* strTalk, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);
//void StringCullingUserData3(char *pLineString, int nState, int nCullStringNum, vector<string> *vecStorage); // Ư������ �տ� �ڸ���

// 2008-01-23 by bhsohn �������� �۾� ¥���� �Լ� ����
BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecDstString, CD3DHanFont* pFont, BOOL bChRt = FALSE); // Ư������ �տ� �ڸ���
char StringCullingAddVector(char chColorSave, char* pString, vector<string> *vecDstString);
// 2008-07-07 by dgwoo �Ƹ��� ���̸� �����Ѵ�.
FLOAT GetAmorGearHeight(INT i_nAmmor, BOOL i_bSelect = FALSE);



// 2007-07-27 by bhsohn �۾� �ø� ���� ����
// 2008-01-23 by bhsohn �������� �۾� ¥���� �Լ� ����
char GetLineFindColor(char* pTxtTmp);				
char GetFindColorOnTxt(char* pTxtTmp);// 2008-07-24 by bhsohn �ø� ���� ���� ����

ATUM_DATE_TIME GetServerDateTime();																// 2006-10-11 by ispark
// 2009. 09. 10 by jsKim �Ʒ��� ���Խ� ���� ī���� ���� ����
ATUM_DATE_TIME GetArenaServerDateTime();														
// end 2009. 09. 10 by jsKim �Ʒ��� ���Խ� ���� ī���� ���� ����
// 2009. 06. 10 by ckPark 255�� �Ѵ� ������ ��� �ð� ����� ������ �ʴ� ���� ����

//ATUM_DATE_TIME GetAtumDateTimeFormSecond(int nSecond);											// 2006-10-11 by ispark
void	GetAtumDateTimeFormSecond( const int nSecond, int* pDay, int* pHour, int* pMin, int* Sec );

// end 2009. 06. 10 by ckPark 255�� �Ѵ� ������ ��� �ð� ����� ������ �ʴ� ���� ����


// 2010. 02. 11 by ckPark �ߵ��� ����������
void	GetString_TimeLeft( DWORD nSec, char* pBuff );			// ���� �ð� nSec�� �ð����ڿ��� pBuff
BOOL	GetString_CoolTime( CItemInfo* pItem, char* pBuff );	// pItem�� ��Ÿ���� �ð����ڿ��� pBuff
// end 2010. 02. 11 by ckPark �ߵ��� ����������


char * GetStringDateTimeFormSecond(char * o_strDateTime, int nSecond);							// 2006-10-11 by ispark
BOOL CheckFontColor( char chr );																// 2006-10-23 by ispark
BOOL CheckFontColor(char* str);																	// 2015-03-22 by killburne
void GetScale(float& fXScale, float& fYScale);													// 2007-07-03 by dgwoo
MapIndex_t GetMapIndexBGM(USHORT nMapIndex);													// 2007-08-02 by dgwoo BGM�� �ε������� ���´�.
DWORD GetCurrentColor(float fTime, DWORD dwType, float fTotal, float fDelay);					// 2007-09-19 by dgwoo�̹����� ���̵� �� �ƿ����� ������ش�.
BOOL IsMousePosClientArea();																	// 2008-01-14 by dgwoo ���콺 ��ġ�� Ŭ���̾�Ʈ ������ �ִ°�?
// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
float FloatRangeDecimal(float fValue, int nDecimalPoint);
INT ChangeChatButtonNumber(INT i_nMenuIndex);

// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���
float FloatSecRangeSharp(float fValue);

// 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�
int GetStringBuffPos(const char* pTxt, int i_nCurSelPos);
int GetStringBuffLen(const char* pTxt);		// �۾� ���̸� �˾ƿ´�.
int NormalizeRect(RECT *prt);
// end 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�

int NumDigits(int x);	// 2015-12-21 Future, int32 optimized method of determinating the number of digits in a number

/*--------------------------------------------------------------------------*/
// 2006-06-15 by ispark
// ���ø�
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::SetIntDataByVector(vector<VECTYPE> &vecInfo, int nIndex)
/// \brief		INT �ڷ��� �Է�
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
template<class VECTYPE> 
void SetIntDataByVector(vector<VECTYPE> &vecInfo, int nIndex)
{
	vector<VECTYPE>::iterator it = vecInfo.begin();

	while(it != vecInfo.end())
	{
		if(*(it) == nIndex)
		{
			return;
		}

		it++;
	}

	vecInfo.push_back(nIndex);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::SetClientIndexDataByVector(ClientIndex_t nClientIndex)
/// \brief		ClientIndex_t �ڷ��� �Է�
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
template<class VECTYPE> 
void SetClientIndexDataByVector(vector<VECTYPE> &vecInfo, ClientIndex_t nClientIndex)
{
	vector<VECTYPE>::iterator it = vecInfo.begin();

	while(it != vecInfo.end())
	{
		if(*(it) == nClientIndex)
		{
			return;
		}

		it++;
	}

	vecInfo.push_back(nClientIndex);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::SendPacketDataByVector(MessageType_t MsgType, VECTYPE &vecInfo)
/// \brief		�޼��� ���ļ� ������ ���� ��û ������
/// \author		ispark
/// \date		2006-06-15 ~ 2006-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
template<class VECTYPE> 
void SendPacketDataByVector(MessageType_t MsgType, vector<VECTYPE> &vecInfo)
{
	char buffer[SIZE_MAX_PACKET];
	int nBytes = 0;
	MessageType_t *pMsgType = NULL;
	VECTYPE *pOtherInfo = NULL;

	memset(buffer, 0x00, SIZE_MAX_PACKET);

	pMsgType	= (MessageType_t*)buffer;
	pOtherInfo	= (VECTYPE*)(buffer + SIZE_FIELD_TYPE_HEADER);

	for(int i=0; i < vecInfo.size(); i++)
	{
		if(nBytes + MSG_SIZE(VECTYPE) > SIZE_MAX_PACKET)
		{
			g_pD3dApp->m_pFieldWinSocket->Write(buffer, nBytes);
			nBytes = 0;
			pMsgType	= (MessageType_t*)buffer;
			pOtherInfo	= (VECTYPE*)(buffer + SIZE_FIELD_TYPE_HEADER);
		}

		*pMsgType				= MsgType;		
		*pOtherInfo				= vecInfo[i];
		nBytes					+= MSG_SIZE(VECTYPE);		
		pMsgType				= (MessageType_t*)(buffer + nBytes);
		pOtherInfo				= (VECTYPE*)(buffer + SIZE_FIELD_TYPE_HEADER + nBytes);
	}

	if(nBytes > 0)
	{
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, nBytes);
//		DBGOUT("SendPacketDataByVector Size (%d)\n", vecInfo.size());
	}
	
	vecInfo.clear();
}
// ���ø�
/*--------------------------------------------------------------------------*/

typedef enum 
{
	APPSTATE_LOGO_LOADING,		// LOGO �ε�
	APPSTATE_LOGO,				// ���� ���� ����� Logo�� �����ִ� ����
	APPSTATE_CONNECTING_SERVER,	// Field server���� ����,�α��� ��� ����
	APPSTATE_GAME_LOADING,		// ���� ���� ����� �ʱ�ȭ �۾�
	APPSTATE_SELECT,			// select menu
	APPSTATE_CREATE,			// create menu
	APPSTATE_MAP_LOADING,		// �� �ε� ����
	APPSTATE_GAME,				// ����
	APPSTATE_GAME_END_LOCK,		// ���� ���� �غ� ����
	APPSTATE_GAME_END,			// ���� ���� �غ� �Ϸ�
	APPSTATE_CLOSE,				// �������� ���� ���� ����
	APPSTATE_ERROR				// ���� ���� ����	
} EnumAppState;
	// APPSTATE_GAME
	typedef enum
	{
		GEARSTATE_FLYING,			// ���� ����
		GEARSTATE_LAND,				// ���� ����
		GEARSTATE_DEAD,				// ���� ����
		GEARSTATE_WARF,				// ���� ����
		GEARSTATE_SHOP				// ����/���� �޴� ����
	} EnumGearState;
		// GEARSTATE_FLYING
		typedef enum
		{
			FLYINGSTATE_NORMAL_FLYING,	// ���� ���� ����
			FLYINGSTATE_ACCELATING,		// �ӵ� ���̴� ����
			FLYINGSTATE_BOOSTER,		// �ν��� ��� ����
			FLYINGSTATE_BREAK,			// �ӵ� ���̴� ����
			FLYINGSTATE_AIR_BREAKING,	// ���� ���� �õ� ����
			FLYINGSTATE_AIR_BREAK		// ���� ���� ����
		} EnumFlyingState;	
		// GEARSTATE_LAND
		typedef enum
		{
			LANDSTATE_LANDING,			// ���� �õ� ����
			LANDSTATE_LANDED,			// ���� ����
			LANDSTATE_TAKEOFF,			// �̷� ����
			LANDSTATE_MOVE,				// �����̴� ����(A���)
			LANDSTATE_ACCELATING,		// �ӵ� ���̴� ����(A���)
			LANDSTATE_BREAK				// �ӵ� ���̴� ����(A���)
		} EnumLandState;
		// GEARSTATE_DEAD
		typedef enum
		{
			GAMESTATE_FALLING,			// ������, ���� ����,�������� ���߿� �����ؼ� �Ѵ�.
			GAMESTATE_FALLEN,			// ���� �Ϸ�
			GAMESTATE_EXPLODING,		// ���� ���� ��
			GAMESTATE_EXPLODED,			// ���� ���� �Ϸ�
		} EnumDeadState;

// 2005-10-10 by ispark
// ������ ���� PK �� ���� Ÿ��
typedef struct PK_TARGET
{
	int			nTargetIndex;
} PK_TARGET;

#endif // !defined(ATUMGLOBAL_H__017A08F0_77F1_4553_AF1A_FDFE85643262__INCLUDED_)
