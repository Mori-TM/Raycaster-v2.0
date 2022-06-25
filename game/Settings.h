vec2 PlayerPos = { 1.5, 5.0 };
float PlayerAngle = 0.0;
float PlayerSpeed = 2.0;
float PlayerRotSpeed = 2.0;
float PlayerScale = 10.0;
int OffsetX = 100;
int OffsetY = 100;

float Fov = M_PI / 3;
float HalfFov = 0.0;
float NumRays = 0.0;
float HalfNumRays = 0.0;
float DeltaAngle = 0.0;
int MaxDepth = 20;
float ScreenDist = 0.0;

unsigned char* Textures[24];
const int TextureSize = 256;
const int HalfTextureSize = 128;