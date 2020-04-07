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


void Object::Update(float elpasedTimeInSecond)
{
	/*float x, y, z;
	
	GetPos(&x, &y, &z);
	SetPos(x, y, z);*/
	//-------------------------------------------------------------------
}
