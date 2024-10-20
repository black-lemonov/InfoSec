#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <set>

class ChineseWall {
private:
    size_t subjects;
    size_t objects;
    size_t firms;
    std::vector<std::vector<bool>> accessMatrix;
    std::vector<std::pair<char, char>> securityLabels;
    bool isHistoryEmpty = true;

public:
    ChineseWall() = default;

    ChineseWall(size_t n, size_t m, size_t f) {
        subjects = n;
        objects = m;
        firms = f;
        accessMatrix = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false));
        securityLabels = std::vector<std::pair<char, char>>(m, {'_', '_'});
    }

    void Start() {
        if (!isHistoryEmpty) {
            for (auto& row : accessMatrix) {
                std::replace(row.begin(), row.end(), true, false);
            }
            isHistoryEmpty = true;
        }
    }

    void Read(size_t s, size_t o) {
        if (isHistoryEmpty || accessMatrix[s][o]) {
            std::cout << "accepted" << '\n';
            accessMatrix[s][o] = true;
        } else {
            std::cout << "refused" << '\n';
        }
    }

    void Write(size_t s, size_t o) {
        if (isHistoryEmpty || accessMatrix[s][o]) {
            std::cout << "accepted" << '\n';
            accessMatrix[s][o] = true;
        } else {
            for (size_t i = 0; i != objects; ++i) {
                if (accessMatrix[s][i] && i != o 
                    && securityLabels[i].first == securityLabels[o].first
                    && securityLabels[i].first != '_') {
                    std::cout << "refused" << '\n';
                    return;
                }
            }
            accessMatrix[s][o] = true;
            std::cout << "accepted" << '\n';
        }
    }

    void ReportS(size_t s) {
        for (size_t i = 0; i != objects; ++i) {
            if (accessMatrix[s][i] || securityLabels[i].second != '_') {
                std::cout << "object " << i;
                std::cout << " firm " << securityLabels[i].second << '\n';
            }
        }
        std::cout << '\n';
    }

    void ReportO(size_t o) {
        std::cout << "subjects: ";
        for (size_t i = 0; i != subjects; ++i) {
            if (accessMatrix[i][o]) {
                std::cout << i << ", ";
            }
        }
        std::cout << '\n';
    }

    void Portfolio(char f) {
        std::cout << "objects: ";
        for (size_t i = 0; i != objects; ++i) {
            if (securityLabels[i].second == f) {
                std::cout << i << ", ";
            }
        }
        std::cout << '\n';
    }

    void AddObject(size_t o, char f) {
        securityLabels[o].second = f;
    }

    void SetConflict(char f, char c) {
        for (auto& o : securityLabels) {
            if (o.second == f) {
                o.first = c;
            }
        }
    }

    void PrintAvailableObjects() {
        for (size_t i = 0; i != objects; ++i) {
            if (securityLabels[i].second == '_') {
                std::cout << i << ", ";
            }
        }
    }

    size_t GetFirms() {
        return firms;
    }

    void PrintFirms() {
        std::set<char> firmsChars;
        for (auto [c, f] : securityLabels) {
            if (f != '_') {
                firmsChars.insert(f);
            }
        }
        if (firmsChars.empty()) {
            std::cout << "no ";
            return;
        }
        for (auto f : firmsChars) {
            std::cout << f << ", ";
        }
    }

    void PrintClasses() {
        std::set<char> conflicts;
        for (auto [c, f] : securityLabels) {
            if (c != '_') {
                conflicts.insert(c);
            }
        }
        if (conflicts.empty()) {
            std::cout << "no classes ";
            return;
        }
        for (auto c : conflicts) {
            std::cout << c << ", ";
        }
    }
};


class ChineseWallContext {
private:
    ChineseWall wall;

    void SetPortfolios() {
        std::cout << "Please, set firms portfolios" << '\n';
        for (size_t i = 0; i != wall.GetFirms(); ++i) {
            char f;
            std::cout << "Enter firm\'s letter (";
            wall.PrintFirms();
            std::cout << "firm(s) already exist): ";
            std::cin >> f;
            std::cout << "Enter firms object(s) (";
            wall.PrintAvailableObjects();
            std::cout << "are available): ";
            int o;
            while (true) {
                std::cin >> o;
                if (o == -1) {
                    break;
                }
                wall.AddObject(o, f);
            }
        }
    }

    void SetConflicts() {
        std::cout << "Please, set conflict classes" << '\n';
        for (size_t i = 0; i != wall.GetFirms(); ++i) {
            char f, c;
            std::cout << "Enter a firm\'s letter ";
            std::cout << '(';
            wall.PrintFirms();
            std::cout << "already exist): ";
            std::cin >> f;
            std::cout << "Set a conflict class ";
            std::cout << '(';
            wall.PrintClasses();
            std::cout << "already exist): ";
            std::cin >> c;
            wall.SetConflict(f, c);
        }
    };
    
public:
    void PrintSummary() {
        std::cout << "Commands available:" << '\n';
        std::cout << "start            - erase all subjects\' histories" << '\n';
        std::cout << "read s o         - read object o by subject s" << '\n';
        std::cout << "report subject s - prints objects available for s" << '\n';
        std::cout << "report object o  - prints subjects having access to o" << '\n';
        std::cout << "briefcase f      - prints objects possessed by f" << '\n'; 
    }

    ChineseWallContext(ChineseWall& chineseWall) {
        wall = chineseWall;
        SetPortfolios();
        SetConflicts();
        std::cout << "The system was successfully initialised!" << '\n';
        PrintSummary();
    }

    void ReadCommand(const std::string& command) {
        size_t s, o;
        char f;
        if (command == "start") {
            wall.Start();
        } else if (command == "read") {
            std::cin >> s >> o;
            wall.Read(s, o);
        } else if (command == "write") {
            std::cin >> s >> o;
            wall.Write(s, o);
        } else if (command == "report subject") {
            std::cin >> s;
            wall.ReportS(s);
        } else if (command == "report object") {
            std::cin >> o;
            wall.ReportO(o);
        } else if (command == "briefcase") {
            std::cin >> f;
            wall.Portfolio(f);
        }
    }
};

int main() {
    size_t n, m, f;
    std::cout << "Enter the number of subjects: ";
    std::cin >> n;
    std::cout << "Enter the number of objects: ";
    std::cin >> m;
    std::cout << "Enter the number of firms: ";
    std::cin >> f;
    ChineseWall cw(n, m, f);
    ChineseWallContext context(cw);
    std::cout << '>';
    std::string command;
    while (true) {
        std::getline(std::cin, command);
        if (command.empty()) {
            break;
        }
        context.ReadCommand(command);
        std::cout << '>';
    }
}