// Cpp program to redirect cout to a file
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	fstream file;
	file.open("cout.txt", ios::out);
	string line;

	// 保存 cin 和 cout 的缓冲区 buffer
	streambuf* stream_buffer_cout = cout.rdbuf();
	streambuf* stream_buffer_cin = cin.rdbuf();

	// 获取文件 file 的缓冲区 buffer
	streambuf* stream_buffer_file = file.rdbuf();

	// cout 重定向文件
	cout.rdbuf(stream_buffer_file);
	cout << "This line written to file" << endl;

	// cout 恢复重定
	cout.rdbuf(stream_buffer_cout);
	cout << "This line is written to screen" << endl;

	file.close();
	return 0;
}