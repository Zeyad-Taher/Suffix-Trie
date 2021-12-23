#include <iostream>
using namespace std;

template <typename Object, typename Pointer>
class List {
public:
    struct Node
    {
        Object data;
        Node *prev;
        Node *next;
        Pointer* child;

        Node(const Object & d = Object{}, Pointer* c = nullptr, Node* p = nullptr, Node* n = nullptr)
                : data{d}, prev{p}, next{n}, child{c} {}

        Node(Object && d, Pointer* c = nullptr, Node* p = nullptr, Node* n = nullptr)
                : data{move(d)}, prev{p}, next{n}, child{c} {}
    };

    int theSize;
    Node *head;
    Node *tail;

    void init()
    {
        theSize = 0;
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;
    }
public:
    class iterator
    {
    protected:
        Node *current;

        Object & retrieve() const
        {
            return current->data;
        }
        Object * retrieveChild() const
        {
            return current->child;
        }
        iterator(Node *p) : current{p}
        {}
        friend class List<Object, Pointer>;
    public:
        iterator() : current{nullptr}
        {}
        const Object & operator* () const
        {
            return retrieve();
        }
        iterator & operator++ ()
        {
            current = current->next;
            return *this;
        }
        iterator & operator-- ()
        {
            current = current->prev;
            return *this;
        }
        bool operator== (const iterator & rhs) const
        {
            return current == rhs.current;
        }
        bool operator!= (const iterator & rhs) const
        {
            return current != rhs.current;
        }
        Node* getCurrent()
        {
            return current;
        }
    };



    List()
    {
        init();
    }
    List(Object obj, int num)
    {
        init();
        for (int i = 0; i < num; i++)
        {
            this->push_front(obj);
        }
    }
    ~List()
    {
        while(size() != 0)
            erase(begin());
    }

    List & operator= (List && rhs)
    {
        List copy = rhs;
        swap(*this, copy);
        return *this;
    }
    int size() const
    {
        return theSize;
    }
    Node * search(Object obj)
    {
        Node *node = this->head->next;
        for (int i = 0; i < theSize; i++)
        {

            if (node->data == obj)
                return node;
            node = node->next;
        }
        return NULL;
    }

    iterator begin()
    {
        return {head->next};
    }
    iterator begin() const
    {
        return {head->next};
    }

    iterator end()
    {
        return tail;
    }
    iterator end() const
    {
        return tail;
    }
    iterator insert(const Object & x, iterator itr, Pointer* c = nullptr)
    {
        Node *p = itr.current;
        theSize++;
        return{p->prev = p->prev->next = new Node{x, c, p->prev,p}};
    }
    iterator insert(Object && x, iterator itr, Pointer* c = nullptr)
    {
        Node *p = itr.current;
        theSize++;
        return {p->prev = p->prev->next = new Node{move(x), c, p->prev,p}};
    }
    iterator erase(iterator itr)
    {
        Node *p = itr.current;
        iterator retVal(p->next);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        theSize--;
        return retVal;
    }

    void push_front(const Object & x)
    {
        insert(x,begin());
    }
    void push_front(Object && x)
    {
        insert(move(x), begin());
    }
    Pointer* push_back(const Object & x)
    {
        Pointer* pointer = new Pointer();
        insert(x, end(), pointer);
        return pointer;
    }
    void push_back(Object && x)
    {
        insert(move(x), end());
    }
};

class TrieNode
{
    List<char, TrieNode> children;
public:
    void insert(char* suffix, int index)
    {
        if(suffix[index] == '\0')
        {
            return;
        }
        List<char, TrieNode>::Node* targetLLNode = children.search(suffix[index]);
        if(targetLLNode == NULL)
        {
            TrieNode* trieNode = children.push_back(suffix[index]);
            trieNode->insert(suffix, index+1);
        }
        else
        {
            targetLLNode->child->insert(suffix, index+1);
        }
    }
    List<char, TrieNode>* getChildren()
    {
        return &children;
    }
    /**
     * This method is only for testing (called from suffix trie
     * @param i
     */
    void print(int i)
    {
        for(char c : children)
        {
            cout<<c<<", ";
        }
        cout<<endl;
        List<char, TrieNode>::iterator it = children.begin();
        for (int j = 0; j < i; ++j) {
            ++it;
        }
        if(it.getCurrent()->child->children.size() == 0){
            return;
        }
        it.getCurrent()->child->print(0);
    }
};

class SuffixTrie
{
    TrieNode* root;
public:
    SuffixTrie(char* word)
    {
        root = new TrieNode();
        for (int i = 0; word[i] != '\0'; ++i)
        {
            root->insert(word, i);
        }
    }

    TrieNode* getRoot()
    {
        return root;
    }

    /**
     * This method is only used to test if it is constructed succesfully
     * @param i
     */
    void print(int i = 0)
    {
        root->print(i);
    }
};

int main() {
    SuffixTrie t("banana$");
    t.print(0);
//    cout<<(*rootChildren->begin().getCurrent()->child->getChildren()->begin());
//    for(char i : *rootChildren)
//    {
//        cout<<(i);
//    }
    return 0;
}
