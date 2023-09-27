#include <iostream>
#include <string>
#include "simpleFactory.hpp"

void Opt::setA(std::string numA)
{
    this->m_numA = numA;
}

void Opt::setB(std::string numB)
{
    this->m_numB = numB;
}

std::string Opt::getA() const
{
    return this->m_numA;
}

std::string Opt::getB() const
{
    return this->m_numB;
}

// std::string Opt::getRes() const {
//     return "0";
// }


std::string OptAdd::getRes() const
{
    return std::to_string(stoi(getA()) + stoi(getB()));
}

std::string OptSub::getRes() const
{
    return std::to_string(stoi(getA()) - stoi(getB()));
}

std::string OptMul::getRes() const
{
    return std::to_string(stoi(getA()) * stoi(getB()));
}

std::string OptDiv::getRes() const
{
    return std::to_string(stoi(getA()) / stoi(getB()));
}


Opt* OptFactory::createOpt(char str)
{
    Opt* optp = nullptr;
    switch (str)
    {
    case '+':
        optp = new OptAdd();
        break;
    case '-':
        optp = new OptSub();
        break;
    case '*':
        optp = new OptMul();
        break;
    case '/':
        optp = new OptDiv();
        break;
    default:
        optp = new OptAdd();
        break;
    }
    return optp;
};

int main ()
{
    char signal = '*';
    Opt* op1 = OptFactory().createOpt(signal);
    op1->setA("9");
    op1->setB("7");
    std::cout << op1->getRes() << std::endl;
    return 0;
}


/*
    设计模式1: 简单工厂模式
    - 缺点：增添功能需要修改工厂类的实现，不符合开闭原则。
*/

#if 0
    所谓开闭原则就是：类、对象、函数和结构体等应该对于扩展是开放的，对于修改是封闭的。
    人话：增添功能时只接受增加代码，不接受修改源代码。
#endif