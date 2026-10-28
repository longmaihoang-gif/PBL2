# TỔNG KẾT TIẾN ĐỘ DỰ ÁN PBL2
## HỆ THỐNG QUẢN LÝ CỬA HÀNG & BÁN HÀNG

---

## 📌 I. Tổng Quan Kiến Trúc & Cấu Trúc Thư Mục

Dự án được phân tách cấu trúc theo mô hình 4 tầng độc lập (4-Tier Architecture), phân định rõ ràng giữa giao diện khai báo (Header `.h`) và hiện thực mã nguồn (Source `.cpp`):

```text
PBL2/
├── Libraries/           # Thư viện cấu trúc dữ liệu, thực thể và dịch vụ nghiệp vụ
│   ├── Base.h           # Lớp cơ sở trừu tượng Entity (GetInfo() thuần ảo)
│   ├── Trade.h          # Quản lý ví tiền (Wallet) & giao dịch
│   ├── Account.h        # Thực thể tài khoản người dùng & phân quyền (Role)
│   ├── Item.h           # Phân cấp kế thừa hàng hóa: Item, ShopItem, FoodItem, ElectronicItem
│   ├── Shop.h           # Quản lý quầy kệ (ShopShelf) và cửa hàng (Shop)
│   ├── HashTable.h      # Khung khai báo Bảng băm mẫu (template HashTable<T>)
│   ├── HashTable.cpp    # Hiện thực chi tiết giải thuật Bảng băm (Separate Chaining + DJB2)
│   ├── DLinkedList.h    # Khung khai báo Nút danh sách liên kết đôi mẫu (template Node<T>)
│   ├── DLinkedList.cpp  # Hiện thực chi tiết thao tác chèn/xóa liên kết đôi
│   ├── DataManager.h    # Khai báo module bóc tách CSV và đồng bộ dữ liệu
│   ├── DataManager.cpp  # Hiện thực chi tiết đọc/ghi CSV chuẩn RFC 4180
│   ├── AccountStorage.h # Tầng 2: Kho trung gian quản lý tài khoản HashTable và CSV
│   ├── AccountStorage.cpp # Hiện thực lưu trữ tài khoản và cơ chế Write-Through
│   ├── AuthManager.h    # Tầng 3: Dịch vụ xác thực tài khoản và quản lý phiên làm việc
│   ├── AuthManager.cpp  # Hiện thực đăng nhập, đăng xuất, phân quyền O(1)
│   ├── ProductStorage.h # Tầng 2: Kho trung gian quản lý sản phẩm HashTable và CSV
│   ├── ProductStorage.cpp # Hiện thực lưu trữ sản phẩm và cơ chế Write-Through
│   ├── ProductService.h # Tầng 3: Dịch vụ nghiệp vụ và thuật toán phân tích hàng hóa
│   ├── ProductService.cpp # Hiện thực tìm kiếm, cảnh báo tồn kho và cận date
│   ├── OrderStorage.h   # Tầng 2: Kho trung gian lưu trữ danh sách đơn hàng và CSV
│   ├── OrderStorage.cpp # Hiện thực lưu trữ lịch sử đơn hàng RAM & CSV
│   ├── OrderManager.h   # Tầng 3: Dịch vụ điều phối thanh toán và phân tích doanh thu
│   ├── OrderManager.cpp # Hiện thực Checkout, sinh hóa đơn và lọc báo cáo
│   ├── Cart.h           # Tầng 3: Thực thể giỏ hàng bán lẻ & cơ chế Hoàn tác (Undo)
│   └── Cart.cpp         # Hiện thực thao tác giỏ hàng, Undo LIFO và xuất phiếu in nhiệt
├── src/                 # Mã nguồn ứng dụng chính
│   ├── Main.cpp         # Điểm khởi chạy kiểm thử toàn diện kiến trúc 4 tầng trên console
│   └── UI.cpp           # Giao diện đồ họa Dear ImGui & GLFW
├── Data/                # Thư mục lưu trữ cơ sở dữ liệu cục bộ (.csv)
│   ├── Accounts.csv     # Cơ sở dữ liệu tài khoản
│   ├── Products.csv     # Cơ sở dữ liệu danh mục hàng hóa
│   └── Orders.csv       # Cơ sở dữ liệu lịch sử đơn hàng
├── Invoices/            # Thư mục lưu trữ phiếu in hóa đơn bán lẻ (.txt) in nhiệt
├── build.bat            # Kịch bản tự động biên dịch và khởi chạy dự án
├── Idea.md              # Tài liệu phân tích yêu cầu & thiết kế tính năng
└── Walkthrough.md       # Báo cáo tổng kết tiến độ và kiến trúc kỹ thuật
```

---

## 🏛️ II. Mô Hình Kiến Trúc 4 Tầng (4-Tier Architecture)

Dự án áp dụng mô hình phân lớp rõ ràng nhằm triệt tiêu sự phụ thuộc lẫn nhau, tăng cường tính module hóa và dễ bảo trì:

| Tầng Kiến Trúc | Trách Nhiệm Chính | Các Thành Phần Mã Nguồn Phụ Trách |
| :--- | :--- | :--- |
| **Tầng 4: Giao diện (Presentation Layer)** | Nhận dữ liệu đầu vào từ người dùng và trực quan hóa thông tin lên màn hình. | `src/UI.cpp` (Dear ImGui + GLFW) và `src/Main.cpp` (Console Test Suite) |
| **Tầng 3: Nghiệp vụ & Thuật toán (Business Logic Layer)** | Xử lý các quy tắc nghiệp vụ, thuật toán tìm kiếm, cảnh báo, thanh toán và hoàn tác. | `AuthManager`, `ProductService`, `OrderManager`, `Cart` |
| **Tầng 2: Truy xuất dữ liệu RAM (Data Access Layer)** | Quản lý cấu trúc dữ liệu bộ nhớ trong (HashTable, DLinkedList) và ánh xạ với đĩa cứng. | `AccountStorage`, `ProductStorage`, `OrderStorage`, `HashTable<T>`, `DLinkedList<T>`, `Entity` |
| **Tầng 1: Cơ sở dữ liệu (Database Layer)** | Lưu trữ dữ liệu bền vững dạng tệp tin và bóc tách dữ liệu theo chuẩn RFC 4180. | `DataManager`, `Accounts.csv`, `Products.csv`, `Orders.csv`, thư mục `Invoices/` |

---

## ⚙️ III. Các Module & Cấu Trúc Dữ Liệu Đã Hoàn Thành

### 1. Bảng Băm Tự Cài Đặt 100% (`HashTable`)
- **Mục đích:** Quản lý kho hàng (tra cứu theo `ProductID`) và tài khoản (tra cứu theo `Username`) với thời gian trung bình $O(1)$.
- **Mô hình Template:** Triển khai dưới dạng `template <typename T> class HashTable`, lưu trữ giá trị kiểu `T` tổng quát theo cặp `(key, value)`. Triệt tiêu hoàn toàn sự phụ thuộc vào `std::any`, bảo đảm an toàn kiểu lúc biên dịch.
- **Phương pháp giải quyết đụng độ:** **Separate Chaining** kết hợp **Danh sách liên kết đôi** (`HashNode<T>` gồm `key`, `value` kiểu `T`, con trỏ `next` và `prev`).
- **Giải thuật băm:** Sử dụng thuật toán băm **DJB2** tối ưu cho chuỗi ký tự.
- **Cơ chế Tái băm (Rehash):** Giám sát hệ số tải (ngưỡng 0.75), tự động mở rộng mảng (`2 * capacity + 1`) và tái băm các node cũ sang mảng mới.
- **An toàn bộ nhớ:** Chặn sao chép ngầm (Copy Constructor private) và thu hồi 100% node con trỏ trong Destructor.

---

### 2. Danh Sách Liên Kết Đôi Độc Lập (`DLinkedList / Node`)
- **Mục đích:** Quản lý danh sách các món hàng trong giỏ và làm nền tảng cho tính năng **Hoàn tác (Undo)**.
- **Mô hình Template:** Cấu trúc `Node<T>` độc lập với con trỏ 2 chiều `prev` và `next`, cung cấp hàm `Insert` và `Remove` tự cô lập node an toàn.

---

### 3. Lớp Cơ Sở Trừu Tượng & Hệ Thống Đa Hình (`Entity` & OOP Models)
- **Lớp Trừu Tượng `Entity` (`Base.h`):**
  - Chuyển đổi thành lớp trừu tượng (Abstract Class) thông qua hàm thuần ảo: `virtual std::string GetInfo() const = 0;`.
  - Cung cấp constructor cơ sở khởi tạo tên định danh `Entity(name)` và destructor ảo `virtual ~Entity()` bảo đảm thu hồi bộ nhớ con trỏ đa hình an toàn.
  - Chuẩn hóa loại bỏ trường dữ liệu dư thừa và không dùng `using namespace std;` trong file header.
- **Hiện Thực Đa Hình (Polymorphism Overriding):**
  - `Account`: Ghi đè `GetInfo()` hiển thị thông tin UID, Username, Họ tên, Quyền hạn (Admin/User) và Số dư ví.
  - `ShopItem`: Ghi đè `GetInfo()` hiển thị thông tin Mã sản phẩm, Tên hàng, Phân loại, Giá bán và Tồn kho.
  - `FoodItem`: Ghi đè `GetInfo()` bổ sung Ngày sản xuất (NSX) và Hạn sử dụng (HSD).
  - `ElectronicItem`: Ghi đè `GetInfo()` bổ sung Thời hạn bảo hành và Công suất tiêu thụ.
  - `Cart`: Ghi đè `GetInfo()` hiển thị Tên giỏ hàng, Tổng số loại mặt hàng và Tổng tiền tạm tính.

---

### 4. Module Quản Lý Dữ Liệu Bền Vững (`DataManager`)
- **Mục đích:** Xử lý đọc/ghi cơ sở dữ liệu dạng bảng phẳng `.csv` (`Accounts.csv`, `Products.csv`, `Orders.csv`).
- **Chuẩn hóa RFC 4180:** Xử lý chuỗi có chứa dấu phẩy, ngắt dòng và dấu ngoặc kép an toàn qua bộ đôi `ParseCSVLine` và `EscapeCSV`.
- **Nguyên lý DRY:** Tách biệt hoàn toàn tầng I/O bằng các hàm cơ sở dùng chung: `ReadCSV`, `WriteCSV`, và `AppendCSVRow`.

---

### 5. Module Giỏ Hàng & Cơ Chế Hoàn Tác (`Cart` & `Undo`)
- **Kế thừa Thực thể Hướng đối tượng:** `Cart` kế thừa từ `Entity`, tích hợp sẵn thuộc tính `Name`.
- **Ứng dụng Danh sách liên kết đôi (`Node<CartItem>`):**
  - Quản lý danh sách các món mua bằng con trỏ `head` và `tail`.
  - Cơ chế **Soft Reservation**: Trừ tạm thời tồn kho `QuantityRemaining` trong bộ nhớ khi thêm món vào giỏ, ngăn chặn bán vượt số lượng.
  - Cơ chế **Undo LIFO**: Sử dụng con trỏ `tail` làm đỉnh ngăn xếp. Khi người dùng hoàn tác, hệ thống rút node ở `tail` trong $O(1)$, thu hồi bộ nhớ bằng `delete` và tự động cộng hoàn trả lại số lượng tồn kho vào `HashTable`.
  - Xuất phiếu in hóa đơn bán lẻ dạng văn bản `.txt` mô phỏng máy in nhiệt tại quầy lưu vào thư mục `Invoices/`.

---

### 6. Tầng Quản Lý Lưu Trữ Dữ Liệu RAM & CSV (Tầng 2: Storages)
Gồm 3 kho lưu trữ dữ liệu trung gian (Repository Pattern), đóng vai trò cầu nối đồng bộ giữa RAM và đĩa cứng:
- **`AccountStorage`:** Quản lý `HashTable<Account>`, đồng bộ với `Accounts.csv`, hỗ trợ thêm, xóa, cập nhật mật khẩu và số dư ví.
- **`ProductStorage`:** Quản lý `HashTable<ShopItem*>`, đồng bộ với `Products.csv`, hỗ trợ CRUD sản phẩm thô với thời gian $O(1)$. Đã được thanh lọc hoàn toàn khỏi các thuật toán nghiệp vụ.
- **`OrderStorage`:** Quản lý danh sách `vector<OrderRecord>`, đồng bộ với `Orders.csv`, hỗ trợ thêm đơn hàng mới và tra cứu theo mã đơn.

---

### 7. Tầng Dịch Vụ Nghiệp Vụ & Thuật Toán (Tầng 3: Services & Managers)
Gồm các bộ điều phối nghiệp vụ và thuật toán, nhận con trỏ từ Tầng 2 theo nguyên lý Dependency Injection:
- **`AuthManager`:** Nhận `AccountStorage*`. Xử lý xác thực đăng nhập, đăng xuất, đăng ký mới, kiểm tra quyền và băm mật khẩu.
- **`ProductService`:** Nhận `ProductStorage*`. Thực thi toàn bộ thuật toán tìm kiếm theo tên (`SearchByName`), lọc phân loại (`GetByCategory`), cảnh báo tồn kho thấp `< 5` (`GetLowStockProducts`), và cảnh báo thực phẩm cận date / quá hạn (`GetExpiringFoodProducts`).
- **`OrderManager`:** Nhận `OrderStorage*` và `AccountStorage*`. Thực thi quy trình thanh toán khép kín (`Checkout`), sinh mã đơn hàng, trừ ví tiền, xuất phiếu hóa đơn `.txt` và thống kê phân tích doanh thu (`GetTotalRevenue`, `FilterByDate`, `FilterByCustomer`).

---

### 8. Tích Hợp & Kịch Bản Kiểm Thử Toàn Diện (`src/Main.cpp`)
- Tích hợp kịch bản kiểm thử toàn diện cho cả 4 tầng:
  1. **Kiểm thử Giỏ hàng & Hoàn tác:** Thêm món, vượt tồn kho, hoàn tác 2 lần LIFO, khôi phục tồn kho.
  2. **Kiểm thử Xác thực & Tài khoản:** 12 ca kiểm thử đăng nhập, đăng ký, đổi mật khẩu, phân quyền, kiểm tra tính bền vững CSV.
  3. **Kiểm thử Kho hàng & Dịch vụ Sản phẩm:** 8 ca kiểm thử tra cứu O(1), tìm kiếm tên qua `ProductService`, lọc tồn kho `< 5`, lọc cận date qua `ProductService`, thêm mới và kiểm tra bền vững CSV.
  4. **Kiểm thử Tính Đa Hình của `Entity`:** Đưa `Account*`, `FoodItem*`, `ElectronicItem*`, `Cart*` vào `vector<Entity*>` và duyệt gọi hàm thuần ảo `GetInfo()`.
  5. **Kiểm thử Hóa đơn & Thanh toán:** Kiểm tra nạp `OrderStorage` (Tầng 2), điều phối `OrderManager` (Tầng 3), thực hiện quy trình `Checkout`, trừ ví tiền, sinh file phiếu in nhiệt trong `Invoices/`.

---

## 🎯 IV. Kế Hoạch Tiếp Theo
1. Hoàn thiện giao diện đồ họa ImGui trong `src/UI.cpp`:
   - Màn hình Đăng nhập / Đăng ký kết nối `AuthManager`.
   - Màn hình User: Xem sản phẩm, giỏ hàng bán lẻ (`Cart`), thực hiện thanh toán trừ ví và xem lịch sử đơn hàng cá nhân.
   - Màn hình Admin: Quản lý kho hàng (`ProductStorage`), xem danh sách cảnh báo tồn kho `< 5` và cận date (`ProductService`), xem bảng doanh thu và lịch sử đơn hàng (`OrderManager`).
