#include <cstdio>
#include <climits>
#include <cstdlib>
#include <cstring>

#include <map>

std::map<long long, long long> answer;

typedef char MultiFactorial[20]; // Descending.
typedef char Factorization[20];

int primes[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
Factorization factorial[64];

void divide(Factorization a, Factorization b)
{
  for (int i = 0; i < 20; i++) a[i] -= b[i];
}

// Calculate f(k)
long long f(MultiFactorial k)
{
  // Only one exponent.
  if (k[1] == 0) return 1LL;

  int sum = 0;
  for (char *p = k; sum += *p, *p; p++);

  // Calculate the multi-factorial (sum!) / (k.exponent[0]! * k.exponent[1]! * ... )
  Factorization multifact;
  memmove(multifact, factorial[sum], 20);

  for (char *p = k; divide(multifact, factorial[*p]), *p; p++);

  long long result = 1LL;
  for (int i = 0; i < 20; i++)
    for (int d = multifact[i]; d > 0; d--) 
      result *= primes[i];
  return result;
}

// Generate the factorization of factorial. Ex. 4! = 2^3 * 3^1.
void generate_factorial()
{
  factorial[2][0] = 1;
  for (int i = 3; i < 64; i++) {
    strcpy(factorial[i], factorial[i - 1]);
    int m = i;
    for (int p = 0; m > 1 && p < 20; p++) 
      while (m % primes[p] == 0) {
        factorial[i][p]++;
        m /= primes[p];
      }
  }
}

// Generate combinatorial of factorization where the exponent is decreasing.
// 2^1
//
// 2^2
// 2^1 * 3^1
//
// 2^3
// 2^2 * 3^1
//
// 2^4
// 2^3 * 3^1
// 2^2 * 3^2
// 2^2 * 3^1 * 5^1
// 2^1 * 3^1 * 5^1 * 7^1 
void generate_candidate_recursive(char buf[20], int pos, int left, long long value)
{
  if (left == 0) {
    for (; pos < 20; pos++) buf[pos] = 0;
    long long f_k = f(buf);
    if (f_k <= 0) return;
    if (answer.find(f_k) == answer.end() || answer[f_k] > value)
      answer[f_k] = value;
  } else {
    int upperbound = (pos > 0 && buf[pos - 1] < left) ? buf[pos - 1] : left;
    
    long long p_k = 1LL; // primes[pos] ^ k
    for (int k = 1; k <= upperbound; k++) {
      p_k *= primes[pos];
      // The following break prevents overflow
      if (p_k < 0) break;
      if (value > 1 + LLONG_MAX / p_k) break;
      if (value * p_k < value) break;

      buf[pos] = k;
      generate_candidate_recursive(buf, pos + 1, left - k, value * p_k);
    }
  }
}

void generate_candidate()
{
  char buf[20];
  for (int i = 1; i < 64; i++)
    generate_candidate_recursive(buf, 0, i, 1LL);
}

int main()
{
  long long n;
  generate_factorial();
  generate_candidate();

  while (scanf("%lld", &n) == 1) {
    printf("%lld %lld\n", n, answer[n]);
  }
  return 0;
}