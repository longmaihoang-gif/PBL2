#pragma once
#include <string>
#include <vector>
#include "Base.h"
#include "Trade.h"

using namespace std;

// =================================================================
//  ROLE: Phân quyền tài khoản hệ thống
// =================================================================
enum Role {
    Admin = 0,
    User = 1
};

// =================================================================
//  ACCOUNT: Thực thể tài khoản người dùng / quản trị viên
// =================================================================
class Account : public Entity {
    private:
        inline static long long GUID = 1000;
        long long UID;
        string Username;
        string Password;
        Role Permission;
        Wallet UserWallet; // Vi tien giao dich

    public:
        // Constructor tao tai khoan moi (tu dong cap UID tang dan)
        Account(const string &username = "", const string &password = "", const string &fullName = "", Role role = User, unsigned long long initialMoney = 0)
            : UID(++GUID), Username(username), Password(password), Permission(role), UserWallet(initialMoney) {
            Name = fullName;
        }

        // Constructor nap tai khoan tu File/Database (UID da co san)
        Account(long long fixedUID, const string &username, const string &password, const string &fullName, Role role = User, unsigned long long initialMoney = 0)
            : UID(fixedUID), Username(username), Password(password), Permission(role), UserWallet(initialMoney) {
            Name = fullName;
            if (fixedUID >= GUID) GUID = fixedUID; // Dong bo moc GUID ke tiep
        }

        virtual ~Account() {}

        // Getter
        long long getUID() const { return UID; }
        string getUsername() const { return Username; }
        string getPassword() const { return Password; }
        string getName() const { return Name; }
        Role getPermission() const { return Permission; }
        Wallet& getWallet() { return UserWallet; }

        // Setter
        void setUsername(const string &username) { Username = username; }
        void setPassword(const string &password) { Password = password; }
        void setName(const string &name) { Name = name; }
        void setPermission(Role role) { Permission = role; }

        // Phuong thuc xac thuc mat khau dang nhap
        bool VerifyPassword(const string &inputPassword) const {
            return Password == inputPassword;
        }
};
