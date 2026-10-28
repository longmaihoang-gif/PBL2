#pragma once
#include <string>
#include <vector>
#include "Cart.h"
#include "Account.h"
#include "AccountStorage.h"
#include "OrderStorage.h"

// =================================================================
//  ORDER MANAGER: Tang xu ly thuat toan & nghiep vu thanh toan (Tang 3)
//  - Dieu phoi quy trinh Checkout: tru vi tien, luu CSV va in phieu txt
//  - Cung cap cac thuat toan thong ke doanh thu va loc lich su don hang
// =================================================================
class OrderManager {
private:
    OrderStorage* orderStorage;         // Con tro kho don hang (Tang 2: Data Access Layer)
    AccountStorage* accountStorage;     // Con tro kho tai khoan de dong bo so du vi tien (Tang 2)
    std::string invoiceFolderPath;      // Duong dan thu muc luu phieu in hoa don .txt

    // Sinh ma don hang tu dong (VD: ORD1003)
    std::string GenerateOrderID();

    // Ham bo tro lay ngay gio he thong hien tai dinh dang YYYY-MM-DD HH:MM
    static std::string GetCurrentDateTime();

    // Ham bo tro so sanh ngay dinh dang YYYY-MM-DD
    static bool IsDateBeforeOrEqual(const std::string& d1, const std::string& d2);

    // Ham bo tro kiem tra ngay co nam trong khoang [fromDate, toDate]
    static bool IsDateInRange(const std::string& targetDate, const std::string& fromDate, const std::string& toDate);

    // Ngan chan sao chep ngam
    OrderManager(const OrderManager&);
    OrderManager& operator=(const OrderManager&);

public:
    // Khoi tao quan ly don hang (Nhan con tro OrderStorage va AccountStorage tu ngoai vao)
    OrderManager(OrderStorage* ordStorage = nullptr,
                 AccountStorage* accStorage = nullptr,
                 const std::string& invoiceDir = "Invoices/");
    ~OrderManager();

    // Kiem tra trang thai nap du lieu tu tep
    bool IsLoaded() const;

    // Gan kho don hang va kho tai khoan (Dependency Injection)
    void SetOrderStorage(OrderStorage* ordStorage) { orderStorage = ordStorage; }
    void SetAccountStorage(AccountStorage* accStorage) { accountStorage = accStorage; }

    // =============================================================
    //  NGHIEP VU THANH TOAN (CHECKOUT FLOW)
    // =============================================================
    bool Checkout(Cart& cart, 
                  Account& customer, 
                  const std::string& cashierName = "Admin", 
                  const std::string& orderDate = "", 
                  std::string* outOrderID = nullptr, 
                  std::string* outReceiptPath = nullptr);

    // =============================================================
    //  NGHIEP VU THONG KE & BAO CAO (ANALYTICS)
    // =============================================================

    // Tinh tong doanh thu toan bo don hang
    unsigned long long GetTotalRevenue() const;

    // Lay tong so luong don hang da xu ly
    int GetTotalOrderCount() const;

    // Loc danh sach don hang theo khoang ngay [fromDate, toDate]
    std::vector<OrderRecord> FilterByDate(const std::string& fromDate, const std::string& toDate) const;

    // Loc danh sach don hang theo dinh danh nguoi mua (UID)
    std::vector<OrderRecord> FilterByCustomer(long long uid) const;

    // Lay toan bo danh sach don hang
    std::vector<OrderRecord> GetAllOrders() const;
};
