#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<clocale>
#include<codecvt>
#include<iterator>
#include<algorithm>
#include<vector>
#include<unordered_map>
#include<map>

class Trie
{
private:
	size_t count = 0;
public:
	std::set<std::wstring> Res; // полученные слова
	std::unordered_multimap<std::wstring, std::wstring> dictonary; //словарь
	std::multimap<size_t, std::wstring> words; // хранение слов опеределенной длины и начинающихся на конкретную букву
	std::map<std::wstring, int> letters_of_word; // буквы заданного слова, контейнер не содержит повторяющиеся буквы
	std::vector<wchar_t> word;// для хранения заданного слова
	bool LoadDictonary(char* Filename);
	void GuessWord(std::wstring str, size_t length);
};


bool Trie::LoadDictonary(char* Filename) //загрузка словаря
{
	std::wifstream wif(Filename);
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	wif.imbue(utf8_locale);
	wchar_t bom = L'\0';
	wif.get(bom);
	std::wstring Buff = L"";
	while (!wif.eof())
	{
		wif >> Buff;
		if (Buff.find_first_of(L"ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ") != std::wstring::npos) // если есть заглавные буквы
		{
			continue;
		}
		if (Buff.find_first_of(L"~`!@#$%^&*()_+=-?><.,\\|/;:'""№qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890") != std::wstring::npos)
		{
			continue;
		}
		dictonary.emplace(Buff.substr(0,1), Buff);
	}
	wif.close();
	Buff.clear();
	return true;
}

void Trie::GuessWord(std::wstring str, size_t length) // поиск слов
{
	std::wstring temp = str;

	for (size_t i = 0; i < temp.size(); ++i)
	{
		word.push_back(temp[i]);
	}

	for (size_t i = 0; i < length; ++i)
	{
		if (letters_of_word.find(str.substr(i,1))->first == str.substr(i,1))
		{
		
			
			auto range = dictonary.equal_range(temp.substr(i, 1));
			for (std::unordered_multimap<std::wstring, std::wstring>::iterator it = range.first; it != range.second; ++it)
			{
				words.emplace(it->second.size(), it->second);
			}
			for (std::multimap<size_t, std::wstring>::iterator it = words.begin(); it != words.end(); ++it)
			{
				if (it->first <= length)
				{
			
					for (size_t j = 0; j < it->second.size(); ++j)
					{
						auto pos = std::find(word.begin(), word.end(), it->second[j]);
						if (pos != word.end())
						{
							count++;
							word.erase(pos);
						}
					}
					if (count == it->second.size())
					{
						Res.insert(it->second);
					}
					count = 0;
					word.clear();
					for (size_t k = 0; k < temp.size(); ++k)
					{
						word.push_back(temp[k]);
					}
				}
				else break;
			}
			words.clear();
			letters_of_word.erase(str.substr(i,1));
		}
	}
	letters_of_word.clear();
}

int main(int argc, char *argv[])
{

	Trie tr;
	std::wstring Word = L"";

	if (argc < 4)
	{
	std::wcout << L"error" << std::endl;
	return -1;
	}
	//, std::ofstream::ios_base::out | std::ofstream::ios_base::ios_base::trunc
	std::wofstream out(argv[3]);
	std::locale utf8_locale1(std::locale(), new std::codecvt_utf8<wchar_t>);
	out.imbue(utf8_locale1);
	if (!out)
	{
		return -1;
	}


	std::wifstream input(argv[2]);
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	input.imbue(utf8_locale);
	wchar_t bom = L'\0';
	input.get(bom);
	if (!input)
	{
		out << L"error" << std::endl;
		return -1;
	}

	if (tr.LoadDictonary(argv[1]) == false) // словарь
	{
		out << "error" << std::endl;
		return -1;
	}



	input >> Word;
	
	if (Word.find_first_of(L"ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ") != std::wstring::npos) // если есть заглавные буквы
	{
		out << L"error" << std::endl;
		return -1;
	}
	if (Word.find_first_of(L"~`!@#$%^&*()_+=-?><.,\\|/;:'""№qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890") != std::wstring::npos)
	{
		out << L"error" << std::endl;
		return -1;
	}
	size_t L = Word.size();
	for (int i = 0; i <L; ++i)
	{
		tr.letters_of_word.insert(std::pair<std::wstring, int>(Word.substr(i, 1), (int)Word[i]));
	}
	tr.GuessWord(Word, L);
	for (auto i : tr.Res)
	{
		out << i << std::endl;
	}
	tr.Res.clear();
	tr.dictonary.clear();
	input.close();
	out.close();
	return 0;
}
