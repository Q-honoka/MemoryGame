#include "GameManager.h"
#include "DxLib.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager() :
	cardManager(CardManager("CSV/card.csv")),
	inputManager(InputManager()),
	currentState(State::START),
	nextstate(State::NONE)
{

}

/// <summary>
/// 更新処理
/// </summary>
void GameManager::Update()
{
	// 次の状態がセットされていたら変更する
	if (nextstate != State::NONE)
	{
		ChangeGameState();
	}

	// 値の更新
	inputManager.Update();

	// カードの更新
	cardManager.Update();

	// 状態に応じて処理を分岐する
	switch (currentState)
	{
	case START:		// ゲーム開始
		SetNextState(FIRST_SELECT);
		break;

	case FIRST_SELECT:	// １枚目を選択中
		// カードが選択されたら、カードをとれるかのチェックに進む
		if (SelectCard())
		{
			SetNextState(SECOND_SELECT);
		}
		break;

	case SECOND_SELECT:		// ２枚目を選択中
		// カードが選択されたら、カードをとれるかのチェックに進む
		if (SelectCard())
		{
			SetNextState(CHECK_CARD);
		}
		break;

	case CHECK_CARD:		// カードがとれるかチェックする
		// カードがとれたら、ゲーム終了チェックをする
		// そうでなければ、１枚目の選択に戻る
		if (cardManager.CheckMatch())
		{
			SetNextState(CHECK_GAMEEND);
		}
		else
		{
			SetNextState(FIRST_SELECT);
		}
		break;

	case CHECK_GAMEEND:		// ゲーム終了かチェックする
		// ゲーム終了なら、ゲーム終了状態に行く
		// そうでなければ、１枚目の選択に戻る
		if (cardManager.IsFrontAllCard())
		{
			SetNextState(END);
		}
		else
		{
			SetNextState(FIRST_SELECT);
		}
		break;

	case END:		// ゲーム終了
		DrawString(0, 0, "終了", GetColor(255, 255, 255));
		break;

	case NONE:
	default:
		break;
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
	if (currentState == nextstate) return;

	currentState = nextstate;
	nextstate = State::NONE;	// リセットする
}

/// <summary>
/// 次の状態をセットする
/// </summary>
void GameManager::SetNextState(State next)
{
	// 状態の上書き防止のため、すでにセットされていたら処理を終える
	if (nextstate != State::NONE) return;
	nextstate = next;
}

/// <summary>
/// カードの選択処理
/// </summary>
/// <returns>
/// true：選択成功, false：選択失敗</returns>
bool GameManager::SelectCard()
{
	if (inputManager.GetMouseState(MouseState::PRESS))
	{
		int x, y;
		GetMousePoint(&x, &y);
		// カードを選択できたかどうかを返す
		return cardManager.IsCardSelect(x, y);
	}

	return false;
}