#pragma once
#include <string>
#include <vector>
#include "DataManager.h"

// =================================================================
//  ORDER STORAGE: He thong trung gian quan ly lich su don hang (Tang 2: Data Access Layer)
//  - Quan ly danh sach don hang tren RAM
//  - Dong bo doc va ghi truc tiep voi tep Orders.csv
// =================================================================
class OrderStorage {
private:
    std::vector<OrderRecord> orderList;   // Danh sach toan bo don hang luu tren bo nho RAM
    std::string dataFilePath;             // Duong dan tep CSV luu tru don hang
    bool isLoadedSuccessfully;            // Trang thai nap du lieu ban dau

    // Ngan chan sao chep ngam gay loi bo nho
    OrderStorage(const OrderStorage&);
    OrderStorage& operator=(const OrderStorage&);

public:
    // Khoi tao va tu dong nap du lieu tu tep CSV
    OrderStorage(const std::string& filePath = "Data/Orders.csv");
    ~OrderStorage();

    // Nap danh sach don hang tu file CSV len RAM
    bool LoadFromFile();

    // Luu toan bo danh sach don hang tu RAM xuong file CSV
    bool SaveToFile();

    // Kiem tra trang thai nap file thanh cong
    bool IsLoaded() const { return isLoadedSuccessfully; }

    // Them mot don hang moi vao RAM va ghi noi tiep vao file CSV (Write-Through)
    bool AddOrder(const OrderRecord& order);

    // Lay so luong don hang hien co
    int GetOrderCount() const { return (int)orderList.size(); }

    // Lay toan bo danh sach don hang
    std::vector<OrderRecord> GetAllOrders() const { return orderList; }

    // Tim kiem don hang theo ma don hang
    const OrderRecord* GetOrderByID(const std::string& orderID) const;

    // Lay duong dan tep CSV
    std::string GetDataFilePath() const { return dataFilePath; }
};

