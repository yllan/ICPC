#include <iostream>
#include <cstdio>
#include <cmath>
#include <deque>
#include <algorithm>

using namespace std;

double p, a, b, c, d;
int n;

inline double price(double k)
{
	return (double)p * (sin(a * k + b) + cos(c * k + d) + 2.0);
}

int main()
{
	cin >> p >> a >> b >> c >> d >> n;

	if (n == 1) {
		printf("0\n");
		return 0;
	}

	double max_decline = 0;
	double max_previous = 0;

	for (int i = 1; i <= n; i++) {
		double p_k = price(i);

		if (p_k > max_previous) 
			max_previous = p_k;
		
		max_decline = max(max_decline, max_previous - p_k);
	}

	printf("%lf\n", max_decline);
	return 0;
}