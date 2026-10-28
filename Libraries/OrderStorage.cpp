#include "OrderStorage.h"

using namespace std;

// =================================================================
//  KHOI TAO & HUY KHO DON HANG
// =================================================================

OrderStorage::OrderStorage(const string& filePath)
    : dataFilePath(filePath), isLoadedSuccessfully(false) {
    isLoadedSuccessfully = LoadFromFile();
}

OrderStorage::~OrderStorage() {
    // Thu hoi vector tu dong khi huy doi tuong
}

// =================================================================
//  THAO TAC DOC / GHI CSV
// =================================================================

// Nap toan bo don hang tu tep CSV len bo nho RAM
bool OrderStorage::LoadFromFile() {
    orderList.clear();
    return DataManager::LoadOrders(dataFilePath, orderList);
}

// Luu de toan bo danh sach don hang tu RAM xuong tep CSV
bool OrderStorage::SaveToFile() {
    return DataManager::SaveOrders(dataFilePath, orderList);
}

// Them don hang moi: Luu tren RAM va ghi noi tiep xuong CSV (Write-Through)
bool OrderStorage::AddOrder(const OrderRecord& order) {
    orderList.push_back(order);
    return DataManager::AppendOrder(dataFilePath, order);
}

// Tra cuu don hang theo ma dinh danh OrderID
const OrderRecord* OrderStorage::GetOrderByID(const string& orderID) const {
    int total = (int)orderList.size();
    for (int i = 0; i < total; i++) {
        if (orderList[i].OrderID == orderID) {
            return &orderList[i];
        }
    }
    return nullptr;
}

