#include <iostream>
#include <math.h>
using std::cin;
using std::cout;
using std::endl;
int prime[3];//prime[1] is p,prime[2] is q
bool is_prime(int n)
{
    if(n==1) return false;
    if(n==2) return true;
    for(int i=2;i<=sqrt(n);i++)
    {
        if(n%i==0) return false;
    }
    return true;
}
int main()
{
    cin>>m>>n;
    n=max(abs(m),abs(n));
    m=min(abs(m),abs(n));
    for(int i=n;i<=m;i++)
    {
        if(is_prime(i))
        {

        }
    }

}