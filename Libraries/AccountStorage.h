#pragma once
#include <string>
#include <vector>
#include "Account.h"
#include "HashTable.h"

using namespace std;

// =================================================================
//  ACCOUNT STORAGE: He thong trung gian quan ly du lieu tai khoan
//  - Dong vai tro cau noi duy nhat giua Bang bam (RAM) va Tep CSV (Disk)
//  - Ap dung co che Write-Through dam bao khong xung dot du lieu
// =================================================================
class AccountStorage {
private:
    HashTable<Account> accountTable; // Bang bam luu tru tai khoan tren RAM (Tra cuu O(1))
    string dataFilePath;            // Duong dan tep CSV du lieu tai khoan
    bool isLoadedSuccessfully;      // Trang thai nap du lieu ban dau

    // Nap du lieu tu tep CSV vao bang bam RAM
    bool LoadFromFile();

    // Dong bo toan bo du lieu tu bang bam xuong tep CSV
    bool SaveToFile();

public:
    // Khoi tao va tu dong nap du lieu tu tep tin
    AccountStorage(const string& filePath = "Data/Accounts.csv");
    ~AccountStorage();

    // Kiem tra trang thai nap du lieu tu tep
    bool IsLoaded() const { return isLoadedSuccessfully; }

    // Tra cuu tai khoan theo ten dang nhap (Do phuc tap O(1))
    Account* GetByUsername(const string& username);

    // Kiem tra tai khoan co ton tai trong he thong hay khong
    bool Exists(const string& username);

    // Them tai khoan moi (Ghi dong thoi vao RAM va tep CSV)
    bool AddAccount(const Account& acc);

    // Cap nhat mat khau cho tai khoan va dong bo xuong tep CSV
    bool UpdatePassword(const string& username, const string& newPassword);

    // Cap nhat toan bo thong tin tai khoan va dong bo xuong tep CSV
    bool UpdateAccount(const Account& acc);

    // Xoa tai khoan khoi he thong va dong bo xuong tep CSV
    bool RemoveAccount(const string& username);

    // Trich xuat toan bo danh sach tai khoan phuc vu hien thi bang
    vector<Account> GetAllAccounts();
};
