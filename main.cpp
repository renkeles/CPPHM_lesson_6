#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>

static std::mutex mtx_cout;

// Asynchronous output
struct acout
{
        std::unique_lock<std::mutex> lk;
        acout()
            :
              lk(std::unique_lock<std::mutex>(mtx_cout))
        {

        }

        template<typename T>
        acout& operator<<(const T& _t)
        {
            std::cout << _t;
            return *this;
        }

        acout& operator<<(std::ostream& (*fp)(std::ostream&))
        {
            std::cout << fp;
            return *this;
        }
};

int main(void)
{


    std::vector<std::thread> workers_cout;
    std::vector<std::thread> workers_acout;

    size_t worker(0);
    size_t threads(5);


    std::cout << "With std::cout:" << std::endl;

    for (size_t i = 0; i < threads; ++i)
    {
        workers_cout.emplace_back([&]
        {
            std::cout << "\tThis is worker " << ++worker << " in thread "
                      << std::this_thread::get_id() << std::endl;
        });
    }
    for (auto& w : workers_cout)
    {
        w.join();
    }

    worker = 0;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\nWith acout():" << std::endl;

    for (size_t i = 0; i < threads; ++i)
    {
        workers_acout.emplace_back([&]
        {
            acout() << "\tThis is worker " << ++worker << " in thread "
                    << std::this_thread::get_id() << std::endl;
        });
    }
    for (auto& w : workers_acout)
    {
        w.join();
    }

    return 0;
}
