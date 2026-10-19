#include "AuthManager.h"

using namespace std;

// =================================================================
//  CONSTRUCTOR & DESTRUCTOR
// =================================================================

AuthManager::AuthManager(AccountRepository* repo)
    : repository(repo), currentAccount(nullptr) {}

AuthManager::~AuthManager() {
    currentAccount = nullptr;
}

// =================================================================
//  NGHIEP VU XAC THUC
// =================================================================

// Dang nhap tai khoan
AuthResult AuthManager::Login(const string& username, const string& password) {
    if (username.empty() || password.empty()) {
        return AUTH_ERR_EMPTY_INPUT;
    }

    if (repository == nullptr) {
        return AUTH_ERR_STORAGE_FAILED;
    }

    // Tra cuu tai khoan qua he thong trung gian voi toc do O(1)
    Account* acc = repository->GetByUsername(username);
    if (acc == nullptr) {
        return AUTH_ERR_USER_NOT_FOUND;
    }

    // Kiem tra tinh chinh xac cua mat khau
    if (!acc->VerifyPassword(password)) {
        return AUTH_ERR_WRONG_PASSWORD;
    }

    // Thiet lap phien lam viec hien tai
    currentAccount = acc;
    return AUTH_SUCCESS;
}

// Dang xuat khoi he thong
void AuthManager::Logout() {
    currentAccount = nullptr;
}

// Kiem tra trang thai dang nhap
bool AuthManager::IsLoggedIn() const {
    return currentAccount != nullptr;
}

// Lay con tro tai khoan hien tai
Account* AuthManager::GetCurrentAccount() const {
    return currentAccount;
}

// Lay quyen han cua phien lam viec
Role AuthManager::GetCurrentRole() const {
    if (currentAccount == nullptr) {
        return User;
    }
    return currentAccount->getPermission();
}

// Dang ky tai khoan moi
AuthResult AuthManager::Register(const string& username, const string& password, const string& fullName, Role role, unsigned long long initialMoney) {
    if (username.empty() || password.empty() || fullName.empty()) {
        return AUTH_ERR_EMPTY_INPUT;
    }

    if (repository == nullptr) {
        return AUTH_ERR_STORAGE_FAILED;
    }

    // Kiem tra trung lap ten dang nhap
    if (repository->Exists(username)) {
        return AUTH_ERR_USER_ALREADY_EXISTS;
    }

    // Khoi tao doi tuong tai khoan moi (GUID tu dong dong bo)
    Account newAcc(username, password, fullName, role, initialMoney);

    // Luu dong thoi vao Bang bam va tep CSV (Write-Through)
    if (!repository->AddAccount(newAcc)) {
        return AUTH_ERR_STORAGE_FAILED;
    }

    return AUTH_SUCCESS;
}

// Doi mat khau cho phien lam viec hien tai
AuthResult AuthManager::ChangePassword(const string& oldPassword, const string& newPassword) {
    if (currentAccount == nullptr) {
        return AUTH_ERR_NOT_LOGGED_IN;
    }

    if (oldPassword.empty() || newPassword.empty()) {
        return AUTH_ERR_EMPTY_INPUT;
    }

    // Kiem tra mat khau cu
    if (!currentAccount->VerifyPassword(oldPassword)) {
        return AUTH_ERR_WRONG_PASSWORD;
    }

    if (repository == nullptr) {
        return AUTH_ERR_STORAGE_FAILED;
    }

    // Cap nhat mat khau moi tren RAM va dong bo xuong CSV
    if (!repository->UpdatePassword(currentAccount->getUsername(), newPassword)) {
        return AUTH_ERR_STORAGE_FAILED;
    }

    return AUTH_SUCCESS;
}
