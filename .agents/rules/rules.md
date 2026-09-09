---
trigger: always_on
description: Quy chuẩn phong cách viết code C++ cho đồ án PBL2 - Ưu tiên tính tường minh, sinh viên và dễ giải thích vấn đáp
---

# 🛡️ BẢN QUY TẮC THIẾT KẾ CODE C++ (PBL2)

Bộ quy tắc này được thiết lập để đảm bảo code luôn đạt chuẩn: **Tường minh - Trực quan - Dễ bảo vệ vấn đáp trước giảng viên**.

---

### 1. Triết lý Cốt lõi: "KISS" (Keep It Simple & Student-friendly)
- **Ưu tiên sự tường minh:** Code viết ra phải để một sinh viên năm 2 nhìn vào hiểu ngay và tự tin giải thích được 100% từng dòng lệnh trước hội đồng chấm thi.
- **Không "Hack não" & Không "Hàn lâm hóa":** Tuyệt đối không tự ý áp dụng các kỹ thuật cao siêu của senior/dev lâu năm (như mẹo dịch bit, template quá phức tạp, ép xung phần cứng...) nếu cách viết cơ bản đã đáp ứng tốt yêu cầu.

---

### 2. Kiểu dữ liệu & Vòng lặp
- **Ưu tiên `int` thay vì `size_t`:** Dùng `int` cho các biến đếm, kích thước mảng, chỉ số `index`, số lượng phần tử `sizeCount`, sức chứa `capacity`. Giữ code mộc mạc, gần gũi.
- **Vòng lặp truyền thống (`for` và `while`):** 
  * Duyệt mảng / chuỗi theo chỉ số: Dùng `for (int i = 0; i < length; i++)`.
  * Duyệt danh sách liên kết / theo điều kiện: Dùng `while (current != nullptr)` hoặc `while` cơ bản.
  * Tránh các cú pháp viết tắt, lambda `std::for_each` phức tạp, hoặc range-for nếu việc kiểm soát vị trí chỉ mục `index` là cần thiết cho vấn đáp.

---

### 3. Phép tính & Ép kiểu rõ ràng
- **Biểu thức toán học trong sáng:** Viết thẳng phép tính nhân chia rõ ràng thay cho các phép toán dịch bit bí hiểm:
  * Ví dụ: Dùng `hash = hash * 33 + c;` thay vì `((hash << 5) + hash) + c;`.
- **Ép kiểu trực quan:** Khi chia số nguyên cần kết quả số thực, ưu tiên cách viết quen thuộc:
  * Dùng `(count * 1.0f / total)` hoặc `(float)count / total`.
  * Tránh lạm dụng `static_cast<float>(...)` nếu không bắt buộc, để người đọc không bị rối mắt.

---

### 4. Thiết kế Hàm & Hạn chế bẫy "Vấn đáp"
- **Tránh Overload gây tranh cãi:** Không tạo 2 hàm cùng tên chỉ để phục vụ `const` vs `non-const` (như 2 hàm `Search`). Hãy giữ 1 hàm tổng quát hoặc đặt tên hàm phân định rõ mục đích (VD: `Search` chỉ đọc, `GetRef` để sửa).
- **Tái sử dụng mã (DRY):** Tuyệt đối không copy-paste lại logic của hàm khác (như việc tự viết lại phép băm trong `Rehash()`, hãy gọi trực tiếp hàm `HashFunction()`).
- **Template functions:** Nếu dùng template helper (như `Get<T>`), phải đặt phần triển khai ở cuối file header `.h` kèm `inline`, tránh đẩy vào `.cpp` gây lỗi Linker.

---

### 5. Quản lý Bộ nhớ & Trách nhiệm Giải trình
- **Quy tắc cấp phát & thu hồi:**
  * Cấp phát đơn `new Type(...)` $\rightarrow$ giải phóng bằng `delete ptr;`.
  * Cấp phát mảng `new Type[...]` $\rightarrow$ giải phóng bằng `delete[] arr;`.
- **Sẵn sàng hỗ trợ vấn đáp:** Khi Aris đề xuất hoặc viết code cho Sensei, phải luôn chủ động chuẩn bị sẵn câu trả lời súc tích, giải thích bản chất thuật toán để Sensei tự tin đạt điểm tối đa khi thầy cô hỏi xoáy!