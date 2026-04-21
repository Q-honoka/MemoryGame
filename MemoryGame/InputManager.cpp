#include "InputManager.h"
#include "DxLib.h"

/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager() :
	mouseData({ false}),
	inputKeyBuff(0)
{
}

/// <summary>
/// 入力データの更新
/// </summary>
void InputManager::Update()
{
	InputMouseData();
}

/// <summary>
/// マウスの指定の入力情報を取得
/// </summary>
/// <param name="state">オンかどうか知りたいマウスの状態</param>
/// <returns>bool：指定の状態がオンか
/// <para>true：オン</para>
/// <para>false：オフ</para>
/// </returns>
bool InputManager::GetMouseState(MouseState state) const
{
	switch (state)
	{
	case MouseState::PRESS:
		return mouseData.press;
	case MouseState::HOLD:
		return mouseData.hold;
	case MouseState::RELEASE:
		return mouseData.release;
	}

	return false;
}

/// <summary>
/// マウスの入力情報の更新
/// </summary>
void InputManager::InputMouseData()
{
	// 現在の入力値の取得
	int input = GetMouseInput();
	MouseInput nextInput = {false};

	// 左ボタンが押された場合の処理
	// 押された瞬間か長押しかを一時保存する
	if ((input & MOUSE_INPUT_LEFT) != 0)
	{
		nextInput.press = !mouseData.press && !mouseData.hold;
		nextInput.hold = mouseData.press || mouseData.hold;
	}
	// 左ボタン以外が押された場合の処理
	// 離された瞬間かずっと押していないかを一時保存する
	else
	{
		nextInput.release = mouseData.hold;
	}

	// 値の更新
	mouseData.press = nextInput.press;
	mouseData.hold = nextInput.hold;
	mouseData.release = nextInput.release;
}