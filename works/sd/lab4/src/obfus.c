#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

#include <sstream>

void deleteComments(std::string& str) {
	size_t commentStart = 0;
	size_t commentEnd = 0;
	while (true) {
		commentStart = str.find("/*", commentStart);
		if (commentStart == std::string::npos) break;
		commentEnd = str.find("*/", commentStart + 1);
		str.erase(str.begin() + commentStart, str.begin() + commentEnd + 2);
	}


	commentStart = 0;
	while (true) {
		commentStart = str.find("//", commentStart);
		if (commentStart == std::string::npos) break;
		commentEnd = str.find("\n", commentStart + 1);
		str.erase(str.begin() + commentStart, str.begin() + commentEnd);
	}
}

void replaceStrings(std::string& str) {
	size_t start = 0;
	size_t end_ = 0;
	while (true) {
		start = str.find("\"", start);
		if (start == std::string::npos) break;
		end_ = str.find("\"", start + 1);
		std::ostringstream ss;
		for (auto i = start + 1; i < end_; ++i) {
			if (str[i] == '\\' && str[i + 1] == 'n') {
				ss << "\\x" << std::hex << (int)'\n';
				++i;
			}
			else
				ss << "\\x" << std::hex << (int)str[i];
		}
		str.erase(str.begin() + start + 1, str.begin() + end_);
		str.insert(start + 1, ss.str());
		start = start + 2 + ss.str().size();
	}
}

void removeFreeLinesAndSpaces(std::string& str) {
	size_t start = 0;
	while (true) {
		start = str.find("\n\n", start);
		if (start == std::string::npos) break;
		str.erase(str.begin() + start, str.begin() + start + 1);
	}

	start = 0;
	while (true) {
		start = str.find("  ", start);
		if (start == std::string::npos) break;
		str.erase(str.begin() + start, str.begin() + start + 1);
	}
}

int main(int argc, char** argv) {
	std::ifstream in("example.cpp");

	std::string str((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	std::cout << str << std::endl << std::endl;

	std::string res;
	std::string cur;

	std::vector<size_t> indexes;
	indexes.push_back(str.rfind("#include")); 
	indexes.erase(std::remove(indexes.begin(), indexes.end(), std::string::npos), indexes.end());
	auto max = std::max_element(indexes.begin(), indexes.end());

	if (max == indexes.end()) {
		cur = str;
	}
	else {
		auto endOfProproc = str.find('\n', *max);
		res.append(str.substr(0, endOfProproc + 1));
		cur = str.substr(endOfProproc + 1, str.size());
	}
	//Рассматриваем всё только после include 
	deleteComments(cur);
	cur.erase(std::remove(cur.begin(), cur.end(), '\n'), cur.end()); //удаляем переносы
	cur.erase(std::remove(cur.begin(), cur.end(), '\t'), cur.end()); //удаляем табуляции

	res.append(cur);
	removeFreeLinesAndSpaces(res);
	replaceStrings(res);

	std::cout << res << std::endl;
	return 0;
}
