// InfluenceRateGridCtrl.cpp: implementation of the CInfluenceRateGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "atumadmintool.h"
#include "InfluenceRateGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfluenceRateGridCtrl::CInfluenceRateGridCtrl()
{

}

CInfluenceRateGridCtrl::~CInfluenceRateGridCtrl()
{

}

void  CInfluenceRateGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - ���ڸ� �Էµǵ��� ����
	CString csCurStr = GetItemText(nRow, nCol);
	if (csCurStr != str)
	{
		int nChangedValues	= atoi(str);
		nChangedValues		= min(100, nChangedValues);
		nChangedValues		= max(0, nChangedValues);		
		str.Format("%d", nChangedValues);
	}
	CGridCtrl::OnEndEditCell(nRow, nCol, str);
}
