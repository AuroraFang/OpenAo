#ifndef __NPC_SCRIPTS_H__
#define __NPC_SCRIPTS_H__

#include "AtumParam.h"
#include "mt_stl.h"

struct NPCTalk
{
	int					TalkID;
	char*				TalkString;		// allocated by strdup(), freed by free()
	vector<char*>		MenuVector;		// allocated by strdup(), freed by free()

	~NPCTalk();
	void reset();
};

struct NPCScript
{
	NPCID_t				NpcID;
	vector<NPCTalk*>	NpcTalkVector;	// NpcScripts ���Ͽ��� talk, menu ��ȣ��
										// �� ������� �ԷµǾ�� ��!
	~NPCScript();
	void reset();
};

void CleanNpcScripts(ez_map<NPCID_t, NPCScript> &mapNpcScripts);

#endif // __NPC_SCRIPTS_H__
