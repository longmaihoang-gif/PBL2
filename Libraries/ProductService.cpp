#include "ProductService.h"

using namespace std;

// =================================================================
//  KHOI TAO & HUY DICH VU SAN PHAM
// =================================================================

ProductService::ProductService(ProductStorage* productStorage)
    : storage(productStorage) {
}

ProductService::~ProductService() {
}

// =================================================================
//  THUAT TOAN TIM KIEM VA LOC DU LIEU
// =================================================================

// Tim kiem san pham theo ten khong phan biet hoa thuong
vector<ShopItem*> ProductService::SearchByName(const string& keyword) {
    vector<ShopItem*> result;
    if (storage == nullptr) return result;

    vector<ShopItem*> allItems = storage->GetAllProducts();

    string lowerKey = "";
    int keyLen = (int)keyword.length();
    for (int i = 0; i < keyLen; i++) {
        lowerKey += (char)tolower((unsigned char)keyword[i]);
    }

    int count = (int)allItems.size();
    for (int i = 0; i < count; i++) {
        ShopItem* item = allItems[i];
        if (item == nullptr) continue;

        string lowerName = "";
        int nameLen = (int)item->Name.length();
        for (int j = 0; j < nameLen; j++) {
            lowerName += (char)tolower((unsigned char)item->Name[j]);
        }

        if (lowerName.find(lowerKey) != string::npos) {
            result.push_back(item);
        }
    }

    return result;
}

// Loc san pham theo phan loai ("Food" hoac "Electronics")
vector<ShopItem*> ProductService::GetByCategory(const string& category) {
    vector<ShopItem*> result;
    if (storage == nullptr) return result;

    vector<ShopItem*> allItems = storage->GetAllProducts();
    int count = (int)allItems.size();
    for (int i = 0; i < count; i++) {
        if (allItems[i] != nullptr && allItems[i]->Category == category) {
            result.push_back(allItems[i]);
        }
    }

    return result;
}

// Thuat toan canh bao ton kho thap duoi nguong quy dinh (mac dinh < 5)
vector<ShopItem*> ProductService::GetLowStockProducts(int threshold) {
    vector<ShopItem*> result;
    if (storage == nullptr) return result;

    vector<ShopItem*> allItems = storage->GetAllProducts();
    int count = (int)allItems.size();
    for (int i = 0; i < count; i++) {
        if (allItems[i] != nullptr && allItems[i]->QuantityRemaining < threshold) {
            result.push_back(allItems[i]);
        }
    }

    return result;
}

// So sanh ngay han dung linh hoat bang cach tim vi tri dau '-'
bool ProductService::IsDateExpiredOrNear(const string& expiryDate, const string& alertDate) {
    if (expiryDate == "-" || alertDate == "-") {
        return false;
    }

    int dash1A = (int)expiryDate.find('-');
    int dash2A = (int)expiryDate.rfind('-');
    int dash1B = (int)alertDate.find('-');
    int dash2B = (int)alertDate.rfind('-');

    if (dash1A == (int)string::npos || dash2A == (int)string::npos || dash1A == dash2A) {
        return false;
    }
    if (dash1B == (int)string::npos || dash2B == (int)string::npos || dash1B == dash2B) {
        return false;
    }

    int y1 = stoi(expiryDate.substr(0, dash1A));
    int m1 = stoi(expiryDate.substr(dash1A + 1, dash2A - dash1A - 1));
    int d1 = stoi(expiryDate.substr(dash2A + 1));

    int y2 = stoi(alertDate.substr(0, dash1B));
    int m2 = stoi(alertDate.substr(dash1B + 1, dash2B - dash1B - 1));
    int d2 = stoi(alertDate.substr(dash2B + 1));

    if (y1 != y2) return y1 < y2;
    if (m1 != m2) return m1 < m2;
    return d1 <= d2;
}

// Thuat toan canh bao thuc pham can date hoac da qua han so voi alertDate
vector<FoodItem*> ProductService::GetExpiringFoodProducts(const string& alertDate) {
    vector<FoodItem*> result;
    if (storage == nullptr) return result;

    vector<ShopItem*> allItems = storage->GetAllProducts();
    int count = (int)allItems.size();
    for (int i = 0; i < count; i++) {
        FoodItem* food = dynamic_cast<FoodItem*>(allItems[i]);
        if (food != nullptr) {
            if (IsDateExpiredOrNear(food->ExpiryDate, alertDate)) {
                result.push_back(food);
            }
        }
    }

    return result;
}

