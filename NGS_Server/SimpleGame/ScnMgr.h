#pragma once
#include "Renderer.h"
#include "Object.h"
#include "Globals.h"
#include "Server.h"


class ScnMgr
{
public:
	ScnMgr();
	~ScnMgr();

	void RenderScene();

	int AddObject(float x, float y, float z,
		float sx, float sy, float sz,
		float vx, float vy, float vz,
		float r, float g, float b, float a,
		float mass,
		float fricCoef,
		float hp,
		int type);
	void DeleteObject(int idx);

	void Update(float elpasedTimeInSecond);
	
	void KeyDownInput(unsigned char key, int x, int y);
	void KeyUpInput(unsigned char key, int x, int y);
	void SpecialKeyDownInput(int key, int x, int y);
	void SpecialKeyUpInput(int key, int x, int y);

private:
	Renderer* m_Renderer = NULL;
	Object* m_Obj[MAX_OBJ_COUNT];
	Object* m_Map[MAX_MAP_X][MAX_MAP_Y];
	SendData m_Info;

	Server* server;


	bool m_KeyW = false;
	bool m_KeyS = false;
	bool m_KeyA = false;
	bool m_KeyD = false;

	bool m_KeyUp = false;
	bool m_KeyDown = false;
	bool m_KeyLeft = false;
	bool m_KeyRight = false;
};

