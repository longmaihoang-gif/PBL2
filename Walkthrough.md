# TỔNG KẾT TIẾN ĐỘ DỰ ÁN PBL2
## HỆ THỐNG QUẢN LÝ CỬA HÀNG & BÁN HÀNG

---

## 📌 I. Tổng Quan Kiến Trúc & Cấu Trúc Thư Mục

Dự án được phân tách cấu trúc rõ ràng theo chuẩn mô hình phát triển C++ hiện đại, phân định độc lập giữa giao diện khai báo (Header `.h`) và hiện thực mã nguồn (Source `.cpp`):

```text
PBL2/
├── Libraries/           # Thư viện cấu trúc dữ liệu và thực thể dùng chung
│   ├── Base.h           # Lớp cơ sở Entity cho mô hình hướng đối tượng
│   ├── Trade.h          # Quản lý ví tiền (Wallet) & giao dịch
│   ├── Account.h        # Thực thể tài khoản người dùng & phân quyền (Role)
│   ├── Item.h           # Phân cấp kế thừa hàng hóa: Item, ShopItem, FoodItem, ElectronicItem
│   ├── Shop.h           # Quản lý quầy kệ (ShopShelf) và cửa hàng (Shop)
│   ├── HashTable.h      # Khung khai báo Bảng băm (Hash Table)
│   ├── Hashtable.cpp    # Hiện thực chi tiết 100% giải thuật Bảng băm
│   ├── DLinkedList.h    # Khung khai báo Nút danh sách liên kết đôi (Node)
│   ├── DLinkedList.cpp  # Hiện thực chi tiết thao tác chèn/xóa liên kết đôi
│   ├── DataManager.h    # Khai báo module bóc tách CSV và đồng bộ dữ liệu
│   ├── DataManager.cpp  # Hiện thực chi tiết đọc/ghi CSV chuẩn RFC 4180
│   ├── AccountStorage.h      # Khai báo kho trung gian đồng bộ HashTable và CSV
│   ├── AccountStorage.cpp    # Hiện thực quản lý dữ liệu tài khoản và Write-Through
│   ├── AuthManager.h         # Khai báo bộ điều phối xác thực và phiên làm việc
│   ├── AuthManager.cpp       # Hiện thực đăng nhập, đăng xuất, phân quyền O(1)
│   ├── ProductStorage.h      # Khai báo kho trung gian quản lý sản phẩm và đồng bộ CSV
│   ├── ProductStorage.cpp    # Hiện thực quản lý kho hàng, cảnh báo tồn kho và cận date
│   ├── OrderManager.h        # Khai báo bộ điều phối thanh toán và phân tích doanh thu
│   └── OrderManager.cpp      # Hiện thực Checkout, sinh mã hóa đơn và lọc báo cáo
├── src/                 # Mã nguồn ứng dụng chính
│   ├── Main.cpp         # Điểm khởi chạy chương trình kiểm thử console
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

## ⚙️ II. Các Module & Cấu Trúc Dữ Liệu Đã Hoàn Thành

### 1. Bảng Băm Tự Cài Đặt 100% (`HashTable`)
- **Mục đích:** Quản lý kho hàng (tra cứu theo `MaSP`) và tài khoản (tra cứu theo `UID` / `Username`) với độ phức tạp O(1).
- **Mô hình Template:** Triển khai dưới dạng `template <typename T> class HashTable`, lưu trữ giá trị kiểu `T` tổng quát theo cặp `(key, value)`. Triệt tiêu hoàn toàn sự phụ thuộc vào `std::any`, đảm bảo an toàn kiểu tại thời điểm biên dịch (Compile-time Type Safety).
- **Phương pháp giải quyết đụng độ:** **Separate Chaining** kết hợp **Danh sách liên kết đôi** (`HashNode<T>` gồm `key`, `value` kiểu `T`, con trỏ `next` và `prev`).
- **Giải thuật băm:** Sử dụng thuật toán băm **DJB2** tối ưu với kiểu `unsigned char` đảm bảo an toàn phân tán chuỗi ký tự.
- **Cơ chế Tái băm (Rehash):**
  - Giám sát hệ số tải (Load Factor threshold mặc định 0.75).
  - Tự động mở rộng dung lượng mảng bucket (2 * capacity + 1).
  - Tái sử dụng trực tiếp các node cũ sang mảng mới, tối ưu hóa bộ nhớ và tốc độ CPU.
- **An toàn bộ nhớ & Nguyên tắc thiết kế:**
  - Khai báo private Copy Constructor và Copy Assignment Operator nhằm triệt tiêu hoàn toàn nguy cơ **Double-Free**.
  - Destructor thu hồi toàn bộ các node con trỏ, không gây rò rỉ bộ nhớ (Memory Leak).
  - Cung cấp các phương thức cốt lõi: `Insert(key, value)`, `Search(key)`, `Remove(key)`, `Contains(key)`, `Clear()`, `GetAllValues()`.

---

### 2. Danh Sách Liên Kết Đôi Độc Lập (`DLinkedList / Node`)
- **Mục đích:** Quản lý giỏ hàng mua sắm và hỗ trợ tính năng **Hoàn tác (Undo)** đơn hàng linh hoạt.
- **Mô hình Template:** Cấu trúc `Node<T>` độc lập, gọn nhẹ:
  - Thành viên: `T value`, con trỏ `Node<T>* prev` (nút trước) và `Node<T>* next` (nút sau).
  - Constructor đa năng với tham số mặc định, hỗ trợ khởi tạo từ 0 đến 3 tham số.
  - `Insert(Node<T>* target)`: Chèn một node mục tiêu vào ngay sau node hiện tại, cập nhật liên kết 2 chiều.
  - `Remove()`: Tự cô lập và ngắt liên kết của node ra khỏi chuỗi một cách an toàn.
  - Tương thích trực quan với mọi kiểu dữ liệu (như `Node<CartItem>`), loại bỏ chi phí ép kiểu động lúc chạy.

---

### 3. Hệ Thống Thực Thể Hướng Đối Tượng (OOP Entities)
- **`Base.h` (`Entity`):** Lớp thực thể cơ sở cung cấp `Name`, danh sách nhãn `Tags` và Destructor ảo (`virtual ~Entity()`) bảo đảm an toàn thu hồi bộ nhớ đa hình.
- **`Trade.h` (`Wallet`):** Thực thể ví tiền kế thừa từ `Entity`, cung cấp đầy đủ các nghiệp vụ tài chính: `AddMoney`, `CostMoney` (kiểm tra số dư), `TransferMoney` (chuyển tiền giữa các ví).
- **`Account.h` (`Account`):** Quản lý tài khoản kế thừa từ `Entity`, tích hợp sẵn `Wallet`, phân cấp quyền hạn qua `Role` (`Admin`, `User`), cơ chế cấp phát tự động `GUID` hoặc nạp định danh từ cơ sở dữ liệu.
- **`Shop.h` (`Item`, `ShopItem`, `ShopShelf`, `Shop`):** Mô hình hóa cấu trúc bán lẻ theo phân tầng: Vật phẩm đơn lẻ $\rightarrow$ Món hàng bày bán $\rightarrow$ Kệ hàng $\rightarrow$ Cửa hàng.

---

### 4. Module Quản Lý Dữ Liệu Bền Vững (`DataManager`)
- **Mục đích:** Xử lý đọc/ghi cơ sở dữ liệu dạng bảng phẳng `.csv` (`Accounts.csv`, `Products.csv`, `Orders.csv`).
- **Chuẩn hóa RFC 4180:** Xử lý chuỗi có chứa dấu phẩy, ngắt dòng và dấu ngoặc kép an toàn qua bộ đôi `ParseCSVLine` và `EscapeCSV`.
- **Kiến trúc phân tầng & Nguyên lý DRY:** 
  - Tách biệt hoàn toàn tầng I/O bằng các hàm cơ sở dùng chung: `ReadCSV` (đọc file thành ma trận 2 chiều `vector<vector<string>>`), `WriteCSV` (ghi ma trận ra file và tự động `EscapeCSV`), và `AppendCSVRow` (ghi nối thêm 1 dòng đơn).
  - Các hàm nghiệp vụ (`LoadAccounts`, `SaveAccounts`, `LoadProducts`, `SaveProducts`, `LoadOrders`, `AppendOrder`) tái sử dụng 100% các hàm cơ sở này, triệt tiêu hoàn toàn sự lặp lại của các thao tác đóng/mở file và xử lý ký tự.
- **Quản lý đa hình thân thiện sinh viên:** Sử dụng con trỏ thô lớp cơ sở `ShopItem*` kết hợp cấp phát động `new FoodItem(...)` / `new ElectronicItem(...)` và ép kiểu an toàn `dynamic_cast`. Thu hồi bộ nhớ rõ ràng bằng `delete` ở cuối chương trình, không lạm dụng các thư viện thông minh phức tạp để đảm bảo tính tường minh khi bảo vệ vấn đáp.
- **Tích hợp nạp tự động:** Nạp dữ liệu tài khoản và hàng hóa đa hình trực tiếp vào `HashTable` với thời gian tra cứu $O(1)$.

---

### 5. Module Giỏ Hàng & Cơ Chế Hoàn Tác (`Cart` & `Undo`)
- **Kế thừa Thực thể Hướng đối tượng:** `Cart` kế thừa từ `Entity`, tích hợp sẵn thuộc tính `Name` (định danh giỏ hàng/khách hàng) và thẻ trạng thái `Tags`.
- **Ứng dụng Danh sách liên kết đôi (`DLinkedList / Node`):**
  - Quản lý danh sách các món mua bằng con trỏ `head` và `tail`.
  - Cơ chế **Soft Reservation**: Tự động trừ tạm thời tồn kho `QuantityRemaining` trong bộ nhớ khi thêm món vào giỏ, ngăn chặn bán vượt số lượng.
  - Cơ chế **Undo LIFO**: Sử dụng con trỏ `tail` làm đỉnh ngăn xếp. Khi người dùng hoàn tác, hệ thống rút node ở `tail` trong $O(1)$, thu hồi bộ nhớ bằng `delete` và tự động cộng hoàn trả lại số lượng tồn kho vào `HashTable`.
  - **Hủy giỏ hàng an toàn (`Clear`):** Hỗ trợ tùy chọn hoàn trả toàn bộ tồn kho cho kho hàng nếu khách hàng hủy đơn trước khi thanh toán.
  - **Tiện ích:** Cung cấp hàm `GetTotalAmount()`, `PrintCart()` định dạng bảng trực quan, và `GetItemsSummary()` phục vụ lưu vết hóa đơn.

---

### 6. Module Kho Lưu Trữ Dữ Liệu Trung Gian (`AccountStorage`)
- **Mục đích:** Đóng vai trò lớp truy xuất dữ liệu trung gian (Repository Pattern), là cầu nối đồng bộ duy nhất giữa Bảng băm `HashTable` trên RAM và tệp tin `Accounts.csv` trên ổ đĩa.
- **Triệt tiêu trùng lặp bộ nhớ:** Không duy trì danh sách `vector` thường trực trong RAM. Chỉ sử dụng duy nhất một bảng băm `accountTable` cho toàn bộ tác nghiệp truy vấn với thời gian trung bình $O(1)$.
- **Cơ chế Ghi xuyên thấu (Write-Through Persistence):**
  - Mọi thao tác thêm tài khoản mới (`AddAccount`), đổi mật khẩu (`UpdatePassword`), cập nhật thông tin (`UpdateAccount`) hoặc xóa tài khoản (`RemoveAccount`) đều được đồng bộ tức thì xuống tệp `Accounts.csv`.
  - Triệt tiêu 100% nguy cơ xung đột dữ liệu giữa bộ nhớ tức thời và lưu trữ vĩnh viễn.
- **Khởi tạo tự động:** Nạp trực tiếp dữ liệu từ file CSV ngay trong Constructor, cung cấp phương thức kiểm tra trạng thái `IsLoaded()` thân thiện và tiện lợi.

---

### 7. Module Xác Thực & Quản Lý Phiên Làm Việc (`AuthManager`)
- **Mục đích:** Điều phối toàn bộ nghiệp vụ xác thực đăng nhập, đăng xuất, đăng ký và phân quyền hệ thống mà không cần quan tâm đến chi tiết đọc/ghi tệp tin hay giải thuật băm.
- **Mã kết quả chuẩn hóa (`enum AuthResult`):** Cung cấp các mã trạng thái rõ ràng (`AUTH_SUCCESS`, `AUTH_ERR_EMPTY_INPUT`, `AUTH_ERR_USER_NOT_FOUND`, `AUTH_ERR_WRONG_PASSWORD`, `AUTH_ERR_USER_ALREADY_EXISTS`, `AUTH_ERR_NOT_LOGGED_IN`) phục vụ thông báo trực quan cho giao diện.
- **Quản lý phiên (Session Management):** Duy trì con trỏ phiên làm việc `Account* currentAccount`, cung cấp các hàm truy vấn `IsLoggedIn()`, `GetCurrentAccount()`, `GetCurrentRole()` để điều hướng giao diện theo quyền `Admin` và `User`.

---

### 8. Module Kho Lưu Trữ Hàng Hóa & Cảnh Báo (`ProductStorage`)
- **Mục đích:** Đóng vai trò lớp Repository quản lý danh mục sản phẩm trên RAM bằng `HashTable<ShopItem*>` và tự động đồng bộ xuống tệp `Data/Products.csv`.
- **Cơ chế Ghi xuyên thấu (Write-Through Persistence):** Mọi thao tác thêm sản phẩm mới (`AddProduct`), sửa thông tin/giá (`UpdateProduct`, `UpdatePrice`), cập nhật tồn kho (`UpdateQuantity`) hoặc xóa sản phẩm (`RemoveProduct`) đều được đồng bộ tức thì xuống đĩa cứng.
- **Quản lý bộ nhớ đa hình an toàn:** `ProductStorage` chịu trách nhiệm thu hồi `delete` toàn bộ con trỏ `ShopItem*` trên Heap khi hủy đối tượng hoặc khi xóa sản phẩm, loại bỏ triệt để nguy cơ rò rỉ bộ nhớ.
- **Bộ lọc & Cảnh báo thông minh:**
  - Tra cứu O(1) theo `ProductID` và tìm kiếm tương đối không phân biệt hoa thường theo `Tên SP` (`SearchByName`).
  - Lọc theo phân loại hàng hóa (`GetByCategory`).
  - Cảnh báo tồn kho thấp dưới ngưỡng quy định (`GetLowStockProducts` với ngưỡng mặc định < 5).
  - Cảnh báo thực phẩm cận date hoặc đã quá hạn (`GetExpiringFoodProducts`), sử dụng thuật toán bóc tách chuỗi ngày tháng sang số nguyên `int` (`substr` + `stoi`) để bảo đảm độ chính xác 100%, không phụ thuộc độ dài năm.

---

### 9. Module Quản Lý Hóa Đơn, Thanh Toán & In Nhiệt (`OrderManager` & `Cart`)
- **Mục đích:** Đóng vai trò lớp điều phối quy trình mua bán (Point of Sale - POS), liên kết giữa giỏ hàng `Cart`, ví tiền người mua `Wallet`, cơ sở dữ liệu `Orders.csv` và hệ thống in phiếu hóa đơn bán lẻ.
- **Quy trình Thanh toán Khép kín (Checkout Flow):**
  - Kiểm tra giỏ hàng và kiểm tra tính hợp lệ của số dư ví tiền người mua (`customer.costBalance`).
  - Tự động đồng bộ số dư ví mới xuống `Data/Accounts.csv` qua `AccountStorage`.
  - Tự động sinh mã hóa đơn theo quy tắc số thứ tự tăng dần (`ORD1001`, `ORD1002`, `ORD1003`...).
  - Ghi nối tiếp thông tin đơn hàng vào `Data/Orders.csv`.
  - Sinh tệp phiếu hóa đơn bán lẻ dạng văn bản `.txt` mô phỏng máy in nhiệt tại quầy lưu vào thư mục `Invoices/HD_ORDxxxx.txt`, sử dụng căn lề chuẩn `<iomanip>`.
  - Làm sạch giỏ hàng `cart.Clear(nullptr)` mà không hoàn trả lại tồn kho vì giao dịch đã hoàn tất.
- **Báo cáo & Phân tích Doanh thu (Analytics):**
  - Tính tổng doanh thu toàn cửa hàng (`GetTotalRevenue`) và đếm tổng số lượng đơn hàng (`GetTotalOrderCount`).
  - Lọc đơn hàng theo khoảng ngày tháng bắt đầu và kết thúc (`FilterByDate`), hỗ trợ xem doanh số theo ngày/tuần/tháng.
  - Lọc lịch sử đơn hàng theo mã định danh khách hàng (`FilterByCustomer`), phục vụ hiển thị lịch sử mua hàng cá nhân của người dùng.

---

### 10. Tích Hợp & Kịch Bản Kiểm Thử Toàn Diện (`src/Main.cpp`)
- Đã liên kết đầy đủ toàn bộ các thư viện nội bộ của dự án.
- Tự động nạp dữ liệu từ thư mục `Data/` vào `HashTable`.
- Tích hợp kịch bản kiểm thử toàn diện:
  1. **Module Giỏ hàng & Hoàn tác:** Thêm món, kiểm tra vượt tồn kho, hoàn tác LIFO 2 lần, thêm món mới sau hoàn tác, hủy giỏ hàng và khôi phục tồn kho.
  2. **Module Xác thực & Kho tài khoản:** 12 ca kiểm thử tự động (đăng nhập rỗng, tài khoản không tồn tại, sai mật khẩu, đăng nhập Admin thành công, đăng xuất, đăng ký mới, chặn trùng tên, đăng nhập tài khoản mới, đổi mật khẩu, kiểm tra tính bền vững từ file CSV, dọn dẹp dữ liệu kiểm thử giữ nguyên bản file `Accounts.csv`).
  3. **Module Kho hàng & Cảnh báo:** 8 ca kiểm thử tự động (tra cứu O(1) theo mã, tìm kiếm theo tên không phân biệt hoa thường, lọc danh mục Food, lọc cảnh báo tồn kho < 5, lọc cảnh báo thực phẩm cận date, thêm mới sản phẩm đa hình và đồng bộ Write-Through, kiểm tra tính bền vững từ file CSV độc lập, dọn dẹp sản phẩm kiểm thử giữ nguyên bản file `Products.csv`).
  4. **Module Hóa đơn & Thanh toán:** 7 ca kiểm thử tự động (nạp lịch sử đơn hàng, tính tổng doanh thu ban đầu, lọc đơn theo ngày và theo khách hàng, thực hiện quy trình Checkout khép kín, kiểm tra trừ ví tiền, kiểm tra tệp hóa đơn in nhiệt `.txt` sinh ra trong `Invoices/`, dọn dẹp đơn kiểm thử bảo toàn `Orders.csv`).

---

## 🎯 III. Kế Hoạch Tiếp Theo
1. Thiết kế giao diện đăng nhập trực tiếp (Màn hình đăng nhập đồ họa trên Dear ImGui kết nối `AuthManager`).
2. Phân luồng tính năng giao diện theo quyền:
   - Giao diện Admin: Quản trị kho hàng (`ProductStorage`), xem cảnh báo tồn kho < 5, xem cảnh báo cận date, xem bảng doanh thu và lịch sử đơn hàng (`OrderManager`).
   - Giao diện User: Xem danh mục sản phẩm theo thẻ/bảng, giỏ hàng bán lẻ (`Cart`), thực hiện thanh toán trừ ví và xem phiếu in nhiệt (`Invoices/`), xem lịch sử mua sắm cá nhân.
