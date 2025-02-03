#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;

#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

float mapHeight = 16;
float mapWidth = 16;

float fDepth = 16.0f;

float fFOV = 3.1459 / 4; //is the theta for fov of player
int main() {
	//creating Screen buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#....#####.....#";
	map += L"#....#####.....#";
	map += L"#....#####.#...#";
	map += L"#..........#...#";
	map += L"#..............#";
	map += L"#..#.....###...#";
	map += L"#..#.......#...#";
	map += L"#..#.......#...#";
	map += L"#..#.......#...#";
	map += L"#..#...........#";
	map += L"#..#...........#";
	map += L"#..........##..#";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (true) //how far does a ray travel before it hits a surface
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count(); //elapsed time basically does the work of time.deltatime

		//controls
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			fPlayerA -= (3.0f) * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			fPlayerA += (3.0f) * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * mapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime; //checks if it hits a wall and if it does doesn't allow further movement
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * mapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime; //checks if it hits a wall and if it does doesn't allow further movement
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

		for (int x = 0; x < nScreenWidth; x++)
		{//potters algo ig.  for each column calculates the projected ray angle into world space
			float fRayAngle = fPlayerA + (((float)x / (float)nScreenWidth) - 0.5f) * fFOV;

			float fDistanceToWall = 0.05f;
			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle); //unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.1f;
				
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
				
				//testing if ray is out of bounds
				if (nTestX < 0 || nTestX >= mapWidth || nTestY < 0 || nTestY >= mapHeight) {
					bHitWall = true;  //setting distance to max depth
					fDistanceToWall = fDepth;
				}
				else {
					//ray inbounds tests to see if ray hits a wall block
					if (map[nTestY * mapWidth + nTestX] == '#') {
						bHitWall = true;

						vector<pair<float, float>> p; //distance, dot
						//stores the closest angles from each cell to player so we 
						//can draw boundaries to increase immersion

						for (int tx = 0; tx < 3; tx++)
						{
							for (int ty = 0; ty < 3; ty++)
							{
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * (vx / d)) + (fEyeY * (vy / d));
								p.push_back(make_pair(d, dot));
							}
						}
						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right)
							{return left.first < right.first; });

						float fBound = 0.01;
						bBoundary = false;
						if (acos(p[0].second) < fBound || acos(p[1].second) < fBound) {
							bBoundary = true;
						}

					}

					}
			}
			//calculate dist ceiling to floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' '; //to create the illusion of depth for walls that are far away

			if (fDistanceToWall <= fDepth / 4.0f) //very close
			{
				nShade = 0x2588;
			}
			else if (fDistanceToWall < fDepth / 3.0f)
			{
				nShade = 0x2593;
			}
			else if (fDistanceToWall < fDepth / 2.0f) {
				nShade = 0x2592;
			}
			else if (fDistanceToWall < fDepth)
			{
				nShade = 0x2591; //too far away
			}
			if(bBoundary && fDistanceToWall < fDepth)
			{
				nShade = ' ';
			}

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling) {
					screen[y * nScreenWidth + x] = ' '; //ceiling
				}
				else if(y > nCeiling && y <= nFloor) {
					screen[y * nScreenWidth + x] = nShade; //wall
				}
				//shading floor based on distance to increase perception
				else {
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)
					{
						nShade = '#';
					}
					else if (b < 0.5)
					{
						nShade = 'X';
					}
					else if (b < 0.75)
					{
						nShade = '.';
					}
					else if (b < 0.9)
					{
						nShade = '-';
					}
					else {
						nShade = ' ';
					}
					screen[y * nScreenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);

		// Display Map
		for (int nx = 0; nx < mapWidth; nx++)
			for (int ny = 0; ny < mapWidth; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * mapWidth + nx];
			}
		screen[((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY] = 'P';

		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

	}

	return 0; 
}