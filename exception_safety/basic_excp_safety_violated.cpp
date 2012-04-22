#include <iostream>
#include <stdexcept>

using namespace std;

void h()
{
  throw logic_error((string("Failure in ") + string(__func__)));
}

void g()
{
  int* pi = new int;
  h();
  delete pi;
}

void f()
{
  try 
  {
    g();
  }
  catch (const exception& e)
  {
    cout << e.what() << endl;
  }
}

int main(int argc, char** argv) 
{
  f();
}
