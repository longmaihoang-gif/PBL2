#pragma once
#include <string>
#include <vector>
#include "Base.h"

using namespace std;

// =================================================================
// 📦 ITEM: Cấu trúc thông tin vật phẩm đơn lẻ
// =================================================================
struct Item {
    string ItemID;
    int Count;

    Item(string id = "", int count = 0) : ItemID(id), Count(count) {}
    ~Item() {}
};

// =================================================================
// 🛒 SHOP ITEM: Món hàng được bày bán trong Shop (Kế thừa Entity)
// =================================================================
class ShopItem : public Entity {
    public:
        vector<Item> Items;
        long long Price;
        int QuantityRemaining;
        string Description;

        ShopItem();
        virtual ~ShopItem();

        void AddItem(string itemID, int count);
};

// =================================================================
// 🏪 SHOP SHELF: Kệ hàng / Quầy trưng bày vật phẩm (Kế thừa Entity)
// =================================================================
class ShopShelf : public Entity {
    public:
        vector<ShopItem> ShelfItems;

        ShopShelf() {}
        virtual ~ShopShelf() {}
};

class Shop : public Entity {
    public:
        vector<ShopShelf> Shelfs;

        Shop() {}
        virtual ~Shop() {}
};

