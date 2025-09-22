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
//CSV�t�@�C���������N���X
//--------------------------------------------------------
class CsvReader {
public:
	CsvReader() {};
	CsvReader(string filename);
	~CsvReader();
	bool Load(string filename);
	//�����̒l�̃R�s�[��Ԃ��������̂�1���₷
	int ReturnAndAddIndex(int& index);
	int GetHeight();
	int GetWidth(int line);
	string GetString(int column, int line);
	int GetInt(int column, int line);
	float GetFloat(int column, int line);
	bool GetBool(const string& str);
	//��������c�̃C���f�b�N�X��T���ĕԂ�
	int ColumnSearch(CsvReader& csv, const std::string& str);
	//�������琔�����������o���ĕԂ�
	int extractNumber(const string& str);
	//�������當�����������o���ĕԂ�
	string extractString(const string& str);
	//�������琔�����������o���ĕԂ�
	int extractInt(const string& str);
	pair<int, int> extractInt2(const string& str);
	float extractFloat(const string& str);
	XMFLOAT2 extractFloat2(const string& str);
	XMFLOAT3 extractFloat3(const string& str);
	XMFLOAT4 extractFloat4(const string& str);
private:
	struct LINEREC { vector<string> record; };
	vector<LINEREC> all;
	//�����̒��ɕ������܂܂�Ă��邩����
	bool isAllDigits(const string& str);

	template<size_t N>
	array<float, N> extractFloats(const string& str);
	XMFLOAT2 toXMFLOAT2(const array<float, 2>& arr);
	XMFLOAT3 toXMFLOAT3(const array<float, 3>& arr);
	XMFLOAT4 toXMFLOAT4(const array<float, 4>& arr);
};

template<size_t N>
std::array<float, N> CsvReader::extractFloats(const std::string& str)//�����̌^�ɑΉ������邽�߂̃e���v���[�g
{
	std::array<float, N> values = {};
	std::stringstream ss(str);
	std::string token;
	size_t i = 0;

	//'/'���Ƃɕ�����
	while (std::getline(ss, token, '/') && i < N) {
		//'f'��'_'����菜��
		token.erase(std::remove(token.begin(), token.end(), '_'), token.end());
		token.erase(std::remove(token.begin(), token.end(), 'f'), token.end());
		std::stringstream valStream(token);
		valStream >> values[i];
		++i;
	}

	return values;
}
