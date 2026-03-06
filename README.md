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