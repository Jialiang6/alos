# include <iostream>
# include <typeinfo>
# include <queue>
# include <algorithm>

using namespace std;

class Complex {
private:
    int real;
    int imaginary;
public:
    Complex(): real(0), imaginary(0) {}
    Complex(int r, int i): real(r), imaginary(i) {}
    int get_real() const { return real; }
    int get_imaginary() const { return imaginary; }
    // 最方便的就是类内定义: cmp4
    bool operator<(const Complex& b) const {
        return this->real*this->real + this->imaginary*this->imaginary <
        b.get_real()*b.get_real() + b.get_imaginary()*b.get_imaginary();
    }
};


template<typename T>
void create_print(priority_queue<Complex, vector<Complex>, T>& q) {
    cout << "This is from " << typeid(T).name() << endl;
    cout << "create complex numbers" << endl;
    for (int i = 0; i < 5; i++) {
        q.emplace(Complex(rand()%10, rand()%10));
    }
    cout << "print sorted complex numbers" << endl;
    while (!q.empty()) {
        auto c = q.top();
        q.pop();
        int rc = c.get_real(), ic = c.get_imaginary();
        cout << rc << " " << ic << " ";
        cout << rc*rc + ic*ic << endl;
    }
}


// 题外话：函数指针，主要作用是打通数据与方法，让函数可以像对象一样传参和使用

// cmp1: 函数
bool cmp1(Complex& com_a, Complex& com_b) {
    int ra = com_a.get_real(), ia = com_a.get_imaginary();
    int rb = com_b.get_real(), ib = com_b.get_imaginary();
    return ra*ra + ia*ia < rb*rb + ib*ib;
}

// cmp2： 函数对象/仿函数，实际上是一种类型，但 类名()这种形式相当于调用函数
struct cmp2 {
    bool operator()(Complex& com_a, Complex& com_b) {
        int ra = com_a.get_real(), ia = com_a.get_imaginary();
        int rb = com_b.get_real(), ib = com_b.get_imaginary();
        return ra*ra + ia*ia < rb*rb + ib*ib;
    }
};

// cmp3： lambda表达式
auto cmp3 = [&](Complex& com_a, Complex& com_b) {
    int ra = com_a.get_real(), ia = com_a.get_imaginary();
    int rb = com_b.get_real(), ib = com_b.get_imaginary();
    return ra*ra + ia*ia < rb*rb + ib*ib;
};

// 题外话：decltype()内是expression
// decltype(cmp)返回的是函数类型而非指针类型，这里就是bool(Complex&, Complex&)
// 一般当作形参的是函数指针，这里构造<>内需要的类型也是函数指针类型，即bool(*)(Compare&, Compare&)
// 另外，制定了指针类型，构造函数里还要传入指针具体指向什么来初始化，不然就产生了野指针
// lambda表达式是一种特殊情况，可以认为返回的是一个函数指针

int main() {
    /*
        priority_queue<T, Container, Compare>
        Compare为函数类型
    */

    // cmp1
    // priority_queue<Complex, vector<Complex>, decltype(&cmp1)> q1(&cmp1);
    priority_queue<Complex, vector<Complex>, decltype(cmp1)*> q1(cmp1);
    // priority_queue<Complex, vector<Complex>, decltype(&cmp1)> q1(cmp1);
    // cmp2
    priority_queue<Complex, vector<Complex>, cmp2> q2;
    // cmp3
    priority_queue<Complex, vector<Complex>, decltype(cmp3)> q3(cmp3);
    // cmp4
    priority_queue<Complex, vector<Complex>> q4;
    
    create_print<decltype(&cmp1)>(q1);
    // create_print<decltype(&cmp1)>(q1);
    create_print<cmp2>(q2);
    create_print<decltype(cmp3)>(q3);
    create_print<>(q4);

    /*
        sort(RandomIt first, RandomIt second, Compare cpmp)
        comp为函数对象
    */

    cout << "sort:" << endl;
    vector<Complex> nums;
    for (int i = 0; i < 5; i++) {
        nums.emplace_back(Complex(rand()%10, rand()%10));
    }
    // sort(nums.begin(), nums.end(), cmp1);
    sort(nums.begin(), nums.end(), cmp2());
    // sort(nums.begin(), nums.end(), cmp3);
    // sort(nums.begin(), nums.end()); // cmp4
    for (int i = 0; i < 5; i++) {
        auto c = nums[i];
        int rc = c.get_real(), ic = c.get_imaginary();
        cout << rc << " " << ic << " ";
        cout << rc*rc + ic*ic << endl;
    }
    return 0;
}