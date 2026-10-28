#include "ProductStorage.h"
#include "DataManager.h"
#include <cctype>

using namespace std;

// =================================================================
//  CONSTRUCTOR & DESTRUCTOR
// =================================================================

ProductStorage::ProductStorage(const string& filePath)
    : dataFilePath(filePath), isLoadedSuccessfully(false) {
    isLoadedSuccessfully = LoadFromFile();
}

ProductStorage::~ProductStorage() {
    // Thu hoi toan bo bo nho con tro san pham tren Heap truoc khi huy doi tuong
    vector<ShopItem*> allItems = productTable.GetAllValues();
    int count = (int)allItems.size();
    for (int i = 0; i < count; i++) {
        delete allItems[i];
    }
    productTable.Clear();
}

// =================================================================
//  CAC PHUONG THUC NOI BO
// =================================================================

// Nap danh sach san pham tu CSV len Bang bam RAM
bool ProductStorage::LoadFromFile() {
    // Giai phong bo nho cu neu bang bam da chua phan tu
    vector<ShopItem*> oldItems = productTable.GetAllValues();
    int oldCount = (int)oldItems.size();
    for (int i = 0; i < oldCount; i++) {
        delete oldItems[i];
    }
    productTable.Clear();

    vector<ShopItem*> loadedList;
    if (!DataManager::LoadProducts(dataFilePath, loadedList)) {
        return false;
    }

    int count = (int)loadedList.size();
    for (int i = 0; i < count; i++) {
        productTable.Insert(loadedList[i]->ProductID, loadedList[i]);
    }
    return true;
}

// Dong bo toan bo san pham tu Bang bam xuong tep CSV
bool ProductStorage::SaveToFile() {
    vector<ShopItem*> allProducts = productTable.GetAllValues();
    return DataManager::SaveProducts(dataFilePath, allProducts);
}

// =================================================================
//  TRUY VAN & THAO TAC DU LIEU
// =================================================================

// Tra cuu san pham theo ma san pham voi thoi gian trung binh O(1)
ShopItem* ProductStorage::GetByID(const string& productID) {
    ShopItem** pItem = productTable.Search(productID);
    if (pItem == nullptr) {
        return nullptr;
    }
    return *pItem;
}

// Kiem tra su ton tai cua ma san pham
bool ProductStorage::Exists(const string& productID) {
    return productTable.Contains(productID);
}

// Them san pham moi (Write-Through: Nap RAM va luu vao CSV)
bool ProductStorage::AddProduct(ShopItem* newItem) {
    if (newItem == nullptr || Exists(newItem->ProductID)) {
        return false;
    }

    // 1. Luu con tro vao bang bam
    productTable.Insert(newItem->ProductID, newItem);

    // 2. Dong bo toan bo danh muc xuong tep CSV
    return SaveToFile();
}

// Cap nhat toan bo thong tin san pham va dong bo xuong CSV
bool ProductStorage::UpdateProduct(ShopItem* updatedItem) {
    if (updatedItem == nullptr) {
        return false;
    }

    ShopItem* existing = GetByID(updatedItem->ProductID);
    if (existing == nullptr) {
        return false;
    }

    // Cap nhat cac thuoc tinh co ban
    existing->Name = updatedItem->Name;
    existing->Category = updatedItem->Category;
    existing->Price = updatedItem->Price;
    existing->QuantityRemaining = updatedItem->QuantityRemaining;
    existing->Description = updatedItem->Description;

    // Cap nhat thuoc tinh chuyen biet da hinh
    FoodItem* existingFood = dynamic_cast<FoodItem*>(existing);
    FoodItem* updatedFood = dynamic_cast<FoodItem*>(updatedItem);
    if (existingFood != nullptr && updatedFood != nullptr) {
        existingFood->ManufactureDate = updatedFood->ManufactureDate;
        existingFood->ExpiryDate = updatedFood->ExpiryDate;
    }

    ElectronicItem* existingElec = dynamic_cast<ElectronicItem*>(existing);
    ElectronicItem* updatedElec = dynamic_cast<ElectronicItem*>(updatedItem);
    if (existingElec != nullptr && updatedElec != nullptr) {
        existingElec->WarrantyMonths = updatedElec->WarrantyMonths;
        existingElec->PowerWatts = updatedElec->PowerWatts;
    }

    return SaveToFile();
}

// Cap nhat so luong ton kho va dong bo xuong CSV
bool ProductStorage::UpdateQuantity(const string& productID, int newQuantity) {
    if (newQuantity < 0) {
        return false;
    }

    ShopItem* item = GetByID(productID);
    if (item == nullptr) {
        return false;
    }

    item->QuantityRemaining = newQuantity;
    return SaveToFile();
}

// Cap nhat gia ban va dong bo xuong CSV
bool ProductStorage::UpdatePrice(const string& productID, long long newPrice) {
    if (newPrice < 0) {
        return false;
    }

    ShopItem* item = GetByID(productID);
    if (item == nullptr) {
        return false;
    }

    item->Price = newPrice;
    return SaveToFile();
}

// Xoa san pham khoi he thong, thu hoi bo nho Heap va dong bo xuong CSV
bool ProductStorage::RemoveProduct(const string& productID) {
    ShopItem** pItem = productTable.Search(productID);
    if (pItem == nullptr) {
        return false;
    }

    ShopItem* itemToDelete = *pItem;

    if (!productTable.Remove(productID)) {
        return false;
    }

    delete itemToDelete;
    return SaveToFile();
}

// Lay toan bo danh sach san pham
vector<ShopItem*> ProductStorage::GetAllProducts() {
    return productTable.GetAllValues();
}
