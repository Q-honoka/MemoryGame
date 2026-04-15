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
	void Draw() const;		// カードの描画を行う
	void Finalize();	// カードの終了処理を行う

	// コピーとムーブを禁止する
	CardManager(const CardManager& other) = delete;
	CardManager& operator=(const CardManager& other) = delete;

	CardManager(CardManager&& other) = delete;
	CardManager&& operator=(CardManager&& other) = delete;

private:
	bool LoadCardsFromCSV(std::string filePath);
	Config::Card::CardData& MakeCardData(int num, std::string color, bool isBack, std::string filePath);
	Config::Card::CardData cards[Config::Card::allCardsNum];
	int backCardHandle;
};