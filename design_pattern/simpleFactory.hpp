#include <string>

// 用一个标识符来判断是否包含该头文件的内容
#ifndef __SIMPLE_HPP
#define __SIMPLE_HPP

class Opt
{
private:
    std::string m_numA;
    std::string m_numB;
public:
    Opt() {}
    virtual ~Opt() {}
    void setA(std::string numA);
    void setB(std::string numB);
    std::string getA() const;
    std::string getB() const;
    virtual std::string getRes() const = 0;
};

//当采用默认的private继承，指向基类的指针（引用）不能指向派生类
//为了让后面的工厂类中基类的指针能够指向派生类，这里采用public继承
class OptAdd : public Opt {
    std::string getRes() const;
};
class OptSub : public Opt {
    std::string getRes() const;
};
class OptMul : public Opt {
    std::string getRes() const;
};
class OptDiv : public Opt {
    std::string getRes() const;
};

class OptFactory
{
public:
    Opt* createOpt(char str);
};


#endif