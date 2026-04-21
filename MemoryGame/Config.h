#pragma once

namespace Config
{
	namespace Window
	{
		const int width = 1920;
		const int height = 1080;
		const int colorBitDepth = 32;
	}

	namespace Card
	{
		const int allCardsNum = 52;	// カードの最大枚数
		// カードのデータ
		struct CardData
		{
			int handle;		// 画像ハンドル
			int number;		// 数字
			bool isRed;		// 赤かどうか（true：赤色, false：黒色）
			bool isBack;	// 裏かどうか（true：裏, false：表）
		};
	}
}