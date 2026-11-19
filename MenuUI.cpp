#include "ParkingSystem.h"
#include <iomanip>
#include <limits>
#include <iostream>
#include <string>

using namespace std;

// 1. Hàm nhập số nguyên an toàn (chống trôi lệnh khi nhập sai kiểu dữ liệu)
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

// 2. Liệt kê toàn bộ xe đang đỗ trong bãi & Thống kê chi tiết
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
    int motorbikeCount = 0;
    int carCount = 0;
    int rowCount[26] = {0}; // Đếm số xe theo từng hàng (A - Z)

    // Duyệt qua toàn bộ các bucket của Bảng băm (Hash Table)
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* current = hashTable[i];
        while (current != nullptr) {
            totalVehicles++;

            char r = current->data.assignedRow;
            int c = current->data.assignedCol;
            if (r >= 'A' && r <= 'Z') {
                rowCount[r - 'A']++;
            }

            if (current->data.vehicleType == TYPE_CAR) {
                carCount++;
            } else {
                motorbikeCount++;
            }

            char timeBuffer[30];
            struct tm* timeInfo = localtime(&(current->data.entryTime));
            strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", timeInfo);

            // Sửa lỗi lệch cột: giữ nguyên chỉ số cột c (1-indexed)
            string posStr = string(1, r) + (c < 10 ? "0" : "") + to_string(c);
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
        cout << "=========================================================================\n";
        return;
    }

    cout << "=========================================================================\n";
    cout << ">>> THONG KE HIEN TRANG BAI XE:\n";
    cout << "  - Tong so xe dang do : " << totalVehicles << " xe (Xe may: " 
         << motorbikeCount << " | O to: " << carCount << ")\n";
    cout << "  - Phan bo theo hang  : ";
    for (int i = 0; i < 26; ++i) {
        if (rowCount[i] > 0) {
            cout << "[Hang " << static_cast<char>('A' + i) << ": " << rowCount[i] << " xe] ";
        }
    }
    cout << "\n=========================================================================\n";
}

// 3. Menu điều khiển trung tâm
void ParkingLot::runMenu() {
    int choice = -1;
    do {
        cout << "\n+==============================================================+\n";
        cout << "|         HE THONG QUAN LY BAI DO XE THONG MINH (C++)          |\n";
        cout << "+==============================================================+\n";
        cout << "| [NHOM 1: DIEU PHOI VAO / RA]                                 |\n";
        cout << "|   1. Cho xe vao bai (Check-in)                               |\n";
        cout << "|   2. Cho xe ra bai & Tinh phi (Check-out)                    |\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| [NHOM 2: SO DO & TRA CUU]                                    |\n";
        cout << "|   3. Xem so do bai do xe (Danh sach cho trong)               |\n";
        cout << "|   4. Liet ke toan bo xe dang do & Thong ke                   |\n";
        cout << "|   5. Tra cuu nhanh vị tri xe theo bien so                    |\n";
        cout << "|--------------------------------------------------------------|\n";
        cout << "| [NHOM 3: QUAN TRI, BAO CAO & DU LIEU FILE]                   |\n";
        cout << "|   6. Xem bao cao doanh thu & Lich su                         |\n";
        cout << "|   7. Luu trang thai bai xe ra file (.txt)                    |\n";
        cout << "|   8. Doc trang thai bai xe tu file (.txt)                    |\n";
        cout << "|   0. Thoat chuong trinh                                      |\n";
        cout << "+==============================================================+\n";

        choice = getSafeIntInput(">> Nhap lua chon cua ban (0 - 8): ", 0, 8);

        switch (choice) {
            case 1: {
                string plate;
                cout << ">> Nhap bien so xe (VD: 59X1-123.45 hoac 51A-123.45): ";
                getline(cin, plate);
                int type = getSafeIntInput(
                    ">> Loai xe (0: Tu nhan dang, 1: Xe may, 2: O to): ", 0, 2
                );
                parkVehicle(plate, type);
                break;
            }
            case 2: {
                string plate;
                cout << ">> Nhap bien so xe can cho ra: ";
                getline(cin, plate);
                retrieveVehicle(plate);
                break;
            }
            case 3:
                displayEmptySlots();
                break;
            case 4:
                displayAllParkedVehicles();
                break;
            case 5: {
                string plate;
                cout << ">> Nhap bien so xe can tim: ";
                getline(cin, plate);
                if (!validateAndNormalizeVNPlate(plate)) {
                    cout << "  [!] Bien so xe khong hop le!\n";
                } else {
                    HashNode* found = searchVehicle(plate);
                    if (found == nullptr) {
                        cout << "  [!] Khong tim thay xe " << plate << " trong bai!\n";
                    } else {
                        char timeBuf[30];
                        strftime(timeBuf, sizeof(timeBuf), "%d/%m/%Y %H:%M:%S",
                                 localtime(&(found->data.entryTime)));
                        cout << "\n  [TIM THAY XE]\n"
                             << "  - Bien so : " << found->data.licensePlate << "\n"
                             << "  - Loai xe : " << (found->data.vehicleType == TYPE_CAR ? "O to" : "Xe may") << "\n"
                             << "  - Vi tri  : " << found->data.assignedRow << found->data.assignedCol << "\n"
                             << "  - Gio vao : " << timeBuf << "\n";
                    }
                }
                break;
            }
            case 6:
                reportRevenue();
                break;
            case 7: {
                string filename;
                cout << ">> Nhap ten file can luu (nhan Enter de dung 'parking_data.txt'): ";
                getline(cin, filename);
                if (filename.empty()) filename = "parking_data.txt";
                if (saveStateToFile(filename)) {
                    cout << "  [OK] Da luu trang thai bai xe vao file '" << filename << "' thanh cong!\n";
                }
                break;
            }
            case 8: {
                string filename;
                cout << ">> Nhap ten file can nap (nhan Enter de dung 'parking_data.txt'): ";
                getline(cin, filename);
                if (filename.empty()) filename = "parking_data.txt";
                if (!loadStateFromFile(filename)) {
                    cout << "  [!] Khong the mo hoac doc file '" << filename << "'!\n";
                }
                break;
            }
            case 0:
                cout << "\n[He thong] Dang thoat chuong trinh. Tam biet!\n";
                break;
        }
    } while (choice != 0);
}
