#include<iostream>
#include<vector>
using namespace std;

int main()
{
    int a;
    char ch[80];
    cin >> a;
    cin.ignore(numeric_limits<streamsize>::max(),'\n'); // 必要！
    cin.getline(ch,80);
    cout << a << endl;
    cout << ch << endl;
    return 0;
}
