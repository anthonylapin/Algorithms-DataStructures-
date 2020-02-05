#ifndef H_Ring
#define H_Ring

/*
    This header file stores the implementation of 
    Doubly Linked Ring named Ring with its general methods
    and classes Iterator and ConstIterator

    Author: Anton Lapin.
*/

#include <iostream>
#include <string>
using namespace std;

template <typename Key, typename Info>
class Ring
{
protected:
    struct Node
    {
        Key key;
        Info info;
        Node *next;
        Node *prev;
    };
    Node *head;
    int size;

private:
    // unique_count and unique_id are private, because I decided making them necessary for usage only
    // inside unique() public method, but not outside the class.
    int unique_count(const Key &key, const Info &info) const;
    int unique_id(const Key &key, const Info &info) const;

    // checks whether the node is in Ring or not
    bool find(Node *adr)
    {
        assert(!empty());
        typename Ring<Key, Info>::Iterator itr = this->begin();
        for (int i = 0; i < size; i++)
        {
            if (itr.adress() == adr)
            {
                return true;
            }
            itr++;
        }

        return false;
    }
    // copyin' data of other ring to this ring.
    void copy(const Ring &other);

public:
    class Iterator;      // +
    class ConstIterator; // +

    template <typename K, typename I>
    friend ostream &operator<<(ostream &os, const Ring<K, I> &rng); // +

    // default constructor +
    Ring();

    // copy constructor
    Ring(const Ring &other);

    // assignment operator
    const Ring &operator=(const Ring &otherRing);

    // printin' ring, if reversed = 1 -> in reversed direction +
    void print(bool reversed = false) const;

    // checks whether ring is empty or not
    bool empty() const;

    // returns length of ring
    int len() const;

    // calculates total amount of elements with given key in ring
    int count(const Key &key) const;

    // returns whether element with given key and info exists or not
    bool search(const Key &key) const;

    // returns index of first occur in ring of element with given key
    int index(const Key &key) const;

    // reversing the list
    void reverse();

    // adds new elem before the head +
    void append(const Key &key, const Info &info);

    // adds new elem after head +
    void push(const Key &key, const Info &info);

    // adds elem to the specified position, if position is zero, new elem becomes a head
    void add(const int position, const Key &key, const Info &info);

    // adds content of another ring to the end of THIS ring
    void extend(const Ring &other);

    // if index not mentioned -> removes the head element
    void pop(const int index = 0);

    // deletes element before the head
    void pop_back();

    // deletes all elements with given key from the ring
    void remove(const Key &key);

    // makes an element unique in ring
    void unique(const Key &key, const Info &info);

    // clears the list
    void clear();

    // destructor
    ~Ring();

public: // Iterator Methods and Iterator() class implementation
    Iterator begin() const
    {
        return Iterator(head);
    }

    Iterator end() const
    {
        if (head == nullptr)
        {
            return head;
        }
        return Iterator(head->prev);
    }

    Iterator operator[](const int index)
    {
        typename Ring<Key, Info>::Iterator itr = this->begin();
        if (index > 0)
        {
            for (int i = 0; i < index; i++)
            {
                itr++;
            }
        }

        if (index < 0)
        {
            for (int i = 0; i > index; i--)
            {
                itr--;
            }
        }

        return itr;
    }

    Iterator search_iterator(const Key &key, bool &found)
    {

        for (int i = 0; i < size; i++)
        {
            if ((this->begin() + i).getKey() == key)
            {
                found = true;
                return (this->begin() + i);
            }
        }

        found = false;
        return Iterator(nullptr);
    }

    Iterator remove_element(typename Ring<Key, Info>::Iterator to_erase)
    {
        assert(find(to_erase.adress()));

        if (size == 1)
        {
            delete head;
            head = nullptr;
            size = 0;
            return this->begin();
        }
        else
        {
            typename Ring<Key, Info>::Iterator previous(to_erase.adressPrev());
            typename Ring<Key, Info>::Iterator next(to_erase.adressNext());

            if (to_erase.adress() == head)
            {
                make_head(this->begin() - 1);
            }

            next.set_prev(previous.adress());
            previous.set_next(next.adress());
            Node *erase = to_erase.adress();
            delete erase;
            size--;
            return previous;
        }
    }

    Iterator insert(typename Ring<Key, Info>::Iterator position, const Key &key, const Info &info)
    {
        // returns iterator pointin' to the newly created val
        // position is the element before which the new element to be created
        typename Ring<Key, Info>::Iterator newElement;
        newElement.set_node(key, info);
        if (empty())
        {
            head = newElement.adress();
            newElement.set_next(head);
            newElement.set_prev(head);
        }
        else
        {
            typename Ring<Key, Info>::Iterator after(position.adressPrev());
            after.set_next(newElement.adress());
            newElement.set_prev(after.adress());
            newElement.set_next(position.adress());
            position.set_prev(newElement.adress());
        }

        size++;
        return newElement;
    }

    Iterator make_head(typename Ring<Key, Info>::Iterator itr)
    {
        assert(!empty());
        if (itr.current != head)
        {
            head = itr.current;
        }
        return itr;
    }

    class Iterator
    {
    private:
        Node *current;

        Iterator(Node *ptr)
        {
            current = ptr;
        }

        Iterator set_node(const Key &key, const Info &info) // function to create node.
        {
            if (current == nullptr)
            {
                current = new Node;
            }
            current->key = key;
            current->info = info;
            return *this;
        }

        void set_next(Node *nextNode)
        {
            current->next = nextNode;
        }

        void set_prev(Node *prevNode)
        {
            current->prev = prevNode;
        }

        Node *adress()
        {
            return current;
        }

        Node *adressNext()
        {
            return current->next;
        }

        Node *adressPrev()
        {
            return current->prev;
        }

    public:
        friend class Ring;
        friend class ConstIterator;
        Iterator()
        {
            current = nullptr;
        }

        Iterator operator++() // ++itr
        {
            if (current)
            {
                current = current->next;
            }
            return *this;
        }

        Iterator operator++(int) // itr++
        {
            Iterator itr = current;
            ++(*this);
            return itr;
        }

        Iterator operator+(const int index) // itr + index
        {
            if (index > 0)
            {
                for (int i = 0; i < index; i++)
                {
                    (*this)++;
                }
            }

            if (index < 0)
            {
                for (int i = 0; i > index; i--)
                {
                    (*this)--;
                }
            }

            return *this;
        }

        Iterator operator-(const int index)
        {
            if (index > 0)
            {
                return (*this) + (-index);
            }
            else if (index < 0)
            {
                return (*this) + index;
            }
            return *this;
        }

        Iterator operator--() // --itr
        {
            if (current)
            {
                current = current->prev;
            }
            return *this;
        }

        Iterator operator--(int) // itr--
        {
            Iterator itr = current;
            --(*this);
            return itr;
        }

        bool operator==(const Iterator &other)
        {
            return (current == other.current);
        }

        bool operator!=(const Iterator &other)
        {
            return (current != other.current);
        }

        Key getKey() const
        {
            assert(current != nullptr);
            return current->key;
        }

        Info getInfo() const
        {
            assert(current != nullptr);
            return current->info;
        }
    };

public: // Const Iterator Methods
    ConstIterator cbegin() const
    {
        return ConstIterator(head);
    }

    ConstIterator cend() const
    {
        if (head == nullptr)
        {
            return head;
        }
        return Iterator(head->prev);
    }

    class ConstIterator
    {
    private:
        Node *current;

        ConstIterator(Node *ptr)
        {
            current = ptr;
        }

    public:
        friend class Ring;
        ConstIterator()
        {
            current = nullptr;
        }

        void data() const
        {
            if (current)
            {
                cout << current->key << " <=> " << current->info << endl;
            }
        }

        ConstIterator operator++() // ++itr
        {
            if (current)
            {
                current = current->next;
            }
            return *this;
        }

        ConstIterator operator++(int) // itr++
        {
            ConstIterator itr = current;
            ++(*this);
            return itr;
        }

        ConstIterator operator+(const int index) // itr + index
        {
            if (index > 0)
            {
                for (int i = 0; i < index; i++)
                {
                    (*this)++;
                }
            }

            if (index < 0)
            {
                for (int i = 0; i > index; i--)
                {
                    (*this)--;
                }
            }

            return *this;
        }

        ConstIterator operator-(const int index)
        {
            if (index > 0)
            {
                return (*this) + (-index);
            }
            else if (index < 0)
            {
                return (*this) + index;
            }
            return *this;
        }

        ConstIterator operator--() // --itr
        {
            if (current)
            {
                current = current->prev;
            }
            return *this;
        }

        ConstIterator operator--(int) // itr--
        {
            ConstIterator itr = current;
            --(*this);
            return itr;
        }

        bool operator==(const ConstIterator &other)
        {
            return (current == other.current);
        }

        bool operator!=(const ConstIterator &other)
        {
            return (current != other.current);
        }

        Key getKey() const
        {
            assert(current != nullptr);
            return current->key;
        }

        Info getInfo() const
        {
            assert(current != nullptr);
            return current->info;
        }
    };
};

template <typename Key, typename Info>
Ring<Key, Info>::Ring()
{
    head = nullptr;
    size = 0;
}

template <typename Key, typename Info>
Ring<Key, Info>::Ring(const Ring &other)
{
    head = nullptr;
    copy(other);
}

template <typename Key, typename Info>
const Ring<Key, Info> &Ring<Key, Info>::operator=(const Ring<Key, Info> &otherRing)
{
    if (this != &otherRing)
    {
        copy(otherRing);
    }

    return *this;
}

template <typename Key, typename Info>
void Ring<Key, Info>::copy(const Ring &other)
{
    if (this->begin().adress() != nullptr)
    {
        clear();
    }

    if (other.begin().adress() == nullptr)
    {
        head = nullptr;
        size = 0;
    }
    else
    {
        for (int i = 0; i < other.size; i++)
        {
            this->append((other.begin() + i).getKey(), (other.begin() + i).getInfo());
        }
        size = other.size;
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::print(bool reversed) const
{
    if (empty())
    {
        return;
    }

    if (reversed == false)
    {
        cout << "Ring : " << endl;
        for (int i = 0; i < size; i++)
        {
            (this->cbegin() + i).data();
        }
    }
    else
    {
        cout << "Ring : " << endl;
        for (int i = 0; i < size; i++)
        {
            (this->cbegin() - i).data();
        }
    }
}

template <typename Key, typename Info>
bool Ring<Key, Info>::empty() const
{
    return (head == nullptr);
}

template <typename Key, typename Info>
int Ring<Key, Info>::len() const
{
    return size;
}

template <typename Key, typename Info>
int Ring<Key, Info>::count(const Key &key) const
{
    if (empty())
    {
        return 0;
    }

    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        if ((this->cbegin() + i).getKey() == key)
        {
            counter++;
        }
    }

    return counter;
}

template <typename Key, typename Info>
int Ring<Key, Info>::unique_count(const Key &key, const Info &info) const
{
    if (empty())
    {
        return 0;
    }
    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        if ((this->begin() + i).getKey() == key && (this->begin() + i).getInfo() == info)
        {
            counter++;
        }
    }
    return counter;
}

template <typename Key, typename Info>
int Ring<Key, Info>::index(const Key &key) const
{
    assert(search(key));
    int i = 0;
    for (; i < size; i++)
    {
        if ((this->begin() + i).getKey() == key)
        {
            break;
        }
    }

    return i;
}

template <typename Key, typename Info>
int Ring<Key, Info>::unique_id(const Key &key, const Info &info) const
{
    assert(search(key));
    int i = 0;
    for (; i < size; i++)
    {
        if ((this->begin() + i).getKey() == key && (this->begin() + i).getInfo() == info)
        {
            break;
        }
    }
    return i;
}

template <typename Key, typename Info>
bool Ring<Key, Info>::search(const Key &key) const
{
    if (empty())
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if ((this->begin() + i).getKey() == key)
        {
            return true;
        }
    }
    return false;
}

template <typename Key, typename Info>
void Ring<Key, Info>::reverse()
{
    if (!empty())
    {
        typename Ring<Key, Info>::Iterator itr = this->begin();

        for (int i = 0; i < size; i++)
        {
            typename Ring<Key, Info>::Iterator temp(itr.adressPrev());
            itr.set_prev(itr.adressNext());
            itr.set_next(temp.adress());
            itr--;
        }
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::append(const Key &key, const Info &info)
{
    this->insert(this->begin(), key, info);
}

template <typename Key, typename Info>
void Ring<Key, Info>::push(const Key &key, const Info &info)
{
    this->insert(++this->begin(), key, info);
}

template <typename Key, typename Info>
void Ring<Key, Info>::add(const int position, const Key &key, const Info &info)
{
    if (position == 0) // then -> make head.
    {
        make_head(this->insert((this->begin() + position), key, info));
    }
    else
    {
        this->insert((this->begin() + position), key, info);
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::extend(const Ring &other)
{
    for (int i = 0; i < other.len(); i++)
    {
        append((other.begin() + i).getKey(), (other.begin() + i).getInfo());
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::pop(const int index)
{
    if (empty())
    {
        return;
    }

    if (size == 1)
    {
        delete head;
        head = nullptr;
        size--;
    }
    else
    {
        typename Ring<Key, Info>::Iterator to_erase = (this->begin() + index);
        if (to_erase == this->begin())
        {
            make_head(this->begin() + 1);
        }
        to_erase.adressNext()->prev = to_erase.adressPrev();
        to_erase.adressPrev()->next = to_erase.adressNext();
        Node *erase = nullptr;
        erase = to_erase.adress();
        delete erase;
        size--;
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::pop_back()
{
    pop(-1);
}

template <typename Key, typename Info>
void Ring<Key, Info>::remove(const Key &key)
{
    int counter = count(key);
    while (counter != 0)
    {
        pop(index(key));
        counter--;
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::unique(const Key &key, const Info &info)
{
    int counter = unique_count(key, info);

    while (counter != 0)
    {
        pop(unique_id(key, info));
        counter--;
    }
}

template <typename Key, typename Info>
void Ring<Key, Info>::clear()
{
    if (empty())
    {
        return;
    }

    typename Ring<Key, Info>::Iterator curr = this->begin();
    typename Ring<Key, Info>::Iterator last = this->end();

    while (curr != last)
    {
        typename Ring<Key, Info>::Iterator erase(curr.adress());
        curr++;
        make_head(curr);
        delete erase.adress();
    }
    delete curr.adress();

    head = nullptr;
    size = 0;
}

template <typename Key, typename Info>
Ring<Key, Info>::~Ring()
{
    clear();
}

template <typename Key, typename Info>
ostream &operator<<(ostream &os, const Ring<Key, Info> &rng)
{
    assert(!rng.empty());

    os << "Ring:" << endl;

    for (int i = 0; i < rng.len(); i++)
    {
        os << "Key: " << (rng.cbegin() + i).getKey() << " <=> "
           << "Info: " << (rng.cbegin() + i).getInfo() << endl;
    }
    os << "Length: " << rng.len() << endl;

    return os;
}

template <typename Key, typename Info>
void split(const Ring<Key, Info> &source, bool direction,
           Ring<Key, Info> &result1, int sequence1, int rep1,
           Ring<Key, Info> &result2, int sequence2, int rep2)
{
    assert(!source.empty());
    if (!result1.empty())
    {
        result1.clear();
    }
    if (!result2.empty())
    {
        result2.clear();
    }
    typename Ring<Key, Info>::ConstIterator curr = source.cbegin();

    bool stop = false;

    if (rep1 == 0 && rep2 == 0)
    {
        stop = true;
    }

    while (stop != true)
    {
        if (rep1 != 0)
        {
            for (int i = 0; i < sequence1; i++)
            {
                result1.append(curr.getKey(), curr.getInfo());
                if (direction == 1)
                {
                    curr++;
                }
                else
                {
                    curr--;
                }
            }
            rep1--;
        }

        if (rep2 != 0)
        {
            for (int i = 0; i < sequence2; i++)
            {
                result2.append(curr.getKey(), curr.getInfo());
                if (direction == 1)
                {
                    curr++;
                }
                else
                {
                    curr--;
                }
            }
            rep2--;
        }

        if (rep1 == 0 && rep2 == 0)
        {
            stop = true;
        }
    }
}

#endif