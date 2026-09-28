#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include "Account.h"
#include "Item.h"
#include "Shop.h"
#include "HashTable.h"

using namespace std;

// =================================================================
//  ORDER RECORD: Cau truc ban ghi lich su don hang
// =================================================================
struct OrderRecord {
    string OrderID;
    long long UID;
    string CustomerName;
    string OrderDate;
    unsigned long long TotalAmount;
    string ItemsSummary;

    OrderRecord(string id = "", long long uid = 0, string name = "", string date = "", unsigned long long amount = 0, string summary = "")
        : OrderID(id), UID(uid), CustomerName(name), OrderDate(date), TotalAmount(amount), ItemsSummary(summary) {}
};

// =================================================================
//  DATA MANAGER: Module quan ly doc, ghi va boc tach tep CSV
// =================================================================
class DataManager {
public:
    // Boc tach mot dong CSV theo chuan RFC 4180 (ho tro dau phay trong dau ngoac kep)
    static vector<string> ParseCSVLine(const string &line);

    // Dinh dang chuoi an toan khi ghi xuong CSV (tu dong boc dau ngoac kep neu chua dau phay)
    static string EscapeCSV(const string &text);

    // =============================================================
    // Quan ly Tai khoan (Accounts)
    // =============================================================
    static bool LoadAccounts(const string &filePath, vector<Account> &accountList);
    static bool SaveAccounts(const string &filePath, const vector<Account> &accountList);
    static bool LoadAccountsToTable(const string &filePath, HashTable &accountTable, vector<Account> &accountList);

    // =============================================================
    // Quan ly San pham & Kho hang (Products)
    // =============================================================
    static bool LoadProducts(const string &filePath, vector<shared_ptr<ShopItem>> &productList);
    static bool SaveProducts(const string &filePath, const vector<shared_ptr<ShopItem>> &productList);
    static bool LoadProductsToTable(const string &filePath, HashTable &productTable, vector<shared_ptr<ShopItem>> &productList);

    // =============================================================
    // Quan ly Hoa don & Doanh thu (Orders)
    // =============================================================
    static bool LoadOrders(const string &filePath, vector<OrderRecord> &orderList);
    static bool AppendOrder(const string &filePath, const OrderRecord &order);
};
