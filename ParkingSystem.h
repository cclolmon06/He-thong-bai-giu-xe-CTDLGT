#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include <iostream>
#include <string>
#include <ctime>

const int TYPE_AUTO_DETECT = 0;
const int TYPE_MOTORBIKE   = 1;
const int TYPE_CAR         = 2;

const long long MOTOR_FIRST_4H_FEE  = 5000;
const long long MOTOR_NEXT_HOUR_FEE = 2000;
const long long MOTOR_OVERNIGHT_FEE = 10000;

const long long CAR_FIRST_2H_FEE    = 25000;
const long long CAR_NEXT_HOUR_FEE   = 15000;
const long long CAR_OVERNIGHT_FEE   = 100000;

struct SlotNode {
    char row;
    int col;
    SlotNode* next;
};

struct Vehicle {
    std::string licensePlate;
    int vehicleType;
    char assignedRow;
    int assignedCol;
    std::time_t entryTime;
};

struct HashNode {
    Vehicle data;
    HashNode* next;
};

struct HistoryNode {
    Vehicle data;
    std::time_t exitTime;
    long long feeVND;
    HistoryNode* next;
};

class ParkingLot {
private:
    int numRows;
    int numCols;
    int emptySlotCount;
    SlotNode* freeSlotsHead;

    static const int TABLE_SIZE = 1009;
    HashNode* hashTable[TABLE_SIZE];

    HistoryNode* historyHead;

    ParkingLot(const ParkingLot&) = delete;
    ParkingLot& operator=(const ParkingLot&) = delete;

public:
    ParkingLot(int n = 5, int m = 10);
    ~ParkingLot();
    void resetLot(int n, int m);
    bool allocateNearestSlot(char& r, int& c);
    bool occupySpecificSlot(char r, int c);
    void releaseSlot(char r, int c);
    void displayEmptySlots() const;
    int getEmptySlotCount() const;
    int getTotalCapacity() const;

    int hashFunction(const std::string& plate) const;
    bool insertVehicleToHash(const Vehicle& v);
    HashNode* searchVehicle(const std::string& plate) const;
    bool removeVehicleFromHash(const std::string& plate, Vehicle& removedOut);

    bool isValidVNProvinceCode(int code) const;
    bool validateAndNormalizeVNPlate(std::string& plate, int expectedType = TYPE_AUTO_DETECT) const;
    bool parkVehicle(std::string plate, int type, std::time_t customEntryTime = 0);
    bool saveStateToFile(const std::string& filename) const;
    bool loadStateFromFile(const std::string& filename);

    long long calculateFeeVND(int type, std::time_t entry, std::time_t exit) const;
    std::string formatCurrencyVND(long long amount) const;
    bool retrieveVehicle(std::string plate, std::time_t customExitTime = 0);
    void recordHistory(const Vehicle& v, std::time_t exitTime, long long feeVND);
    void reportRevenue() const;

    void displayAllParkedVehicles() const;
    void runMenu();

    std::string generateRandomVNPlate(int vehicleType, int seedIndex) const;
    void runEdgeCaseTests();
    void runPerformanceBenchmark(int testSize = 10000);
};

#endif
