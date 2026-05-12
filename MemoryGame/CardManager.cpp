#include "CardManager.h"
#include "DxLib.h"
#include "Config.h"
#include <sstream>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="filePath">string：ファイルパス</param>
CardManager::CardManager(std::string filePath) :
	card(),
	renderCard(),
	id(0),
	selectCardID(-1),
	selectCount(0)
{
	Initialize(filePath);
}

/// <summary>
/// デストラクタ
/// </summary>
CardManager::~CardManager()
{
	Finalize();
}

/// <summary>
/// カードのデータ読み込みと初期化
/// </summary>
/// <param name="filePath">string：カードデータのCSVファイルパス</param>
void CardManager::Initialize(std::string filePath)
{
	// カードのデータ読み込み
	if (!LoadCardsFromCSV(filePath))
	{
		printfDx("%sの読み込みに失敗しました\n", filePath);
	}

	// カードの裏の画像ハンドル取得
	renderCard.backCardHandle = LoadGraph("image/card/torannpu-illust54.png");
	ShuffleCards();
}

/// <summary>
/// カードの更新
/// </summary>
void CardManager::Update()
{

}

/// <summary>
/// すべてのカードの描画
/// </summary>
void CardManager::Draw() const
{
	// カードを並べて表示
	int row = renderCard.row;	// 行数
	int col = renderCard.col;	// 列数
	int hdl = -1;		// 仮画像ハンドル
	int width = renderCard.width;		// 幅
	int height = renderCard.height;		// 高さ

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			// 表示する画像ハンドルを取得
			if (card[i * col + j].isBack)
			{
				hdl = renderCard.backCardHandle;
			}
			else
			{
				hdl = renderCard.handle[card[i * col + j].id];
			}

			// 1枚のカードを表示
			DrawExtendGraph(
				width * j,
				height * i,
				width * j + width,
				height * i + height,
				hdl, TRUE);
		}
	}
}

/// <summary>
/// カードの画像ハンドルの消去と終了処理
/// </summary>
void CardManager::Finalize()
{
	// 画像ハンドルの消去
	for (int i = 0; i < 52; i++)
	{
		DeleteGraph(renderCard.handle[i]);
	}
	DeleteGraph(renderCard.backCardHandle);
}

/// <summary>
/// カードを選択する
/// </summary>
/// <param name="x">クリック位置X</param>
/// <param name="y">クリック位置Y</param>
/// <returns>
/// 選択できたか
/// <para>true：成功</para>
/// <para>false：失敗</para>
/// </returns>
bool CardManager::IsCardSelect(int posX, int posY)
{
	// 選択回数が規定数以上だったら、falseを返す
	if (2 <= selectCount) return false;

	int width = renderCard.width;
	int height = renderCard.height;
	int numX = -1, numY = -1;
	// どの列に入るか調べる
	for (int col = 0; col < renderCard.col; col++)
	{
		if (width * col <= posX && posX <= width * col + width)
		{
			numX = col;
			break;
		}
	}

	// どの行に入るか調べる
	for (int row = 0; row < renderCard.row; row++)
	{
		if (height * row <= posY && posY <= height * row + height)
		{
			numY = row;
			break;
		}
	}

	// 一度選択済みの場合は、重複を確認してから保存する
	int num = numY * renderCard.col + numX;
	if (selectCount == 1)
	{
		if (selectCardID[0] == num) return false;
	}

	selectCardID[selectCount] = num;
	selectCount++;
	// カードを表にする
	FlipCard(num);

	return true;
}

/// <summary>
/// 選択した２枚のカードが同じ数字と色か調べる
/// </summary>
/// <returns></returns>
bool CardManager::CheckMatch()
{
	bool isMatch = true;	// そろったかどうか
	// ２枚が異なる色なら、そろっていない
	if (card[selectCardID[0]].isRed != card[selectCardID[1]].isRed)
	{
		isMatch = false;
	}
	// ２枚が異なる数字なら、そろっていない
	if (card[selectCardID[0]].number != card[selectCardID[1]].number)
	{
		isMatch = false;
	}

	// 揃っていないなら、カードを裏返す
	if (isMatch == false)
	{
		FlipCard(selectCardID[0]);
		FlipCard(selectCardID[1]);
	}

	// 選択状態を初期化する
	ResetSelect();

	return isMatch;
}

/// <summary>
/// すべてのカードが表向きか返す
/// </summary>
/// <returns>
/// true：表向き, false：裏向きカードが存在する
/// </returns>
bool CardManager::IsFrontAllCard()
{
	for (int i = 0; i < 52; i++)
	{
		// 裏向きが１枚でもあればfalseを返す
		if (card[i].isBack) return false;
	}

	return true;
}

/// <summary>
/// 選択をリセットする
/// </summary>
void CardManager::ResetSelect()
{
	// 選択したカード番号を初期化
	for (int i = 0; i < 2; i++)
	{
		selectCardID[i] = -1;
	}

	// 選択回数を初期化
	selectCount = 0;
}

/// <summary>
/// カードのシャッフル
/// （フィッシャーイーツ法を使用しています）
/// </summary>
void CardManager::ShuffleCards()
{
	// 配列の末尾から先頭にかけてシャッフルする
	for (int i = sizeof(card) / sizeof(CardData) - 1; i > 0; i--)
	{
		// 乱数をiから求める
		int random = GetRand(i);

		// ランダムな要素と入れ替える
		CardData tep = card[i];
		card[i] = card[random];
		card[random] = tep;
	}
}

/// <summary>
/// カードの表裏を変える
/// </summary>
/// <param name="num">ひっくり返すカードのID</param>
void CardManager::FlipCard(int num)
{
	card[num].isBack = !card[num].isBack;
}

/// <summary>
/// 与えられたCSVファイルのパスから指定の枚数のカードデータを読み込む関数
/// </summary>
/// <param name="filePath">string[ファイルパス]</param>
/// <param name="cards">CardData[カード構造体]</param>
/// <param name="cardCount">int[データを入れる数]</param>
/// <returns>bool[ファイルの読み込みに成功したか]
/// <para>true：成功</para>
/// <para>false：失敗</para>
/// </returns>
bool CardManager::LoadCardsFromCSV(std::string filePath)
{
	char readBuf[256];	// 読み込んだファイルを保存する
	int count = 0;		// 読み込み回数

	// ファイルを開いてハンドルを作成
	int fileHandle = FileRead_open(filePath.c_str());
	// 開けなかったらfalseを返す
	if (fileHandle == 0) return false;

	// ヘッダは捨てる
	FileRead_gets(readBuf, 256, fileHandle);

	// 指定回数データを読み込む
	while (FileRead_eof(fileHandle) == 0 && count < Config::Card::allCardsNum + 1)
	{
		// １行読み込む
		FileRead_gets(readBuf, 256, fileHandle);

		// 文字列の末尾にある改行を消去する
		int len = (int)strlen(readBuf);
		// 改行もしくはキャリッジリターンがあれば、末尾を終端文字にする
		while (len > 0 && (readBuf[len - 1] == '\n' || readBuf[len - 1] == '\r'))
		{
			readBuf[len - 1] = '\0';
			len--;
		}

		std::stringstream ss(readBuf);	// 読み込んだ１行を一時保存する配列
		std::string line;		// カンマで区切った文字列を一時保存する
		std::string columns[4];	// 各要素を入れる

		// 各要素の代入
		int i = 0;
		while (std::getline(ss, line, ',') && i < 4)
		{
			columns[i] = line;
			i++;
		}

		// カードデータ格納
		card[count] = SaveCardData(std::stoi(columns[1]), columns[2], true, columns[3]);
		count++;
	}

	FileRead_close(fileHandle);		// ファイルを閉じる

	return true;
}

/// <summary>
/// 受け取ったデータでカード構造体を作成して返す
/// </summary>
/// <param name="num">int[カードの番号]</param>
/// <param name="color">string[色名]</param>
/// <param name="isBack">bool[裏向きか]</param>
/// <param name="filePath">string[画像ファイルパス]</param>
/// <returns>CardData[データ入りのカード構造体]</returns>
CardManager::CardData& CardManager::SaveCardData(int num, std::string color, bool isBack, std::string filePath)
{
	// カードの情報を代入
	CardData data = {};
	data.id = id;
	data.number = num;
	data.isRed = color == "red";
	data.isBack = isBack;

	// ファイルパスの組み立て
	std::string path = "image/card/" + filePath;
	renderCard.handle[data.id] = LoadGraph(path.c_str());

	id++;

	return data;
}