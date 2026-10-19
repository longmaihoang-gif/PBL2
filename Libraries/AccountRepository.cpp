#include "AccountRepository.h"
#include "DataManager.h"

using namespace std;

// =================================================================
//  CONSTRUCTOR & DESTRUCTOR
// =================================================================

AccountRepository::AccountRepository(const string& filePath)
    : dataFilePath(filePath), isLoadedSuccessfully(false) {
    isLoadedSuccessfully = LoadFromDisk();
}

AccountRepository::~AccountRepository() {
    // HashTable tu dong thu hoi bo nho bucket va node trong destructor cua no
}

// =================================================================
//  CAC PHUONG THUC NOI BO
// =================================================================

// Nap danh sach tai khoan tu CSV len Bang bam RAM
bool AccountRepository::LoadFromDisk() {
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
bool AccountRepository::SyncToDisk() {
    vector<Account> allAccounts = accountTable.GetAllValues<Account>();
    return DataManager::SaveAccounts(dataFilePath, allAccounts);
}

// =================================================================
//  TRUY VAN & THAO TAC DU LIEU
// =================================================================

// Tra cuu tai khoan theo username voi thoi gian trung binh O(1)
Account* AccountRepository::GetByUsername(const string& username) {
    return accountTable.Get<Account>(username);
}

// Kiem tra su ton tai cua ten dang nhap
bool AccountRepository::Exists(const string& username) {
    return accountTable.Contains(username);
}

// Them tai khoan moi (Write-Through: Nap RAM va ghi them dong vao CSV)
bool AccountRepository::AddAccount(const Account& acc) {
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
bool AccountRepository::UpdatePassword(const string& username, const string& newPassword) {
    Account* acc = GetByUsername(username);
    if (acc == nullptr) {
        return false;
    }

    acc->setPassword(newPassword);
    return SyncToDisk();
}

// Cap nhat thong tin tai khoan va dong bo xuong CSV
bool AccountRepository::UpdateAccount(const Account& acc) {
    Account* existing = GetByUsername(acc.getUsername());
    if (existing == nullptr) {
        return false;
    }

    *existing = acc;
    return SyncToDisk();
}

// Xoa tai khoan khoi he thong va dong bo xuong CSV
bool AccountRepository::RemoveAccount(const string& username) {
    if (!accountTable.Remove(username)) {
        return false;
    }
    return SyncToDisk();
}

// Lay toan bo danh sach tai khoan (phuc vu bang quan tri Admin)
vector<Account> AccountRepository::GetAllAccounts() {
    return accountTable.GetAllValues<Account>();
}

