#ifndef AVLBIMAP
#define AVLBIMAP

#include "AVLTree.h"

template <class K, class V>
class PointerPair {
public:
    K *Key = nullptr;
    V *Value = nullptr;

    PointerPair(K *Key) : Key(Key) { }
    PointerPair(K *Key, V *Value) : Key(Key), Value(Value) { }

    bool operator==(PointerPair KVP) { return *Key == *KVP.Key; }
    bool operator<(PointerPair KVP) { return *Key < *KVP.Key; }
    bool operator>(PointerPair KVP) { return *Key > *KVP.Key; }

    friend std::ostream& operator<<(std::ostream& os, PointerPair KVP) {
        if(KVP.Value == nullptr) os<<"["<<*KVP.Key<<", ]";
        else os<<"["<<*KVP.Key<<","<<*KVP.Value<<"]";
        return os;
    }
};

template <class K, class V>
class AVLBiMap {
private:
    AVLTree<PointerPair<K,V> > m_KeyTree;
    AVLTree<PointerPair<V,K> > m_ValueTree;
public:
    ~AVLBiMap() {
        std::vector<K> keys = Keys();
        for(K k : keys) DeleteByKey(k);
    }

    bool Insert(K Key, V Value);

    void DeleteByKey(K Key);
    void DeleteByValue(V Value);

    bool ExistsByKey(K Key);
    bool ExistsByValue(V Value);

    V Value(K Key);
    K Key(V Value);

    std::vector<K> Keys();
    std::vector<V> Values();

    void PrintByKey() { m_KeyTree.Print(); }
    void PrintByValue() { m_ValueTree.Print(); }
};

template<class K, class V>
bool AVLBiMap<K, V>::Insert(K Key, V Value){
    K *KeyPointer = new K(Key);
    V *ValuePointer = new V(Value);
    if(m_KeyTree.Exists(PointerPair<K,V>(KeyPointer, ValuePointer))) return false;
    if(m_ValueTree.Exists(PointerPair<V,K>(ValuePointer, KeyPointer))) return false;
    m_KeyTree.Insert(PointerPair<K,V>(KeyPointer, ValuePointer));
    m_ValueTree.Insert(PointerPair<V,K>(ValuePointer, KeyPointer));
    return true;
}

template<class K, class V>
void AVLBiMap<K,V>::DeleteByKey(K Key){
    K *KeyPointer = new K(Key);
    V *ValuePointer = nullptr;

    PointerPair<K,V> PP = m_KeyTree.Value(PointerPair<K,V>(KeyPointer));
    delete KeyPointer;

    KeyPointer = PP.Key;
    ValuePointer = PP.Value;

    m_KeyTree.Delete(PointerPair<K,V>(KeyPointer));
    m_ValueTree.Delete(PointerPair<V,K>(ValuePointer));

    delete KeyPointer;
    delete ValuePointer;
}

template<class K, class V>
void AVLBiMap<K,V>::DeleteByValue(V Value){
    K *KeyPointer = nullptr;
    V *ValuePointer = new V(Value);

    PointerPair<V,K> PP = m_ValueTree.Value(PointerPair<V,K>(ValuePointer));
    delete ValuePointer;

    KeyPointer = PP.Value;
    ValuePointer = PP.Key;

    m_KeyTree.Delete(PointerPair<K,V>(KeyPointer));
    m_ValueTree.Delete(PointerPair<V,K>(ValuePointer));

    delete KeyPointer;
    delete ValuePointer;
}

template<class K, class V>
bool AVLBiMap<K,V>::ExistsByKey(K Key){
    K *KeyPointer = new K(Key);
    bool result = m_KeyTree.Exists(PointerPair<K,V>(KeyPointer));
    delete KeyPointer;
    return result;
}

template<class K, class V>
bool AVLBiMap<K,V>::ExistsByValue(V Value){
    V *ValuePointer = new V(Value);
    bool result = m_ValueTree.Exists(PointerPair<V,K>(ValuePointer));
    delete ValuePointer;
    return result;
}

template<class K, class V>
V AVLBiMap<K,V>::Value(K Key){
    K *KeyPointer = new K(Key);
    V Value = *m_KeyTree.Value(PointerPair<K,V>(KeyPointer)).Value;
    delete KeyPointer;
    return Value;
}

template<class K, class V>
K AVLBiMap<K,V>::Key(V Value){
    V *ValuePointer = new V(Value);
    K Key = *m_ValueTree.Value(PointerPair<V,K>(ValuePointer)).Value;
    delete ValuePointer;
    return Key;
}

template<class K, class V>
std::vector<K> AVLBiMap<K,V>::Keys(){
    auto temp = m_KeyTree.Values();
    std::vector<K> Keys;
    for(auto a : temp) Keys.push_back(*a.Key);
    return Keys;
}

template<class K, class V>
std::vector<V> AVLBiMap<K,V>::Values(){
    auto temp = m_ValueTree.Values();
    std::vector<V> Values;
    for(auto a : temp) Values.push_back(*a.Key);
    return Values;
}

#endif
