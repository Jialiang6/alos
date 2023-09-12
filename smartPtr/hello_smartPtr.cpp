#include <iostream>

using namespace std;


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
        // ptr_ = p.ptr_;
        // p.ptr_ = nullptr;
        ptr_ = p.release();
    }
    // 实现转移所有权的拷贝赋值函数
    // (特殊的移动赋值函数，被移动的智能指针为左值引用)
    auto_ptr& operator=(auto_ptr& p) {
        // ptr_ = p.ptr_;
        // p.ptr = nullptr;
        auto_ptr(p.release()).swap(*this);
        return *this;
    }
    // 封装的函数
    T* release() {
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }
    void swap(auto_ptr& p) {
        using std::swap;
        swap(ptr_, p.ptr_);
    }
};

struct Date {
    Date(): year(1998), month(9), day(21) {}
    Date(int y, int m, int d): year(y), month(m), day(d) {}
    int year;
    int month;
    int day;
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
    void reset(T* ptr) { scoped_ptr(ptr).swap(*this); }
    void swap(scoped_ptr& p) {
        using std::swap;
        swap(ptr_, p.ptr_);
    }
};


template<typename T>
class unique_str{
private:
    T* ptr_;
public:
    explicit unique_str(T* ptr = nullptr): ptr_(ptr) {}
    ~unique_str() { delete[] ptr_; }
    T* get() {return ptr_; }
    T& operator*() { return *ptr_; }
    T* operator->() { return ptr_; }
    operator bool() ( return ptr_; )
};


int main() {
    // chapter: auto_ptr
    auto_ptr<Date> ap1(new Date());
    cout << ap1->year << endl;
    auto_ptr<Date> ap2(ap1); 
    cout << ap2->year << endl;
    if (ap1.get() == nullptr && ap2.get()) {
        cout << "移动构造：ap1释放所有权,ap2获得所有权" << endl;
    }
    auto_ptr<Date> ap3 = ap2; 
    cout << ap3->year << endl;
    if (ap2.get() == nullptr && ap3.get()) {
        cout << "移动赋值：ap2释放所有权,ap3获得所有权" << endl;
    }
    auto_ptr<int> ap4(new int (5));
    cout << *ap4 << endl;

    // chapter: scoped_ptr
    scoped_ptr<Date> sp(new Date());
    // error!
    // scoped_ptr<Date> sp1(sp);
    return 0;
}