#pragma once
#include <string>
#include <vector>
#include "Base.h"

using namespace std;

// =================================================================
//  ITEM: Cau truc thong tin vat pham don le
// =================================================================
struct Item {
    string ItemID;
    int Count;

    Item(string id = "", int count = 0) : ItemID(id), Count(count) {}
    ~Item() {}
};

// =================================================================
//  SHOP ITEM: Mon hang tong quat trong Cua hang (Ke thua Entity)
// =================================================================
class ShopItem : public Entity {
    public:
        string ProductID;
        string Category;
        long long Price;
        int QuantityRemaining;
        string Description;

        ShopItem(string id = "", string name = "", string category = "", long long price = 0, int qty = 0, string desc = "")
            : ProductID(id), Category(category), Price(price), QuantityRemaining(qty), Description(desc) {
            Name = name;
        }

        virtual ~ShopItem() {}

        // Thong tin tong quan mon hang (Ghi de Entity)
        virtual string GetInfo() const override {
            return "[San pham] Ma: " + ProductID + " | Ten: " + Name + " | Loai: " + Category + " | Gia: " + to_string(Price) + " VND | Ton: " + to_string(QuantityRemaining);
        }
};

// =================================================================
//  FOOD ITEM: Thuc pham voi han dung va ngay san xuat (Ke thua ShopItem)
// =================================================================
class FoodItem : public ShopItem {
    public:
        string ManufactureDate;
        string ExpiryDate;

        FoodItem(string id = "", string name = "", long long price = 0, int qty = 0, string desc = "", string nsx = "", string hsd = "")
            : ShopItem(id, name, "Food", price, qty, desc), ManufactureDate(nsx), ExpiryDate(hsd) {}

        virtual ~FoodItem() {}

        // Thong tin tong quan thuc pham (Ghi de da hinh)
        virtual string GetInfo() const override {
            return ShopItem::GetInfo() + " | NSX: " + ManufactureDate + " | HSD: " + ExpiryDate;
        }
};

// =================================================================
//  ELECTRONIC ITEM: Do dien may voi bao hanh va cong suat (Ke thua ShopItem)
// =================================================================
class ElectronicItem : public ShopItem {
    public:
        int WarrantyMonths;
        int PowerWatts;

        ElectronicItem(string id = "", string name = "", long long price = 0, int qty = 0, string desc = "", int warranty = 0, int power = 0)
            : ShopItem(id, name, "Electronics", price, qty, desc), WarrantyMonths(warranty), PowerWatts(power) {}

        virtual ~ElectronicItem() {}

        // Thong tin tong quan dien may (Ghi de da hinh)
        virtual string GetInfo() const override {
            return ShopItem::GetInfo() + " | Bao hanh: " + to_string(WarrantyMonths) + " thang | Cong suat: " + to_string(PowerWatts) + "W";
        }
};
