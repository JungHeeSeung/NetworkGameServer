#pragma once

enum
{
	OBJ_NORMAL
};

class Object
{
public:
	Object();
	~Object();

	void SetPos(float x, float y, float z);
	void GetPos(float* x, float* y, float* z);

	void SetVol(float x, float y, float z);
	void GetVol(float* x, float* y, float* z);

	void SetColor(float r, float g, float b, float a);
	void GetColor(float* r, float* g, float* b, float* a);

	void Update(float elpasedTimeInSecond);

private:
	float	m_r, m_g, m_b, m_a;		// color
	float	m_posX, m_posY, m_posZ;	// position
	float	m_volX, m_volY, m_volZ;	// volume
};

struct Packet
{
	bool m_KeyUp;
	bool m_KeyDown;
	bool m_KeyLeft;
	bool m_KeyRight;
};