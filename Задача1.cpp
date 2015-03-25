//Александр Жиров, ККС-1-12
//Задача 1
//Поиск в глубину, поиск в ширину, Алгоритм Прима, Краскала, Дейкстры, Флойда-Уоршелла
//начальный граф:
//https://upload.wikimedia.org/wikipedia/commons/a/a8/Prim_Algorithm_0.svg
#include <iostream>
#include <vector>
#include <limits.h>
#include <algorithm>
using namespace std;

template<typename It>
void Kraskal(int m, int n, It begin, It end)
{

    int cost = 0;
    vector < pair<int,int> > res;

    sort (begin, end);
    vector<int> tree_id (n);
    for (int i=0; i<=n; i++)
        tree_id[i] = i;

    for (auto i=begin; i!=end; ++i)
    {
        int a = (*i).second.first,  b = (*i).second.second,  l = (*i).first;
        if (tree_id[a] != tree_id[b])
        {
            cost += l;
            res.push_back (make_pair (a, b));
            cout<<a<<" "<<b<<endl;
            int old_id = tree_id[b],  new_id = tree_id[a];
            for (int j=0; j<=n; ++j)
                if (tree_id[j] == old_id)
                    tree_id[j] = new_id;
        }
    }
}

void prima(int n, int *g)
{
    const int INF = INT_MAX;
    vector<bool> used (n);
    vector<int> min_e (n, INF), sel_e (n, -1);
    min_e[0] = 0;
    for (int i=0; i<n; ++i)
    {
        int v = -1;
        for (int j=0; j<n; ++j)
            if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
                v = j;
        if (min_e[v] == INF)
        {
            cout << "No MST!";
            return;
        }

        used[v] = true;
        if (sel_e[v] != -1)
            cout << v+1 << " " << sel_e[v]+1 << endl;

        for (int to=0; to<n; ++to)
            if (*(g+v*n+to) < min_e[to])
            {
                min_e[to] = *(g+v*n+to);
                sel_e[to] = v;
            }
    }

}
void FU(int *D1, const int V)
{
    int D[V][V];
    for(int i=0; i<V; i++)
        for(int j=0; j<V; j++)
            D[i][j]=*(D1+i*V+j);


    for (int i=0; i<V; i++) D[i][i]=0;
    for (int k=0; k<V; k++)
        for (int i=0; i<V; i++)
            for (int j=0; j<V; j++)
                if (D[i][k] && D[k][j] && i!=j)
                    if (D[i][k]+D[k][j]<D[i][j] || D[i][j]==0)
                        D[i][j]=D[i][k]+D[k][j];

    for (int i=0; i<V; i++)
    {
        for (int j=0; j<V; j++) cout<<D[i][j]<<"\t";
        cout<<endl;
    }
}
void Dijkstra(int *GR, int st, const int V)
{
    int distance[V], count, index, i, u, m=st+1;
    bool visited[V];
    for (i=0; i<V; i++)
    {
        distance[i]=INT_MAX;
        visited[i]=false;
    }
    distance[st]=0;
    for (count=0; count<V-1; count++)
    {
        int min=INT_MAX;
        for (i=0; i<V; i++)
            if (!visited[i] && distance[i]<=min)
            {
                min=distance[i];
                index=i;
            }
        u=index;
        visited[u]=true;
        for (i=0; i<V; i++)
            if (!visited[i] && *(GR+u*V+i) && distance[u]!=INT_MAX &&
                    distance[u]+*(GR+u*V+i)<distance[i])
                distance[i]=distance[u]+*(GR+u*V+i);
    }
    cout<<"Путь из стартовой вершины до остальных:"<<endl;
    for (i=0; i<V; i++) if (distance[i]!=INT_MAX)
            cout<<m<<" > "<<i+1<<" = "<<distance[i]<<endl;
        else cout<<m<<" > "<<i+1<<" = "<<"маршрут недоступен"<<endl;
}
void BFS( int unit, int *GM, int n, bool *visited)
{
    int *queue=new int[n];
    int count, head;
    for (int i=0; i<n; i++) queue[i]=0;
    count=0;
    head=0;
    queue[count++]=unit;
    visited[unit]=true;
    while (head<count)
    {
        unit=queue[head++];
        cout<<unit+1<<" ";
        for (int i=0; i<n; i++)
            if (*(GM+unit*n+i) && !visited[i])
            {
                queue[count++]=i;
                visited[i]=true;
            }
    }
    delete []queue;
}
void DFS(int st, int *graph, int n, bool *visited)
{
    int r;
    cout<<st+1<<" ";
    visited[st]=true;
    for (r=0; r<n; r++)
        if ( ( (*(graph+st*n+r)) !=0)  && (!visited[r]))
            DFS(r,graph,n, visited);
}
void convertMatix(int *a, int n1)
{
    for (int i=0; i<n1; i++)
    {

        for (int j=0; j<n1; j++)
            if (*(a+i*n1+j)==0)
                *(a+i*n1+j)=INT_MAX;

    }
}
void printMatrix(int *a, int n1)
{
    for (int i=0; i<n1; i++)
    {

        for (int j=0; j<n1; j++)
            cout<<*(a+i*n1+j)<<"\t";
        cout<<endl;
    }
}
void toFalse(bool *visited, int n)
{
    for (int i=0; i<n; i++)
        visited[i]=false;
}
int main()
{
    int start;
    const int n=7;
    int graph[n][n] = //Граф 1
    {
        {0, 7, 0, 5,  0,  0,  0},
        {7, 0, 8, 9,  7,  0,  0},
        {0, 8, 0, 0,  5,  0,  0},
        {5, 9, 0, 0,  15, 6,  0},
        {0, 7, 5, 15, 0,  8,  9},
        {0, 0, 0, 6,  8,  0,  11},
        {0, 0, 0, 0,  9,  11, 0}
    };

    vector < pair < int, pair<int,int> > > g ; //Граф 2 (вес,вершина1,вершина2)
    g.push_back(make_pair(7, make_pair(1,2)));
    g.push_back(make_pair(5, make_pair(1,4)));
    g.push_back(make_pair(8, make_pair(2,3)));
    g.push_back(make_pair(9, make_pair(2,4)));
    g.push_back(make_pair(7, make_pair(2,5)));
    g.push_back(make_pair(5, make_pair(3,5)));
    g.push_back(make_pair(15, make_pair(4,5)));
    g.push_back(make_pair(6, make_pair(4,6)));
    g.push_back(make_pair(8, make_pair(5,6)));
    g.push_back(make_pair(9, make_pair(5,7)));
    g.push_back(make_pair(11, make_pair(6,7)));
    bool *visited=new bool[n];
    cout<<"Матрица смежности графа: "<<endl;
    //for (i=0; i<n; i++)
    printMatrix(&graph[0][0], n);
    toFalse(visited,n);
    cout<<"Стартовая вершина >> ";
    cin>>start;
//массив посещенных вершин
    bool *vis=new bool[n];
    cout<<"Порядок обхода в глубину: ";


    toFalse(visited,n);
    DFS(start-1,&graph[0][0],n, visited);
    toFalse(visited,n);

    cout<<endl<<"Порядок обхода в ширину: ";
    BFS(start-1, &graph[0][0], n, visited);

    cout<<endl<<"(Алгоритм Дейкстры) ";
    Dijkstra(&graph[0][0],start-1, n);


    cout<<endl<<"Матрица (Флойд-Уоршелл): "<<endl;
    FU(&graph[0][0],n);


    cout<<endl<<"(Алгоритм Прима) Ребра: "<<endl;
    convertMatix(&graph[0][0], n);
    prima(n,&graph[0][0]);

    cout<<endl<<"(Алгоритм Краскала) Ребра: "<<endl;
    Kraskal(g.size(),n, g.begin(), g.end());

}
