#ifndef _BIHIN_MODEL_H_
#define _BIHIN_MODEL_H_

#include <vector>
#include <map>
#include <set>
#include <string>
using namespace std;

/*!
 * \brief BihinModelクラス。データ部分はここですべて処理する（ファイルの書き込み、読み込みも）
 * \author OKA
 */
class BihinModel {
	public:
		//! 静的メンバ定数 "bihinName"
		static const char *KEY_BIHIN_NAME;
		//! 静的メンバ定数 "size"
		static const char *KEY_SIZE;
		//! 静的メンバ定数 "count"
		static const char *KEY_COUNT;
		//! 静的メンバ定数 "cleaning"
		static const char *KEY_CLEANING;
		//! 静的メンバ定数 "cleaned"
		static const char *KEY_CLEANED;
		//! 静的メンバ定数 "whenCleaned"
		static const char *KEY_WHEN_CLEANED;
		//! 静的メンバ定数 "whoCleaned"
		static const char *KEY_WHO_CLEANED;
		//! 静的メンバ定数 "allCount"
		static const char *KEY_ALL_COUNT;
		//! 静的メンバ定数 "storedPlace"
		static const char *KEY_STORED_PLACE;
	
	public:
		//! デフォルトコンストラクタ
		BihinModel();
		//! コンストラクタ
		//! filenameのセットのみ行う。実際のデータの読み込みなどは行わない
		BihinModel(string filename);
		
		//! デストラクタ
		//! BihinModelはデータファイルを読み込む度にnewされるので、メンバ変数をdeleteする必要がある
		~BihinModel() {
			delete columnsSequence;
			delete bihinNames;
			for (map<string, vector<string> *>::iterator itr = bihinsSizes->begin(); itr != bihinsSizes->end(); ++itr) {
				delete itr->second;
			}
			delete bihinsSizes;
			delete counts;
			delete cleanings;
			delete cleaneds;
			delete whenCleaneds;
			delete whoCleaneds;
			delete allCounts;
			delete storedPlaces;
		};
	
	public:
		//! データファイルを読み込む
		//! \return 正しく読めたらtrueを返す
		bool read();
		
		//! データファイル上書き保存
		//! \return 正しく書き込めたらtrueを返す
		bool save() const;
		
		//! データファイル新規保存
		//! 実体は、filenameにnewfilenameをセットしてsave()を呼び出しているだけ
		//! \return 正しく書き込めたらtrueを返す
		//! \sa BihinKanri::save()
		bool save(string newfilename);
		
		//! 保存用のデータをstringとして所得する
		//! 一覧表示をプログラムで行う際に用いる
		//! \param 文字列の生成に成功したかどうかbool変数を参照渡ししてチェック
		//! \param デリミタ
		//! \param 正規化（値がない部分に"<n/a>"という文字列を入れる）かどうか
		//! \param ヘッダー情報を先頭行に付けるかどうか デフォルトではヘッダー情報を付けるので、falseにしておく
		//! \param フォーマット（各カラムを一定の文字列で揃えること）するかどうか（未実装）
		//! \return 最終的にデータファイルに書き込まれる内容
		string getBihinText(bool &isWritable, const char delim = ',', bool regular = false, bool noHeader = false, bool format = false) const;
		
		//! 保存用のデータをvector<string>形式で所得する
		//! 1行のデータがvectorのひとつの要素となる
		//! \param 文字列の生成に成功したかどうかbool変数を参照渡ししてチェック
		//! \param デリミタ
		//! \param 正規化（値がない部分に"<n/a>"という文字列を入れる）かどうか
		//! \param ヘッダー情報を先頭行に付けるかどうか デフォルトではヘッダー情報を付けるので、falseにしておく
		//! \param フォーマット（各カラムを20文字で揃えること）するかどうか（未実装）
		//! \return 1行のデータがvectorの1つの要素
		vector<string> getBihinTextLines(bool &isWritable, const char delim = ',', bool regular = false, bool noHeader = false, bool format = false) const;
		
		//! いくつかのstringをキーにして、値を所得する
		//! \param 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc）をstringで指定
		//! \param サイズ（"2L"、"23.5"、etc）をstringで指定
		//! \param タイプ（数、クリーニング中、保管場所、etc）を静的メンバ定数KEY_*で指定
		//! \return 値（"3"、"4階大会議室倉庫"、etc）がstringで返される
		string getValue(const string &name, const string &size, const string &type) const;
		
		//! いくつかのstringをキーにして、値を設定する
		//! \param 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc）をstringで指定
		//! \param サイズ（"2L"、"23.5"、etc）をstringで指定
		//! \param タイプ（数、クリーニング中、保管場所、etc）を静的メンバ定数KEY_*で指定
		//! \param 値（"3"、"4階大会議室倉庫"、etc）をstringで指定
		void setValue(const string &name, const string &size, const string &type, const string &value);
		
		//! 名称のリストを所得する
		//! \return vector<string>参照型 \n
		//! イメージ: {"災害対策用上着", "災害対策用上着夏季薄手", ..., "帰宅支援キット", ...}
		vector<string> &getBihinNames() const;
		
		//! 指定した名称のサイズのリストを所得する
		//! \param 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc）をstringで指定
		//! \return vector<string>参照型 \n
		//! イメージ: {"2L", "L", "M", "S"} \n
		//! イメージ: {"23.5", "24", "24,5", "25", "25.5", "26"}
		vector<string> &getBihinSizes(string &bihinName) const;
		
		//! カラムの並びを所得する
		//! \return カラムの並び
		vector<string> &getColumnsSequence() const;
		
		//! 指定した名称の備品を追加する
		//! すでに追加された備品の場合は、上書きを行う
		//! 各値（現在数、保管場所、etc）の削除は行わない
		//! \param 備品名称
		//! \param 備品サイズ
		void addBihin(const string &bihinName, const vector<string> &bihinSizes);
		
		//! 指定した名称の備品を削除する
		//! 各値を完全に削除
		//! \param 備品名称
		void deleteBihin(const string &bihinName);
	
	private:
		//! データファイルのカラムの並び
		vector<string> *columnsSequence;
				
		//! 名称のリスト
		vector<string> *bihinNames;
		
		//! サイズのマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc）
		//! \value サイズのリストへのポインタ
		map<string, vector<string> *> *bihinsSizes;
		
		//! 数のマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi可能）
		map<string, string> *counts;
		
		//! クリーニング中のマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi可能）
		map<string, string> *cleanings;
		
		//! クリーニングのマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi可能）
		map<string, string> *cleaneds;
		
		//! いつクリーニングしたかのマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi不可能）
		map<string, string> *whenCleaneds;
		
		//! 誰がクリーニングしたかのマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi不可能）
		map<string, string> *whoCleaneds;
		
		//! 総数のマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi可能）
		map<string, string> *allCounts;
		
		//! 保管場所のマップ
		//! \key 名称（"災害対策用上着"、"折畳み式ヘルメット"、etc） + サイズ
		//! \value string型（atoi不可能）
		map<string, string> *storedPlaces;
		
		//! データファイル名（絶対パス）
		string filename;
};

#endif