#pragma once
#include <string>
#include <vector>
#include "Base.h"
#include "Trade.h"

using namespace std;

// =================================================================
// 🛡️ ROLE: Phân quyền tài khoản hệ thống
// =================================================================
enum Role {
    Admin = 0,
    User = 1
};

// =================================================================
// 👤 ACCOUNT: Thực thể tài khoản người dùng / quản trị viên
// =================================================================
class Account : public Entity {
    private:
        inline static long long GUID = 1000;
        long long UID;
        Role Permission;
        Wallet UserWallet; // 💰 Ví tiền giao dịch

    public:
        // Constructor tạo tài khoản mới (tự động cấp UID tăng dần)
        Account(const string &name = "", Role role = User, unsigned long long initialMoney = 0)
            : UID(++GUID), Permission(role), UserWallet(initialMoney) {
            Name = name;
        }

        // Constructor nạp tài khoản từ File/Database (UID đã có sẵn)
        Account(long long fixedUID, const string &name, Role role = User, unsigned long long initialMoney = 0)
            : UID(fixedUID), Permission(role), UserWallet(initialMoney) {
            Name = name;
            if (fixedUID >= GUID) GUID = fixedUID; // Đồng bộ mốc GUID kế tiếp
        }

        virtual ~Account() {}

        // Getter
        long long getUID() const { return UID; }
        string getName() const { return Name; }
        Role getPermission() const { return Permission; }
        Wallet& getWallet() { return UserWallet; }

        // Setter
        void setName(const string &name) { Name = name; }
        void setPermission(Role role) { Permission = role; }
};
