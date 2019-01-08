/* Portable pseudorandom number generator
 * Based on a 24,55 Fibonacci generator, using 55/503 rejection
 * c.f.- TAoCP, 3.2.2(7), for j=24,k=55,m=2^64
 * 
 * THIS FILE IS PUBLIC DOMAIN CODE.
 *
 * Written by Bob Adolf.
 * Attribution is appreciated but by no means legally required.
 *
 * This function is sufficient for most non-crypto applications.
 * It passes all but one of George Marsaglia's "diehard" randomness tests.
 *  (overlapping 5-tuple permutations, which is allegedly buggy)
 */

#ifndef __PRNG_H__
#define __PRNG_H__

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#define _PRNG_LAG1 (UINT16_C(24))
#define _PRNG_LAG2 (UINT16_C(55))
#define _PRNG_RAND_SSIZE ((UINT16_C(1))<<6)
#define _PRNG_RAND_SMASK (_PRNG_RAND_SSIZE-1)
#define _PRNG_RAND_EXHAUST_LIMIT _PRNG_LAG2
// 10x is a heuristic, it just needs to be large enough to remove correlation
#define _PRNG_RAND_REFILL_COUNT ((_PRNG_LAG2*10)-_PRNG_RAND_EXHAUST_LIMIT)
struct prng_rand_t {
  uint64_t s[_PRNG_RAND_SSIZE]; // Lags
  uint_fast16_t i; // Location of the current lag
  uint_fast16_t c; // Exhaustion count
};

#define PRNG_RAND_MAX UINT64_MAX

static inline uint64_t prng_rand(struct prng_rand_t *state) {
  uint_fast16_t i;
  uint_fast16_t r, new_rands=0;

  if( !state->c ) { // Randomness exhausted, run forward to refill
    new_rands += _PRNG_RAND_REFILL_COUNT+1;
    state->c = _PRNG_RAND_EXHAUST_LIMIT-1;
  } else {
    new_rands = 1;
    state->c--;
  }

  for( r=0; r<new_rands; r++ ) {
    i = state->i;
    state->s[i&_PRNG_RAND_SMASK] =
      state->s[(i+_PRNG_RAND_SSIZE-_PRNG_LAG1)&_PRNG_RAND_SMASK] +
      state->s[(i+_PRNG_RAND_SSIZE-_PRNG_LAG2)&_PRNG_RAND_SMASK];
    state->i++;
  }
  return state->s[i&_PRNG_RAND_SMASK];
}

static inline void prng_srand(uint64_t seed, struct prng_rand_t *state) {
  uint_fast16_t i;
  // Naive seed
  state->c = _PRNG_RAND_EXHAUST_LIMIT;
  state->i = 0;

  state->s[0] = seed;
  for(i=1; i<_PRNG_RAND_SSIZE; i++) {
    // Arbitrary magic, mostly to eliminate the effect of low-value seeds.
    // Probably could be better, but the run-up obviates any real need to.
    state->s[i] = i*(UINT64_C(2147483647)) + seed;
  }

  // Run forward 10,000 numbers
  for(i=0; i<10000; i++) {
    prng_rand(state);
  }
}

// Clean up our macros
#undef _PRNG_LAG1
#undef _PRNG_LAG2
#undef _PRNG_RAND_SSIZE
#undef _PRNG_RAND_SMASK
#undef _PRNG_RAND_EXHAUST_LIMIT
#undef _PRNG_RAND_REFILL_COUNT

// PRNG_RAND_MAX is exported

#endif
