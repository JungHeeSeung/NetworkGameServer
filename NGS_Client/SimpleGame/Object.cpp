#include "stdafx.h"
#include "Object.h"
#include <math.h>
#include <iostream>

Object::Object()
{
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;

	m_volX = 0;
	m_volY = 0;
	m_volZ = 0;

	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_a = 0;

	m_accX = 0;
	m_accY = 0;
	m_accZ = 0;

	m_velX = 0;
	m_velY = 0;
	m_velZ = 0;

	m_mass = 0;
	m_type = 0;
}


Object::~Object()
{
}

void Object::SetPos(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}

void Object::GetPos(float* x, float* y, float* z)
{
	*x = m_posX;
	*y = m_posY;
	*z = m_posZ;
}

void Object::SetVol(float x, float y, float z)
{
	m_volX = x;
	m_volY = y;
	m_volZ = z;
}

void Object::GetVol(float* x, float* y, float* z)
{
	*x = m_volX;
	*y = m_volY;
	*z = m_volZ;
}

void Object::SetVel(float x, float y, float z)
{
	m_velX = x;
	m_velY = y;
	m_velZ = z;
}

void Object::GetVel(float* x, float* y, float* z)
{
	*x = m_velX;
	*y = m_velY;
	*z = m_velZ;
}

void Object::SetAcc(float x, float y, float z)
{
	m_accX = x;
	m_accY = y;
	m_accZ = z;
}

void Object::GetAcc(float* x, float* y, float* z)
{
	*x = m_accX;
	*y = m_accY;
	*z = m_accZ;
}

void Object::SetColor(float r, float g, float b, float a)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

void Object::GetColor(float* r, float* g, float* b, float* a)
{
	*r = m_r;
	*g = m_g;
	*b = m_b;
	*a = m_a;
}

void Object::SetMass(float mass)
{
	m_mass = mass;
}

void Object::GetMass(float* mass)
{
	*mass = m_mass;
}

void Object::SetType(int type)
{
	m_type = type;
}

void Object::GetType(int* type)
{
	*type = m_type;
}

void Object::InitThis()
{
	m_posX, m_posY, m_posZ = -1.f;
	m_velX, m_velY, m_velZ = -1.f;
	m_accX, m_accY, m_accZ = -1.f;
	m_volX, m_volY, m_volZ = -1.f;
	m_r, m_g, m_b = -1.f;
	m_mass = -1.f;

}


void Object::Update(float elpasedTimeInSecond)
{
	float x, y, z;
	
	GetPos(&x, &y, &z);
	SetPos(x, y, z);
	

	//-------------------------------------------------------------------
}

SendData::SendData()
{
	bool m_KeyUp = false;
	bool m_KeyDown = false;
	bool m_KeyLeft = false;
	bool m_KeyRight = false;
}

SendData::~SendData()
{
}
