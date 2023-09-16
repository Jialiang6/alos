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


// cmp1
bool cmp1(Complex& com_a, Complex& com_b) {
    int ra = com_a.get_real(), ia = com_a.get_imaginary();
    int rb = com_b.get_real(), ib = com_b.get_imaginary();
    return ra*ra + ia*ia < rb*rb + ib*ib;
}

// cmp2
struct cmp2 {
    bool operator()(Complex& com_a, Complex& com_b) {
        int ra = com_a.get_real(), ia = com_a.get_imaginary();
        int rb = com_b.get_real(), ib = com_b.get_imaginary();
        return ra*ra + ia*ia < rb*rb + ib*ib;
    }
};

// cmp3
auto cmp3 = [&](Complex& com_a, Complex& com_b) {
    int ra = com_a.get_real(), ia = com_a.get_imaginary();
    int rb = com_b.get_real(), ib = com_b.get_imaginary();
    return ra*ra + ia*ia < rb*rb + ib*ib;
};


int main() {
    /*
        priority_queue<T, Container, Compare>
        Compare为函数类型
    */

    // cmp1
    priority_queue<Complex, vector<Complex>, decltype(&cmp1)> q1(&cmp1);
    // cmp2
    priority_queue<Complex, vector<Complex>, cmp2> q2;
    // cmp3
    priority_queue<Complex, vector<Complex>, decltype(cmp3)> q3(cmp3);
    // cmp4
    priority_queue<Complex, vector<Complex>> q4;
    
    create_print<decltype(&cmp1)>(q1);
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