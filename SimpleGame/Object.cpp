#include "stdafx.h"
#include "Object.h"

#include <float.h>

Object::Object()
{

}


Object::~Object()
{
}

void Object::Update(float eTime)
{	
	if (m_NoDamage)
	{
		static int count = 0;
		
		count++;
		int a = count % 10;
		if( a < 5)
			SetAlpha(0);
		else
			SetAlpha(1);
		if (count > 100)
		{
			m_NoDamage = false;
			SetAlpha(1);
			count = 0;
		}
	}
	float velocity = sqrtf(m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y + m_Velocity.z * m_Velocity.z); // size of velocity

	if (velocity < FLT_EPSILON)
	{
		m_Velocity = { 0, };
	}
	else
	{
		////////////마찰력 계산///////////////////
		if (m_State == STATE_GROUND)
		{
			//gravity force
			float gZ;
			gZ = GRAVITY * m_Mass; //수직항력
			float friction;
			friction = m_frict * gZ;

			//friction
			float fX, fY;
			fX = -friction * m_Velocity.x / velocity;
			fY = -friction * m_Velocity.y / velocity;

			//Calc Acc
			float accX, accY;
			accX = fX / m_Mass;
			accY = fY / m_Mass;

			float afterVelX = m_Velocity.x + eTime * accX;
			float afterVelY = m_Velocity.y + eTime * accY;

			if (afterVelX * m_Velocity.x < 0.f)
				m_Velocity.x = 0;
			else
				m_Velocity.x += eTime * accX;

			if (afterVelY * m_Velocity.y < 0.f)
				m_Velocity.y = 0;
			else
				m_Velocity.y += eTime * accY;
		}
		else if( m_State == STATE_AIR )
			m_Velocity.z = m_Velocity.z + eTime * -GRAVITY;
	}
	
	//속도 계산
	m_Velocity.x += eTime * m_Accel.x;
	m_Velocity.y += eTime * m_Accel.y;
	m_Velocity.z += eTime * m_Accel.z;

	//위치 계산
	m_Position.x += eTime * m_Velocity.x;
	m_Position.y += eTime * m_Velocity.y;
	m_Position.z += eTime * m_Velocity.z;

	if (m_Position.z > 0.f)
	{
		m_State = STATE_AIR;
	}
	else
	{
		m_State = STATE_GROUND;
		m_Position.z = 0.f;
		m_Velocity.z = 0.f;
	}
}

void Object::AddForce(float fX, float fY, float fZ, float eTime)
{
	//Calc acc
	m_Accel.x = fX / m_Mass;
	m_Accel.y = fY / m_Mass;
	m_Accel.z = fZ / m_Mass;

	m_Velocity.x += m_Accel.x * eTime;
	m_Velocity.y += m_Accel.y * eTime;
	m_Velocity.z += m_Accel.z * eTime;

	m_Accel.x = 0.f;
	m_Accel.y = 0.f;
	m_Accel.z = 0.f;

}



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

PlayerObject::PlayerObject()
{
}


PlayerObject::~PlayerObject()
{
}

void PlayerObject::KeyInput(unsigned char key, int x, int y)
{
	//if (key == 'w' || key == 'W')
	//	SetDirection(Dir)
	//if (key == 'a' || key == 'A')
	//arrowPos -= 0x08;
	//if (key == 's' || key == 'S')
	//arrowPos -= 0x04;
	//if (key == 'd' || key == 'D')
	//arrowPos -= 0x10;
}

void PlayerObject::keyboard_up(unsigned char key, int x, int y)
{
	/*if (key == 'w' || key == 'W')
	arrowPos -= 0x02;
	if (key == 'a' || key == 'A')
	arrowPos -= 0x08;
	if (key == 's' || key == 'S')
	arrowPos -= 0x04;
	if (key == 'd' || key == 'D')
	arrowPos -= 0x10;*/
}

void PlayerObject::SpecialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		m_Direction = Direction::UP;
		break;
	case GLUT_KEY_DOWN:
		m_Direction = Direction::DOWN;
		break;
	case GLUT_KEY_LEFT:
		m_Direction = Direction::LEFT;
		break;
	case GLUT_KEY_RIGHT:
		m_Direction = Direction::RIGHT;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

FlyObject::FlyObject()
{
}


FlyObject::~FlyObject()
{
}

void FlyObject::Update(float eTime)
{
	if (m_BirthSeq == -1)
	{
		//fly animation
		/////////////////////////////////////////////////////////////
		m_Seq++;
		m_SpriteSeq = m_Seq % 10;
		if (m_SpriteSeq < 5)
			m_SpriteSeq = 0;
		else
			m_SpriteSeq = 1;

		//////////////////////////////////////////////////////////////


		if (!m_TargetOn)
		{
			m_DoubleX += 0.1;
			m_Position.x = 0.01 * cos(m_DoubleX) + m_Position.x;
			m_Position.y = 0.01 * sin(m_DoubleX) + m_Position.y;
		}
		else
		{
			if (m_TargetPos.x > m_Position.x)
				m_Position.x += 0.01;
			else
				m_Position.x -= 0.01;
			if (m_TargetPos.y > m_Position.y)
				m_Position.y += 0.01;
			else
				m_Position.y -= 0.01;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

SunkenObject::SunkenObject()
{
}


SunkenObject::~SunkenObject()
{
}

void SunkenObject::Update(float eTime)
{
	if (m_BirthSeq == -1)
	{
		//sunken animation
		/////////////////////////////////////////////////////////////
		m_Seq++;
		m_SpriteSeq = m_Seq % 120;
		if (m_SpriteSeq < 20)
			m_SpriteSeq = 0;
		else if (m_SpriteSeq < 80)
		{
			if (m_Count == 15)
				m_Attack = true;
			else
				m_Attack = false;
			m_Count++;

			m_SpriteSeq = 1;
		}
		else
		{
			m_Count = 0;
			m_SpriteSeq = 2;
		}

		//////////////////////////////////////////////////////////////
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

Sunken2Object::Sunken2Object()
{
}


Sunken2Object::~Sunken2Object()
{
}

void Sunken2Object::Update(float eTime)
{
	if (m_BirthSeq == -1)
	{
		//sunken2 animation
		/////////////////////////////////////////////////////////////
		m_Seq++;
		m_SpriteSeq = m_Seq % 120;
		if (m_SpriteSeq < 20)
			m_SpriteSeq = 0;
		else if (m_SpriteSeq < 80)
		{
			if (m_Count == 15)
				m_Attack = true;
			else
				m_Attack = false;
			m_Count++;

			m_SpriteSeq = 1;
		}
		else
		{
			m_Count = 0;
			m_SpriteSeq = 2;
		}

		//////////////////////////////////////////////////////////////
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

SpiderObject::SpiderObject()
{
}


SpiderObject::~SpiderObject()
{
}

void SpiderObject::Update(float eTime)
{
	if (m_BirthSeq == -1)
	{
		//spider animation
		/////////////////////////////////////////////////////////////
		m_Seq++;
		m_SpriteSeq = m_Seq % 8;
		m_TargetOn = true;
		//////////////////////////////////////////////////////////////


		if (m_TargetPos.x > m_Position.x)
			m_Position.x += 0.01;
		else
			m_Position.x -= 0.01;
		if (m_TargetPos.y > m_Position.y)
			m_Position.y += 0.01;
		else
			m_Position.y -= 0.01;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

BossObject::BossObject()
{
}


BossObject::~BossObject()
{
}

void BossObject::Update(float eTime)
{
	
	if (m_BirthSeq == -1)
	{
		//boss animation
		/////////////////////////////////////////////////////////////
		m_Seq++;
		m_BossAttack++;
		m_TargetOn = true;

		m_SpriteSeq = m_Seq % 121;
		if (m_BossAttack < 150)
		{
			if (m_SpriteSeq < 20)
				m_SpriteSeq = 0;
			else if (m_SpriteSeq < 60)
				m_SpriteSeq = 1;
			else if (m_SpriteSeq < 80)
				m_SpriteSeq = 0;
			else if (m_SpriteSeq < 120)
				m_SpriteSeq = 1;
			else
			{
				m_SpriteSeq = 0;
				m_Seq = 0;
			}
		}
		else
		{
			if (m_SpriteSeq < 10)
				m_SpriteSeq = 0;
			else if (m_SpriteSeq < 20)
				m_SpriteSeq = 1;
			else if (m_SpriteSeq < 30)
				m_SpriteSeq = 2;
			else if (m_SpriteSeq < 40)
				m_SpriteSeq = 3;
			else if (m_SpriteSeq < 120)
			{

				if (m_SpriteSeq == 40 || m_SpriteSeq == 44 || m_SpriteSeq == 48 || m_SpriteSeq == 60 ||
					m_SpriteSeq == 64 || m_SpriteSeq == 68 || m_SpriteSeq == 80 || m_SpriteSeq == 84 ||
					m_SpriteSeq == 88 || m_SpriteSeq == 100 || m_SpriteSeq == 104 || m_SpriteSeq == 108)
				{
					if (m_SpriteSeq == 108)
						m_Spawn = true;
					m_Attack = true;
				}
				else
					m_Attack = false;

				m_SpriteSeq = 4;
			}
			else
			{
				m_SpriteSeq = 0;
				m_Seq = 0;
				m_BossAttack = 0;
				m_Spawn = false;
			}
		}

		//////////////////////////////////////////////////////////////

	}
}

