#include <iostream>
#include "Aset.hpp"

/*
    设计模式3: 抽象工厂模式
      优点：等于将工厂方法模式中一些工厂类抽象为一个大的工厂
    - 缺点：要求一个大工厂内合并的工厂的方法是类似的
*/


Opt* AddFactoryAbs::createOpt() {
    return new OptAdd();
}
Opt* AddFactoryAbs::createOptNeg() {
    return new OptAddNeg();
}

Opt* SubFactoryAbs::createOpt() {
    return new OptSub();
}
Opt* SubFactoryAbs::createOptNeg() {
    return new OptSubNeg();
}

Opt* MulFactoryAbs::createOpt() {
    return new OptMul();
}
Opt* MulFactoryAbs::createOptNeg() {
    return new OptMulNeg();
}

Opt* DivFactoryAbs::createOpt() {
    return new OptDiv();
}
Opt* DivFactoryAbs::createOptNeg() {
    return new OptDivNeg();
}

int main () {
    Opt* op1 = MulFactoryAbs().createOpt();
    op1->setA("7");
    op1->setB("9");
    std:: cout << op1->getRes() << std::endl;

    Opt* op2 = MulFactoryAbs().createOptNeg();
    op2->setA("7");
    op2->setB("9");
    std:: cout << op2->getRes() << std::endl;
    return 0;
}