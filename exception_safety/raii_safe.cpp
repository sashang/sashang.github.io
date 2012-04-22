#include <iostream>
#include <stdexcept>

using namespace std;

class Resource
{
  private:
    int* pi;
  
  public:
    Resource()
    {
      cout << __func__ << endl;
      pi = new int;
    }

    ~Resource()
    {
      cout << __func__ << endl;
      delete pi;
    }
};

void h()
{
  throw logic_error((string("Bad logic in ") + string(__func__)));
}

void g()
{
  Resource r;
  h();
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
