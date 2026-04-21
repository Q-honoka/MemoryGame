#pragma once

/*
	名前：InputManager.h
	役割：ユーザーからの入力情報の管理
*/
// マウスの状態
enum class MouseState
{
	PRESS,		// 押した瞬間
	HOLD,		// 長押し
	RELEASE		// 離した瞬間
};

class InputManager
{
public:
	InputManager();
	~InputManager() = default;
	void Update();	// 更新処理
	bool GetMouseState(MouseState state) const;	// マウスの指定の入力情報を取得

private:
	struct MouseInput
	{
		bool press;		// 押された瞬間trueになる
		bool hold;		// 押されている間trueになる
		bool release;	// 離した瞬間trueになる
	};
	MouseInput mouseData;	// マウスの入力状態
	char inputKeyBuff[256];	// すべてのキーの押下状態

	void InputMouseData();	// マウスの入力情報の更新
};