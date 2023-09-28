#include <string>

// 用一个标识符来判断是否包含该头文件的内容
#ifndef __ASET_HPP
#define __ASET_HPP

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


inline void Opt::setA(std::string numA)
{
    this->m_numA = numA;
}

inline void Opt::setB(std::string numB)
{
    this->m_numB = numB;
}

inline std::string Opt::getA() const
{
    return this->m_numA;
}

inline std::string Opt::getB() const
{
    return this->m_numB;
}

// std::string Opt::getRes() const {
//     return "0";
// }


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

inline std::string OptAdd::getRes() const
{
    return std::to_string(stoi(getA()) + stoi(getB()));
}

inline std::string OptSub::getRes() const
{
    return std::to_string(stoi(getA()) - stoi(getB()));
}

inline std::string OptMul::getRes() const
{
    return std::to_string(stoi(getA()) * stoi(getB()));
}

inline std::string OptDiv::getRes() const
{
    return std::to_string(stoi(getA()) / stoi(getB()));
}


/**
 * 1. 简单工厂模式
 * 
 */

class SimpleFactory
{
public:
    Opt* createOpt(char str);
};


/**
 * 2. 工厂方法模式
 * 
 */

class FactoryMethod
{
public:
    virtual Opt* createOpt() = 0;
};

class AddFactory : public FactoryMethod
{
public:
    Opt* createOpt();
};
class SubFactory : public FactoryMethod
{
public:
    Opt* createOpt();
};
class MulFactory : public FactoryMethod
{
public:
    Opt* createOpt();
};
class DivFactory : public FactoryMethod
{
public:
    Opt* createOpt();
};


/**
 * 3. 抽象工厂模式
 * 
 */

class OptAddNeg : public Opt {
    std::string getRes() const;
};
class OptSubNeg : public Opt {
    std::string getRes() const;
};
class OptMulNeg : public Opt {
    std::string getRes() const;
};
class OptDivNeg : public Opt {
    std::string getRes() const;
};

inline std::string OptAddNeg::getRes() const
{
    return std::to_string(-stoi(getA()) - stoi(getB()));
}
inline std::string OptSubNeg::getRes() const
{
    return std::to_string(-stoi(getA()) + stoi(getB()));
}
inline std::string OptMulNeg::getRes() const
{
    return std::to_string(-stoi(getA()) * stoi(getB()));
}
inline std::string OptDivNeg::getRes() const
{
    return std::to_string(-stoi(getA()) / stoi(getB()));
}

class FactoryAbstract
{
public:
    virtual Opt* createOpt() = 0;
    virtual Opt* createOptNeg() = 0;
};

class AddFactoryAbs : public FactoryMethod
{
public:
    Opt* createOpt();
    Opt* createOptNeg();
};
class SubFactoryAbs : public FactoryMethod
{
public:
    Opt* createOpt();
    Opt* createOptNeg();
};
class MulFactoryAbs : public FactoryMethod
{
public:
    Opt* createOpt();
    Opt* createOptNeg();
};
class DivFactoryAbs : public FactoryMethod
{
public:
    Opt* createOpt();
    Opt* createOptNeg();
};


#endif