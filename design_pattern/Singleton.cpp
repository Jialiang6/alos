/*
    单例模式：
      保证类的实例化对象仅有一个，并且提供一个全局访问点
    应用场景：
      有且仅需一个对象时，例如操作系统只要一个文件系统实例
    实现方式：
      1）默认的构造函数，拷贝构造函数，复制拷贝函数为私有
      2）全局访问点定义为静态成员函数，没有参数，返回该类的指针
*/

#include <iostream>
#include <mutex>
using namespace std;

/*
    不安全的方式(最初的懒汉)：
      当两个线程同时调用 getInstance()，且instance==nullptr,
      两个线程会同时实例化对象
*/

// class Singleton{
// private:
//     static Singleton* instance;
//     Singleton(){}
//     Singleton(const Singleton& tmp){}
//     Singleton& operator=(const Singleton& tmp){}
// public:
//     static Singleton* getInstance(){
//         if(instance == nullptr){
//             instance = new Singleton();
//         }
//         return instance;
//     }
// };
// Singleton* Singleton::instance = nullptr;


/*
    懒汉模式：直到第一次用对象时才实例化
        缺点：线程不安全，得用额外开销保证线程安全
    饿汉模式：类定义时就实例化
        优点：1）线程安全 2）在类加载时已经创建好静态对象，调用速度快
        缺点：当永远不调用getInstance()，这个实例依然初始化
*/

/**
 * 懒汉模式实现1：加锁
 * 
 */

class Singleton1{
private:
    static mutex mtx;
    static Singleton1* instance;
    Singleton1(){}
    Singleton1(const Singleton1& tmp){}
    Singleton1& operator=(const Singleton1& tmp){}
public:
    static Singleton1* getInstance(){
        if (instance == nullptr) {
            unique_lock lck(mtx);
            if(instance == nullptr){
                instance = new Singleton1();            
            }
            return instance;
        }
    }

    // static Singleton1* getInstance(){
    //     mtx.lock();
    //     if(instance == nullptr){            
    //         instance = new Singleton1();            
    //     }
    //     mtx.unlock();
    //     return instance;
    // }
};
Singleton1* Singleton1::instance = nullptr;
mutex Singleton1::mtx;

/**
 * 懒汉模式实现2：局部静态变量
 * （利用局部静态变量只在第一次使用时初始化/
 *   全局静态变量是在main前定义，在后面任意地方初始化）
 */

class Singleton2{
private:
    Singleton2(){}
    Singleton2(const Singleton2& temp){}
    Singleton2& operator=(const Singleton2& temp){}
public:
    static Singleton2* getInstance(){ 
        static Singleton2 instance;
        return &instance;
    }
};


/**
 * 饿汉模式实现：线程安全(多线程调用getInstance也没事)，不用加锁
 * 
 */

class Singleton{
private:
    static Singleton* instance;
    Singleton(const Singleton& temp){}
    Singleton& operator=(const Singleton& temp){}
protected:
	Singleton(){} 
public:
    static Singleton* getInstance(){ 
        return instance;    
    }
};
Singleton* Singleton::instance = new Singleton();


int main() {
    return 0;
}

