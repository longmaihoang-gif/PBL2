#pragma once
#include <any>

using namespace std;

// =================================================================
// 🧱 NODE: Nút danh sách liên kết đôi
// =================================================================
struct Node {
    public:
        any value;
        Node *p; // Con trỏ trỏ về nút trước (prev)
        Node *n; // Con trỏ trỏ tới nút sau (next)

        Node(any val = any(), Node *prev = nullptr, Node *next = nullptr)
            : value(val), p(prev), n(next) {}

        ~Node() {}

        void Insert(Node *target);
        void Remove();

        // 🌟 Chiêu thức bổ trợ: Ép kiểu nhanh giá trị (Trả về con trỏ, nullptr nếu sai kiểu)
        template <typename T>
        T* Get() {
            return any_cast<T>(&value);
        }
};