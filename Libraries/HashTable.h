#pragma once
#include <string>
#include <any>

using namespace std;

// =================================================================
// 🧱 HASH NODE: Nút danh sách liên kết cho Separate Chaining
// =================================================================
struct HashNode {
    string key;
    any value;     // Chứa mọi kiểu dữ liệu (tương đương "object")
    HashNode* next;

    HashNode(const string& k, const any& v);
};

// =================================================================
// ⚡ HASH TABLE: Bảng băm tự build 100% (Khung giao diện Header)
// =================================================================
class HashTable {
private:
    HashNode** buckets;  // Mảng con trỏ động chứa các danh sách liên kết
    size_t capacity;     // Sức chứa mảng bucket
    size_t count;        // Số lượng phần tử hiện có
    const float MAX_LOAD_FACTOR = 0.75f;

    // Các chiêu thức nội tại (Private Helpers)
    size_t hashFunction(const string& key) const;
    void rehash();
public:
    // Khởi tạo & Hủy
    HashTable(size_t initialCapacity = 101);
    ~HashTable();

    // Thao tác dữ liệu cốt lõi
    void insert(const string& key, const any& value);
    bool remove(const string& key);
    any* search(const string& key);
    const any* search(const string& key) const;
    bool contains(const string& key) const;
    void clear();

    // Thuộc tính & Trạng thái
    size_t size() const;
    bool isEmpty() const;
    size_t getCapacity() const;

    // 🌟 Chiêu thức bổ trợ (Helper): Lấy dữ liệu đã ép kiểu nhanh gọn
    template <typename T>
    T* get(const string& key) {
        any* val = search(key);
        if (val == nullptr) return nullptr;
        return any_cast<T>(val);
    }
};
