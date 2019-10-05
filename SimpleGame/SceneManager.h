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

// ��ӵ� � = �ӵ��� ������ �ʴ´�.
/*
���ŵ� ��ġ = ���� ��ġ + �ӵ�*�ð�
���ŵ� �ӵ� = ���� �ӵ� + ���ӵ�*�ð�
���ӵ� = �� / ����
�� = WSAD ��� ���� ���� ����
�ð� : ��, �Ÿ� : ����, �� : N, ���� : kg
�ӵ� : m/s, ���ӵ� : m/(s^2)
�߷� ���ӵ� : 9.8m/(s^2)
*/

// ���ӵ� ����
/*
1.������ 10kg�� ��ü�� 1�ʵ��� 10N�� ���� ���������� �������ٸ�?
���ӵ� = �� / ���� = 1m/(s^2)
1�� �� �� ��ü�� �ӵ� = 1m/s

2.������ 5kg�� ��ü�� 2�ʵ��� 10N�� ���� ���������� �������ٸ�?
���ӵ� = �� / ���� = 2m/(s^2)
2�� �� �� ��ü�� �ӵ� = 4m/s
*/

/*
ApplyForce(float fx, float fy)
���� �Է� ���� �Ҹ��� �Լ�
���� N ������ �ѱ�
�Ѱܹ��� ���� �ð��� �������� Object�� ���ӵ� ����
*/