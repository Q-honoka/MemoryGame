#pragma once

namespace Config
{
	// ウィンドウ設定
	namespace Window
	{
		const int width = 1920;			// 幅
		const int height = 1080;		// 高さ
		const int colorBitDepth = 32;	// カラービット数
	}

	// ゲーム設定
	namespace Game
	{
		// ゲームの状態
		enum State
		{
			GAME_STATE_START,			// スタート
			GAME_STATE_FIRST_SELECT,	// 1枚目選択中
			GAME_STATE_SECOND_SELECT,	// 2枚目選択中
			GAME_STATE_CHECK_CARD,		// カードの比較
			GAME_STATE_END,				// 終了
			GAME_STATE_NONE				// 状態なし
		};
	}

	// カード設定
	namespace Card
	{
		const int allCardsNum = 52;	// カードの最大枚数
	}
}