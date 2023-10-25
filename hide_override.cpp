#include <iostream>
#include <string>
#include <cstring>

using namespace std;

class father {
private:
    char* _name;
    int _age;
public:
    father(const char* name, int age): _age(age) {
        this->_name = new char[strlen(name) + 1];
        strcpy(this->_name, name);
    }
    virtual ~father() {
        delete[] _name;
        cout << "father died." << endl;
    }
    string get_name() const {
        return _name;
    }
    int get_age() const {
        return _age;
    }
    virtual void eat(string food) {
        cout << "The father " << get_name() << " eats " << food << endl;
    }
};

class son : public father {
public:
    char* _tall;

    son(const char* name, int age, const char* tall) : father(name, age) {
        this->_tall = new char[strlen(tall)+1];
        strcpy(this->_tall, tall);
    }
    void eat(string food) {
        cout << "The son " << get_name() << " who is " << _tall 
             << " eats " << food << endl;
    }
    ~son() {
        delete[] _tall; // delete额外的
        cout << "son died." << endl;
    }
};

class daughter : public father {
public:
    char* _weight;
    daughter(const char* name, int age, const char* weight) : father(name, age) {
        this->_weight = new char[strlen(weight)+1];
        strcpy(this->_weight, weight);
    }
    void eat(string food, bool happy) {
        cout << "The daughter " << get_name() << " who is " << _weight
             << " eats " << food << (happy ? " and happy" : " and not happy") << endl;
    }
    ~daughter() {
        delete[] _weight;
        cout << "daughter died." << endl;
    }
};

int main() {
    // 隐藏 (根据指针类型)
    daughter* d = new daughter("Lin", 12, "50kg");
    d->eat("tomato", true);
    // d->eat("tomato"); // error
    delete d; // dauther died. father died.

    father* fd = new daughter("Li", 13, "60kg");
    fd->eat("hamgburger");
    delete fd; // dauther died. father died.

    // 多态, 虚函数覆写 （根据对象类型）
    father* s = new son("Yong", 13, "175cm");
    s->eat("Tofu");
    delete s; // son died. father died.
    
    // 重写参数列表个数和类型得一致
    // father* rd = new daughter("Lin", 12, "50kg"s);
    // rd->eat("tomato", true); // error
    return 0;
}