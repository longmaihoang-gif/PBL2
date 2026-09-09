#include "DLinkedList.h"

// ============================================================
// Insert - Chèn target vào ngay sau node hiện tại (Insert After)
// ============================================================

void Node::Insert(Node* target) {
    // Nếu target không hợp lệ hoặc chính là node hiện tại thì bỏ qua
    if (target == nullptr || target == this) {
        return;
    }

    // --------------------------------------------------------
    // 1. Lưu lại node phía sau hiện tại (nếu có)
    // --------------------------------------------------------
    Node* oldNext = this->next;

    // --------------------------------------------------------
    // 2. Nối node hiện tại với target
    // --------------------------------------------------------
    this->next = target;
    target->prev = this;

    // --------------------------------------------------------
    // 3. Nối target với node phía sau cũ
    // --------------------------------------------------------
    target->next = oldNext;
    if (oldNext != nullptr) {
        oldNext->prev = target;
    }
}


// ============================================================
// Remove - Tự ngắt liên kết node hiện tại khỏi chuỗi
// ============================================================

void Node::Remove() {
    // --------------------------------------------------------
    // 1. Nối node phía trước với node phía sau
    // --------------------------------------------------------
    if (this->prev != nullptr) {
        this->prev->next = this->next;
    }

    // --------------------------------------------------------
    // 2. Nối node phía sau với node phía trước
    // --------------------------------------------------------
    if (this->next != nullptr) {
        this->next->prev = this->prev;
    }

    // --------------------------------------------------------
    // 3. Xóa sạch con trỏ liên kết của node hiện tại
    // --------------------------------------------------------
    this->prev = nullptr;
    this->next = nullptr;
}
