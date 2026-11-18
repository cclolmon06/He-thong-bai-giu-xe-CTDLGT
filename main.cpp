#include "ParkingSystem.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Thiết lập bảng mã UTF-8 trên Windows để hiển thị đúng tiếng Việt có dấu
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    // Khởi tạo bãi xe (gọi constructor mặc định của ParkingLot)
    ParkingLot myParkingLot;

    // Chạy Menu điều khiển tổng (TV5)
    myParkingLot.runMenu();

    return 0;
}
