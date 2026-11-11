# Hệ thống Quản lý Bãi giữ xe (C++)

Bài tập dự án học phần **Cấu trúc dữ liệu và Giải thuật** – Đề tài số 06.

Chương trình mô phỏng nghiệp vụ quản lý bãi giữ xe gồm `n` hàng (`A`–`Z`) và `m` vị trí mỗi hàng (`1`–`m`), áp dụng quy chuẩn biển số xe cơ giới Việt Nam (Thông tư 24/2023 & 79/2024/TT-BCA) và biểu phí tính cước theo giờ/qua đêm (VNĐ).

## 1. Cấu trúc dữ liệu & Giải thuật sử dụng

* **Danh sách liên kết đơn sắp xếp (`Sorted Singly Linked List`):** Quản lý các vị trí đỗ còn trống theo thứ tự tăng dần từ `A1` đến `Zm`. Cấp chỗ trống gần cổng nhất ở đầu danh sách với độ phức tạp $O(1)$ và chèn thu hồi chỗ đúng thứ tự từ điển trong $O(K)$.

* **Bảng băm nối kết (`Hash Table` với `Separate Chaining`):** Quản lý phương tiện đang đỗ trong bãi với khóa băm là biển số xe đã chuẩn hóa (`Polynomial Rolling Hash`, `TABLE_SIZE = 1009`), giúp tra cứu và lấy xe đạt độ phức tạp trung bình $O(1)$.

* **Danh sách liên kết đơn (`History Linked List`):** Lưu biên lai lịch sử xe rời bãi và thống kê doanh thu.

## 2. Cấu trúc mã nguồn

```text
├── ParkingSystem.h        # Khai báo struct, hằng số biểu phí VNĐ và class ParkingLot dùng chung
├── SlotManager.cpp        # Quản lý DSLK chỗ trống, cấp/trả ô đỗ và in sơ đồ bãi [ ] / [X]
├── HashTable.cpp          # Hàm băm biển số và các thao tác thêm/tìm/xóa trên Bảng băm
├── CheckInFile.cpp        # Kiểm tra, chuẩn hóa biển số VN, nghiệp vụ gửi xe và đọc/ghi file .txt
├── CheckOutBilling.cpp    # Nghiệp vụ trả xe, tính phí giữ xe (VNĐ) và DSLK lịch sử doanh thu
├── MenuUI.cpp             # Giao diện Menu Console tiếng Việt và liệt kê phương tiện toàn bãi
├── BenchmarkTest.cpp      # Sinh dữ liệu biển số VN tự động, test biên và đo hiệu năng <chrono>
└── main.cpp               # Điểm khởi chạy chương trình
```

## 3. Phân công thành viên

| STT | Thành viên | GitHub | Module phụ trách |
|-----|-----------|--------|-----------------|
| 1 | (Nhóm trưởng) | [@cclolmon06](https://github.com/cclolmon06) | ParkingSystem.h, SlotManager.cpp |
| 2 | Nguyễn Đức Hoàng | [@nguyenduchoang9a777-glitch](https://github.com/nguyenduchoang9a777-glitch) | HashTable.cpp |
| 3 | Nguyễn Xuân Quý | [@quyx931-crypto](https://github.com/quyx931-crypto) | CheckInFile.cpp |
| 4 | Khánh Nguyên | [@nguyenndk6377-ux](https://github.com/nguyenndk6377-ux) | CheckOutBilling.cpp |
| 5 | Nguyễn Trường | [@tn2013199-debug](https://github.com/tn2013199-debug) | MenuUI.cpp, main.cpp |
| 6 | Nb Toàn | [@toan497sudien](https://github.com/toan497sudien) | BenchmarkTest.cpp |

## 4. Hướng dẫn biên dịch & Chạy chương trình

Yêu cầu trình biên dịch hỗ trợ chuẩn C++17 trở lên (g++ / MinGW trên Windows hoặc GCC trên Linux/macOS).

```bash
# Biên dịch toàn bộ project
g++ -std=c++17 SlotManager.cpp HashTable.cpp CheckInFile.cpp CheckOutBilling.cpp MenuUI.cpp BenchmarkTest.cpp main.cpp -o parking_app

# Chạy chương trình
./parking_app
```

> **Lưu ý cho các thành viên trong nhóm:** Trước khi push code mới, chạy `git pull origin main` để đồng bộ. Không tự ý thay đổi chữ ký hàm trong `ParkingSystem.h` khi chưa thống nhất với nhóm.
