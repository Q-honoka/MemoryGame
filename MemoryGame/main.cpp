#include "DxLib.h"
#include "Config.h"
#include <sstream>

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	Config::Card::CardData cards[52];
	char readBuf[256];

	// ファイルを開く
	std::string filePath = "CSV/card.csv";
	int fileHandle = FileRead_open(filePath.c_str());

	if (fileHandle == 0)
	{
		printfDx("エラー\n");
		WaitKey();
		return 0;
	}

	int y = 0;
	while (FileRead_eof(fileHandle) == 0)
	{
		FileRead_gets(readBuf, sizeof(char) * 256, fileHandle);
		std::stringstream ss;
		ss << readBuf;
		std::string line;

		int x = 0;
		while (std::getline(ss, line, ','))
		{
			DrawString(x, y, line.c_str(), GetColor(255, 255, 255));
			
			x += 100;
		}

		y += 20;
		WaitKey();
	}

	WaitKey();				// キー入力待ち

	FileRead_close(fileHandle);		// ファイルを閉じる

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}