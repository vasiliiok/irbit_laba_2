#include <iostream>
#include <ctime>
#include "big_integer.h"

int main() {
    BigInt num_2("100000000000000000000000000000000000");
    BigInt num_1("10000000000000000000000000");

    num_2 / num_1;
}

/*int main() {
    BigInt num_1("111111111111111111111");
    BigInt num_2("2222222222222222222222222222222222");


    // Сложение
    std::cout << num_1 << " + " << num_2 << " = ";
    std::cout << num_1 + num_2 << std::endl;

    std::cout << num_2 << " + " << num_1 << " = ";
    std::cout << num_2 + num_1 << std::endl;

    std::cout << std::endl;
    // Сложение


    // Вычитание
    std::cout << num_1 << " - " << num_2 << " = ";
    std::cout << num_1 - num_2 << std::endl;

    std::cout << num_2 << " - " << num_1 << " = ";
    std::cout << num_2 - num_1 << std::endl;

    std::cout << std::endl;
    // Вычитание


    // Умножение
    std::cout << num_1 << " * " << num_2 << " = ";
    std::cout << num_1 * num_2 << std::endl;

    std::cout << num_2 << " * " << num_1 << " = ";
    std::cout << num_2 * num_1 << std::endl;

    std::cout << std::endl;
    // Умножение


    // Деление
    std::cout << num_1 << " / " << num_2 << " = ";
    std::cout << num_1 / num_2 << std::endl;

    std::cout << num_2 << " / " << num_1 << " = ";
    std::cout << num_2 / num_1 << std::endl;

    std::cout << std::endl;
    // Деление


    // Остаток от деления
    std::cout << num_1 << " % " << num_2 << " = ";
    std::cout << num_1 % num_2 << std::endl;

    std::cout << num_2 << " % " << num_1 << " = ";
    std::cout << num_2 % num_1 << std::endl;

    std::cout << std::endl;
    // Остаток от деления
}*/




/*int main() {
    //---------------------------------
    uint64_t start_time = std::clock();
    //---------------------------------

    BigInt num_1("876567890987679");

    //---------------------------------
    std::cout << "Conversion from string to 2^32 base takes: " << std::clock() - start_time << " milliseconds" << std::endl;
    //---------------------------------

    std::cout << num_1;
    return 0;
}*/
