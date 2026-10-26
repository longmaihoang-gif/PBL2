#include "OrderManager.h"
#include <ctime>

using namespace std;

// =================================================================
//  CONSTRUCTOR & DESTRUCTOR
// =================================================================

OrderManager::OrderManager(AccountStorage* accStorage, const string& filePath, const string& invoiceDir)
    : accountStorage(accStorage), dataFilePath(filePath), invoiceFolderPath(invoiceDir), isLoadedSuccessfully(false) {
    // Dam bao duong dan thu muc hoa don co dau gach o cuoi
    if (!invoiceFolderPath.empty() && invoiceFolderPath.back() != '/' && invoiceFolderPath.back() != '\\') {
        invoiceFolderPath += "/";
    }

    isLoadedSuccessfully = LoadFromFile();
}

OrderManager::~OrderManager() {
}

// =================================================================
//  CAC PHUONG THUC NOI BO
// =================================================================

// Nap danh sach don hang tu CSV len RAM
bool OrderManager::LoadFromFile() {
    orderList.clear();
    return DataManager::LoadOrders(dataFilePath, orderList);
}

// Lay ngay gio he thong hien tai theo chuan YYYY-MM-DD HH:MM
string OrderManager::GetCurrentDateTime() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", ltm);
    return string(buffer);
}

// So sanh ngay dinh dang YYYY-MM-DD bang tim vi tri dau '-'
bool OrderManager::IsDateBeforeOrEqual(const string& d1, const string& d2) {
    int dash1A = (int)d1.find('-');
    int dash2A = (int)d1.rfind('-');
    int dash1B = (int)d2.find('-');
    int dash2B = (int)d2.rfind('-');

    if (dash1A == (int)string::npos || dash2A == (int)string::npos || dash1A == dash2A) return false;
    if (dash1B == (int)string::npos || dash2B == (int)string::npos || dash1B == dash2B) return false;

    int y1 = stoi(d1.substr(0, dash1A));
    int m1 = stoi(d1.substr(dash1A + 1, dash2A - dash1A - 1));
    int day1 = stoi(d1.substr(dash2A + 1));

    int y2 = stoi(d2.substr(0, dash1B));
    int m2 = stoi(d2.substr(dash1B + 1, dash2B - dash1B - 1));
    int day2 = stoi(d2.substr(dash2B + 1));

    if (y1 != y2) return y1 < y2;
    if (m1 != m2) return m1 < m2;
    return day1 <= day2;
}

// Kiem tra ngay co nam trong khoang [fromDate, toDate]
bool OrderManager::IsDateInRange(const string& targetDate, const string& fromDate, const string& toDate) {
    string targetOnlyDate = targetDate;
    int spacePos = (int)targetDate.find(' ');
    if (spacePos != (int)string::npos) {
        targetOnlyDate = targetDate.substr(0, spacePos);
    }

    if (!fromDate.empty() && !IsDateBeforeOrEqual(fromDate, targetOnlyDate)) {
        return false;
    }
    if (!toDate.empty() && !IsDateBeforeOrEqual(targetOnlyDate, toDate)) {
        return false;
    }
    return true;
}

// Sinh ma don hang tu dong (Tim chi so lon nhat va tang 1)
string OrderManager::GenerateOrderID() {
    int maxID = 1000;
    int count = (int)orderList.size();

    for (int i = 0; i < count; i++) {
        const string& idStr = orderList[i].OrderID;
        if (idStr.length() > 3 && idStr.substr(0, 3) == "ORD") {
            int num = stoi(idStr.substr(3));
            if (num > maxID) {
                maxID = num;
            }
        }
    }

    return "ORD" + to_string(maxID + 1);
}

// =================================================================
//  NGHIEP VU THANH TOAN (CHECKOUT FLOW)
// =================================================================

bool OrderManager::Checkout(Cart& cart, 
                            Account& customer, 
                            const string& cashierName, 
                            const string& orderDate, 
                            string* outOrderID, 
                            string* outReceiptPath) {
    if (cart.IsEmpty()) {
        return false;
    }

    long long totalAmount = cart.GetTotalAmount();
    if (totalAmount <= 0) {
        return false;
    }

    // 1. Kiem tra va khau tru so du vi tien cua khach hang
    if (!customer.costBalance((unsigned long long)totalAmount)) {
        return false; // So du khong du de thanh toan
    }

    // 2. Dong bo so du vi moi xuong Accounts.csv
    if (accountStorage != nullptr) {
        accountStorage->SaveToFile();
    }

    // 3. Xac dinh ngay gio giao dich
    string finalDate = orderDate;
    if (finalDate.empty()) {
        finalDate = GetCurrentDateTime();
    }

    // 4. Sinh ma don hang va tom tat mat hang
    string newOrderID = GenerateOrderID();
    string itemsSummary = cart.GetItemsSummary();

    // 5. Luu ban ghi vao danh sach bo nho RAM
    OrderRecord newOrder(newOrderID, customer.getUID(), customer.getName(), finalDate, (unsigned long long)totalAmount, itemsSummary);
    orderList.push_back(newOrder);

    // 6. Ghi noi tiep vao Data/Orders.csv
    DataManager::AppendOrder(dataFilePath, newOrder);

    // 7. Xuat phieu in hoa don .txt vao thu muc Invoices/
    string receiptPath = invoiceFolderPath + "HD_" + newOrderID + ".txt";
    cart.ExportReceiptTXT(receiptPath, newOrderID, customer.getName(), cashierName, finalDate);

    // 8. Tra ve thong tin neu co yeu cau
    if (outOrderID != nullptr) {
        *outOrderID = newOrderID;
    }
    if (outReceiptPath != nullptr) {
        *outReceiptPath = receiptPath;
    }

    // 9. Lam sach gio hang (khong hoan lai ton kho vi da thanh toan thanh cong)
    cart.Clear(nullptr);

    return true;
}

// =================================================================
//  NGHIEP VU THONG KE & BAO CAO (ANALYTICS)
// =================================================================

// Tinh tong doanh thu toan bo don hang
unsigned long long OrderManager::GetTotalRevenue() const {
    unsigned long long total = 0;
    int count = (int)orderList.size();
    for (int i = 0; i < count; i++) {
        total += orderList[i].TotalAmount;
    }
    return total;
}

// Lay tong so luong don hang da xu ly
int OrderManager::GetTotalOrderCount() const {
    return (int)orderList.size();
}

// Loc danh sach don hang theo khoang ngay [fromDate, toDate]
vector<OrderRecord> OrderManager::FilterByDate(const string& fromDate, const string& toDate) const {
    vector<OrderRecord> result;
    int count = (int)orderList.size();

    for (int i = 0; i < count; i++) {
        if (IsDateInRange(orderList[i].OrderDate, fromDate, toDate)) {
            result.push_back(orderList[i]);
        }
    }

    return result;
}

// Loc danh sach don hang theo dinh danh nguoi mua (UID)
vector<OrderRecord> OrderManager::FilterByCustomer(long long uid) const {
    vector<OrderRecord> result;
    int count = (int)orderList.size();

    for (int i = 0; i < count; i++) {
        if (orderList[i].UID == uid) {
            result.push_back(orderList[i]);
        }
    }

    return result;
}

// Lay toan bo danh sach don hang
vector<OrderRecord> OrderManager::GetAllOrders() const {
    return orderList;
}
