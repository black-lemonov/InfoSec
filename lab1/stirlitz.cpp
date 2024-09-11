#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>


//  Переводит текстовый шифр в двоичный код
std::string MessageToCode(const std::string& msg, const std::vector<std::string>& alphabet)
{
    std::string sorted_msg = msg;
    std::sort(sorted_msg.begin(), sorted_msg.end());

    std::set<char> letters;
    std::map<char, std::string> alphabet_map;

    for (auto c : msg) {
        letters.insert(c);
    }

    size_t i = 0;
    for (auto l : letters) {
        alphabet_map[l] = alphabet[i];
        ++i;
    }

    std::string code_str;
    for (auto c : msg) {
        for (const auto& [k, v] : alphabet_map) {
            if (c == k) {
                code_str += v;
            }
        }
    }
    return code_str;
}


int CountSplits(const std::string& code_str, const std::vector<std::string>& alphabet) {
    std::string cur_str;
    int max_splits = -1;
    for (size_t i = 0; i < code_str.size(); ++i) {
        cur_str += code_str[i];

        if (std::find(alphabet.begin(), alphabet.end(), cur_str) != alphabet.end()) {
            return max_splits;
        }

        if (i + 2 <= code_str.size()) {
            int tmp = CountSplits(code_str.substr(i+1), alphabet);

            if (tmp != -1) {
                max_splits = std::max(1 + tmp, max_splits);
            }
        }
    }
    return std::max(max_splits, 1); 
}


int main(){
    size_t k;
    std::cout << "k = "; std::cin >> k;
    std::vector<std::string> alphabet;
    alphabet.reserve(k);
    std::string message;
    std::cout << "Enter a message: "; std::cin >> message;
    std::cout << "Enter codes sep-ed with Enter:\n";
    for (int i = 0 ; i < k; i++) {
        std::string word;
        std::cin >> word;
        alphabet.push_back(word);
    }
    std::string code = MessageToCode(message, alphabet);
    int res = CountSplits(code, alphabet);
    if (res == -1) {
        std::cout << "Message cant\'be split" << '\n';
    } else {
        std::cout << "Message can be split into " << res << " parts" << '\n';
    }
}