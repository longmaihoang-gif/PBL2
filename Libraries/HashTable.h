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

    size_t HashFunction(const string& key) const;
    void Rehash();

public:
    // Khởi tạo & Hủy
    HashTable(size_t initCapacity = 101, float threshold = 0.75f);
    ~HashTable();

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Thao tác dữ liệu cốt lõi (Chuẩn PascalCase)
    void Insert(const string& key, const any& value);
    bool Remove(const string& key);
    any* Search(const string& key);
    const any* Search(const string& key) const;
    bool Contains(const string& key) const;
    void Clear();

    // Thuộc tính & Trạng thái
    size_t Size() const;
    bool IsEmpty() const;
    size_t GetCapacity() const;

    // 🌟 Chiêu thức bổ trợ (Helper): Lấy dữ liệu đã ép kiểu nhanh gọn
    template <typename T>
    T* Get(const string& key) {
        any* val = Search(key);
        if (val == nullptr) return nullptr;
        return any_cast<T>(val);
    }
};
