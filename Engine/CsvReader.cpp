#include "csvReader.h"
#include <fstream>
#include <sstream>
#include <assert.h>

CsvReader::CsvReader(string filename)
{
	Load(filename);
}

bool CsvReader::Load(string filename)
{
	all.clear();

	std::ifstream ifs(filename);
	if (!ifs) return false;

	// BOM Skipする
	unsigned char BOMS[] = { 0xEF, 0xBB, 0xBF };
	bool found = true;
	for (int i = 0; i < 3; i++) {
		if (ifs.get() != BOMS[i]) {
			found = false;
			break;
		}
	}
	if (!found) {
		ifs.seekg(std::ios_base::beg);
	}

	// データを読む
	string lineStr;
	while (getline(ifs, lineStr)) {
		while (true) {
			int dq = 0;
			for (int i = 0; i < lineStr.size(); i++) {
				if (lineStr[i] == '"')
					dq++;
			}
			if (dq % 2 == 0)
				break;
			string s;
			getline(ifs, s);
			lineStr += "\n" + s;
		}
		for (auto it = lineStr.begin(); it != lineStr.end();) {
			if (*it == '"')
				it = lineStr.erase(it);
			if (it != lineStr.end())
				it++;
		}

		// 行内を,で切り分ける
		LINEREC lineRecord;
		int top = 0;
		bool indq = false;
		for (int n = 0; n < lineStr.size(); n++) {
			if (lineStr[n] == ',') {
				if (!indq) {
					lineRecord.record.emplace_back(lineStr.substr(top, (size_t)(n - top)));
					top = n + 1;
				}
			}
			else if (lineStr[n] == '"')
				indq = !indq;
		}
		lineRecord.record.emplace_back(lineStr.substr(top, lineStr.size() - top));
		all.emplace_back(lineRecord);
	}
	ifs.close();
	return true;
}

int CsvReader::ReturnAndAddIndex(int& index)
{
	int currentindex = index;
	index++;
	return currentindex;
}

CsvReader::~CsvReader()
{
	for (auto rec : all)
		rec.record.clear();
	all.clear();
}

int CsvReader::GetHeight()
{
	return all.size();
}

int CsvReader::GetWidth(int line)
{
	assert(line < GetHeight());
	return all[line].record.size();
}

std::string CsvReader::GetString(int column, int line)
{
	assert(line < GetHeight());
	if (column >= GetWidth(line))
		return "";
	return all[line].record[column];
}

int CsvReader::GetInt(int column, int line)
{
	std::string str = GetString(column, line);

	if (str == "")
	{
		return 0;
	}

	if (isAllDigits(str))
	{
		return std::stoi(str);
	}
	else
	{
		return 0;
	}
}

float CsvReader::GetFloat(int column, int line)
{
	std::string str = GetString(column, line);

	if (str == "")
		return 0.0f;
	return std::stof(str);
}

bool CsvReader::GetBool(const string& str)
{
	if (str == "TRUE" || str == "true")
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CsvReader::ColumnSearch(CsvReader& csv, const string& str)
{
	int width_ = csv.GetWidth(0);//横幅を取得
	int height_ = csv.GetHeight();//縦幅を取得

	for (int w = 0; w < width_; w++)
	{
		for (int h = 0; h < height_; h++)
		{
			if (str == csv.GetString(w, h))
			{
				return h;
			}
		}
	}

	return -1;//見つからなかった場合
}

int CsvReader::extractNumber(const string& str)
{
	string tmp = "";
	for (char c : str)
	{
		if (isdigit(c))
		{
			tmp += c;
		}
	}
	int resultNum = atoi(tmp.c_str());
	return resultNum;
}

string CsvReader::extractString(const string& str)
{
	string resultStr = "";
	for (char c : str)
	{
		if (!isdigit(c))
		{
			resultStr += c;
		}
	}
	return resultStr;
}

int CsvReader::extractInt(const string& str)
{
	string token = str;
	int values;

	//文字列から'f'と'_'を取り除く
	token.erase(std::remove(token.begin(), token.end(), '_'), token.end());
	std::stringstream valStream(token);
	valStream >> values;

	int result = 0;
	result = values;

	return result;
}

pair<int, int> CsvReader::extractInt2(const string& str)
{
	std::stringstream ss(str);
	string token;
	int values[2] = { 0, 0 };
	int i = 0;

	// '/' で分割して整数を抽出
	while (std::getline(ss, token, '/') && i < 2)
	{
		// 不要な文字を削除
		token.erase(std::remove(token.begin(), token.end(), '_'), token.end());
		token.erase(std::remove(token.begin(), token.end(), 'f'), token.end());

		std::stringstream valStream(token);
		valStream >> values[i];
		++i;
	}

	pair<int, int> result = {values[0], values[1]};
	return result;
}

bool CsvReader::isAllDigits(const string& str) {
	if (str.empty())//strが空だったらfalse
	{
		return false;
	}

	for (char ch : str) {//strの中に数字以外が含まれていたらfalse
		if (!isdigit(ch)) {
			return false;
		}
	}

	return true;
}

float CsvReader::extractFloat(const string& str) {
	return extractFloats<1>(str)[0];
}

XMFLOAT2 CsvReader::extractFloat2(const string& str) {
	return toXMFLOAT2(extractFloats<2>(str));
}

XMFLOAT3 CsvReader::extractFloat3(const string& str) {
	return toXMFLOAT3(extractFloats<3>(str));
}

XMFLOAT4 CsvReader::extractFloat4(const string& str) {
	return toXMFLOAT4(extractFloats<4>(str));
}

XMFLOAT2 CsvReader::toXMFLOAT2(const array<float, 2>& arr) {
	return XMFLOAT2{ arr[0], arr[1] };
}

XMFLOAT3 CsvReader::toXMFLOAT3(const array<float, 3>& arr) {
	return XMFLOAT3{ arr[0], arr[1], arr[2] };
}

XMFLOAT4 CsvReader::toXMFLOAT4(const array<float, 4>& arr) {
	return XMFLOAT4{ arr[0], arr[1], arr[2], arr[3] };
}
