#include "CardManager.h"
#include "DxLib.h"
#include <sstream>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="filePath">string：ファイルパス</param>
CardManager::CardManager(std::string filePath) :
	cards(),
	backCardHandle(-1),
	selectCardID(-1),
	selectCount(0),
	showRow(4),
	showCol(13),
	showWidth(Config::Window::width / showCol),
	showHeight(Config::Window::height / showRow)
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
	backCardHandle = LoadGraph("image/card/torannpu-illust54.png");

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
	for (int i = 0; i < showRow; i++)
	{
		for (int j = 0; j < showCol; j++)
		{
			// 裏表のフラグによって表示するハンドルを切り替える
			int hdl = -1;
			if (cards[i * showCol + j].isBack) hdl = backCardHandle;
			else hdl = cards[i * showCol + j].handle;

			// 拡大縮小して表示
			DrawExtendGraph(
				showWidth * j,
				showHeight * i,
				showWidth * j + showWidth,
				showHeight * i + showHeight,
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
	for (Config::Card::CardData& card : cards)
	{
		DeleteGraph(card.handle);
	}

	DeleteGraph(backCardHandle);
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

	int numX = -1, numY = -1;
	// どの列に入るか調べる
	for (int col = 0; col < showCol; col++)
	{
		if (showWidth * col <= posX && posX <= showWidth * col + showWidth) 
		{
			numX = col;
			break;
		}
	}

	// どの行に入るか調べる
	for (int row = 0; row < showRow; row++)
	{
		if (showHeight * row <= posY && posY <= showHeight * row + showHeight)
		{
			numY = row;
			break;
		}
	}

	// 一度選択済みの場合は、重複を確認してから保存する
	int num = numY * showCol + numX;
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
/// カードのシャッフル
/// （フィッシャーイーツ法を使用しています）
/// </summary>
void CardManager::ShuffleCards()
{
	// 配列の末尾から先頭にかけてシャッフルする
	for (int i = sizeof(cards) / sizeof(Config::Card::CardData) - 1; i > 0; i--)
	{
		// 乱数をiより前から求める
		int random = GetRand(i);
		// ランダムな要素と入れ替える
		Config::Card::CardData temp = cards[i];
		cards[i] = cards[random];
		cards[random] = temp;
	}
}

/// <summary>
/// カードの表裏を変える
/// </summary>
/// <param name="num">ひっくり返すカードのID</param>
void CardManager::FlipCard(int num)
{
	cards[num].isBack = !cards[num].isBack;
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
		cards[count] = MakeCardData(std::stoi(columns[1]), columns[2], true, columns[3]);

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
Config::Card::CardData& CardManager::MakeCardData(int num, std::string color, bool isBack, std::string filePath)
{
	Config::Card::CardData card;
	// 構造体に代入
	card.number = num;
	card.isRed = color == "red";
	card.isBack = isBack;

	// ファイルパスの組み立て
	std::string path = "image/card/" + filePath;
	card.handle = LoadGraph(path.c_str());

	return card;
}