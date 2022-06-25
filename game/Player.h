void PlayerWallCollision(float DX, float DY, glFramebuffer* Framebuffer)
{
	float Scale = 14.0;

	if (MapGetField(PlayerPos.x + DX * Scale, PlayerPos.y, Framebuffer) == ' ')
		PlayerPos.x += DX;
	if (MapGetField(PlayerPos.x , PlayerPos.y + DY * Scale, Framebuffer) == ' ')
		PlayerPos.y += DY;
}

void PlayerUpdate(glFramebuffer* Framebuffer)
{
	float SinA = sinf(PlayerAngle);
	float CosA = cosf(PlayerAngle);

	float DX = 0.0;
	float DY = 0.0;

	float Speed = PlayerSpeed * DeltaTime;
	float SpeedSin = Speed * SinA;
	float SpeedCos = Speed * CosA;

	Uint8* Key = SDL_GetKeyboardState(NULL);

	if (Key[SDL_SCANCODE_W])
	{
		DX += SpeedCos;
		DY += SpeedSin;
	}
	if (Key[SDL_SCANCODE_S])
	{
		DX -= SpeedCos;
		DY -= SpeedSin;
	}
	if (Key[SDL_SCANCODE_A])
	{
		DX += SpeedSin;
		DY -= SpeedCos;
	}
	if (Key[SDL_SCANCODE_D])
	{
		DX -= SpeedSin;
		DY += SpeedCos;
	}

	PlayerWallCollision(DX, DY, Framebuffer);

	if (Key[SDL_SCANCODE_LEFT])
		PlayerAngle -= PlayerRotSpeed * DeltaTime;
	if (Key[SDL_SCANCODE_RIGHT])
		PlayerAngle += PlayerRotSpeed * DeltaTime;
	
	PlayerAngle = fmod(PlayerAngle, 6.283185307179586);
}

void PlayerDraw(glFramebuffer* Framebuffer)
{
	int Width = Framebuffer->Width * 0.25;
	int Height = Framebuffer->Height * 0.25;
	OffsetX = Width / MapWidth;
	OffsetY = Height / MapHeight;

	glDrawLine((PlayerPos.x * OffsetX), (PlayerPos.y * OffsetY),
				(PlayerPos.x * OffsetX + (Width * 0.1) * cosf(PlayerAngle)),
				(PlayerPos.y * OffsetY + (Width * 0.1) * sinf(PlayerAngle)), 22, 200, 99, Framebuffer);
	glDrawRect((PlayerPos.x * OffsetX) - 2.5, (PlayerPos.y * OffsetY) - 2.5, (PlayerPos.x * OffsetX) + 2.5, (PlayerPos.y * OffsetY) + 2.5, 255, 255, 22, Framebuffer);

	OffsetX = Framebuffer->Width / MapWidth;
	OffsetY = Framebuffer->Height / MapHeight;
}