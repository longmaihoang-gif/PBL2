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
│   ├── Shop.h           # Quản lý vật phẩm, kệ hàng và cửa hàng
│   ├── HashTable.h      # Khung khai báo Bảng băm (Hash Table)
│   ├── Hashtable.cpp    # Hiện thực chi tiết 100% giải thuật Bảng băm
│   ├── DLinkedList.h    # Khung khai báo Nút danh sách liên kết đôi (Node)
│   └── DLinkedList.cpp  # Hiện thực chi tiết thao tác chèn/xóa liên kết đôi
├── src/                 # Mã nguồn ứng dụng chính
│   └── Main.cpp         # Điểm khởi chạy chương trình (Entry Point)
├── Data/                # Thư mục lưu trữ cơ sở dữ liệu cục bộ (.csv)
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

### 4. Tích Hợp Chương Trình Chính (`src/Main.cpp`)
- Đã liên kết đầy đủ toàn bộ các thư viện nội bộ của dự án.
- Chuẩn hóa các header C++17 tiêu chuẩn (`<iostream>`, `<string>`, `<vector>`, `<any>`), đảm bảo tính tương thích đa nền tảng (Cross-Platform) với mọi trình biên dịch (GCC/MinGW, MSVC, Clang).
- Chuẩn bị sẵn khung hàm `main()` để triển khai luồng nghiệp vụ và giao diện điều khiển (CLI).

---

## 🎯 III. Kế Hoạch Tiếp Theo
1. Xây dựng module đọc/ghi dữ liệu từ các tệp `.csv` trong thư mục `Data/` (`Accounts.csv`, `Products.csv`, `Orders.csv`).
2. Hiện thực logic giỏ hàng (Cart) và cơ chế Undo đơn hàng sử dụng `DLinkedList`.
3. Thiết kế menu giao diện dòng lệnh (CLI) phân quyền theo phiên đăng nhập của `Admin` và `User`.
4. Hoàn thiện tính năng xuất hóa đơn bán lẻ dạng file `.txt`.
