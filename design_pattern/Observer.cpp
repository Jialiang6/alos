#include <iostream>
#include <string>
#include <list>
using namespace std;

/*
    设计模式4: 观察者模式
      定义一种一（被观察类）对多（观察类）的关系，
      让多个观察对象同时监听一个被观察对象，
      被观察对象状态发生变化时，会通知所有的观察对象，使他们能够更新自己的状态。

    观察者模式中存在两种角色：
      观察者：
        内部包含被观察者对象，
        当被观察者对象的状态发生变化时，更新自己的状态。（接收通知更新状态）
      被观察者：
        内部包含了所有观察者对象，
        当状态发生变化时通知所有的观察者更新自己的状态。（发送通知）

    应用场景：
        当一个对象的改变需要同时改变其他对象，且不知道具体有多少对象有待改变时，
        应该考虑使用观察者模式；一个抽象模型有两个方面，
        其中一方面依赖于另一方面，
        这时可以用观察者模式将这两者封装在独立的对象中使它们各自独立地改变和复用
*/


class Subject;

//观察者 基类 （内部实例化了被观察者的对象sub）
class Observer
{
protected:
    string name;
    Subject *sub;

public:
    Observer(string name, Subject *sub)
    {
        this->name = name;
        this->sub = sub;
    }
    virtual void update() = 0;
};

class StockObserver : public Observer
{
public:
    StockObserver(string name, Subject *sub) : Observer(name, sub) {}
    void update();
};

class NBAObserver : public Observer
{
public:
    NBAObserver(string name, Subject *sub) : Observer(name, sub) {}
    void update();
};


//被观察者 基类 （内部存放了所有的观察者对象，以便状态发生变化时，给观察者发通知）
class Subject
{
protected:
    list<Observer *> observers;

public:
    string action; //被观察者对象的状态
    virtual void attach(Observer *) = 0;
    virtual void detach(Observer *) = 0;
    virtual void notify() = 0;
};

class Secretary : public Subject
{
    void attach(Observer *observer)
    {
        observers.push_back(observer);
    }

    void detach(Observer *observer)
    {
        list<Observer *>::iterator iter = observers.begin();
        while (iter != observers.end())
        {
            if ((*iter) == observer)
            {
                observers.erase(iter);
                return;
            }
            ++iter;
        }
    }

    void notify()
    {
        list<Observer *>::iterator iter = observers.begin();
        while (iter != observers.end())
        {
            (*iter)->update();
            ++iter;
        }
    }
};

void StockObserver::update()
{
    cout << name << " 收到消息：" << sub->action << endl;
    if (sub->action == "老板来了!")
    {
        cout << "我马上关闭股票，装做很认真工作的样子！" << endl;
    }
}

void NBAObserver::update()
{
    cout << name << " 收到消息：" << sub->action << endl;
    if (sub->action == "老板来了!")
    {
        cout << "我马上关闭NBA，装做很认真工作的样子！" << endl;
    }
}


int main()
{
    Subject *boss = new Secretary();
    Observer *xm = new NBAObserver("小美", boss);
    Observer *xs = new NBAObserver("小帅", boss);
    Observer *ds = new StockObserver("大傻", boss);

    boss->attach(xm);
    boss->attach(xs);
    boss->attach(ds);

    boss->action = "去吃饭了！";
    boss->notify();

    cout << endl;

    boss->action = "老板来了!";
    boss->notify();
    return 0;
}

/*
小美 收到消息：去吃饭了！
小帅 收到消息：去吃饭了！
大傻 收到消息：去吃饭了！

小美 收到消息：老板来了!
我马上关闭NBA，装做很认真工作的样子！
小帅 收到消息：老板来了!
我马上关闭NBA，装做很认真工作的样子！
大傻 收到消息：老板来了!
我马上关闭股票，装做很认真工作的样子！
*/
