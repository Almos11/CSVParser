#include "parser.h"

void CSV_parser::print_csv_file() {
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

void CSV_parser::handler(const std::string &file_name) {
    read_the_file(file_name);
    process_file();
}

void CSV_parser::read_the_file(const std::string &file_name) {
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
        std::size_t count_elements_in_line = 0;
        std::string row_line;
        std::stringstream ss(line);
        bool is_first_element_in_line = true;
        while (std::getline(ss, row_line, ',')) {
            if (is_first_line_in_file) {
                if (map_columns.count(row_line)) {
                    file.close();
                    throw std::runtime_error(
                        "Bad file format. Incorrect form of the table!");
                }
                map_columns[row_line] = col++;
            } else {
                if (is_first_element_in_line) {
                    if (map_rows.count(row_line)) {
                        file.close();
                        throw std::runtime_error(
                            "Bad file format. Incorrect form of the table!");
                    }
                    map_rows[row_line] = row++;
                    is_first_element_in_line = false;
                }
            }
            list_cells.push_back(row_line);
            count_elements_in_line++;
        }
        if (count_elements_in_line != map_columns.size()) {
            throw std::runtime_error("Bad file format. File format does not match CSV format!");
        }
        is_first_line_in_file = false;
    }
    file.close();
}

int CSV_parser::get_number(const std::string &line, int &i) {
    int negative_number = (line[i] == '-') ? -1 : 1;
    if (negative_number == -1) {
        i++;
    }
    int number = 0;
    while (std::isdigit(line[i])) {
        number *= 10;
        number += line[i++] - '0';
    }
    return number * negative_number;
}

void CSV_parser::skip_space(const std::string &line, int &pointer) {
    while (std::isspace(line[pointer])) {
        pointer++;
    }
}

void CSV_parser::check_trash_after_expression(const std::string &line,
                                              int &pointer) {
    skip_space(line, pointer);
    if (line[pointer] != '\0') {
        throw std::runtime_error(
            "Bad file format. Wrong expression in a cell!");
    }
}

int CSV_parser::get_number_from_exact_cell(const std::string &name_cell,
                                           int &pointer) {
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
        throw std::runtime_error(
            "Bad file format. Calling a non-existent cell!");
    }
    int column_number = map_columns[name_column];
    int row_number = map_rows[name_row];
    int cell_number =
        (row_number + 1) * static_cast<int>(map_columns.size()) + column_number;
    int number = parser(list_cells[cell_number]);
    return number;
}

int CSV_parser::expression_parser(const std::string &expression) {
    int pointer = 0;
    skip_space(expression, pointer);
    if (expression[pointer] != '=') {
        throw std::runtime_error(
            "Bad file format. Unknown character at the beginning!");
    }
    pointer++;
    skip_space(expression, pointer);
    int first_number = (can_be_a_number(expression, pointer))
                           ? get_number(expression, pointer)
                           : get_number_from_exact_cell(expression, pointer);
    skip_space(expression, pointer);
    int index_operator = pointer;
    pointer++;
    skip_space(expression, pointer);
    int second_number = (can_be_a_number(expression, pointer))
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
            throw std::runtime_error("Bad file format. Division by zero!");
        }
        first_number /= second_number;
        break;
    default:
        throw std::runtime_error("Bad file format. Unknown operator!");
    }
    return first_number;
}

bool CSV_parser::can_be_a_number(const std::string &line, int &pointer) {
    return (std::isdigit(line[pointer]) || line[pointer] == '-');
}

int CSV_parser::parser(std::string &line) {
    int number;
    int pointer = 0;
    skip_space(line, pointer);
    if (can_be_a_number(line, pointer)) {
        number = get_number(line, pointer);
        check_trash_after_expression(line, pointer);
    } else {
        number = expression_parser(line);
    }
    line = std::to_string(number);
    return number;
}

void CSV_parser::process_file() {
    for (std::size_t i = map_columns.size(); i < list_cells.size(); i++) {
        parser(list_cells[i]);
    }
}