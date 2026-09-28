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
//  Doc danh sach tai khoan tu CSV
// =================================================================
bool DataManager::LoadAccounts(const string &filePath, vector<Account> &accountList) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) return false;

    accountList.clear();
    string line;

    // Doc dong tieu de (Header)
    if (!getline(inFile, line)) {
        inFile.close();
        return false;
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        vector<string> tokens = ParseCSVLine(line);
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

    inFile.close();
    return true;
}

// =================================================================
//  Ghi danh sach tai khoan xuong CSV
// =================================================================
bool DataManager::SaveAccounts(const string &filePath, const vector<Account> &accountList) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) return false;

    // Ghi dong tieu de
    outFile << "UID,Username,Password,FullName,Role,Balance\n";

    int count = (int)accountList.size();
    for (int i = 0; i < count; i++) {
        const Account &acc = accountList[i];
        outFile << acc.getUID() << ","
                << EscapeCSV(acc.getUsername()) << ","
                << EscapeCSV(acc.getPassword()) << ","
                << EscapeCSV(acc.getName()) << ","
                << (acc.getPermission() == Admin ? 0 : 1) << ","
                << acc.getWallet().GetMoneyCount() << "\n";
    }

    outFile.close();
    return true;
}

// =================================================================
//  Nap tai khoan vao Bang bam (HashTable)
// =================================================================
bool DataManager::LoadAccountsToTable(const string &filePath, HashTable &accountTable, vector<Account> &accountList) {
    if (!LoadAccounts(filePath, accountList)) return false;

    int count = (int)accountList.size();
    for (int i = 0; i < count; i++) {
        // Luu vao bang bam tra cuu theo Username
        accountTable.Insert(accountList[i].getUsername(), accountList[i]);
    }

    return true;
}

// =================================================================
//  Doc danh muc san pham tu CSV (Ho tro da hinh FoodItem & ElectronicItem)
// =================================================================
bool DataManager::LoadProducts(const string &filePath, vector<shared_ptr<ShopItem>> &productList) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) return false;

    productList.clear();
    string line;

    // Doc dong tieu de (Header)
    if (!getline(inFile, line)) {
        inFile.close();
        return false;
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        vector<string> tokens = ParseCSVLine(line);
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

            // Phan loai da hinh dua tren category hoac tien to ma dinh danh
            if (category == "Food" || id.rfind("FOOD-", 0) == 0) {
                productList.push_back(make_shared<FoodItem>(id, name, price, qty, desc, nsx, hsd));
            } else if (category == "Electronics" || id.rfind("ELEC-", 0) == 0) {
                productList.push_back(make_shared<ElectronicItem>(id, name, price, qty, desc, warranty, power));
            } else {
                productList.push_back(make_shared<ShopItem>(id, name, category, price, qty, desc));
            }
        }
    }

    inFile.close();
    return true;
}

// =================================================================
//  Ghi danh muc san pham xuong CSV
// =================================================================
bool DataManager::SaveProducts(const string &filePath, const vector<shared_ptr<ShopItem>> &productList) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) return false;

    // Ghi dong tieu de
    outFile << "ProductID,ProductName,Category,Price,Quantity,Description,ManufactureDate,ExpiryDate,WarrantyMonths,PowerWatts\n";

    int count = (int)productList.size();
    for (int i = 0; i < count; i++) {
        const shared_ptr<ShopItem> &item = productList[i];
        if (!item) continue;

        outFile << EscapeCSV(item->ProductID) << ","
                << EscapeCSV(item->Name) << ","
                << EscapeCSV(item->Category) << ","
                << item->Price << ","
                << item->QuantityRemaining << ","
                << EscapeCSV(item->Description) << ",";

        // Kiem tra kieu da hinh de ghi cac thuoc tinh dac thu
        shared_ptr<FoodItem> food = dynamic_pointer_cast<FoodItem>(item);
        shared_ptr<ElectronicItem> elec = dynamic_pointer_cast<ElectronicItem>(item);

        if (food) {
            outFile << EscapeCSV(food->ManufactureDate) << ","
                    << EscapeCSV(food->ExpiryDate) << ",0,0\n";
        } else if (elec) {
            outFile << "-,-," << elec->WarrantyMonths << "," << elec->PowerWatts << "\n";
        } else {
            outFile << "-,-,0,0\n";
        }
    }

    outFile.close();
    return true;
}

// =================================================================
//  Nap san pham vao Bang bam (HashTable)
// =================================================================
bool DataManager::LoadProductsToTable(const string &filePath, HashTable &productTable, vector<shared_ptr<ShopItem>> &productList) {
    if (!LoadProducts(filePath, productList)) return false;

    int count = (int)productList.size();
    for (int i = 0; i < count; i++) {
        productTable.Insert(productList[i]->ProductID, productList[i]);
    }

    return true;
}

// =================================================================
//  Doc lich su hoa don tu CSV
// =================================================================
bool DataManager::LoadOrders(const string &filePath, vector<OrderRecord> &orderList) {
    ifstream inFile(filePath);
    if (!inFile.is_open()) return false;

    orderList.clear();
    string line;

    // Doc dong tieu de
    if (!getline(inFile, line)) {
        inFile.close();
        return false;
    }

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        vector<string> tokens = ParseCSVLine(line);
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

    inFile.close();
    return true;
}

// =================================================================
//  Ghi them (Append) mot don hang moi vao CSV
// =================================================================
bool DataManager::AppendOrder(const string &filePath, const OrderRecord &order) {
    // Kiem tra neu tep chua ton tai thi tao moi kem header
    ifstream testFile(filePath);
    bool needHeader = !testFile.is_open() || testFile.peek() == ifstream::traits_type::eof();
    testFile.close();

    ofstream outFile(filePath, ios::app);
    if (!outFile.is_open()) return false;

    if (needHeader) {
        outFile << "OrderID,UID,CustomerName,OrderDate,TotalAmount,ItemsSummary\n";
    }

    outFile << EscapeCSV(order.OrderID) << ","
            << order.UID << ","
            << EscapeCSV(order.CustomerName) << ","
            << EscapeCSV(order.OrderDate) << ","
            << order.TotalAmount << ","
            << EscapeCSV(order.ItemsSummary) << "\n";

    outFile.close();
    return true;
}
