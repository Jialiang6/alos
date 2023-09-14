#include <iostream>
#include <typeinfo>

using namespace std;

enum color { 
    red, // 实际为0
    green, // 实际为1
    blue, // 实际为2
    pink=5,
    purple // 实际是6
};
// 底层类型是编译器根据成员数量选定的

// 成员静态，在定义时必须初始化
enum class MySize : int {
    small,
    medium,
    big
};
// 底层类型是强制指定的整型

void print_color(color c) {
    switch(c) {
        case red:
            cout << "is red." << endl;
            break;
        case green:
            cout << "is green." << endl;
            break;
        case blue:
            cout << "is blue." << endl;
            break;
        default:
            cout << "other color." << endl;
            break;
    }
}

// Struct空间为成员空间大小的和(对齐)
struct MyStruct {
    struct inside {
        int y;
        int z;
    } u;
    int x;
};

// Union空间为占空间最大成员的空间大小(对齐)
union MyUnion {
    struct inside {
        int y;
        int z;
    } u;
    int x;
};


int main() {
    color c1 = blue, c2 = purple; // 可以直接访问,成员的作用域共享
    cout << c1 << endl;
    print_color(c1);
    cout << c2 << endl;
    print_color(c2);
    MySize s = MySize::big; // 需要通过限定符::访问，成员的作用域独占
    cout << sizeof(s) << endl;

    MyStruct a;
    MyUnion b;
    a.u.y = b.u.y = 1;
    a.u.z = b.u.z = 0;
    a.x = b.x = 8; // 联合体的x赋值后覆盖了y的值
    printf("%d %d %d\n", a.x, a.u.y, a.u.z);
    printf("%d %d %d\n", b.x, b.u.y, b.u.z);

    return 0;
}