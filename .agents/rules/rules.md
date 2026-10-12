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
- **Nguyên tắc trần kiến thức năm 2 (Year-2 Knowledge Ceiling):** Mọi cú pháp, hàm hay thư viện vượt quá phạm vi hai môn học *"Lập trình hướng đối tượng"* và *"Cấu trúc Dữ liệu"* đều bị nghiêm cấm. Nếu cần một chức năng (tìm kiếm, sắp xếp, lọc, băm), phải tự viết bằng vòng lặp và câu lệnh rẽ nhánh cơ bản.

---

### 2. Danh Sách Trắng Thư Viện (Strict Whitelist - Mọi thứ nằm ngoài đều bị CẤM)
Để ngăn ngừa tuyệt đối việc phát sinh các thư viện lạ gây bẫy vấn đáp, toàn bộ dự án tuân thủ cơ chế **Default-Deny** (Mặc định cấm, chỉ được dùng những gì có trong danh sách trắng dưới đây):

1. **Thư viện C++ tiêu chuẩn (STL Whitelist) - CHỈ CHO PHÉP:**
   - `<iostream>`: Nhập/xuất console cơ bản (`cin`, `cout`).
   - `<fstream>`: Đọc/ghi tập tin văn bản (`ifstream`, `ofstream`).
   - `<sstream>`: Tách chuỗi và định dạng chuỗi (`stringstream`).
   - `<string>`: Xử lý chuỗi và hàm ép kiểu cơ bản (`to_string`, `stoi`, `stoll`, `stoull`).
   - `<vector>`: Mảng động phục vụ lưu danh sách tạm hoặc bảng dữ liệu CSV.
   - `<iomanip>`: Căn lề console cơ bản nếu cần hiển thị bảng (`setw`, `setfill`, `left`, `right`).
   - `<cmath>`, `<cstdlib>`, `<ctime>`: Các hàm toán học, sinh số ngẫu nhiên thời gian thực cơ bản.
   - **CẤM TUYỆT ĐỐI MỌI HEADER STL KHÁC:**
     * Cấm `<algorithm>`: Tuyệt đối không dùng `std::for_each`, `std::find_if`, `std::transform`, `std::remove_if`... Mọi thao tác tìm kiếm, lọc, duyệt phải tự viết bằng vòng lặp `for` hoặc `while` cơ bản.
     * Cấm `<memory>`: Không dùng smart pointer (`shared_ptr`, `unique_ptr`).
     * Cấm `<regex>`, `<filesystem>`, `<chrono>`, `<ranges>`, `<functional>`, `<optional>`, `<variant>`, `<tuple>`, `<thread>`.

2. **Cấu trúc Dữ liệu Đồ án (DSA Whitelist):**
   - CẤM các container STL có sẵn thay thế cấu trúc dữ liệu bài tập lớn: Cấm `std::map`, `std::unordered_map`, `std::unordered_set`, `std::list`, `std::stack`, `std::queue`.
   - Các cấu trúc dữ liệu cốt lõi (Bảng băm `HashTable`, Danh sách liên kết đôi `DLinkedList`) bắt buộc phải tự lập trình thủ công bằng con trỏ thuần (`Type*`).

3. **Thư viện bên ngoài (Third-party GUI Whitelist):**
   - Chỉ cho phép đúng 2 thư viện phục vụ đồ họa đã thống nhất: **Dear ImGui** và **GLFW / OpenGL3**.
   - CẤM thêm bất kỳ thư viện bên thứ 3 nào khác (không dùng Boost, nlohmann/json, spdlog, fmt...). Mọi thao tác định dạng CSV hay dữ liệu đều xử lý bằng mã C++ cơ bản tự viết.

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

### 5. Xử lý Tập tin (File I/O) & Tuyệt đối Tránh Cú pháp Lạ (Standard Library Quirks)
- **Chuẩn hóa thao tác Đọc / Ghi:**
  * Đọc file: Dùng `ifstream inFile(path);` và `getline(inFile, line);`, kiểm tra bằng `if (!inFile.is_open())`.
  * Ghi file: Dùng `ofstream outFile(path);` (ghi đè) hoặc `ofstream outFile(path, ios::app);` (ghi nối tiếp).
  * Giả định cấu trúc dữ liệu tệp: Các tệp CSV trong thư mục `Data/` luôn được giả định đã có sẵn dòng tiêu đề (Header). Không tự ý chèn logic kiểm tra tệp rỗng bằng mẹo lạ (`peek()`, `traits_type::eof()`, hoặc mở `ifstream` lồng trong hàm ghi file).
- **Cấm các phương thức chuỗi & STL lạ lùng:**
  * Không dùng `rfind("...", 0) == 0` để kiểm tra tiền tố (Prefix). Ưu tiên so sánh thuộc tính rõ ràng (ví dụ: `category == "Food"`), hoặc viết hàm con/vòng lặp so sánh ký tự tường minh.
  * Không lạm dụng `std::regex` cho các tác vụ tách chuỗi cơ bản; ưu tiên `stringstream` hoặc duyệt mảng ký tự truyền thống.
  * Tuyệt đối không dùng con trỏ thông minh (`std::shared_ptr`, `std::unique_ptr`) trong các module yêu cầu tự quản lý bộ nhớ, chỉ dùng con trỏ cơ bản (`Type*`).

---

### 6. Quản lý Bộ nhớ & Trách nhiệm Giải trình
- **Quy tắc cấp phát & thu hồi:**
  * Cấp phát đơn `new Type(...)` -> giải phóng bằng `delete ptr;`.
  * Cấp phát mảng `new Type[...]` -> giải phóng bằng `delete[] arr;`.
- **Sẵn sàng hỗ trợ vấn đáp:** Khi đề xuất hoặc viết code, phải luôn chủ động chuẩn bị sẵn câu trả lời súc tích, giải thích bản chất thuật toán để sinh viên tự tin hiểu rõ và đạt điểm tối đa khi thầy cô hỏi xoáy!