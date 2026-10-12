#include "DataManager.h"
#include <iostream>

using namespace std;

// =================================================================
//  Boc tach mot dong CSV ho tro dau ngoac kep (RFC 4180)
// =================================================================
vector<string> DataManager::ParseCSVLine(const string &line) {
    vector<string> result;
    string token = "";
    bool insideQuotes = false;
    int len = (int)line.length();

    for (int i = 0; i < len; i++) {
        char c = line[i];

        if (c == '"') {
            // Xu ly truong hop hai dau ngoac kep lien tiep dai dien cho mot dau ngoac kep ("")
            if (insideQuotes && i + 1 < len && line[i + 1] == '"') {
                token += '"';
                i++; // Bo qua dau ngoac kep tiep theo
            } else {
                insideQuotes = !insideQuotes;
            }
        } else if (c == ',' && !insideQuotes) {
            result.push_back(token);
            token = "";
        } else if (c != '\r') { // Loai bo ky tu carriage return neu co
            token += c;
        }
    }
    result.push_back(token);
    return result;
}

// =================================================================
//  Dinh dang chuoi an toan khi ghi xuong CSV
// =================================================================
string DataManager::EscapeCSV(const string &text) {
    bool needQuotes = false;
    int len = (int)text.length();

    for (int i = 0; i < len; i++) {
        if (text[i] == ',' || text[i] == '"' || text[i] == '\n') {
            needQuotes = true;
            break;
        }
    }

    if (!needQuotes) return text;

    string escaped = "\"";
    for (int i = 0; i < len; i++) {
        if (text[i] == '"') {
            escaped += "\"\""; // Nhan doi dau ngoac kep theo chuan CSV
        } else {
            escaped += text[i];
        }
    }
    escaped += "\"";
    return escaped;
}

// =================================================================
//  CAC PHUONG THUC CO SO DOC / GHI CSV (DUNG CHUNG)
// =================================================================

// Doc toan bo file CSV thanh ma tran du lieu 2 chieu
bool DataManager::ReadCSV(const string &filePath, vector<vector<string>> &rows, bool skipHeader) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) return false;

    rows.clear();
    string line;

    // Bo qua dong tieu de neu co yeu cau
    if (skipHeader && getline(inFile, line)) {
        // Dong header duoc bo qua
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        rows.push_back(ParseCSVLine(line)); // Tai su dung ParseCSVLine
    }

    inFile.close();
    return true;
}

// Ghi toan bo ma tran 2 chieu xuong CSV (tu dong EscapeCSV tung o)
bool DataManager::WriteCSV(const string &filePath, const string &header, const vector<vector<string>> &rows) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) return false;

    if (!header.empty()) {
        outFile << header << "\n";
    }

    int rowCount = (int)rows.size();
    for (int i = 0; i < rowCount; i++) {
        const vector<string> &row = rows[i];
        int colCount = (int)row.size();
        for (int j = 0; j < colCount; j++) {
            outFile << EscapeCSV(row[j]);
            if (j + 1 < colCount) {
                outFile << ",";
            }
        }
        outFile << "\n";
    }

    outFile.close();
    return true;
}

// Ghi them (Append) 1 dong vao cuoi file CSV
bool DataManager::AppendCSVRow(const string &filePath, const string &headerIfNew, const vector<string> &row) {
    ifstream testFile(filePath);
    bool needHeader = !testFile.is_open() || testFile.peek() == ifstream::traits_type::eof();
    testFile.close();

    ofstream outFile(filePath, ios::app);
    if (!outFile.is_open()) return false;

    if (needHeader && !headerIfNew.empty()) {
        outFile << headerIfNew << "\n";
    }

    int colCount = (int)row.size();
    for (int j = 0; j < colCount; j++) {
        outFile << EscapeCSV(row[j]);
        if (j + 1 < colCount) {
            outFile << ",";
        }
    }
    outFile << "\n";

    outFile.close();
    return true;
}

// =================================================================
//  QUAN LY TAI KHOAN (ACCOUNTS)
// =================================================================

// Doc danh sach tai khoan tu CSV
bool DataManager::LoadAccounts(const string &filePath, vector<Account> &accountList) {
    vector<vector<string>> rows;
    if (!ReadCSV(filePath, rows, true)) return false;

    accountList.clear();
    int count = (int)rows.size();
    for (int i = 0; i < count; i++) {
        const vector<string> &tokens = rows[i];
        if (tokens.size() >= 6) {
            long long uid = stoll(tokens[0]);
            string username = tokens[1];
            string password = tokens[2];
            string fullName = tokens[3];
            Role role = (stoi(tokens[4]) == 0) ? Admin : User;
            unsigned long long balance = stoull(tokens[5]);

            Account acc(uid, username, password, fullName, role, balance);
            accountList.push_back(acc);
        }
    }
    return true;
}

// Ghi danh sach tai khoan xuong CSV
bool DataManager::SaveAccounts(const string &filePath, const vector<Account> &accountList) {
    string header = "UID,Username,Password,FullName,Role,Balance";
    vector<vector<string>> rows;

    int count = (int)accountList.size();
    for (int i = 0; i < count; i++) {
        const Account &acc = accountList[i];
        vector<string> row;
        row.push_back(to_string(acc.getUID()));
        row.push_back(acc.getUsername());
        row.push_back(acc.getPassword());
        row.push_back(acc.getName());
        row.push_back(acc.getPermission() == Admin ? "0" : "1");
        row.push_back(to_string(acc.getWallet().GetMoneyCount()));
        rows.push_back(row);
    }

    return WriteCSV(filePath, header, rows);
}

// Nap tai khoan vao Bang bam (HashTable)
bool DataManager::LoadAccountsToTable(const string &filePath, HashTable &accountTable, vector<Account> &accountList) {
    if (!LoadAccounts(filePath, accountList)) return false;

    int count = (int)accountList.size();
    for (int i = 0; i < count; i++) {
        accountTable.Insert(accountList[i].getUsername(), accountList[i]);
    }
    return true;
}

// =================================================================
//  QUAN LY SAN PHAM (PRODUCTS)
// =================================================================

// Doc danh muc san pham tu CSV (Ho tro da hinh FoodItem & ElectronicItem)
bool DataManager::LoadProducts(const string &filePath, vector<ShopItem*> &productList) {
    vector<vector<string>> rows;
    if (!ReadCSV(filePath, rows, true)) return false;

    // Giai phong bo nho cu neu danh sach da co phan tu
    for (int i = 0; i < (int)productList.size(); i++) {
        delete productList[i];
    }
    productList.clear();

    int count = (int)rows.size();
    for (int i = 0; i < count; i++) {
        const vector<string> &tokens = rows[i];
        if (tokens.size() >= 10) {
            string id = tokens[0];
            string name = tokens[1];
            string category = tokens[2];
            long long price = stoll(tokens[3]);
            int qty = stoi(tokens[4]);
            string desc = tokens[5];
            string nsx = tokens[6];
            string hsd = tokens[7];
            int warranty = stoi(tokens[8]);
            int power = stoi(tokens[9]);

            // Phan loai da hinh bang cap phat dong new truyen thong
            if (category == "Food" || id.rfind("FOOD-", 0) == 0) {
                productList.push_back(new FoodItem(id, name, price, qty, desc, nsx, hsd));
            } else if (category == "Electronics" || id.rfind("ELEC-", 0) == 0) {
                productList.push_back(new ElectronicItem(id, name, price, qty, desc, warranty, power));
            } else {
                productList.push_back(new ShopItem(id, name, category, price, qty, desc));
            }
        }
    }
    return true;
}

// Ghi danh muc san pham xuong CSV
bool DataManager::SaveProducts(const string &filePath, const vector<ShopItem*> &productList) {
    string header = "ProductID,ProductName,Category,Price,Quantity,Description,ManufactureDate,ExpiryDate,WarrantyMonths,PowerWatts";
    vector<vector<string>> rows;

    int count = (int)productList.size();
    for (int i = 0; i < count; i++) {
        const ShopItem* item = productList[i];
        if (item == nullptr) continue;

        vector<string> row;
        row.push_back(item->ProductID);
        row.push_back(item->Name);
        row.push_back(item->Category);
        row.push_back(to_string(item->Price));
        row.push_back(to_string(item->QuantityRemaining));
        row.push_back(item->Description);

        // Kiem tra kieu da hinh bang dynamic_cast truyen thong
        const FoodItem* food = dynamic_cast<const FoodItem*>(item);
        const ElectronicItem* elec = dynamic_cast<const ElectronicItem*>(item);

        if (food != nullptr) {
            row.push_back(food->ManufactureDate);
            row.push_back(food->ExpiryDate);
            row.push_back("0");
            row.push_back("0");
        } else if (elec != nullptr) {
            row.push_back("-");
            row.push_back("-");
            row.push_back(to_string(elec->WarrantyMonths));
            row.push_back(to_string(elec->PowerWatts));
        } else {
            row.push_back("-");
            row.push_back("-");
            row.push_back("0");
            row.push_back("0");
        }

        rows.push_back(row);
    }

    return WriteCSV(filePath, header, rows);
}

// Nap san pham vao Bang bam (HashTable)
bool DataManager::LoadProductsToTable(const string &filePath, HashTable &productTable, vector<ShopItem*> &productList) {
    if (!LoadProducts(filePath, productList)) return false;

    int count = (int)productList.size();
    for (int i = 0; i < count; i++) {
        productTable.Insert(productList[i]->ProductID, productList[i]);
    }
    return true;
}

// =================================================================
//  QUAN LY HOA DON (ORDERS)
// =================================================================

// Doc lich su hoa don tu CSV
bool DataManager::LoadOrders(const string &filePath, vector<OrderRecord> &orderList) {
    vector<vector<string>> rows;
    if (!ReadCSV(filePath, rows, true)) return false;

    orderList.clear();
    int count = (int)rows.size();
    for (int i = 0; i < count; i++) {
        const vector<string> &tokens = rows[i];
        if (tokens.size() >= 6) {
            string id = tokens[0];
            long long uid = stoll(tokens[1]);
            string name = tokens[2];
            string date = tokens[3];
            unsigned long long amount = stoull(tokens[4]);
            string summary = tokens[5];

            orderList.push_back(OrderRecord(id, uid, name, date, amount, summary));
        }
    }
    return true;
}

// Ghi them (Append) mot don hang moi vao CSV
bool DataManager::AppendOrder(const string &filePath, const OrderRecord &order) {
    string header = "OrderID,UID,CustomerName,OrderDate,TotalAmount,ItemsSummary";
    vector<string> row;
    row.push_back(order.OrderID);
    row.push_back(to_string(order.UID));
    row.push_back(order.CustomerName);
    row.push_back(order.OrderDate);
    row.push_back(to_string(order.TotalAmount));
    row.push_back(order.ItemsSummary);

    return AppendCSVRow(filePath, header, row);
}
