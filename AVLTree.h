#ifndef AVLTREE
#define AVLTREE

#include <fstream>
#include <iostream>
#include <vector>

template <class T>
struct AVLNode {
    T Value;
    int Ballance = 0;
    AVLNode *Left = nullptr;
    AVLNode *Right = nullptr;

    AVLNode() { }
    AVLNode(T Value) : Value(Value) { }
    ~AVLNode() { delete Left; delete Right; }

    // Zabezpieczenie przed kopiowaniem
    AVLNode(const AVLNode<T> &Node) = delete;
    AVLNode<T>& operator=(AVLNode<T> Node) = delete;
};

template <class T>
class AVLTree {
private:
    AVLNode<T> *m_Root = nullptr;

    // Operacje na drzewie
    void RecursiveInsert(AVLNode<T> *&Node, T Value);
    void RecursiveDelete(AVLNode<T> *&Node, T Value, AVLNode<T> *Parent = nullptr);
    bool RecursiveExists(AVLNode<T>  *Node, T Value) const;
    T FindPred(AVLNode<T> *Node) const;
    AVLNode<T>* FindNode(AVLNode<T> *Node, T Value) const;
    void Preorder(AVLNode<T> *Node, std::vector<T> &Values) const;

    // Rotacje pojedyncze
    void RR(AVLNode<T> *Node);
    void LL(AVLNode<T> *Node);

    // Rotacje podwójne
    void RL(AVLNode<T> *Node);
    void LR(AVLNode<T> *Node);

    bool Ballance(AVLNode<T> *Node);

    // Rysuje drzewo i wyświetla na ekranie
    void Print(AVLNode<T> *Node);
    std::ofstream out;

public:
    AVLTree() { out = std::ofstream("drzewo.dot"); }
    AVLTree(const AVLTree &Tree){
        std::vector<T> Values = Tree.Values();
        for(T V : Values) Insert(V);
    }

    AVLTree(AVLTree &&Tree) : m_Root(Tree.m_Root) { Tree.m_Root = nullptr; }

    AVLTree& operator=(AVLTree Tree) { std::swap(m_Root, Tree.m_Root); return *this; }

    ~AVLTree() { delete m_Root;}

    std::vector<T> Values() const;
    void Insert(T Value) { std::cout<<"Dodaję "<<Value<<std::endl; RecursiveInsert(m_Root, Value); }
    void Delete(T Value) { std::cout<<"Usuwam "<<Value<<std::endl; if(Exists(Value)) RecursiveDelete(m_Root, Value); }
    bool Exists(T Value) const { return RecursiveExists(m_Root, Value); }
    T Value(T Value) { return FindNode(m_Root, Value)->Value; }
    void Print();
};


template <class T>
void AVLTree<T>::RecursiveInsert(AVLNode<T> *&Node, T Value){
    static bool d = false;
    if(Node == nullptr){
        Node = new AVLNode<T>(Value);
        d = false;
    }
    else {
        if(Value < Node->Value){
            RecursiveInsert(Node->Left, Value);
            if(!d) Node->Ballance++;
        }
        else if(Value > Node->Value){
            RecursiveInsert(Node->Right, Value);
            if(!d) Node->Ballance--;
        }

        if(Node->Ballance == 0) d = true;
        if(Ballance(Node)) d = true;
    }
}

template <class T>
void AVLTree<T>::RecursiveDelete(AVLNode<T> *&Node, T Value, AVLNode<T> *Parent){
    static bool d = false;
    if(Value < Node->Value){
        RecursiveDelete(Node->Left, Value, Node);
        if(!d) Node->Ballance--;
        if(Node->Ballance == -1) d = true;
        if(Ballance(Node)) d = true;
    }
    else if(Value > Node->Value){
        RecursiveDelete(Node->Right, Value, Node);
        if(!d) Node->Ballance++;
        if(Node->Ballance == 1) d = true;
        if(Ballance(Node)) d = true;
    }
    else {
        d = false;
        AVLNode<T> *NodeToDelete = Node;
        AVLNode<T> *&LeftChild = NodeToDelete == m_Root ? m_Root : Parent->Left;
        AVLNode<T> *&RightChild = NodeToDelete == m_Root ? m_Root : Parent->Right;

        if(NodeToDelete->Left == nullptr && NodeToDelete->Right == nullptr){
            LeftChild == NodeToDelete ? LeftChild = nullptr : RightChild = nullptr;
            delete NodeToDelete;
        }
        else if(NodeToDelete->Left == nullptr){
            if(LeftChild == NodeToDelete) LeftChild = NodeToDelete->Right;
            else RightChild = NodeToDelete->Right;

            NodeToDelete->Right = nullptr;
            delete NodeToDelete;
        }
        else if(NodeToDelete->Right == nullptr){
            if(LeftChild == NodeToDelete) LeftChild = NodeToDelete->Left;
            else RightChild = NodeToDelete->Left;

            NodeToDelete->Left = nullptr;
            delete NodeToDelete;
        }
        else {
            T TempValue = FindPred(NodeToDelete);
            RecursiveDelete(m_Root, TempValue);
            std::swap(FindNode(m_Root, Value)->Value, TempValue);
        }
        NodeToDelete = nullptr;
    }
}

template <class T>
bool AVLTree<T>::RecursiveExists(AVLNode<T> *Node, T Value) const {
    if(Node == nullptr) return false;
    if(Node->Value == Value) return true;
    if(Value < Node->Value) return RecursiveExists(Node->Left, Value);
    else return RecursiveExists(Node->Right, Value);
}

template <class T>
T AVLTree<T>::FindPred(AVLNode<T> *Node) const {
    Node = Node->Left;
    while(Node->Right != nullptr){
        Node = Node->Right;
    }
    return Node->Value;
}

template <class T>
AVLNode<T>* AVLTree<T>::FindNode(AVLNode<T> *Node, T Value) const {
    if(Node->Value == Value) return Node;
    if(Value < Node->Value) return FindNode(Node->Left, Value);
    else return FindNode(Node->Right, Value);
}

template <class T>
void AVLTree<T>::Preorder(AVLNode<T> *Node, std::vector<T> &Values) const {
    if(Node == nullptr) return;
    Values.push_back(Node->Value);
    Preorder(Node->Left, Values);
    Preorder(Node->Right, Values);
}

template <class T>
void AVLTree<T>::RR(AVLNode<T> *Node){
    std::swap(Node->Value, Node->Right->Value);
    std::swap(Node->Left, Node->Right);
    std::swap(Node->Right, Node->Left->Right);
    std::swap(Node->Left->Left, Node->Left->Right);

    // Przeliczanie wag
    switch (Node->Left->Ballance) {
    case -1:
        Node->Ballance = 0;
        Node->Left->Ballance = 0;
        break;

    case 0:
        Node->Ballance = 1;
        Node->Left->Ballance = -1;
        break;
    }
}

template <class T>
void AVLTree<T>::LL(AVLNode<T> *Node){
    std::swap(Node->Value, Node->Left->Value);
    std::swap(Node->Left, Node->Right);
    std::swap(Node->Left, Node->Right->Left);
    std::swap(Node->Right->Left, Node->Right->Right);

    // Przeliczanie wag
    switch (Node->Right->Ballance) {
    case 0:
        Node->Ballance = -1;
        Node->Right->Ballance = 1;
        break;

    case 1:
        Node->Ballance = 0;
        Node->Right->Ballance = 0;
        break;
    }
}

template <class T>
void AVLTree<T>::RL(AVLNode<T> *Node){
    std::swap(Node->Value, Node->Right->Left->Value);
    std::swap(Node->Left, Node->Right->Left);
    std::swap(Node->Right->Left, Node->Left->Right);
    std::swap(Node->Left->Right, Node->Left->Left);

    // Przeliczanie wag
    switch (Node->Left->Ballance) {
    case -1:
        Node->Ballance = 0;
        Node->Left->Ballance = 1;
        Node->Right->Ballance = 0;
        break;

    case 0:
        Node->Ballance = 0;
        Node->Left->Ballance = 0;
        Node->Right->Ballance = 0;
        break;

    case 1:
        Node->Ballance = 0;
        Node->Left->Ballance = 0;
        Node->Right->Ballance = -1;
        break;
    }
}

template <class T>
void AVLTree<T>::LR(AVLNode<T> *Node){
    std::swap(Node->Value, Node->Left->Right->Value);
    std::swap(Node->Right, Node->Left->Right);
    std::swap(Node->Right->Left, Node->Left->Right);
    std::swap(Node->Right->Left, Node->Right->Right);

    // Przeliczanie wag
    switch (Node->Right->Ballance) {
    case -1:
        Node->Ballance = 0;
        Node->Right->Ballance = 0;
        Node->Left->Ballance = 1;
        break;

    case 0:
        Node->Ballance = 0;
        Node->Right->Ballance = 0;
        Node->Left->Ballance = 0;
        break;

    case 1:
        Node->Ballance = 0;
        Node->Right->Ballance = -1;
        Node->Left->Ballance = 0;
        break;
    }
}

template <class T>
bool AVLTree<T>::Ballance(AVLNode<T> *Node){
    if(Node->Ballance == -2){
        if(Node->Right->Ballance != 1){
            std::cout<<"Rotacja prawa, wierzchołek "<<Node->Value<<"-"<<Node->Right->Value<<std::endl;
            RR(Node);
        }
        else {
            std::cout<<"Rotacja prawo-lewa, wierzchołki "<<Node->Value<<"-"<<Node->Right->Value<<"-"<<Node->Right->Left->Value<<std::endl;
            RL(Node);
        }
        return true;
    }
    if(Node->Ballance == 2){
        if(Node->Left->Ballance != -1){
            std::cout<<"Rotacja lewa, wierzchołek "<<Node->Value<<"-"<<Node->Left->Value<<std::endl;
            LL(Node);
        }
        else {
            std::cout<<"Rotacja lewo-prawa, wierzchołki "<<Node->Value<<"-"<<Node->Left->Value<<"-"<<Node->Left->Right->Value<<std::endl;
            LR(Node);
        }
        return true;
    }
    return false;
}

template <class T>
void AVLTree<T>::Print(){
    out = std::ofstream("drzewo.dot");
    out<<"digraph BinaryTree \{"<<std::endl;
    if (m_Root != nullptr && m_Root->Left == nullptr && m_Root->Right == nullptr)
        out<<"    \""<<m_Root->Value<<"\";"<<std::endl;
    else
        Print(m_Root);
    out<<"}"<<std::endl;
    out.flush();
    out.close();
    system("dot -T png -O drzewo.dot");
    system("eog drzewo.dot.png");
}

template <class T>
void AVLTree<T>::Print(AVLNode<T> *Node){
    static int nullcount = 0;
    if(Node == nullptr) return;
    out<<"    \""<<Node->Value<<"\" [label=\""<<Node->Value<<" "<<Node->Ballance<<"\"];"<<std::endl;
    if(Node->Left != nullptr){
        out<<"    \""<<Node->Value<<"\" -> \""<<Node->Left->Value<<"\";"<<std::endl;
        Print(Node->Left);
    }
    else {
        nullcount++;
        out<<"    null"<<nullcount<<" [shape=point];"<<std::endl;
        out<<"    \""<<Node->Value<<"\" -> null"<<nullcount<<";"<<std::endl;
    }

    if(Node->Right != nullptr){
        out<<"    \""<<Node->Value<<"\" -> \""<<Node->Right->Value<<"\";"<<std::endl;
        Print(Node->Right);
    }
    else {
        nullcount++;
        out<<"    null"<<nullcount<<" [shape=point];"<<std::endl;
        out<<"    \""<<Node->Value<<"\" -> null"<<nullcount<<";"<<std::endl;
    }
}

template <class T>
std::vector<T> AVLTree<T>::Values() const {
    std::vector<T> Values;
    Preorder(m_Root, Values);
    return Values;
}

#endif
