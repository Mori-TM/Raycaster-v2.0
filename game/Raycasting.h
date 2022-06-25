void RaycastingInit(glFramebuffer* Framebuffer)
{
	HalfFov = Fov * 0.5;
	NumRays = floorf(Framebuffer->Width * 0.5);
	HalfNumRays = floorf(NumRays * 0.5);
	DeltaAngle = Fov / NumRays;
	ScreenDist = (Framebuffer->Width * 0.5) / tanf(HalfFov);
}

void RaycastingUpdate(glFramebuffer* Framebuffer)
{
	RaycastingInit(Framebuffer);

	//Floor
	glDrawRect(0, Framebuffer->Height * 0.5, Framebuffer->Width, Framebuffer->Height, 44, 44, 44, Framebuffer);
	glDrawTexture(0, 0, Framebuffer->Width, Framebuffer->Height * 0.5, 1200, 400, 4, Textures[6], 1.0, Framebuffer);

//	SkyOffset = modf(SkyOffset + 4.5 * PlayerAngle, Framebuffer->Width);
//	glDrawSubTexture(0, 0, Framebuffer->Width, Framebuffer->Height * 0.5, SkyOffset, 400, SkyOffset + Framebuffer->Width, 400, 1200, 400, 4, Textures[6], Framebuffer);


	
}

typedef struct
{
	float Begin; 
	float End;
	float RayAngle;
	glFramebuffer* Framebuffer;
} RayData;

int Raycast(void* Data)
{
	RayData* RD = (RayData*)Data;

	int MapX = PlayerPos.x;
	int MapY = PlayerPos.y;

//	float RayAngle = PlayerAngle - HalfFov + 0.0001;

	for (float Ray = RD->Begin; Ray < RD->End; Ray++)
	{
		float SinA = sinf(RD->RayAngle);
		float CosA = cosf(RD->RayAngle);

		//horizontal
		float HorzY;
		float DY;
		if (SinA > 0.0)
		{
			HorzY = MapY + 1.0;
			DY = 1.0;
		}
		else
		{
			HorzY = MapY - 1e-6;
			DY = -1.0;
		}

		float DepthHorz = (HorzY - PlayerPos.y) / SinA;
		float HorzX = PlayerPos.x + DepthHorz * CosA;

		float DeltaDepth = DY / SinA;
		float DX = DeltaDepth * CosA;

		char HorzTexture = 0;
		for (int i = 0; i < MaxDepth; i++)
		{
			char Tile = MapGetField(HorzX, HorzY, RD->Framebuffer);
			if (Tile != ' ')
			{
				HorzTexture = Tile;
				break;
			}

			HorzX += DX;
			HorzY += DY;
			DepthHorz += DeltaDepth;
		}

		//vertical
		float VertX;
		if (CosA > 0.0)
		{
			VertX = MapX + 1.0;
			DX = 1.0;
		}
		else
		{
			VertX = MapX - 1e-6;
			DX = -1.0;
		}

		float DepthVert = (VertX - PlayerPos.x) / CosA;
		float VertY = PlayerPos.y + DepthVert * SinA;

		DeltaDepth = DX / CosA;
		DY = DeltaDepth * SinA;

		char VertTexture = 0;
		for (int i = 0; i < MaxDepth; i++)
		{
			char Tile = MapGetField(VertX, VertY, RD->Framebuffer);
			if (Tile != ' ')
			{
				VertTexture = Tile;
				break;
			}

			VertX += DX;
			VertY += DY;
			DepthVert += DeltaDepth;
		}

		float Brightness;
		char Texture;
		float Depth;
		float Offset;
		if (DepthVert < DepthHorz)
		{
			Depth = DepthVert;
			Texture = VertTexture;
			VertY = fmod(VertY, 1);
			Offset = CosA > 0.0 ? VertY : (1 - VertY);
			Brightness = 1.0;
		}
		else
		{
			Depth = DepthHorz;
			Texture = HorzTexture;
			HorzX = fmod(HorzX, 1);
			Offset = SinA > 0.0 ? (1 - HorzX) : HorzX;
			Brightness = 0.7;
		}


		//	glDrawLine(PlayerPos.x * Offset, PlayerPos.y * Offset, 
		//				PlayerPos.x * Offset + Offset * Depth * CosA, 
		//				PlayerPos.y * Offset + Offset * Depth * SinA, 255, 255, 255, Framebuffer);

		Depth *= cosf(PlayerAngle - RD->RayAngle);
		float Scale = floorf(RD->Framebuffer->Width / NumRays);
		float ProjHeight = ScreenDist / (Depth + 0.0001);

		if (ProjHeight < RD->Framebuffer->Height)
		{
			glDrawSubTexture(Ray * Scale, RD->Framebuffer->Height * 0.5 - floorf(ProjHeight * 0.5),
				Scale, ProjHeight,
				Offset * (TextureSize - Scale), 0, Scale, TextureSize,
				TextureSize, TextureSize,
				4, Textures[Texture - 48], Brightness, RD->Framebuffer);
		}
		else
		{
			float TextureHeight = TextureSize * RD->Framebuffer->Height / ProjHeight;
			glDrawSubTexture(Ray * Scale, 0,
				Scale, RD->Framebuffer->Height,
				Offset * (TextureSize - Scale), HalfTextureSize - (floorf(TextureHeight * 0.5)), Scale, TextureHeight,
				TextureSize, TextureSize,
				4, Textures[Texture - 48], Brightness, RD->Framebuffer);
		}

		RD->RayAngle += DeltaAngle;
	}

	return 1;
}