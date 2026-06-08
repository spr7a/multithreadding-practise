
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;
class counter{
    private:
    int value=0;
    mutable mutex mtx;
    public:

    void increment(){
        lock_guard<mutex> lock(mtx);
        value++;
    }
    int getvalue(){
        lock_guard<mutex> lock(mtx);
        return value;
    }

};
void work(counter &Counter,int iteration){
    for(int i=0;i<iteration;i++){
        Counter.increment();
    }
}

int main(){
    int threads=4;
    int iterations=500;
    counter Counter;
    vector<thread>threadd;
    for(int i=0;i<threads;i++){
        threadd.emplace_back(work,ref(Counter),iterations);
    }

    for(auto& t:threadd)t.join();

    cout<<Counter.getvalue()<<endl;
    return 0;

}