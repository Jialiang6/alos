#include <iostream>
using namespace std;

int main() {

    /**
     *  cpp 数组的数组名都是一个指针，指向数组的第一个元素
     */ 

    int a0[3] = {1,2,3};
    int* p0 = a0; // a0的类型就是int*

    /**
     *  2D array
     */ 

    int a[2][3] = {
        {1,2,3},
        {4,5,6},
    };
    /* 
     * 这个二维数组在内存的布局是{1，2，3，4，5，6} 
     * type of a: int(*)[3]，指向包含三个整数的数组（作为整体存在）
     * 
     * cout << a << endl;    // 0x61fdb0
     * cout << a+1 << endl;  // 0x61fdbc
     * cout << &a << endl;   // 0x61fdb0
     * cout << &a+1 << endl; // 0x61fdc8
     * 
     * a(等同&a[0])与&a都指向二维数组的第一个元素，但类型不一样
     * a是int(*)[3],+1表示下个1*3的数组,实际+12B
     * &a是int(*)[2][3],+1表示下个2*3的二维数组,实际+24B
     */
    cout << *(*(a+1) + 2) << endl; // 6


    int* p1 = *a;   // p1指向第一个包含三个整数的数组a[0]的第一个元素，type: int*
                    // cout << p1 << endl;  // 0x61fdb0
    int* p2 = a[1]; // p2指向第二个包含三个整数的数组a[1]的第一个元素，type: int*
                    // cout << p2 << endl;  // 0x61fdbc

    // int **p3 = &a[0]; // error: int (*)[3]" cannot be usded to "int **"
    // int **p3 = a;     // error: int (*)[3]" cannot be usded to "int **"
    int (*p3)[3] = a;    // 声明一个type: int(*)[3]的指针p3,也是0x61fdb0
    cout << *(*(p3 + 1) + 2) << endl;  // 6


    /**
     *  二级指针，type: int**
     */ 

    /*
     * 二级指针int**指向的是一个指针变量int*
     * 我们可以显示地看到在内存布局上
     * 与int(*)[3]类型的指针的差异！
     */

    int** p4 = new int*[2];
    for (int i = 0; i < 2; i++) {
        p4[i] = new int[3]; // *(p4+i) = new int[3]
        for (int j = 0; j < 3; j++) {
            // cout << p4[i][j] << " "; // 未初始化的指针指向一个很小的负值
            *(p4[i]+j) = a[i][j];
            cout << p4[i][j] << " "; // 指向好了就能这么访问
        }
        cout << endl;
    }

    /**
     * 二级指针的复制
     * int** p5 = new int*[2], p5[0]=p5[1]=0;
     */

    int* p5[2] = {0, 0};
    p5[0] = a[0], p5[1] = a[1];
    // p5[0] = p4[0], p5[1] = p4[1];
    cout << *(*(p5 + 1) + 2) << endl; // 6

    /*
     * 小结（如何用二级指针表示二维数组，一一指向）：
     * 开辟了存放两个int*的数组
     * 让p4(p5)指向数组的第一个int*
     * 又让每个int*分别指向包含三个整数的数组

     * cout << p4 << endl;   // 0x8e1a90
     * cout << p4+1 << endl; // 0x8e1a98

     * 不能直接int** p6 = (int**) a
     * 因为int(*)[3]与int**在内存布局上不一致
     * 例如p4+1实际移动了一个int*指针变量，+8B
     * 而p3+1实际移动了一个包含三个整数的数组，+12B
     */
    
    int* tmp_p6 = a[0]; // 让int** 变量指针指向二维数组得二层转化
    int** p6 = &tmp_p6; // 这里只指向了二维数组的第一个元素
                        // 不能完全访问二维数组所有元素
    cout << **p6 << endl;             // 1
    cout << *p6 << endl;              // 0x61fdb0
    cout << *(p6+1) << endl;          // 0x61fdb0 (未定义行为,取决于编译器)
    cout << *(*(p6 + 1) + 2) << endl; // 3
    // cout << *(*(p6 - 1) + 2) << endl; // segment fault!

    int ** p7;
    int * p7_1;
    p7 = &p7_1;

    return 0;
}