#include <xinu.h>

static unsigned int seed = 1;

/**
 *
 * Sets the random seed that will be used in future calls to rand().
 *
 * @param x
 *      the random seed to set
 */

void seed_random(uint x)
{
    seed = x;
}

/**
 *
 * Generates a pseudorandom integer in the range [0, RAND_MAX).
 *
 * @return the random integer
 */
int random(uint max)
{
    /* This uses a linear congruential generator.  The seed retains a 32-bit
     * state, but for the actual random numbers returned we take just some of
     * the upper bits, which are more random than the lower bits.  */
    seed = (seed * 1103515245) + 12345;
    return (seed >> 16) % max;
}
