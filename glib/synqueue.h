//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//

#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_


template <typename T>
class SynQueue {
private:
	std::queue<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
public:
	SynQueue()=default;
	SynQueue(const SynQueue&) = delete;		     // disable copying
	SynQueue& operator=(const SynQueue&) = delete; // disable assignment

	T Pop(){
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) cond_.wait(mlock);
		auto val = queue_.front();
		queue_.pop();
		return val;
	}

	/**
	 * The method is protected by a mutex, which is locked using
	 * scope guard std::unique_lock. If the queue is empty, we wait
	 * on the condition variable cond_. This releases the lock to
	 * other threads and blocks until we are notified that the
	 * condition has been met. In this case, the call to wait()
	 * returns and we check the while condition again.
	 * This extra check is because conditions may experience spurious
	 * wakes: we could be wrongly notified while the queue is still empty.
	 */
	void Pop(T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty()) cond_.wait(mlock);
		item = queue_.front();
		queue_.pop();
	}

	/**
	 * Try to pop an item from the queue.
	 * return False if no item is available.
	 * (it is useful in the case of no producer threads.)
	 */
	bool TryPop(T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		if(queue_.empty()) return false;
		item = queue_.front();
		queue_.pop();
		return true;
	}

	void Push(const T& item){
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();  // unlock before notificiation to minimize mutex contention
		cond_.notify_one(); // notify one waiting thread
	}

	/**
	 * Inefficiency. Don't use this.
	 */
	bool Empty() {
		std::unique_lock<std::mutex> mlock(mutex_);
		return queue_.empty();
	}
};

#endif
