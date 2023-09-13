/*  shared_ptr可以多个智能指针对象指向同一块空间
    因此shared_ptr不仅可以移动拷贝构造，也可以拷贝构造
 */

#include <iostream>
#include <typeinfo>
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


int i = 0;

class shared_count {
private:
    long cnt_;
public:
    shared_count(): cnt_(1) {}
    void add_count() { ++cnt_; }
    long reduce_count() { return --cnt_; }
    long get_count() const { return cnt_; }
};

template<typename T>
class shared_ptr {
private:
    T* ptr_;
    shared_count* shared_count_;
public:
    // 构造函数；包括开辟空间存储计数
    explicit shared_ptr(T* ptr = nullptr): ptr_(ptr) {
        if (ptr) shared_count_ = new shared_count();
    }
    // 实现强制类型转换需要的构造函数
    template <typename U>
    shared_ptr(const shared_ptr<U> &p, T *ptr) noexcept {
        ptr_ = ptr;
        if (ptr_) {
            p.shared_count_->add_count();
            shared_count_ = p.shared_count_;
        }
    }
    // 析构函数；释放两个指针指向的空间
    ~shared_ptr() {
        if (ptr_ && shared_count_->reduce_count() == 0) {
            delete[] ptr_;
            delete shared_count_;
        }
    }
    // 让智能指针对象更像指针
    T* get() const noexcept {return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    operator bool() const { return ptr_; }

    // 显示声明类型为U的智能指针与类型为T的智能指针为friend，这样可以互访私有成员
    template<typename U> friend class shared_ptr;
    // 带模板(隐式类型转换)的拷贝(浅拷贝)构造
    template<typename U> shared_ptr(shared_ptr<U>& p) {
        ptr_ = p.ptr_; // 类型在此隐式转换
        if (ptr_) {
            p.shared_count_->add_count();
            shared_count_ = p.shared_count_;
        }
    }
    // 带模板(隐式类型转换)的移动拷贝构造
    template<typename U> shared_ptr(shared_ptr<U>&& p) {
        ptr_ = p.ptr_; // 类型在此隐式转换
        if (ptr_) {
            shared_count_ = p.shared_count_;
            p.ptr_ = nullptr;
            p.shared_count_ = nullptr;
        }
    }

    // 赋值数不是引用类型，决定调用拷贝构造还是移动构造来创建临时对象
    shared_ptr& operator=(shared_ptr p) {
        p.swap(*this);
        return *this;
    }
    // 封装swap
    void swap(shared_ptr& p) {
        using std::swap;
        swap(ptr_, p.ptr_);
        swap(shared_count_, p.shared_count_);
    }

    long use_count() const noexcept {
        if (ptr_) {
            return shared_count_->get_count();
        } else {
            return 0;
        }
    }

    
};


// 强制类型转换，从U转为T
template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U> &p) noexcept {
  T *ptr = dynamic_cast<T*>(p.get());
  return shared_ptr<T>(p, ptr);
}

template <typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U> &p) noexcept {
  T *ptr = static_cast<T*>(p.get());
  return shared_ptr<T>(p, ptr);
}

template <typename T, typename U>
shared_ptr<T> const_pointer_cast(const shared_ptr<U> &p) noexcept {
  T *ptr = const_cast<T*>(p.get());
  return shared_ptr<T>(p, ptr);
}

template <typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U> &p) noexcept {
  T *ptr = reinterpret_cast<T*>(p.get());
  return shared_ptr<T>(p, ptr);
}


int main() {
    shared_ptr<Time> sp1(new Time());
    cout << "use count of sp1: " << sp1.use_count() << endl; // 1
    shared_ptr<Date> sp2, sp3;
    cout << "use count of sp2: " << sp2.use_count() << endl; // 0
    sp2 = sp1;
    sp3 = sp1;
    cout << "3指针同时指向一块空间" << endl;
    cout << "use count of sp1: " << sp1.use_count() << endl; // 3
    cout << "use count of sp2: " << sp2.use_count() << endl; // 3
    cout << "use count of sp3: " << sp3.use_count() << endl; // 3
    sp2 = move(sp1);
    cout << "2指针同时指向一块空间" << endl;
    if (!sp1 && sp2) { // 调用bool重载操作符
        cout << "sp1 move to sp2" << endl;
        cout << "use count of sp1: " << sp1.use_count() << endl; // 0
        cout << "use count of sp2: " << sp2.use_count() << endl; // 2
        cout << "use count of sp3: " << sp3.use_count() << endl; // 2
    }

    cout << "强制类型转换" << endl; // 基类Date,子类Time

    // [dynamic_cast] 将基类指针/引用转换为子类的指针/引用，反之也行
    // 使用前提(不是成功前提): 基类必须有虚函数（因为dynamic_cast靠虚函数表）
    // 安全的转换：转换失败则返回空指针（成功说明没有空间越界）

    // 基类转子类，转换指针类型，但指针为空，此时空间还是由dsp1拥有
    shared_ptr<Date> dsp1(new Date); 
    shared_ptr<Time> dsp2 = dynamic_pointer_cast<Time>(dsp1);
    cout << "use count of dsp1: " << dsp1.use_count() << endl; // 1
    cout << "use count of dsp2: " << dsp2.use_count() << endl; // 0
    // 同类转同类(基类转子类)，指针非空，此时空间被两者共有
    shared_ptr<Date> dsp3(new Time);      // dsp1指针类型Date,指向Time的空间
    // shared_ptr<Time> dsp3(new Time);   // 与上行结果一致
    shared_ptr<Time> dsp4 = dynamic_pointer_cast<Time>(dsp3); // 空间不越界
    cout << "use count of dsp3: " << dsp3.use_count() << endl; // 2
    cout << "use count of dsp4: " << dsp4.use_count() << endl; // 2
    // 子类转基类，指针非空，此时空间被两者共有
    shared_ptr<Time> dsp5(new Time);
    shared_ptr<Date> dsp6 = dynamic_pointer_cast<Date>(dsp5); // 空间不越界
    cout << "use count of dsp5: " << dsp5.use_count() << endl; // 2
    cout << "use count of dsp6: " << dsp6.use_count() << endl; // 2

    // [static_cast]（dynamic_cast里失败的也可转换成功,所以多态类最好用dynamic_cast,更安全）
    // 告诉编译器明确需要转换，不加时编译器会报的错在加上不报错
    // 但不能转换非继承类（C语言风格的却可以，例如用(A*)来转化一个*C）

    // 基类转子类，dynamic_cast失败的类型转换这里也成功了（不安全,后面可能崩溃）
    shared_ptr<Date> ssp1(new Date);
    shared_ptr<Time> ssp2 = static_pointer_cast<Time>(ssp1);
    cout << "use count of ssp1: " << ssp1.use_count() << endl; // 2
    cout << "use count of ssp2: " << ssp2.use_count() << endl; // 2
    // 同类转同类(基类转子类)，指针非空，此时空间被两者共有
    shared_ptr<Date> ssp3(new Time);
    // shared_ptr<Time> ssp3(new Time); // 与上行结果一致
    shared_ptr<Time> ssp4 = static_pointer_cast<Time>(ssp3);
    cout << "use count of ssp3: " << ssp3.use_count() << endl; // 2
    cout << "use count of ssp4: " << ssp4.use_count() << endl; // 2
    // 子类转基类，指针非空，此时空间被两者共有
    shared_ptr<Time> ssp5(new Time);
    shared_ptr<Date> ssp6 = static_pointer_cast<Date>(ssp5);
    cout << "use count of ssp5: " << ssp5.use_count() << endl; // 2
    cout << "use count of ssp6: " << ssp6.use_count() << endl; // 2

    shared_ptr<const int> cp1(new int);
    shared_ptr<int> cp2 = const_pointer_cast<int>(cp1); // const 转非const
    int a = reinterpret_pointer_cast<int>(cp2); // 啥都能转，但不安全
    cout << a << endl;
    return 0;
}