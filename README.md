# C++ Coroutines Cheatsheet

## Setup
1. **Compiler**: MSVC (v143) or any C++20+ compatible compiler
2. **Flags**: `/await` for MSVC, `-fcoroutines` for GCC/Clang
3. **Headers**: `<coroutine>`

## Cases Overview

### 1️⃣ Basic Suspension
- `co_await` demonstration
- `suspend_always` vs `suspend_never`
- Coroutine state machine basics

### 2️⃣ Generator Pattern
- `co_yield` implementation
- Lazy value generation
- Coroutine handle management

### 3️⃣ Async Simulation
- Simulating async operations
- Suspension points
- Return value handling

### 4️⃣ Coroutine Chains
- Multiple coroutine coordination
- Handle management patterns
- Suspension/resumption flow

### 5️⃣ Error Handling
- Exception propagation
- `unhandled_exception` in promise
- Exception_ptr usage

### 6️⃣ Coroutine vs Threads
- Lightweight vs OS threads
- Suspension vs preemption
- Use case comparisons

## Key Types
- `coroutine_handle`: Controls coroutine execution
- `suspend_always`: Always suspends
- `suspend_never`: Never suspends
- `promise_type`: Required interface for coroutines

## Usage Patterns
1. Generators: Lazy value sequences
2. Async Tasks: Non-blocking operations
3. State Machines: Suspension as state transitions
4. Pipelines: Chained processing

## Result
```bash
=== Starting Case 1 ===

=== Case 1: Basic Coroutine Suspension ===
Creating coroutine
Coroutine started
Coroutine created (suspended)

=== Starting Case 2 ===

=== Case 2: Generator Pattern ===
Generator created (suspended)
Generated value: 0
Generated value: 1
Generated value: 2
Generated value: 3
Generated value: 4

=== Starting Case 3 ===

=== Case 3: Async Simulation ===
Launching async task
Async work started...
Main thread continues

=== Starting Case 4 ===

=== Case 4: Coroutine Chains ===
First task step 1
Second task step 1
Coroutines created, would manage handles in real code

=== Starting Case 5 ===

=== Case 5: Error Handling ===
Calling coroutine that throws
Coroutine that will throw

=== Starting Case 6 ===

=== Case 6: Coroutine vs Threads ===
Thread approach:
  - Thread running (OS thread)
Coroutine approach:
  - Coroutine running (can be on any thread)
Key Differences:
  - Coroutines are lighter weight than threads
  - Coroutines can suspend/resume without OS involvement
  - Threads have true parallelism

=== Starting Case 7 ===

=== Case 7: std::generate & Coroutines ===
Fibonacci sequence via std::generate: 0 1 1 2 3 5 8 13 21 34 
```