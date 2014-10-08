#include<iostream>
using namespace std;
void factorialCola(int n, int &res)
{
  if(n > 0) { res = res * n; factorialCola(n-1, res); }
}

int main()
{
  int res = 1;
  factorialCola(5, res);
  std::cout << res << endl;
}
