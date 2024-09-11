#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

// Написать программу, которая по заданному входному
// текстовому файлу input.txt формирует результирующий текстовый файл
// output.txt. Содержимое входного файла – последовательность, разделенных
// одним или несколькими пробелами зеркально отраженных слов. Требуется в
// той же последовательности вывести в output.txt результаты зеркального
// отображения этих слов. Например, файл input.txt: отЭ ремипр оготсорп атсет.
// илсЕ ыВ еще ен иляноп, от етишипаз ывкуб огоджак аволс в монтарбо
// екдяроп.
// Файл output.txt: Это пример простого теста. Если Вы еще не поняли, то
// запишите буквы каждого слова в обратном порядке


/** Возвращает зеркальное отражение строки */
std::wstring Mirror(const std::wstring& str) {
    std::wstring mirrored_str;
    wchar_t punctuation = L'\0';
    for (int i = str.length() - 1; i >= 0; --i) {
        if (str[i] == L',' || str[i] == L'.' || str[i] == L'!' || str[i] == L'?') {
            punctuation = str[i];
        } else {
            mirrored_str += str[i];
        }
    }
    return mirrored_str + punctuation;
}

int main() {
    std::wifstream fin(L"files/input.txt");
    std::wofstream fout(L"files/output.txt");
    
    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
    fout.imbue(std::locale(fout.getloc(), new std::codecvt_utf8<wchar_t>));
    
    if (fin.is_open() && fout.is_open()) {
        fout << L"\xFEFF";

        std::wstring word;
        while (fin >> word) {
            fout << Mirror(word);
        }
    } 
    fin.close();
    fout.close();
}
