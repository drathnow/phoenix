#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <iterator>
#include <mutex>

#include "threadpool.h"

using namespace std;
using namespace zios::foundation;

static mutex mtx;

static void Foo()
{
    std::unique_lock<mutex> lock(mtx);
    cout << "Foo is running" << endl;
}

struct Bar
{
    void Foo() {
        std::unique_lock<mutex> lock(mtx);
        cout << "Foo Bar is Running" << endl;
    }
};

class Runner
{
public:
    Runner() {}
    ~Runner() {}

    void execute() {
        std::unique_lock<mutex> lock(mtx);
        cout << "Runner is execute" << endl;
    }
};

int main(int argc, char **argv)
{

    Bar b;
    Runner* r1 = new Runner();
    std::thread t1(&Runner::execute, r1);
    std::thread t2(&Foo);
    std::thread t3(&Bar::Foo, &b);

    std::this_thread::sleep_for(std::chrono::seconds(20));

    return 0;
}
