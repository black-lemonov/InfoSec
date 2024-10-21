#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <set>
#include <functional>

class ChineseWall {
private:
    size_t subjects;
    size_t objects;
    size_t firms;
    std::vector<std::vector<bool>> accessMatrix;
    std::vector<std::pair<char, char>> securityLabels;

    bool isHistoryEmpty(size_t s) {
        return std::all_of(
            accessMatrix[s].begin(),
            accessMatrix[s].end(),
            std::logical_not<bool>()
        );
    }

    bool hasConflict(size_t s, size_t o) {
        for (size_t i = 0; i != objects; ++i) {
            if (i != o ) {
                if (accessMatrix[s][i] 
                    && securityLabels[i].first == securityLabels[o].first) {
                    return true;
                }
            }
        }
        return false;
    }

    bool sameFirm(size_t s, size_t o) {
        for (size_t i = 0; i != objects; ++i) {
            if (i != o) {
                if (accessMatrix[s][i] 
                    && securityLabels[i].second == securityLabels[o].second) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    static const char NONE;

    ChineseWall() = default;

    ChineseWall(size_t n, size_t m, size_t f) {
        subjects = n;
        objects = m;
        firms = f;
        accessMatrix = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false));
        securityLabels = std::vector<std::pair<char, char>>(m, {NONE, NONE});
    }

    void Start() {
        for (auto& row : accessMatrix) {
            std::replace(row.begin(), row.end(), true, false);
        }
    }

    void Read(size_t s, size_t o) {
        if (accessMatrix[s][o]) {
            std::cout << "accepted";
        } else if (isHistoryEmpty(s) || !hasConflict(s, o) || sameFirm(s, o)) {
            std::cout << "accepted";
            accessMatrix[s][o] = true;
        } else {
            std::cout << "refused";
        }
    }

    void Write(size_t s, size_t o) {
        Read(s, o);
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

    size_t GetFirmsNumber() const {
        return firms;
    }

    size_t GetObjectsNumber() const {
        return objects;
    }

    size_t GetSubjectsNumber() const {
        return subjects;
    }

    char GetFirm(size_t o) const {
        return securityLabels[o].second;
    }

    char GetConflict(size_t o) const {
        return securityLabels[o].first;
    }

    bool GetAccess(size_t s, size_t o) const {
        return accessMatrix[s][o];
    }
};

const char ChineseWall::NONE = '_';


class ChineseWallContext {
private:
    ChineseWall& wall;
    std::istream& in;
    std::ostream& out;

    void PrintSubjectReport(size_t s) const {
        std::string delimiter = "\n";
        for (size_t i = 0, n = wall.GetObjectsNumber(); i != n; ) {
            if (wall.GetAccess(s, i)) {
                out << "object: " << i;
                if (char f = wall.GetFirm(i); f != ChineseWall::NONE) {
                    out << " firm: " << f;
                }
                if (i + 1 != n) {
                    out << delimiter;
                }
            }
            ++i;
        }
    }

    void PrintObjectReport(size_t o) {
        std::string delimiter = ", ";
        for (size_t i = 0, n = wall.GetSubjectsNumber(); i != n; ) {
            if (wall.GetAccess(i, o)) {
                out << i;
                if (i + 1 != n) {
                    out << delimiter;
                }
            }
            ++i;
        }
    }

    void PrintFirmPortfolio(char f) const {
        std::string delimiter = ", ";
        for (size_t i = 0, n = wall.GetObjectsNumber(); i != n; ) {
            if (wall.GetFirm(i) == f) {
                out << i;
                if (i + 1 != n) {
                    out << delimiter;
                }
            }
            ++i;
        }
    }

    void PrintObjectsAvailable() const {
        std::string delimiter = ", ";
        for (size_t i = 0; i != wall.GetObjectsNumber(); ) {
            if (wall.GetFirm(i) == ChineseWall::NONE) {
                out << i;
                if (i + 1 != wall.GetObjectsNumber()) {
                    out << delimiter;
                }
            }
            ++i;
        }
    }
    
    void PrintAllFirms() const {
        std::string noMsg = "no";
        std::string delimiter = ", ";
        std::set<char> letters;
        for (size_t i = 0; i != wall.GetObjectsNumber(); ++i) {
            if (wall.GetFirm(i) != ChineseWall::NONE) {
                letters.insert(wall.GetFirm(i));
            }
        }
        size_t size = letters.size();
        if (size == 0) {
            out << noMsg;
            return;
        }
        auto it = letters.begin();
        for (size_t i = 0; i != size; ) {
            out << *it;
            ++i;
            ++it;
            if (i != size) {
                out << delimiter;
            }
        }
    }

    void PrintAllConflicts() const {
        std::string noMsg = "no";
        std::string delimiter = ", ";
        std::set<char> letters;
        for (size_t i = 0; i != wall.GetObjectsNumber(); ++i) {
            if (wall.GetConflict(i) != ChineseWall::NONE) {
                letters.insert(wall.GetConflict(i));
            }
        }
        size_t conflicts = letters.size();
        if (conflicts == 0) {
            out << noMsg;
            return;
        }
        auto it = letters.begin();
        for (size_t i = 0; i != conflicts; ) {
            out << *it;
            ++i;
            ++it;
            if (i != conflicts) {
                out << delimiter;
            }
        }
    }

    void PrintAllSubjects() const {
        std::string delimiter = ", ";
        for (size_t i = 0; i != wall.GetSubjectsNumber(); ) {
            out << i;
            ++i;
            if (i != wall.GetSubjectsNumber()) {
                out << delimiter;
            }
        }
    }

    void SetPortfolios() {
        out << "Please, set firms portfolios" << '\n';
        for (size_t i = 0; i != wall.GetFirmsNumber(); ++i) {
            char f;
            out << "Enter firm\'s letter (";
            PrintAllFirms();
            out << " firm(s) already exist): ";
            in >> f;
            out << "Enter firms object(s) (";
            PrintObjectsAvailable();
            out << " are available): ";
            int o;
            while (true) {
                in >> o;
                if (o == -1) {
                    break;
                }
                wall.AddObject(o, f);
            }
        }
    }
    void SetConflicts() {
        out << "Please, set conflict classes" << '\n';
        for (size_t i = 0; i != wall.GetFirmsNumber(); ++i) {
            char f, c;
            out << "Enter a firm\'s letter ";
            out << '(';
            PrintAllFirms();
            out << " already exist): ";
            in >> f;
            out << "Set a conflict class ";
            out << '(';
            PrintAllConflicts();
            out << " already exist): ";
            in >> c;
            wall.SetConflict(f, c);
        }
    }

    void Help() const {
        out << "Subjects in the system: ";
        PrintAllSubjects();
        out << '\n';
        out << "Commands available:" << '\n';
        out << "start            - erase all subjects\' histories" << '\n';
        out << "read s o         - read object o by subject s" << '\n';
        out << "reportsub s      - prints objects available for s" << '\n';
        out << "reportobj o      - prints subjects having access to o" << '\n';
        out << "briefcase f      - prints objects possessed by f" << '\n';
        out << "exit             - exit the program" << '\n'; 
    }

    void ReadCommand(const std::string& command) {
        size_t s, o;
        char f;
        if (command == "help") {
            Help();
        } else if (command == "start") {
            wall.Start();
        } else if (command == "read") {
            in >> s >> o;
            wall.Read(s, o);
            out << '\n';
        } else if (command == "write") {
            in >> s >> o;
            wall.Write(s, o);
            out << '\n';
        } else if (command == "reportsub") {
            in >> s;
            PrintSubjectReport(s);
            out << '\n';
        } else if (command == "reportobj") {
            in >> o;
            PrintObjectReport(o);
            out << '\n';
        } else if (command == "briefcase") {
            in >> f;
            PrintFirmPortfolio(f);
            out << '\n';
        } else {
            out << '\n';
        }
    }
    
public:
    ChineseWallContext(ChineseWall& chineseWall, 
                        std::istream& in_, 
                        std::ostream& out_) : wall(chineseWall), in(in_), out(out_) {   
        SetPortfolios();
        SetConflicts();
        out << "The system was successfully initialised!" << '\n';
        out << "Enter 'help' to get the list of all available commands." << '\n';
    }

    ChineseWallContext(ChineseWall& chineseWall,
                       std::istream& in_) : ChineseWallContext(chineseWall, in_, std::cout) {}

    ChineseWallContext(ChineseWall& chineseWall,
                       std::ostream& out_) : ChineseWallContext(chineseWall, std::cin, out_) {}

    ChineseWallContext(ChineseWall& chineseWall) : ChineseWallContext(chineseWall, std::cin, std::cout) {}

    void Run(const std::string& cursor) {
        std::string command;
        while (true) {
            out << cursor << ' ';
            in >> command;
            if (command == "exit") {
                break;
            }
            ReadCommand(command);
        }  
    };
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
    context.Run("sit~$");
}