#include <stdio.h>
#include <libmill.h>

#define DEFAULT_LIMIT 10000
#define NUM_OF_PRIMES_TO_GENERATE 100

// this program will print out the first hundred primes

// this coroutine will generate a "stream" (more like a filled channel) of numbers to put through the sieve
coroutine void generate_stream(chan ch) {
   for(int i = 2; i < DEFAULT_LIMIT; i++) {
       // this loop was originally going to be infinite
       // but the channels only get so big
       chs(ch, int, i);
   }
}

coroutine void cull(chan in, chan out, int p) {
    for(;;) {
        int num = chr(in, int);
        if(num % p != 0) {
            // if it isn't divisible
            // then we keep it in the channel and send it out
            chs(out, int, num);
        }
    }
}

int main() {

    chan init_ch = chmake(int, DEFAULT_LIMIT); // always bigger than default limit?
    go(generate_stream(init_ch));

    chan ch = init_ch;
    for(int i = 1; i <= NUM_OF_PRIMES_TO_GENERATE; i++) {
        int p = chr(ch, int); // whatever is available in the previous out channel must be a prime (even initially, since 2 is prime)
        printf("prime #%d is : %d\n", i, p);
        chan out = chmake(int, DEFAULT_LIMIT);
        go(cull(ch, out, p));
        ch = out;
        // definitely needs something to clean up all the memory though
    }
    chclose(init_ch);
    chclose(ch);

    return 0;

}