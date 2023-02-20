#include "parser.h"

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            throw std::runtime_error("File not found!");
        }
        CSV_parser my_csv_parser;
        my_csv_parser.handler(argv[1]);
        my_csv_parser.print_csv_file();
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    return 0;
}