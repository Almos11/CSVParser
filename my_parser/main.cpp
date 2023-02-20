#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class CSV_parser {
  public:
    void print_csv_file() {
        for (const auto &key : map_columns) {
            std::cout << ',' << key.first;
        }
        std::cout << '\n';
        std::size_t count_elements_in_row = map_columns.size() + 1;
        for (std::size_t i = 0; i < list_cells.size(); i++) {
            std::cout << list_cells[i];
            if ((i + 1) % count_elements_in_row == 0) {
                std::cout << '\n';
            } else {
                std::cout << ',';
            }
        }
    }

    void handler(const std::string &file_name) {
        read_the_file(file_name);
        process_file();
    }

  private:
    void read_the_file(const std::string &file_name) {
        bool is_first_line_in_file = true;
        int col = 0;
        int row = 0;
        std::string line;
        std::string fst_line;
        std::ifstream file(file_name);
        if (!file.is_open()) {
            throw std::runtime_error("File not found!");
        }
        while (std::getline(file, line)) {
            std::string row_line;
            std::stringstream ss(line);
            bool is_first_element_in_line = true;
            while (std::getline(ss, row_line, ',')) {
                if (is_first_line_in_file && is_first_element_in_line) {
                    is_first_element_in_line = false;
                    col++;
                    continue;
                }
                if (is_first_line_in_file) {
                    map_columns[row_line] = col++;
                } else {
                    if (is_first_element_in_line) {
                        map_rows[row_line] = row++;
                        is_first_element_in_line = false;
                    }
                    list_cells.push_back(row_line);
                }
            }
            is_first_line_in_file = false;
        }
        file.close();
    }

    int get_number(const std::string &line, int &i) {
        int number = 0;
        while (std::isdigit(line[i])) {
            number *= 10;
            number += line[i++] - '0';
        }
        return number;
    }

    void skip_space(const std::string &line, int &pointer) {
        while (std::isspace(line[pointer])) {
            pointer++;
        }
    }

    void check_trash_after_expression(const std::string &line, int &pointer) {
        skip_space(line, pointer);
        if (line[pointer] != '\0') {
            throw std::runtime_error("Bad file format!");
        }
    }

    int get_number_from_exact_cell(const std::string &name_cell, int &pointer) {
        std::string name_column;
        std::string name_row;
        while (!std::isdigit(name_cell[pointer])) {
            name_column += name_cell[pointer];
            pointer++;
        }
        while (std::isdigit(name_cell[pointer])) {
            name_row += name_cell[pointer];
            pointer++;
        }
        if (!map_columns.count(name_column) || !map_rows.count(name_row)) {
            throw std::runtime_error("Bad file format!");
        }
        int column_number = map_columns[name_column];
        int row_number = map_rows[name_row];
        int cell_number =
            row_number * static_cast<int>(map_columns.size() + 1) +
            column_number;
        int number = parser(list_cells[cell_number]);
        return number;
    }

    int expression_parser(const std::string &expression) {
        int pointer = 0;
        skip_space(expression, pointer);
        if (expression[pointer] != '=') {
            throw std::runtime_error("Bad file format!");
        }
        pointer++;
        skip_space(expression, pointer);
        int first_number =
            (std::isdigit(expression[pointer]))
                ? get_number(expression, pointer)
                : get_number_from_exact_cell(expression, pointer);
        skip_space(expression, pointer);
        int index_operator = pointer;
        pointer++;
        skip_space(expression, pointer);
        int second_number =
            (std::isdigit(expression[pointer]))
                ? get_number(expression, pointer)
                : get_number_from_exact_cell(expression, pointer);
        check_trash_after_expression(expression, pointer);
        switch (expression[index_operator]) {
        case '+':
            first_number += second_number;
            break;
        case '-':
            first_number -= second_number;
            break;
        case '*':
            first_number *= second_number;
            break;
        case '/':
            if (second_number == 0) {
                throw std::runtime_error("Division by zero!");
            }
            first_number /= second_number;
            break;
        }
        return first_number;
    }

    int parser(std::string &line) {
        int number;
        int pointer = 0;
        skip_space(line, pointer);
        if (std::isdigit(line[pointer])) {
            number = get_number(line, pointer);
            check_trash_after_expression(line, pointer);
        } else {
            number = expression_parser(line);
        }
        line = std::to_string(number);
        return number;
    }

    void process_file() {
        for (auto &list_cell : list_cells) {
            parser(list_cell);
        }
    }

    std::map<std::string, int> map_columns;
    std::unordered_map<std::string, int> map_rows;
    std::vector<std::string> list_of_column_names;
    std::vector<std::string> list_of_row_names;
    std::vector<std::string> list_cells;
};

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