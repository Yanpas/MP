//Александр Жиров, ККС-1-12
/**
Public Functions,Ctors & Dtors graph's:
  Ctors & Dtors:
    graph(const graph<T,V> &g)
    graph()
    ~graph()
  Functions:
    add(T x, T y, T w)  :bool
    size()              :unsigned int
    clear()             :void
    print()             :void
    sortW()             :void
    sortX()             :void
    sortY()             :void
    searchBCC()         :void
    clearTMPgraph()     :void
    calculateGraph()    :void
    search_euler_path() :void
    searchStronglyConnectedComponents() :void

    operator()(const unsigned int i)    :void
    operator[](const unsigned int i)    :V &
    operator=(graph<T,V> &g)            :graph&<T,V>

Variables:
    root        : Вектор структур edge
    root2       : Добавляет граф в map после выполнения функции calculateGraph()
    root2T      : Добавляет транспонированный граф в map после выполнения функции calculateGraph()
    vertices    : Добавляет все вершины в map после выполнения функции calculateGraph()

*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "stdlib.h"
using namespace std;
template<typename T, typename V>
struct edge
{
    T x;
    T y;
    V w;
    edge(T x1,T y1,V w1):x(x1),y(y1),w(w1)
    {}
};
template<typename T, typename V>
class graph
{
private:
    vector< edge<T,V> > root;
    map< T, map<T,V> > root2;
    map< T, map<T,V> > root2T;
    map< T,int> vertices;
    class cmpX
    {
    public:
        bool operator()(const edge<T,V> &a, const edge<T,V> &b)
        {
            return (a.x < b.x);
        }

    };
    class cmpY
    {
    public:
        bool operator()(const edge<T,V> &a, const edge<T,V> &b)
        {
            return (a.y < b.y);
        }

    };
    class cmpW
    {
    public:
        bool operator()(const edge<T,V> &a, const edge<T,V> &b)
        {
            return (a.w < b.w);
        }

    };
    void dfs1(T v, map <T,bool> &used, vector<T> &order)
    {
        used[v] = true;
        for (auto i=(root2)[v].begin(); i!=(root2)[v].end(); i++)
            if (!used[ (*i).first ])
                dfs1 ((*i).first,used,order);
        order.push_back (v);
    }
    void dfs2(T v,map <T,bool> &used, vector<T> &components)
    {
        used[v] = true;
        components.push_back (v);
        for (auto i=(root2T)[v].begin(); i!=(root2T)[v].end(); i++)
            if (!used[ (*i).first ])
                dfs2 ((*i).first,used,components);
    }
    void dfs3(T x, T parent, vector<T> &stk, map<T,int> &low,map<T,int> &seen, int counter)
    {
        int kids = 0, art = 0;
        low[x] = seen[x] = ++counter;
        stk.push_back(x);
        for(auto i=root2[x].begin(); i!=root2[x].end(); i++)
        {
            T y =(*i).first;
            if (y==parent) continue;
            if (seen[y] == 0)
            {
                dfs3(y, x,stk,low,seen,counter);
                low[x] = min(low[x], low[y]);
                art |= (low[y] >= seen[x]);
                kids++;

                if (low[y] == seen[y])
                {
                    cout<<"Ребро "<<x<<" "<<y<< " является мостом"<<endl;
                }
            }
            else
            {
                low[x] = min(low[x], seen[y]);
            }

        }
        if (parent == T())
            art = (kids >= 2);
        if (art)
            cout<<"Вершина "<<x<< " - точка сочленения."<<endl;
        if (low[x] == seen[x])
        {
            cout<<"Двусвязные компоненты: ";
            while (true)
            {
                T y = stk.back();
                stk.pop_back();
                cout<<y<<" ";
                if (y == x) break;
            }
            cout<<endl;
        }

    }
    void search_euler(T v, map< T, map<T,V> > &D)
    {
        for (auto i = vertices.begin(); i!=vertices.end(); i++)

            if (D[v][(*i).first]) // если ребро есть
            {
                //cout<<(D[v][(*i).first]) <<endl;
                // проходим по нему
                D[v][(*i).first] = 0;
                D[(*i).first][v] = 0;
                search_euler ((*i).first, D);
            }
        cout<<v<<" ";
    }
public:
    graph()
    {
        /* root=new vector< edge<T,V> >;
         root2 = new map< T, map<T,V> >;
         root2T= new map< T, map<T,V> >;
         vertices=new map<T,int>; */
    }
    void clear()
    {
        root.clear();
        root2.clear();
        root2T.clear();
        vertices.clear();
    }
    ~graph()
    {
        clear();
    }
    bool add(T x, T y, V w)
    {
        root.push_back(edge<T,V>(x,y,w));
        return 1;
    }
    graph(const graph<T,V> &g)
    {
        root=new vector< edge<T,V> >;
        root2=new vector< T, map<T,V> >;
        for(auto i=(g.root).begin(); i!=(g.root).end(); i++)
            add((*i).x,(*i).y,(*i).w);
    }
    void clearTMPgraph()
    {
        root2.clear();
        root2T.clear();
        vertices.clear();
    }
    void calculateGraph()
    {
        root2.clear();
        root2T.clear();
        vertices.clear();
        for(auto i=root.begin(); i!=root.end(); i++)
        {
            vertices[(*i).x]++;
            vertices[(*i).y]++;
            root2[(*i).x][(*i).y]=(*i).w;
            root2T[(*i).y][(*i).x]=(*i).w;
        }
    }

    V& operator[](const size_t i)
    {
        return root[i].w;
    }
    void operator()(const size_t i)
    {
        cout<<"x="<<root[i].x<<"\ty="<<root[i].y<<"\tw="<<root[i].w<<endl;
    }
    graph<T,V>& operator=(graph<T,V> &g)
    {
        if (g.root.begin()==root.begin()) return *this;
        clear();
        for(auto i=g.root.begin(); i!=g.root.end(); i++)
            add((*i).x,(*i).y,(*i).w);
        return *this;
    }
    ///вывод на экран ребер и весов
    void print()
    {
        cout<<"x\ty\tweight"<<endl;
        for(auto i=root.begin(); i!=root.end(); i++)
            cout<<(*i).x<<"\t"<<(*i).y<<"\t"<<(*i).w<<endl;
    }
    ///количество ребер в графе
    size_t size()
    {
        return root.size();
    }
    ///сортировка по "левой" вершине в ребре

    void sortX()
    {
        sort(root.begin(),root.end(), cmpX());
    }


///сортировка по "правой" вершине в ребре
    void sortY()
    {
        sort(root.begin(),root.end(), cmpY());
    }
///сортировка по весам в ребрах
    void sortW()
    {
        sort(root.begin(),root.end(), cmpW());
    }
    void searchStronglyConnectedComponents()
    {
        map <T,bool> used;
        vector<T> order, components;
        for(auto i=vertices.begin(); i!=vertices.end(); i++)
        {
            if (!used[(*i).first]) dfs1((*i).first,used,order);
        }
        used.clear();
        size_t k=0;
        for (auto i=vertices.begin(); i!=vertices.end(); i++)
        {
            T v = order[vertices.size()-1-k];
            k++;
            if (!used[v]) dfs2 (v,used,components);
            for(size_t l=0; l<components.size(); cout<<components[l++]<<" ");
            if (components.size()!=0) cout<<endl;
            components.clear();
        }

    }
    void search_euler_path()
    {
        map<T,int> degree;
        for(auto i=vertices.begin(); i!=vertices.end(); i++)
            for(auto j=vertices.begin(); j!=vertices.end(); j++)
                if (root2[(*i).first][(*j).first])
                    ++degree[(*i).first];
        int count = 0;
        T j=(*vertices.begin()).first;
        for (auto i = degree.begin(); i!=degree.end(); i++)
        {
            if ((*i).second % 2 != 0)
            {
                ++ count;
                if (count > 2)
                {
                    cout<<"NOT FOUND"<<endl;
                    return;
                };
                j = (*i).first;
            }
        }
        cout<<"FOUND!!!"<<endl;
        search_euler(j,root2);
        cout<<endl;
    }
    void searchBCC()
    {
        vector<T> stk;
        map<T,int> low;
        map<T,int> seen;
        for(auto i=vertices.begin(); i!=vertices.end(); i++)
            if (seen[(*i).first] == 0) dfs3((*i).first,T(),stk,low,seen,0);

    }
};

//void enterAnySybol();
void enterAnySybol()
{
    cout<<"Enter any symbol"<<endl;
    char anykey;
    cin>>anykey;
    if (system("clear")) system( "cls" );
}
int main()
{
    graph<int,int> g1;
    graph<int,int> g;
    g.add(1,4,4);
    g.add(2,2,3);
    g.add(8,1,2);
    g.add(4,3,1);
    g1=g;
    g.add(3,2,3);
    g1.add(4,5,1);
    g.print();
    g1.print();
    enterAnySybol();
    graph<char,int> g2;
    g2.add('A','B',7);
    g2.add('A','D',5);
    g2.add('B','D',9);
    g2.add('B','C',8);
    g2.add('B','E',7);
    g2.add('C','E',5);
    g2.add('D','E',15);
    g2.add('D','F',6);
    g2.add('E','F',8);
    g2.add('E','G',9);
    g2.add('F','G',11);
    g2.print();
    enterAnySybol();
    g.sortX();
    g.print();
    enterAnySybol();
    g.sortY();
    g.print();
    enterAnySybol();
    g.sortW();
    g.print();
    cout<<"g[0]=5"<<endl;
    g[0]=5;
    enterAnySybol();
    cout<<"g[0]="<<g[0]<<endl;
    enterAnySybol();
    g.print();
    cout<<g.size()<<endl;
    g(0);
    enterAnySybol();
    g2.print();
    enterAnySybol();
    graph<char,int> g3;
    g3.add('a','b',1);
    g3.add('b','e',1);
    g3.add('b','c',1);
    g3.add('b','f',1);
    g3.add('c','d',1);
    g3.add('c','g',1);
    g3.add('d','h',1);
    g3.add('d','c',1);
    g3.add('e','a',1);
    g3.add('e','f',1);
    g3.add('f','g',1);
    g3.add('g','f',1);
    g3.add('h','g',1);
    g3.add('h','d',1);
    g3.print();
    g3.calculateGraph();
    cout<<"StronglyConnectedComponents:"<<endl;
    g3.searchStronglyConnectedComponents();
    cout<<"euler path:"<<endl;
    g3.search_euler_path();
    enterAnySybol();
    graph<int,int> g4;
    g4.add(1,2,1);
    g4.add(1,3,1);
    g4.add(1,4,1);
    g4.add(1,5,1);
    g4.add(2,1,1);
    g4.add(2,3,1);
    g4.add(2,4,1);
    g4.add(2,5,1);
    g4.add(3,2,1);
    g4.add(3,1,1);
    g4.add(3,4,1);
    g4.add(3,5,1);
    g4.add(4,1,1);
    g4.add(4,3,1);
    g4.add(4,2,1);
    g4.add(4,5,1);
    g4.add(5,1,1);
    g4.add(5,3,1);
    g4.add(5,2,1);
    g4.add(5,4,1);
    g4.print();
    g4.calculateGraph();
    cout<<"StronglyConnectedComponents:"<<endl;
    g4.searchStronglyConnectedComponents();
    cout<<"euler path:"<<endl;
    g4.search_euler_path();
    enterAnySybol();
    graph<int,int> g5;
    g5.add(1,2,1);
    g5.add(1,3,1);
    g5.add(1,5,1);
    g5.add(2,1,1);
    g5.add(2,3,1);
    g5.add(3,1,1);
    g5.add(3,6,1);
    g5.add(5,1,1);
    g5.add(5,6,1);
    g5.add(6,5,1);
    g5.add(6,3,1);
    g5.print();
    g5.calculateGraph();
    cout<<"euler path:"<<endl;
    g5.search_euler_path();
    enterAnySybol();
    graph<int,int> g6;
    g6.add(1,2,1);
    g6.add(1,4,1);
    g6.add(2,1,1);
    g6.add(2,3,1);
    g6.add(2,4,1);
    g6.add(3,2,1);
    g6.add(4,1,1);
    g6.add(4,2,1);
    g6.add(4,6,1);
    g6.add(4,7,1);
    g6.add(5,8,1);
    g6.add(6,4,1);
    g6.add(6,7,1);
    g6.add(7,4,1);
    g6.add(7,6,1);
    g6.add(8,5,1);
    g6.print();
    g6.calculateGraph();
    g6.searchBCC();
    enterAnySybol();
    g3.print();
    g3.searchBCC();
    return 0;
}
