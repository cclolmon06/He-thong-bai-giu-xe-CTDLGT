#include "ParkingSystem.h"
#include <iomanip>
#include <limits>
#include <iostream>

using namespace std;

// 1. Hàm nhập an toàn (hàm tự do, giữ nguyên)
int getSafeIntInput(const string& prompt, int minVal, int maxVal) {
    int choice;
    while (true) {
        cout << prompt;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Loi: Vui long chi nhap so nguyen!\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice >= minVal && choice <= maxVal) {
                return choice;
            }
            cout << "  [!] Loi: Vui long nhap so trong khoang [" << minVal << " - " << maxVal << "]!\n";
        }
    }
}

// 2. Tra cứu toàn bãi (đã chuyển thành hàm thành viên của ParkingLot)
void ParkingLot::displayAllParkedVehicles() const {
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
    int rowCount[26] = {0}; // Đếm xe theo hàng (tối đa 26 hàng A-Z)

    // Duyệt qua mảng hashTable nội bộ của class ParkingLot
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = hashTable[i];
        while (current != nullptr) {
            totalVehicles++;
            
            // Lấy thông tin từ struct Vehicle (data)
            char r = current->data.assignedRow;
            int c = current->data.assignedCol;
            if (r >= 'A' && r <= 'Z') rowCount[r - 'A']++;

            char timeBuffer[30];
            struct tm* timeInfo = localtime(&(current->data.entryTime));
            strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", timeInfo);

            string posStr = string(1, r) + "-" + (c < 9 ? "0" : "") + to_string(c + 1);
            string typeStr = (current->data.vehicleType == TYPE_CAR) ? "O to" : "Xe may";

            cout << left << setw(6)  << totalVehicles
                 << left << setw(15) << current->data.licensePlate
                 << left << setw(12) << typeStr
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
    cout << ">>> TONG CONG: " << totalVehicles << " xe dang do.\n";
    cout << "=========================================================================\n";
}

// 3. Menu điều khiển
void ParkingLot::runMenu() {
    int choice = -1;
    do {
        cout << "\n+==============================================================+\n";
        cout << "|         HE THONG QUAN LY BAI DO XE THONG MINH                |\n";
        cout << "+==============================================================+\n";
        cout << "| [NHOM 1: DIEU PHOI VAO / RA]                                 |\n";
        cout << "|   1. Cho xe vao bai (Check-in)                               |\n";
        cout << "|   2. Cho xe ra bai (Check-out)                               |\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| [NHOM 2: SO DO & TRA CUU]                                    |\n";
        cout << "|   3. Xem so do bai do xe (Danh sach cho trong)               |\n";
        cout << "|   4. Liet ke toan bo xe dang do & Thong ke (TV5)             |\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| [NHOM 3: QUAN TRI & BAO CAO]                                 |\n";
        cout << "|   5. Xem bao cao doanh thu & Lich su                         |\n";
        cout << "|   0. Thoat chuong trinh                                      |\n";
        cout << "+==============================================================+\n";

        choice = getSafeIntInput(">> Nhap lua chon cua ban (0 - 5): ", 0, 5);

        switch (choice) {
            case 1: {
                string plate;
                cout << ">> Nhap bien so xe (VD: 59X1-12345): ";
                cin >> plate;
                int type = getSafeIntInput(">> Loai xe (1: Xe may, 2: O to): ", 1, 2);
                parkVehicle(plate, type);
                break;
            }
            case 2: {
                string plate;
                cout << ">> Nhap bien so xe can cho ra: ";
                cin >> plate;
                retrieveVehicle(plate);
                break;
            }
            case 3:
                displayEmptySlots();
                break;
            case 4:
                displayAllParkedVehicles();
                break;
            case 5:
                reportRevenue();
                break;
            case 0:
                cout << "\n[He thong] Dang thoat chuong trinh. Tam biet!\n";
                break;
        }
    } while (choice != 0);
}