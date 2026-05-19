#include "GameManager.h"
#include "DxLib.h"
#include "Config.h"

constexpr int WAIT_TIME = 15;		// 待ち時間
constexpr int LINE_SPACE = 100;		// 行間

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager() :
	cardManager(CardManager("CSV/card.csv")),
	inputManager(InputManager()),
	playerData(0),
	currentState(State::START),
	nextstate(State::NONE),
	elapsedTime(0),
	isPlaySound(false),
	isMatch(false),
	currentStreak(0)
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
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			SetNextState(FIRST_SELECT);
		}
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
			elapsedTime = 0;
		}
		break;

	case CHECK_CARD:		// カードがとれるかチェックする
		if (elapsedTime < WAIT_TIME)
		{
			elapsedTime++;
			break;
		}
		// カードがとれたら、ゲーム終了チェックをする
		// そうでなければ、１枚目の選択に戻る
		if (cardManager.CheckMatch())
		{
			SetNextState(CHECK_GAMEEND);
			// 連続成功記録をつける
			if (isMatch)
			{
				currentStreak++;
			}
			else
			{
				currentStreak = 1;
				isMatch = true;
			}
		}
		else
		{
			// 連続成功記録の更新をする
			if (playerData.streak < currentStreak)
			{
				playerData.streak = currentStreak;
			}

			playerData.missCount++;
			// リセット処理
			isMatch = false;
			currentStreak = 0;

			SetNextState(FIRST_SELECT);
		}
		break;

	case CHECK_GAMEEND:		// ゲーム終了かチェックする
		// ゲーム終了なら、ゲーム終了状態に行く
		// そうでなければ、１枚目の選択に戻る
		if (cardManager.IsFrontAllCard())
		{
			// 連続成功記録の更新をする
			if (playerData.streak < currentStreak)
			{
				playerData.streak = currentStreak;
			}
			SetNextState(END);
		}
		else
		{
			SetNextState(FIRST_SELECT);
		}
		break;

	case END:		// ゲーム終了
		// クリア時の音を一度だけ再生する
		if (isPlaySound == false)
		{
			PlaySoundFile("sound/clear.mp3", DX_PLAYTYPE_BACK);
			isPlaySound = true;
		}
		// リトライ処理
		if (CheckHitKey(KEY_INPUT_R))
		{
			SetNextState(START);
		}
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
	switch (currentState)
	{
	case START:		// ゲーム開始
		DrawString(Config::Window::width / 6 * 2, Config::Window::height - LINE_SPACE, "SPACEキーでスタート", GetColor(0, 255, 255));
		break;

	case FIRST_SELECT:	// １枚目を選択中
		break;

	case SECOND_SELECT:		// ２枚目を選択中
		break;

	case CHECK_CARD:		// カードがとれるかチェックする
		break;

	case CHECK_GAMEEND:		// ゲーム終了かチェックする
		break;

	case END:		// ゲーム終了
		DrawString(Config::Window::width / 6 * 2, Config::Window::height - LINE_SPACE * 3, "Rキーでリトライ", GetColor(0, 255, 255));
		DrawFormatString(Config::Window::width / 6 * 2, Config::Window::height - LINE_SPACE * 2, GetColor(0, 255, 255), "ミスした回数：%d", playerData.missCount);
		DrawFormatString(Config::Window::width / 6 * 2, Config::Window::height - LINE_SPACE, GetColor(0, 255, 255), "連続成功回数：%d", playerData.streak);
		break;

	case NONE:
	default:
		break;
	}
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