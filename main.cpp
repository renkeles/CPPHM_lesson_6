#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

static std::mutex m;

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
            ++count;
        }
    }
    return candidate-1;
}

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

int main(){

    //task_1();

    std::cout << nthPrime(1000000);

    return 0;
}
