// SoundProcessor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include "Interpreter.h"


void help() {
    std::cout << "Sound processor" << std::endl;
    std::cout << "Usage: sound_processor -c <config.txt> <output.wav> <input1.wav> [<input2.wav> …]" << std::endl;
    std::cout << "Found processors:" << std::endl;
    try {
        for (const auto& entry : std::filesystem::directory_iterator("config")) {
            std::ifstream f(entry.path());
            std::cout << "------------------------" << std::endl;
            while (!f.eof()) {
                std::string line;
                std::getline(f, line);
                std::cout << line << std::endl;
            }
        }
    }
    catch (std::filesystem::filesystem_error e) {
        std::cout << "config folder not found" << std::endl;
    }
}

int main(int argc, char** argv)
{
    if (argc > 1) {
        std::string cmd(argv[1]);
        if (cmd == "-c" && argc > 3) {
            std::vector<Wav>* input = new std::vector<Wav>;
            std::string config(argv[2]);
            std::string out(argv[3]);
            for (int i = 4; i < argc; i++) {
                input->push_back(Wav(std::string(argv[i])));
            }
            Interpreter inter(input, out);
            inter.process(config);
        }
        else if (cmd == "-h") {
            help();
        }
        else {
            std::cerr << "invalid flag. type \"help\" to more info" << std::endl;
            throw "invalid flag";
        }
    }
    else {
        std::cerr << "command line args not found" << std::endl;
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
