#pragma once

// カード一枚の情報
struct CardData
{
	int number;		// 1～13の数字
	bool isRed;		// 赤色か（true：赤, false：黒）
	bool isBack;	// 裏向きか（true：裏, false：表）
};