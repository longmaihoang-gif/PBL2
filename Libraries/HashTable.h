#pragma once
#include <string>
#include <vector>

// =================================================================
// HASH NODE: Nut danh sach lien ket doi cho Separate Chaining
// =================================================================
template <typename T>
struct HashNode {
    std::string key;
    T value;
    HashNode<T>* next;
    HashNode<T>* prev;

    HashNode(const std::string& k, const T& v)
        : key(k), value(v), next(nullptr), prev(nullptr) {}
};

// =================================================================
// HASH TABLE: Bang bam tu cai dat su dung Template Class
// =================================================================
template <typename T>
class HashTable {
private:
    HashNode<T>** buckets;         // Mang con tro dong chua cac danh sach lien ket
    int capacity;                  // Suc chua mang bucket
    int sizeCount;                 // So luong phan tu hien tai
    float loadFactorThreshold;     // Nguong qua tai de Rehash

    // Ham bam chuoi ky tu (Thuat toan DJB2)
    int HashFunction(const std::string& key) const {
        unsigned long hash = 5381;
        int len = (int)key.length();
        for (int i = 0; i < len; i++) {
            unsigned char c = (unsigned char)key[i];
            hash = hash * 33 + c;
        }
        return (int)(hash % capacity);
    }

    // Tai cau truc lai bang bam khi vuot nguong tai
    void Rehash() {
        int oldCapacity = capacity;
        int newCapacity = capacity * 2 + 1;

        HashNode<T>** newBuckets = new HashNode<T>*[newCapacity];
        for (int i = 0; i < newCapacity; i++) {
            newBuckets[i] = nullptr;
        }

        capacity = newCapacity;

        for (int i = 0; i < oldCapacity; i++) {
            HashNode<T>* current = buckets[i];
            while (current != nullptr) {
                HashNode<T>* nextNode = current->next;

                int newIndex = HashFunction(current->key);

                current->prev = nullptr;
                current->next = newBuckets[newIndex];

                if (newBuckets[newIndex] != nullptr) {
                    newBuckets[newIndex]->prev = current;
                }

                newBuckets[newIndex] = current;
                current = nextNode;
            }
        }

        delete[] buckets;
        buckets = newBuckets;
    }

    // Ngan chan sao chep ngam gay loi double-free
    HashTable(const HashTable&);
    HashTable& operator=(const HashTable&);

public:
    // Khoi tao bang bam
    HashTable(int initCapacity = 101, float threshold = 0.75f)
        : capacity(initCapacity), sizeCount(0), loadFactorThreshold(threshold) {
        if (capacity <= 0) {
            capacity = 101;
        }
        if (loadFactorThreshold <= 0.0f || loadFactorThreshold >= 1.0f) {
            loadFactorThreshold = 0.75f;
        }

        buckets = new HashNode<T>*[capacity];
        for (int i = 0; i < capacity; i++) {
            buckets[i] = nullptr;
        }
    }

    // Huy bang bam va thu hoi bo nho
    ~HashTable() {
        Clear();
        delete[] buckets;
    }

    // Chen hoac cap nhat cap (key, value)
    void Insert(const std::string& key, const T& value) {
        int index = HashFunction(key);
        HashNode<T>* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        float newLoadFactor = (sizeCount + 1) * 1.0f / capacity;
        if (newLoadFactor > loadFactorThreshold) {
            Rehash();
            index = HashFunction(key);
        }

        HashNode<T>* newNode = new HashNode<T>(key, value);
        newNode->next = buckets[index];
        newNode->prev = nullptr;

        if (buckets[index] != nullptr) {
            buckets[index]->prev = newNode;
        }

        buckets[index] = newNode;
        sizeCount++;
    }

    // Tra cuu phan tu theo key (Tra ve con tro toi value, nullptr neu khong tim thay)
    T* Search(const std::string& key) const {
        int index = HashFunction(key);
        HashNode<T>* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    // Ham bo tro Get tra ve con tro toi gia tri
    T* Get(const std::string& key) const {
        return Search(key);
    }

    // Kiem tra su ton tai cua key
    bool Contains(const std::string& key) const {
        return Search(key) != nullptr;
    }

    // Xoa phan tu theo key khoi bang bam
    bool Remove(const std::string& key) {
        int index = HashFunction(key);
        HashNode<T>* current = buckets[index];

        while (current != nullptr) {
            if (current->key == key) {
                if (current->prev != nullptr) {
                    current->prev->next = current->next;
                } else {
                    buckets[index] = current->next;
                }

                if (current->next != nullptr) {
                    current->next->prev = current->prev;
                }

                delete current;
                sizeCount--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Xoa toan bo cac phan tu trong bang bam
    void Clear() {
        for (int i = 0; i < capacity; i++) {
            HashNode<T>* current = buckets[i];
            while (current != nullptr) {
                HashNode<T>* nextNode = current->next;
                delete current;
                current = nextNode;
            }
            buckets[i] = nullptr;
        }
        sizeCount = 0;
    }

    // Lay so luong phan tu hien tai
    int Size() const {
        return sizeCount;
    }

    // Kiem tra bang bam rong
    bool IsEmpty() const {
        return sizeCount == 0;
    }

    // Lay suc chua hien tai cua mang bucket
    int GetCapacity() const {
        return capacity;
    }

    // Trich xuat toan bo cac gia tri trong bang bam vao mang vector
    std::vector<T> GetAllValues() const {
        std::vector<T> result;
        for (int i = 0; i < capacity; i++) {
            HashNode<T>* current = buckets[i];
            while (current != nullptr) {
                result.push_back(current->value);
                current = current->next;
            }
        }
        return result;
    }
};
