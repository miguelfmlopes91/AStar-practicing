//
//  fibonacciheap.h
//  Test
//
//  Created by Miguel Lopes on 12/10/2019.
//  Copyright © 2019 Miguel Lopes. All rights reserved.
//

template <class T> class FibonacciHeap;

template <class T> struct node {
private:
    node<T>* prev;
    node<T>* next;
    node<T>* child;
    node<T>* parent;
    T value;
    int degree;
    bool marked;
public:
    friend class FibonacciHeap<T>;
    node<T>* getPrev() {return prev;}
    node<T>* getNext() {return next;}
    node<T>* getChild() {return child;}
    node<T>* getParent() {return parent;}
    T getValue() {return value;}
    bool isMarked() {return marked;}
    
    bool hasChildren() {return child;}
    bool hasParent() {return parent;}
};

template <class T> class FibonacciHeap {
protected:
    node<T>* heap;
public:
    
    FibonacciHeap() {
        heap=_empty();
    }
    virtual ~FibonacciHeap() {
        if(heap) {
            _deleteAll(heap);
        }
    }
    node<T>* insert(T value) {
        node<T>* ret=_singleton(value);
        heap=_merge(heap,ret);
        return ret;
    }
    void merge(FibonacciHeap& other) {
        heap=_merge(heap,other.heap);
        other.heap=_empty();
    }
    
    bool empty() {
        return heap==nullptr;
    }
    
    T getMinimum() {
        return heap->value;
    }
    
    T removeMinimum() {
        node<T>* old=heap;
        heap=_removeMinimum(heap);
        T ret=old->value;
        delete old;
        return ret;
    }
    
    void decreaseKey(node<T>* n,T value) {
        heap=_decreaseKey(heap,n,value);
    }
    
    node<T>* find(T value) {
        return _find(heap,value);
    }
private:
    node<T>* _empty() {
        return nullptr;
    }
    
    node<T>* _singleton(T value) {
        node<T>* n=new node<T>;
        n->value=value;
        n->prev=n->next=n;
        n->degree=0;
        n->marked=false;
        n->child=nullptr;
        n->parent=nullptr;
        return n;
    }
    
    node<T>* _merge(node<T>* a,node<T>* b) {
        if(a==nullptr)return b;
        if(b==nullptr)return a;
        if(a->value>b->value) {
            node<T>* temp=a;
            a=b;
            b=temp;
        }
        node<T>* an=a->next;
        node<T>* bp=b->prev;
        a->next=b;
        b->prev=a;
        an->prev=bp;
        bp->next=an;
        return a;
    }
    
    void _deleteAll(node<T>* n) {
        if(n!=nullptr) {
            node<T>* c=n;
            do {
                node<T>* d=c;
                c=c->next;
                _deleteAll(d->child);
                delete d;
            } while(c!=n);
        }
    }
    
    void _addChild(node<T>* parent,node<T>* child) {
        child->prev=child->next=child;
        child->parent=parent;
        parent->degree++;
        parent->child=_merge(parent->child,child);
    }
    
    void _unMarkAndUnParentAll(node<T>* n) {
        if(n==nullptr)return;
        node<T>* c=n;
        do {
            c->marked=false;
            c->parent=nullptr;
            c=c->next;
        }while(c!=n);
    }
    
    node<T>* _removeMinimum(node<T>* n) {
        _unMarkAndUnParentAll(n->child);
        if(n->next==n) {
            n=n->child;
        } else {
            n->next->prev=n->prev;
            n->prev->next=n->next;
            n=_merge(n->next,n->child);
        }
        if(n==nullptr)return n;
        node<T>* trees[64]={nullptr};
        
        while(true) {
            if(trees[n->degree]!=nullptr) {
                node<T>* t=trees[n->degree];
                if(t==n)break;
                trees[n->degree]=nullptr;
                if(n->value<t->value) {
                    t->prev->next=t->next;
                    t->next->prev=t->prev;
                    _addChild(n,t);
                } else {
                    t->prev->next=t->next;
                    t->next->prev=t->prev;
                    if(n->next==n) {
                        t->next=t->prev=t;
                        _addChild(t,n);
                        n=t;
                    } else {
                        n->prev->next=t;
                        n->next->prev=t;
                        t->next=n->next;
                        t->prev=n->prev;
                        _addChild(t,n);
                        n=t;
                    }
                }
                continue;
            } else {
                trees[n->degree]=n;
            }
            n=n->next;
        }
        node<T>* min=n;
        do {
            if(n->value<min->value)min=n;
            n=n->next;
        } while(n!=n);
        return min;
    }
    
    node<T>* _cut(node<T>* heap,node<T>* n) {
        if(n->next==n) {
            n->parent->child=nullptr;
        } else {
            n->next->prev=n->prev;
            n->prev->next=n->next;
            n->parent->child=n->next;
        }
        n->next=n->prev=n;
        n->marked=false;
        return _merge(heap,n);
    }
    
    node<T>* _decreaseKey(node<T>* heap,node<T>* n,T value) {
        if(n->value<value)return heap;
        n->value=value;
        if(n->value<n->parent->value) {
            heap=_cut(heap,n);
            node<T>* parent=n->parent;
            n->parent=nullptr;
            while(parent!=nullptr && parent->marked) {
                heap=_cut(heap,parent);
                n=parent;
                parent=n->parent;
                n->parent=nullptr;
            }
            if(parent!=nullptr && parent->parent!=nullptr)parent->marked=true;
        }
        return heap;
    }
    
    node<T>* _find(node<T>* heap,T value) {
        node<T>* n=heap;
        if(n==nullptr)return nullptr;
        do {
            if(n->value==value)return n;
            node<T>* ret=_find(n->child,value);
            if(ret)return ret;
            n=n->next;
        }while(n!=heap);
        return nullptr;
    }
};
