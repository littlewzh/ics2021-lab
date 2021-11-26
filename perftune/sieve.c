#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  assert(n + 1 < N);
  for (int i = 1; i <= n; i+=2){
    is_prime[i] = true;
    is_prime[i+1]=false;
  }
  is_prime[2]=true;
  /*for (int i = 3; i<n; i++) {
    if(is_prime[i]){
       for (int j = i*i; j < n; j += i) {
        is_prime[j] = false;
       }
    }
  }

  int *p = primes;
  for (int i = 2; i <n; i++)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;*/
  //int *p=primes;
  int cnt=0;
for(int i=2; i<=n; i++){
	if(is_prime[i]==false)
		primes[++cnt] = i;
	for(int j=1; j<=cnt&&i*primes[j]<=n; j++){
		is_primes[i*primes[j]] = true ;
		if(i%primes[j]==0)
			break;
	} 
}
  return primes;
}
