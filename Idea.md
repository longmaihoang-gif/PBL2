# HỆ THỐNG QUẢN LÝ CỬA HÀNG & BÁN HÀNG

---

## 📌 I. Tính Năng Chính

### 1. Quản lý Kho hàng (Inventory Management)
- **Thêm sản phẩm:** Hỗ trợ nhập thông tin sản phẩm mới, phân loại rõ ràng theo **Thực phẩm** và **Đồ điện máy**.
- **Cập nhật & Xóa:** Chỉnh sửa thông tin, tăng/giảm số lượng tồn kho hoặc xóa sản phẩm theo `Mã SP`.
- **Tra cứu & Hiển thị:** Tìm kiếm sản phẩm nhanh chóng theo `Mã SP` hoặc `Tên SP`, in danh sách dạng bảng trực quan.

### 2. Bán hàng & Tạo Hóa đơn (Point of Sale - POS)
- **Thêm vào đơn hàng:** Nhập mã sản phẩm và số lượng mua, hệ thống tự động khấu trừ tồn kho.
- **Tính năng Hoàn tác (Undo):** Cho phép nhân viên hủy bỏ thao tác nhập sản phẩm gần nhất khỏi hóa đơn và hoàn trả lại số lượng kho tương ứng.
- **Thanh toán & Xuất hóa đơn:** 
  - Tính tổng tiền (đã áp dụng thuế / giảm giá) và trừ số dư tài khoản khách hàng.
  - Xuất file phiếu in hóa đơn bán lẻ dạng `.txt` mô phỏng hóa đơn in nhiệt cho khách hàng.
  - Tự động ghi nhận thông tin giao dịch vào cơ sở dữ liệu lịch sử `.csv`.

### 3. Thống kê & Báo cáo (Analytics)
- **Cảnh báo tồn kho:** Thống kê các sản phẩm có số lượng tồn kho dưới ngưỡng cảnh báo ($< 5$).
- **Cảnh báo hạn dùng:** Thống kê các mặt hàng thực phẩm có nguy cơ hết hạn sử dụng.
- **Báo cáo doanh thu:** Báo cáo tổng doanh thu và số lượng đơn hàng đã xử lý trong phiên làm việc, lọc báo cáo từ dữ liệu `.csv`.

### 4. Phân quyền quản lý
- **Admin (Quản trị viên):** Có toàn quyền thêm / sửa / xóa sản phẩm, nạp tiền / phân quyền tài khoản và xem báo cáo doanh thu.
- **Nhân viên / User:** Chỉ được cấp quyền thực hiện các nghiệp vụ bán hàng, mua sắm và tra cứu cá nhân.

### 5. Lưu trữ Dữ liệu Cục bộ & Lịch sử Giao dịch
- **Cơ sở dữ liệu cục bộ (.csv):** Toàn bộ dữ liệu hệ thống (Tài khoản, Kho hàng, Lịch sử hóa đơn) được lưu trữ dưới dạng các tệp `.csv`, dễ dàng bóc tách (parse) trong C++ và tương thích mở trực tiếp bằng Microsoft Excel.
- **Truy xuất & Đối soát:** Hỗ trợ đọc tệp `.csv` để truy xuất lịch sử đơn hàng theo khoảng thời gian, phục vụ thống kê doanh số.

### 6. Cấu trúc dữ liệu & Định dạng Lưu trữ
| Cấu trúc / Định dạng | Mục đích sử dụng | Chi tiết áp dụng |
| :--- | :--- | :--- |
| **Hash Table** | Quản lý kho hàng & Tài khoản | Tìm kiếm / truy xuất theo `MaSP`, quản lý tài khoản thành viên theo `UID` |
| **Vector / Danh sách động** | Giỏ hàng & Xử lý đơn | Lưu trữ tạm thời các mặt hàng trong đơn hàng hiện tại, xử lý hóa đơn |
| **Struct / Class (OOP)** | Phân loại & Mô hình hóa | Kế thừa `Entity` định nghĩa Sản phẩm (Thực phẩm, Điện máy), Tài khoản (`Account`) |
| **File CSV (`.csv`)** | Cơ sở dữ liệu cục bộ | Quản lý `Accounts.csv`, `Products.csv`, `Orders.csv` (dễ mở bằng Excel) |
| **File TXT (`.txt`)** | Hóa đơn bán lẻ (Receipt) | Xuất phiếu thanh toán định dạng trực quan cho khách hàng lưu trữ |

---

## 👥 II. Phân Quyền Hệ Thống

### 1. Quyền Admin (Quản trị viên)

#### 📦 Quản lý Kho hàng
- Thêm sản phẩm mới (phân loại **Thực phẩm** / **Đồ điện máy**).
- Cập nhật thông tin chi tiết, giá bán và điều chỉnh số lượng tồn kho.
- Xóa sản phẩm khỏi hệ thống dựa theo `Mã sản phẩm`.
- Đồng bộ tự động các thay đổi vào tệp dữ liệu `Products.csv`.

#### 📊 Thống kê & Phân tích
- Xem báo cáo tổng doanh thu và tổng số lượng đơn hàng theo ca / ngày / tháng từ `Orders.csv`.
- Xem danh sách các sản phẩm sắp hết hàng (tồn kho $< 5$) để chủ động lập kế hoạch nhập hàng.
- Theo dõi danh sách thực phẩm sắp hết hạn sử dụng (hàng cận date).

#### ⚙️ Quản lý Giao dịch & Hệ thống
- Tra cứu, lọc lịch sử toàn bộ hóa đơn của cửa hàng theo mốc thời gian.
- Quản lý, phân quyền, cấp tài khoản và nạp số dư (`Money`) cho nhân viên / khách hàng trong `Accounts.csv`.

---

### 2. Quyền User / Khách mua hàng

#### 🔍 Tra cứu & Xem sản phẩm
- Xem danh mục sản phẩm (**Thực phẩm**, **Đồ điện máy**) kèm giá bán và tình trạng còn hàng.
- Tìm kiếm nhanh sản phẩm theo `Tên` hoặc `Mã sản phẩm`.
- Xem thông tin chi tiết:
  - **Thực phẩm:** Hạn sử dụng, ngày sản xuất.
  - **Đồ điện máy:** Thời gian bảo hành, công suất tiêu thụ.

#### 🛒 Mua hàng & Giỏ hàng
- Chọn sản phẩm và số lượng muốn mua để thêm vào giỏ hàng cá nhân.
- Sử dụng tính năng **Hoàn tác (Undo)** để xóa món vừa chọn nhầm hoặc thay đổi số lượng trước khi chốt đơn.

#### 💳 Thanh toán & Hóa đơn
- Xác nhận thanh toán đơn hàng (áp dụng mã giảm giá / thuế nếu có, trừ số dư `Money` nếu thanh toán qua tài khoản).
- Nhận phiếu in hóa đơn bán lẻ dạng file `.txt` chi tiết (danh sách món, đơn giá, số lượng, tổng tiền).
- Lưu vết đơn hàng vào lịch sử cá nhân trong hệ thống để tra cứu lại.
