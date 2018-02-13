// DissolutionItemmanager.h: interface for the DissolutionItemmanager class.
//
//////////////////////////////////////////////////////////////////////

#include "AtumParam.h"
#include "AtumFieldDBManager.h"

class CDissolutionItemManager  
{
public: //���� Ŭ���� ����
	friend CAtumDBHelper;

public:
	CDissolutionItemManager();
	virtual ~CDissolutionItemManager();

	void Initialize ( void );
	void Release ( void );
	// ���� ������ ���� �ε�
	BOOL LoadDBDissolutionItemData ( CODBCStatement *i_odbcstmt );
	
	// ���� ������ üũ �Լ� ���ϰ�: �ش� ���� ���� ������ ����(Create �Լ����� ����ϱ� ����)
	tDissolutionItemInfo CheckDissolutionItem(INT i_itemnum);

	// ������ ���� �Լ� ���ϰ� : ���� ������ ���
	BOOL CreateDissolutionItem(INT i_itemnum, tDissolutionItemInfo i_DissolutionData, DissolutionItems* o_Dissolved_ItemData );

private:

	mtmapDissolutionItemInfo m_mapDissolutionItemData; // ���� �������� ������ �����̳�

};

