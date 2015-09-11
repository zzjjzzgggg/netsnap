#include <stdio.h>
#include <stdlib.h>

#include "synqueue.h"


class Exam {
private:
    SynQueue<int> queue_;
    std::vector<std::thread> workers_;
    std::vector<bool> is_working_;
    int rst_;
private:
    void Job(const int id) {
        int item;
        while (true) {
            // doing jobs
            is_working_[id] = false;
            queue_.Pop(item);
            is_working_[id] = true;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            printf("From %d: %d, %d\n", id, rst_, item);
            rst_ = item;
        }
    }

    void WaitToFinish(const int set_num_tries) {
        int num_tries = set_num_tries;
        bool still_working;
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            printf("testing...[%d][%d]\n", num_tries, rst_);
            still_working = false;
            for (bool is_working: is_working_)
                if(is_working) {
                    still_working = true;
                    break;
                }
            if(still_working || num_tries-- > 0) continue;
            else if(!queue_.Empty()) {
                num_tries = set_num_tries;
                continue;
            } else
                break;
        }
    }
public:
    Exam(const int num_workers): rst_(0) {
        queue_.SetCapacity(5);
        is_working_.resize(num_workers);
        for (int i=0; i<num_workers; i++) {
            workers_.emplace_back([&i, this] { this->Job(i); });
            workers_[i].detach();
        }
    }

    void Execute() {
        for (int i=0; i<50; i++) queue_.Push(i);
        printf("all added\n");
        WaitToFinish(5);
    }
};


int main(int argc, char *argv[]) {
    Exam ex(std::thread::hardware_concurrency());
    ex.Execute();
    return 0;
}
