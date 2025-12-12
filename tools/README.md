# Performance Profiling Tools

## String Move Performance Profiler

This directory contains a standalone performance profiler that compares two approaches for passing string parameters:
- **Pass-by-value** (current implementation)
- **Rvalue reference** (original implementation)

### Building

The profiler is built automatically with the project:
```bash
bash compile.sh
```

### Running

```bash
./build/profile_string_moves
```

### Sample Output

```
=== String Move Performance Comparison ===

Short strings (SSO):
  Pass-by-value:    14.6 ns/op
  Rvalue reference: 12.0 ns/op
  Difference:       2.6 ns
  Overhead:         22%

Long strings (heap allocated):
  Pass-by-value:    29.4 ns/op
  Rvalue reference: 27.0 ns/op
  Difference:       2.4 ns
  Overhead:         9%


Conclusion:
Pass-by-value adds ~2.6 ns overhead per operation
This represents 1 additional move constructor call

For a typical chess game (80 moves):
Total overhead: ~0.2 microseconds (0.0002 milliseconds)
Performance impact: Negligible
```

### Interpretation

#### Short Strings (Small String Optimization)

| Metric           | Time (ns/op) |
|------------------|--------------|
| Pass-by-value    | ~14.6        |
| Rvalue reference | ~12.0        |
| **Difference**   | **~2.6**     |
| **Overhead**     | **~22%**     |

#### Long Strings (Heap Allocated)

| Metric           | Time (ns/op) |
|------------------|--------------|
| Pass-by-value    | ~29.4        |
| Rvalue reference | ~27.0        |
| **Difference**   | **~2.4**     |
| **Overhead**     | **~9%**      |

#### Key Findings

- **Pass-by-value performs 1 additional move constructor call**
- Short strings show higher relative overhead (~22%) but only 2.6 nanoseconds in absolute terms
- Long strings show lower relative overhead (~9%) as heap allocation dominates the cost
- Both approaches are extremely fast - the difference is negligible in practice

### Real-World Impact

For a typical chess game with **80 moves total**:
- **Total overhead:** ~0.2 microseconds = **0.0002 milliseconds**
- **Conclusion:** This performance difference is **negligible** for real-world usage

The pass-by-value approach offers better code flexibility and maintainability with virtually no measurable performance cost in practice.