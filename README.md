### What is this?

There are some common, valid complaints with `rand()` (and `random()` and `drand48()`...), which mainly fall into two categories:

  - It's not very random.
  - It's not portable.

The first is due to the prevalence of LCG's in system libraries.
These suffer from very predictable behavior in high-dimensional space, but more importantly, they are often limited in size by the constants chosen.

The second is ironically a result of the first: in order to provide a better random number source, several other functions have cropped up, from extensions (like `drand48`) to in-place upgrades (like `random()`) to cryptographic options (like `arc4random`).

Unfortunately, most of these still cause problems for one reason or another.
  - Original `rand()` implementations are usually weak random sources.
  - Almost all are 32-bit only (and stacking 32-bit values can sometimes introduce biases, due to lower randomness in LSB's)
  - Cryptographic functions are overkill for many applications.
  - And the biggest killer: portability. The only standard-required function (`rand()`) has no specified implementation, so different systems provide different results. The constants for the `drand48` LCG are at least documented, but it suffers from randomness and size issues.

## `prng` is a portable pseudorandom number generator

I needed a portable, deterministic, and "sufficiently random" library for a past project, so I wrote one that I could reuse.

`prng` is a 24,55 lagged Fibonacci generator which operates on 64-bit values, using 10:1 rejection to eliminate correlation.
The implementation is largely based on Knuth ( 3.2.2(7) ), and it's been checked against George Marsaglia's diehard test suite.

### License and copyright

This code is placed in the PUBLIC DOMAIN.

This means that you can edit it, distribute it, sell it, or basically anything else.

My name is in the header, but you are under no legal obligation to keep it there.

Enjoy.
