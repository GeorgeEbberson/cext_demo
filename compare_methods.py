"""
Attempt to use my Cprimes module.
"""
import time

import CPrimes
import primes

NUM_TRIALS = 1
NUM_PRIMES = 10_000
EXPLAIN_STR = f"{NUM_TRIALS} trial{'s' if NUM_TRIALS > 1 else ''} of finding the first {NUM_PRIMES} primes"

METHODS = (
    ("Unoptimised C", CPrimes.get_primes),
    ("Unoptimised Python", primes.get_primes),
    ("Optimised Python", primes.get_primes_faster),
)


def time_many_runs(desc, func, runs=NUM_TRIALS, args=(NUM_PRIMES,)):
    """Run func with args runs times."""
    tic = time.perf_counter()
    for _ in range(runs):
        ans = func(*args)
    toc = time.perf_counter()
    return ans, desc, (toc - tic)


def main():
    """Compare and time different methods of calculating the first N primes."""

    # Now do some timing to show how much faster one than the other is.
    results = [time_many_runs(*x) for x in METHODS]

    # Check they all gave the same answer before we print any messages.
    for answer, _, _ in results:
        assert answer == results[0][0], "Methods do not give the same results"

    # Now print.
    for _, desc, duration in results:
        print(f"{desc} took {duration:.3f}s to do {EXPLAIN_STR}")


if __name__ == "__main__":
    main()
