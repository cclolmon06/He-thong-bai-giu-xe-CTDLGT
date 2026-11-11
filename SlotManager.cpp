#include "ParkingSystem.h"
#include <iomanip>
#include <cctype>

using namespace std;

static inline bool isSlotBefore(char r1, int c1, char r2, int c2) {
    if (r1 != r2) {
        return r1 < r2;
    }
    return c1 < c2;
}

ParkingLot::ParkingLot(int n, int m)
    : numRows(0), numCols(0), emptySlotCount(0),
      freeSlotsHead(nullptr), historyHead(nullptr) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hashTable[i] = nullptr;
    }
    resetLot(n, m);
}

ParkingLot::~ParkingLot() {
    while (freeSlotsHead != nullptr) {
        SlotNode* temp = freeSlotsHead;
        freeSlotsHead = freeSlotsHead->next;
        delete temp;
    }
    emptySlotCount = 0;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* curr = hashTable[i];
        while (curr != nullptr) {
            HashNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        hashTable[i] = nullptr;
    }

    while (historyHead != nullptr) {
        HistoryNode* temp = historyHead;
        historyHead = historyHead->next;
        delete temp;
    }
}

void ParkingLot::resetLot(int n, int m) {
    while (freeSlotsHead != nullptr) {
        SlotNode* temp = freeSlotsHead;
        freeSlotsHead = freeSlotsHead->next;
        delete temp;
    }
    for (int i = 0; i < TABLE_SIZE; ++i) {
        HashNode* curr = hashTable[i];
        while (curr != nullptr) {
            HashNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        hashTable[i] = nullptr;
    }
    while (historyHead != nullptr) {
        HistoryNode* temp = historyHead;
        historyHead = historyHead->next;
        delete temp;
    }

    if (n < 1) {
        cout << "[Cảnh báo] Số hàng tối thiểu là 1. Tự động đặt n = 1.\n";
        n = 1;
    } else if (n > 26) {
        cout << "[Cảnh báo] Số hàng tối đa là 26 (A-Z). Tự động giới hạn n = 26.\n";
        n = 26;
    }
    if (m < 1) {
        cout << "[Cảnh báo] Số vị trí mỗi hàng tối thiểu là 1. Tự động đặt m = 1.\n";
        m = 1;
    }

    numRows = n;
    numCols = m;
    emptySlotCount = 0;
    freeSlotsHead = nullptr;

    SlotNode* tail = nullptr;
    for (int i = 0; i < numRows; ++i) {
        char currentRow = static_cast<char>('A' + i);
        for (int col = 1; col <= numCols; ++col) {
            SlotNode* newNode = new SlotNode{currentRow, col, nullptr};
            if (freeSlotsHead == nullptr) {
                freeSlotsHead = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            ++emptySlotCount;
        }
    }
}

bool ParkingLot::allocateNearestSlot(char& r, int& c) {
    if (freeSlotsHead == nullptr) {
        return false;
    }

    SlotNode* allocatedNode = freeSlotsHead;
    r = allocatedNode->row;
    c = allocatedNode->col;

    freeSlotsHead = freeSlotsHead->next;
    delete allocatedNode;
    --emptySlotCount;

    return true;
}

bool ParkingLot::occupySpecificSlot(char r, int c) {
    r = static_cast<char>(toupper(static_cast<unsigned char>(r)));
    if (r < 'A' || r >= static_cast<char>('A' + numRows) || c < 1 || c > numCols) {
        return false;
    }
    if (freeSlotsHead == nullptr) {
        return false;
    }

    if (freeSlotsHead->row == r && freeSlotsHead->col == c) {
        SlotNode* temp = freeSlotsHead;
        freeSlotsHead = freeSlotsHead->next;
        delete temp;
        --emptySlotCount;
        return true;
    }

    SlotNode* prev = freeSlotsHead;
    SlotNode* curr = freeSlotsHead->next;
    while (curr != nullptr && isSlotBefore(curr->row, curr->col, r, c)) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != nullptr && curr->row == r && curr->col == c) {
        prev->next = curr->next;
        delete curr;
        --emptySlotCount;
        return true;
    }

    return false;
}

void ParkingLot::releaseSlot(char r, int c) {
    r = static_cast<char>(toupper(static_cast<unsigned char>(r)));

    if (r < 'A' || r >= static_cast<char>('A' + numRows) || c < 1 || c > numCols) {
        cerr << "[Lỗi] Vị trí thu hồi " << r << c << " vượt ngoài phạm vi bãi xe!\n";
        return;
    }

    if (freeSlotsHead == nullptr || isSlotBefore(r, c, freeSlotsHead->row, freeSlotsHead->col)) {
        SlotNode* newNode = new SlotNode{r, c, freeSlotsHead};
        freeSlotsHead = newNode;
        ++emptySlotCount;
        return;
    }

    if (freeSlotsHead->row == r && freeSlotsHead->col == c) {
        return;
    }

    SlotNode* curr = freeSlotsHead;
    while (curr->next != nullptr && isSlotBefore(curr->next->row, curr->next->col, r, c)) {
        curr = curr->next;
    }

    if (curr->next != nullptr && curr->next->row == r && curr->next->col == c) {
        return;
    }

    SlotNode* newNode = new SlotNode{r, c, curr->next};
    curr->next = newNode;
    ++emptySlotCount;
}

void ParkingLot::displayEmptySlots() const {
    int total = getTotalCapacity();
    int occupied = total - emptySlotCount;

    cout << "\n================ TRẠNG THÁI CHỖ ĐỖ BÃI GIỮ XE ================\n";
    cout << "• Quy mô bãi xe : " << numRows << " hàng (A-"
         << static_cast<char>('A' + numRows - 1) << ") x "
         << numCols << " vị trí/hàng = " << total << " chỗ\n";
    cout << "• Đang có xe đỗ : " << occupied << " chỗ\n";
    cout << "• Chỗ còn trống : " << emptySlotCount << " chỗ\n";

    if (freeSlotsHead != nullptr) {
        cout << "• Ô cấp tiếp theo: " << freeSlotsHead->row << freeSlotsHead->col
             << " (Gần cổng nhất - O(1))\n";
    } else {
        cout << "• Trạng thái     : BÃI XE ĐÃ ĐẦY HOÀN TOÀN!\n";
    }
    cout << "--------------------------------------------------------------\n";

    if (numCols <= 20) {
        cout << "SƠ ĐỒ BÃI XE (Chú thích: [ ] = Trống, [X] = Đã có xe):\n\n";

        cout << "     ";
        for (int col = 1; col <= numCols; ++col) {
            cout << setw(3) << col << " ";
        }
        cout << "\n";

        SlotNode* scanPtr = freeSlotsHead;
        for (int i = 0; i < numRows; ++i) {
            char rowChar = static_cast<char>('A' + i);
            cout << "  " << rowChar << "  ";
            for (int col = 1; col <= numCols; ++col) {
                if (scanPtr != nullptr && scanPtr->row == rowChar && scanPtr->col == col) {
                    cout << " [ ]";
                    scanPtr = scanPtr->next;
                } else {
                    cout << " [X]";
                }
            }
            cout << "\n";
        }
    } else {
        cout << "THỐNG KÊ CHỖ TRỐNG THEO HÀNG (Bãi kích thước lớn "
             << numRows << "x" << numCols << "):\n";
        SlotNode* scanPtr = freeSlotsHead;
        for (int i = 0; i < numRows; ++i) {
            char rowChar = static_cast<char>('A' + i);
            int rowEmpty = 0;
            while (scanPtr != nullptr && scanPtr->row == rowChar) {
                ++rowEmpty;
                scanPtr = scanPtr->next;
            }
            cout << "  - Hàng " << rowChar << ": trống "
                 << setw(4) << rowEmpty << "/" << numCols << " chỗ\n";
        }
    }

    cout << "--------------------------------------------------------------\n";
    if (freeSlotsHead == nullptr) {
        cout << "Danh sách chỗ trống: (Rỗng)\n";
    } else {
        cout << "Danh sách chỗ trống (theo thứ tự ưu tiên cấp phát):\n  ";
        SlotNode* curr = freeSlotsHead;
        int printed = 0;
        const int MAX_PREVIEW = 40;

        while (curr != nullptr && printed < MAX_PREVIEW) {
            cout << curr->row << curr->col;
            if (curr->next != nullptr) {
                cout << " -> ";
            }
            curr = curr->next;
            ++printed;
            if (printed % 10 == 0 && curr != nullptr && printed < MAX_PREVIEW) {
                cout << "\n  ";
            }
        }
        if (curr != nullptr) {
            cout << " ... (còn " << (emptySlotCount - printed) << " chỗ trống khác)";
        }
        cout << "\n";
    }
    cout << "==============================================================\n";
}

int ParkingLot::getEmptySlotCount() const {
    return emptySlotCount;
}

int ParkingLot::getTotalCapacity() const {
    return numRows * numCols;
}
