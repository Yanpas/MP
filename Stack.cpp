//Александр Жиров, KKC-1-12
//stack
#include <iostream>

#include <vector>

//stack.h
template<typename T,typename container=std::vector<int>>
class my_stack
{
private:
    container c;
public:
    void push(const T& x)
    {
        c.push_back(x);
    }
    void pop()
    {
        c.pop_back();
    }
    bool empty()
    {
        return c.empty();
    }
    size_t size()
    {
        return c.size();
    }
    T& top()
    {
        return c.back();
    }
    void swap(my_stack &x)
    {
        c.swap(x.c);
    }
};
using namespace std;
int main()
{
    my_stack<int> a,b;
    for(int i=1;i!=10;a.push(i++));

    for(int i=0;i!=9;i++)
    {
       cout<<a.top();
       a.pop();
    }
    for(int i=9;i!=0;a.push(i--));
    b.swap(a);
    cout<<endl<<b.empty()<<endl;
     while(!b.empty())
    {
       cout<<b.top();
       b.pop();
    }
    std::vector<int> c;
    return 0;
}
