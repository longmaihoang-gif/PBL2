#pragma once
#include <string>
#include <vector>
#include "Item.h"
#include "HashTable.h"

using namespace std;

// =================================================================
//  PRODUCT STORAGE: He thong trung gian quan ly kho hang (Repository)
//  - Dong vai tro cau noi duy nhat giua Bang bam (RAM) va Tep CSV (Disk)
//  - Ap dung co che Write-Through dam bao dong bo du lieu tuc thi
// =================================================================
class ProductStorage {
private:
    HashTable<ShopItem*> productTable;  // Bang bam luu tru con tro san pham tren RAM (Tra cuu O(1))
    string dataFilePath;                // Duong dan tep CSV du lieu san pham
    bool isLoadedSuccessfully;          // Trang thai nap du lieu ban dau

    // Nap du lieu tu tep CSV vao bang bam RAM
    bool LoadFromFile();

    // Dong bo toan bo du lieu tu bang bam xuong tep CSV
    bool SaveToFile();

    // Ham bo tro so sanh ngay han dung (yyyy-mm-dd) dung substr va stoi
    static bool IsDateExpiredOrNear(const string& expiryDate, const string& alertDate);

    // Ngan chan sao chep ngam gay loi double-free
    ProductStorage(const ProductStorage&);
    ProductStorage& operator=(const ProductStorage&);

public:
    // Khoi tao va tu dong nap du lieu tu tep tin
    ProductStorage(const string& filePath = "Data/Products.csv");
    ~ProductStorage();

    // Kiem tra trang thai nap du lieu tu tep
    bool IsLoaded() const { return isLoadedSuccessfully; }

    // Tra cuu san pham theo ma san pham (Do phuc tap O(1))
    ShopItem* GetByID(const string& productID);

    // Kiem tra san pham co ton tai trong he thong hay khong
    bool Exists(const string& productID);

    // Them san pham moi (Ghi dong thoi vao RAM va tep CSV)
    bool AddProduct(ShopItem* newItem);

    // Cap nhat toan bo thong tin san pham va dong bo xuong CSV
    bool UpdateProduct(ShopItem* updatedItem);

    // Cap nhat so luong ton kho va dong bo xuong CSV
    bool UpdateQuantity(const string& productID, int newQuantity);

    // Cap nhat gia ban va dong bo xuong CSV
    bool UpdatePrice(const string& productID, long long newPrice);

    // Xoa san pham khoi he thong, thu hoi bo nho va dong bo xuong CSV
    bool RemoveProduct(const string& productID);

    // Trich xuat toan bo danh sach san pham
    vector<ShopItem*> GetAllProducts();

    // Tim kiem san pham theo ten (khong phan biet hoa thuong co ban)
    vector<ShopItem*> SearchByName(const string& keyword);

    // Loc san pham theo phan loai ("Food" hoac "Electronics")
    vector<ShopItem*> GetByCategory(const string& category);

    // Loc danh sach san pham co ton kho duoi nguong canh bao (mac dinh < 5)
    vector<ShopItem*> GetLowStockProducts(int threshold = 5);

    // Loc danh sach thuc pham can date hoac da qua han so voi alertDate
    vector<FoodItem*> GetExpiringFoodProducts(const string& alertDate);

    // Lay tham chieu toi Bang bam de ket noi voi gio hang (Cart Undo/Clear)
    HashTable<ShopItem*>& GetTableRef() { return productTable; }
};
