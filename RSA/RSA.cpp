#pragma GCC optimize(1)
#pragma GCC optimize(2)
#pragma GCC optimize(3)
#include <iostream>
#include <ctime>
#include <cmath>
using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::min;
using std::pow;
int prime[3]; // prime[1] is p,prime[2] is q
int m, n, N, Z, e, d;
int es = 0, how_e = -1;
bool is_prime(int n)
{
    if (n == 1)
        return false;
    if (n == 2)
        return true;
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

void find_e()
{
    es = 0;
    for (int i = 3; i <= N; i += 2)
    {
        if (gcd(i, Z) == 1)
        {
            e = i;
            if (es < how_e)
                es++;
            else
                break;
        }
    }
}
void find_d()
{
    for (int i = 1; i <= N; i++)
    {
        if ((i * e) % Z == 1 && e != d)
        {
            d = i;
            break;
        }
    }
}
int main()
{
    cin >> m >> n;
    double start = clock();
    n = max(abs(m), abs(n));
    m = min(abs(m), abs(n));
    for (int i = m; i <= n && prime[2] == 0; i += (i % 2 == 0 ? 1 : 2))
    {
        if (is_prime(i) && i & 1 == 1) // i&1 = i%2
        {
            prime[(prime[1] != 0 ? 2 : 1)] = i;
        }
    }
    N = prime[1] * prime[2];
    Z = (prime[1] - 1) * (prime[2] - 1);
    while (d == 0)
    {
        how_e++;
        find_e();
        find_d();
    }

    double end = clock();
    double last = end - start;
    cout << last << "ms" << endl;
    cout << "N=" << N << endl;
    cout << "e=" << e << endl;
    cout << "d=" << d << endl;
}