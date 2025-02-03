// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<Windows.h>
using namespace std;

wstring tetrominoAssets[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr; //storing grid for tetris
int nScreenWidth = 80;
int nScreenHeight = 30;

int Rotate(int px, int py, int r) {
	switch (r % 4)
	{
	case 0:
		return py * 4 + px; //0 degree rotation
	case 1:
		return 12 + py - (px * 4); //90 deg
	case 3: 
		return 15 - (py * 4) - px; //180 deg
	case 4:
		return 3 - py + (px * 4); //270 deg
	}
	return 0;
}

int main()
{
	//asset creatiin
	tetrominoAssets[0].append(L"..X.");
	tetrominoAssets[0].append(L"..X.");
	tetrominoAssets[0].append(L"XXX.");
	tetrominoAssets[0].append(L"....");

	tetrominoAssets[1].append(L"..X.");
	tetrominoAssets[1].append(L".XX.");
	tetrominoAssets[1].append(L".X..");
	tetrominoAssets[1].append(L"....");

	tetrominoAssets[2].append(L"....");
	tetrominoAssets[2].append(L".XX.");
	tetrominoAssets[2].append(L".XX.");
	tetrominoAssets[2].append(L"....");

	tetrominoAssets[3].append(L"....");
	tetrominoAssets[3].append(L"....");
	tetrominoAssets[3].append(L"..XX");
	tetrominoAssets[3].append(L"..X.");

	tetrominoAssets[4].append(L"....");
	tetrominoAssets[4].append(L"...X");
	tetrominoAssets[4].append(L"..XX");
	tetrominoAssets[4].append(L"...X");

	tetrominoAssets[5].append(L"....");
	tetrominoAssets[5].append(L"..X.");
	tetrominoAssets[5].append(L"..X.");
	tetrominoAssets[5].append(L"..XX");

	tetrominoAssets[6].append(L"XXXX");
	tetrominoAssets[6].append(L"....");
	tetrominoAssets[6].append(L"....");
	tetrominoAssets[6].append(L"....");

	pField = new unsigned char[nFieldWidth * nFieldHeight]; //creating play field buffer
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			pField[y * nFieldWidth + x] = (x == 0 || nFieldWidth - 1 || y == nFieldWidth - 1) ? 9 : 0;
		}
	}

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
	{
		screen[i] = L' ';
	}
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool bGameOver = false;

	while (!bGameOver)
	{
		//Draw Field
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
			}
		}

		//Display frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
