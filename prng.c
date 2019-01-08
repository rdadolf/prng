#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "prng.h"

#ifdef USE_RAND
  #define SRAND(seed) srand(seed)
  #define RAND() rand()
#else
  struct prng_rand_t state;
  #define SRAND(seed) prng_srand(seed,&state)
  #define RAND() prng_rand(&state)
#endif

int main(int argc, char **argv) {
  int i, fd, status, written;

  SRAND(1);
  #define N 10000000
  uint32_t *diehard = (uint32_t *)malloc(N*sizeof(uint32_t));
  #ifdef USE_RAND
  for( i=0; i<N; i++ ) {
    diehard[i] = RAND();
    //printf("%ld ", diehard[i]);
  }
  #else
  for( i=0; i<N/2; i++ ) {
    uint64_t rn = RAND();
    ((uint64_t *)diehard)[i] = rn;
    //printf("%"PRIu64" ", rn);
  }
  #endif
  fd = open("numbers", O_WRONLY|O_CREAT|O_TRUNC, 0660);
  written = 0;
  while( written<N*sizeof(uint32_t) ) {
    status = write(fd, diehard, N*sizeof(uint32_t)-written);
    assert(status>=0 && "Failed write");
    written += status;
  }
  close(fd);
  return 0;
}
