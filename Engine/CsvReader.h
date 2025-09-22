#pragma once
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <array>
using std::array;
using std::pair;
#include <algorithm>
#include <DirectXMath.h>
using namespace DirectX;

//--------------------------------------------------------
//CSVファイルを扱うクラス
//--------------------------------------------------------
class CsvReader {
public:
	CsvReader() {};
	CsvReader(string filename);
	~CsvReader();
	bool Load(string filename);
	//引数の値のコピーを返し引数自体は1増やす
	int ReturnAndAddIndex(int& index);
	int GetHeight();
	int GetWidth(int line);
	string GetString(int column, int line);
	int GetInt(int column, int line);
	float GetFloat(int column, int line);
	bool GetBool(const string& str);
	//文字から縦のインデックスを探して返す
	int ColumnSearch(CsvReader& csv, const std::string& str);
	//引数から数字だけを取り出して返す
	int extractNumber(const string& str);
	//引数から文字だけを取り出して返す
	string extractString(const string& str);
	//引数から数字だけを取り出して返す
	int extractInt(const string& str);
	pair<int, int> extractInt2(const string& str);
	float extractFloat(const string& str);
	XMFLOAT2 extractFloat2(const string& str);
	XMFLOAT3 extractFloat3(const string& str);
	XMFLOAT4 extractFloat4(const string& str);
private:
	struct LINEREC { vector<string> record; };
	vector<LINEREC> all;
	//引数の中に文字が含まれているか判定
	bool isAllDigits(const string& str);

	template<size_t N>
	array<float, N> extractFloats(const string& str);
	XMFLOAT2 toXMFLOAT2(const array<float, 2>& arr);
	XMFLOAT3 toXMFLOAT3(const array<float, 3>& arr);
	XMFLOAT4 toXMFLOAT4(const array<float, 4>& arr);
};

template<size_t N>
std::array<float, N> CsvReader::extractFloats(const std::string& str)//複数の型に対応させるためのテンプレート
{
	std::array<float, N> values = {};
	std::stringstream ss(str);
	std::string token;
	size_t i = 0;

	//'/'ごとに分ける
	while (std::getline(ss, token, '/') && i < N) {
		//'f'と'_'を取り除く
		token.erase(std::remove(token.begin(), token.end(), '_'), token.end());
		token.erase(std::remove(token.begin(), token.end(), 'f'), token.end());
		std::stringstream valStream(token);
		valStream >> values[i];
		++i;
	}

	return values;
}
