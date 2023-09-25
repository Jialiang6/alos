#include <iostream>
#include <string>

using namespace std;

class father {
private:
    string name;
    int age;
public:
    father(): name("Bage"), age(50) {}
    father(string name, int age): name(name), age(age) {}
    virtual ~father() {}
    string get_name() const {
        return name;
    }
    int get_age() const {
        return age;
    }
    virtual void eat(string food) {
        cout << "The father " << get_name() << " eat " << food << endl;
    }
};

class son : public father {
// private:
//     string name;
//     int age;
public:
    son(string name, int age) : father(name, age) {}
    void eat(string food) {
        cout << "The son " << get_name() << " eat " << food << endl;
    }
};

class daughter : public father {
// private:
//     string name;
//     int age;
public:
    daughter(string name, int age) : father(name, age) {}
    void eat(string food, bool happy) {
        cout << "The daughter " << get_name() 
        << " eat " << food << (happy ? " and happy" : " and not happy") << endl;
    }
};

int main() {
    // 隐藏
    daughter* d = new daughter("Lin", 12);
    d->eat("tomato", true);
    // d->eat("tomato"); // error

    // 多态, 虚函数覆写
    father* s = new son("Yong", 13);
    s->eat("Tofu");
    // 重写参数列表个数和类型得一致
    // father* rd = new daughter("Lin", 12);
    // rd->eat("tomato", true); // error
    return 0;
}