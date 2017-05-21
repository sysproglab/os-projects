#include <iostream>
#include <fstream>
#include <algorithm>

#include <sstream>

void deleteComments(std::string& str) {
    size_t commentStart = 0;
    size_t commentEnd = 0;
    while (true) {
        commentStart = str.find("/*", commentStart);
        if (commentStart == std::string::npos) break;
        commentEnd = str.find("*/", commentStart + 1);
        if (commentEnd == std::string::npos) {
            std::cout << "No end of comment, exiting" << std::endl;
            exit(-1);
        }
        str.erase(str.begin() + commentStart, str.begin() + commentEnd + 2);
    }


    commentStart = 0;
    while (true) {
        commentStart = str.find("//", commentStart);
        if (commentStart == std::string::npos) break;
        commentEnd = str.find("\n", commentStart + 1);
        if (commentEnd == std::string::npos) {
            std::cout << "No end of comment, exiting" << std::endl;
            exit(-1);
        }
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
        if (end_ == std::string::npos) {
            std::cout << "No end of string, exiting" << std::endl;
            exit(-1);
        }
        std::ostringstream ss;
        for (auto i = start + 1; i < end_; ++i) {
            if (str[i] == '\\' && str[i + 1] == 'n') {
                ss <<  "\\x" << std::hex << (int)'\n';
                ++i;
            } else
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
    if (argc < 2) {
        std::cout << "No input file" << std::endl;
        return -1;
    }


    std::ifstream in(argv[1]);
    std::string str((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
    std::cout << str << std::endl << std::endl;

    std::string result;
    std::string work;

    std::vector<size_t> indexes;
    indexes.push_back(str.rfind("#include"));
    indexes.push_back(str.rfind("#define"));
    indexes.push_back(str.rfind("#pragma"));
    indexes.push_back(str.rfind("#using"));
    indexes.erase(std::remove(indexes.begin(), indexes.end(), std::string::npos), indexes.end());
    auto max = std::max_element(indexes.begin(), indexes.end());

    if (max == indexes.end()) {
        work = str;
    } else {
        auto endOfProproc = str.find('\n', *max);
        result.append(str.substr(0, endOfProproc + 1));
        work = str.substr(endOfProproc + 1, str.size());
    }

    deleteComments(work);
    work.erase(std::remove(work.begin(), work.end(), '\n'), work.end());
    work.erase(std::remove(work.begin(), work.end(), '\t'), work.end());

    result.append(work);
    removeFreeLinesAndSpaces(result);
    replaceStrings(result);

    std::cout << result << std::endl;
    return 0;
}