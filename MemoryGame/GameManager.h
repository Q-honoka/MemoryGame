#pragma once
#include "CardManager.h"
#include "InputManager.h"

/*
	名前：GameManager.h
	役割：ゲーム全体の進行管理
*/

class GameManager
{
public:
	GameManager();		// コンストラクタ
	~GameManager() = default;		// デストラクタ
	void Update();		// 更新
	void Draw() const;	// 描画

	// コピーとムーブを禁止する
	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;

	GameManager(GameManager&& other) = delete;
	GameManager&& operator=(GameManager&& other) = delete;

private:
	// プレイヤーのデータ
	struct PlayerData
	{
		int missCount;	// ミス回数
		int streak;		// 連続成功回数
	};

	// ゲーム内の状態
	enum State
	{
		START,				// 開始
		FIRST_SELECT,		// １枚目を選択中
		SECOND_SELECT,		// ２枚目を選択中
		CHECK_CARD,			// カードがとれるかチェック
		CHECK_GAMEEND,		// ゲーム終了か調べる
		END,				// ゲーム終了
		NONE,				// なにもなし
	};

	void ChangeGameState();			// 状態を変える
	void SetNextState(State next);	// 次の状態をセットする
	bool SelectCard();				// カードの選択

	CardManager cardManager;		// カードの管理クラス
	InputManager inputManager;		// 入力の管理クラス
	State currentState;		// 現在の状態
	State nextstate;		// 次の状態
	PlayerData playerData;	// プレイデータ
	int elapsedTime;		// 経過時間
	bool isPlaySound;		// 音を再生したかどうか（true：再生した, false：再生していない）
	bool isMatch;			// 前ターンに成功したか（true：成功した, false：失敗した）
	int currentStreak;		// 現在の連続成功回数
};