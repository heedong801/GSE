#pragma once

enum STAGEID {
	STAGE_ONE, STAGE_TWO
};

class Stage
{
public:
	Stage();
	~Stage();

	int m_Wave = 3;
	STAGEID m_StageID = STAGE_ONE;
	int m_MonsterCount = 0;
	bool m_SpawnCheck = false;
};

