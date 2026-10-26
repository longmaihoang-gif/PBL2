#include "AccountStorage.h"
#include "DataManager.h"

using namespace std;

// =================================================================
//  CONSTRUCTOR & DESTRUCTOR
// =================================================================

AccountStorage::AccountStorage(const string& filePath)
    : dataFilePath(filePath), isLoadedSuccessfully(false) {
    isLoadedSuccessfully = LoadFromFile();
}

AccountStorage::~AccountStorage() {
    // HashTable tu dong thu hoi bo nho bucket va node trong destructor cua no
}

// =================================================================
//  CAC PHUONG THUC NOI BO
// =================================================================

// Nap danh sach tai khoan tu CSV len Bang bam RAM
bool AccountStorage::LoadFromFile() {
    accountTable.Clear();
    vector<Account> loadedList;
    if (!DataManager::LoadAccounts(dataFilePath, loadedList)) {
        return false;
    }

    int count = (int)loadedList.size();
    for (int i = 0; i < count; i++) {
        accountTable.Insert(loadedList[i].getUsername(), loadedList[i]);
    }
    return true;
}

// Dong bo toan bo tai khoan tu Bang bam xuong tep CSV
bool AccountStorage::SaveToFile() {
    vector<Account> allAccounts = accountTable.GetAllValues();
    return DataManager::SaveAccounts(dataFilePath, allAccounts);
}

// =================================================================
//  TRUY VAN & THAO TAC DU LIEU
// =================================================================

// Tra cuu tai khoan theo username voi thoi gian trung binh O(1)
Account* AccountStorage::GetByUsername(const string& username) {
    return accountTable.Search(username);
}

// Kiem tra su ton tai cua ten dang nhap
bool AccountStorage::Exists(const string& username) {
    return accountTable.Contains(username);
}

// Them tai khoan moi (Write-Through: Nap RAM va ghi them dong vao CSV)
bool AccountStorage::AddAccount(const Account& acc) {
    if (Exists(acc.getUsername())) {
        return false;
    }

    // 1. Cap nhat tren RAM
    accountTable.Insert(acc.getUsername(), acc);

    // 2. Ghi dong bo xuong tep CSV
    vector<string> row;
    row.push_back(to_string(acc.getUID()));
    row.push_back(acc.getUsername());
    row.push_back(acc.getPassword());
    row.push_back(acc.getName());
    row.push_back(acc.getPermission() == Admin ? "0" : "1");
    row.push_back(to_string(acc.getBalance()));

    return DataManager::AppendCSVRow(dataFilePath, row);
}

// Cap nhat mat khau cho tai khoan va dong bo xuong CSV
bool AccountStorage::UpdatePassword(const string& username, const string& newPassword) {
    Account* acc = GetByUsername(username);
    if (acc == nullptr) {
        return false;
    }

    acc->setPassword(newPassword);
    return SaveToFile();
}

// Cap nhat thong tin tai khoan va dong bo xuong CSV
bool AccountStorage::UpdateAccount(const Account& acc) {
    Account* existing = GetByUsername(acc.getUsername());
    if (existing == nullptr) {
        return false;
    }

    *existing = acc;
    return SaveToFile();
}

// Xoa tai khoan khoi he thong va dong bo xuong CSV
bool AccountStorage::RemoveAccount(const string& username) {
    if (!accountTable.Remove(username)) {
        return false;
    }
    return SaveToFile();
}

// Lay toan bo danh sach tai khoan (phuc vu bang quan tri Admin)
vector<Account> AccountStorage::GetAllAccounts() {
    return accountTable.GetAllValues();
}
