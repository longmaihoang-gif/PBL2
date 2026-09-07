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
- **Thanh toán & Xuất hóa đơn:** Tính tổng tiền (đã áp dụng thuế / giảm giá), xuất file chi tiết hóa đơn `.txt` để lưu trữ.

### 3. Thống kê & Báo cáo (Analytics)
- **Cảnh báo tồn kho:** Thống kê các sản phẩm có số lượng tồn kho dưới ngưỡng cảnh báo ($< 5$).
- **Cảnh báo hạn dùng:** Thống kê các mặt hàng thực phẩm có nguy cơ hết hạn sử dụng.
- **Báo cáo doanh thu:** Báo cáo tổng doanh thu và số lượng đơn hàng đã xử lý trong phiên làm việc.

### 4. Phân quyền quản lý
- **Admin (Quản trị viên):** Có toàn quyền thêm / sửa / xóa sản phẩm và xem báo cáo doanh thu.
- **Nhân viên / User:** Chỉ được cấp quyền thực hiện các nghiệp vụ bán hàng và tra cứu.

### 5. Lịch sử giao dịch
- Lưu lại tất cả hóa đơn theo ngày, hỗ trợ truy xuất lại theo khoảng thời gian.
- Mở rộng tự nhiên từ tính năng xuất hóa đơn và tận dụng hệ thống File I/O đã xây dựng.

### 6. Cấu trúc dữ liệu & Ứng dụng
| Cấu trúc dữ liệu | Mục đích sử dụng | Chi tiết áp dụng |
| :--- | :--- | :--- |
| **Hash Table** | Quản lý kho hàng & Tài khoản | Tìm kiếm / truy xuất theo `MaSP`, quản lý tài khoản thành viên theo `ID` |
| **Vector / Danh sách động** | Giỏ hàng & Hóa đơn | Lưu trữ tạm thời các mặt hàng trong đơn hàng hiện tại, xuất hóa đơn |
| **Struct / Class (OOP)** | Phân loại & Mô hình hóa | Định nghĩa các thuộc tính sản phẩm theo từng nhóm phân loại (Thực phẩm, Điện máy) |

---

## 👥 II. Phân Quyền Hệ Thống

### 1. Quyền Admin (Quản trị viên)

#### 📦 Quản lý Kho hàng
- Thêm sản phẩm mới (phân loại **Thực phẩm** / **Đồ điện máy**).
- Cập nhật thông tin chi tiết, giá bán và điều chỉnh số lượng tồn kho.
- Xóa sản phẩm khỏi hệ thống dựa theo `Mã sản phẩm`.

#### 📊 Thống kê & Phân tích
- Xem báo cáo tổng doanh thu và tổng số lượng đơn hàng theo ca / ngày / tháng.
- Xem danh sách các sản phẩm sắp hết hàng (tồn kho $< 5$) để chủ động lập kế hoạch nhập hàng.
- Theo dõi danh sách thực phẩm sắp hết hạn sử dụng (hàng cận date).

#### ⚙️ Quản lý Giao dịch & Hệ thống
- Tra cứu, lọc lịch sử toàn bộ hóa đơn của cửa hàng theo mốc thời gian.
- Quản lý, phân quyền và cấp tài khoản cho nhân viên.

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
- Xác nhận thanh toán đơn hàng (áp dụng mã giảm giá / thuế nếu có).
- Nhận hóa đơn điện tử dạng file `.txt` chi tiết (danh sách món, đơn giá, số lượng, tổng tiền).
- Xem lại lịch sử các đơn hàng cá nhân đã từng mua.
