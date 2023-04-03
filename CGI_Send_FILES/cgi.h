#pragma once
#include <stdlib.h>
#include <string.h>
bool is_get();
size_t get_content_length();
void get_form_data(char*& data);
void get_param_value(char*& value, const char* param_name, const char* data);
size_t get_file_data(char*& filename, char*& content, const char* data);