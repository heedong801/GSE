/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "SceneManager.h"

//#include "Renderer.h"
//#include "Object.h"

SceneManager * g_ScnMgr = nullptr;
DWORD g_PrevTime = 0;


using namespace std;

bool g_KeyW, g_KeyA, g_KeyS, g_KeyD, g_KeySP;

int g_Shoot = SHOOT_NONE;

void RenderScene(void)
{	
	if (g_PrevTime == 0)
	{
		g_PrevTime = timeGetTime();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

	DWORD currentTime = timeGetTime();
	DWORD elapsedTime = currentTime - g_PrevTime;
	g_PrevTime = currentTime;
	float eTime = static_cast<float>(elapsedTime) / 1000.f;

	float fx = 0.f, fy = 0.f, fz = 0.f;
	float amount = 7.f;
	float jumpAmount = 200.f;

	if (g_KeyW)
	{
		fy += amount;
	}
	if (g_KeyS)
	{
		fy -= amount;
	}
	if (g_KeyA)
	{
		fx -= amount;
	}
	if (g_KeyD)
	{
		fx += amount;
	}
	if (g_KeySP)
	{
		fz += jumpAmount;
	}
	//Normalize(&fx, &fy);
	g_ScnMgr->AddForce(fx, fy, fz,eTime);
	g_ScnMgr->Shoot(g_Shoot);
	g_ScnMgr->Update(eTime);
	g_ScnMgr->UpdateCollision();
	g_ScnMgr->RenderScene();
	g_ScnMgr->DoGarbageCollect();
	glutSwapBuffers();

}

void Idle(void)
{	
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	//printf("%d %d\n", x, y);
}

void KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		g_KeyA = true;
		break;
	case 'w':
	case 'W':
		g_KeyW = true;
		break;
	case 'd':
	case 'D':
		g_KeyD = true;
		break;
	case 's':
	case 'S':
		g_KeyS = true;
		break;
	case ' ':
		g_KeySP = true;
	}
	g_ScnMgr->KeyInput(key, x, y);
}

void SpecialKeyDownInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_Shoot = SHOOT_UP;
		break;
	case GLUT_KEY_DOWN:
		g_Shoot = SHOOT_DOWN;
		break;
	case GLUT_KEY_LEFT:
		g_Shoot = SHOOT_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		g_Shoot = SHOOT_RIGHT;
		break;
	}
	g_ScnMgr->SpecialKeyInput(key, x, y);
}

void SpecialKeyUpInput(int key, int x, int y)
{
	g_Shoot = SHOOT_NONE;
}

void keyboard_up(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		g_KeyA = false;
		break;
	case 'w':
	case 'W':
		g_KeyW = false;
		break;
	case 'd':
	case 'D':
		g_KeyD = false;
		break;
	case 's':
	case 'S':
		g_KeyS = false;
		break;
	case ' ':
		g_KeySP = false;
	}

	//g_ScnMgr->keyboard_up(key, x, y);

	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);
	g_ScnMgr = new SceneManager();

	glutKeyboardUpFunc(keyboard_up);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	glutMainLoop();
	
	delete g_ScnMgr;

    return 0;
}


