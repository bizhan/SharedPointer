//NOTES:
// 0. Shared_ptr is a smart pointer which can share the ownership of oject (managed object).
// 1. Several shared_ptr can point to the same object
// 2. It keep a reference count to maintain how many shared_ptr are pointing to the same objects.
// 3. Shared_ptr is threads safe and not thread save.
//  a. control block is thread safe
//  b. managed object is  not
// 4. There are three ways shared_ptr will destroyed managed object.
//  a. If the last shared_ptr goes out of scope.
//  b. If you initialize shared_ptr with some other shared_ptr.
//  c. If you reset shared_ptr
// 5. Reference count doesnot work when we use reference or pointer of shared _ptr



#include <iostream>
#include <memory>
#include <thread>
using namespace std;
class Foo{
int x;
public:
Foo(int x):x{x} {}
int getX() {return x;}
~Foo() {cout<< "-Foo" <<endl;}
};

void fun(shared_ptr<Foo> sp){
  cout<< "fun: " <<sp.use_count()<<endl;
}

//could we pass unique_ptr to a function?
void fun2(unique_ptr<Foo> up){
  cout<< "fun2: " <<up->getX()<<endl;
}
int main() {
  // Memory leak if delete is not called
  Foo *f = new Foo(1);
  cout<<f->getX()<<endl;
  shared_ptr<Foo> sp1(new Foo(1));
  cout<<sp1->getX()<<endl;
  cout<<sp1.use_count()<<endl;
  //or
  shared_ptr<Foo> sp2 = make_shared<Foo>(2);
  cout<<sp2->getX()<<endl;
  cout<<sp2.use_count()<<endl;
  // share the same manage object
  shared_ptr<Foo> sp3 = sp2;
  // reference count is increased by one and is same for both sp3, sp2
  cout<<sp3->getX()<<endl;
  cout<<sp2.use_count()<<endl;
  // only one destructor is calle dor sp2 and sp3
  cout<<sp3.use_count()<<endl;
  //Shared pointer points to the reference of the another sh pointer
  shared_ptr<Foo> sp5(new Foo(5));
  // In this case reference counter stay the same
  // here sp4 save sp5 as reference not value
  shared_ptr<Foo> &sp4 = sp5;
  cout<<sp4.use_count()<<endl; // : 1

  // Here sp6 save address of sp5
  shared_ptr<Foo> *sp6 = &sp5;
  cout<<sp6->use_count()<<endl; // : 1
  //Reference counter is only incremented for copy as value
  // Not for reference or pointer (sp4 and sp6)

  // Thread example
  shared_ptr<Foo> sp7(new Foo(7));
  // Shared pointer passed as value
  // Each thread has a new shared pointer
  // and reference number increases.
  thread t1(fun, sp7), t2(fun, sp7), t3(fun, sp7);
  cout<< "main: " <<sp7.use_count()<<endl;
  t1.join(); t2.join(); t3.join();

  unique_ptr<Foo> up = make_unique<Foo>(13);
  // The two lines before does not compile
  // thread t4(fun2, up);
  // t4.join();

  // PAY ATTENTION, HOW SP ACCESS MANAGED OBJ AND ITS CB
  // sp->getX()    vs  sp.use_count()
}