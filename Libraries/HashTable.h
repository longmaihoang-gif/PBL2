#pragma once
#include <string>
#include <any>
#include <cstddef>

using namespace std;

// Nút danh sách liên kết đôi cho Separate Chaining
struct HashNode {
    string key;
    any value;
    HashNode* next;
    HashNode* prev; // Con trỏ trỏ ngược về nút trước

    HashNode(const string& k, const any& v)
        : key(k), value(v), next(nullptr), prev(nullptr) {}
};

// Bảng băm tự cài đặt 100%
class HashTable {
private:
    HashNode** buckets;         // Mảng con trỏ động chứa các danh sách liên kết
    size_t capacity;            // Sức chứa mảng bucket
    size_t sizeCount;           // Số lượng phần tử hiện tại
    float loadFactorThreshold;  // Ngưỡng quá tải để Rehash

    size_t hashFunction(const string& key) const;
    void rehash();

public:
    // Khởi tạo & Hủy
    HashTable(size_t initCapacity = 101, float threshold = 0.75f);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
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
