#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class CSV_parser{
  public:
    void solution(const std::string &file_name) {
        bool is_first_line_in_file = true;
        int col = 0;
        int row = 0;
        std::string line;
        std::string fst_line;
        try {
            std::ifstream file(file_name);
            if (!file.is_open()) {
                throw std::ifstream::failure("Error opening file!");
            }
            while (std::getline(file, line)) {
                if (is_first_line_in_file) {
                    fst_line = line;
                }
                std::string row_line;
                std::stringstream ss(line);
                bool is_first_element_in_line = true;
                while (std::getline(ss, row_line, ',')) {
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
            process_file();
            print_ans(fst_line);
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() << '\n';
            exit(1);
        }
    }
  private:
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

    void check_trash_after_num(const std::string &line, int &i) {
        skip_space(line, i);
        if (line[i] != '\0') {
            std::cerr << "Bad file format" << '\n';
            exit(1);
        }
    }

    int get_num_from_exact_cell(const std::string &name_cell, int &i) {
        std::string name_column;
        std::string name_row;
        while (!std::isdigit(name_cell[i])) {
            name_column += name_cell[i];
            i++;
        }
        while (std::isdigit(name_cell[i])) {
            name_row += name_cell[i];
            i++;
        }
        check_trash_after_num(name_cell, i);
        if (!map_columns.count(name_column) || !map_rows.count(name_row)) {
            std::cerr << "Bad file format" << '\n';
            exit(1);
        }
        int column_number = map_columns[name_column];
        int row_number = map_rows[name_row];
        int cell_number =
            row_number * static_cast<int>(map_columns.size()) + column_number;
        int number = parse(list_cells[cell_number]);
        return number;
    }

    int parse_expr(const std::string &expression) {
        int i = 0;
        skip_space(expression, i);
        if (expression[i] != '=') {
            std::cerr << "Bad file format" << '\n';
            exit(1);
        }
        i++;
        skip_space(expression, i);
        int first_number = (std::isdigit(expression[i]))
                               ? get_number(expression, i)
                               : get_num_from_exact_cell(expression, i);
        skip_space(expression, i);
        int index_operator = i;
        i++;
        skip_space(expression, i);
        int second_number = (std::isdigit(expression[i]))
                                ? get_number(expression, i)
                                : get_num_from_exact_cell(expression, i);
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
                std::cerr << "Division by zero" << '\n';
                exit(1);
            }
            first_number /= second_number;
            break;
        }
        return first_number;
    }

    int parse(std::string &line) {
        int number;
        int i = 0;
        skip_space(line, i);
        if (std::isdigit(line[i])) {
            number = get_number(line, i);
            check_trash_after_num(line, i);
        } else {
            number = parse_expr(line);
        }
        line = std::to_string(number);
        return number;
    }

    void process_file() {
        for (std::size_t i = 0; i < list_cells.size(); i++) {
            parse(list_cells[i]);
        }
    }

    void print_ans(const std::string &first_line_in_file) {
        std::cout << first_line_in_file << '\n';
        std::size_t count_elements_in_row = map_columns.size();
        for (std::size_t i = 0; i < list_cells.size(); i++) {
            std::cout << list_cells[i];
            if ((i + 1) % count_elements_in_row == 0) {
                std::cout << '\n';
            } else {
                std::cout << ',';
            }
        }
    }

    std::unordered_map<std::string, int> map_columns;
    std::unordered_map<std::string, int> map_rows;
    std::vector<std::string> list_of_column_names;
    std::vector<std::string> list_of_row_names;
    std::vector<std::string> list_cells;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No input file" << '\n';
        return 1;
    }
    CSV_parser my_csv_parser;
    my_csv_parser.solution(argv[1]);
    return 0;
}