#include <iostream>
#include <coroutine>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std::chrono_literals;

// =============================================
// Basic Coroutine Types
// =============================================
struct Task
{
	struct promise_type
	{
		Task get_return_object() { return {}; }
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() {}
	};
};

struct Generator
{
	struct promise_type
	{
		int current_value;

		Generator get_return_object()
		{
			return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
		}
		std::suspend_always initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		std::suspend_always yield_value(int value)
		{
			current_value = value;
			return {};
		}
		void return_void() {}
		void unhandled_exception() {}
	};

	std::coroutine_handle<promise_type> coro;
	explicit Generator(std::coroutine_handle<promise_type> h) : coro(h) {}
	~Generator()
	{
		if (coro)
			coro.destroy();
	}

	int value() { return coro.promise().current_value; }
	bool next()
	{
		if (!coro.done())
		{
			coro.resume();
			return !coro.done();
		}
		return false;
	}
};

struct GeneratorWithIterator : Generator {
    using Generator::Generator;

    struct iterator {
        std::coroutine_handle<promise_type> coro;
        bool done;

        void operator++() { 
            coro.resume(); 
            done = coro.done(); 
        }
        int operator*() const { return coro.promise().current_value; }
        bool operator!=(std::default_sentinel_t) const { return !done; }
    };

    iterator begin() { 
        if (coro) coro.resume(); 
        return {coro, coro.done()}; 
    }
    std::default_sentinel_t end() { return {}; }
};

// =============================================
// Case Implementations
// =============================================

void case1()
{
	std::cout << "\n=== Case 1: Basic Coroutine Suspension ===" << std::endl;

	auto simple_coro = []() -> Task
	{
		std::cout << "Coroutine started\n";
		co_await std::suspend_always{};
		std::cout << "Coroutine resumed\n";
	};

	std::cout << "Creating coroutine\n";
	auto coro = simple_coro();
	std::cout << "Coroutine created (suspended)\n";
}

void case2()
{
	std::cout << "\n=== Case 2: Generator Pattern ===" << std::endl;

	auto number_generator = []() -> Generator
	{
		for (int i = 0; i < 5; ++i)
		{
			co_yield i;
		}
	};

	auto gen = number_generator();
	std::cout << "Generator created (suspended)\n";

	while (gen.next())
	{
		std::cout << "Generated value: " << gen.value() << "\n";
	}
}

void case3()
{
	std::cout << "\n=== Case 3: Async Simulation ===" << std::endl;

	struct AsyncTask
	{
		struct promise_type
		{
			int result;

			AsyncTask get_return_object() { return {}; }
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_always final_suspend() noexcept { return {}; }
			void return_value(int v) { result = v; }
			void unhandled_exception() {}
		};
	};

	auto async_work = []() -> AsyncTask
	{
		std::cout << "Async work started...\n";
		co_await std::suspend_always{};
		std::this_thread::sleep_for(500ms);
		std::cout << "Async work completed\n";
		co_return 42;
	};

	std::cout << "Launching async task\n";
	auto task = async_work();
	std::cout << "Main thread continues\n";
	std::this_thread::sleep_for(1s);
}

void case4()
{
	std::cout << "\n=== Case 4: Coroutine Chains ===" << std::endl;

	auto first_task = []() -> Task
	{
		std::cout << "First task step 1\n";
		co_await std::suspend_always{};
		std::cout << "First task step 2\n";
	};

	auto second_task = []() -> Task
	{
		std::cout << "Second task step 1\n";
		co_await std::suspend_always{};
		std::cout << "Second task step 2\n";
	};

	std::vector<std::coroutine_handle<>> handles;

	auto coro1 = first_task();
	auto coro2 = second_task();

	// In real usage, we'd store and manage these handles properly
	std::cout << "Coroutines created, would manage handles in real code\n";
}

void case5()
{
	std::cout << "\n=== Case 5: Error Handling ===" << std::endl;

	struct TaskWithError
	{
		struct promise_type
		{
			std::exception_ptr eptr;

			TaskWithError get_return_object() { return {}; }
			std::suspend_never initial_suspend() { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }
			void return_void() {}
			void unhandled_exception() { eptr = std::current_exception(); }
		};
	};

	auto failing_coro = []() -> TaskWithError
	{
		std::cout << "Coroutine that will throw\n";
		throw std::runtime_error("Demo error");
		co_return;
	};

	try
	{
		std::cout << "Calling coroutine that throws\n";
		auto coro = failing_coro();
	}
	catch (const std::exception &e)
	{
		std::cout << "Caught error: " << e.what() << "\n";
	}
}

void case6()
{
	std::cout << "\n=== Case 6: Coroutine vs Threads ===" << std::endl;

	auto thread_work = []()
	{
		std::cout << "  - Thread running (OS thread)\n";
		std::this_thread::sleep_for(200ms);
	};

	auto coro_work = []() -> Task
	{
		std::cout << "  - Coroutine running (can be on any thread)\n";
		co_await std::suspend_always{};
	};

	std::cout << "Thread approach:\n";
	std::thread t(thread_work);
	t.join();

	std::cout << "Coroutine approach:\n";
	auto coro = coro_work();

	std::cout << "Key Differences:\n";
	std::cout << "  - Coroutines are lighter weight than threads\n";
	std::cout << "  - Coroutines can suspend/resume without OS involvement\n";
	std::cout << "  - Threads have true parallelism\n";
}

void case7()
{
    std::cout << "\n=== Case 7: std::generate & Coroutines ===" << std::endl;

    auto fibonacci_gen = []() -> Generator {
        int a = 0, b = 1;
        while (true) {
            co_yield a;
            int next = a + b;
            a = b;
            b = next;
        }
    };

    auto gen = fibonacci_gen();
    std::vector<int> fib_numbers(10);

    // std::generate expects a callable (Lambda)
    // We wrap the coroutine resume/value logic in a lambda
    std::generate(fib_numbers.begin(), fib_numbers.end(), [&gen]() {
        gen.next();
        return gen.value();
    });

    std::cout << "Fibonacci sequence via std::generate: ";
    for (int n : fib_numbers) std::cout << n << " ";
    std::cout << std::endl;
}

auto main() -> int
{
	std::cout << "=== Starting Case 1 ===\n";
	case1();

	std::cout << "\n=== Starting Case 2 ===\n";
	case2();

	std::cout << "\n=== Starting Case 3 ===\n";
	case3();

	std::cout << "\n=== Starting Case 4 ===\n";
	case4();

	std::cout << "\n=== Starting Case 5 ===\n";
	case5();

	std::cout << "\n=== Starting Case 6 ===\n";
	case6();

	std::cout << "\n=== Starting Case 7 ===\n";
	case7();

	return 0;
}