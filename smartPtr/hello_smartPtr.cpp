#include <iostream>

using namespace std;


class Date {
public:
    int year;
    int month;
    int day;
    Date(): year(1998), month(9), day(21) {}
    Date(int y, int m, int d): year(y), month(m), day(d) {}
};

class Time: public Date {
public:
    int hour;
    int minute;
    int second;
    Time(): hour(12), minute(0), second(0) {}
};


template<typename T>
class scoped_ptr {
private:
    T* ptr_;
    // 相当于禁用
    scoped_ptr(scoped_ptr& p);
    scoped_ptr& operator=(scoped_ptr& p);
public:
    explicit scoped_ptr(T* ptr = nullptr): ptr_(ptr) {}
    ~scoped_ptr() { delete[] ptr_; }
    T* get() const noexcept {return ptr_;}
    T& operator*() const {return *ptr_;}
    T* operator->() const {return ptr_;}
    operator bool() const {return ptr_;}
    void reset(T* ptr) { swap(ptr_, ptr); }
};


template<typename T>
class auto_ptr {
private:
    T* ptr_;
public:
    explicit auto_ptr(T* ptr = nullptr): ptr_(ptr) {}
    ~auto_ptr(){
        delete[] ptr_;
    }
    T* get() const noexcept {return ptr_;}
    T& operator*() const {return *ptr_;}
    T* operator->() const {return ptr_;}
    operator bool() const {return ptr_;}

    // auto_ptr(const auto_ptr&) = delete;
    // auto_ptr& operator=(const auto_ptr&) = delete;
    // 简单禁用

    // 实现转移所有权的拷贝构造函数
    // (特殊的移动构造函数,被移动的智能指针为左值引用)
    auto_ptr(auto_ptr& p) {
        ptr_ = p.ptr_;
        p.ptr_ = nullptr;
    }
    // 实现转移所有权的拷贝赋值函数
    // (特殊的移动赋值函数，被移动的智能指针为左值引用)
    auto_ptr& operator=(auto_ptr& p) {
        using std::swap;
        swap(ptr_, p.ptr);
        return *this;
    }
};


template<typename T>
class unique_ptr{
private:
    T* ptr_;
    // 显示禁用拷贝构造和拷贝赋值函数，实际上也禁用了(没实现)
    unique_ptr(const unique_ptr& p);
    unique_ptr& operator=(const unique_ptr& p);
public:
    explicit unique_ptr(T* ptr = nullptr): ptr_(ptr) {}
    ~unique_ptr() { delete[] ptr_; }
    T* get() const noexcept {return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    operator bool() const { return ptr_; }

    unique_ptr(unique_ptr&& p) { // 这里改成了右值引用
        ptr_ = p.release();
    }

    // 实现指针的隐式类型转换
    // 子类向基类的隐式转换
    template<typename U>
    unique_ptr(unique_ptr<U>&& p) {
        ptr_ = p.release(); // 这样的实现避开继承中访问成员的权限问题
    }

    // unique_ptr& operator=(unique_ptr p) { // 不加引用相当于调用了拷贝
                                        //(或移动，取决于实现了什么)构造
    unique_ptr& operator=(unique_ptr&& p) { // 显示说明右值引用，可读性强，且效率更高
        p.swap(*this);
        return *this;
    }

    T *release() noexcept {
        T *ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    void swap(unique_ptr& p) {
        using std::swap;
        swap(ptr_, p.ptr_);
    }
};




int main() {

    // chapter[1]
    // auto_ptr

    auto_ptr<Date> ap1(new Date());
    cout << ap1->year << endl;
    auto_ptr<Date> ap2(ap1); 
    cout << ap2->year << endl;
    if (ap1.get() == nullptr && ap2.get()) {
        cout << "隐式移动构造：ap1释放所有权,ap2获得所有权" << endl;
    }
    auto_ptr<Date> ap3 = ap2; 
    cout << ap3->year << endl;
    if (ap2.get() == nullptr && ap3.get()) {
        cout << "隐式移动赋值：ap2释放所有权,ap3获得所有权" << endl;
    }
    auto_ptr<int> ap4(new int (5));
    cout << *ap4 << endl;


    // chapter[2]
    // scoped_ptr

    scoped_ptr<Date> sp(new Date());
    // scoped_ptr<Date> sp1(sp); // error


    // chapter:[3]
    // unique_ptr

    unique_ptr<Date> up1(new Date());
    // unique_ptr<Date> up2(up1); // error
    unique_ptr<Date> up2(move(up1));
    cout << up2->year << endl;
    if (up1.get() == nullptr && up2.get()) {
        cout << "显示移动构造" << endl;
    }
    // unique_ptr<Date> up3 = up2; // error
    unique_ptr<Date> up3 = move(up2);
    cout << up3->year << endl;
    if (up2.get() == nullptr && up3.get()) {
        cout << "显示赋值" << endl;
    }
    unique_ptr<Time> up4(new Time());
    unique_ptr<Date> up5(move(up4));
    cout << up5->year << endl;
    if (up4.get() == nullptr && up5.get()) {
        cout << "显示隐式转换赋值" << endl;
    }

    return 0;
}