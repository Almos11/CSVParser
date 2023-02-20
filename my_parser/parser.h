#ifndef MY_PARSER_PARSER_H
#define MY_PARSER_PARSER_H

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
    void print_csv_file();
    void handler(const std::string &file_name);

  private:
    void read_the_file(const std::string &file_name);
    static int get_number(const std::string &line, int &i);
    static void skip_space(const std::string &line, int &pointer);
    static void check_trash_after_expression(const std::string &line, int &pointer);
    int get_number_from_exact_cell(const std::string &name_cell, int &pointer);
    int expression_parser(const std::string &expression);
    int parser(std::string &line);
    void process_file();

    std::map<std::string, int> map_columns;
    std::unordered_map<std::string, int> map_rows;
    std::vector<std::string> list_of_column_names;
    std::vector<std::string> list_of_row_names;
    std::vector<std::string> list_cells;
};

#endif  // MY_PARSER_PARSER_H
