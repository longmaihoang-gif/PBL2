#pragma once
#include <string>
#include <vector>
#include "Cart.h"
#include "Account.h"
#include "AccountStorage.h"
#include "DataManager.h"

using namespace std;

// =================================================================
//  ORDER MANAGER: He thong dieu phoi thanh toan va quan ly hoa don
//  - Dieu phoi quy trinh Checkout: tru vi tien, luu CSV va in phieu txt
//  - Cung cap cac phuong thuc thong ke doanh thu va loc lich su don hang
// =================================================================
class OrderManager {
private:
    AccountStorage* accountStorage;     // Con tro kho tai khoan de dong bo so du
    vector<OrderRecord> orderList;      // Danh sach toan bo don hang tren RAM
    string dataFilePath;                // Duong dan tep CSV don hang
    string invoiceFolderPath;           // Duong dan thu muc luu phieu in hoa don .txt
    bool isLoadedSuccessfully;          // Trang thai nap du lieu ban dau

    // Nap danh sach don hang tu CSV len RAM
    bool LoadFromFile();

    // Sinh ma don hang tu dong (VD: ORD1003)
    string GenerateOrderID();

    // Ham bo tro lay ngay gio he thong hien tai dinh dang YYYY-MM-DD HH:MM
    static string GetCurrentDateTime();

    // Ham bo tro so sanh ngay dinh dang YYYY-MM-DD
    static bool IsDateBeforeOrEqual(const string& d1, const string& d2);

    // Ham bo tro kiem tra ngay co nam trong khoang [fromDate, toDate]
    static bool IsDateInRange(const string& targetDate, const string& fromDate, const string& toDate);

    // Ngan chan sao chep ngam
    OrderManager(const OrderManager&);
    OrderManager& operator=(const OrderManager&);

public:
    // Khoi tao va nap du lieu lich su don hang
    OrderManager(AccountStorage* accStorage = nullptr, 
                 const string& filePath = "Data/Orders.csv", 
                 const string& invoiceDir = "Invoices/");
    ~OrderManager();

    // Kiem tra trang thai nap du lieu tu tep
    bool IsLoaded() const { return isLoadedSuccessfully; }

    // Gan kho tai khoan phuc vu dong bo Write-Through vi tien
    void SetAccountStorage(AccountStorage* accStorage) { accountStorage = accStorage; }

    // =============================================================
    //  NGHIEP VU THANH TOAN (CHECKOUT FLOW)
    // =============================================================
    bool Checkout(Cart& cart, 
                  Account& customer, 
                  const string& cashierName = "Admin", 
                  const string& orderDate = "", 
                  string* outOrderID = nullptr, 
                  string* outReceiptPath = nullptr);

    // =============================================================
    //  NGHIEP VU THONG KE & BAO CAO (ANALYTICS)
    // =============================================================

    // Tinh tong doanh thu toan bo don hang
    unsigned long long GetTotalRevenue() const;

    // Lay tong so luong don hang da xu ly
    int GetTotalOrderCount() const;

    // Loc danh sach don hang theo khoang ngay [fromDate, toDate]
    vector<OrderRecord> FilterByDate(const string& fromDate, const string& toDate) const;

    // Loc danh sach don hang theo dinh danh nguoi mua (UID)
    vector<OrderRecord> FilterByCustomer(long long uid) const;

    // Lay toan bo danh sach don hang
    vector<OrderRecord> GetAllOrders() const;
};
