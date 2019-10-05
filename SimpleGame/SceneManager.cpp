#pragma once
#include "stdafx.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "Object.h"
#include <random>
#include <time.h>
//////////////////////////////////////////////////////////////////////////////////
Vector3D Normalize(Vector3D tmp)
{
	float velocity = sqrtf(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);

	Vector3D result;
	result.x = tmp.x / velocity; result.y = tmp.y / velocity; result.z = tmp.z / velocity;
	return result;
}

Vector3D Minus(Vector3D a, Vector3D b)
{
	Vector3D result;

	result.x = a.x - b.x; result.y = a.y - b.y; result.z = a.z - b.z;
	return result;
}

Vector3D Plus(Vector3D a, Vector3D b)
{
	Vector3D result;

	result.x = a.x + b.x; result.y = a.y + b.y; result.z = a.z + b.z;
	return result;
}

Vector3D Scale(Vector3D a, float b)
{
	Vector3D result;

	result.x = a.x * b; result.y = a.y * b; result.z = a.z * b;
	return result;
}
//////////////////////////////////////////////////////////////////////////////////

SceneManager::SceneManager()
{
	//Init Object array
	for (int i = 0; i < MAX_OBJECTS; i++)
		m_Objects[i] = NULL;
	m_Renderer = new Renderer(500, 500);
	//m_Tex[11] = m_Renderer->CreatePngTexture("./Textures/loading.png");
	//m_Renderer->DrawTextureRectDepth(0, 0, 0, 500, 500, 1, 1, 1, 1, m_Tex[11], 1);
	
	//Load png
	m_Tex[0] = m_Renderer->CreatePngTexture("./Textures/PlayerMove.png");
	m_Tex[1] = m_Renderer->CreatePngTexture("./Textures/tear.png");
	m_Tex[2] = m_Renderer->CreatePngTexture("./Textures/fly.png");
	m_Tex[3] = m_Renderer->CreatePngTexture("./Textures/sunken.png");
	m_Tex[4] = m_Renderer->CreatePngTexture("./Textures/sunkenTear.png");
	m_Tex[5] = m_Renderer->CreatePngTexture("./Textures/sunken2.png");
	m_Tex[6] = m_Renderer->CreatePngTexture("./Textures/spider.png");
	m_Tex[7] = m_Renderer->CreatePngTexture("./Textures/birth.png");
	m_Tex[8] = m_Renderer->CreatePngTexture("./Textures/map.png");
	m_Tex[9] = m_Renderer->CreatePngTexture("./Textures/rect.png");
	m_Tex[10] = m_Renderer->CreatePngTexture("./Textures/fly2.png");
	m_Tex[11] = m_Renderer->CreatePngTexture("./Textures/boss.png");
	m_Tex[12] = m_Renderer->CreatePngTexture("./Textures/bosstear.png");
	m_Tex[13] = m_Renderer->CreatePngTexture("./Textures/ending.png");

	// Initializationm_Renderer->CreatePngTexture("./Textures/fly.png");
	m_Objects[HERO_ID] = new PlayerObject();
	m_Objects[HERO_ID]->SetPosition(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetShadowHeight(90.f);
	m_Objects[HERO_ID]->SetVel(0.0f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetAcc(0.f, 0.f, 0.f);
	m_Objects[HERO_ID]->SetSize(0.3f, 0.3f, 0.3f);
	m_Objects[HERO_ID]->SetColor(1.f, 1.f, 1.f, 1.f);
	m_Objects[HERO_ID]->SetFriction(0.5f);
	m_Objects[HERO_ID]->SetMas(1.f);
	m_Objects[HERO_ID]->SetKind(KIND_HERO);
	m_Objects[HERO_ID]->SetHP(7500);
	m_Objects[HERO_ID]->SetState(STATE_GROUND);
	m_Objects[HERO_ID]->SetMaxHp(7500);
	m_Objects[HERO_ID]->SetTexture(m_Tex[0]);
	m_Objects[HERO_ID]->SetDirection(Direction::DOWN);
	
	AddObject(-2.5, 0, 0, 0, 0.4, 4, 1, 0, 0, 0, KIND_BUILDING, STATE_GROUND, 10000, 10000, 0, m_Tex[9]);
	AddObject(2.5, 0, 0, 0, 0.4, 4, 1, 0, 0, 0, KIND_BUILDING, STATE_GROUND, 10000, 10000, 0, m_Tex[9]);
	AddObject(0, 2.5, 0, 0, 4.5, 0.8, 1, 0, 0, 0, KIND_BUILDING, STATE_GROUND, 10000, 10000, 0, m_Tex[9]);
	AddObject(0, -2.5, 0, 0, 4.5, 0.8, 1, 0, 0, 0, KIND_BUILDING, STATE_GROUND, 10000, 10000, 0, m_Tex[9]);

	
	m_Stage = new Stage();
	

	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}
	m_Sound = new Sound();
	m_SoundBG = m_Sound->CreateSound("./Sounds/bgm.wav");
	m_Sound->PlaySounds(m_SoundBG, true, 3);
	m_BossSound = m_Sound->CreateSound("./Sounds/satan.mp3");
	m_MobDeathSound = m_Sound->CreateSound("./Sounds/death.mp3");
	m_SoundFiring = m_Sound->CreateSound("./Sounds/gun-shot.wav");
	
}


SceneManager::~SceneManager()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		DeleteObject(i);
	}
	for( int i = 0 ; i < 15; i++)
		m_Renderer->DeleteTexture(m_Tex[i]);
	m_Sound->DeleteSound(m_SoundBG);
	m_Sound->DeleteSound(m_SoundFiring);

	delete m_Renderer;
	delete m_Stage;
	delete m_Sound;
}

void SceneManager::RenderScene()
{
	if (!m_GameEnd)
	{
		float x, y, z, posHeight;
		float sizeX, sizeY, sizeZ;
		float r, g, b, a;
		float vx, vy, vz;
		float ax, ay, az;
		int objectKind;
		int objectMaxHp;
		int objectHp;
		GLuint texID;

		m_Renderer->DrawTextureRectDepth(0, 0, 0, 500, 500, 1, 1, 1, 1, m_Tex[8], 1);

		//draw Objects
		//////////////////////////////////////////////////////////////
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			if (m_Objects[i] != nullptr)
			{
				m_Objects[i]->GetPosition(&x, &y, &z);
				m_Objects[i]->GetShadowHeight(&posHeight);
				m_Objects[i]->GetSize(&sizeX, &sizeY, &sizeZ);
				m_Objects[i]->GetColor(&r, &g, &b, &a);
				m_Objects[i]->GetVel(&vx, &vy, &vz);
				m_Objects[i]->GetAcc(&ax, &ay, &az);
				m_Objects[i]->GetKind(&objectKind);
				m_Objects[i]->GetMaxHP(&objectMaxHp);
				m_Objects[i]->GetHP(&objectHp);
				m_Objects[i]->GetTexture(&texID);

				if (m_Objects[i]->GetTargetOn())
				{
					if (m_Objects[KIND_HERO] != NULL)
						m_Objects[i]->SetTarget(m_Objects[KIND_HERO]->GetPosition());
				}

				float newX, newY, newZ, newW, newH;
				newX = x * 100.f;
				newY = y * 100.f;
				newZ = z * 100.f;
				newW = sizeX * 100.f;
				newH = sizeY * 100.f;
				//m_Renderer->DrawTextureRectHeight(newX, newY, 0, newW, -newH, r, g, b, a, m_Player, newZ + posHeight);
				//m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW - 15, newH - 15, r, g, b, a, m_BodySeq, seqX, 1, 10, 4, newZ);
				//m_Renderer->DrawTextureRectSeqXY(newX, newY + 20, 0, newW, newH, r, g, b, a, m_HeadSeq, 0, 1, 2, 4);
				if (objectKind == KIND_HERO)
				{
					Direction texDir;
					texDir = m_Objects[i]->GetDirection();

					//play animation
					/////////////////////////////////////////////////////////////
					static int seqPlayercount = 0;
					static int g_PlayerSeq = 0;
					int seqX = g_PlayerSeq % 10;
					m_Objects[KIND_HERO]->GetVel(&vx, &vy, &vz);
					if (vx == 0 && vy == 0 && vz == 0)
						seqX = 1;
					else
					{
						seqPlayercount++;
						if (seqPlayercount % 10 == 0)
							g_PlayerSeq++;
						if (seqPlayercount > 2000)
							seqPlayercount = 0;
					}
					//////////////////////////////////////////////////////////////

					m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, seqX, texDir, 3, 4, newZ);
					m_Renderer->DrawTextureRectDepth(newX, newY, 0, newW, newH, 1, 0, 1, 1, m_Tex[9], 1);
				}
				else if (objectKind == KIND_ENEMY)
				{
					int seqX;
					m_Objects[i]->GetSpriteSeq(&seqX);
					if (m_Objects[i]->m_BirthCount != -1)
						m_Objects[i]->m_BirthCount++;
					if (m_Objects[i]->m_BirthCount % 5 == 0)
					{
						m_Objects[i]->m_BirthSeq++;
						if (m_Objects[i]->m_BirthSeq == 10)
						{
							m_Objects[i]->m_BirthSeq = -1;
							m_Objects[i]->m_BirthCount = -1;
						}
					}
					if (texID == m_Tex[TEXID::FLY] || texID == m_Tex[TEXID::FLY2])
					{
						if (m_Objects[i]->m_BirthSeq != -1)
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, m_Tex[7], m_Objects[i]->m_BirthSeq, 0, 9, 1, posHeight);
						else
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, seqX, 0, 2, 1, posHeight);
					}
					else if (texID == m_Tex[TEXID::SUNKEN])
					{
						m_Objects[i]->GetSpriteSeq(&seqX);
						if (m_Objects[i]->m_BirthCount != -1)
							m_Objects[i]->m_BirthCount++;
						if (m_Objects[i]->m_BirthCount % 5 == 0)
						{
							m_Objects[i]->m_BirthSeq++;
							if (m_Objects[i]->m_BirthSeq == 10)
							{
								m_Objects[i]->m_BirthSeq = -1;
								m_Objects[i]->m_BirthCount = -1;
							}
						}
						if (m_Objects[i]->m_BirthSeq != -1)
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, m_Tex[7], m_Objects[i]->m_BirthSeq, 0, 9, 1, posHeight);
						else
						{
							SunkenObject* tmp = (SunkenObject*)m_Objects[i];
							bool attackOn = tmp->GetAttack();

							if (attackOn)
								SunkenAttack(i, TEXID::SUNKEN);

							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, seqX, 0, 3, 1, posHeight);
						}
					}
					else if (texID == m_Tex[TEXID::SUNKEN2])
					{
						m_Objects[i]->GetSpriteSeq(&seqX);
						if (m_Objects[i]->m_BirthCount != -1)
							m_Objects[i]->m_BirthCount++;
						if (m_Objects[i]->m_BirthCount % 5 == 0)
						{
							m_Objects[i]->m_BirthSeq++;
							if (m_Objects[i]->m_BirthSeq == 10)
							{
								m_Objects[i]->m_BirthSeq = -1;
								m_Objects[i]->m_BirthCount = -1;
							}
						}
						if (m_Objects[i]->m_BirthSeq != -1)
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, m_Tex[7], m_Objects[i]->m_BirthSeq, 0, 9, 1, posHeight);
						else
						{
							Sunken2Object* tmp = (Sunken2Object*)m_Objects[i];
							bool attackOn = tmp->GetAttack();

							if (attackOn)
								SunkenAttack(i, TEXID::SUNKEN2);

							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, seqX, 0, 3, 1, posHeight);
						}
					}
					else if (texID == m_Tex[TEXID::SPIDER])
					{
						m_Objects[i]->GetSpriteSeq(&seqX);
						if (m_Objects[i]->m_BirthCount != -1)
							m_Objects[i]->m_BirthCount++;
						if (m_Objects[i]->m_BirthCount % 5 == 0)
						{
							m_Objects[i]->m_BirthSeq++;
							if (m_Objects[i]->m_BirthSeq == 10)
							{
								m_Objects[i]->m_BirthSeq = -1;
								m_Objects[i]->m_BirthCount = -1;
							}
						}
						if (m_Objects[i]->m_BirthSeq != -1)
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, m_Tex[7], m_Objects[i]->m_BirthSeq, 0, 9, 1, posHeight);
						else
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, seqX, 0, 8, 1, posHeight);
					}
					else if (texID == m_Tex[TEXID::BOSS])
					{
						m_Objects[i]->GetSpriteSeq(&seqX);
						if (m_Objects[i]->m_BirthCount != -1)
							m_Objects[i]->m_BirthCount++;
						if (m_Objects[i]->m_BirthCount % 5 == 0)
						{
							m_Objects[i]->m_BirthSeq++;
							if (m_Objects[i]->m_BirthSeq == 10)
							{
								m_Objects[i]->m_BirthSeq = -1;
								m_Objects[i]->m_BirthCount = -1;
							}
						}
						if (m_Objects[i]->m_BirthSeq != -1)
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, m_Tex[7], m_Objects[i]->m_BirthSeq, 0, 9, 1, posHeight);
						else
						{
							BossObject* tmp = (BossObject*)m_Objects[i];
							bool attackOn = tmp->GetAttack();

							if (attackOn)
							{
								BossAttack(i);
								m_Sound->PlaySounds(m_BossSound, false, 0.09f);

							}
							m_Renderer->DrawTextureRectSeqXYHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, seqX, 0, 6, 1, posHeight);
						}
					}
				}
				else
				{
					if (texID != m_Tex[9])
						m_Renderer->DrawTextureRectHeight(newX, newY, 0, newW, newH, r, g, b, a, texID, newZ);
				}

				if (objectKind == KIND_HERO)
					m_Renderer->DrawSolidRectGauge(newX, newY + newH, 0, newW, 3, 1, 0, 0, 1, newZ, objectHp / (float)objectMaxHp);
			}
		}
		//////////////////////////////////////////////////////////////



		//m_Renderer->DrawParticleClimate(0, 0, 0,
		//	1,
		//	1, 1, 1, 1,
		//	-1.f, -1.f,	//방향
		//	m_Player,	
		//	1.f,		//뿌리는 놈의 퍼센트
		//	g_Time);
		//g_Time += 0.01f;
	}
	else
	{
		m_Renderer->DrawTextureRect(0, 0, 0, 500, -500, 1, 1, 1, 1, m_Tex[13]);
	}
}

void SceneManager::Update(float eTime)
{
	if (!m_GameEnd)
	{
		m_eTime = eTime;

		srand(time(NULL));

		if (m_Stage->m_StageID == STAGE_ONE)
		{
			if (!m_Stage->m_SpawnCheck)
			{
				if (m_Stage->m_Wave == 1)
				{
					for (int i = 0; i < 8; i++)
					{
						float tmpX, tmpY;
						tmpX = (rand() % 40) * 0.1 - 2;
						tmpY = (rand() % 40) * 0.1 - 2;

						if (i < 6)
							FlySpawn(tmpX, tmpY, 0, 0, m_Tex[2]);
						else
							SpiderSpawn(tmpX, tmpY, 0, m_Tex[6]);
					}

					m_Stage->m_MonsterCount = 8;
					SunkenSpawn(0, 0, 0, m_Tex[3]);


					m_Stage->m_SpawnCheck = true;
				}
				else if (m_Stage->m_Wave == 2)
				{
					SunkenSpawn(1, 1, 0, m_Tex[3]);
					SunkenSpawn(1, -1, 0, m_Tex[3]);
					SunkenSpawn(-1, 1, 0, m_Tex[3]);
					SunkenSpawn(-1, -1, 0, m_Tex[3]);

					for (int i = 0; i < 5; i++)
					{
						float tmpX, tmpY;
						tmpX = (rand() % 40) * 0.1 - 2;
						tmpY = (rand() % 40) * 0.1 - 2;

						FlySpawn(tmpX, tmpY, 0, 1, m_Tex[10]);
					}

					m_Stage->m_MonsterCount = 9;
					m_Stage->m_SpawnCheck = true;
				}
				else if (m_Stage->m_Wave == 3)
				{
					Sunken2Spawn(1, 0, 0, m_Tex[5]);
					Sunken2Spawn(-1, 0, 0, m_Tex[5]);

					for (int i = 0; i < 5; i++)
					{
						float tmpX, tmpY;
						tmpX = (rand() % 40) * 0.1 - 2;
						tmpY = (rand() % 40) * 0.1 - 2;

						SpiderSpawn(tmpX, tmpY, 0, m_Tex[6]);
					}
					m_Stage->m_MonsterCount = 7;
					m_Stage->m_SpawnCheck = true;
				}
				else if (m_Stage->m_Wave == 4)
				{
					BossSpawn(0, 0, 0, m_Tex[11]);
					m_Stage->m_MonsterCount = 1;
					m_Stage->m_SpawnCheck = true;
				}
				else
				{
					m_GameEnd = true;
				}

			}
			if (m_Stage->m_MonsterCount == 0)
			{
				m_Stage->m_Wave++;
				m_Stage->m_SpawnCheck = false;
			}
		}
		m_Stage->m_MonsterCount = 0;
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			int kind;

			if (m_Objects[i] != nullptr)
			{
				m_Objects[i]->GetKind(&kind);
				m_Objects[i]->Update(eTime);
				if (kind == KIND_ENEMY)
				{
					m_Stage->m_MonsterCount++;

				}
			}
		}
	}
}

void SceneManager::KeyInput(unsigned char key, int x, int y)
{
	if (!m_GameEnd)
	{
		if (m_Objects[HERO_ID] != NULL)
			m_Objects[HERO_ID]->KeyInput(key, x, y);
	}
}

void SceneManager::keyboard_up(unsigned char key, int x, int y)
{
	if (!m_GameEnd)
	{
		if (m_Objects[HERO_ID] != NULL)
			m_Objects[HERO_ID]->keyboard_up(key, x, y);
	}
}

void SceneManager::SpecialKeyInput(int key, int x, int y)
{
	if (!m_GameEnd)
	{
		if (m_Objects[HERO_ID] != NULL)
			m_Objects[HERO_ID]->SpecialKeyInput(key, x, y);
	}
}

void SceneManager::AddForce(float fX, float fY, float fZ, float eTime)
{
	if (!m_GameEnd)
	{
		if (m_Objects[HERO_ID] != NULL)
		{
			int state;
			m_Objects[HERO_ID]->GetState(&state);
			if (state == STATE_AIR)
			{
				fZ = 0;
			}
			m_Objects[HERO_ID]->AddForce(fX, fY, fZ, eTime);
		}
	}
}


void SceneManager::AddObject(float x, float y, float z, float shadowHeight, float sx, float sy, float sz, float vx, float vy, float vz, int kind, int state, int Hp, int maxHp, int damage, GLuint texID)
{
	if (!m_GameEnd)
	{
		//find empty slot
		int index = FindEmptySlot();

		//allocate
		if (index < 0)
			return;
		if (texID == m_Tex[TEXID::FLY] || texID == m_Tex[TEXID::FLY2])
			m_Objects[index] = new FlyObject();
		else if (texID == m_Tex[TEXID::SUNKEN])
			m_Objects[index] = new SunkenObject();
		else if (texID == m_Tex[TEXID::SUNKEN2])
			m_Objects[index] = new Sunken2Object();
		else if (texID == m_Tex[TEXID::SPIDER])
			m_Objects[index] = new SpiderObject();
		else if (texID == m_Tex[TEXID::BOSS])
			m_Objects[index] = new BossObject();
		else
			m_Objects[index] = new Object();

		m_Objects[index]->SetPosition(x, y, z);
		m_Objects[index]->SetShadowHeight(shadowHeight);
		m_Objects[index]->SetSize(sx, sy, sz);
		m_Objects[index]->SetColor(1.f, 1.f, 1.f, 1.f);
		m_Objects[index]->SetAcc(0.f, 0.f, 0.f);
		m_Objects[index]->SetVel(vx, vy, vz);
		m_Objects[index]->SetMas(1.f);
		m_Objects[index]->SetFriction(5.0f);
		m_Objects[index]->SetHP(Hp);
		m_Objects[index]->SetMaxHp(maxHp);
		m_Objects[index]->SetKind(kind);
		m_Objects[index]->SetState(state);
		m_Objects[index]->SetDamage(damage);
		m_Objects[index]->SetTexture(texID);
	}
}

void SceneManager::DeleteObject(unsigned int idx)
{
	if (m_Objects[idx] != NULL)
	{
		delete m_Objects[idx];
		m_Objects[idx] = NULL;
	}
}

void SceneManager::Shoot(int dir)
{
	if (!m_GameEnd)
	{
		if (m_Objects[HERO_ID] != NULL)
		{
			static int count = 0;

			if (dir == SHOOT_NONE)
				return;

			if (count % 15 == 0)
			{
				float bulletSpeedX = 0, bulletSpeedY = 0.f;
				float amount = 5.5f;

				switch (dir)
				{
				case SHOOT_UP:
					bulletSpeedX = 0.f;
					bulletSpeedY = amount;
					break;
				case SHOOT_DOWN:
					bulletSpeedX = 0.f;
					bulletSpeedY = -amount;
					break;
				case SHOOT_LEFT:
					bulletSpeedX = -amount;
					bulletSpeedY = 0.f;
					break;
				case SHOOT_RIGHT:
					bulletSpeedX = amount;
					bulletSpeedY = 0.f;
					break;
				}

				float pX, pY, pZ, pSH, vX, vY, vZ;
				m_Objects[HERO_ID]->GetPosition(&pX, &pY, &pZ);
				m_Objects[HERO_ID]->GetShadowHeight(&pSH);
				m_Objects[HERO_ID]->GetVel(&vX, &vY, &vZ);

				bulletSpeedX += vX;
				bulletSpeedY += vY;

				AddObject(pX, pY, pZ + 0.2f, pSH, 0.1f, 0.1f, 0.1f, bulletSpeedX, bulletSpeedY, 0, KIND_PLAYERBULLET, STATE_AIR, 500, 500, 500, m_Tex[1]);
				m_Sound->PlaySounds(m_SoundFiring, false, 0.09f);
			}
			count++;
			if (count > 10000)
				count = 0;
		}
	}
}

void SceneManager::SunkenAttack(int mobIndex, GLuint kind)
{
	if (!m_GameEnd)
	{
		if (kind == TEXID::SUNKEN)
		{
			static bool attackX = false;

			float bulletSpeedX = 5, bulletSpeedY = 5.f;

			float pX, pY, pZ, pSH;
			m_Objects[mobIndex]->GetPosition(&pX, &pY, &pZ);
			m_Objects[mobIndex]->GetShadowHeight(&pSH);

			if (!attackX)
			{
				for (int i = 0; i < 4; i++)
				{
					if (i == 0)
					{
						bulletSpeedX = 0;
						bulletSpeedY = 5;
					}
					else if (i == 1)
					{
						bulletSpeedX = 5;
						bulletSpeedY = 0;
					}
					else if (i == 2)
					{
						bulletSpeedX = 0;
						bulletSpeedY = -5;
					}
					else if (i == 3)
					{
						bulletSpeedX = -5;
						bulletSpeedY = 0;
					}
					AddObject(pX, pY, pZ + 0.2f, pSH, 0.1f, 0.1f, 0.1f, bulletSpeedX, bulletSpeedY, 0, KIND_SUNKENBULLET, STATE_AIR, 500, 500, 500, m_Tex[4]);
				}
				attackX = true;
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (i == 0)
					{
						bulletSpeedX = 5;
						bulletSpeedY = 5;
					}
					else if (i == 1)
					{
						bulletSpeedX = 5;
						bulletSpeedY = -5;
					}
					else if (i == 2)
					{
						bulletSpeedX = -5;
						bulletSpeedY = -5;
					}
					else if (i == 3)
					{
						bulletSpeedX = -5;
						bulletSpeedY = 5;
					}
					AddObject(pX, pY, pZ + 0.2f, pSH, 0.1f, 0.1f, 0.1f, bulletSpeedX, bulletSpeedY, 0, KIND_SUNKENBULLET, STATE_AIR, 500, 500, 500, m_Tex[4]);
				}
				attackX = false;
			}
		}
		else
		{
			float bulletSpeedX = 5, bulletSpeedY = 5.f;

			float pX, pY, pZ, pSH;
			m_Objects[mobIndex]->GetPosition(&pX, &pY, &pZ);
			m_Objects[mobIndex]->GetShadowHeight(&pSH);


			for (int i = 0; i < 8; i++)
			{
				if (i == 0)
				{
					bulletSpeedX = 0;
					bulletSpeedY = 5;
				}
				else if (i == 1)
				{
					bulletSpeedX = 5;
					bulletSpeedY = 0;
				}
				else if (i == 2)
				{
					bulletSpeedX = 0;
					bulletSpeedY = -5;
				}
				else if (i == 3)
				{
					bulletSpeedX = -5;
					bulletSpeedY = 0;
				}
				else if (i == 4)
				{
					bulletSpeedX = 5;
					bulletSpeedY = 5;
				}
				else if (i == 5)
				{
					bulletSpeedX = 5;
					bulletSpeedY = -5;
				}
				else if (i == 6)
				{
					bulletSpeedX = -5;
					bulletSpeedY = -5;
				}
				else if (i == 7)
				{
					bulletSpeedX = -5;
					bulletSpeedY = 5;
				}
				AddObject(pX, pY, pZ + 0.2f, pSH, 0.1f, 0.1f, 0.1f, bulletSpeedX, bulletSpeedY, 0, KIND_SUNKENBULLET, STATE_AIR, 500, 500, 500, m_Tex[4]);

			}
		}
	}

}


void SceneManager::BossAttack(int mobIndex)
{
	if (!m_GameEnd)
	{
		static int count = 0;
		Vector3D playerPos;
		Vector3D bossPos;
		Vector3D bossBulletSpeed;

		if (m_Objects[HERO_ID] != NULL)
			playerPos = m_Objects[HERO_ID]->GetPosition();

		BossObject* tmp = (BossObject*)m_Objects[mobIndex];
		if (!tmp->GetSpawn()) {
			count = 0;
			bossPos = m_Objects[mobIndex]->GetPosition();
			bossBulletSpeed = Scale(Normalize(Minus(playerPos, bossPos)), 9);

			AddObject(bossPos.x, bossPos.y, bossPos.z + 0.2f, 0, 0.1f, 0.1f, 0.1f, bossBulletSpeed.x, bossBulletSpeed.y, 0, KIND_SUNKENBULLET, STATE_AIR, 1500, 1500, 1500, m_Tex[12]);
		}
		else {
			if (count == 0)
			{
				count = 1;
				float randPattern;
				randPattern = rand() % 3;

				if (randPattern == 0)
				{
					Sunken2Spawn(1.5, 0, 0, m_Tex[5]);
					Sunken2Spawn(-1.5, 0, 0, m_Tex[5]);
				}
				else if (randPattern == 1)
				{
					for (int i = 0; i < 2; i++)
					{
						float tmpX, tmpY;
						tmpX = (rand() % 40) * 0.1 - 2;
						tmpY = (rand() % 40) * 0.1 - 2;

						SpiderSpawn(tmpX, tmpY, 0, m_Tex[6]);
					}
				}
				else if (randPattern == 2)
				{
					for (int i = 0; i < 2; i++)
					{
						float tmpX, tmpY;
						tmpX = (rand() % 40) * 0.1 - 2;
						tmpY = (rand() % 40) * 0.1 - 2;

						FlySpawn(tmpX, tmpY, 0, 1, m_Tex[12]);
					}
				}
			}
		}

	}
	
}
void SceneManager::FlySpawn(float x,float y,float z, int kind, GLuint texID)
{
	if ( kind == 0 )
		AddObject(x, y, z, 50, 0.2f, 0.2f, 0.2f, 0, 0, 0, KIND_ENEMY, STATE_GROUND, 1000, 1000, 500, texID);
	else if ( kind == 1 )
		AddObject(x, y, z, 50, 0.2f, 0.2f, 0.2f, 0, 0, 0, KIND_ENEMY, STATE_GROUND, 2000, 2000, 1000, texID);
}

void SceneManager::SunkenSpawn(float x, float y, float z, GLuint texID)
{
	AddObject(x, y, z, 0, 0.6f, 0.6f, 0.6f, 0, 0, 0, KIND_ENEMY, STATE_GROUND, 3000, 3000, 750, texID);
}

void SceneManager::Sunken2Spawn(float x, float y, float z, GLuint texID)
{
	AddObject(x, y, z, 0, 0.6f, 0.6f, 0.6f, 0, 0, 0, KIND_ENEMY, STATE_GROUND, 3000, 3000, 750, texID);
}

void SceneManager::SpiderSpawn(float x, float y, float z, GLuint texID)
{
	AddObject(x, y, z, -5, 0.2f, 0.2f, 0.2f, 0, 0, 0, KIND_ENEMY, STATE_GROUND, 2000, 2000, 500, texID);
}

void SceneManager::BossSpawn(float x, float y, float z, GLuint texID)
{
	AddObject(x, y, z, -5, 1.f, 1.f, 1.f, 0, 0, 0, KIND_ENEMY, STATE_GROUND, 10000, 10000, 1500, texID);
}

int SceneManager::FindEmptySlot()
{
	int index = -1;
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL)
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		std::cout << "all slots are occpied" << std::endl;
	}

	return index;
}


bool SceneManager::RRCollision(float minX1, float minY1, float maxX1, float maxY1,
	float minX2, float minY2, float maxX2, float maxY2)
{
	if (minX1 > maxX2)
		return false;
	if (maxX1 < minX2)
		return false;
	if (minY1 > maxY2)
		return false;
	if (maxY1 < minY2)
		return false;
	//정밀한 충돌 계산
	//Here
	//
	return true;
}

bool SceneManager::BBCollision(float minX1, float minY1, float minZ1,
	float maxX1, float maxY1, float maxZ1,
	float minX2, float minY2, float minZ2,
	float maxX2, float maxY2, float maxZ2)
{
	if (minX1 > maxX2)
		return false;
	if (maxX1 < minX2)
		return false;
	if (minY1 > maxY2)
		return false;
	if (maxY1 < minY2)
		return false;
	if (minZ1 > maxZ2)
		return false;
	if (maxZ1 < minZ2)
		return false;
	//정밀한 충돌 계산
	//Here
	//
	return true;
}

void SceneManager::UpdateCollision()
{
	if (!m_GameEnd)
	{
		for (int i = 0; i < MAX_OBJECTS; i++)
		{
			if (m_Objects[i] == NULL)
				continue;
			int collisionCount = 0;

			for (int j = i + 1; j < MAX_OBJECTS; j++)
			{
				if (m_Objects[j] == NULL)
					continue;

				float minX1, minY1, maxX1, maxY1, minZ1, maxZ1;
				float minX2, minY2, maxX2, maxY2, minZ2, maxZ2;
				float pX, pY, pZ, sX, sY, sZ;

				m_Objects[i]->GetPosition(&pX, &pY, &pZ);
				m_Objects[i]->GetSize(&sX, &sY, &sZ);

				minX1 = pX - sX / 2.f;
				maxX1 = pX + sX / 2.f;
				minY1 = pY - sY / 2.f;
				maxY1 = pY + sY / 2.f;
				minZ1 = pZ - sZ / 2.f;
				maxZ1 = pZ + sZ / 2.f;

				m_Objects[j]->GetPosition(&pX, &pY, &pZ);
				m_Objects[j]->GetSize(&sX, &sY, &sZ);

				minX2 = pX - sX / 2.f;
				maxX2 = pX + sX / 2.f;
				minY2 = pY - sY / 2.f;
				maxY2 = pY + sY / 2.f;
				minZ2 = pZ - sZ / 2.f;
				maxZ2 = pZ + sZ / 2.f;


				if (BBCollision(minX1, minY1, minZ1, maxX1, maxY1, maxZ1, minX2, minY2, minZ2, maxX2, maxY2, maxZ2))
				{
					ProcessCollision(i, j);
				}

			}
			if (i == 0)
			{
				PlayerObject* tmp = (PlayerObject*)m_Objects[i];
				tmp->m_PrevPos = tmp->GetPosition();
			}
		}
	}
}

void SceneManager::ProcessCollision(int i, int j)
{
	if (!m_GameEnd)
	{
		int kind1, kind2;
		m_Objects[i]->GetKind(&kind1);
		m_Objects[j]->GetKind(&kind2);
		if (!m_Objects[i]->GetNoDamage())
		{
			if (kind1 == KIND_SUNKENBULLET && kind2 == KIND_HERO)
			{
				int playerHp;
				int bulletHp;

				Vector3D bulletVel;

				m_Objects[j]->GetHP(&playerHp);
				m_Objects[i]->GetHP(&bulletHp);

				m_Objects[j]->SetHP(playerHp - bulletHp);
				m_Objects[j]->GetHP(&playerHp);
				m_Objects[i]->SetHP(0);

				if (playerHp > 0)
				{
					m_Objects[j]->SetNoDamage(true);
					bulletVel = Scale(Normalize(m_Objects[i]->GetVel()), 125);

					m_Objects[HERO_ID]->AddForce(bulletVel.x, bulletVel.y, bulletVel.z, m_eTime);
				}
				else
				{
					m_GameEnd = true;

				}
		
			}
			else if (kind2 == KIND_SUNKENBULLET && kind1 == KIND_HERO)
			{
				int playerHp;
				int bulletHp;

				Vector3D playerVel;
				Vector3D bulletVel;
				m_Objects[i]->GetHP(&playerHp);
				m_Objects[j]->GetHP(&bulletHp);

				m_Objects[i]->SetHP(playerHp - bulletHp);
				m_Objects[i]->GetHP(&playerHp);
				m_Objects[j]->SetHP(0);

				if (playerHp > 0)
				{
					m_Objects[i]->SetNoDamage(true);
					bulletVel = Scale(Normalize(m_Objects[j]->GetVel()), 125);

					m_Objects[HERO_ID]->AddForce(bulletVel.x, bulletVel.y, bulletVel.z, m_eTime);
				}
				else
				{
					m_GameEnd = true;
	
				}
		
			}

			else if (kind1 == KIND_ENEMY && kind2 == KIND_HERO)
			{
				if (m_Objects[i]->m_BirthSeq == -1)
				{
					int playerHp;
					int enemyDamage;

					Vector3D playerPos;
					Vector3D enemyPos;
					Vector3D reflectVector;
					m_Objects[j]->GetHP(&playerHp);
					m_Objects[i]->GetDamage(&enemyDamage);

					m_Objects[j]->SetHP(playerHp - enemyDamage);
					m_Objects[j]->GetHP(&playerHp);

					playerPos = m_Objects[j]->GetPosition();
					enemyPos = m_Objects[i]->GetPosition();
					if (playerHp > 0)
					{
						m_Objects[j]->SetNoDamage(true);

						reflectVector = Scale(Normalize(Minus(playerPos, enemyPos)), 125);
						m_Objects[HERO_ID]->AddForce(reflectVector.x, reflectVector.y, reflectVector.z, m_eTime);
					}
					else
					{
						m_GameEnd = true;
					
					}
				}
			
			}
			else if (kind2 == KIND_ENEMY && kind1 == KIND_HERO)
			{
				if (m_Objects[j]->m_BirthSeq == -1)
				{
					int playerHp;
					int enemyDamage;

					Vector3D playerPos;
					Vector3D enemyPos;
					Vector3D reflectVector;
					m_Objects[i]->GetHP(&playerHp);
					m_Objects[j]->GetDamage(&enemyDamage);

					m_Objects[i]->SetHP(playerHp - enemyDamage);
					m_Objects[i]->GetHP(&playerHp);

					playerPos = m_Objects[i]->GetPosition();
					enemyPos = m_Objects[j]->GetPosition();
					if (playerHp > 0)
					{
						m_Objects[i]->SetNoDamage(true);

						reflectVector = Scale(Normalize(Minus(playerPos, enemyPos)), 125);
						m_Objects[HERO_ID]->AddForce(reflectVector.x, reflectVector.y, reflectVector.z, m_eTime);
					}
					else
					{
						m_GameEnd = true;
						
					}
				}

			}
		}
	
		if (kind1 == KIND_PLAYERBULLET && kind2 == KIND_ENEMY)
		{
			int enemyHp;
			int bulletHp;
			m_Objects[j]->GetHP(&enemyHp);
			m_Objects[i]->GetHP(&bulletHp);

			m_Objects[j]->SetHP(enemyHp - bulletHp);
			m_Objects[j]->GetHP(&enemyHp);
			m_Objects[i]->SetHP(0);

			m_Objects[j]->SetTargetOn(true);
			if (enemyHp <= 0)
			{
				GLuint objectTextureID;
				m_Objects[j]->GetTexture(&objectTextureID);
				m_Sound->PlaySounds(m_MobDeathSound, false, 0.9);

				if (objectTextureID == m_Tex[11])
					m_GameEnd = true;
			}	

		}
		else if (kind2 == KIND_PLAYERBULLET && kind1 == KIND_ENEMY)
		{
			int enemyHp;
			int bulletHp;
			m_Objects[i]->GetHP(&enemyHp);
			m_Objects[j]->GetHP(&bulletHp);

			m_Objects[i]->SetHP(enemyHp - bulletHp);
			m_Objects[i]->GetHP(&enemyHp);
			m_Objects[j]->SetHP(0);

			m_Objects[i]->SetTargetOn(true);
			if (enemyHp <= 0)
			{
				GLuint objectTextureID;
				m_Objects[i]->GetTexture(&objectTextureID);
				m_Sound->PlaySounds(m_MobDeathSound, false, 0.9);

				if (objectTextureID == m_Tex[11])
					m_GameEnd = true;
			}

		}
		if (kind1 == KIND_BUILDING && kind2 == KIND_HERO)
		{
			if (m_Objects[j] != NULL)
			{
				PlayerObject* tmp = (PlayerObject*)m_Objects[j];
				m_Objects[j]->SetPosition(tmp->m_PrevPos);

				m_Objects[j]->SetVel(0, 0, 0);
				m_Objects[j]->SetAcc(0, 0, 0);
			}
		}
		else if (kind2 == KIND_BUILDING && kind1 == KIND_HERO)
		{
			if (m_Objects[i] != NULL)
			{
				PlayerObject* tmp = (PlayerObject*)m_Objects[i];
				m_Objects[i]->SetPosition(tmp->m_PrevPos);

				m_Objects[i]->SetVel(0, 0, 0);
				m_Objects[j]->SetAcc(0, 0, 0);
			}
		}

		if (kind1 == KIND_BUILDING && kind2 == KIND_PLAYERBULLET)
		{
			m_Objects[j]->SetHP(0);
		}
		else if (kind2 == KIND_BUILDING && kind1 == KIND_PLAYERBULLET)
		{
			m_Objects[i]->SetHP(0);
		}

		if (kind1 == KIND_BUILDING && kind2 == KIND_SUNKENBULLET)
		{
			m_Objects[j]->SetHP(0);
		}
		else if (kind2 == KIND_BUILDING && kind1 == KIND_SUNKENBULLET)
		{
			m_Objects[i]->SetHP(0);
		}
	}
	return;
}

void SceneManager::DoGarbageCollect()
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL)
		{	
			int objectKind;
			m_Objects[i]->GetKind(&objectKind);
			//HP 0 Kill
			int objectHp;
			m_Objects[i]->GetHP(&objectHp);
		
			if (objectHp <= 0)
			{
				DeleteObject(i);
				continue;
			}
			//Check Out of Bound
			float px, py, pz;
			
			m_Objects[i]->GetPosition(&px, &py, &pz);
			if (px > 250.f || px < -250.f || py > 250.f || py < -250.f)
			{
				if (objectKind == KIND_PLAYERBULLET || objectKind == KIND_SUNKENBULLET)
				{
					DeleteObject(i);
					continue;
				}
			}

			//Check Velocity
			float vx, vy, vz;
			m_Objects[i]->GetVel(&vx, &vy, &vz);
			float mag = sqrtf(vx*vx + vy * vy + vz * vz);
			if (mag < FLT_EPSILON )
			{
				if (objectKind == KIND_PLAYERBULLET || objectKind == KIND_SUNKENBULLET)
				{
					DeleteObject(i);
					continue;
				}
			}
		}
	}
}