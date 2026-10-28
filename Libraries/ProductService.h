#pragma once
#include <string>
#include <vector>
#include "Item.h"
#include "ProductStorage.h"

// =================================================================
//  PRODUCT SERVICE: Tang xu ly thuat toan & nghiep vu hang hoa (Tang 3)
//  - Thuat toan tim kiem chuoi con khong phan biet hoa thuong
//  - Thuat toan loc san pham theo phan loai
//  - Thuat toan canh bao hang ton kho thap (< 5)
//  - Thuat toan canh bao thuc pham can date / qua han su dung
// =================================================================
class ProductService {
private:
    ProductStorage* storage;

public:
    ProductService(ProductStorage* productStorage = nullptr);
    ~ProductService();

    // Gan kho luu tru san pham (Dependency Injection)
    void SetStorage(ProductStorage* productStorage) { storage = productStorage; }

    // Thuat toan tim kiem san pham theo ten (khong phan biet hoa thuong)
    std::vector<ShopItem*> SearchByName(const std::string& keyword);

    // Thuat toan loc san pham theo phan loai ("Food" hoac "Electronics")
    std::vector<ShopItem*> GetByCategory(const std::string& category);

    // Thuat toan canh bao ton kho thap duoi nguong quy dinh (mac dinh < 5)
    std::vector<ShopItem*> GetLowStockProducts(int threshold = 5);

    // Thuat toan canh bao thuc pham can date hoac qua han
    std::vector<FoodItem*> GetExpiringFoodProducts(const std::string& alertDate);

    // Ham bo tro so sanh ngay han dung dinh dang YYYY-MM-DD
    static bool IsDateExpiredOrNear(const std::string& expiryDate, const std::string& alertDate);
};

