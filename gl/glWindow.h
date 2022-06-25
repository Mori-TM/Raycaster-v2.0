struct
{
	int Width;
	int Height;

	SDL_Window* Window;
	SDL_Surface* Surface;
	SDL_Renderer* Renderer;
	SDL_Texture* Swapchain;
	SDL_Event Event;
} glInfo;

void glCreateWindow(const char* Title, int x, int y, int Width, int Height)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	glInfo.Width = Width;
	glInfo.Height = Height;

	glInfo.Window = SDL_CreateWindow(Title, x, y, Width, Height, SDL_WINDOW_RESIZABLE);
	glInfo.Surface = SDL_GetWindowSurface(glInfo.Window);
	glInfo.Renderer = SDL_CreateRenderer(glInfo.Window, -1, SDL_RENDERER_SOFTWARE);
	glInfo.Swapchain = SDL_CreateTexture(glInfo.Renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, Width, Height);
}

void glDestroy()
{
	SDL_DestroyTexture(glInfo.Swapchain);
	SDL_DestroyRenderer(glInfo.Renderer);
	SDL_DestroyWindow(glInfo.Window);
	SDL_Quit();
}

extern inline void glSetWindowTitle(const char* Title)
{
	SDL_SetWindowTitle(glInfo.Window, Title);
}

int glPollEvent()
{
	while (SDL_PollEvent(&glInfo.Event))
	{
		if (glInfo.Event.type == SDL_QUIT)
			return 0;
	}

	return 1;
}

typedef struct
{
	int Width;
	int Height;
	int Size;
	unsigned char* Data;
} glFramebuffer;

void glCreateFramebuffer(int Width, int Height, glFramebuffer* Framebuffer)
{
	Framebuffer->Width = Width;
	Framebuffer->Height = Height;
	Framebuffer->Size = Width * Height * 3;
	Framebuffer->Data = (unsigned char*)malloc(Framebuffer->Size);
	memset(Framebuffer->Data, 255, Framebuffer->Size);
}

void glClearFramebuffer(int Shade, glFramebuffer* Framebuffer)
{
	memset(Framebuffer->Data, Shade, Framebuffer->Size);
}

extern inline void glDrawPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, glFramebuffer* Framebuffer)
{
	int Index = (x + y * Framebuffer->Width) * 3;
	if (Index >= 0 && Index < Framebuffer->Size)
	{
		Framebuffer->Data[Index] = r;
		Framebuffer->Data[Index + 1] = g;
		Framebuffer->Data[Index + 2] = b;
	}	
}

extern inline void glDrawRect(int MinX, int MinY, int MaxX, int MaxY, unsigned char r, unsigned char g, unsigned char b, glFramebuffer* Framebuffer)
{
	for (int x = MinX; x < MaxX; x++)
	{
		for (int y = MinY; y < MaxY; y++)
		{
			glDrawPixel(x, y, r, g, b, Framebuffer);
		}
	}
}

extern inline void glDrawQuad(int PosX, int PosY, int ScaleX, int ScaleY, unsigned char r, unsigned char g, unsigned char b, glFramebuffer* Framebuffer)
{
	for (int x = PosX; x < PosX + ScaleX; x++)
	{
		for (int y = PosY; y < PosY + ScaleY; y++)
		{
			glDrawPixel(x, y, r, g, b, Framebuffer);
		}
	}
}

extern inline void glDrawPoint(int Scale, int PosX, int PosY, unsigned char r, unsigned char g, unsigned char b, glFramebuffer* Framebuffer)
{
	float Half = Scale * 0.5;

	for (int x = PosX - Half; x < PosX + Half; x++)
	{
		for (int y = PosY - Half; y < PosY + Half; y++)
		{
			glDrawPixel(x, y, r, g, b, Framebuffer);
		}
	}
}

void glDrawSubTexture(int PosX, int PosY, int ScaleX, int ScaleY, int StartX, int StartY, int EndX, int EndY, int Width, int Height, int Channels, unsigned char* Data, float Brightness, glFramebuffer* Framebuffer)
{
	float ImageX = StartX;
	float ImageY = StartY;

	float ImageW = (float)EndX / (float)ScaleX;
	float ImageH = (float)EndY / (float)ScaleY;

	for (int x = PosX; x < PosX + ScaleX; x++)
	{
		for (int y = PosY; y < PosY + ScaleY; y++)
		{
			int Index = ((int)ImageX + (int)ImageY * Width) * Channels;

			unsigned char r = Data[Index];
			unsigned char g = Data[Index + 1];
			unsigned char b = Data[Index + 2];

			ImageY += ImageH;
		
			glDrawPixel(x, y, r * Brightness, g * Brightness, b * Brightness, Framebuffer);
		}

		ImageY = StartY;
		ImageX += ImageW;
	}
}

extern inline void glDrawTexture(int PosX, int PosY, int ScaleX, int ScaleY, int Width, int Height, int Channels, unsigned char* Data, float Brightness, glFramebuffer* Framebuffer)
{
	glDrawSubTexture(PosX, PosY, ScaleX, ScaleY, 0, 0, Width, Height, Width, Height, Channels, Data, Brightness, Framebuffer);
}

void glDrawLine(int x1, int y1, int x2, int y2, unsigned char r, unsigned char g, unsigned char b, glFramebuffer* Framebuffer)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		glDrawPixel(x, y, r, g, b, Framebuffer);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			glDrawPixel(x, y, r, g, b, Framebuffer);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		glDrawPixel(x, y, r, g, b, Framebuffer);
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			glDrawPixel(x, y, r, g, b, Framebuffer);
		}
	}
	/*
	int dx, dy, p, x, y;

	dx = x1 - x0;
	dy = y1 - y0;

	x = x0;
	y = y0;

	p = 2 * dy - dx;

	while (x < x1)
	{
		if (p >= 0)
		{
			glDrawPixel(x, y, r, g, b, Framebuffer);
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			glDrawPixel(x, y, r, g, b, Framebuffer);
			p = p + 2 * dy;
		}
		x = x + 1;
	}
	*/
}

void glDrawFramebuffer(glFramebuffer* Framebuffer)
{
	void* Pixels;
	int Pitch;

	SDL_LockTexture(glInfo.Swapchain, NULL, &Pixels, &Pitch);
	memcpy(Pixels, Framebuffer->Data, Pitch * Framebuffer->Height);
	SDL_UnlockTexture(glInfo.Swapchain);

	SDL_RenderClear(glInfo.Renderer);
	SDL_RenderCopy(glInfo.Renderer, glInfo.Swapchain, NULL, NULL);
	SDL_RenderPresent(glInfo.Renderer);
}

void glDestroyFramebuffer(glFramebuffer* Framebuffer)
{
	free(Framebuffer->Data);
}