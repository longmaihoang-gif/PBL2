#pragma once
#include <string>
#include <vector>
#include <any>

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

// Bảng băm tự cài đặt
class HashTable {
private:
    HashNode** buckets;         // Mảng con trỏ động chứa các danh sách liên kết
    int capacity;            // Sức chứa mảng bucket
    int sizeCount;           // Số lượng phần tử hiện tại
    float loadFactorThreshold;  // Ngưỡng quá tải để Rehash

    int HashFunction(const string& key) const;
    void Rehash();

public:
    // Khởi tạo & Hủy
    HashTable(int initCapacity = 101, float threshold = 0.75f);
    ~HashTable();

    // Thao tác dữ liệu cốt lõi (Chuẩn PascalCase)
    void Insert(const string& key, const any& value);
    bool Remove(const string& key);
    any* Search(const string& key);
    bool Contains(const string& key);
    void Clear();

    // Thuộc tính & Trạng thái
    int Size() const;
    bool IsEmpty() const;
    int GetCapacity() const;

    // Chiêu thức bổ trợ (Helper): Lấy dữ liệu đã ép kiểu nhanh gọn
    template <typename T>
    T* Get(const string& key);

    // Trích xuất toàn bộ giá trị có kiểu T trong bảng băm
    template <typename T>
    vector<T> GetAllValues() const;
};

// ============================================================
// Triển khai Template (Implementation) đặt ở đáy file Header
// ============================================================
template <typename T>
inline T* HashTable::Get(const string& key) {
    any* val = Search(key);
    if (val == nullptr) return nullptr;
    return any_cast<T>(val);
}

template <typename T>
inline vector<T> HashTable::GetAllValues() const {
    vector<T> result;
    for (int i = 0; i < capacity; i++) {
        HashNode* curr = buckets[i];
        while (curr != nullptr) {
            const T* valPtr = any_cast<T>(&curr->value);
            if (valPtr != nullptr) {
                result.push_back(*valPtr);
            }
            curr = curr->next;
        }
    }
    return result;
}
