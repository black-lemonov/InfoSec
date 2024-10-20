#include <iostream>
#include <vector>


class ComputerSystem {
private:
    size_t subjectsNumber, objectsNumber;
    std::vector<std::vector<char>> accessMatrix;
    std::vector<int> subjectsLevels;
    std::vector<int> objectsLevels;
public:
    ComputerSystem(size_t n, size_t m, std::vector<std::vector<char>> A, std::vector<int> LS, std::vector<int> LO) {
        subjectsNumber = n;
        objectsNumber = m;
        accessMatrix = A;
        subjectsLevels = LS;
        objectsLevels = LO;
    }

    int GetObjectLevel(size_t object) const {
        return objectsLevels[object - 1];   
    }

    int GetSubjectLevel(size_t subject) const {
        return subjectsLevels[subject - 1];
    }

    char GetAccess(size_t subject, size_t object) const {
        return accessMatrix[subject - 1][object - 1];
    }

    size_t subjects() const {
        return subjectsNumber;
    }    

    size_t objects() const {
        return objectsNumber;
    }

    bool IsSafe() const;
};


template <typename T>
void EnterArray(size_t length, std::vector<T>& saveTo) {
    saveTo.clear();
    saveTo.reserve(length);
    while (length--) {
        T el;
        std::cin >> el; 
        saveTo.push_back(el);
    }
}


template <typename T>
void EnterMatrix(size_t rows, size_t columns, std::vector<std::vector<T>>& saveTo) {
    saveTo.clear();
    saveTo.reserve(rows);
    while (rows--) {
        std::vector<T> row;
        EnterArray(columns, row);
        saveTo.push_back(row);
    }
}


template <typename T>
void PrintMatrix(const std::vector<std::vector<T>> matrix) {
    for (size_t i = 0; i != matrix.size(); ++i) {
        for (const T& el : matrix[i]) {
            std::cout << el << ' ';
        }
        std::cout << '\n';
    }
}

// Критерий безопасности системы:
// безопасны все достижимые состояния системы
// (состояние безопасно если выполнено NRU и NWD)
bool ComputerSystem::IsSafe() const {
    for (size_t s = 1; s != subjects() + 1; ++s) {
        for (size_t o = 1; o != objects() + 1; ++o) {
            char access = GetAccess(s, o);
            if (access == 'r') {
                if (GetSubjectLevel(s) < GetObjectLevel(o)) {
                    return false;
                }
            } else if (access == 'w') {
                if (GetSubjectLevel(s) > GetObjectLevel(o)) {
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    return true;
}


int main() {
    size_t n, m; // кол-во субъектов и кол-во объектов
    std::cout << "Enter the number of subjects and the number of objects: " << '\n';
    std::cin >> n >> m;
    std::vector<std::vector<char>> A;
    std::cout << "Enter the elements of access matrix: " << '\n';
    EnterMatrix(n, m, A);
    PrintMatrix(A);
    std::vector<int> LS;
    std::cout << "Enter subjects levels: ";
    EnterArray(n, LS);
    std::vector<int> LO;
    std::cout << "Enter objects levels: ";
    EnterArray(m, LO);
    ComputerSystem cs(n, m, A, LS, LO);
    if (cs.IsSafe()) {
        std::cout << "Congrats! Your system is safe!" << '\n';
    } else {
        std::cout << "Warning! Your system is not safe!" << '\n';
    }
}