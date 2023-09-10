#include <iostream>

using namespace std;

template<typename T>
class asmart_ptr {
private:
    T* ptr_;
public:
    explicit asmart_ptr(T* ptr = nullptr): ptr_(ptr) {}
    ~asmart_ptr(){
        delete ptr_;
    }
    T* get() const {return ptr_;}
    T& operator*() const {return *ptr_;}
    T* operator->() const {return ptr_;}
};

int main() {

    return 0;
}