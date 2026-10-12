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
│   └── DataManager.cpp  # Hiện thực chi tiết đọc/ghi CSV chuẩn RFC 4180
├── src/                 # Mã nguồn ứng dụng chính
│   └── Main.cpp         # Điểm khởi chạy chương trình (Entry Point)
├── Data/                # Thư mục lưu trữ cơ sở dữ liệu cục bộ (.csv)
│   ├── Accounts.csv     # Cơ sở dữ liệu tài khoản
│   ├── Products.csv     # Cơ sở dữ liệu danh mục hàng hóa
│   └── Orders.csv       # Cơ sở dữ liệu lịch sử đơn hàng
├── build.bat            # Kịch bản tự động biên dịch và khởi chạy dự án
├── Idea.md              # Tài liệu phân tích yêu cầu & thiết kế tính năng
└── Walkthrough.md       # Báo cáo tổng kết tiến độ và kiến trúc kỹ thuật
```

---

## ⚙️ II. Các Module & Cấu Trúc Dữ Liệu Đã Hoàn Thành

### 1. Bảng Băm Tự Cài Đặt 100% (`HashTable`)
- **Mục đích:** Quản lý kho hàng (tra cứu theo `MaSP`) và tài khoản (tra cứu theo `UID` / `Username`) với độ phức tạp $O(1)$.
- **Phương pháp giải quyết đụng độ:** **Separate Chaining** kết hợp **Danh sách liên kết đôi** (`HashNode` gồm `key`, `value` (`std::any`), con trỏ `next` và `prev`).
- **Giải thuật băm:** Sử dụng thuật toán băm **DJB2** tối ưu với kiểu `unsigned char` đảm bảo an toàn phân tán chuỗi ký tự.
- **Cơ chế Tái băm (Rehash):**
  - Giám sát hệ số tải (Load Factor threshold mặc định $0.75$).
  - Tự động mở rộng dung lượng mảng bucket ($2 \times capacity + 1$).
  - Tái sử dụng trực tiếp các node cũ sang mảng mới, tối ưu hóa bộ nhớ và tốc độ CPU.
- **An toàn bộ nhớ & Nguyên tắc thiết kế:**
  - Áp dụng kỹ thuật cấm sao chép ngầm (`= delete` đối với Copy Constructor và Copy Assignment Operator) nhằm triệt tiêu hoàn toàn nguy cơ **Double-Free**.
  - Destructor thu hồi toàn bộ các node con trỏ, không gây rò rỉ bộ nhớ (Memory Leak).
  - Hỗ trợ hàm tiện ích mẫu `get<T>(key)` ép kiểu an toàn từ `std::any` về con trỏ `T*` (trả về `nullptr` nếu sai kiểu, tránh ngoại lệ ngắt chương trình).

---

### 2. Danh Sách Liên Kết Đôi Độc Lập (`DLinkedList / Node`)
- **Mục đích:** Quản lý giỏ hàng mua sắm và hỗ trợ tính năng **Hoàn tác (Undo)** đơn hàng linh hoạt.
- **Mô hình triển khai:** Cấu trúc `Node` độc lập, gọn nhẹ:
  - Thành viên: `any value`, con trỏ `Node* p` (nút trước) và `Node* n` (nút sau).
  - Constructor đa năng với tham số mặc định, hỗ trợ khởi tạo từ 0 đến 3 tham số.
  - `Insert(Node* target)`: Chèn một node mục tiêu vào ngay sau node hiện tại, cập nhật liên kết 2 chiều.
  - `Remove()`: Tự cô lập và ngắt liên kết của node ra khỏi chuỗi một cách an toàn.
  - Hỗ trợ hàm mẫu `get<T>()` truy xuất và ép kiểu giá trị bên trong node.

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

### 6. Tích Hợp Chương Trình Chính (`src/Main.cpp`)
- Đã liên kết đầy đủ toàn bộ các thư viện nội bộ của dự án.
- Tự động nạp dữ liệu từ thư mục `Data/` vào `HashTable`.
- Tích hợp kịch bản kiểm thử toàn diện cho `Cart` và `Undo` (thêm món, kiểm tra vượt tồn kho, Undo lần 1, Undo lần 2, thêm món mới và giải phóng bộ nhớ).

---

## 🎯 III. Kế Hoạch Tiếp Theo
1. Thiết kế menu giao diện dòng lệnh (CLI) tương tác trực tiếp, phân quyền theo phiên đăng nhập của `Admin` và `User`.
2. Hoàn thiện tính năng thanh toán (trừ số dư ví tiền `Wallet`, chốt đơn vào `Orders.csv`, cập nhật tồn kho `Products.csv`).
3. Xuất hóa đơn bán lẻ dạng file `.txt` mô phỏng hóa đơn in nhiệt tại quầy.
