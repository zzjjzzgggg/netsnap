#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include <functional>
#include <vector>
#include <thread>

class ThreadPool {
private:
    SynQueue<int> queue_;
    std::vector<std::thread> workers_;

public:
	ThreadPool();

	void AssignJob (std::function<void()> job) { WorkerPool.emplace_back(job); }
	void Start() { for(std::thread& t: WorkerPool) t.join(); }

};

#endif /* __THREADPOOL_H_ */
