#include <iostream>
#include "Aset.hpp"

/*
    设计模式2: 工厂方法模式 (一个方法对应一个工厂，这些工厂都继承自一个抽象工厂类)
      优点：解决了简单工厂模式不符合开闭原则的问题。
    - 缺点：代码量增加。
*/

Opt* AddFactory::createOpt() {
    return new OptAdd();
}

Opt* SubFactory::createOpt() {
    return new OptSub();
}

Opt* MulFactory::createOpt() {
    return new OptMul();
}

Opt* DivFactory::createOpt() {
    return new OptDiv();
}

int main() {
    Opt* op1 = MulFactory().createOpt();
    op1->setA("7");
    op1->setB("9");
    std::cout << op1->getRes() << std::endl;
    return 0;
}