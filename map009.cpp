//Александр Жиров, ККС-1-12
//map v0.0.9

#include <iostream>
#include <map>
//#define TEST


#ifdef TEST
 size_t MEMORY=0;
#endif // TEST


using namespace std;
template<typename T>
struct node
{
    static node<T>*NIL;
    T data;
    node<T> *left;
    node<T> *right;
    node<T> *parent;
    node():left(NIL),right(NIL) {}
    node(T d, node<T> *p):data(d),parent(p),left(NIL),right(NIL) {}
    const bool is_right_son( ) const
    {
        return (parent != NIL) && (parent->right == this ) ;
    }
     const bool is_left_son( ) const {
        return (parent != NIL ) && (parent->left == this ) ;
    }
private:
    static node SENTINEL_OBJECT ;
};
template< typename T >
node<T> node<T>::SENTINEL_OBJECT =  node<T>() ;

template< typename T >
node<T> * node<T>::NIL = &node<T>::SENTINEL_OBJECT ;

template<typename K>
class default_comparator
{
public:
    bool operator()(const K &a, const K &b)
    {
        return a<b;
    }
};
template<typename K, typename V, typename CMP=default_comparator<K> >
class my_map
{

private:
    typedef pair<const K,V> node_data;
    typedef node<node_data> Node;
    Node *root;
    static Node *NIL;
    size_t SIZE;
    CMP C;
private:
    Node* find (Node*p, const K &x)
    {
        if (p == Node::NIL)
            return Node::NIL;
        if (p->data.first == x)
            return p;
        else if (x < p->data.first)
            return(find (p->left, x));
        else
            return(find (p->right,x));
    }
    void clear(Node *p)
    {

        if (p==Node::NIL) return;
        clear(p->left);
        clear(p->right);
#ifdef TEST
        cout<<"-";
#endif
        delete p;
#ifdef TEST
    MEMORY=MEMORY-1;
#endif // TEST
        if (p==root) root=Node::NIL;
#ifdef TEST
    cout<<"Memory: "<<MEMORY;
#endif // TEST
    }
    Node* insert(Node *parent, const node_data &data)
    {
        if (parent==Node::NIL) return Node::NIL;
        if (C(data.first,parent->data.first)) //parent->data.first>data.first)
        {
            if (parent->left==Node::NIL)
            {
                SIZE++;
                parent->left=new Node(data,parent);
#ifdef TEST
    MEMORY++;
#endif // TEST
                return parent->left;
            }
            else return insert(parent->left,data);
        }
        else if (C(parent->data.first,data.first)) //(parent->data.first<data.first)
        {
            if (parent->right==Node::NIL)
            {
                SIZE++;
                parent->right=new Node(data,parent);
                #ifdef TEST
    MEMORY++;
#endif // TEST
                return parent->right;
            }
            else return insert(parent->right,data);
        }
        else return parent;
    }
    Node *at(Node *parent,const K &key)
    {
        if (parent==NIL) return NIL;
        if (C(key,parent->data.first)) //(key<parent->data.first)
        {
            return at(parent->left, key);
        }
        else if (C(parent->data.first, key)) //(key>parent->data.first)
        {
            return at(parent->right, key);
        }
        else return parent;
    }
    void copy(Node* p, Node* x)
    {
#ifdef TEST
        cout<<"+";
#endif
        if (x==Node::NIL)
            return;
        if (p==Node::NIL)
        {
#ifdef TEST
            cout<<"addROOT "<<x->data.first<<", " <<endl;
#endif
            SIZE++;
            root=new Node(x->data,Node::NIL);//insert(root,x->data);
            #ifdef TEST
    MEMORY++;
#endif // TEST
            p = root;
            // p==root;
        }
        if (x->left!=Node::NIL)
        {
#ifdef TEST
            cout<<"LEFT "<<x->left->data.first<<", "<<endl;
#endif
            SIZE++;
            p->left=new Node(x->left->data, p);//insert( p,x->left->data);
            #ifdef TEST
    MEMORY++;
#endif // TEST
            copy(p->left, x->left);
        }
        if (x->right!=Node::NIL)
        {
#ifdef TEST
            cout<<"RIGHT "<<x->right->data.first<<", "<<endl;
#endif
            SIZE++;
            p->right=new Node(x->right->data, p); //  insert(  p,x->right->data);
            #ifdef TEST
    MEMORY++;
#endif // TEST
            copy(p->right, x->right);
        }
    }
    bool boolcompare(Node* x, Node* y)
    {
        if  ((x==Node::NIL)&&(y!=Node::NIL)) return false;
        if  ((x!=Node::NIL)&&(y==Node::NIL)) return false;

        if  ((x!=Node::NIL)&&(y!=Node::NIL))
        {
            if (x->data!=y->data) return false;
            if (!boolcompare(x->left, y->left)) return false;
            if (!boolcompare(x->right, y->right)) return false;
        }
        //boolcompare(x->right, y->right);
        return true;
    }
public:
    ///Ctors & Dtors
    my_map()
    {
        root=Node::NIL;
        SIZE=0;
    }
    my_map(const my_map& p)
    {
        SIZE=0;
        C=p.C;
        // Node *q=0;
        copy(Node::NIL,p.root);
        root=root;
        //delete q;
    }
    ~my_map()
    {
        clear(root);
    }
    ///OPERATORS
    V& operator[](const K &key)
    {
        return (*(insert(node_data(key,V())))).second;
    }
    bool operator==(const my_map &x)
    {
        if (SIZE!=x.SIZE) return false;
        if (root==x.root) return true;
        return boolcompare(root,x.root);
        //return (root==x.root);
    }
    my_map& operator=(const my_map &p)
    {
        if (root==p.root) return *this;
        clear(root);
        SIZE=0;
        copy(Node::NIL, p.root);
        return *this;
    }
    ///PUBLIC FUNTIONS

    V& at(const K &key)
    {
        Node *temp;
        temp=at(root,key);
        if (temp==0)
            throw exception();
        else return temp->data.second;
    }
    size_t size()
    {
#ifdef TEST
        cout<<"size: ";
#endif // defined
        return SIZE;
    }
    ///ITERATOR
    class iterator
    {
        friend class my_map;

        Node *it;

    public:
        iterator(Node *i=Node::NIL):it(i)
        {
        }
    private:
        Node* getNext(Node *i)
        {
            if ((i==Node::NIL)) return Node::NIL;

            if (i->right!=Node::NIL)
            {
                Node *t=i->right;
                while(t->left!=Node::NIL)
                    t=t->left;
                return t;
            }
            else
            {
                Node *t=i->parent;
                while (t!=Node::NIL && t->right==i)
                {
                    t=t->parent;
                    i=i->parent;
                }
                return t;
            }
        }
        Node* getPrev(Node *i)
        {

            if (i==Node::NIL) return Node::NIL->parent;
            if (i->left!=Node::NIL)
            {
                Node *t=i->left;
                while(t->right!=Node::NIL)
                    t=t->right;
                return t;
            };
            if (i->is_right_son()) return i->parent ;
            Node* pred = it ;

            do
            {
                pred = pred->parent ;
            }
            while ((pred != Node::NIL ) && pred->is_left_son() ) ;
            if (pred != Node::NIL )
                return pred->parent ;
            else
                return Node::NIL ;
        }
    public:
        bool operator!=(const iterator &r)
        {
            return it!=r.it;
        }
        bool operator==(const iterator &r)
        {
            return it==r.it;
        }
        iterator& operator++()
        {
            it=getNext(it);
            return *this;
        }
        iterator operator++(int)
        {
            Node* t=it;
            operator++();
            return iterator(t);
        }
        iterator& operator--()
        {
            it=getPrev(it);
            return *this;
        }
        iterator operator--(int)
        {
            Node* t=it;
            operator--();
           // t=getPrev(it);
            return iterator(it);
        }
        node_data& operator*()
        {
            if (it==0) throw exception();
            if (it==Node::NIL)
            {
                it=it->parent;
               return it->data;
            };
            return it->data;
        }
        node_data* operator->()
        {
            if (it==0) throw exception();
            if (it==Node::NIL)
            {
                    it=it->parent;
                    return &it->data;
            };
            return &it->data;
        }
    };
    iterator begin()
    {
        if (root==Node::NIL) return iterator(Node::NIL);
        Node *t=root;
        while(t->left!=Node::NIL) t=t->left;
        return iterator(t);
    }
    iterator end()
    {
        if (root==Node::NIL) return iterator(Node::NIL);
        Node *t=root;
        while(t->right!=Node::NIL) t=t->right;
        Node::NIL->parent=t;
        return iterator();
    }
    iterator find(const K& x)
    {
        return iterator(find(root, x) );
    }
    iterator insert(const node_data &data)
    {
        if (root==Node::NIL)
        {
            root=new Node(data,Node::NIL);
#ifdef TEST
    MEMORY++;
#endif // TEST
            SIZE++;
            return iterator(root);
        }
        return iterator(insert(root, data));
    }
};
void print(my_map<int,int> a)
{

    for(my_map<int,int>::iterator it=a.begin(); it!=a.end(); it++)
    {
        cout<<(*it).first<<" "<<(*it).second<<endl;
    }
}
int main()
{
    my_map<int,int> a;
    a.insert(make_pair(4,4));
    a.insert(make_pair(2,5));
    a.insert(make_pair(6,2));
    a.insert(make_pair(5,1));
    a.insert(make_pair(7,3));
    a.insert(make_pair(1,2));
    a.insert(make_pair(3,4));
    my_map<int,int> a1;
    a1.insert(make_pair(4,4));
    a1.insert(make_pair(2,5));
    a1.insert(make_pair(6,2));
    a1.insert(make_pair(5,1));
    a1.insert(make_pair(7,4));
    a1.insert(make_pair(1,2));
    a1.insert(make_pair(3,4));
    cout<<(a==a1) <<endl;
    a1[7]=3;
    cout<<(a==a1) <<endl;
    a1[7]=4;
    cout<<a.size()<<endl;
    print(a);
     cout<<endl;
    print(a1);
    a=a1;
    print(a);
     cout<<endl;
    print(a1);
    cout<<endl;
    print(a);
     cout<<endl;
    for(my_map<int,int>::iterator it=a.begin(); it!=a.end(); it++)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }
    cout<<endl;
    for(my_map<int,int>::iterator it=a.end(); it!=a.begin(); --it)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }
     cout<<endl;
    for(my_map<int,int>::iterator it=a.end(); it!=a.find(4); --it)
    {
        cout<<it->first<<" "<<it->second<<endl;
    }
     cout<<endl;
    print(a);
    cout<<endl;
    cout<<a.size()<<endl;
    return 0;
}
