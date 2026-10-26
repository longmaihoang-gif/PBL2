#include "Cart.h"
#include <iostream>
#include <iomanip>

using namespace std;

// =================================================================
//  Khoi tao & Huy gio hang
// =================================================================
Cart::Cart(string ownerName) : head(nullptr), tail(nullptr), totalItems(0) {
    Name = "Gio hang cua " + ownerName;
}

Cart::~Cart() {
    // Thu hoi toan bo cac node con lai khi huy doi tuong
    Clear(nullptr);
}

// =================================================================
//  Them san pham vao gio hang (Soft Reservation kho hang)
// =================================================================
bool Cart::AddItem(ShopItem* product, int quantity) {
    if (product == nullptr || quantity <= 0) {
        return false;
    }

    // Kiem tra so luong ton kho co du de ban hay khong
    if (product->QuantityRemaining < quantity) {
        return false;
    }

    // Giam tru tam thoi ton kho trong bo nho
    product->QuantityRemaining -= quantity;

    // Tao du lieu muc gio hang
    CartItem item(product->ProductID, product->Name, product->Category, quantity, product->Price);

    // Tao node moi su dung Node<CartItem>
    Node<CartItem>* newNode = new Node<CartItem>(item);

    // Chen node moi vao cuoi danh sach lien ket doi
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->Insert(newNode);
        tail = newNode;
    }

    totalItems++;
    return true;
}

// =================================================================
//  Hoan tac thao tac them gan nhat (Undo LIFO)
// =================================================================
bool Cart::Undo(HashTable<ShopItem*> &productTable, CartItem &undoneItem) {
    if (IsEmpty()) {
        return false;
    }

    // Lay du lieu muc gio hang o cuoi danh sach (tail)
    undoneItem = tail->value;

    // Hoan tra lai so luong ton kho cho san pham trong HashTable
    ShopItem** pProduct = productTable.Search(undoneItem.ProductID);
    if (pProduct != nullptr && *pProduct != nullptr) {
        (*pProduct)->QuantityRemaining += undoneItem.Quantity;
    }

    // Ngat ket noi va thu hoi node o duoi danh sach
    Node<CartItem>* toDelete = tail;
    tail = tail->prev;

    toDelete->Remove();
    delete toDelete;

    // Neu danh sach khong con phan tu, dat lai head ve nullptr
    if (tail == nullptr) {
        head = nullptr;
    }

    totalItems--;
    return true;
}

// =================================================================
//  Don dep gio hang (Ho tro tuy chon hoan tra ton kho)
// =================================================================
void Cart::Clear(HashTable<ShopItem*> *productTableToRestore) {
    Node<CartItem>* curr = head;

    while (curr != nullptr) {
        Node<CartItem>* nextNode = curr->next;

        // Neu co yeu cau hoan tra ton kho (truong hop huy gio hang truoc khi thanh toan)
        if (productTableToRestore != nullptr) {
            ShopItem** pProduct = productTableToRestore->Search(curr->value.ProductID);
            if (pProduct != nullptr && *pProduct != nullptr) {
                (*pProduct)->QuantityRemaining += curr->value.Quantity;
            }
        }

        delete curr;
        curr = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    totalItems = 0;
}

// =================================================================
//  Tinh tong tien cua gio hang
// =================================================================
long long Cart::GetTotalAmount() const {
    long long total = 0;
    Node<CartItem>* curr = head;

    while (curr != nullptr) {
        total += curr->value.SubTotal;
        curr = curr->next;
    }

    return total;
}

// =================================================================
//  Lay so luong loai mat hang
// =================================================================
int Cart::GetItemCount() const {
    return totalItems;
}

// =================================================================
//  Kiem tra gio hang rong
// =================================================================
bool Cart::IsEmpty() const {
    return head == nullptr;
}

// =================================================================
//  Chuyen danh sach lien ket sang vector de phuc vu in an hoac xuat file
// =================================================================
vector<CartItem> Cart::ToVector() const {
    vector<CartItem> list;
    Node<CartItem>* curr = head;

    while (curr != nullptr) {
        list.push_back(curr->value);
        curr = curr->next;
    }

    return list;
}

// =================================================================
//  Tao chuoi tong ket mat hang (ghi vao Orders.csv)
// =================================================================
string Cart::GetItemsSummary() const {
    string summary = "";
    Node<CartItem>* curr = head;
    bool isFirst = true;

    while (curr != nullptr) {
        if (!isFirst) {
            summary += "; ";
        }
        summary += curr->value.ProductName + " (x" + to_string(curr->value.Quantity) + ")";
        isFirst = false;
        curr = curr->next;
    }

    return summary;
}

// =================================================================
//  In danh sach gio hang ra man hinh
// =================================================================
void Cart::PrintCart() const {
    cout << "\n================================================================================\n";
    cout << "                         " << Name << "\n";
    cout << "================================================================================\n";

    if (IsEmpty()) {
        cout << "  (Gio hang hien tai dang trong)\n";
        cout << "================================================================================\n";
        return;
    }

    cout << left << setw(5) << "STT"
         << setw(12) << "Ma SP"
         << setw(32) << "Ten San Pham"
         << right << setw(10) << "Don Gia"
         << setw(6) << "SL"
         << setw(13) << "Thanh Tien" << "\n";
    cout << "--------------------------------------------------------------------------------\n";

    Node<CartItem>* curr = head;
    int index = 1;
    long long grandTotal = 0;

    while (curr != nullptr) {
        const CartItem& item = curr->value;
        cout << left << setw(5) << index
             << setw(12) << item.ProductID
             << setw(32) << item.ProductName
             << right << setw(10) << item.UnitPrice
             << setw(6) << item.Quantity
             << setw(13) << item.SubTotal << "\n";
        grandTotal += item.SubTotal;
        index++;
        curr = curr->next;
    }

    cout << "--------------------------------------------------------------------------------\n";
    cout << right << setw(65) << "TONG TIEN THANH TOAN: " << setw(13) << grandTotal << " VND\n";
    cout << "================================================================================\n";
}
