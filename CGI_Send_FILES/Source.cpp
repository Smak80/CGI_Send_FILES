#include <iostream>
#include <fstream>
#include "cgi.h"
using namespace std;

int main()
{
	cout << "Content-type: text/html; charset=windows-1251\n";
	cout << endl;
	cout << "<form method='post' enctype='multipart/form-data' action='file.cgi'>";
	cout << "<input type='file' name='ufile'>";
	cout << "<input type='submit'>";
	cout << "</form><br>";
	char* data = nullptr;
	if (!is_get()) {
		get_form_data(data);
		cout << "size=" << get_content_length() << "<br>\n";
		//cout << "<br>" << data << "<br>\n";
		char *fn = nullptr, *cont = nullptr;
		auto sz = get_file_data(fn, cont, data);
		cout << "<br>filename=" << fn << endl;
		//cout << "<br>cont=<br>" << cont << endl;
		fstream f(fn, ios_base::out | ios_base::trunc | ios_base::binary);
		if (f.is_open())
		{
			f.write(cont, sz);
			f.close();
		}
	}
	delete[] data;
}