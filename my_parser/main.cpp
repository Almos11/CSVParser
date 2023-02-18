#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_map>
#include <vector>

struct Data {
    std::unordered_map<std::string , int> map_columns;
    std::vector<std::string> list_columns;
    std::vector<std::string> list_rows;
    std::unordered_map<std::string, int> map_rows;
    std::vector<std::string> list_cells;
    std::vector<int> ans_list_cells;
    Data () {
        list_cells = {};
        map_columns = {};
        map_rows = {};
    }
};

int parse(const std::string& value, Data* data);

int get_number(const std::string& value, int i) {
    int number = 0;
    while (std::isdigit(value[i])) {
        number *= 10;
        number += value[i++] - '0';
    }
    return number;
}

int skip_space(const std::string& value, int pointer) {
    while(std::isspace(value[pointer])) {
        pointer++;
    }
    return pointer;
}

int get_num_cell(const std::string& value, int& i, Data *data) {
    std::string name_col;
    std::string name_row;
    while (!std::isdigit(value[i])) {
        name_col += value[i];
        i++;
    }
    while (std::isdigit(value[i])) {
        name_row += value[i];
        i++;
    }
    int num_col = data->map_columns[name_col];
    int num_row = data->map_rows[name_row];
    int num_cell = num_row * (int) data->map_columns.size() + num_col;
    int num = parse(data->list_cells[num_cell], data);
    data->list_cells[num_cell] = std::to_string(num);
    return num;
}

int parse_expr(const std::string& value, Data *data) {
    int index_operator;
    int num_1 = 0;
    int num_2 = 0;
    int i = 0;
    i = skip_space(value, i);
    if (value[i] != '=') {
        return 0;
    }
    i++;
    i = skip_space(value, i);
    if (std::isdigit(value[i])) {
        num_1 = get_number(value, i);
        i++;
    } else {
        num_1 = get_num_cell(value, i, data);
    }
    i = skip_space(value, i);
    index_operator = i;
    i++;
    i = skip_space(value, i);
    if (std::isdigit(value[i])) {
        num_2 = get_number(value, i);
    } else {
        num_2 = get_num_cell(value, i, data);
    }
    switch (value[index_operator]) {
        case '+':
            num_1 += num_2;
            break;
        case '-':
            num_1 -= num_2;
            break;
        case '*':
            num_1 *= num_2;
            break;
        case '/':
            num_1 /= num_2;
            break;
    }
    return num_1;

}

int parse(const std::string& value, Data* data) {
    int number = 0;
    int i = 0;
    i = skip_space(value, i);
    if (std::isdigit(value[i])) {
        number = get_number(value, i);
    } else {
        number = parse_expr(value, data);
    }
    return number;
}

void process_file (Data *data) {
    std::size_t count_el_in_row = data->map_columns.size();
    for (size_t i = 0; i < data->list_cells.size(); i++) {
        data->ans_list_cells.push_back(parse(data->list_cells[i], data));
        std::cout << data->ans_list_cells[i];
        if ((i + 1) % count_el_in_row == 0) {
            std::cout << '\n';
        } else {
            std::cout << ',';
        }
    }
}

void solution(const std::string& file_name) {
    bool is_fst_line = true;
    int col = 0;
    bool is_fst_el_in_line = true;
    int row = 0;
    Data data;
    std::ifstream file(file_name);
    std::string line;
    while (std::getline(file, line)) {
        if (is_fst_line) {
            std::cout << line << '\n';
        }
        std::string value;
        std::stringstream ss(line);
        is_fst_el_in_line = true;
        while (std::getline(ss, value, ',')) {
            if (is_fst_line) {
                data.map_columns[value] = col++;
            } else {
                if (is_fst_el_in_line) {
                    data.map_rows[value] = row++;
                    is_fst_el_in_line = false;
                }
                data.list_cells.push_back(value);
            }
        }
        is_fst_line = false;
    }
    file.close();
    process_file(&data);
}

int main(int argc, char *argv[]) {
    solution(argv[1]);
    return 0;
}