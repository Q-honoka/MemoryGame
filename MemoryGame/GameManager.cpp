#include "GameManager.h"
#include "DxLib.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager() :
	cardManager(CardManager("CSV/card.csv")),
	inputManager(InputManager()),
	state(Config::Game::State::GAME_STATE_FIRST_SELECT),
	nextState(Config::Game::State::GAME_STATE_NONE)
{

}

/// <summary>
/// 更新処理
/// </summary>
void GameManager::Update()
{
	// 次の状態がセットされていたら変更する
	if (nextState != Config::Game::GAME_STATE_NONE)
	{
		ChangeGameState();
	}

	// プレイヤー入力の更新
	inputManager.Update();

	// カードの更新
	cardManager.Update();

	// カードが2枚選択されたら、チェックする
	if (state == Config::Game::GAME_STATE_CHECK_CARD)
	{
		
	}
	else
	{
		// 左クリックされたら、そのカードを選択する
		if (inputManager.GetMouseState(MouseState::PRESS))
		{
			int x, y;
			GetMousePoint(&x, &y);
			// カードを選択できたら、状態を次に変える
			if (cardManager.IsCardSelect(x, y))
			{
				nextState = (Config::Game::State)((int)state + 1);
			}
		}
	}


}

/// <summary>
/// 描画処理
/// </summary>
void GameManager::Draw() const
{
	cardManager.Draw();
}

/// <summary>
/// ゲーム内の状態を変更する
/// </summary>
void GameManager::ChangeGameState()
{
	// 同じだったら変更しない
	if (state == nextState) return;

	state = nextState;
	nextState = Config::Game::GAME_STATE_NONE;
}