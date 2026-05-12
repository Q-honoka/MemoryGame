#pragma once
#include "Config.h"
#include <string>

/*
	名前：CardManager.h
	役割：すべてのカードの管理と制御
*/

class CardManager
{
public:
	CardManager(std::string filePath);		// コンストラクタ
	~CardManager();		// デストラクタ
	void Initialize(std::string filePath);	// カードの読み込みと初期化を行う
	void Update();		// カードの更新を行う
	void Draw() const;	// カードの描画を行う
	void Finalize();	// カードの終了処理を行う
	bool IsCardSelect(int posX, int posY);	// カードを選択する（true：成功, false：失敗）
	void CheckMatch();		// 選択した２枚のカードが同じ数字と色か調べる
	void ResetSelect();		// 選択を初期化する

	// コピーとムーブを禁止する
	CardManager(const CardManager& other) = delete;
	CardManager& operator=(const CardManager& other) = delete;

	CardManager(CardManager&& other) = delete;
	CardManager&& operator=(CardManager&& other) = delete;

private:
	// カード１枚のデータ
	struct CardData
	{
		int id;			// 生成ID
		int number;		// 1～13の数字を表す
		bool isRed;		// 赤色かどうか（true：赤色, false：黒色）
		bool isBack;	// 裏向きかどうか（true：裏, false：表）
	};

	// カードの描画用のデータ
	struct CardRenderData
	{
		int handle[52];		// 52枚の画像ハンドル
		int backCardHandle;		// 裏向きカードの画像ハンドル
		const int row = 4;		// 行数
		const int col = 13;		// 列数
		const int width = 140;	// カードの幅
		const int height = 200;	// カードの高さ
	};

	void ShuffleCards();	// カードをシャッフルする
	void FlipCard(int num);	// カードの表裏を変える
	bool LoadCardsFromCSV(std::string filePath);	// CSVファイルの読み込み
	CardData& SaveCardData(int num, std::string color, bool isBack, std::string filePath);	// カード構造体を作成して返す

	CardData card[52];		// すべてのカードデータ
	CardRenderData renderCard;		// カードの描画用データ
	int id;		// カードの生成回数
	int selectCardID[2];	// 選んだカードの番号
	int selectCount;		// カードの選択回数
};