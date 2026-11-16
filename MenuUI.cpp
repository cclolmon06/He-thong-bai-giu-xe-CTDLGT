#include "MenuUI.h"
#include <iomanip>
#include <limits>

using namespace std;

// 1. Hàm nhập số nguyên an toàn chống lặp vô hạn khi nhập chữ vào cin
int getSafeIntInput(const string& prompt, int minVal, int maxVal) {
    int choice;
    while (true) {
        cout << prompt;
        cin >> choice;

        // Kiểm tra nếu người dùng nhập chữ hoặc ký tự đặc biệt
        if (cin.fail()) {
            cin.clear(); // Xóa cờ lỗi (failbit) của cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa sạch bộ đệm đến ký tự xuống dòng
            cout << "  [!] Loi: Vui long chi nhap so nguyen!\n";
        } 
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa phần dư thừa (VD: nhập "12abc")
            if (choice >= minVal && choice <= maxVal) {
                return choice; // Giá trị hợp lệ
            }
            cout << "  [!] Loi: Vui long nhap so trong khoang [" << minVal << " - " << maxVal << "]!\n";
        }
    }
}

// 2. Tra cứu & Liệt kê toàn bãi: Duyệt toàn bộ TABLE_SIZE bucket
void displayAllParkedVehicles(const HashTable& ht) {
    cout << "\n=========================================================================\n";
    cout << "                    DANH SACH XE DANG DO TRONG BAI                       \n";
    cout << "=========================================================================\n";
    cout << left << setw(6)  << "STT"
         << left << setw(15) << "Bien So"
         << left << setw(12) << "Loai Xe"
         << left << setw(12) << "Vi Tri"
         << left << setw(10) << "Bucket"
         << left << setw(22) << "Gio Vao Bai" << "\n";
    cout << "-------------------------------------------------------------------------\n";

    int totalVehicles = 0;
    int rowCount[MAX_ROWS] = {0}; // Mảng đếm số lượng xe theo từng hàng

    // Duyệt qua toàn bộ TABLE_SIZE bucket của bảng băm
    for (int i = 0; i < TABLE_SIZE; ++i) {
        VehicleNode* current = ht.buckets[i];
        while (current != nullptr) {
            totalVehicles++;

            // Thống kê xe theo hàng
            if (current->row >= 0 && current->row < MAX_ROWS) {
                rowCount[current->row]++;
            }

            // Định dạng thời gian đọc được bằng strftime
            char timeBuffer[30];
            struct tm* timeInfo = localtime(&(current->entryTime));
            strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", timeInfo);

            // Tạo chuỗi vị trí Hàng - Cột (VD: Hàng 0 Cột 2 -> A-02)
            string posStr = string(1, 'A' + current->row) + "-" + (current->col < 9 ? "0" : "") + to_string(current->col + 1);

            cout << left << setw(6)  << totalVehicles
                 << left << setw(15) << current->licensePlate
                 << left << setw(12) << current->vehicleType
                 << left << setw(12) << posStr
                 << left << setw(10) << i
                 << left << setw(22) << timeBuffer << "\n";

            current = current->next;
        }
    }

    if (totalVehicles == 0) {
        cout << "                 (Hien tai khong co xe nao trong bai)                    \n";
    }
    cout << "=========================================================================\n";

    // In thống kê số lượng xe theo từng hàng
    cout << ">>> THONG KE SO LUong XE THEO HANG:\n";
    for (int r = 0; r < MAX_ROWS; ++r) {
        cout << "  - Hang " << (char)('A' + r) << ": " << rowCount[r] << " xe\n";
    }
    cout << "  => TONG CONG: " << totalVehicles << " xe dang do.\n";
    cout << "=========================================================================\n";
}

// 3. Thiết kế Menu điều khiển trực quan, phân nhóm rõ ràng
void runMenu(HashTable& ht) {
    int choice = -1;
    do {
        cout << "\n+==============================================================+\n";
        cout << "|         HE THONG QUAN LY BAI DO XE THONG MINH                |\n";
        cout << "+==============================================================+\n";
        cout << "| [NHOM 1: DIEU PHOI VAO / RA]                                 |\n";
        cout << "|   1. Cho xe vao bai (Check-in & Cap vi tri)                  |\n";
        cout << "|   2. Cho xe ra bai (Check-out & Tinh phi)                    |\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| [NHOM 2: SO DO & TRA CUU]                                    |\n";
        cout << "|   3. Xem so do bai do xe (Ma tran 2D trực quan)              |\n";
        cout << "|   4. Tra cuu xe theo bien so (Tim kiem tren Hash Table)      |\n";
        cout << "|   5. Liet ke toan bo xe dang do & Thong ke theo hang (TV5)   |\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| [NHOM 3: QUAN TRI & BAO CAO]                                 |\n";
        cout << "|   6. Xem bao cao doanh thu & Lich su ra/vao                  |\n";
        cout << "|   0. Thoat chuong trinh                                      |\n";
        cout << "+==============================================================+\n";

        choice = getSafeIntInput(">> Nhap lua chon cua ban (0 - 6): ", 0, 6);

        switch (choice) {
            case 1:
                cout << "\n[Thong bao] Goi ham Cho xe vao bai (Ket noi module TV1/TV2)...\n";
                // parkVehicle(ht, ...);
                break;
            case 2:
                cout << "\n[Thong bao] Goi ham Cho xe ra bai (Ket noi module TV3/TV4)...\n";
                // removeVehicle(ht, ...);
                break;
            case 3:
                cout << "\n[Thong bao] Goi ham Hien thi so do ma tran 2D...\n";
                // displayParkingGrid(...);
                break;
            case 4:
                cout << "\n[Thong bao] Goi ham Tim kiem xe theo bien so...\n";
                // searchVehicle(ht, ...);
                break;
            case 5:
                displayAllParkedVehicles(ht);
                break;
            case 6:
                cout << "\n[Thong bao] Goi ham Thong ke doanh thu...\n";
                break;
            case 0:
                cout << "\n[He thong] Dang luu du lieu va thoat chuong trinh. Tam biet!\n";
                break;
        }
    } while (choice != 0);
}