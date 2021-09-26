#include <coroutine>
#include <iostream>
#include <chrono>
#include <thread>

auto dbg = [](const char* s) {
    std::cout << "Function " << s << " called.\n";
};

struct Sleeper {
    bool await_ready() const noexcept {
        dbg(__PRETTY_FUNCTION__);
        return false;
    }
    void await_suspend(std::coroutine_handle<> h) const {
        dbg(__PRETTY_FUNCTION__);
        auto t = std::jthread([h,l = length] {
            dbg(__PRETTY_FUNCTION__);
            std::this_thread::sleep_for(l);
            h.resume();
        });
    }
    void await_resume() const noexcept {
        dbg(__PRETTY_FUNCTION__);
    }
    const std::chrono::duration<int, std::milli> length;
};

struct Task {
    // The coroutine level type
    struct promise_type {
        Task get_return_object() {
            dbg(__PRETTY_FUNCTION__);
            return {};
        }
        std::suspend_never initial_suspend() {
            dbg(__PRETTY_FUNCTION__);
            return {};
        }
        std::suspend_never final_suspend() noexcept {
            dbg(__PRETTY_FUNCTION__);
            return {};
        }
        void return_void() {
            dbg(__PRETTY_FUNCTION__);
        }
        void unhandled_exception() {
            dbg(__PRETTY_FUNCTION__);
        }
    };
};


Task myCoroutine() {
    dbg(__PRETTY_FUNCTION__);
    using namespace std::chrono_literals;
    auto before = std::chrono::steady_clock::now();
    co_await Sleeper{200ms};
    auto after = std::chrono::steady_clock::now();
    std::cout << "Slept for " << (after-before) / 1ms << " ms\n";
}

int main() {
    myCoroutine();
}
