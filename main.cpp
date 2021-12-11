#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>
#include <iterator>

static std::mutex m;
const int count = 10;
std::vector<int> vec(count);

struct pcout
{
        std::unique_lock<std::mutex> lk;
        pcout() : lk(std::unique_lock<std::mutex>(m)){}

        template<typename T>
        pcout& operator<<(const T& _t){
            std::cout << _t;
            return *this;
        }

        pcout& operator<<(std::ostream& (*fp)(std::ostream&)){
            std::cout << fp;
            return *this;
        }
};

void tJoin(std::vector<std::thread> &workers){
    for (auto& w : workers){
        w.join();
    }
}

bool isPrime(int n) {
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    int step = 4, m = (int)sqrt(n) + 1;
    for(int i = 5; i < m; step = 6-step, i += step) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int nthPrime(int n) {
    int candidate, count;
    for(candidate = 2, count = 0; count < n; ++candidate) {
        if (isPrime(candidate)) {
            std::cout << candidate << "\t";
            ++count;
        }
    }
    return candidate-1;
}

template <class Container>
void printContainer(Container& container, std::string name) {
    std::cout << name << ": ";
    std::copy(container.begin(), container.end(), std::ostream_iterator<int>(std::cout, "."));
    std::cout << std::endl;
}
/*
void pushElem(int count){
    std::thread thread([count](){
        std::thread::id thread_id = std::this_thread::get_id();
        for(int i{0}; i < count; ++i){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::mt19937 gen(time(0));
            std::uniform_int_distribution<> urd(1,99);
            auto newElem = urd(gen);
            std::cout << "Thread id = " << thread_id << " ";
            std::cout << "New element: " << newElem << " ";
            std::unique_lock<std::mutex>(m);
            vec.push_back(newElem);
            printContainer(vec, "Push print: ");
        }
    });
    thread.join();
}

void deleteMaxElem(int count){
    std::thread thread([count](){
        std::thread::id thread_id = std::this_thread::get_id();
        for(int i{0}; i < count; ++i){
            std::this_thread::sleep_for(std::chrono::microseconds (500));
            auto highScore = std::max_element( vec.begin(), vec.end() );
            std::cout << "Thread id = " << thread_id << " ";
            std::cout << "High score: " << *highScore << " ";
            std::unique_lock<std::mutex>(m);
            vec.erase(highScore);
            printContainer(vec, "Delete print: ");
        }
    });
    thread.join();
}
*/

void task_1(){
    std::vector<std::thread> workers_cout;
    std::vector<std::thread> workers_pcout;

    int worker{0};
    int threads{5};

    std::cout << "With std::cout:" << std::endl;

    for (auto i{0}; i < threads; ++i){
        workers_cout.emplace_back([&](){
            std::cout << "This is worker " << ++worker << " in thread "
                      << std::this_thread::get_id() << std::endl;
        });
    }

    tJoin(workers_cout);

    worker = 0;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "With pcout():" << std::endl;

    for (auto i{0}; i < threads; ++i){
        workers_pcout.emplace_back([&](){
            pcout() << "This is worker " << ++worker << " in thread "
                    << std::this_thread::get_id() << std::endl;
        });
    }

    tJoin(workers_pcout);
}

void task_2(){
    int simpleNum = 0;
    int count = 100; //N-simple
    std::thread th([&](){
        simpleNum = nthPrime(count);
    });
    th.join();
    std::cout << "\n" << count << " simpleNum: " << simpleNum;
}

void task_3(int count){
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> urd(1,99);

    std::generate(vec.begin(), vec.end(), [&urd, &gen](){
        return urd(gen);
    });

    std::thread thread1([count](){
        std::thread::id thread_id = std::this_thread::get_id();
        for(int i{0}; i < count; ++i){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::mt19937 gen(time(0));
            std::uniform_int_distribution<> urd(1,99);
            auto newElem = urd(gen);
            std::cout << "Thread id = " << thread_id << " ";
            std::cout << "New element: " << newElem << " ";
            std::unique_lock<std::mutex>(m);
            vec.push_back(newElem);
            printContainer(vec, "Push print: ");
        }
    });

    std::thread thread2([count](){
        std::thread::id thread_id = std::this_thread::get_id();
        for(int i{0}; i < count; ++i){
            std::this_thread::sleep_for(std::chrono::microseconds (500));
            auto highScore = std::max_element( vec.begin(), vec.end() );
            std::cout << "Thread id = " << thread_id << " ";
            std::cout << "High score: " << *highScore << " ";
            std::unique_lock<std::mutex>(m);
            vec.erase(highScore);
            printContainer(vec, "Delete print: ");
        }
    });


    thread1.join();
    thread2.join();

}


int main(){

    //task_1();
    //task_2();
    task_3(count);


    return 0;
}
