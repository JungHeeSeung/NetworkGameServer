#include "stdafx.h"
#include "ScnMgr.h"
#include "Dependencies\freeglut.h"

ScnMgr::ScnMgr()
{
	server = new Server();
	// Initialize Renderer
	m_Renderer = new Renderer(1000, 1000);
	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	// Initialize m_Obj
	for (int i = 0; i < MAX_OBJ_COUNT; i++)
	{
		m_Obj[i] = NULL;
	}

	// Initialize m_Map
	for (int x = 0; x < MAX_MAP_X; x++)
	{
		for (int y = 0; y < MAX_MAP_Y; y++)
		{
			m_Map[x][y] = new Object();
			m_Map[x][y]->SetPos(x - 4.f, y - 4.f, 0);
			m_Map[x][y]->SetVel(0, 0, 0);
			m_Map[x][y]->SetVol(1, 1, 1);
			m_Map[x][y]->SetMass(10);

			if ((x + y + 1) % 2 != 1) // 1이 아니면 흰색 타일
			{
				m_Map[x][y]->SetColor(1, 1, 1, 1);
				m_Map[x][y]->SetType(OBJ_WHITETILE);

			}
			else					// 검은색 타일일 경우
			{
				m_Map[x][y]->SetColor(0, 0, 0, 1);
				m_Map[x][y]->SetType(OBJ_BLACKTILE);
			}
		}
	}

	m_Obj[HERO_ID] = new Object();
	m_Obj[HERO_ID]->SetPos(-4, -4, 0);
	m_Obj[HERO_ID]->SetVel(0, 0, 0);
	m_Obj[HERO_ID]->SetVol(0.5, 0.5, 0.5);
	m_Obj[HERO_ID]->SetMass(10);
	m_Obj[HERO_ID]->SetColor(1, 0, 0, 1);

}


ScnMgr::~ScnMgr()
{
}

void ScnMgr::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// 맵을 그리자!

	for (int i = 0; i < MAX_MAP_X; i++)
	{
		for (int j = 0; j < MAX_MAP_Y; j++)
		{
			float x, y, z;
			float sx, sy, sz;
			float r, g, b, a;

			m_Map[i][j]->GetPos(&x, &y, &z);
			x = 100.f * x;  // convert to pixel size
			y = 100.f * y;
			z = 100.f * z;
			m_Map[i][j]->GetVol(&sx, &sy, &sz);
			sx = 100.f * sx;  // convert to pixel size
			sy = 100.f * sy;
			sz = 100.f * sz;
			m_Map[i][j]->GetColor(&r, &g, &b, &a);

			m_Renderer->DrawSolidRect(x, y, z, sx, r, g, b, a);
		}
	}

	// OBJ 그리는 부분
	for (int i = 0; i < MAX_OBJ_COUNT; i++)
	{
		if (m_Obj[i] != NULL)
		{
			float x, y, z;
			float sx, sy, sz;
			float r, g, b, a;

			m_Obj[i]->GetPos(&x, &y, &z);
			x = 100.f * x;  // convert to pixel size
			y = 100.f * y;
			z = 100.f * z;
			m_Obj[i]->GetVol(&sx, &sy, &sz);
			sx = 100.f * sx;  // convert to pixel size
			sy = 100.f * sy;
			sz = 100.f * sz;
			m_Obj[i]->GetColor(&r, &g, &b, &a);

			m_Renderer->DrawSolidRect(x, y, z, sx, r, g, b, a);
		}
	}

	
}

void ScnMgr::KeyDownInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		m_KeyW = true;
	}
	else if (key == 'a' || key == 'A')
	{
		m_KeyA = true;
	}
	else if (key == 's' || key == 'S')
	{
		m_KeyS = true;
	}
	else if (key == 'd' || key == 'D')
	{
		m_KeyD = true;
	}
}

void ScnMgr::KeyUpInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		m_KeyW = false;
	}
	else if (key == 'a' || key == 'A')
	{
		m_KeyA = false;
	}
	else if (key == 's' || key == 'S')
	{
		m_KeyS = false;
	}
	else if (key == 'd' || key == 'D')
	{
		m_KeyD = false;
	}
}

void ScnMgr::SpecialKeyDownInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		m_KeyUp = true;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		m_KeyDown = true;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		m_KeyLeft = true;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		m_KeyRight = true;
	}
}

void ScnMgr::SpecialKeyUpInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		m_KeyUp = false;
	}
	else if (key == GLUT_KEY_DOWN)
	{
		m_KeyDown = false;
	}
	else if (key == GLUT_KEY_LEFT)
	{
		m_KeyLeft = false;
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		m_KeyRight = false;
	}
}

int ScnMgr::AddObject(float x, float y, float z,
	float sx, float sy, float sz,
	float vx, float vy, float vz,
	float r, float g, float b, float a,
	float mass,
	float fricCoef,
	float hp,
	int type)
{
	// Search Empty slot idx
	int idx = -1;
	for (int i = 0; i < MAX_OBJ_COUNT; i++)
	{
		if (m_Obj[i] == NULL)
		{
			idx = i;
			break;
		}
	}

	if (idx == -1)
	{
		std::cout << "No more empty slot" << std::endl;
		return -1;
	}

	m_Obj[idx] = new Object();
	m_Obj[idx]->SetPos(x, y, z);
	m_Obj[idx]->SetVel(vx, vy, vz);
	m_Obj[idx]->SetVol(sx, sy, sz);
	m_Obj[idx]->SetMass(mass);
	m_Obj[idx]->SetColor(r, g, b, a);
	m_Obj[idx]->SetType(type);

	return idx;
}

void ScnMgr::DeleteObject(int idx)
{
	if (idx < 0)
	{
		std::cout << "Input idx is negative  : " << idx << std::endl;
		return;
	}

	if (idx >= MAX_OBJ_COUNT)
	{
		std::cout << "m_Obj[" << idx << "] is NULL" << std::endl;;

		return;
	}

	if (m_Obj[idx] == NULL)
	{
		std::cout << "m_Obj[" << idx << "] is NULL" << std::endl;;
		return;
	}
	delete m_Obj[idx];
	m_Obj[idx] = NULL;
}


void ScnMgr::Update(float elapsedTime)
{
	m_Info = *server->RecvBufFromClient();

	static int x = 0, y = 0;

	if (m_Info.m_KeyUp)
	{ 
		if (y < 7)
		{
			y += 1;
		}
	}
	if (m_Info.m_KeyDown)
	{
		if (y > 0)
		{
			y -= 1;
		}
	}
	if (m_Info.m_KeyLeft)
	{
		if (x > 0)
		{
			x -= 1;
		}
	}
	if (m_Info.m_KeyRight)
	{
		if (x < 7)
		{
			x += 1;
		}
	}
	
	float posX, posY, posZ;
	m_Map[x][y]->GetPos(&posX, &posY, &posZ);
	m_Obj[HERO_ID]->SetPos(posX, posY, posZ);

	server->SendBufToClient(*m_Obj[HERO_ID]);

	//---------------------------------------------------------------------------

	for (int i = 0; i < MAX_OBJ_COUNT; i++)
	{
		if (m_Obj[i] != NULL)
		{
			m_Obj[i]->Update(elapsedTime);
		}
	}
}



