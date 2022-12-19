#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

template <typename T>
class ThreadsafeQueue
{
    std::queue<T>           _queue;
    mutable std::mutex      _queue_mutex;
    std::condition_variable _queue_waiting_condition;

public:

    ThreadsafeQueue() = default;
    ThreadsafeQueue(ThreadsafeQueue const& other)
    {
        std::lock_guard<std::mutex> locker(other._queue_mutex);
        _queue = other._queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
        _queue.push(std::move(new_value));
        _queue_waiting_condition.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> locker(_queue_mutex);
        _queue_waiting_condition.wait(locker,[this]{return !_queue.empty();});
        value = std::move(_queue.front());
        _queue.pop();
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
    
        if (_queue.empty())
            return false;
    
        value = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
        return _queue.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> locker(_queue_mutex);
        return _queue.size();
    }
};