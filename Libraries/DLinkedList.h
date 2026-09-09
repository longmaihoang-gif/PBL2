#pragma once
#include <any>

using namespace std;

// =================================================================
// NODE: Nút danh sách liên kết đôi
// =================================================================
struct Node {
    public:
        any value;
        Node *prev; // Con trỏ trỏ về nút trước (prev)
        Node *next; // Con trỏ trỏ tới nút sau (next)

        Node(any val = any(), Node *prevNode = nullptr, Node *nextNode = nullptr)
            : value(val), prev(prevNode), next(nextNode) {}

        ~Node() {}

        void Insert(Node *target);
        void Remove();

        // Ép kiểu nhanh giá trị (Trả về con trỏ, nullptr nếu sai kiểu)
        template <typename T>
        T* Get();
};

// ============================================================
// Triển khai Template đặt ở đáy file Header
// ============================================================
template <typename T>
inline T* Node::Get() {
    return any_cast<T>(&value);
}
