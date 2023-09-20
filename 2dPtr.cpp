#include <iostream>
using namespace std;

int main() {
    int a[2][3] = {
        {1,2,3},
        {4,5,6},
    }; // type: int(*)[3]
    cout << *(*(a+1) + 2) << endl; // 6

    int* p1 = *a; // p1指向a[0]，type: int*
    int* p2 = a[1]; // p2指向a[1]，type: int*

    // int **p3 = &a[0] //error: int (*)[3]" cannot be usded to "int **"
    int (*p3)[3] = a; // 声明一个type: int(*)[3]的指针p3
    cout << *(*(p3 + 1) + 2) << endl; // 6

    // 二级指针，type: int**
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

    int* p5[2] = {0, 0}; // type: int**, 类似于int** p5 = new int*[2], p5[0]=p5[1]=0;
    p5[0] = p4[0];
    p5[1] = p4[1];
    cout << *(*(p5 + 1) + 2) << endl; // 6


    return 0;
}