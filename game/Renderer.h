#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#include "ImageLoader.h"
#include "Settings.h"
#include "Map.h"
#include "Player.h"
#include "Raycasting.h"

int ProcessorCount;
SDL_Thread** Threads;
RayData* ThreadData;
int RayThreadLen;

void GameInit(glFramebuffer* Framebuffer)
{
	int Width;
	int Height;
	int ChannelCount;
	Textures[0] = LoadImage("textures/1.bmp", &Width, &Height, &ChannelCount, true);
	Textures[1] = LoadImage("textures/2.bmp", &Width, &Height, &ChannelCount, true);
	Textures[2] = LoadImage("textures/3.bmp", &Width, &Height, &ChannelCount, true);
	Textures[3] = LoadImage("textures/4.bmp", &Width, &Height, &ChannelCount, true);
	Textures[4] = LoadImage("textures/5.bmp", &Width, &Height, &ChannelCount, true);
	Textures[5] = LoadImage("textures/win.bmp", &Width, &Height, &ChannelCount, true);
	Textures[6] = LoadImage("textures/sky.bmp", &Width, &Height, &ChannelCount, true);

	RaycastingInit(Framebuffer);

	ProcessorCount = SDL_GetCPUCount();
	Threads = (SDL_Thread**)malloc(ProcessorCount * sizeof(SDL_Thread*));
	ThreadData = (RayData*)malloc(ProcessorCount * sizeof(RayData));
	RayThreadLen = (NumRays / (float)ProcessorCount);
}

void GameRender(glFramebuffer* Framebuffer)
{
	OffsetX = Framebuffer->Width / MapWidth;
	OffsetY = Framebuffer->Height / MapHeight;
	
	PlayerUpdate(Framebuffer);
	RaycastingUpdate(Framebuffer);

//	RayData RD;
//	RD.Begin = 0.0;
//	RD.End = NumRays;
//	RD.RayAngle = PlayerAngle - HalfFov + 0.0001;
//	RD.Framebuffer = Framebuffer;
//	Raycast(&RD);
	
	float RayAngle = PlayerAngle - HalfFov + 0.0001;

	for (int i = 0; i < ProcessorCount; i++)
	{
		ThreadData[i].Begin = (float)i * RayThreadLen;
		ThreadData[i].End = ((float)i + 1) * RayThreadLen;

		if (i > 0)
			for (float Ray = ThreadData[i - 1].Begin; Ray < ThreadData[i - 1].End; Ray++)
				RayAngle += DeltaAngle;

		ThreadData[i].RayAngle = RayAngle;
		ThreadData[i].Framebuffer = Framebuffer;

		Threads[i] = SDL_CreateThread(Raycast, "ComputeThread", &ThreadData[i]);
	}
	

	for (int i = 0; i < ProcessorCount; i++)
		SDL_WaitThread(Threads[i], NULL);
	
	MapDraw(Framebuffer);
	PlayerDraw(Framebuffer);
}