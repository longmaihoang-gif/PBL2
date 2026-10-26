#pragma once

// =================================================================
// NODE: Nut danh sach lien ket doi dung Template
// =================================================================
template <typename T>
struct Node {
    T value;
    Node<T>* prev; // Con tro tro ve nut truoc (prev)
    Node<T>* next; // Con tro tro toi nut sau (next)

    Node(const T& val = T(), Node<T>* prevNode = nullptr, Node<T>* nextNode = nullptr)
        : value(val), prev(prevNode), next(nextNode) {}

    ~Node() {}

    // Chen target vao ngay sau nut hien tai (Insert After)
    void Insert(Node<T>* target) {
        if (target == nullptr || target == this) {
            return;
        }

        Node<T>* oldNext = this->next;

        this->next = target;
        target->prev = this;

        target->next = oldNext;
        if (oldNext != nullptr) {
            oldNext->prev = target;
        }
    }

    // Tu ngat lien ket nut hien tai khoi chuoi (Remove Self)
    void Remove() {
        if (this->prev != nullptr) {
            this->prev->next = this->next;
        }

        if (this->next != nullptr) {
            this->next->prev = this->prev;
        }

        this->prev = nullptr;
        this->next = nullptr;
    }
};
