#ifndef AVLMAP
#define AVLMAP

#include "AVLTree.h"

template <class K, class V>
class KeyValuePair {
public:
    K Key;
    V Value;

    KeyValuePair(K Key) : Key(Key) { }
    KeyValuePair(K Key, V Value) : Key(Key), Value(Value) { }

    bool operator==(KeyValuePair KVP) { return Key == KVP.Key; }
    bool operator<(KeyValuePair KVP) { return Key < KVP.Key; }
    bool operator>(KeyValuePair KVP) { return Key > KVP.Key; }

    friend std::ostream& operator<<(std::ostream& os, KeyValuePair KVP) { os<<"["<<KVP.Key<<","<<KVP.Value<<"]"; return os; }
};

template <class K, class V>
class AVLMap {
private:
    AVLTree<KeyValuePair<K,V> > m_Tree;
public:
    void Insert(K Key, V Value) { m_Tree.Insert(KeyValuePair<K,V>(Key,Value)); }
    void Delete(K Key) { m_Tree.Delete(KeyValuePair<K,V>(Key)); }
    bool Exists(K Key) { return m_Tree.Exists(KeyValuePair<K,V>(Key)); }
    V Value(K Key) { return m_Tree.Value(KeyValuePair<K,V>(Key)).Value; }
    void Print() { m_Tree.Print(); }
};

#endif
