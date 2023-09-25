#include <iostream>
#include <cstring>

using namespace std;

class Person {
private:
    char* Name;
    int Age;
public:
    // 构造函数
    Person(const char* name, int age): Age(age) {
        Name = new char[strlen(name) + 1];
        strcpy(Name, name);
        cout << "构造函数" << endl;
    }

    // 不实现拷贝构造，则默认拷贝构造为浅拷贝 (Name = name, Age = age;);
    // Age正常拷贝（和深拷贝没区别），但分配动态空间的指针会指向同一个空间;
    // 析构函数有释放操作的话，作用域结束以后，
    // 两个指针指向的空间会释放两次，造成未知错误(未定义或程序崩溃)
    // ！！因此，如果类有指针，则最好自己实现（深）拷贝构造

    // 析构函数，这里释放了指针的空间; 默认的析构函数什么也不做；
    ~Person() {
        delete[] Name;
        Name = nullptr;
        cout << "析构函数" << endl;
    }

    // 实现拷贝构造，这里是深拷贝，指针指向了新开辟的空间
    Person(const Person& p): Age(p.Age) {
        Name = new char[strlen(p.Name) + 1];
        strcpy(Name, p.Name);
        cout << "深拷贝构造函数" << endl;
    }

    // 实现移动拷贝构造
    Person(Person&& p): Age(p.Age) {
        Name = p.Name;
        p.Name = nullptr;
        p.Age = -1;
        cout << "移动构造函数" << endl;
    }

    // 实现拷贝赋值函数
    Person& operator=(const Person& p) {
        Name = new char[strlen(p.Name) + 1];
        strcpy(Name, p.Name);
        Age = p.Age;
        cout << "拷贝赋值函数" << endl;
        return *this;
    };

    // 实现移动赋值函数
    Person& operator=(Person&& p) {
        Name = p.Name;
        p.Name = nullptr;
        Age = p.Age;
        cout << "移动赋值函数" << endl;
        return *this;
    };

    // const成员函数只读取成员变量
    // 这里用来打印私有成员变量
    void get() const {
        cout << "Name: " << Name << endl;
    }
};


int main() {
    Person p1("lisa", 22);
    Person p2(p1);
    Person p3(Person("Mike", 80)); // 这里直接构造了p3而不是先创建临时对象再拷贝
    p1.get();
    p2.get();
    p3.get();
    Person p4(move(p3));
    p4.get();

    p2 = Person("Bruce", 55);
    p2.get();

    // nullptr
    // p3.get();

    /*
        构造函数
        深拷贝构造函数
        构造函数      
        Name: lisa    
        Name: lisa    
        Name: Mike    
        移动构造函数
        Name: Mike
        构造函数
        移动赋值函数
        析构函数
        Name: Bruce
        析构函数
        析构函数
        析构函数
        析构函数 
    */

    return 0;
}