#pragma once
#include "Global.h"
#include "Dependencies\glew.h"
#include "Sound.h"
#include "Stage.h"

//ScnMgr

class Renderer;
class Object;
class Stage;

enum TEXID {
	PLAYER, TEAR, FLY, SUNKEN, SUNKENBULLET, SUNKEN2, SPIDER, BIRTH, MAP, FLY2 = 10, BOSS, BOSSTEAR
};

class SceneManager
{
private:
	Stage* m_Stage;
	Renderer* m_Renderer = nullptr;
	Object* m_Objects[MAX_OBJECTS];
	Sound* m_Sound;

	GLuint m_Tex[15];

	int m_SoundBG = 0;
	int m_SoundFiring = 0;
	int m_BossSound = 0;
	int m_MobDeathSound = 0;
	int m_SoundBulletCollision = 0;

	float m_eTime;
	float g_Time = 0.f;

public:
	SceneManager();
	~SceneManager();

	void RenderScene();
	void Update(float elapsedTime);

	void KeyInput(unsigned char key, int x, int y);
	void keyboard_up(unsigned char key, int x, int y);
	void SpecialKeyInput(int key, int x, int y);

	void AddForce(float fX, float fY, float fZ, float eTime);
	
	void AddObject(float x, float y, float z, float shadowHeight, float sx, float sy, float sz, float vx, float vy, float vz, int kind, int state, int Hp, int maxHp, int damage, GLuint texID);
	void DeleteObject(unsigned int idx);
	int FindEmptySlot();

	void FlySpawn(float x, float y, float z, int kind, GLuint texID);
	void SunkenSpawn(float x, float y, float z, GLuint texID);
	void Sunken2Spawn(float x, float y, float z, GLuint texID);
	void SpiderSpawn(float x, float y, float z, GLuint texID);
	void BossSpawn(float x, float y, float z, GLuint texID);

	void Shoot(int dir);
	void SunkenAttack(int mobIndex, GLuint kind);
	void BossAttack(int mobIndex);

	void UpdateCollision();
	void ProcessCollision(int i, int j);

	bool RRCollision(float minX1, float minY1,
		float maxX1, float maxY1,
		float minX2, float minY2, 
		float maxX2, float maxY2);

	bool BBCollision(float minX1, float minY1, float minZ1, 
		float maxX1, float maxY1, float maxZ1,
		float minX2, float minY2, float minZ2, 
		float maxX2, float maxY2, float maxZ2);

	void DoGarbageCollect();

	bool m_GameEnd = false;
};

// 등속도 운동 = 속도가 변하지 않는다.
/*
갱신될 위치 = 이전 위치 + 속도*시간
갱신될 속도 = 이전 속도 + 가속도*시간
가속도 = 힘 / 질량
힘 = WSAD 기반 힘의 방향 설정
시간 : 초, 거리 : 미터, 힘 : N, 질량 : kg
속도 : m/s, 가속도 : m/(s^2)
중력 가속도 : 9.8m/(s^2)
*/

// 가속도 결정
/*
1.정지된 10kg의 물체에 1초동안 10N의 힘이 지속적으로 가해진다면?
가속도 = 힘 / 질량 = 1m/(s^2)
1초 후 이 물체의 속도 = 1m/s

2.정지된 5kg의 물체에 2초동안 10N의 힘이 지속적으로 가해진다면?
가속도 = 힘 / 질량 = 2m/(s^2)
2초 후 이 물체의 속도 = 4m/s
*/

/*
ApplyForce(float fx, float fy)
유저 입력 마다 불리는 함수
힘을 N 단위로 넘김
넘겨받은 힘과 시간을 기준으로 Object에 가속도 적용
*/