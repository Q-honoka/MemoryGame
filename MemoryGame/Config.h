#pragma once

namespace Config
{
	namespace Card
	{
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