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

        ShopShelf() {}
        virtual ~ShopShelf() {}
};

// =================================================================
//  SHOP: Cua hang tong the chua cac quay ke (Ke thua Entity)
// =================================================================
class Shop : public Entity {
    public:
        vector<ShopShelf> Shelfs;

        Shop() {}
        virtual ~Shop() {}
};
