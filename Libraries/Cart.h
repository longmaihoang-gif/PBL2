#pragma once
#include <string>
#include <vector>
#include "Base.h"
#include "Item.h"
#include "DLinkedList.h"
#include "HashTable.h"

using namespace std;

// =================================================================
//  CART ITEM: Muc hang hoa trong gio hang
// =================================================================
struct CartItem {
    string ProductID;        // Ma san pham
    string ProductName;      // Ten san pham
    string Category;         // Phan loai (Food / Electronics)
    int Quantity;            // So luong mua
    long long UnitPrice;     // Don gia tai thoi diem them vao gio
    long long SubTotal;      // Thanh tien = Quantity * UnitPrice

    CartItem(string id = "", string name = "", string cat = "", int qty = 0, long long price = 0)
        : ProductID(id), ProductName(name), Category(cat), Quantity(qty), UnitPrice(price) {
        SubTotal = (long long)qty * price;
    }
};

// =================================================================
//  CART: Gio hang ban le ke thua Entity, su dung DLinkedList
// =================================================================
class Cart : public Entity {
    private:
        Node* head;          // Con tro quan ly nut dau gio hang
        Node* tail;          // Con tro quan ly nut cuoi (dong vai tro dinh Undo)
        int totalItems;      // Tong so loai mat hang trong gio

        // Khai bao private de ngan chan sao chep ngam gay loi double-free
        Cart(const Cart&);
        Cart& operator=(const Cart&);

    public:
        Cart(string ownerName = "Khach vang lai");
        virtual ~Cart();

        // Nghiep vu gio hang & Undo
        bool AddItem(ShopItem* product, int quantity);
        bool Undo(HashTable &productTable, CartItem &undoneItem);
        void Clear(HashTable *productTableToRestore = nullptr);

        // Truy xuat thong tin
        long long GetTotalAmount() const;
        int GetItemCount() const;
        bool IsEmpty() const;
        vector<CartItem> ToVector() const;
        string GetItemsSummary() const;

        // Hien thi giao dien
        void PrintCart() const;
};
