#include "HashTable.h"
#include <stdexcept>

using namespace std;

// ============================================================
// Constructor
// ============================================================

HashTable::HashTable(size_t initCapacity, float threshold)
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
    for (size_t i = 0; i < capacity; i++) {
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

size_t HashTable::HashFunction(const string& key) const {

    unsigned long hash = 5381;

    for (unsigned char c : key) {
        hash = ((hash << 5) + hash) + c;
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

    size_t index = HashFunction(key);

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

    float newLoadFactor =
        static_cast<float>(sizeCount + 1) / capacity;

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
// Search - phiên bản thường
// ============================================================

any* HashTable::Search(const string& key) {

    size_t index = HashFunction(key);

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
// Search - phiên bản const
// ============================================================

const any* HashTable::Search(const string& key) const {

    size_t index = HashFunction(key);

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

    size_t index = HashFunction(key);

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

bool HashTable::Contains(const string& key) const {

    return Search(key) != nullptr;
}


// ============================================================
// Rehash
// ============================================================

void HashTable::Rehash() {

    // --------------------------------------------------------
    // 1. Tính capacity mới
    // --------------------------------------------------------

    size_t oldCapacity = capacity;

    size_t newCapacity =
        capacity * 2 + 1;


    // --------------------------------------------------------
    // 2. Tạo bucket mới
    // --------------------------------------------------------

    HashNode** newBuckets =
        new HashNode*[newCapacity];

    for (size_t i = 0; i < newCapacity; i++) {
        newBuckets[i] = nullptr;
    }


    // --------------------------------------------------------
    // 3. Chuyển tất cả node sang bucket mới
    // --------------------------------------------------------

    for (size_t i = 0; i < oldCapacity; i++) {

        HashNode* current = buckets[i];

        while (current != nullptr) {

            // Lưu node tiếp theo
            HashNode* nextNode = current->next;


            // ------------------------------------------------
            // Tính hash theo capacity mới
            // ------------------------------------------------

            unsigned long hash = 5381;

            for (unsigned char c : current->key) {
                hash = ((hash << 5) + hash) + c;
            }

            size_t newIndex =
                hash % newCapacity;


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
    // 5. Cập nhật HashTable
    // --------------------------------------------------------

    buckets = newBuckets;
    capacity = newCapacity;
}


// ============================================================
// Clear
// ============================================================

void HashTable::Clear() {

    for (size_t i = 0; i < capacity; i++) {

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

size_t HashTable::Size() const {

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

size_t HashTable::GetCapacity() const {

    return capacity;
}