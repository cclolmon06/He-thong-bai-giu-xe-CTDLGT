#include "ParkingSystem.h"
#include <iostream>
#include <ctime>
#include <string>

using namespace std;

long long ParkingLot::calculateFeeVND(
    int type,
    std::time_t entry,
    std::time_t exit
) const {
    double seconds = difftime(exit, entry);

    if (seconds < 0) {
        return -1;
    }

    long long hours = static_cast<long long>(seconds / 3600);

    if (hours * 3600 < seconds) {
        hours++;
    }

    if (hours == 0) {
        hours = 1;
    }

    if (type == TYPE_MOTORBIKE) {
        if (hours <= 4) {
            return MOTOR_FIRST_4H_FEE;
        }

        return MOTOR_FIRST_4H_FEE
             + (hours - 4) * MOTOR_NEXT_HOUR_FEE;
    }

    if (type == TYPE_CAR) {
        if (hours <= 2) {
            return CAR_FIRST_2H_FEE;
        }

        return CAR_FIRST_2H_FEE
             + (hours - 2) * CAR_NEXT_HOUR_FEE;
    }

    return -1;
}

string ParkingLot::formatCurrencyVND(long long amount) const {
    string result = to_string(amount);

    for (int i = static_cast<int>(result.length()) - 3; i > 0; i -= 3) {
        result.insert(i, ".");
    }

    return result + " VND";
}

bool ParkingLot::retrieveVehicle(
    string plate,
    time_t customExitTime
) {
    // Kiem tra bien so xe
    if (!validateAndNormalizeVNPlate(plate)) {
        cout << "Bien so xe khong hop le!\n";
        return false;
    }

    // Tim xe trong bai
    HashNode* found = searchVehicle(plate);

    if (found == nullptr) {
        cout << "Khong tim thay xe trong bai!\n";
        return false;
    }

    // Lay thoi gian tra xe
    time_t exitTime;

    if (customExitTime == 0) {
        exitTime = time(nullptr);
    } else {
        exitTime = customExitTime;
    }

    // Kiem tra thoi gian hop le
    if (exitTime < found->data.entryTime) {
        cout << "Thoi gian tra xe khong hop le!\n";
        return false;
    }

    // Tinh phi gui xe
    long long fee = calculateFeeVND(
        found->data.vehicleType,
        found->data.entryTime,
        exitTime
    );

    if (fee < 0) {
        cout << "Khong the tinh phi gui xe!\n";
        return false;
    }

    // Xoa xe khoi bang bam
    Vehicle removed;

    if (!removeVehicleFromHash(plate, removed)) {
        cout << "Khong the xoa xe khoi bai!\n";
        return false;
    }

    // Tra lai cho do
    releaseSlot(removed.assignedRow, removed.assignedCol);

    // Luu lich su
    recordHistory(removed, exitTime, fee);

    // In hoa don
    cout << "\n===== HOA DON GUI XE =====\n";
    cout << "Bien so: " << removed.licensePlate << endl;
    cout << "Loai xe: ";

    if (removed.vehicleType == TYPE_MOTORBIKE) {
        cout << "Xe may\n";
    } else {
        cout << "O to\n";
    }

    cout << "Vi tri: "
         << removed.assignedRow
         << removed.assignedCol << endl;

    cout << "Tien gui xe: "
         << formatCurrencyVND(fee) << endl;

    cout << "=========================\n";

    return true;
}

void ParkingLot::recordHistory(
    const Vehicle& v,
    time_t exitTime,
    long long feeVND
) {
    // Tao node moi de luu lich su
    HistoryNode* newNode = new HistoryNode;

    // Luu thong tin giao dich
    newNode->data = v;
    newNode->exitTime = exitTime;
    newNode->feeVND = feeVND;

    // Chen node moi vao dau danh sach
    newNode->next = historyHead;
    historyHead = newNode;
}

void ParkingLot::reportRevenue() const {
    int motorbikeCount = 0;
    int carCount = 0;
    long long totalRevenue = 0;

    HistoryNode* current = historyHead;

    while (current != nullptr) {
        if (current->data.vehicleType == TYPE_MOTORBIKE) {
            motorbikeCount++;
        } else if (current->data.vehicleType == TYPE_CAR) {
            carCount++;
        }

        totalRevenue += current->feeVND;
        current = current->next;
    }
    
    cout << "\n===== BAO CAO DOANH THU =====\n";
    cout << "So luot xe may: " << motorbikeCount << endl;
    cout << "So luot o to: " << carCount << endl;
    cout << "Tong luot xe: " << motorbikeCount + carCount << endl;
    cout << "Tong doanh thu: "
         << formatCurrencyVND(totalRevenue) << endl;
    cout << "============================\n";
}