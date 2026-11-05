#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

// =================================================================
//  THƯ VIỆN ĐỒ HỌA DEAR IMGUI & GLFW
// =================================================================
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <GLFW/glfw3native.h>
#endif

// =================================================================
//  THƯ VIỆN DỰ ÁN PBL2 (KIẾN TRÚC 4 TẦNG)
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
#include "../Libraries/AccountStorage.h"
#include "../Libraries/AuthManager.h"
#include "../Libraries/ProductStorage.h"
#include "../Libraries/ProductService.h"
#include "../Libraries/OrderStorage.h"
#include "../Libraries/OrderManager.h"

using namespace std;

// =================================================================
//  TRẠNG THÁI TOÀN CỤC CỦA GIAO DIỆN ỨNG DỤNG
// =================================================================
enum AppScreen {
    SCREEN_LOGIN = 0,      // Màn hình Đăng nhập / Đăng ký
    SCREEN_USER = 1,       // Màn hình Khách hàng mua sắm & Giỏ hàng
    SCREEN_ADMIN = 2       // Màn hình Quản trị viên cửa hàng
};

// Định dạng số tiền có dấu chấm phân cách hàng nghìn (VD: 150000 -> 150.000 đ)
string FormatMoney(unsigned long long amount) {
    string s = to_string(amount);
    int n = (int)s.length();
    string res = "";
    int count = 0;
    for (int i = n - 1; i >= 0; i--) {
        res = s[i] + res;
        count++;
        if (count == 3 && i > 0) {
            res = "." + res;
            count = 0;
        }
    }
    return res + " đ";
}

// Thiết lập giao diện màu tối (Dark Theme) hiện đại, chuyên nghiệp
void SetupCustomDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.WindowRounding    = 8.0f;
    style.FrameRounding     = 5.0f;
    style.PopupRounding     = 6.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding      = 4.0f;
    style.TabRounding       = 6.0f;
    style.ItemSpacing       = ImVec2(10.0f, 8.0f);
    style.FramePadding      = ImVec2(8.0f, 5.0f);

    colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.53f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.12f, 0.13f, 0.17f, 1.00f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.16f, 0.21f, 1.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.14f, 0.15f, 0.20f, 0.98f);
    colors[ImGuiCol_Border]                = ImVec4(0.24f, 0.26f, 0.35f, 0.80f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.18f, 0.20f, 0.26f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.24f, 0.27f, 0.36f, 1.00f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.28f, 0.32f, 0.42f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.15f, 0.16f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.20f, 0.22f, 0.30f, 1.00f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4(0.15f, 0.16f, 0.22f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.12f, 0.13f, 0.17f, 0.60f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.28f, 0.31f, 0.40f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.30f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.30f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.22f, 0.35f, 0.55f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.45f, 0.70f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.35f, 0.55f, 0.85f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.28f, 0.42f, 1.00f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.38f, 0.56f, 1.00f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.32f, 0.46f, 0.68f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4(0.24f, 0.26f, 0.35f, 1.00f);
    colors[ImGuiCol_Tab]                   = ImVec4(0.16f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_TabHovered]            = ImVec4(0.28f, 0.42f, 0.65f, 1.00f);
    colors[ImGuiCol_TabActive]             = ImVec4(0.22f, 0.35f, 0.55f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.18f, 0.22f, 0.30f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.28f, 0.32f, 0.42f, 1.00f);
    colors[ImGuiCol_TableBorderLight]      = ImVec4(0.20f, 0.23f, 0.30f, 1.00f);
    colors[ImGuiCol_TableRowBg]            = ImVec4(0.14f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4(0.16f, 0.18f, 0.24f, 1.00f);
}

// =================================================================
//  BIẾN DỮ LIỆU ĐIỀU KHIỂN & BỘ ĐỆM NHẬP LIỆU (BUFFERS)
// =================================================================
static AppScreen currentScreen = SCREEN_LOGIN;

// Bộ đệm Đăng nhập & Đăng ký
static char loginUser[64] = "";
static char loginPass[64] = "";
static string loginStatusMsg = "";
static ImVec4 loginStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

static char regFullName[128] = "";
static char regUser[64] = "";
static char regPass[64] = "";
static int regMoney = 500000;
static int regRoleIndex = 1; // 0: Admin, 1: User
static string regStatusMsg = "";
static ImVec4 regStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

// Bộ đệm Mua sắm (User)
static char searchKeyword[128] = "";
static int filterCategoryIdx = 0; // 0: Tat ca, 1: Food, 2: Electronics
static int buyQuantity[500] = {0}; // Bo dem so luong mua cho tung san pham
static string userActionMsg = "";
static string userLastReceiptPath = "";

// Bộ đệm Đổi mật khẩu
static char changeOldPass[64] = "";
static char changeNewPass[64] = "";
static char changeConfirmPass[64] = "";
static string changePassMsg = "";

// Bộ đệm Quản lý Sản phẩm (Admin)
static bool showAddProdModal = false;
static char newProdID[32] = "";
static char newProdName[128] = "";
static int newProdType = 0; // 0: Food, 1: Electronics
static int newProdPrice = 20000;
static int newProdQty = 10;
static char newProdDesc[256] = "";
static char newProdNSX[32] = "2024-03-01";
static char newProdHSD[32] = "2024-09-01";
static int newProdWarranty = 12;
static int newProdPower = 500;
static string adminProdMsg = "";

// Bộ đệm Cảnh báo (Admin)
static char alertExpDate[32] = "2024-04-01";

// Bộ đệm Doanh thu & Lọc ngày (Admin)
static char filterFromDate[32] = "2024-01-01";
static char filterToDate[32] = "2024-12-31";
static bool useDateFilter = false;

// Bộ đệm Quản lý Tài khoản & Nạp ví (Admin)
static char depositUsername[64] = "";
static int depositAmount = 100000;
static string adminAccountMsg = "";

// =================================================================
//  HÀM KẺ THANH TIÊU ĐỀ CHUNG CHO USER VÀ ADMIN (TOP HEADER BAR)
// =================================================================
void RenderHeaderBar(AuthManager& authManager, ProductStorage& productStorage, Cart*& activeCart) {
    Account* acc = authManager.GetCurrentAccount();
    if (acc == nullptr) return;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.10f, 0.12f, 0.16f, 1.0f));
    ImGui::BeginChild("HeaderBar", ImVec2(0, 48), true, ImGuiWindowFlags_NoScrollbar);

    // Tên ứng dụng
    ImGui::SetCursorPosY(10);
    ImGui::TextColored(ImVec4(0.40f, 0.80f, 1.00f, 1.0f), "PBL2 - HE THONG BAN HANG & QUAN LY KHO");

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 560);
    
    // Thông tin người dùng
    string roleStr = (acc->getPermission() == Admin) ? "[ADMIN]" : "[KHACH HANG]";
    ImGui::Text("Nguoi dung: %s (%s)", acc->getName().c_str(), roleStr.c_str());

    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.40f, 1.00f, 0.50f, 1.0f), "| Vi: %s", FormatMoney(acc->getBalance()).c_str());

    ImGui::SameLine();
    ImGui::SetCursorPosY(6);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.20f, 0.20f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.25f, 0.25f, 1.0f));
    if (ImGui::Button("Dang xuat", ImVec2(90, 32))) {
        // Neu co gio hang cua user, hoan tra ton kho truoc khi dang xuat
        if (activeCart != nullptr) {
            activeCart->Clear(&productStorage.GetTableRef());
            delete activeCart;
            activeCart = nullptr;
        }
        authManager.Logout();
        currentScreen = SCREEN_LOGIN;
        userActionMsg = "";
    }
    ImGui::PopStyleColor(2);

    ImGui::EndChild();
    ImGui::PopStyleColor();
}

// =================================================================
//  MÀN HÌNH 1: ĐĂNG NHẬP & ĐĂNG KÝ TÀI KHOẢN
// =================================================================
void RenderLoginScreen(AuthManager& authManager, ProductStorage& productStorage, Cart*& activeCart) {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImVec2 windowSize = ImVec2(500, 520);
    ImGui::SetNextWindowPos(ImVec2(center.x - windowSize.x * 0.5f, center.y - windowSize.y * 0.5f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("DANG NHAP HE THONG##LoginWindow", nullptr, flags);

    ImGui::Spacing();
    ImGui::SetCursorPosX((windowSize.x - 380) * 0.5f);
    ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.00f, 1.0f), "DO AN PBL2: HE THONG QUAN LY CUA HANG");
    ImGui::SetCursorPosX((windowSize.x - 300) * 0.5f);
    ImGui::TextDisabled("Lap trinh Huong doi tuong & Cau truc Du lieu");
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::BeginTabBar("AuthTabBar")) {
        // =========================================================
        // TAB: DANG NHAP
        // =========================================================
        if (ImGui::BeginTabItem("  DANG NHAP  ")) {
            ImGui::Spacing();
            ImGui::Text("Ten dang nhap:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##loginUser", loginUser, sizeof(loginUser));

            ImGui::Spacing();
            ImGui::Text("Mat khau:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##loginPass", loginPass, sizeof(loginPass), ImGuiInputTextFlags_Password);

            ImGui::Spacing();
            if (!loginStatusMsg.empty()) {
                ImGui::TextColored(loginStatusColor, "%s", loginStatusMsg.c_str());
                ImGui::Spacing();
            }

            // Nut Dang nhap
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.55f, 0.35f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.70f, 0.45f, 1.0f));
            if (ImGui::Button("DANG NHAP VAO HE THONG", ImVec2(-1, 40))) {
                AuthResult res = authManager.Login(loginUser, loginPass);
                if (res == AUTH_SUCCESS) {
                    loginStatusMsg = "";
                    Role r = authManager.GetCurrentRole();
                    if (r == Admin) {
                        currentScreen = SCREEN_ADMIN;
                    } else {
                        currentScreen = SCREEN_USER;
                        if (activeCart != nullptr) delete activeCart;
                        activeCart = new Cart(authManager.GetCurrentAccount()->getName());
                    }
                } else if (res == AUTH_ERR_EMPTY_INPUT) {
                    loginStatusMsg = "Vui long nhap day du Ten dang nhap va Mat khau!";
                    loginStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                } else if (res == AUTH_ERR_USER_NOT_FOUND) {
                    loginStatusMsg = "Tai khoan khong ton tai trong he thong!";
                    loginStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                } else if (res == AUTH_ERR_WRONG_PASSWORD) {
                    loginStatusMsg = "Mat khau khong chinh xac!";
                    loginStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                }
            }
            ImGui::PopStyleColor(2);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Phim tat dang nhap nhanh danh cho buoi van dap bao ve
            ImGui::TextDisabled("Phim tat dang nhap nhanh (Thao tac bao ve do an):");
            if (ImGui::Button("Admin (admin / admin123)", ImVec2(-1, 28))) {
                strcpy(loginUser, "admin");
                strcpy(loginPass, "admin123");
                if (authManager.Login("admin", "admin123") == AUTH_SUCCESS) {
                    currentScreen = SCREEN_ADMIN;
                }
            }
            if (ImGui::Button("Khach hang (khachhang1 / 123456)", ImVec2(-1, 28))) {
                strcpy(loginUser, "khachhang1");
                strcpy(loginPass, "123456");
                if (authManager.Login("khachhang1", "123456") == AUTH_SUCCESS) {
                    currentScreen = SCREEN_USER;
                    if (activeCart != nullptr) delete activeCart;
                    activeCart = new Cart(authManager.GetCurrentAccount()->getName());
                }
            }
            if (ImGui::Button("Nhan vien (nhanvien1 / 123456)", ImVec2(-1, 28))) {
                strcpy(loginUser, "nhanvien1");
                strcpy(loginPass, "123456");
                if (authManager.Login("nhanvien1", "123456") == AUTH_SUCCESS) {
                    currentScreen = SCREEN_USER;
                    if (activeCart != nullptr) delete activeCart;
                    activeCart = new Cart(authManager.GetCurrentAccount()->getName());
                }
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB: DANG KY TAI KHOAN MOI
        // =========================================================
        if (ImGui::BeginTabItem("  DANG KY MOI  ")) {
            ImGui::Spacing();
            ImGui::Text("Ho va ten chu tai khoan:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##regFullName", regFullName, sizeof(regFullName));

            ImGui::Spacing();
            ImGui::Text("Ten dang nhap:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##regUser", regUser, sizeof(regUser));

            ImGui::Spacing();
            ImGui::Text("Mat khau:");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##regPass", regPass, sizeof(regPass), ImGuiInputTextFlags_Password);

            ImGui::Spacing();
            ImGui::Text("So du vi khoi tao (VND):");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputInt("##regMoney", &regMoney, 50000, 100000);
            if (regMoney < 0) regMoney = 0;

            ImGui::Spacing();
            ImGui::Text("Vai tro tai khoan:");
            ImGui::RadioButton("User (Khach hang)", &regRoleIndex, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Admin (Quan tri vien)", &regRoleIndex, 0);

            ImGui::Spacing();
            if (!regStatusMsg.empty()) {
                ImGui::TextColored(regStatusColor, "%s", regStatusMsg.c_str());
                ImGui::Spacing();
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.40f, 0.70f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.50f, 0.85f, 1.0f));
            if (ImGui::Button("DANG KY TAI KHOAN NGAY", ImVec2(-1, 38))) {
                Role r = (regRoleIndex == 0) ? Admin : User;
                AuthResult res = authManager.Register(regUser, regPass, regFullName, r, (unsigned long long)regMoney);
                if (res == AUTH_SUCCESS) {
                    regStatusMsg = "Dang ky thanh cong! Vui long chuyen sang tab Dang nhap.";
                    regStatusColor = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
                    strcpy(loginUser, regUser);
                    strcpy(loginPass, regPass);
                } else if (res == AUTH_ERR_USER_ALREADY_EXISTS) {
                    regStatusMsg = "Ten dang nhap nay da ton tai!";
                    regStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                } else if (res == AUTH_ERR_EMPTY_INPUT) {
                    regStatusMsg = "Vui long dien day du tat ca thong tin!";
                    regStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                } else {
                    regStatusMsg = "Khong the luu tai khoan xuong CSV!";
                    regStatusColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                }
            }
            ImGui::PopStyleColor(2);

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// =================================================================
//  MÀN HÌNH 2: GIAO DIỆN KHÁCH HÀNG (USER POS / SHOPPING)
// =================================================================
void RenderUserScreen(AuthManager& authManager, ProductStorage& productStorage, ProductService& productService, OrderManager& orderManager, Cart*& activeCart) {
    RenderHeaderBar(authManager, productStorage, activeCart);

    ImGui::SetNextWindowPos(ImVec2(0, 52));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 52));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("UserMainWindow", nullptr, flags);

    Account* currentUser = authManager.GetCurrentAccount();

    if (ImGui::BeginTabBar("UserTabBar")) {
        // =========================================================
        // TAB 1: MUA SẮM & DANH MỤC SẢN PHẨM
        // =========================================================
        if (ImGui::BeginTabItem("  1. MUA SAM & DANH MUC HANG HOA  ")) {
            ImGui::Spacing();

            // Thanh tìm kiếm và bộ lọc phân loại
            ImGui::Text("Tim kiem san pham:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(300);
            ImGui::InputTextWithHint("##SearchBox", "Nhap ten san pham can tim...", searchKeyword, sizeof(searchKeyword));

            ImGui::SameLine();
            ImGui::Text("Phan loai:");
            ImGui::SameLine();
            const char* categories[] = { "Tat ca san pham", "Thuc pham (Food)", "Do dien may (Electronics)" };
            ImGui::SetNextItemWidth(220);
            ImGui::Combo("##CategoryFilter", &filterCategoryIdx, categories, 3);

            ImGui::SameLine();
            if (ImGui::Button("Lam moi danh sach")) {
                searchKeyword[0] = '\0';
                filterCategoryIdx = 0;
            }

            ImGui::Spacing();
            if (!userActionMsg.empty()) {
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", userActionMsg.c_str());
                ImGui::Spacing();
            }

            // Lấy danh sách sản phẩm theo bộ lọc
            vector<ShopItem*> displayItems;
            if (strlen(searchKeyword) > 0) {
                displayItems = productService.SearchByName(searchKeyword);
            } else if (filterCategoryIdx == 1) {
                displayItems = productService.GetByCategory("Food");
            } else if (filterCategoryIdx == 2) {
                displayItems = productService.GetByCategory("Electronics");
            } else {
                displayItems = productStorage.GetAllProducts();
            }

            // Bảng danh mục sản phẩm ImGui Table
            ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
            if (ImGui::BeginTable("ProductsTable", 8, tableFlags, ImVec2(0, ImGui::GetWindowHeight() - 170))) {
                ImGui::TableSetupColumn("Ma SP", ImGuiTableColumnFlags_WidthFixed, 90.0f);
                ImGui::TableSetupColumn("Ten san pham", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Phan loai", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Don gia", ImGuiTableColumnFlags_WidthFixed, 110.0f);
                ImGui::TableSetupColumn("Ton kho", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Thong tin dac thu", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("So luong mua", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Thao tac", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableHeadersRow();

                int itemCount = (int)displayItems.size();
                for (int i = 0; i < itemCount; i++) {
                    ShopItem* item = displayItems[i];
                    if (item == nullptr) continue;

                    ImGui::TableNextRow();

                    // Cột 1: Mã SP
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", item->ProductID.c_str());

                    // Cột 2: Tên SP
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", item->Name.c_str());

                    // Cột 3: Loại
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", item->Category.c_str());

                    // Cột 4: Giá
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", FormatMoney(item->Price).c_str());

                    // Cột 5: Tồn kho
                    ImGui::TableSetColumnIndex(4);
                    if (item->QuantityRemaining <= 5) {
                        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d (Sap het)", item->QuantityRemaining);
                    } else {
                        ImGui::Text("%d", item->QuantityRemaining);
                    }

                    // Cột 6: Đa hình thông tin đặc thù
                    ImGui::TableSetColumnIndex(5);
                    FoodItem* food = dynamic_cast<FoodItem*>(item);
                    ElectronicItem* elec = dynamic_cast<ElectronicItem*>(item);
                    if (food != nullptr) {
                        ImGui::TextDisabled("NSX: %s | HSD: %s", food->ManufactureDate.c_str(), food->ExpiryDate.c_str());
                    } else if (elec != nullptr) {
                        ImGui::TextDisabled("Bao hanh: %d thang | %dW", elec->WarrantyMonths, elec->PowerWatts);
                    } else {
                        ImGui::TextDisabled("%s", item->Description.c_str());
                    }

                    // Cột 7: Số lượng chọn mua
                    ImGui::TableSetColumnIndex(6);
                    int safeIdx = (i < 500) ? i : 499;
                    if (buyQuantity[safeIdx] <= 0) buyQuantity[safeIdx] = 1;
                    ImGui::SetNextItemWidth(90);
                    string qtyID = "##qty_" + to_string(i);
                    ImGui::InputInt(qtyID.c_str(), &buyQuantity[safeIdx], 1, 5);
                    if (buyQuantity[safeIdx] < 1) buyQuantity[safeIdx] = 1;

                    // Cột 8: Nút Thêm vào giỏ
                    ImGui::TableSetColumnIndex(7);
                    string btnID = "Them vao gio##" + to_string(i);
                    bool canBuy = (item->QuantityRemaining >= buyQuantity[safeIdx]);

                    if (!canBuy) {
                        ImGui::BeginDisabled();
                    }
                    if (ImGui::Button(btnID.c_str())) {
                        if (activeCart != nullptr) {
                            if (activeCart->AddItem(item, buyQuantity[safeIdx])) {
                                userActionMsg = "Da them " + to_string(buyQuantity[safeIdx]) + " " + item->Name + " vao gio hang!";
                            } else {
                                userActionMsg = "Khong the them! So luong ton kho khong du.";
                            }
                        }
                    }
                    if (!canBuy) {
                        ImGui::EndDisabled();
                    }
                }
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB 2: GIỎ HÀNG & THANH TOÁN (CƠ CHẾ UNDO LIFO)
        // =========================================================
        if (ImGui::BeginTabItem("  2. GIO HANG & THANH TOAN (POS)  ")) {
            ImGui::Spacing();

            if (activeCart == nullptr || activeCart->IsEmpty()) {
                ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.4f, 1.0f), "Gio hang hien tai dang trong! Vui long chon them hang hoa tu tab '1. Mua sam'.");
            } else {
                vector<CartItem> cartList = activeCart->ToVector();
                int totalCartCount = (int)cartList.size();
                long long totalAmount = activeCart->GetTotalAmount();

                ImGui::Text("So loai mat hang trong gio: %d | Tong tien tam tinh: ", totalCartCount);
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", FormatMoney(totalAmount).c_str());

                ImGui::Spacing();

                // Bảng chi tiết giỏ hàng
                ImGuiTableFlags cartTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                if (ImGui::BeginTable("CartTable", 6, cartTableFlags, ImVec2(0, 240))) {
                    ImGui::TableSetupColumn("STT", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                    ImGui::TableSetupColumn("Ma SP", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Ten san pham", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Don gia", ImGuiTableColumnFlags_WidthFixed, 130.0f);
                    ImGui::TableSetupColumn("So luong", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Thanh tien", ImGuiTableColumnFlags_WidthFixed, 140.0f);
                    ImGui::TableHeadersRow();

                    for (int i = 0; i < totalCartCount; i++) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", i + 1);

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", cartList[i].ProductID.c_str());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%s", cartList[i].ProductName.c_str());

                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("%s", FormatMoney(cartList[i].UnitPrice).c_str());

                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("%d", cartList[i].Quantity);

                        ImGui::TableSetColumnIndex(5);
                        ImGui::Text("%s", FormatMoney(cartList[i].SubTotal).c_str());
                    }
                    ImGui::EndTable();
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // KHU VỰC THAO TÁC & HOÀN TÁC (UNDO LIFO)
                ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.0f, 1.0f), "Thao tac giỏ hàng & Co che Hoan tac (LIFO Stack):");
                ImGui::Spacing();

                // NÚT HOÀN TÁC (UNDO)
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.45f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.55f, 0.20f, 1.0f));
                if (ImGui::Button("HOAN TAC MON VUA THEM (UNDO LIFO)", ImVec2(320, 38))) {
                    CartItem undoneItem;
                    if (activeCart->Undo(productStorage.GetTableRef(), undoneItem)) {
                        userActionMsg = "DA HOAN TAC: Rut [" + undoneItem.ProductID + "] " + undoneItem.ProductName + " (x" + to_string(undoneItem.Quantity) + ") khoi gio va hoan tra lai ton kho!";
                    } else {
                        userActionMsg = "Gio hang da trong, khong con thao tac de hoan tac!";
                    }
                }
                ImGui::PopStyleColor(2);

                ImGui::SameLine();
                // NÚT HỦY TOÀN BỘ GIỎ HÀNG
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.55f, 0.20f, 0.20f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.25f, 0.25f, 1.0f));
                if (ImGui::Button("HUY TOAN BO GIO HANG", ImVec2(220, 38))) {
                    activeCart->Clear(&productStorage.GetTableRef());
                    userActionMsg = "Da huy gio hang va hoan tra toan bo hang hoa ve lai kho!";
                }
                ImGui::PopStyleColor(2);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // NÚT XÁC NHẬN THANH TOÁN (CHECKOUT)
                unsigned long long userBalance = currentUser->getBalance();
                bool enoughMoney = (userBalance >= (unsigned long long)totalAmount);

                ImGui::Text("So du vi cua ban: %s | So du con lai sau thanh toan: ", FormatMoney(userBalance).c_str());
                ImGui::SameLine();
                if (enoughMoney) {
                    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", FormatMoney(userBalance - totalAmount).c_str());
                } else {
                    ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "KHONG DU SO DU! (Can nap them %s)", FormatMoney(totalAmount - userBalance).c_str());
                }

                ImGui::Spacing();

                if (!enoughMoney) ImGui::BeginDisabled();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.60f, 0.30f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.75f, 0.35f, 1.0f));
                if (ImGui::Button("XAC NHAN THANH TOAN & XUAT HOA DON IN NHIET", ImVec2(400, 42))) {
                    string createdOrderID = "";
                    string createdReceiptPath = "";

                    // Luu danh sach mon de dong bo ton kho xuong Products.csv
                    vector<CartItem> itemsToSync = activeCart->ToVector();

                    bool ok = orderManager.Checkout(*activeCart, *currentUser, "Self-Service", "", &createdOrderID, &createdReceiptPath);
                    if (ok) {
                        // Dong bo so luong con lai xuong CSV
                        int syncCount = (int)itemsToSync.size();
                        for (int k = 0; k < syncCount; k++) {
                            ShopItem* target = productStorage.GetByID(itemsToSync[k].ProductID);
                            if (target != nullptr) {
                                productStorage.UpdateQuantity(target->ProductID, target->QuantityRemaining);
                            }
                        }

                        userActionMsg = "THANH TOAN THANH CONG! Ma don: " + createdOrderID;
                        userLastReceiptPath = createdReceiptPath;
                    } else {
                        userActionMsg = "Thanh toan that bai! Vui long thu lai.";
                    }
                }
                ImGui::PopStyleColor(2);
                if (!enoughMoney) ImGui::EndDisabled();

                if (!userLastReceiptPath.empty()) {
                    ImGui::Spacing();
                    ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "-> Phieu hoa don in nhiet (.txt) da luu tai: %s", userLastReceiptPath.c_str());
                }
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB 3: LỊCH SỬ ĐƠN HÀNG CÁ NHÂN
        // =========================================================
        if (ImGui::BeginTabItem("  3. LICH SU DON HANG CA NHAN  ")) {
            ImGui::Spacing();
            vector<OrderRecord> myOrders = orderManager.FilterByCustomer(currentUser->getUID());

            ImGui::Text("Ban da thuc hien tong cong %d don hang:", (int)myOrders.size());
            ImGui::Spacing();

            ImGuiTableFlags histFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
            if (ImGui::BeginTable("UserOrdersTable", 5, histFlags, ImVec2(0, ImGui::GetWindowHeight() - 150))) {
                ImGui::TableSetupColumn("Ma don hang", ImGuiTableColumnFlags_WidthFixed, 110.0f);
                ImGui::TableSetupColumn("Ngay gio dat", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Nguoi mua", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Tong gia tri", ImGuiTableColumnFlags_WidthFixed, 130.0f);
                ImGui::TableSetupColumn("Chi tiet mat hang", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                int ordCount = (int)myOrders.size();
                for (int i = 0; i < ordCount; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", myOrders[i].OrderID.c_str());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", myOrders[i].OrderDate.c_str());

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", myOrders[i].CustomerName.c_str());

                    ImGui::TableSetColumnIndex(3);
                    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", FormatMoney(myOrders[i].TotalAmount).c_str());

                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%s", myOrders[i].ItemsSummary.c_str());
                }
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB 4: THÔNG TIN TÀI KHOẢN & ĐỔI MẬT KHẨU
        // =========================================================
        if (ImGui::BeginTabItem("  4. THONG TIN TAI KHOAN  ")) {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.0f, 1.0f), "HO SO NGUOI DUNG:");
            ImGui::Text("UID he thong: %lld", currentUser->getUID());
            ImGui::Text("Ten dang nhap: %s", currentUser->getUsername().c_str());
            ImGui::Text("Ho va ten: %s", currentUser->getName().c_str());
            ImGui::Text("Vai tro: %s", currentUser->getPermission() == Admin ? "Admin (Quan tri vien)" : "User (Khach hang)");
            ImGui::Text("So du vi tien hien tai: %s", FormatMoney(currentUser->getBalance()).c_str());

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.0f, 1.0f), "DOI MAT KHAU:");
            ImGui::SetNextItemWidth(300);
            ImGui::InputText("Mat khau cu##oldPass", changeOldPass, sizeof(changeOldPass), ImGuiInputTextFlags_Password);

            ImGui::SetNextItemWidth(300);
            ImGui::InputText("Mat khau moi##newPass", changeNewPass, sizeof(changeNewPass), ImGuiInputTextFlags_Password);

            ImGui::SetNextItemWidth(300);
            ImGui::InputText("Xac nhan mat khau moi##confirmPass", changeConfirmPass, sizeof(changeConfirmPass), ImGuiInputTextFlags_Password);

            ImGui::Spacing();
            if (!changePassMsg.empty()) {
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%s", changePassMsg.c_str());
                ImGui::Spacing();
            }

            if (ImGui::Button("CAP NHAT MAT KHAU", ImVec2(200, 34))) {
                if (string(changeNewPass) != string(changeConfirmPass)) {
                    changePassMsg = "Mat khau xac nhan khong khop!";
                } else {
                    AuthResult res = authManager.ChangePassword(changeOldPass, changeNewPass);
                    if (res == AUTH_SUCCESS) {
                        changePassMsg = "Doi mat khau thanh cong!";
                        changeOldPass[0] = '\0';
                        changeNewPass[0] = '\0';
                        changeConfirmPass[0] = '\0';
                    } else if (res == AUTH_ERR_WRONG_PASSWORD) {
                        changePassMsg = "Mat khau cu khong dung!";
                    } else {
                        changePassMsg = "Khong the doi mat khau!";
                    }
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// =================================================================
//  MÀN HÌNH 3: GIAO DIỆN QUẢN TRỊ VIÊN (ADMIN DASHBOARD)
// =================================================================
void RenderAdminScreen(AuthManager& authManager, ProductStorage& productStorage, ProductService& productService, OrderManager& orderManager, AccountStorage& accountStorage, Cart*& activeCart) {
    RenderHeaderBar(authManager, productStorage, activeCart);

    ImGui::SetNextWindowPos(ImVec2(0, 52));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - 52));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("AdminMainWindow", nullptr, flags);

    if (ImGui::BeginTabBar("AdminTabBar")) {
        // =========================================================
        // TAB 1: QUẢN LÝ KHO HÀNG (CRUD SẢN PHẨM)
        // =========================================================
        if (ImGui::BeginTabItem("  1. QUAN LY KHO HANG  ")) {
            ImGui::Spacing();

            // Nút mở form thêm sản phẩm
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.50f, 0.30f, 1.0f));
            if (ImGui::Button("+ THEM SAN PHAM MOI", ImVec2(180, 32))) {
                showAddProdModal = !showAddProdModal;
            }
            ImGui::PopStyleColor();

            ImGui::SameLine();
            if (!adminProdMsg.empty()) {
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%s", adminProdMsg.c_str());
            }

            // FORM THÊM SẢN PHẨM MỚI (COLLAPSIBLE / MODAL)
            if (showAddProdModal) {
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.18f, 0.24f, 1.0f));
                ImGui::BeginChild("AddProdBox", ImVec2(0, 240), true);

                ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "NHAP THONG TIN SAN PHAM MOI (Tu dong ghi Write-Through vao Products.csv):");
                ImGui::Spacing();

                ImGui::Columns(2, "AddProdCols", false);
                ImGui::SetColumnWidth(0, 320);

                ImGui::Text("Ma SP (VD: FOOD-005):");
                ImGui::SetNextItemWidth(280);
                ImGui::InputText("##newID", newProdID, sizeof(newProdID));

                ImGui::Text("Ten san pham:");
                ImGui::SetNextItemWidth(280);
                ImGui::InputText("##newName", newProdName, sizeof(newProdName));

                ImGui::Text("Phan loai:");
                ImGui::RadioButton("Thuc pham (Food)", &newProdType, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Dien may (Electronics)", &newProdType, 1);

                ImGui::NextColumn();

                ImGui::Text("Gia ban (VND):");
                ImGui::SetNextItemWidth(250);
                ImGui::InputInt("##newPrice", &newProdPrice, 5000, 10000);
                if (newProdPrice < 0) newProdPrice = 0;

                ImGui::Text("So luong ton kho:");
                ImGui::SetNextItemWidth(250);
                ImGui::InputInt("##newQty", &newProdQty, 1, 10);
                if (newProdQty < 0) newProdQty = 0;

                if (newProdType == 0) {
                    ImGui::Text("Ngay san xuat (NSX) & Han dung (HSD):");
                    ImGui::SetNextItemWidth(120);
                    ImGui::InputText("##newNSX", newProdNSX, sizeof(newProdNSX));
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    ImGui::InputText("##newHSD", newProdHSD, sizeof(newProdHSD));
                } else {
                    ImGui::Text("Bao hanh (thang) & Cong suat (W):");
                    ImGui::SetNextItemWidth(120);
                    ImGui::InputInt("##newBH", &newProdWarranty, 1, 6);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(120);
                    ImGui::InputInt("##newCS", &newProdPower, 50, 100);
                }

                ImGui::Columns(1);
                ImGui::Spacing();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.60f, 0.40f, 1.0f));
                if (ImGui::Button("LUU SAN PHAM VAO KHO", ImVec2(200, 32))) {
                    if (strlen(newProdID) == 0 || strlen(newProdName) == 0) {
                        adminProdMsg = "Vui long nhap Ma SP va Ten san pham!";
                    } else if (productStorage.Exists(newProdID)) {
                        adminProdMsg = "Ma san pham nay da ton tai!";
                    } else {
                        ShopItem* newItem = nullptr;
                        if (newProdType == 0) {
                            newItem = new FoodItem(newProdID, newProdName, (long long)newProdPrice, newProdQty, "Thuc pham tuoi ngon", newProdNSX, newProdHSD);
                        } else {
                            newItem = new ElectronicItem(newProdID, newProdName, (long long)newProdPrice, newProdQty, "Thiet bi dien gia dung", newProdWarranty, newProdPower);
                        }

                        if (productStorage.AddProduct(newItem)) {
                            adminProdMsg = "Da them thanh cong san pham [" + string(newProdID) + "] vao he thong!";
                            showAddProdModal = false;
                            newProdID[0] = '\0';
                            newProdName[0] = '\0';
                        } else {
                            adminProdMsg = "Loi khi luu san pham vao he thong!";
                        }
                    }
                }
                ImGui::PopStyleColor();

                ImGui::SameLine();
                if (ImGui::Button("Huy bo", ImVec2(90, 32))) {
                    showAddProdModal = false;
                }

                ImGui::EndChild();
                ImGui::PopStyleColor();
            }

            ImGui::Spacing();

            // BẢNG DANH MỤC TOÀN BỘ SẢN PHẨM TRONG KHO
            vector<ShopItem*> allProds = productStorage.GetAllProducts();
            int totalProds = (int)allProds.size();

            ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
            if (ImGui::BeginTable("AdminProdTable", 7, tableFlags, ImVec2(0, ImGui::GetWindowHeight() - (showAddProdModal ? 430 : 160)))) {
                ImGui::TableSetupColumn("Ma SP", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Ten san pham", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Loai", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Don gia", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Ton kho", ImGuiTableColumnFlags_WidthFixed, 90.0f);
                ImGui::TableSetupColumn("Thong tin dac thu", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Thao tac", ImGuiTableColumnFlags_WidthFixed, 180.0f);
                ImGui::TableHeadersRow();

                for (int i = 0; i < totalProds; i++) {
                    ShopItem* item = allProds[i];
                    if (item == nullptr) continue;

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", item->ProductID.c_str());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", item->Name.c_str());

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", item->Category.c_str());

                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", FormatMoney(item->Price).c_str());

                    ImGui::TableSetColumnIndex(4);
                    if (item->QuantityRemaining <= 5) {
                        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d (Thap)", item->QuantityRemaining);
                    } else {
                        ImGui::Text("%d", item->QuantityRemaining);
                    }

                    ImGui::TableSetColumnIndex(5);
                    FoodItem* food = dynamic_cast<FoodItem*>(item);
                    ElectronicItem* elec = dynamic_cast<ElectronicItem*>(item);
                    if (food != nullptr) {
                        ImGui::TextDisabled("NSX: %s | HSD: %s", food->ManufactureDate.c_str(), food->ExpiryDate.c_str());
                    } else if (elec != nullptr) {
                        ImGui::TextDisabled("BH: %d thang | %dW", elec->WarrantyMonths, elec->PowerWatts);
                    } else {
                        ImGui::TextDisabled("%s", item->Description.c_str());
                    }

                    // Thao tác: Nhập thêm kho & Xóa sản phẩm
                    ImGui::TableSetColumnIndex(6);
                    string addStockBtn = "+5 Kho##" + to_string(i);
                    if (ImGui::Button(addStockBtn.c_str())) {
                        productStorage.UpdateQuantity(item->ProductID, item->QuantityRemaining + 5);
                        adminProdMsg = "Da cong them 5 ton kho cho [" + item->ProductID + "]!";
                    }

                    ImGui::SameLine();
                    string delBtn = "Xoa##" + to_string(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.65f, 0.20f, 0.20f, 1.0f));
                    if (ImGui::Button(delBtn.c_str())) {
                        string delID = item->ProductID;
                        if (productStorage.RemoveProduct(delID)) {
                            adminProdMsg = "Da xoa san pham [" + delID + "] khoi he thong!";
                        }
                    }
                    ImGui::PopStyleColor();
                }
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB 2: CẢNH BÁO TỒN KHO THẤP & CẬN DATE
        // =========================================================
        if (ImGui::BeginTabItem("  2. CANH BAO TON KHO & CAN DATE  ")) {
            ImGui::Spacing();

            // Phần 1: Cảnh báo hàng tồn kho dưới 5
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "CANH BAO 1: CAC SAN PHAM CO TON KHO DUOI 5 MON (Can nhap them):");
            vector<ShopItem*> lowStock = productService.GetLowStockProducts(5);

            if (lowStock.empty()) {
                ImGui::TextDisabled("Tuyet voi! Khong co san pham nao duoi nguong canh bao.");
            } else {
                ImGuiTableFlags lowFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                if (ImGui::BeginTable("LowStockTable", 4, lowFlags, ImVec2(0, 160))) {
                    ImGui::TableSetupColumn("Ma SP", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Ten san pham", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Phan loai", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                    ImGui::TableSetupColumn("Ton kho hien tai", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                    ImGui::TableHeadersRow();

                    int count = (int)lowStock.size();
                    for (int i = 0; i < count; i++) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", lowStock[i]->ProductID.c_str());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", lowStock[i]->Name.c_str());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%s", lowStock[i]->Category.c_str());

                        ImGui::TableSetColumnIndex(3);
                        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d mon", lowStock[i]->QuantityRemaining);
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Phần 2: Cảnh báo thực phẩm cận date / hết hạn
            ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "CANH BAO 2: THUC PHAM CAN DATE HOAC HET HAN TRUOC MOC NGAY:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(140);
            ImGui::InputText("##alertDate", alertExpDate, sizeof(alertExpDate));

            vector<FoodItem*> nearExpiry = productService.GetExpiringFoodProducts(alertExpDate);

            if (nearExpiry.empty()) {
                ImGui::TextDisabled("Khong co thuc pham nao can date truoc moc ngay tren.");
            } else {
                ImGuiTableFlags expFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                if (ImGui::BeginTable("ExpiringFoodTable", 5, expFlags, ImVec2(0, 180))) {
                    ImGui::TableSetupColumn("Ma SP", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Ten thuc pham", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Ngay san xuat", ImGuiTableColumnFlags_WidthFixed, 130.0f);
                    ImGui::TableSetupColumn("Han su dung (HSD)", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                    ImGui::TableSetupColumn("Ton kho con lai", ImGuiTableColumnFlags_WidthFixed, 130.0f);
                    ImGui::TableHeadersRow();

                    int expCount = (int)nearExpiry.size();
                    for (int i = 0; i < expCount; i++) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", nearExpiry[i]->ProductID.c_str());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", nearExpiry[i]->Name.c_str());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%s", nearExpiry[i]->ManufactureDate.c_str());

                        ImGui::TableSetColumnIndex(3);
                        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "%s", nearExpiry[i]->ExpiryDate.c_str());

                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("%d mon", nearExpiry[i]->QuantityRemaining);
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB 3: BÁO CÁO DOANH THU & ĐƠN HÀNG
        // =========================================================
        if (ImGui::BeginTabItem("  3. BAO CAO DOANH THU & DON HANG  ")) {
            ImGui::Spacing();

            // Tổng quan thẻ chỉ số (Dashboard Cards)
            int totalOrders = orderManager.GetTotalOrderCount();
            unsigned long long totalRevenue = orderManager.GetTotalRevenue();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.18f, 0.22f, 0.30f, 1.0f));
            ImGui::BeginChild("MetricCard1", ImVec2(280, 80), true);
            ImGui::TextDisabled("TONG DOANH THU CUA HANG");
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", FormatMoney(totalRevenue).c_str());
            ImGui::EndChild();

            ImGui::SameLine();
            ImGui::BeginChild("MetricCard2", ImVec2(240, 80), true);
            ImGui::TextDisabled("TONG SO LUONG DON HANG");
            ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "%d don da xu ly", totalOrders);
            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // Bộ lọc theo khoảng ngày
            ImGui::Text("Loc don theo khoang ngay:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            ImGui::InputText("Tu ngay##from", filterFromDate, sizeof(filterFromDate));

            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            ImGui::InputText("Den ngay##to", filterToDate, sizeof(filterToDate));

            ImGui::SameLine();
            if (ImGui::Button("Loc don hang")) {
                useDateFilter = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Xem tat ca")) {
                useDateFilter = false;
            }

            vector<OrderRecord> displayOrders = useDateFilter ? 
                orderManager.FilterByDate(filterFromDate, filterToDate) : orderManager.GetAllOrders();

            ImGui::Spacing();
            ImGui::Text("Danh sach don hang (%d don):", (int)displayOrders.size());

            // Bảng danh sách đơn hàng
            ImGuiTableFlags ordFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
            if (ImGui::BeginTable("AdminOrdersTable", 6, ordFlags, ImVec2(0, ImGui::GetWindowHeight() - 250))) {
                ImGui::TableSetupColumn("Ma don", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("UID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("Khach hang", ImGuiTableColumnFlags_WidthFixed, 140.0f);
                ImGui::TableSetupColumn("Ngay gio dat", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Tong gia tri", ImGuiTableColumnFlags_WidthFixed, 130.0f);
                ImGui::TableSetupColumn("Chi tiet mat hang", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                int oCount = (int)displayOrders.size();
                for (int i = 0; i < oCount; i++) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", displayOrders[i].OrderID.c_str());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lld", displayOrders[i].UID);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", displayOrders[i].CustomerName.c_str());

                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", displayOrders[i].OrderDate.c_str());

                    ImGui::TableSetColumnIndex(4);
                    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", FormatMoney(displayOrders[i].TotalAmount).c_str());

                    ImGui::TableSetColumnIndex(5);
                    ImGui::Text("%s", displayOrders[i].ItemsSummary.c_str());
                }
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        // =========================================================
        // TAB 4: QUẢN LÝ TÀI KHOẢN & NẠP VÍ TIỀN
        // =========================================================
        if (ImGui::BeginTabItem("  4. QUAN LY TAI KHOAN & NAP VI  ")) {
            ImGui::Spacing();

            // Khung nạp tiền vào ví người dùng
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.18f, 0.24f, 1.0f));
            ImGui::BeginChild("DepositBox", ImVec2(0, 100), true);

            ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "NAP TIEN VAO VI CHO TAI KHOAN KHACH HANG:");
            ImGui::Spacing();

            ImGui::Text("Ten dang nhap:");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(160);
            ImGui::InputText("##depUser", depositUsername, sizeof(depositUsername));

            ImGui::SameLine();
            ImGui::Text("So tien nap (VND):");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(160);
            ImGui::InputInt("##depAmount", &depositAmount, 50000, 100000);
            if (depositAmount < 0) depositAmount = 0;

            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.55f, 0.35f, 1.0f));
            if (ImGui::Button("XAC NHAN NAP TIEN", ImVec2(160, 26))) {
                Account* targetAcc = accountStorage.GetByUsername(depositUsername);
                if (targetAcc == nullptr) {
                    adminAccountMsg = "Loi: Khong tim thay ten dang nhap '" + string(depositUsername) + "'!";
                } else {
                    targetAcc->getWallet().AddMoney((unsigned long long)depositAmount);
                    if (accountStorage.UpdateAccount(*targetAcc)) {
                        adminAccountMsg = "Da nap thanh cong " + FormatMoney(depositAmount) + " cho tai khoan " + string(depositUsername) + "!";
                    } else {
                        adminAccountMsg = "Khong the luu so du xuong CSV!";
                    }
                }
            }
            ImGui::PopStyleColor();

            if (!adminAccountMsg.empty()) {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", adminAccountMsg.c_str());
            }

            ImGui::EndChild();
            ImGui::PopStyleColor();

            ImGui::Spacing();
            ImGui::Text("Danh sach toan bo tai khoan he thong:");

            // Bảng danh sách tài khoản
            vector<Account> allAccs = accountStorage.GetAllAccounts();
            int totalAccs = (int)allAccs.size();

            ImGuiTableFlags accTableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
            if (ImGui::BeginTable("AccountsTable", 5, accTableFlags, ImVec2(0, ImGui::GetWindowHeight() - 220))) {
                ImGui::TableSetupColumn("UID", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                ImGui::TableSetupColumn("Ten dang nhap", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("Ho va ten", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Vai tro quyen han", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                ImGui::TableSetupColumn("So du vi tien", ImGuiTableColumnFlags_WidthFixed, 160.0f);
                ImGui::TableHeadersRow();

                for (int i = 0; i < totalAccs; i++) {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%lld", allAccs[i].getUID());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", allAccs[i].getUsername().c_str());

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", allAccs[i].getName().c_str());

                    ImGui::TableSetColumnIndex(3);
                    if (allAccs[i].getPermission() == Admin) {
                        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Admin (Quan tri)");
                    } else {
                        ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "User (Khach hang)");
                    }

                    ImGui::TableSetColumnIndex(4);
                    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.5f, 1.0f), "%s", FormatMoney(allAccs[i].getBalance()).c_str());
                }
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

// =================================================================
//  HÀM MAIN (KHỞI TẠO CỬA SỔ, TẢI FONT VÀ VÒNG LẶP IMGUI)
// =================================================================
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 1. Khoi tao GLFW
    if (!glfwInit()) {
        cerr << "[Loi] Khong the khoi tao thu vien GLFW!" << endl;
        return 1;
    }

    // Tao cua so GLFW 1280x740
    GLFWwindow* window = glfwCreateWindow(1280, 740, "PBL2 - He Thong Quan Ly Cua Hang & Ban Hang (Dear ImGui)", NULL, NULL);
    if (!window) {
        cerr << "[Loi] Khong the tao cua so GLFW!" << endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable V-Sync

    // 2. Khoi tao Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Ap dung giao dien Dark Theme dep mat
    SetupCustomDarkTheme();

    // Nap Font Tieng Viet Tahoma ho tro ky tu co dau
    const ImWchar* vietnameseRanges = io.Fonts->GetGlyphRangesVietnamese();
    ImFont* tahomaFont = nullptr;
    ifstream checkFontLib("Libraries/tahoma.ttf");
    if (checkFontLib.is_open()) {
        checkFontLib.close();
        tahomaFont = io.Fonts->AddFontFromFileTTF("Libraries/tahoma.ttf", 16.0f, NULL, vietnameseRanges);
    } else {
        ifstream checkFontBin("tahoma.ttf");
        if (checkFontBin.is_open()) {
            checkFontBin.close();
            tahomaFont = io.Fonts->AddFontFromFileTTF("tahoma.ttf", 16.0f, NULL, vietnameseRanges);
        }
    }
    if (tahomaFont == nullptr) {
        io.Fonts->AddFontDefault();
    }

    // Khoi tao Backends cho OpenGL3 & GLFW
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 3. Khoi tao cac Module Backend PBL2
    AccountStorage accountStorage("Data/Accounts.csv");
    ProductStorage productStorage("Data/Products.csv");
    OrderStorage orderStorage("Data/Orders.csv");

    AuthManager authManager(&accountStorage);
    ProductService productService(&productStorage);
    OrderManager orderManager(&orderStorage, &accountStorage, "Invoices/");

    Cart* activeCart = nullptr;

    // 4. Vong lap render do hoa chinh
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Dieu huong render theo man hinh hien tai
        if (currentScreen == SCREEN_LOGIN) {
            RenderLoginScreen(authManager, productStorage, activeCart);
        } else if (currentScreen == SCREEN_USER) {
            RenderUserScreen(authManager, productStorage, productService, orderManager, activeCart);
        } else if (currentScreen == SCREEN_ADMIN) {
            RenderAdminScreen(authManager, productStorage, productService, orderManager, accountStorage, activeCart);
        }

        // Render ra OpenGL
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.12f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 5. Giai phong bo nho & don dep truoc khi thoat
    if (activeCart != nullptr) {
        activeCart->Clear(&productStorage.GetTableRef());
        delete activeCart;
        activeCart = nullptr;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
