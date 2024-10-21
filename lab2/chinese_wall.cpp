// Можно сделать консольным приложением bash
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <set>
#include <functional>
#include <sstream>

class ChineseWall {
private:
    size_t subjects;
    size_t objects;
    size_t firms;
    std::vector<std::vector<bool>> accessMatrix;
    std::vector<std::pair<char, char>> securityLabels;

    bool IsHistoryEmpty(size_t s) {
        return std::all_of(
            accessMatrix[s].begin(),
            accessMatrix[s].end(),
            std::logical_not<bool>()
        );
    }

    bool HasConflict(size_t s, size_t o) {
        for (size_t i = 0; i != objects; ++i) {
            if (i != o) {
                if (HasAccess(s, i) && (GetConflict(i) == GetConflict(o))) {
                    return true;
                }
            }
        }
        return false;
    }

    bool SameFirm(size_t s, size_t o) {
        for (size_t i = 0; i != objects; ++i) {
            if (i != o) {
                if (HasAccess(s, i) && (GetFirm(i) == GetFirm(o))) {
                    return true;
                }
            }
        }
        return false;
    }

public:
    static const char NONE;

    ChineseWall() = default;

    ChineseWall(size_t n, size_t m, size_t f) : subjects(n), objects(m), firms(f) {
        accessMatrix = std::vector<std::vector<bool>>(n, std::vector<bool>(m, false));
        securityLabels = std::vector<std::pair<char, char>>(m, {NONE, NONE});
    }

    void Start() {
        for (auto& row : accessMatrix) {
            std::replace(row.begin(), row.end(), true, false);
        }
    }

    bool Read(size_t s, size_t o) {
        if (accessMatrix[s][o]) {
            return true;
        }
        if (IsHistoryEmpty(s) || !HasConflict(s, o) || SameFirm(s, o)) {
            accessMatrix[s][o] = true;
            return true;
        } 
        return false;
    }

    bool Write(size_t s, size_t o) {
        return Read(s, o);
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

    inline size_t GetFirmsNumber() const {
        return firms;
    }

    inline size_t GetObjectsNumber() const {
        return objects;
    }

    // Число объектов без владельца
    inline size_t GetFreeObjectsNumber() const {
        return std::count_if(
            securityLabels.begin(),
            securityLabels.end(),
            [](const auto& p){
                return p.second == NONE;
            }
        );
    }

    inline size_t GetSubjectsNumber() const {
        return subjects;
    }

    inline char GetFirm(size_t o) const {
        return securityLabels[o].second;
    }

    inline char GetConflict(size_t o) const {
        return securityLabels[o].first;
    }

    inline bool HasAccess(size_t s, size_t o) const {
        return accessMatrix[s][o];
    }

    // Число объектов к которым имел доступ субъект s
    size_t GetObjects(size_t s) const {
        return std::count(accessMatrix[s].begin(), accessMatrix[s].end(), true);
    }

    // Число субъектов обращавшихся к объекту o
    size_t GetSubjects(size_t o) const {
        size_t count = 0;
        for (size_t i = 0; i != subjects; ++i) {
            if (accessMatrix[i][o]) {
                ++count;
            }
        }
        return count;
    }

    // Число объектов в портфеле компании
    size_t GetFirmObjects(char f) const {
        return std::count_if(securityLabels.begin(),
                          securityLabels.end(),
                          [&](const auto& p){
                            return p.second == f;
                          });
    }
};

const char ChineseWall::NONE = '_';


class ChineseWallContext {
private:
    ChineseWall wall;
    std::istream& in;
    std::ostream& out;

    void PrintSubjectReport(size_t s) const {
        std::string delimiter = "\n";
        for (size_t i = 0, n = wall.GetObjects(s); n != 0; ++i) {
            if (wall.HasAccess(s, i)) {
                out << "object: " << i;
                if (char f = wall.GetFirm(i); f != ChineseWall::NONE) {
                    out << " firm: " << f;
                }
                --n;
                if (n != 0) {
                    out << delimiter;
                }
            }
        }
    }

    void PrintObjectReport(size_t o) {
        std::string delimiter = "\n";
        for (size_t i = 0, n = wall.GetSubjects(o); n != 0; ++i) {
            if (wall.HasAccess(i, o)) {
                out << "subject: " << i;
                --n;
                if (n != 0) {
                    out << delimiter;
                }
            }
        }
    }

    void PrintFirmPortfolio(char f) const {
        std::string delimiter = "\n";
        for (size_t i = 0, n = wall.GetFirmObjects(f); i != n; ++i) {
            if (wall.GetFirm(i) == f) {
                out << "object: " << i;
                --n;
                if (n != 0) {
                    out << delimiter;
                }
            }
        }
    }

    void Help() const {
        out << "Commands available:" << '\n';
        out << "start       - erase all subjects\' histories" << '\n';
        out << "write s o   - write into object o by subject s" << '\n';
        out << "read s o    - read object o by subject s" << '\n';
        out << "report -s s - prints objects available for s" << '\n';
        out << "report -o o - prints subjects having access to o" << '\n';
        out << "briefcase f - prints objects possessed by f" << '\n';
        out << "exit        - exit the program" << '\n'; 
    }

    void ReadCommand(const std::string& command) {
        size_t s, o;
        char f;
        std::string flag;
        if (command == "help") {
            Help();
        } else if (command == "start") {
            wall.Start();
        } else if (command == "read") {
            in >> s >> o;
            out << (wall.Read(s, o)? "accepted" : "refused");
            out << '\n';
        } else if (command == "write") {
            in >> s >> o;
            out << (wall.Write(s, o)? "accepted" : "refused");
            out << '\n';
        } else if (command == "report") {
            in >> flag;
            if (flag == "-s") {
                in >> s;
                PrintSubjectReport(s);
            } else if (flag == "-o") {
                in >> o;
                PrintObjectReport(o);
            } else {
                out << "flags: -s for subject, -o for object";
            }
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
    ChineseWallContext(ChineseWall chineseWall, 
                        std::istream& in_, 
                        std::ostream& out_) : wall(chineseWall), in(in_), out(out_) {
        out << "Enter 'help' to get the list of all available commands." << '\n';
    }

    ChineseWallContext(ChineseWall chineseWall,
                       std::istream& in_) : ChineseWallContext(chineseWall, in_, std::cout) {}

    ChineseWallContext(ChineseWall chineseWall,
                       std::ostream& out_) : ChineseWallContext(chineseWall, std::cin, out_) {}

    ChineseWallContext(ChineseWall chineseWall) : ChineseWallContext(chineseWall, std::cin, std::cout) {}

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


class WallBuilder {
private:
    std::istream& in;
    std::ostream& out;
    
    void PrintObjectsAvailable(ChineseWall& wall) {
        std::string delimiter = ", ";
        for (size_t i = 0, n = wall.GetFreeObjectsNumber(); n != 0; ++i) {
            if (wall.GetFirm(i) == ChineseWall::NONE) {
                out << i;
                --n;
                if (n != 0) {
                    out << delimiter;
                }
            }
        }
    }

    void PrintAllFirms(ChineseWall& wall) {
        std::string noMsg = "no";
        std::string delimiter = ", ";
        std::set<char> letters;
        for (size_t i = 0; i != wall.GetObjectsNumber(); ++i) {
            if (wall.GetFirm(i) != ChineseWall::NONE) {
                letters.insert(wall.GetFirm(i));
            }
        }
        if (letters.empty()) {
            out << noMsg;
            return;
        }
        for (auto it = letters.begin(); it != letters.end(); ) {
            out << *it;
            ++it;
            if (it != letters.end()) {
                out << delimiter;
            }
        }
    }

    void PrintAllConflicts(ChineseWall& wall) {
        std::string noMsg = "no";
        std::string delimiter = ", ";
        std::set<char> letters;
        for (size_t i = 0; i != wall.GetObjectsNumber(); ++i) {
            if (wall.GetConflict(i) != ChineseWall::NONE) {
                letters.insert(wall.GetConflict(i));
            }
        }
        if (letters.empty()) {
            out << noMsg;
            return;
        }
        for (auto it = letters.begin(); it != letters.end(); ) {
            out << *it;
            ++it;
            if (it != letters.end()) {
                out << delimiter;
            }
        }
    }

    void PrintAllSubjects(ChineseWall& wall) {
        std::string delimiter = ", ";
        for (size_t i = 0, n = wall.GetSubjectsNumber(); i != n; ) {
            out << i;
            ++i;
            if (i != n) {
                out << delimiter;
            }
        }
    }

    void SetPortfolio(ChineseWall& wall, char f) {
        out << "Enter firms object(s) (";
        PrintObjectsAvailable(wall);
        out << " available): ";
        in >> std::ws;
        std::string objects;
        std::getline(in, objects);
        std::istringstream iss(objects);
        size_t o;
        while (iss >> o) {
            wall.AddObject(o, f);
        }
    }

    void SetPortfolios(ChineseWall& wall) {
        out << "Please, set firms portfolios" << '\n';
        for (size_t i = 0; i != wall.GetFirmsNumber(); ++i) {
            char f;
            out << "Enter firm\'s letter (";
            PrintAllFirms(wall);
            out << " exist(s)): ";
            in >> f;    
            SetPortfolio(wall, f);
        }
    }

    void SetConflict(ChineseWall& wall, char f) {
        char c;
        out << "Set a conflict class ";
        out << '(';
        PrintAllConflicts(wall);
        out << " exist(s)): ";
        in >> c;
        wall.SetConflict(f, c);
    }

    void SetConflicts(ChineseWall& wall) {
        out << "Please, set conflict classes" << '\n';
        for (size_t i = 0; i != wall.GetFirmsNumber(); ++i) {
            char f;
            out << "Enter a firm\'s letter ";
            out << '(';
            PrintAllFirms(wall);
            out << " exist(s)): ";
            in >> f;
            SetConflict(wall, f);
        }
    }

public:
    WallBuilder(std::istream& in_, std::ostream& out_) : in(in_), out(out_) {}

    ChineseWall BuildWall() {
        size_t n, m, f;
        out << "Enter the number of subjects: ";
        in >> n;
        out << "Enter the number of objects: ";
        in >> m;
        out << "Enter the number of firms: ";
        in >> f;
        ChineseWall wall(n, m, f);
        SetPortfolios(wall);
        SetConflicts(wall);
        out << "The system was successfully initialised!" << '\n';
        return wall;
    }; 
};


int main() {    
    WallBuilder builder(std::cin, std::cout);
    ChineseWall wall = builder.BuildWall();
    ChineseWallContext context(wall);
    context.Run("<3");
}