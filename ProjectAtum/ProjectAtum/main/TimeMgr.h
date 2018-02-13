// 2011-08-12 by hsson ��� �뿭�� �߰�/Ż�� �� �����̸� ���� (��� ����ȭ ���׸� ����)

#pragma once
//
// Ư���� �ð� ���� 1���� �������� �����ϰ� ���� �� ���, �ִ� eMAX_TIMER������ ��� �����մϴ�.
// �� ����ϰ� ���� ���� eMAX_TIMER ���� �����ϸ� �˴ϴ�.
// 
// ex code)
// ����ϴ� cpp or h ���� #include "TimeMgr.h" �� �� ���� �ڵ带 ����
// if( g_cTimeMgr.CheckTime( 1, 1000 ) )
// {
// 	_asm nop
// }
// 
// 1�� Ÿ�̸Ӹ� ����ϰ� 1�ʿ� 1���� if���� ��� �� _asm nop�� ���� �մϴ�.
// ��, Ÿ�̸� ��ȣ�� �ߺ������ Ư���� �뵵 �̿ܿ��� ������� �ʴ°��� �����մϴ�.
//

#define g_cTimeMgr CTimeMgr::GetInstance()

class CTimeMgr
{
private:
	enum { eMAX_TIMER = 30, };
	DWORD m_bCur[eMAX_TIMER];

public:
	CTimeMgr()
	{
		for( int i = 0; i < eMAX_TIMER; ++i )
			m_bCur[i] = timeGetTime();
	}
	
	static CTimeMgr& GetInstance()
	{
		static CTimeMgr Instance;
		return Instance;
	}

	bool CheckTime( const int& _nIndex, const int& _nDelay )
	{
		if( eMAX_TIMER <= _nIndex || 0 > _nIndex )
			return false;

		if( m_bCur[_nIndex] + _nDelay < timeGetTime() )
		{
			m_bCur[_nIndex] = timeGetTime();
			return true;
		}

		return false;
	}
};


// end 2011-08-12 by hsson ��� �뿭�� �߰�/Ż�� �� �����̸� ���� (��� ����ȭ ���׸� ����)