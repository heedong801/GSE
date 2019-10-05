#pragma once
#include "math.h"
#include "Global.h"
#include "stdafx.h"

enum Direction
{
	DOWN, LEFT, RIGHT, UP
};

struct Vector2D {
	float x = 0, y = 0;
};

struct Vector3D {
	float x = 0, y = 0, z = 0;
};

struct WHSize {
	float width = 0, height = 0;
};

struct Color4 {
	float r = 0, g = 0, b = 0, a = 1;
};


class Object
{
protected:
	Vector3D m_Position;
	Vector3D m_Size;
	Color4 m_Color;

	Vector3D m_Velocity;
	Vector3D m_Accel;

	unsigned char arrowPos = 0x00;
	float m_Mass;
	float m_frict;
	float m_posHeight = 0.f;

	int m_Kind;
	int m_Hp;
	int m_MaxHp;
	int m_Damage;

	int m_State;
	GLuint m_Texture;

	int m_SpriteSeq;
	int m_Seq = 0;

	bool m_NoDamage = false;


	Vector3D m_TargetPos;
	bool m_TargetOn = false;
	
public:
	Object();
	~Object();

	void SetX(float x) { m_Position.x = x; }
	void SetY(float y) { m_Position.y = y; }
	float GetX() const { return m_Position.x; }
	float GetY() const { return m_Position.y; }
	void SetPosition(float x, float y, float z) { m_Position = { x,y,z }; }
	void SetPosition(Vector3D vector) { m_Position = vector; }
	Vector3D GetPosition() const { return m_Position; }
	void GetPosition(float * x, float * y, float *z) { *x = m_Position.x; *y = m_Position.y; *z = m_Position.z; }
	void SetShadowHeight(float h) { m_posHeight = h; }
	void GetShadowHeight(float *h) { *h = m_posHeight; }
	void SetHP(int hp) { m_Hp = hp; }
	void GetHP(int *hp) { *hp= m_Hp; }
	void SetMaxHp(int maxHp) { m_MaxHp = maxHp; }
	void GetMaxHP(int *maxhp) { *maxhp = m_MaxHp; }
	void SetDamage(int d) { m_Damage = d; }
	void GetDamage(int *d) { *d = m_Damage; }
	void SetSize(float x, float y, float z) { m_Size = { x, y, z }; }
	void GetSize(float * x, float * y, float* z) { *x = m_Size.x; *y = m_Size.y; *z = m_Size.z; }

	int m_BirthCount = 0;
	int m_BirthSeq = 0;

	void SetVel(float x, float y, float z) { m_Velocity.x = x, m_Velocity.y = y , m_Velocity.z = z; }
	void GetVel(float *x, float *y, float *z) { *x = m_Velocity.x, *y = m_Velocity.y, *z = m_Velocity.z; }
	Vector3D GetVel() const { return m_Velocity; }
	void SetAcc(float x, float y, float z) { m_Accel.x = x, m_Accel.y = y , m_Accel.z = z; }
	void GetAcc(float *x, float *y, float *z) { *x = m_Accel.x, *y = m_Accel.y, *z = m_Accel.z; }
	void SetMas(float w) { m_Mass = w; }
	float GetMas() { return m_Mass; }
	void SetFriction(float f) { m_frict = f; }
	float GetFriction() { return m_frict; }
	void SetKind(int kind) { m_Kind = kind; }
	void GetKind(int *kind) { *kind = m_Kind; }
	void SetState(int state) { m_State = state; }
	void GetState(int *state) { *state = m_State; }
	void SetSpriteSeq(int seq) { m_SpriteSeq = seq; }
	void GetSpriteSeq(int *seq) { *seq = m_SpriteSeq; }

	void SetColor(float r, float g, float b) { m_Color = { r, g, b }; }
	void SetColor(float r, float g, float b, float a) { m_Color = { r, g, b, a }; }
	void SetAlpha(float a) { m_Color.a = a; }
	void GetColor(float * r, float * g, float * b, float * a) { *r = m_Color.r; *g = m_Color.g; *b = m_Color.b; *a = m_Color.a; }

	void SetTexture(GLuint texID) { m_Texture = texID; }
	void GetTexture(GLuint* texID) { *texID = m_Texture; }
	
	void SetTarget(Vector3D playerPos) { m_TargetPos = playerPos; }
	void GetTarget(float * x, float * y, float *z) { *x = m_TargetPos.x; *y = m_TargetPos.y; *z = m_TargetPos.z; }
	void SetTargetOn(bool lockon) { m_TargetOn = lockon; }
	bool GetTargetOn() { return m_TargetOn; }
	void SetNoDamage(bool nod) { m_NoDamage = nod; }
	bool GetNoDamage() { return m_NoDamage; }

	virtual void KeyInput(unsigned char key, int x, int y) {};
	virtual void keyboard_up(unsigned char key, int x, int y) {};
	virtual void SpecialKeyInput(int key, int x, int y) {};

	virtual void SetDirection(Direction Dir) {}
	virtual Direction GetDirection() const { return Direction::DOWN; }

	virtual void Update(float eTime);
	void AddForce(float fX, float fY, float fZ, float eTime);

};

class PlayerObject : public Object
{
private:
	int m_PlayerState;
	
	Direction m_Direction = DOWN;
public:
	PlayerObject();
	~PlayerObject();

	void KeyInput(unsigned char key, int x, int y);
	void keyboard_up(unsigned char key, int x, int y);
	void SpecialKeyInput(int key, int x, int y);

	Vector3D m_PrevPos;

	void SetDirection(Direction Dir) { m_Direction = Dir; }
	Direction GetDirection() const { return m_Direction; }
};

class FlyObject : public Object
{
private:
	double m_DoubleX = 0;
public:
	FlyObject();
	~FlyObject();
	void Update(float eTime);
	
};

class SunkenObject : public Object
{
private:
	//sunken
	bool m_Attack = false;
	int m_Count = 0;
public:
	SunkenObject();
	~SunkenObject();

	void Update(float eTime);

	void SetAttack(bool attack) { m_Attack = attack; }
	bool GetAttack() { return m_Attack; }

};

class Sunken2Object : public Object
{
private:
	//sunken
	bool m_Attack = false;
	int m_Count = 0;
public:
	Sunken2Object();
	~Sunken2Object();

	void Update(float eTime);

	void SetAttack(bool attack) { m_Attack = attack; }
	bool GetAttack() { return m_Attack; }

};

class SpiderObject : public Object
{
private:

public:
	SpiderObject();
	~SpiderObject();

	void Update(float eTime);

};

class BossObject : public Object
{
private:
	bool m_Attack = false;
	bool m_Spawn = false;
public:
	BossObject();
	~BossObject();

	void SetAttack(bool attack) { m_Attack = attack; }
	bool GetAttack() { return m_Attack; }
	void SetSpawn(bool s) { m_Spawn = s; }
	bool GetSpawn() { return m_Spawn; }

	int m_BossAttack = 0;
	void Update(float eTime);

};