#include "MenuUI.h"

// Hàm khởi tạo bảng băm rỗng (Nếu TV làm Hash Table đã có thì gọi hàm của TV đó)
void initHashTable(HashTable& ht) {
    ht.currentCount = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        ht.buckets[i] = nullptr;
    }
}
// Hàm tạo dữ liệu mẫu để TV5 test tính năng hiển thị trước khi ghép code
void seedSampleData(HashTable& ht) {
    VehicleNode* v1 = new VehicleNode{"59-X3 123.45", "Xe may", 0, 1, time(nullptr) - 3600, nullptr};
    VehicleNode* v2 = new VehicleNode{"51G-888.88", "O to", 1, 4, time(nullptr) - 1800, nullptr};
    VehicleNode* v3 = new VehicleNode{"93-P1 567.89", "Xe may", 0, 5, time(nullptr) - 600, nullptr};

    ht.buckets[12] = v1;
    ht.buckets[45] = v2;
    v1->next = v3; // Giả lập đụng độ (Collision) tại bucket 12
    ht.currentCount = 3;
}

int main() {
    HashTable parkingHash;
    initHashTable(parkingHash);

    // Tạo dữ liệu giả lập để kiểm thử ngay hàm displayAllParkedVehicles()
    seedSampleData(parkingHash);

    // Khởi chạy giao diện điều khiển chính
    runMenu(parkingHash);

    return 0;
}