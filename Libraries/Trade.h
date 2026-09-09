#pragma once
#include "Base.h"

// =================================================================
// 💰 WALLET: Quản lý ví tiền & số dư giao dịch
// =================================================================
class Wallet : public Entity {
    private:
        unsigned long long Balance;
        unsigned long long Point;

    public:
        Wallet(unsigned long long initialBalance = 0) : Balance(initialBalance) {}
        virtual ~Wallet() {}

        // Truy xuất số lượng tiền (Get Money Count)
        unsigned long long GetMoneyCount() const { return Balance; }
        unsigned long long getBalance() const { return Balance; }

        // Nạp tiền vào ví (Add Money)
        void AddMoney(const unsigned long long &amount) {
            Balance += amount;
        }

        // Tiêu trừ tiền từ ví (Cost Money - trả về false nếu không đủ tiền)
        bool CostMoney(const unsigned long long &amount) {
            if (Balance >= amount) {
                Balance -= amount;
                return true;
            }
            return false;
        }

        // Chuyển tiền sang ví mục tiêu (Transfer Money)
        bool TransferMoney(Wallet &target, const unsigned long long &amount) {
            if (this == &target) return true;
            if (CostMoney(amount)) {
                target.AddMoney(amount);
                return true;
            }
            return false;
        }

        // Hỗ trợ alias Transfer ngắn gọn
        bool Transfer(Wallet &target, const unsigned long long &amount) {
            return TransferMoney(target, amount);
        }
};
