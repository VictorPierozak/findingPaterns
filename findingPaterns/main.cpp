#include<iostream>
#include<fstream>
#include<vector>
#include<array>
#include<cmath>
#include<algorithm>

// Configuration 

const int ALPHABET_SIZE = 255;
const size_t S_SIZE = 1000;
const size_t L_SIZE = 10000;
const size_t X_SIZE = 100000;

const int PATTERN_SIZE = 100;

const char* KMP = "KMP";
const char* BM	= "BM";
const char* KR	= "KR";

//Function definition

//Reading text and pattern from files

int init_txt(char*& txt, char opt, std::ifstream& file);
void adjust_txt(char*& txt, const int size);

int init_pattern(char*& pattern, char* argv);

//Core function

std::vector<int> search_pattern(char* algorithm, char*& txt, const size_t txtLength, const char* pattern, const int patternLength);

//KMP

int* makeLPS(const char* text, const int patternLength);
std::vector<int> findPatternKMP(const char* txt, const size_t txtLength, const char* pattern, const int patternLength);

//BM

std::array<int,ALPHABET_SIZE>&& makeLast(const char* pattern, const int patternLength);
std::vector<int> findPatternBM(const char* txt, const size_t txtLength, const char* pattern, const int patternLength);

//KR
int hash(const char* pattern, const int patternLength, const int alphabetSize, const int fCode, const int P);
std::vector<int> findPatternKR(const char* txt, const size_t txtLength, const char* pattern, const int patternLength, const int alphabetSize,
	const int fCode, const int P);

//Another
int max(int A, int B);

void test();

/* 
	Arguments to program should come in the order :
		- path to the file containing text
		- number of character read at once:
			- 'S' - 1000
			- 'L' - 10 000
			- 'X' - 100 000
		- path to the file containing pattern
		- selected algorithm
			- "KMP"	- Knuth–Morris–Pratt 
			- "KR"	- Karp-Rabin
			- "BM"	- Boyet Moore (UNFINISHED)
*/

int main(int argc, char** argv)
{
	std::ifstream file;
	file.open(argv[1]);
	if (file.is_open() == false)
	{
		std::cerr << "TEXT HAS NOT BEEN LOADED";
		exit(EXIT_FAILURE);
	}
	
	char* txt = nullptr;
	size_t txtSize = init_txt(txt, *argv[2], file);
	
	char* pattern = nullptr;
	int patternSize = init_pattern(pattern, argv[3]);

	std::vector<int> results;
	std::vector<int> finalResults;

	int pos = 0;

	if (file.eof() == true)
	{
		finalResults = search_pattern(argv[4], txt, txtSize, pattern, patternSize);
	}
	else
	{
		while (true)
		{
			results = search_pattern(argv[4], txt, txtSize, pattern, patternSize);

			for (int i = 0; i < results.size(); i++)
				results[i] += pos;

			finalResults.insert(finalResults.end(), results.begin(), results.end());

			pos += txtSize;
			if (file.eof() == true) break;
			txtSize = init_txt(txt, *argv[2], file);

		}
	}

	file.close();

	std::cout << "Positions:\n";
	for (auto x : finalResults)
		std::cout << x << ' ';

	return 0;
}

//
//	Reading text from file
//

int init_txt(char*& txt, char opt, std::ifstream& file)
{
	if (txt != nullptr)
	{
		delete[] txt;
		txt = nullptr;
	}

	int txtSize = 0;
	
	if (opt == 'X')
		txtSize = X_SIZE;
	else if (opt == 'L')
		txtSize = L_SIZE;
	else txtSize = S_SIZE;

	txt = new char[txtSize];

	int n = 0;
	for (n = 0; n < txtSize && file.eof() == false; n++)
	{
		txt[n] = file.get();
		if (txt[n] == '\n')
			n--;
	}

	if (n < txtSize)
	{
		n = n - 1;
		adjust_txt(txt, n);
	}

	return n;
}

void adjust_txt(char*& txt, const int size)
{
	char* buffer = new char[size];
	strncpy(buffer, txt, size);
	delete[] txt;
	txt = new char[size];
	strncpy(txt, buffer, size);
	delete[] buffer;
}

int init_pattern(char*& pattern, char* argv)
{
	std::ifstream file;
	file.open(argv);
	if (file.is_open() == false)
	{
		std::cerr << "PATTERN HAS NOT BEEN LOADED!";
		exit(EXIT_FAILURE);
	}

	int pSize = PATTERN_SIZE;
	pattern = new char[pSize];

	int i = 0;
	do
	{
		if (i != 0)
		{
			pSize += PATTERN_SIZE;
			adjust_txt(pattern, pSize);
		}

		for (; i < pSize && file.eof() == false; i++)
		{
			pattern[i] = file.get();
			if (pattern[i] == '\n')
				i--;
		}

	} while (file.eof() == false);

		pSize = i-1;
		adjust_txt(pattern, pSize);

	file.close();

	return pSize;
}

//
//	Core function
//

std::vector<int> search_pattern(char* algorithm, char*& txt, const size_t txtLength, const char* pattern, const int patternLength)
{
	std::vector<int> results;
	if (strcmp(algorithm, KMP) == 0)
	{
		results = findPatternKMP(txt, txtLength, pattern, patternLength);
	}
	else if (strcmp(algorithm, KMP) == 0)
	{
		// not finished yet
	}
	else if (strcmp(algorithm, KMP) == 0)
	{
		results = findPatternBM(txt, txtLength, pattern, patternLength);
	}

	return results;
}

//
// KMP
//

int* makeLPS(const char* pattern, const int patternLength)
{
	int* lps = new int[patternLength];
	lps[0] = -1;
	int t = -1;
	for (int i = 1; i < patternLength; i++)
	{
		while (t >= 0 && pattern[t] != pattern[i])
			t = lps[t];
		t = t + 1;
		lps[i] = t;
	}
	return lps;
}

std::vector<int> findPatternKMP(const char* text, const size_t textLen, const char* pattern, const int patternLen)
{
	std::vector<int> result;
	int patternPos = 0;
	int* lps = makeLPS(pattern, patternLen);
	size_t textPos = 0;
	while (textPos < textLen)
	{
		while (pattern[patternPos] == text[textPos + patternPos] && patternPos < patternLen)
			patternPos++;

		if (patternPos == patternLen)
		{
			result.push_back(textPos);
			textPos += patternPos;
			patternPos = 0;
			continue;
		}

		textPos += size_t( patternPos - lps[patternPos] );
		patternPos = max(0, lps[patternPos]);

	}
	delete[] lps;
	return result;
}

//
// BM
//

std::array<int, ALPHABET_SIZE>&& makeLast(const char* pattern, const int patternLength)
{
	std::array<int, ALPHABET_SIZE> lastArr;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		lastArr[i] = -1;
	for (int i = 0; i < patternLength; i++)
		lastArr[pattern[i]] = i;

	return std::move(lastArr);
}
std::vector<int> findPatternBM(const char* txt, const size_t txtLength, const char* pattern, const int patternLength)
{
	std::array<int, ALPHABET_SIZE> lastArr = makeLast(pattern, patternLength);
	std::vector<int> result;
	size_t textPos = 0;
	int patternPos = patternLength - 1;
	while (textPos <= txtLength - patternLength)
	{
		patternPos = patternLength - 1;
		while (patternPos > -1 && pattern[patternPos] == txt[textPos + patternPos])
			patternPos--;
		if (patternPos == -1)
		{
			result.push_back(textPos);
			textPos += patternLength;
			continue;
		}
		textPos += size_t(max(1, patternPos - lastArr[txt[textPos + patternPos]] ));
	}

	return result;
}

//
// KR 
//

void copyStr(char* dest, const int size, const char* source, const int sPos = 0)
{
	for (int i = sPos; i < sPos + size; i++)
	{
		dest[i - sPos] = source[i];
	}
}

int hash(const char* pattern, const int patternLength, const int alphabetSize,const int fCode, const int P)
{
	long long result = 0;
	for (int i = 0; i < patternLength; i++)
	{
		result = alphabetSize * result + (pattern[i] - fCode);
	}
	result = result % P;
	return result;
}

std::vector<int> findPatternKR(const char* txt, const size_t txtLength, const char* pattern, const int patternLength, const int alphabetSize, 
	const int fCode, const int P)
{
	const int patternHash = hash(pattern, patternLength, alphabetSize, fCode, P);
	char* frame = new char[patternLength];
	copyStr(frame, patternLength, txt);
	int frameHash = hash(frame, patternLength, alphabetSize, fCode, P);

	size_t txtPos = 0;
	std::vector<int> result;
	const int D = ((patternLength-1) * alphabetSize) % P;
	bool flag = false;
	while (txtPos < txtLength)
	{
		if (frameHash == patternHash)
		{
			for (int i = 0; i <= patternLength; i++)
				if (i == patternLength)
				{
					result.push_back(txtPos);
					txtPos += patternLength;
					if ((txtLength - txtPos) - patternLength <= 0) break;
					copyStr(frame, patternLength, txt, txtPos);
					frameHash = hash(frame, patternLength, alphabetSize, fCode, P);
				}
				else if (pattern[i] != txt[txtPos + i])
				{
					txtPos++;
					frameHash = (((frameHash - D * (txt[txtPos - 1] - fCode)) % P * alphabetSize) % P + txt[txtPos + 1] - fCode) % P;
					break;
				}
		}
		else
		{
			txtPos++;
			frameHash = (((frameHash - D * (txt[txtPos - 1] - fCode)) % P * alphabetSize) % P + txt[txtPos + 1] - fCode) % P;
		}
	}

	delete[] frame;

	return result;
}

//
//	Another functions
//

void test()
{
	const char* txt = "PPLSbABPlSPllPlSOp";
	const char* pattern = "PlS";
	std::vector<int> results = findPatternKMP(txt, strlen(txt), pattern, strlen(pattern));
	for (auto x : results)
		std::cout << x << ' ';
	std::cout << std::endl;

	results = findPatternBM(txt, strlen(txt), pattern, strlen(pattern));
	for (auto x : results)
		std::cout << x << ' ';
	std::cout << std::endl;

	const char txt2[] = "AABABBBABCABAAB";
	const char pattern2[] = "AB";
	results = findPatternKR(txt2, strlen(txt2), pattern2, strlen(pattern2), 3, 'A', 23);
	for (auto x : results)
		std::cout << x << ' ';
	std::cout << std::endl;
}

int max(int A, int B)
{
	if (A > B) return A;
	return B;
}