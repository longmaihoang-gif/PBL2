#include <iostream>
#include <string>
#include <vector>
#include <any>

#ifdef _WIN32
#include <windows.h>
#endif

// =================================================================
//  THU VIEN DU AN PBL2
// =================================================================
#include "../Libraries/Base.h"
#include "../Libraries/Trade.h"
#include "../Libraries/Account.h"
#include "../Libraries/Item.h"
#include "../Libraries/Shop.h"
#include "../Libraries/HashTable.h"
#include "../Libraries/DLinkedList.h"
#include "../Libraries/DataManager.h"

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    cout << "============================================================\n";
    cout << "   HE THONG QUAN LY CUA HANG & BAN HANG (PBL2)\n";
    cout << "============================================================\n\n";

    // 1. Khoi tao cac cau truc du lieu trong bo nho
    HashTable accountTable;
    HashTable productTable;
    vector<Account> accountList;
    vector<shared_ptr<ShopItem>> productList;
    vector<OrderRecord> orderList;

    // 2. Nap du lieu tu cac tep CSV
    cout << "[INFO] Dang nap du lieu tu thu muc Data/...\n";

    if (DataManager::LoadAccountsToTable("Data/Accounts.csv", accountTable, accountList)) {
        cout << "  -> Nap thanh cong " << accountList.size() << " tai khoan vao HashTable.\n";
    } else {
        cout << "  -> [Canh bao] Khong the mo hoac nap Data/Accounts.csv\n";
    }

    if (DataManager::LoadProductsToTable("Data/Products.csv", productTable, productList)) {
        cout << "  -> Nap thanh cong " << productList.size() << " san pham vao HashTable.\n";
    } else {
        cout << "  -> [Canh bao] Khong the mo hoac nap Data/Products.csv\n";
    }

    if (DataManager::LoadOrders("Data/Orders.csv", orderList)) {
        cout << "  -> Nap thanh cong " << orderList.size() << " don hang tu lich su.\n";
    } else {
        cout << "  -> [Canh bao] Khong the mo hoac nap Data/Orders.csv\n";
    }

    cout << "\n------------------------------------------------------------\n";
    cout << "DANH SACH SAN PHAM TRONG KHO (NAP TU CSV):\n";
    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < (int)productList.size(); i++) {
        const auto &item = productList[i];
        cout << "[" << item->ProductID << "] " << item->Name 
             << " | Gia: " << item->Price << " VND"
             << " | Ton kho: " << item->QuantityRemaining
             << " | Mo ta: " << item->Description << "\n";

        shared_ptr<FoodItem> food = dynamic_pointer_cast<FoodItem>(item);
        if (food) {
            cout << "      (Thuc pham - NSX: " << food->ManufactureDate 
                 << " | HSD: " << food->ExpiryDate << ")\n";
        }

        shared_ptr<ElectronicItem> elec = dynamic_pointer_cast<ElectronicItem>(item);
        if (elec) {
            cout << "      (Dien may - Bao hanh: " << elec->WarrantyMonths 
                 << " thang | Cong suat: " << elec->PowerWatts << "W)\n";
        }
    }

    cout << "\n[INFO] He thong khoi dong hoan tat va san sang hoat dong!\n";
    return 0;
}