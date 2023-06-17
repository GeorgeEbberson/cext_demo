"""
Python implementation of prime finding.
"""
import itertools


def get_primes(target):
    """Get the first `num` prime numbers.

    Intentionally mirrors FindFirstNPrimes from primemodule.c, so times can be
    roughly compared.
    """
    primes = [None] * target  # Force preallocation of output array.
    primes[0] = 2  # Initialise.

    idx = 1
    counter = 1  # We add 2 on entering the loop so 3 is checked first.
    while idx < target:
        counter += 2  # Only check odd numbers.
        skip = False
        for divisorIdx in range(idx):
            if not (counter % primes[divisorIdx]):
                skip = True
        if not skip:
            primes[idx] = counter
            idx += 1

    return primes


def psieve():
    """A well-optimised prime sieve algorithm in pure Python.

    Heavily based on https://stackoverflow.com/a/19391111
    """
    yield from (2, 3, 5, 7)  # Hardcoded first 4 primes

    # Record the next multiple of (double) each of the primes we've found so far
    # (except two because even numbers are skipped), and also record how much we
    # can advance the multiple next time we reach it.
    next_multiples = {}

    prime_sieve = psieve()
    next(prime_sieve)
    current_prime = next(prime_sieve)
    assert current_prime == 3
    prime_squared = current_prime * current_prime

    # For each odd number greater than 9
    for candidate in itertools.count(9, 2):
        #print(f"{candidate}: {next_multiples}")
        if candidate in next_multiples:
            # If the number is in our dictionary, we already know it's a
            # composite number so we recall how much we can advance by.
            # We pop() it from the dictionary (i.e. remove it), but we will
            # add a later multiple of it below.
            step = next_multiples.pop(candidate)
        elif candidate < prime_squared:
            # If it's not in our dictionary of multiples then it must be prime,
            # so yield it and, when we resume, cancel the rest of this loop (so
            # we don't add it to our "not prime" dictionary). Only do this for
            # numbers up to the current_prime squared (which is, by definition,
            # not prime).
            yield candidate
            continue
        else:
            # The number is equal to our current_prime squared, so now we start
            # looking for primes from here to the *next* prime squared. Update
            # all our state variables. Because i must be odd here, adding a step
            # of current_prime is guaranteed to be even and therefore we can
            # skip to 2 * current_prime (which is guaranteed to be odd).
            assert candidate == prime_squared
            step = 2 * current_prime
            current_prime = next(prime_sieve)
            prime_squared = current_prime * current_prime

        # We can definitely advance candidate by step, because no more odd
        # multiple of the prime (step/2) can exist in the window
        # candidate->candidate+step. As a bonus optimisation, we can skip ahead
        # another step if candidate is already a multiple of a different number.
        candidate += step
        while candidate in next_multiples:
            candidate += step

        # We preserve step for this new candidate - which has moved forward by
        # a multiple of step.
        next_multiples[candidate] = step


def get_primes_faster(target):
    """Faster implementation of getting primes."""
    sieve = psieve()
    return [next(sieve) for _ in range(target)]

get_primes_faster(10)
