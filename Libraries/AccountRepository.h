#pragma once
#include <string>
#include <vector>
#include "Account.h"
#include "HashTable.h"

using namespace std;

// =================================================================
//  ACCOUNT REPOSITORY: Hệ thống trung gian quản lý dữ liệu tài khoản
//  - Đóng vai trò cầu nối duy nhất giữa Bảng băm (RAM) và Tệp CSV (Disk)
//  - Áp dụng cơ chế Write-Through đảm bảo không xung đột dữ liệu
// =================================================================
class AccountRepository {
private:
    HashTable accountTable;         // Bảng băm lưu trữ tài khoản trên RAM (Tra cứu O(1))
    string dataFilePath;            // Đường dẫn tệp CSV dữ liệu tài khoản
    bool isLoadedSuccessfully;      // Trạng thái nạp dữ liệu ban đầu

    // Nạp dữ liệu từ tệp CSV vào bảng băm RAM
    bool LoadFromDisk();

    // Đồng bộ toàn bộ dữ liệu từ bảng băm xuống tệp CSV
    bool SyncToDisk();

public:
    // Khởi tạo và tự động nạp dữ liệu từ tệp tin
    AccountRepository(const string& filePath = "Data/Accounts.csv");
    ~AccountRepository();

    // Kiểm tra trạng thái nạp dữ liệu từ tệp
    bool IsLoaded() const { return isLoadedSuccessfully; }

    // Tra cứu tài khoản theo tên đăng nhập (Độ phức tạp O(1))
    Account* GetByUsername(const string& username);

    // Kiểm tra tài khoản có tồn tại trong hệ thống hay không
    bool Exists(const string& username);

    // Thêm tài khoản mới (Ghi đồng thời vào RAM và tệp CSV)
    bool AddAccount(const Account& acc);

    // Cập nhật mật khẩu cho tài khoản và đồng bộ xuống tệp CSV
    bool UpdatePassword(const string& username, const string& newPassword);

    // Cập nhật toàn bộ thông tin tài khoản và đồng bộ xuống tệp CSV
    bool UpdateAccount(const Account& acc);

    // Xóa tài khoản khỏi hệ thống và đồng bộ xuống tệp CSV
    bool RemoveAccount(const string& username);

    // Trích xuất toàn bộ danh sách tài khoản phục vụ hiển thị bảng
    vector<Account> GetAllAccounts();
};
