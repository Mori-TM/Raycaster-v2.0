char MiniMap[] =
{
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
	'0', ' ', ' ', '4', '1', '1', '1', ' ', '3', ' ', '0', '0', '0', ' ', ' ', '0',
	'0', ' ', ' ', '5', ' ', ' ', '0', ' ', '3', ' ', ' ', ' ', '0', ' ', ' ', '0',
	'0', ' ', ' ', ' ', ' ', ' ', '0', ' ', '3', ' ', ' ', ' ', '0', ' ', ' ', '0',
	'0', ' ', ' ', '2', '2', '2', '2', ' ', '3', ' ', ' ', ' ', ' ', ' ', ' ', '0',
	'0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
	'0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', '0',
	'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',

//	 '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
//	 '0', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
//	 '0', ' ', '0', ' ', ' ', '0', ' ', '0', ' ', ' ', '0', ' ', '0', ' ', ' ', '0',
//	 '0', ' ', '0', ' ', ' ', '0', ' ', '0', ' ', '0', ' ', ' ', '0', ' ', '0', '0',
//	 '0', '0', '0', '0', ' ', '0', ' ', '0', ' ', '0', ' ', ' ', '0', '0', ' ', '0',
//	 '0', ' ', '0', ' ', ' ', '0', ' ', '0', ' ', '0', ' ', ' ', '0', '0', ' ', '0',
//	 '0', ' ', '0', ' ', ' ', ' ', '0', ' ', ' ', ' ', '0', ' ', '0', ' ', '0', '0',
//	 '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0',
//	 '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
	
};

#define MapSize ARRAY_SIZE(MiniMap)
#define MapWidth 16
#define MapHeight (MapSize / MapWidth)

void MapDraw(glFramebuffer* Framebuffer)
{
	int Width = Framebuffer->Width * 0.25;
	int Height = Framebuffer->Height * 0.25;
	glDrawRect(0, 0, Width, Height, 0, 0, 0, Framebuffer);

	char Tile;
	for (int x = 0; x < MapWidth; x++)
	{
		for (int y = 0; y < MapHeight; y++)
		{
			Tile = MiniMap[y * MapWidth + x];
		//	if (Tile != ' ')
		//	{
		//		r = 255;
		//		g = 55;
		//		b = 89;
		//	}
		//	else
		//	{
		//		r = 0;
		//		g = 55;
		//		b = 89;
		//	}

			float MinX = ((float)x / (float)MapWidth) * Width + 2;
			float MinY = ((float)y / (float)MapHeight) * Height + 2;

			float MaxX = ((float)(x + 1) / (float)MapWidth) * Width - 2;
			float MaxY = ((float)(y + 1) / (float)MapHeight) * Height - 2;
		//	glDrawRect(MinX, MinY, MaxX, MaxY, r, g, b, Framebuffer);
		//	glDrawQuad(MinX, MinY, MaxX - MinX, MaxY - MinY, r, g, b, Framebuffer);
			if (Tile == ' ')
				glDrawQuad(MinX, MinY, MaxX - MinX, MaxY - MinY, 0, 55, 89, Framebuffer);
			else
				glDrawTexture(MinX, MinY, MaxX - MinX, MaxY - MinY, TextureSize, TextureSize, 4, Textures[Tile - 48], 1.0, Framebuffer);
		//	MinX = ((float)x / (float)MapWidth) * Width;
		//	MinY = ((float)y / (float)MapHeight) * Height;

		//	MaxX = Width / MapWidth * 0.5;
		//	MaxY = Height / MapHeight * 0.5;
		//
		//	MinX += MaxX - 5;
		//	MinY += MaxY - 5;
		//	MaxX = MinX + 5;
		//	MaxY = MinY + 5;
		//
		//	glDrawRect(MinX, MinY, MaxX, MaxY, 44, 255, 255, Framebuffer);
		}
	}
}

char MapGetField(float PX, float PY, glFramebuffer* Framebuffer)
{
	int x = PX * OffsetX / Framebuffer->Width * MapWidth;
	int y = PY * OffsetY / Framebuffer->Height * MapHeight;

	int Index = y * MapWidth + x;
	if (Index >= MapSize)
		return 0;

	return MiniMap[Index];
}