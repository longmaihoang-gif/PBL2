#pragma once
#include <string>
#include <vector>
#include "Base.h"
#include "Item.h"

using namespace std;

// =================================================================
//  SHOP SHELF: Ke hang / Quay trung bay vat pham (Ke thua Entity)
// =================================================================
class ShopShelf : public Entity {
    public:
        vector<ShopItem> ShelfItems;

        ShopShelf() { Name = "Ke Hang"; }
        virtual ~ShopShelf() {}

        virtual string GetInfo() const override {
            return "[Ke hang] So loai mat hang trung bay: " + to_string(ShelfItems.size());
        }
};

// =================================================================
//  SHOP: Cua hang tong the chua cac quay ke (Ke thua Entity)
// =================================================================
class Shop : public Entity {
    public:
        vector<ShopShelf> Shelfs;

        Shop() { Name = "Cua Hang"; }
        virtual ~Shop() {}

        virtual string GetInfo() const override {
            return "[Cua hang] So quay ke: " + to_string(Shelfs.size());
        }
};
