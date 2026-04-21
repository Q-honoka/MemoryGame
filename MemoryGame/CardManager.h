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

	// コピーとムーブを禁止する
	CardManager(const CardManager& other) = delete;
	CardManager& operator=(const CardManager& other) = delete;

	CardManager(CardManager&& other) = delete;
	CardManager&& operator=(CardManager&& other) = delete;

private:
	void ShuffleCards();	// カードをシャッフルする
	void FlipCard(int num);	// カードの表裏を変える
	bool LoadCardsFromCSV(std::string filePath);	// CSVファイルの読み込み
	Config::Card::CardData& MakeCardData(int num, std::string color, bool isBack, std::string filePath);	// カード構造体を作成して返す
	
	Config::Card::CardData cards[Config::Card::allCardsNum];	// すべてのカード
	int backCardHandle;		// カードの裏の画像ハンドル
	int selectCardID[2];	// 選んだカードの番号
	int selectCount;		// カードの選択回数
	const int showRow;		// 表示時の行数
	const int showCol;		// 表示時の列数
	const int showWidth;	// 表示時のカードの幅
	const int showHeight;	// 表示時のカードの高さ
};