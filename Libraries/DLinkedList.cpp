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
    Node* oldNext = this->n;

    // --------------------------------------------------------
    // 2. Nối node hiện tại với target
    // --------------------------------------------------------
    this->n = target;
    target->p = this;

    // --------------------------------------------------------
    // 3. Nối target với node phía sau cũ
    // --------------------------------------------------------
    target->n = oldNext;
    if (oldNext != nullptr) {
        oldNext->p = target;
    }
}


// ============================================================
// Remove - Tự ngắt liên kết node hiện tại khỏi chuỗi
// ============================================================

void Node::Remove() {
    // --------------------------------------------------------
    // 1. Nối node phía trước với node phía sau
    // --------------------------------------------------------
    if (this->p != nullptr) {
        this->p->n = this->n;
    }

    // --------------------------------------------------------
    // 2. Nối node phía sau với node phía trước
    // --------------------------------------------------------
    if (this->n != nullptr) {
        this->n->p = this->p;
    }

    // --------------------------------------------------------
    // 3. Xóa sạch con trỏ liên kết của node hiện tại
    // --------------------------------------------------------
    this->p = nullptr;
    this->n = nullptr;
}
