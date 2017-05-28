#include <iostream>
#include <fstream>
#include <sstream>

void insertSpacesAndLines(std::string& str) {
    size_t start = 0;
    while (true) {
        start = str.find_first_of(";", start);
        if (start == std::string::npos) break;
        str.insert(start + 1, "\n");
        start += 2;
    }

    start = 0;
    while (true) {
        start = str.find_first_of("{}", start);
        if (start == std::string::npos) break;
        str.insert(start + 1, "\n");
        start += 2;
    }

    start = 0;
    size_t end = 0;
    while (true) {
        start = str.find("\n ", start);
        if (start == std::string::npos) break;
        end = str.find_first_not_of(" ", start + 1);
        if (end == std::string::npos) break;
        str.erase(str.begin() + start + 1, str.begin() + end);
    }
}

void normalizeStrings(std::string& str) {
    size_t start = 0;
    size_t end_ = 0;
    while (true) {
        start = str.find("\"", start);
        if (start == std::string::npos) break;
        for (auto i = start + 1; i; ++i) {
            if (str[i] == '\"') {
                end_ = i;
                break;
            }
        }

        auto conststr = str.substr(start + 1, end_ - start - 1);
        auto len = conststr.length();
        std::string newString;
        for(auto  i = 0U; i < len; i += 4) {
            std::string byte = conststr.substr(i + 2, 2);
            char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
            if (chr == '\n') {
                newString += "\\n";
            } else {
                newString.push_back(chr);
            }
        }

        str.erase(str.begin() + start + 1, str.begin() + end_);
        str.insert(start + 1, newString);
        start = start + newString.size() + 2;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "No input file" << std::endl;
        return -1;
    }


    std::ifstream in(argv[1]);
    std::string str((std::istreambuf_iterator<char>(in)),
                    std::istreambuf_iterator<char>());
    std::cout << str << std::endl << std::endl;

    insertSpacesAndLines(str);
    normalizeStrings(str);
    std::cout << str << std::endl;
    return 0;
}