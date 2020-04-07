/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "ScnMgr.h"
#include "Client.h"

ScnMgr* g_ScnMgr = NULL;
int g_prevTimeInMillisecond = 0;

void RenderScene(int temp)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - g_prevTimeInMillisecond;

	g_prevTimeInMillisecond = currentTime;
	
	g_ScnMgr->Update((float)elapsedTime / 1000.f /* 초로 바꿔서 넣어줄 것 */);
	g_ScnMgr->RenderScene();
	
	glutSwapBuffers();
	glutTimerFunc(16, RenderScene, 16);

}

void Idle(void)
{
	//RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	//RenderScene();
}

void KeyDownInput(unsigned char key, int x, int y)
{
	g_ScnMgr->KeyDownInput(key, x, y);
}

void KeyUpInput(unsigned char key, int x, int y)
{
	g_ScnMgr->KeyUpInput(key, x, y);
}

void SpecialKeyDownInput(int key, int x, int y)
{
	g_ScnMgr->SpecialKeyDownInput(key, x, y);
}

void SpecialKeyUpInput(int key, int x, int y)
{
	g_ScnMgr->SpecialKeyUpInput(key, x, y);
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Client Window");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize ScnMgr
	g_ScnMgr = new ScnMgr();

	glutDisplayFunc(Idle);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(16, RenderScene, 16);

	glutMainLoop();


    return 0;
}

