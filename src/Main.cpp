#include <iostream>
#include <string>
#include <vector>
#include <any>

#ifdef _WIN32
#include <windows.h>
#endif

// =================================================================
//  THU VIEN DU AN PBL2
// =================================================================
#include "../Libraries/Base.h"
#include "../Libraries/Trade.h"
#include "../Libraries/Account.h"
#include "../Libraries/Item.h"
#include "../Libraries/Shop.h"
#include "../Libraries/HashTable.h"
#include "../Libraries/DLinkedList.h"
#include "../Libraries/DataManager.h"
#include "../Libraries/Cart.h"
#include "../Libraries/AccountRepository.h"
#include "../Libraries/AuthManager.h"

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    cout << "============================================================\n";
    cout << "   HE THONG QUAN LY CUA HANG & BAN HANG (PBL2)\n";
    cout << "============================================================\n\n";

    // 1. Khoi tao cac cau truc du lieu trong bo nho
    HashTable accountTable;
    HashTable productTable;
    vector<Account> accountList;
    vector<ShopItem*> productList;
    vector<OrderRecord> orderList;

    // 2. Nap du lieu tu cac tep CSV
    cout << "[INFO] Dang nap du lieu tu thu muc Data/...\n";

    if (DataManager::LoadAccountsToTable("Data/Accounts.csv", accountTable, accountList)) {
        cout << "  -> Nap thanh cong " << accountList.size() << " tai khoan vao HashTable.\n";
    } else {
        cout << "  -> [Canh bao] Khong the mo hoac nap Data/Accounts.csv\n";
    }

    if (DataManager::LoadProductsToTable("Data/Products.csv", productTable, productList)) {
        cout << "  -> Nap thanh cong " << productList.size() << " san pham vao HashTable.\n";
    } else {
        cout << "  -> [Canh bao] Khong the mo hoac nap Data/Products.csv\n";
    }

    if (DataManager::LoadOrders("Data/Orders.csv", orderList)) {
        cout << "  -> Nap thanh cong " << orderList.size() << " don hang tu lich su.\n";
    } else {
        cout << "  -> [Canh bao] Khong the mo hoac nap Data/Orders.csv\n";
    }

    // 3. Kiem thu Module Gio hang (Cart) & Co che Hoan tac (Undo LIFO)
    cout << "\n============================================================\n";
    cout << "   KIEM THU MODULE GIO HANG & CO CHE HOAN TAC (UNDO)\n";
    cout << "============================================================\n";

    Cart myCart("Nguyen Van A");

    if (productList.size() >= 4) {
        cout << "\n[BUOC 1] Thuc hien them san pham vao gio hang:\n";
        cout << "  - Ton kho ban dau " << productList[0]->Name << ": " << productList[0]->QuantityRemaining << "\n";
        cout << "  - Ton kho ban dau " << productList[1]->Name << ": " << productList[1]->QuantityRemaining << "\n";
        cout << "  - Ton kho ban dau " << productList[3]->Name << ": " << productList[3]->QuantityRemaining << "\n";

        // Them san pham hop le
        myCart.AddItem(productList[0], 2); // 2 Banh mi
        myCart.AddItem(productList[1], 3); // 3 Coca Cola
        myCart.AddItem(productList[3], 1); // 1 Chuot Logitech

        cout << "\n  -> Da them 3 mat hang vao gio hang!\n";
        cout << "  - Ton kho sau khi them:\n";
        cout << "    + " << productList[0]->Name << ": " << productList[0]->QuantityRemaining << "\n";
        cout << "    + " << productList[1]->Name << ": " << productList[1]->QuantityRemaining << "\n";
        cout << "    + " << productList[3]->Name << ": " << productList[3]->QuantityRemaining << "\n";

        // Thu them vuot qua so luong ton kho
        cout << "\n  [Kiem tra vuot ton kho] Thu them 999 Chuot Logitech...\n";
        bool overLimitResult = myCart.AddItem(productList[3], 999);
        cout << "  -> Ket qua them 999 mon: " << (overLimitResult ? "THANH CONG" : "THAT BAI (Dung nhu ky vong)") << "\n";

        // In gio hang hien tai
        myCart.PrintCart();

        // [BUOC 2] Thuc hien Undo lan 1
        cout << "\n[BUOC 2] Thuc hien Undo thao tac them gan nhat (LIFO):\n";
        CartItem undoneItem;
        if (myCart.Undo(productTable, undoneItem)) {
            cout << "  -> [UNDO THANH CONG] Da rut khoi gio: [" << undoneItem.ProductID 
                 << "] " << undoneItem.ProductName << " | So luong hoan tra: " << undoneItem.Quantity << "\n";
            cout << "  -> Ton kho san pham [" << undoneItem.ProductID << "] sau khi Undo: " 
                 << productList[3]->QuantityRemaining << " (Da phuc hoi nguyen trang)\n";
        }
        myCart.PrintCart();

        // [BUOC 3] Thuc hien Undo lan 2
        cout << "\n[BUOC 3] Thuc hien Undo tiep mot lan nua:\n";
        if (myCart.Undo(productTable, undoneItem)) {
            cout << "  -> [UNDO THANH CONG] Da rut khoi gio: [" << undoneItem.ProductID 
                 << "] " << undoneItem.ProductName << " | So luong hoan tra: " << undoneItem.Quantity << "\n";
            cout << "  -> Ton kho san pham [" << undoneItem.ProductID << "] sau khi Undo: " 
                 << productList[1]->QuantityRemaining << " (Da phuc hoi nguyen trang)\n";
        }
        myCart.PrintCart();

        // [BUOC 4] Them mat hang moi sau khi Undo de kiem tra tinh toan ven lien ket
        cout << "\n[BUOC 4] Them mat hang moi sau cac thao tac Undo:\n";
        myCart.AddItem(productList[2], 1); // 1 Sua tuoi Vinamilk
        myCart.PrintCart();

        cout << "Chuoi tom tat don hang de ghi CSV:\n\"" << myCart.GetItemsSummary() << "\"\n";

        // [BUOC 5] Huy gio hang va phuc hoi toan bo ton kho
        cout << "\n[BUOC 5] Thu nghiem huy gio hang (Clear va hoan tra toan bo ton kho):\n";
        myCart.Clear(&productTable);
        cout << "  -> Gio hang sau khi Clear: " << (myCart.IsEmpty() ? "RONG (Dung ky vong)" : "CHUA RONG") << "\n";
        cout << "  -> Ton kho " << productList[0]->Name << " sau khi Clear: " << productList[0]->QuantityRemaining << " (Da hoan tra du)\n";
    }

    // 4. Thu hoi bo nho dong cho danh muc san pham tren Heap
    for (int i = 0; i < (int)productList.size(); i++) {
        delete productList[i];
    }
    productList.clear();
    // 5. Kiem thu Module AccountRepository & AuthManager
    cout << "\n============================================================\n";
    cout << "   KIEM THU MODULE XAC THUC & QUAN LY PHIEN (AUTH MANAGER)\n";
    cout << "============================================================\n";

    AccountRepository accountRepo("Data/Accounts.csv");
    cout << "[KHOI TAO] AccountRepository: " << (accountRepo.IsLoaded() ? "THANH CONG" : "THAT BAI") << "\n";

    AuthManager auth(&accountRepo);

    // Kiem thu dang nhap rong
    AuthResult resEmpty = auth.Login("", "");
    cout << "  1. Kiem tra nhap rong: " << (resEmpty == AUTH_ERR_EMPTY_INPUT ? "DUNG (AUTH_ERR_EMPTY_INPUT)" : "SAI") << "\n";

    // Kiem thu tai khoan khong ton tai
    AuthResult resNotFound = auth.Login("khong_ton_tai_123", "123");
    cout << "  2. Kiem tra tai khoan khong ton tai: " << (resNotFound == AUTH_ERR_USER_NOT_FOUND ? "DUNG (AUTH_ERR_USER_NOT_FOUND)" : "SAI") << "\n";

    // Kiem thu sai mat khau
    AuthResult resWrongPass = auth.Login("admin", "sai_mat_khau");
    cout << "  3. Kiem tra sai mat khau: " << (resWrongPass == AUTH_ERR_WRONG_PASSWORD ? "DUNG (AUTH_ERR_WRONG_PASSWORD)" : "SAI") << "\n";

    // Kiem thu dang nhap hop le (Admin)
    AuthResult resAdmin = auth.Login("admin", "admin123");
    cout << "  4. Dang nhap Admin ('admin'): " << (resAdmin == AUTH_SUCCESS ? "THANH CONG" : "THAT BAI") << "\n";
    if (auth.IsLoggedIn()) {
        cout << "     -> Ten nguoi dung: " << auth.GetCurrentAccount()->getName() << "\n";
        cout << "     -> Phan quyen: " << (auth.GetCurrentRole() == Admin ? "Admin (Chinh xac)" : "User") << "\n";
    }

    // Kiem thu dang xuat
    auth.Logout();
    cout << "  5. Kiem tra dang xuat: " << (!auth.IsLoggedIn() ? "THANH CONG (Da xoa phien)" : "THAT BAI") << "\n";

    // Kiem thu dang ky tai khoan moi
    string testUser = "test_member";
    if (accountRepo.Exists(testUser)) {
        accountRepo.RemoveAccount(testUser);
    }
    AuthResult resReg = auth.Register(testUser, "matkhau123", "Nguyen Van Test", User, 350000);
    cout << "  6. Dang ky tai khoan moi ('" << testUser << "'): " << (resReg == AUTH_SUCCESS ? "THANH CONG" : "THAT BAI") << "\n";

    // Kiem thu dang ky trung ten
    AuthResult resDuplicate = auth.Register(testUser, "123", "Test Duplicate", User, 0);
    cout << "  7. Kiem tra dang ky trung ten: " << (resDuplicate == AUTH_ERR_USER_ALREADY_EXISTS ? "DUNG (AUTH_ERR_USER_ALREADY_EXISTS)" : "SAI") << "\n";

    // Dang nhap voi tai khoan vua tao
    AuthResult resLoginNew = auth.Login(testUser, "matkhau123");
    cout << "  8. Dang nhap tai khoan moi: " << (resLoginNew == AUTH_SUCCESS ? "THANH CONG" : "THAT BAI") << "\n";
    if (auth.IsLoggedIn()) {
        cout << "     -> So du vi: " << auth.GetCurrentAccount()->getBalance() << " VND (Dung 350000)\n";
    }

    // Kiem thu doi mat khau
    AuthResult resChangePass = auth.ChangePassword("matkhau123", "matkhau_moi_456");
    cout << "  9. Doi mat khau: " << (resChangePass == AUTH_SUCCESS ? "THANH CONG" : "THAT BAI") << "\n";

    auth.Logout();

    // Kiem thu dang nhap voi mat khau moi
    AuthResult resReLogin = auth.Login(testUser, "matkhau_moi_456");
    cout << "  10. Dang nhap lai voi mat khau moi: " << (resReLogin == AUTH_SUCCESS ? "THANH CONG" : "THAT BAI") << "\n";

    // Kiem tra tinh ben vung tu o dia (Khoi tao AccountRepository doc lap de kiem tra)
    AccountRepository diskChecker("Data/Accounts.csv");
    Account* checkAcc = diskChecker.GetByUsername(testUser);
    bool persistentOk = (checkAcc != nullptr && checkAcc->VerifyPassword("matkhau_moi_456"));
    cout << "  11. Kiem tra dong bo CSV (Write-Through): " << (persistentOk ? "THANH CONG (Du lieu da luu ben vung vao file)" : "THAT BAI") << "\n";

    // Don dep tai khoan test de giu file Data/Accounts.csv nguyen ban
    accountRepo.RemoveAccount(testUser);
    cout << "  12. Don dep tai khoan test sau kiem thu: " << (!accountRepo.Exists(testUser) ? "THANH CONG (File Accounts.csv sach se)" : "THAT BAI") << "\n";

    cout << "\n[INFO] Hoan tat toan bo kiem thu Backend Dang nhap va Cart!\n";
    return 0;
}