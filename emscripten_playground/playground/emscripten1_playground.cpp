#include <iostream>
#include <vector>
#include <thread>
#include <chrono>


using namespace std;
using namespace std::literals;

#ifdef __EMSCRIPTEN_PTHREADS__
#error dsfsdg
#endif

#if 1
int main(int /*argc*/, char* /*argv*/ [])
{
    cout << "Hello from emscripten 1" << endl;
    return 0;
}
#endif

#if 0
int main(int /*argc*/, char* /*argv*/ [])
{
    cout << "Hello emscripten 3 -> 1" << endl;
    std::this_thread::sleep_for(2000ms);
    cout << "Hello emscripten 3 -> 2" << endl;


    cout << endl;
    return 0;
}
#endif

#if 0
void* f1(int n)
{
    for (int i = 0; i < 5; ++i) {
        std::cout << "Thread 1 executing" << endl;
        ++n;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return nullptr;
}


int main()
{
    cout << "Hello emscripten 3 -> 1" << endl;
    std::this_thread::sleep_for(2000ms);
    cout << "Hello emscripten 3 -> 2" << endl;
    int n = 0;
    std::thread t2(f1, n + 1); // pass by value
    cout << "Hello emscripten 3 -> 3" << endl;
//    t2.join();
    std::cout << "Final value of n is " << n << endl;
}
#endif

