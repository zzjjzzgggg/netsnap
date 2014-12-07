/*
 * ThreadPool.h
 *
 *  Created on: Dec 7, 2014
 *      Author: jzzhao
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_


#include <vector>
#include <thread>

class ThreadPool {
private:
	std::vector<std::thread> WorkerPool;

public:
	ThreadPool();

	void AssignJob (std::function<void()> job) { WorkerPool.emplace_back(job); }
	void Start() { for(std::thread& t: WorkerPool) t.join(); }

};

#endif /* THREADPOOL_H_ */
