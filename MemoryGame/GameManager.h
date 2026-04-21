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
	void ChangeGameState();			// 状態を変える
	CardManager cardManager;		// カードの管理クラス
	InputManager inputManager;		// 入力の管理クラス
	Config::Game::State state;		// ゲームの現在の状態
	Config::Game::State nextState;	// ゲームの次の状態
};