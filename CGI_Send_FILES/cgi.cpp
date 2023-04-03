#include "cgi.h"

#include <iostream>
#include <stdio.h>

bool is_get()
{
	size_t realsize;
	const size_t buf_sz = 5;
	char* value = new char[buf_sz];
	getenv_s(&realsize, value, buf_sz, "REQUEST_METHOD");
	bool res = !_strcmpi(value, "GET");
	delete[] value;
	return res;
}

size_t get_content_length()
{
	size_t realsize;
	char* value = new char[11];
	getenv_s(&realsize, value, 11, "CONTENT_LENGTH");
	size_t size;
	if (!realsize) size = 0;
	else sscanf_s(value, "%lld", &size);
	delete[] value;
	return size;
}


void get_form_data(char*& data)
{
	delete[] data;
	if (is_get())
	{
		size_t realsize;
		const size_t buf_sz = 65536;
		char* value = new char[buf_sz];
		getenv_s(&realsize, value, buf_sz, "QUERY_STRING");
		data = new char[realsize + 1];
		strcpy_s(data, realsize + 1, value);
		delete[] value;
	}
	else
	{
		size_t buf_size = get_content_length();
		data = new char[buf_size + 1];
		fread_s(
			data,
			buf_size + 1,
			sizeof(char),
			buf_size,
			stdin
		);
		data[buf_size] = 0;
	}
}

void str_decode(char*& dec_str, const char* enc_str)
{
	char* res = new char[strlen(enc_str) + 1];
	int i = 0, j = 0;
	while (enc_str[i])
	{
		if (enc_str[i] == '+')
		{
			res[j] = ' ';
		}
		else
		{
			if (enc_str[i] == '%')
			{
				char ch[3] = { enc_str[i + 1], enc_str[i + 2], 0 };
				int c;
				sscanf_s(ch, "%X", &c);
				res[j] = c;
				i += 2;
			}
			else
			{
				res[j] = enc_str[i];
			}
		}
		i++;
		j++;
	}
	res[j] = 0;
	size_t len = strlen(res) + 1;
	delete[] dec_str;
	dec_str = new char[len];
	strcpy_s(dec_str, len, res);
	delete[] res;
}

size_t get_file_data(char*& filename, char*& content, const char* data)
{
#ifndef _DEBUG
	auto full_len = get_content_length();
	std::cout << full_len << std::endl;
	char* data_copy = new char[full_len];
	memcpy_s(data_copy, full_len, data, full_len);
#else
	char* data_copy = _strdup("------WebKitFormBoundaryAsNg3MnuUO1Ai8uF\n"
		"Content-Disposition: form-data; name=\"ufile\"; filename=\"myfile.txt\"\n"
		"Content-Type: text/plain\n"
		"\n"
		"‎עמ למי פאיכ\n"
		"\n"
		"\n"
		"it's my file\n"
		"------WebKitFormBoundaryAsNg3MnuUO1Ai8uF--\n");
	auto full_len = strlen(data_copy);
#endif
	char *t1, *t2;
	const auto fn_b = strstr(data_copy, "filename=\"") + 10;
	const auto fn_e = strstr(fn_b, "\"");
	const auto fn_ln = fn_e - fn_b;
	delete[] filename;
	filename = new char[fn_ln+1];
	memcpy_s(filename, fn_ln+1, fn_b, fn_ln);
	filename[fn_ln] = 0;
	const char *edge = strtok_s(data_copy, "\n\r", &t1);

	const auto beg_pos = strstr(t1, "Content-Type:");
	strtok_s(beg_pos, "\n", &t2);
	t2++; // \n
	const auto prefix_sz = t2 - data_copy;
	auto len = full_len - (t2 - data_copy) - (strlen(edge) + 4);
	delete[] content;
	content = new char[len+1];
#ifndef _DEBUG
	auto src = data + prefix_sz;
#else
	auto src = data_sz = prefix_sz;
#endif
	memcpy_s(content, len+1, src, len);
	content[len] = 0;
	return len;
}

void get_param_value(char*& value, const char* param_name, const char* data)
{
	delete[] value;
	value = nullptr;
	char* str = _strdup(data);
	char* tmp = str;
	char* cont;
	while (char* part = strtok_s(tmp, "&", &cont))
	{
		tmp = nullptr;
		char* val;
		char* key = strtok_s(part, "=", &val);
		if (!_strcmpi(param_name, key))
		{
			str_decode(value, val);
			//value = _strdup(val);
			delete[] str;
			return;
		}
	}
	delete[] str;
	value = new char[1];
	value[0] = 0;
}
