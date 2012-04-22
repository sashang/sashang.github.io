#include <iostream>
#include <stdexcept>

using namespace std;

void h()
{
  throw logic_error((string("Bad logic in ") + string(__func__)));
}

void g()
{
  int* pi = new int;
  try
  {
    h();
  }
  catch (const exception& e)
  {
    cout << "Caught exception: \"" << e.what() << "\" in " << __func__  << endl;
    delete pi;
    throw;
  }
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
    cout << "Caught exception: \"" << e.what() << "\" in " << __func__  << endl;
  }
}

int main(int argc, char** argv) 
{
  f();
}
