#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "gl/glMath.h"
#include "gl/glWindow.h"

int Width = 1600;
int Height = 900;

#define true 1
#define false 0
typedef int bool;

glFramebuffer Framebuffer;

//Fps
int Frame = 0;
int Final_Time;
int Init_Time = 0;
int Final_Fps = 0;
char CharFPS[64];

void CalcFps(const char* Title)
{
	Frame++;
	Final_Time = time(NULL);
	if (Final_Time - Init_Time > 0)
	{
		Final_Fps = Frame / (Final_Time - Init_Time);
		Frame = 0;
		Init_Time = Final_Time;

		sprintf(CharFPS, "%s fps: %d ms: %f", Title, Final_Fps, 1000.0 / Final_Fps);
		glSetWindowTitle(CharFPS);
	}
}

float DeltaTime = 0.0;
float LastTime = 0.0;

float GetDeltaTime()
{
	float CurrentTime = (float)SDL_GetTicks() / 1000.0;
	DeltaTime = CurrentTime - LastTime;
	LastTime = CurrentTime;

	return DeltaTime;
}

#include "game/Renderer.h"


int main(int argc, char** argv)
{
	glCreateWindow("GL Software", 0, 30, Width, Height);
	glCreateFramebuffer(Width, Height, &Framebuffer);

	GameInit(&Framebuffer);

	while (1)
	{
		if (!glPollEvent())
			break;

		glClearFramebuffer(66, &Framebuffer);

		GameRender(&Framebuffer);

	//	glDrawTexture(200, 200, 60, 600, 60, TextureSize, 4, Textures[2], &Framebuffer);
		glDrawFramebuffer(&Framebuffer);

		CalcFps("Doom but bad");
		GetDeltaTime();
	}

	free(ThreadData);
	free(Threads);
	glDestroyFramebuffer(&Framebuffer);
	glDestroy();

	return 0;
}