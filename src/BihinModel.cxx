#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "BihinModel.hxx"
using namespace std;

const char *BihinModel::KEY_BIHIN_NAME    = "bihinName";
const char *BihinModel::KEY_SIZE         = "size";
const char *BihinModel::KEY_COUNT        = "count";
const char *BihinModel::KEY_CLEANING     = "cleaning";
const char *BihinModel::KEY_CLEANED      = "cleaned";
const char *BihinModel::KEY_WHEN_CLEANED = "whenCleaned";
const char *BihinModel::KEY_WHO_CLEANED  = "whoCleaned";
const char *BihinModel::KEY_ALL_COUNT    = "allCount";
const char *BihinModel::KEY_STORED_PLACE = "storedPlace";

BihinModel::BihinModel() {
	counts = new map<string, string>();
	cleanings = new map<string, string>();
	cleaneds = new map<string, string>();
	whenCleaneds = new map<string, string>();
	whoCleaneds = new map<string, string>();
	allCounts = new map<string, string>();
	storedPlaces = new map<string, string>();
	
	columnsSequence = new vector<string>();
	columnsSequence->push_back(BihinModel::KEY_BIHIN_NAME);
	columnsSequence->push_back(BihinModel::KEY_SIZE);
	columnsSequence->push_back(BihinModel::KEY_COUNT);
	columnsSequence->push_back(BihinModel::KEY_CLEANING);
	columnsSequence->push_back(BihinModel::KEY_CLEANED);
	columnsSequence->push_back(BihinModel::KEY_WHEN_CLEANED);
	columnsSequence->push_back(BihinModel::KEY_WHO_CLEANED);
	columnsSequence->push_back(BihinModel::KEY_ALL_COUNT);
	columnsSequence->push_back(BihinModel::KEY_STORED_PLACE);
	
	bihinNames = new vector<string>();
	bihinsSizes = new map<string, vector<string> *>();
}

BihinModel::BihinModel(string filename): filename(filename) {
	cout << "BihinModel object initialized with filename." << endl;
}

bool BihinModel::read() {
	cout << "Read data file." << endl;
	
	bool isRead = true;
	
	set<string> bihinNamesSet;
	
	bihinNames = new vector<string>();
	bihinsSizes = new map<string, vector<string> *>();
	map<string, vector<string> *>::iterator itemsSize;
	
	counts = new map<string, string>();
	cleanings = new map<string, string>();
	cleaneds = new map<string, string>();
	whenCleaneds = new map<string, string>();
	whoCleaneds = new map<string, string>();
	allCounts = new map<string, string>();
	storedPlaces = new map<string, string>();
	
	//! データファイル読み込み開始
	ifstream ifs(filename.c_str());
	cout << "database file = " << filename << endl;
	
	if (ifs) {
		//! 1行読み込み用変数
		string line;
		
		try {
			//! 1行目は、以降のファイルのフォーマットが書いてあるので、最初に読み込む必要がある
			//! 1行目を読み込んで、columnsSequenceを作成する	
			columnsSequence = new vector<string>();
			
			//! カラムの番号
			int columnsNumber = 0;
			
			//! 1行読み込む
			getline(ifs, line);
			
			boost::tokenizer<boost::escaped_list_separator<char> > firsttoks(line);
			for (boost::tokenizer<boost::escaped_list_separator<char> >::iterator itr = firsttoks.begin(); itr != firsttoks.end(); ++itr) {
				string columnKey = boost::trim_copy((*itr));
				columnsSequence->push_back(columnKey);
				cout << boost::format("Column #%d is \"%s\"") % columnsNumber % columnKey << endl;
				++columnsNumber;
			}
		} catch (exception &e) {
			cerr << e.what() << endl;
			isRead = false;
		}
		
		//! データ本体の読み込み
		while (!ifs.eof()) {
			cout << "-----" << endl;
			
			//! 1行読み込む
			getline(ifs, line);
			
			try {
				//! columnの番号
				int columnsNumber = 0;
				
				//! 各値を格納するための変数
				map<string, string> bihinData;
				string bihinName, bihinSize;
				map<string, string>::iterator bihinDatum;
				
				boost::tokenizer<boost::escaped_list_separator<char> > toks(line);
				for (boost::tokenizer<boost::escaped_list_separator<char> >::iterator itr = toks.begin(); itr != toks.end(); ++itr) {
					//! カラム番号がcolumnsSequenceのサイズ以上になっていたら、break
					if (columnsNumber >= columnsSequence->size())
						break;
					
					string columnValue = boost::trim_copy((*itr));
					string columnKey = (*columnsSequence)[columnsNumber];
					
					bihinData.insert(make_pair(columnKey, columnValue));
					++columnsNumber;
				}
				
				//! bihinDataの値を使って、メンバ変数の初期化を行う
				
				//! bihinName（名称） 重複を避けるため、まずset<string>に追加する。最終的にvectorにする
				bihinDatum = bihinData.find(BihinModel::KEY_BIHIN_NAME);
				bihinName = bihinDatum->second;
				bihinNamesSet.insert(bihinName);
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_BIHIN_NAME % bihinName << endl;
				
				//! size（寸法） bihinsSizesマップに、bihinNameをキーとして持つ要素があればそこにsizeを追加、なければ新しくvector<string>を作成しbihinsSizesマップに追加する
				bihinDatum = bihinData.find(BihinModel::KEY_SIZE);
				bihinSize = bihinDatum->second;
				itemsSize = bihinsSizes->find(bihinName);
				if (itemsSize != bihinsSizes->end()) {
					itemsSize->second->push_back(bihinSize);
				} else {
					vector<string> *bihinSizes = new vector<string>();
					bihinSizes->push_back(bihinSize);
					bihinsSizes->insert(make_pair(bihinName, bihinSizes));
				}
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_SIZE % bihinSize << endl;
				
				//! 以降、bihinNameとbihinSizeを組み合わせた文字列をキーとして各値を当該マップ変数に挿入していく
				string keyStr = bihinName + bihinSize;
				
				//! count（現在数）
				bihinDatum = bihinData.find(BihinModel::KEY_COUNT);
				counts->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_COUNT % bihinDatum->second << endl;
				
				//! cleaning（クリーニング中）
				bihinDatum = bihinData.find(BihinModel::KEY_CLEANING);
				cleanings->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_CLEANING % bihinDatum->second << endl;
				
				//! cleaned（クリーニング済み）
				bihinDatum = bihinData.find(BihinModel::KEY_CLEANED);
				cleaneds->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_CLEANED % bihinDatum->second << endl;
				
				//! whenCleaned（いつクリーニングしたか）
				bihinDatum = bihinData.find(BihinModel::KEY_WHEN_CLEANED);
				whenCleaneds->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_WHEN_CLEANED % bihinDatum->second << endl;
				
				//! whoCleaned（誰がクリーニングしたか）
				bihinDatum = bihinData.find(BihinModel::KEY_WHO_CLEANED);
				whoCleaneds->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_WHO_CLEANED % bihinDatum->second << endl;
				
				//! allCount（総数）
				bihinDatum = bihinData.find(BihinModel::KEY_ALL_COUNT);
				allCounts->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_ALL_COUNT % bihinDatum->second << endl;
				
				//! storedPlace（保管場所）
				bihinDatum = bihinData.find(BihinModel::KEY_STORED_PLACE);
				storedPlaces->insert(make_pair(keyStr, bihinDatum->second));
				cout << boost::format("column(\"%s\") = \"%s\"") % BihinModel::KEY_STORED_PLACE % bihinDatum->second << endl;
			} catch (exception &e) {
				cerr << e.what() << endl;
				isRead = false;
			}			
		}
		
		//! 最後にbihinNamesSetからbihinNamesへ要素を入れ替える
		for (set<string>::iterator bihinNamesSetItr = bihinNamesSet.begin(); bihinNamesSetItr != bihinNamesSet.end(); bihinNamesSetItr++) {
			bihinNames->push_back((*bihinNamesSetItr));
		}
		
	}
	ifs.close();
	
	return isRead;
}

string BihinModel::getValue(const string &name, const string &size, const string &type) const {
	map<string, string>::iterator itr;
	string value("");
	string key = name + size;
	if (type == BihinModel::KEY_COUNT) {
		itr = counts->find(key);
		if (itr != counts->end())
			value = itr->second;
	} else if (type == BihinModel::KEY_CLEANING) {
		itr = cleanings->find(key);
		if (itr != cleanings->end())
			value = itr->second;
	} else if (type == BihinModel::KEY_CLEANED) {
		itr = cleaneds->find(key);
		if (itr != cleaneds->end())
			value = itr->second;
	} else if (type == BihinModel::KEY_WHEN_CLEANED) {
		itr = whenCleaneds->find(key);
		if (itr != whenCleaneds->end())
			value = itr->second;
	} else if (type == BihinModel::KEY_WHO_CLEANED) {
		itr = whoCleaneds->find(key);
		if (itr != whoCleaneds->end())
			value = itr->second;
	} else if (type == BihinModel::KEY_ALL_COUNT) {
		itr = allCounts->find(key);
		if (itr != allCounts->end())
			value = itr->second;
	} else if (type == BihinModel::KEY_STORED_PLACE) {
		itr = storedPlaces->find(key);
		if (itr != storedPlaces->end())
			value = itr->second;
	}
	return value;
}

void BihinModel::setValue(const string &name, const string &size, const string &type, const string &value) {
	string key = name + size;
	if (type == BihinModel::KEY_COUNT) {
		counts->erase(key);
		counts->insert(make_pair(key, value));
	} else if (type == BihinModel::KEY_CLEANING) {
		cleanings->erase(key);
		cleanings->insert(make_pair(key, value));
	} else if (type == BihinModel::KEY_CLEANED) {
		cleaneds->erase(key);
		cleaneds->insert(make_pair(key, value));
	} else if (type == BihinModel::KEY_WHEN_CLEANED) {
		whenCleaneds->erase(key);
		whenCleaneds->insert(make_pair(key, value));
	} else if (type == BihinModel::KEY_WHO_CLEANED) {
		whoCleaneds->erase(key);
		whoCleaneds->insert(make_pair(key, value));
	} else if (type == BihinModel::KEY_ALL_COUNT) {
		allCounts->erase(key);
		allCounts->insert(make_pair(key, value));
	} else if (type == BihinModel::KEY_STORED_PLACE) {
		storedPlaces->erase(key);
		storedPlaces->insert(make_pair(key, value));
	}
}

vector<string> &BihinModel::getBihinNames() const {
	return (*bihinNames);
}

vector<string> &BihinModel::getBihinSizes(string &bihinName) const {
	map<string, vector<string> *>::iterator itr = bihinsSizes->find(bihinName);
	return *(itr->second);
}

vector<string> &BihinModel::getColumnsSequence() const {
	return (*columnsSequence);
}

bool BihinModel::save() const {
	if (filename == "")
		return false;
	
	bool isWritable = false;

	//! データファイル保存
	ofstream ofs(filename.c_str());
	string data = getBihinText(isWritable);
	ofs << data;
	ofs.close();
	return isWritable;
}

bool BihinModel::save(string newfilename) {
	filename = newfilename;
	return save();
}

string BihinModel::getBihinText(bool &isWritable, char delim, bool regular, bool noHeader, bool format) const {
	//! 返り値用stringstream
	stringstream ss;
	//! 値はgetBihinTextLinesからvector<string>形式で所得
	const vector<string> lines = getBihinTextLines(isWritable, delim, regular, noHeader, format);
	
	for (vector<string>::const_iterator line = lines.begin(); line < lines.end(); ++line) {
		ss << (*line) << endl;
	}
	
	return ss.str();
}

vector<string> BihinModel::getBihinTextLines(bool &isWritable, char delim, bool regular, bool noHeader, bool format) const {
	//! 返り値用vector
	vector<string> retvec;
	//! このstringstreamに流し込んだ値をvectorにpush_backしていく
	stringstream ss;
	//! falseで初期化
	isWritable = false;
	
	if (!noHeader) {
		//! 最初にcolumnsSequenceを書き込む	
		for (int columnNumber = 0; columnNumber < columnsSequence->size(); ++columnNumber) {
			if (columnNumber != (columnsSequence->size() - 1)) {
				ss << (*columnsSequence)[columnNumber] << delim;
			} else {
				ss << (*columnsSequence)[columnNumber];
			}
		}
	}
	retvec.push_back(ss.str());
	ss.str("");
	
	//! データを書き込んでいく
	for (vector<string>::iterator bihinName = bihinNames->begin(); bihinName < bihinNames->end(); ++bihinName) {
		map<string, vector<string> *>::iterator bihinSizes = bihinsSizes->find((*bihinName));
		
		//! bihinSizesがなかったら、continue
		if (bihinSizes == bihinsSizes->end())
			continue;
		
		for (vector<string>::iterator bihinSize = bihinSizes->second->begin(); bihinSize < bihinSizes->second->end(); ++bihinSize) {			
			for (int columnNumber = 0; columnNumber < columnsSequence->size(); ++columnNumber) {
				string columnKey = (*columnsSequence)[columnNumber];
				string columnValue;
				if (columnKey == BihinModel::KEY_BIHIN_NAME) {
					columnValue = (*bihinName);
				} else if (columnKey == BihinModel::KEY_SIZE) {
					columnValue = (*bihinSize);
				} else if (columnKey == BihinModel::KEY_COUNT) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_COUNT);
				} else if (columnKey == BihinModel::KEY_CLEANING) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_CLEANING);
				} else if (columnKey == BihinModel::KEY_CLEANED) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_CLEANED);
				} else if (columnKey == BihinModel::KEY_WHEN_CLEANED) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_WHEN_CLEANED);
				} else if (columnKey == BihinModel::KEY_WHO_CLEANED) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_WHO_CLEANED);
				} else if (columnKey == BihinModel::KEY_ALL_COUNT) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_ALL_COUNT);
				} else if (columnKey == BihinModel::KEY_STORED_PLACE) {
					columnValue = getValue((*bihinName), (*bihinSize), BihinModel::KEY_STORED_PLACE);
				}
				if (regular && columnValue == "")
					columnValue = string("<n/a>");
				if (columnNumber != (columnsSequence->size() - 1)) {
					ss << columnValue << delim;
				} else {
					ss << columnValue;
				}
			}
			retvec.push_back(ss.str());
			ss.str("");
			isWritable = true;
		}
		
	}
	
	return retvec;
}

void BihinModel::addBihin(const string &bihinName, const vector<string> &bihinSizes) {
	cout << "add bihin " << bihinName << endl;
	//! 名称のリストから、備品名称を削除
	for (vector<string>::iterator itr = bihinNames->begin(); itr < bihinNames->end(); ++itr) {
		if ((*itr) == bihinName) {
			bihinNames->erase(itr);
		}
	}
	
	//! 備品サイズの登録
	bihinNames->push_back(bihinName);
	vector<string> *newBihinSizes = new vector<string>();
	for (vector<string>::const_iterator bihinSize = bihinSizes.begin(); bihinSize < bihinSizes.end(); ++bihinSize) {
		newBihinSizes->push_back((*bihinSize));
		cout << "add size " << *bihinSize << endl;
	}
	bihinsSizes->erase(bihinName);
	bihinsSizes->insert(make_pair(bihinName, newBihinSizes));
}

void BihinModel::deleteBihin(const string &bihinName) {
	//! 名称のリストから、備品名称を削除
	for (vector<string>::iterator itr = bihinNames->begin(); itr < bihinNames->end(); ++itr) {
		if ((*itr) == bihinName) {
			bihinNames->erase(itr);
		}
	}
	
	//! 各値（現在数、保管場所など）を削除するために、サイズベクターを所得
	map<string, vector<string> *>::iterator bihinsSizesItr = bihinsSizes->find(bihinName);
	
	if (bihinsSizesItr != bihinsSizes->end()) {
		vector<string> *bihinSizes = bihinsSizesItr->second;
		//! 各値を削除
		for (vector<string>::iterator bihinSize = bihinSizes->begin(); bihinSize < bihinSizes->end(); ++bihinSize) {
			string key = bihinName + (*bihinSize);
			counts->erase(key);
			cleanings->erase(key);
			cleaneds->erase(key);
			whenCleaneds->erase(key);
			whoCleaneds->erase(key);
			allCounts->erase(key);
			storedPlaces->erase(key);
		}
		//! 最後にsizesをdelete
		delete bihinSizes;
		//! マップの要素もeraseしておく
		bihinsSizes->erase(bihinName);
	}
	
}