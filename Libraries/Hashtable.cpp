#include "HashTable.h"
#include <stdexcept>

using namespace std;

// ============================================================
// Constructor
// ============================================================

HashTable::HashTable(int initCapacity, float threshold)
    : capacity(initCapacity),
      sizeCount(0),
      loadFactorThreshold(threshold)
{
    // Đảm bảo capacity hợp lệ
    if (capacity == 0) {
        capacity = 1;
    }

    // Load factor phải nằm trong khoảng (0, 1)
    if (loadFactorThreshold <= 0.0f ||
        loadFactorThreshold >= 1.0f) {
        throw invalid_argument(
            "Load factor threshold must be between 0 and 1."
        );
    }

    // Tạo mảng bucket
    buckets = new HashNode*[capacity];

    // Ban đầu tất cả bucket đều rỗng
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
}


// ============================================================
// Destructor
// ============================================================

HashTable::~HashTable() {
    Clear();

    delete[] buckets;
}


// ============================================================
// Hash Function - DJB2
// ============================================================

int HashTable::HashFunction(const string& key) const {

    unsigned long hash = 5381;

    for (int i = 0; i < key.length(); i++) {
        unsigned char c = key[i];
        hash = hash * 33 + c;
    }

    return hash % capacity;
}


// ============================================================
// Insert
// ============================================================

void HashTable::Insert(const string& key, const any& value) {

    // --------------------------------------------------------
    // 1. Kiểm tra key đã tồn tại chưa
    // --------------------------------------------------------

    int index = HashFunction(key);

    HashNode* current = buckets[index];

    while (current != nullptr) {

        if (current->key == key) {

            // Key đã tồn tại → update value
            current->value = value;

            return;
        }

        current = current->next;
    }


    // --------------------------------------------------------
    // 2. Kiểm tra Load Factor
    // --------------------------------------------------------

    float newLoadFactor = (sizeCount + 1) * 1.0f / capacity;

    if (newLoadFactor > loadFactorThreshold) {

        Rehash();

        // Sau khi rehash capacity đã thay đổi
        // nên phải tính lại index
        index = HashFunction(key);
    }


    // --------------------------------------------------------
    // 3. Tạo node mới
    // --------------------------------------------------------

    HashNode* newNode =
        new HashNode(key, value);


    // --------------------------------------------------------
    // 4. Chèn node vào đầu linked list
    // --------------------------------------------------------

    newNode->next = buckets[index];
    newNode->prev = nullptr;

    if (buckets[index] != nullptr) {
        buckets[index]->prev = newNode;
    }

    buckets[index] = newNode;


    // --------------------------------------------------------
    // 5. Tăng số lượng phần tử
    // --------------------------------------------------------

    sizeCount++;
}


// ============================================================
// Search
// ============================================================

any* HashTable::Search(const string& key) {

    int index = HashFunction(key);

    HashNode* current = buckets[index];

    while (current != nullptr) {

        if (current->key == key) {
            return &current->value;
        }

        current = current->next;
    }

    return nullptr;
}


// ============================================================
// Remove
// ============================================================

bool HashTable::Remove(const string& key) {

    int index = HashFunction(key);

    HashNode* current = buckets[index];

    while (current != nullptr) {

        if (current->key == key) {

            // ------------------------------------------------
            // Trường hợp node có node phía trước
            // ------------------------------------------------

            if (current->prev != nullptr) {
                current->prev->next = current->next;
            }
            else {
                // current là node đầu tiên
                buckets[index] = current->next;
            }


            // ------------------------------------------------
            // Trường hợp node có node phía sau
            // ------------------------------------------------

            if (current->next != nullptr) {
                current->next->prev = current->prev;
            }


            // ------------------------------------------------
            // Xóa node
            // ------------------------------------------------

            delete current;

            sizeCount--;

            return true;
        }

        current = current->next;
    }

    return false;
}


// ============================================================
// Contains
// ============================================================

bool HashTable::Contains(const string& key) {

    return Search(key) != nullptr;
}


// ============================================================
// Rehash
// ============================================================

void HashTable::Rehash() {

    // --------------------------------------------------------
    // 1. Tính capacity mới
    // --------------------------------------------------------

    int oldCapacity = capacity;

    int newCapacity =
        capacity * 2 + 1;


    // --------------------------------------------------------
    // 2. Tạo bucket mới
    // --------------------------------------------------------

    HashNode** newBuckets =
        new HashNode*[newCapacity];

    for (int i = 0; i < newCapacity; i++) {
        newBuckets[i] = nullptr;
    }

    // Cập nhật capacity mới trước để tái sử dụng hàm HashFunction
    capacity = newCapacity;


    // --------------------------------------------------------
    // 3. Chuyển tất cả node sang bucket mới
    // --------------------------------------------------------

    for (int i = 0; i < oldCapacity; i++) {

        HashNode* current = buckets[i];

        while (current != nullptr) {

            // Lưu node tiếp theo
            HashNode* nextNode = current->next;


            // ------------------------------------------------
            // Tính vị trí mới bằng HashFunction
            // ------------------------------------------------

            int newIndex = HashFunction(current->key);


            // ------------------------------------------------
            // Đưa node vào đầu bucket mới
            // ------------------------------------------------

            current->prev = nullptr;
            current->next = newBuckets[newIndex];

            if (newBuckets[newIndex] != nullptr) {
                newBuckets[newIndex]->prev = current;
            }

            newBuckets[newIndex] = current;


            // Sang node tiếp theo
            current = nextNode;
        }
    }


    // --------------------------------------------------------
    // 4. Xóa mảng bucket cũ
    // --------------------------------------------------------

    delete[] buckets;


    // --------------------------------------------------------
    // 5. Cập nhật con trỏ buckets
    // --------------------------------------------------------

    buckets = newBuckets;
}


// ============================================================
// Clear
// ============================================================

void HashTable::Clear() {

    for (int i = 0; i < capacity; i++) {

        HashNode* current = buckets[i];

        while (current != nullptr) {

            HashNode* nextNode =
                current->next;

            delete current;

            current = nextNode;
        }

        buckets[i] = nullptr;
    }

    sizeCount = 0;
}


// ============================================================
// Size
// ============================================================

int HashTable::Size() const {
    return sizeCount;
}


// ============================================================
// Is Empty
// ============================================================

bool HashTable::IsEmpty() const {
    return sizeCount == 0;
}


// ============================================================
// Get Capacity
// ============================================================

int HashTable::GetCapacity() const {
    return capacity;
}
