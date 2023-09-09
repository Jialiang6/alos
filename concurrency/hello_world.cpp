#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void hello() {
    cout << "Hello World!" << endl;
}

int main() {
    thread t(hello);
    t.join();
    return 0;
}