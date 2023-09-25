#include <iostream>
using namespace std;

class Date {
public:
    int year;
    int month;
    int day;
    Date(): year(1998), month(9), day(21) {}
    Date(int y, int m, int d): year(y), month(m), day(d) {}
    virtual void get_year() const { cout << year << endl; }
};

class Time: public Date {
public:
    int hour;
    int minute;
    int second;
    Time(): hour(12), minute(0), second(0) {}
};

int main () {
    /* 准备 */
    Date* p = new Date();
    Time* pt = new Time();
    int a = 1;

    /**
     * 1. dynamic_cast
     *  1) 派生类指针或引用转为基类指针或引用(或反之)
     * 唯一运行时转换，如果转换不成功不是报错或存在未知错误，而是返回空指针
     * 使用条件是基类必须有虚函数(非成功条件)
     */

    Time* dp = dynamic_cast<Time*>(p);
    if (dp) cout << "下行成功" << endl; // 不成功
    Date* dp_1 = dynamic_cast<Date*>(dp);
    if (dp_1) cout << "上行成功" << endl; // 不成功
    Date* dpt = dynamic_cast<Date*>(pt);
    if (dpt) cout << "上行成功" << endl; // 成功

    /**
     * 2. static_cast
     *  1) 基本类型间的转化
     *  2）派生类指针或引用转为基类指针或引用(上行是安全的)
     *  3）基类指针或引用转为派生类指针或引用(下行不安全)
     * 区别于dynamic运行时转换，在编译期间强制转换
     * 不可以转换不同普通类型的指针或引用(特例: 空指针)
     * 也不能用于整型和指针或引用，可把任意类型转为void类型
     */

    int t = 10086;
    char st = static_cast<char>(t);
    long long st_1 = static_cast<long long>(t); // 不安全
    int st_2 = static_cast<int>(st_1);
    Time* sp = static_cast<Time*>(p); // 不安全
    Date* sp_1 = static_cast<Date*>(sp);

    int* p2 = &t;
    // char* sp2 = static_cast<char*>(p2); // error
    // int spt = static_cast<int>(p2); // error
    void* sp2 = static_cast<void*>(p2);

    /**
     * 3. const_cast
     *  1) const与非const的转化
     *  2）volatile与非volatile的转化
     * 只能作用于指针或引用，不能作用于变量 //尽量不用
     */

    const int* p3 = &a;
    int* cp3 = const_cast<int*>(p3);
    p3 = const_cast<const int*>(cp3);
    // volatile int* p3 = &a;
    // int* cp3 = const_cast<int*>(p3);
    // p3 = const_cast<volatile int*>(cp3);
    int* const p3_1 = &a;
    int* cp3_1 = const_cast<int*>(p3_1);
    // char* cp3_1 = const_cast<char*>(p3_1); // error

    /**
     * 4. reinterpret_cast
     *  1) 改变指针或引用的类型(强大)
     *  2）将指针或引用转化为一个足够长度的整型(或反之)
     * 执行的过程是逐个比特复制，不安全 //尽量不用
     */

    int** p4;
    int* rp4 = reinterpret_cast<int*>(p4);
    char* rp4_1 = reinterpret_cast<char*>(p4);
    long* rp4_2 = reinterpret_cast<long*>(p4);
    long long int ra = reinterpret_cast<long long int>(p4);
    p4 = reinterpret_cast<int**>(ra);
    // long int ra = reinterpret_cast<long int>(p4); // error,win里long为4B

    return 0;
}