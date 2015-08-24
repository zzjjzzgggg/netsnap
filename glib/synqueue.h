//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>


template <typename T>
class SynQueue {
private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable item_avail_cond_, space_avail_cond_;
	size_t capacity_;
public:
	SynQueue(): capacity_(std::numeric_limits<size_t>::max()) {}
	SynQueue(const size_t capacity): capacity_(capacity) {}
	SynQueue(const SynQueue&) = delete;			 // disable copying
	SynQueue& operator=(const SynQueue&) = delete; // disable assignment

	void SetCapacity(const size_t capacity) { capacity_ = capacity; }

	T Pop(){
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) item_avail_cond_.wait(mlock);
		auto val = queue_.front();
		queue_.pop();
		mlock.unlock();
		space_avail_cond_.notify_one();
		return val;
	}

	/**
	 * The method is protected by a mutex, which is locked using scope
	 * guard std::unique_lock. If the queue is empty, we wait on the
	 * condition variable item_avail_cond_. This releases the lock to
	 * other threads and blocks until we are notified that the condition
	 * has been met. In this case, the call to wait() returns and we check
	 * the while condition again.  This extra check is because conditions
	 * may experience spurious wakes: we could be wrongly notified while
	 * the queue is still empty.
	 */
	void Pop(T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) item_avail_cond_.wait(mlock);
		item = queue_.front();
		queue_.pop();
		mlock.unlock();
		space_avail_cond_.notify_one();
	}

	/**
	 * Try to pop an item from the queue. Return False if no item is
	 * available.  (it is useful in the case of no producer threads.)
	 */
	bool TryPop(T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		if(queue_.empty()) return false;
		item = queue_.front();
		queue_.pop();
		mlock.unlock();
		space_avail_cond_.notify_one();
		return true;
	}

	void Push(const T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		while(queue_.size() >= capacity_) space_avail_cond_.wait(mlock);
		queue_.push(item);
		mlock.unlock();  // unlock before notificiation to minimize mutex contention
		item_avail_cond_.notify_one(); // notify one waiting thread
	}

	bool TryPush(const T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		if(queue_.size() >= capacity_) return false;
		queue_.push(item);
		mlock.unlock();  // unlock before notificiation to minimize mutex contention
		item_avail_cond_.notify_one(); // notify one waiting thread
	}

	/**
	 * Inefficiency. Don't use this.
	 */
	bool Empty() {
		std::unique_lock<std::mutex> mlock(mutex_);
		return queue_.empty();
	}

	const size_t GetSize() const {
		return queue_.size();
	}

};

#endif
