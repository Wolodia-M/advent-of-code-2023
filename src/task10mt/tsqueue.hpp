#include "condition_variable"
#include "mutex"
#include "queue"

template <class T> class ThreadSafeQueue {
private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;
  std::condition_variable cond_;
  bool started = false;

public:
  void push(const T &value) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->queue_.push(value);
  }

  bool try_pop(T &value) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->cond_.wait(lock, [this]() { return this->started; });
    if (this->queue_.empty()) {
      return false;
    }
    value = this->queue_.front();
    this->queue_.pop();
    return true;
  }

  bool empty() const {
    std::unique_lock<std::mutex> lock(this->mutex_);
    return this->queue_.empty();
  }
  void run() {
    this->started = true;
    this->cond_.notify_all();
  }
};