#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

using namespace std;

class AtomicCounter{
    private:
    atomic<int>value={0};
    
    public:

    void increment(){
        int old_value=value.load(memory_order_relaxed);
        int new_value;
        do{
            new_value= old_value + 1;
        }
        while(!value.compare_exchange_weak(old_value,new_value,memory_order_release,memory_order_relaxed));

    }
    int get() const{
        return value.load(memory_order_acquire);
    }

};

void work(AtomicCounter& counter, int iterations) {
    for (int i = 0; i < iterations; ++i) {
        counter.increment();
    }
}

int main() {
    AtomicCounter counter;
    const int num_threads = 4;
    const int iterations = 100000;

    vector<thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(work, ref(counter), iterations);
    }

    for (auto& t : threads) t.join();

    cout << "Final value (optimistic locking): " << counter.get() << endl;
    return 0;
}