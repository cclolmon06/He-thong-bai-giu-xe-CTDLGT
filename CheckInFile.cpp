#include "ParkingSystem.h"
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;
static bool stripAndUpper(const string& in, string& out) {
    out.clear();
    for (size_t i = 0; i < in.size(); i++) {
        unsigned char ch = static_cast<unsigned char>(in[i]);
        if (isspace(ch) || ch == '-' || ch == '.') continue;
        if (!isalnum(ch)) return false;
        out += static_cast<char>(toupper(ch));
    }
    return true;
}

static bool splitPlate(const string& raw, int& province, string& series,
                       string& number, int& type) {
    string s;
    if (!stripAndUpper(raw, s)) return false;
    if (s.size() != 8 && s.size() != 9) return false;

    if (!isdigit(static_cast<unsigned char>(s[0])) ||
        !isdigit(static_cast<unsigned char>(s[1]))) return false;
    province = (s[0] - '0') * 10 + (s[1] - '0');

    size_t seriesLen = s.size() - 7;         
    series = s.substr(2, seriesLen);
    number = s.substr(2 + seriesLen);         

    for (size_t i = 0; i < number.size(); i++)
        if (!isdigit(static_cast<unsigned char>(number[i]))) return false;
    if (number == "00000") return false;      

    if (!isalpha(static_cast<unsigned char>(series[0]))) return false;
    if (seriesLen == 1) {
        type = TYPE_CAR;
    } else {
        char c = series[1];
        bool ok = isalpha(static_cast<unsigned char>(c)) || (c >= '1' && c <= '9');
        if (!ok) return false;
        type = TYPE_MOTORBIKE;
    }
    return true;
}

bool ParkingLot::isValidVNProvinceCode(int code) const {
    return code >= 11 && code <= 99;
}

bool ParkingLot::validateAndNormalizeVNPlate(string& plate, int expectedType) const {
    int province, type;
    string series, number;
    if (!splitPlate(plate, province, series, number, type)) return false;
    if (!isValidVNProvinceCode(province)) return false;
    if (expectedType != TYPE_AUTO_DETECT && expectedType != type) return false;

    string norm;
    norm += static_cast<char>('0' + province / 10);
    norm += static_cast<char>('0' + province % 10);
    norm += series;
    norm += '-';
    norm += number.substr(0, 3);
    norm += '.';
    norm += number.substr(3, 2);
    plate = norm;
    return true;
}

bool ParkingLot::parkVehicle(string plate, int type, time_t customEntryTime) {
    if (type != TYPE_AUTO_DETECT && type != TYPE_MOTORBIKE && type != TYPE_CAR) {
        cout << "[LỖI] Loại xe không hợp lệ (0: tự nhận dạng, 1: xe máy, 2: ô tô).\n";
        return false;
    }
    if (!validateAndNormalizeVNPlate(plate, type)) {
        cout << "[LỖI] Biển số không hợp lệ hoặc không khớp loại xe.\n";
        return false;
    }
    if (type == TYPE_AUTO_DETECT) {          
        int province;
        string series, number;
        splitPlate(plate, province, series, number, type);
    }

    if (searchVehicle(plate) != nullptr) {
        cout << "[LỖI] Xe " << plate << " đã có trong bãi.\n";
        return false;
    }

    char r;
    int c;
    if (!allocateNearestSlot(r, c)) {
        cout << "[LỖI] Bãi xe đã đầy.\n";
        return false;
    }

    Vehicle v;
    v.licensePlate = plate;
    v.vehicleType = type;
    v.assignedRow = r;
    v.assignedCol = c;
    v.entryTime = (customEntryTime != 0) ? customEntryTime : time(nullptr);

    if (!insertVehicleToHash(v)) {
        releaseSlot(r, c);                   
        cout << "[LỖI] Không lưu được thông tin xe.\n";
        return false;
    }

    char buf[64];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", localtime(&v.entryTime));
    cout << "===== VÉ GIỮ XE =====\n"
         << "Biển số : " << v.licensePlate << "\n"
         << "Loại xe : " << (type == TYPE_MOTORBIKE ? "Xe máy" : "Ô tô") << "\n"
         << "Vị trí  : " << r << c << "\n"
         << "Giờ vào : " << buf << "\n"
         << "=====================\n";
    return true;
}

bool ParkingLot::saveStateToFile(const string& filename) const {
    ofstream out(filename.c_str());
    if (!out) {
        cerr << "[LỖI] Không mở được file " << filename << " để ghi.\n";
        return false;
    }

    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++)
        for (HashNode* p = hashTable[i]; p != nullptr; p = p->next) count++;

    out << numRows << " " << numCols << "\n" << count << "\n";
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (HashNode* p = hashTable[i]; p != nullptr; p = p->next) {
            const Vehicle& v = p->data;
            out << v.licensePlate << " " << v.vehicleType << " "
                << v.assignedRow << " " << v.assignedCol << " "
                << static_cast<long long>(v.entryTime) << "\n";
        }
    }
    return static_cast<bool>(out);
}

bool ParkingLot::loadStateFromFile(const string& filename) {
    ifstream in(filename.c_str());
    if (!in) return false;

    int n, m, cnt;
    if (!(in >> n >> m >> cnt) || n < 1 || n > 26 || m < 1 || cnt < 0 ||
        static_cast<long long>(n) * m > 1000000) {
        cerr << "[LỖI] File " << filename << " sai định dạng.\n";
        return false;
    }

    resetLot(n, m);                                   
    vector<char> used(static_cast<size_t>(n) * m, 0); 
    int loaded = 0, skipped = 0;

    for (int i = 0; i < cnt; i++) {
        string plate;
        int type, col;
        char row;
        long long t;
        if (!(in >> plate >> type >> row >> col >> t)) break;   

        if ((type != TYPE_MOTORBIKE && type != TYPE_CAR) ||
            !validateAndNormalizeVNPlate(plate, type) ||
            row < 'A' || row >= static_cast<char>('A' + n) || col < 1 || col > m) {
            skipped++;
            continue;
        }
        size_t idx = static_cast<size_t>(row - 'A') * m + (col - 1);
        if (used[idx] || searchVehicle(plate) != nullptr) {      
            skipped++;
            continue;
        }

        Vehicle v;
        v.licensePlate = plate;
        v.vehicleType = type;
        v.assignedRow = row;
        v.assignedCol = col;
        v.entryTime = static_cast<time_t>(t);
        if (!insertVehicleToHash(v)) { skipped++; continue; }

        if (!occupySpecificSlot(row, col)) {                     
            Vehicle bo;
            removeVehicleFromHash(plate, bo);
            skipped++;
            continue;
        }
        used[idx] = 1;
        loaded++;
    }

    cout << "[INFO] Đã nạp " << loaded << " xe từ file " << filename << ".\n";
    if (skipped > 0) cout << "[CẢNH BÁO] Bỏ qua " << skipped << " dòng lỗi hoặc trùng.\n";
    return true;
}
