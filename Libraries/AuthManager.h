#pragma once
#include <string>
#include "Account.h"
#include "AccountStorage.h"

using namespace std;

// =================================================================
//  AUTH RESULT: Mã kết quả các nghiệp vụ xác thực
// =================================================================
enum AuthResult {
    AUTH_SUCCESS = 0,               // Thao tác thành công
    AUTH_ERR_EMPTY_INPUT,           // Dữ liệu nhập vào bị bỏ trống
    AUTH_ERR_USER_NOT_FOUND,        // Tên tài khoản không tồn tại
    AUTH_ERR_WRONG_PASSWORD,        // Mật khẩu không chính xác
    AUTH_ERR_USER_ALREADY_EXISTS,   // Tên tài khoản đã tồn tại
    AUTH_ERR_NOT_LOGGED_IN,         // Chưa đăng nhập hệ thống
    AUTH_ERR_STORAGE_FAILED         // Lỗi khi ghi lưu trữ dữ liệu
};

// =================================================================
//  AUTH MANAGER: Module nghiệp vụ xác thực & quản lý phiên làm việc
// =================================================================
class AuthManager {
private:
    AccountStorage* storage;        // Con trỏ tới kho lưu trữ tài khoản
    Account* currentAccount;        // Con trỏ tới tài khoản đang đăng nhập hiện tại

public:
    // Khởi tạo AuthManager với kho tài khoản tương ứng
    AuthManager(AccountStorage* store);
    ~AuthManager();

    // Nghiệp vụ đăng nhập & đăng xuất
    AuthResult Login(const string& username, const string& password);
    void Logout();

    // Quản lý phiên làm việc hiện tại
    bool IsLoggedIn() const;
    Account* GetCurrentAccount() const;
    Role GetCurrentRole() const;

    // Nghiệp vụ đăng ký & đổi mật khẩu
    AuthResult Register(const string& username, const string& password, const string& fullName, Role role = User, unsigned long long initialMoney = 0);
    AuthResult ChangePassword(const string& oldPassword, const string& newPassword);
};
