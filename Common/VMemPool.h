#pragma once

namespace VMemPool
{
	BOOL vmPoolAddObject(DWORD dwObjTypeSize, DWORD dwObjPoolSize = 1024);
	BOOL vmPoolInit();
	void vmPoolClean();
	void vmPoolSaveCurrentStatus();		// 2009-05-04 by cmkwon, �޸�Ǯ �������� �α� ����� - 

	void* vmObjectNew(DWORD dwObjSize);
	void vmObjectDelete(void *p, DWORD dwObjSize);
};
