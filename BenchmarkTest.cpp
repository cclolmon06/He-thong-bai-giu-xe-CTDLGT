
#include "ParkingSystem.h"
#include <vector>
#include <set>
#include <random>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>

using namespace std;

vector<Vehicle> generateRandomVehicles(int count) {
    const int MAX_ROWS = 26;
    const int MAX_COLS = 400;
    const int MAX_CAPACITY = MAX_ROWS * MAX_COLS;

    if (count < 0 || count > MAX_CAPACITY) {
        cout << "So luong xe khong hop le!\n";
        return {};
    }

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> province(11, 99);
    uniform_int_distribution<int> letter(0, 25);
    uniform_int_distribution<int> digits(0, 99999);
    uniform_int_distribution<int> type(1, 2);
    uniform_int_distribution<long long> timeOffset(0, 30LL * 24 * 60 * 60);

    const string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    set<string> usedPlates;
    vector<Vehicle> vehicles;

    vehicles.reserve(count);

    time_t now = time(nullptr);

    while (static_cast<int>(vehicles.size()) < count) {
        string plate = to_string(province(gen));
        plate += letters[letter(gen)];
        plate += to_string(100000 + digits(gen)).substr(1);

        if (!usedPlates.insert(plate).second) {
            continue;
        }

        int index = vehicles.size();

        Vehicle v;
        v.licensePlate = plate;
        v.vehicleType = type(gen);
        v.assignedRow = 'A' + index / MAX_COLS;
        v.assignedCol = index % MAX_COLS + 1;
        v.entryTime = now - timeOffset(gen);

        vehicles.push_back(v);
    }

    return vehicles;
}

bool saveRandomVehiclesToFile(
    const vector<Vehicle>& vehicles,
    const string& filename
) {
    ofstream fout(filename);

    if (!fout.is_open()) {
        cout << "Khong the tao file!\n";
        return false;
    }

    fout << "LicensePlate,VehicleType,AssignedRow,AssignedCol,EntryTime\n";

    for (const Vehicle& v : vehicles) {
        fout << v.licensePlate << ","
             << v.vehicleType << ","
             << v.assignedRow << ","
             << v.assignedCol << ","
             << static_cast<long long>(v.entryTime) << "\n";
    }

    fout.close();

    cout << "Da tao " << vehicles.size()
         << " xe trong file " << filename << "\n";

    return true;
}
