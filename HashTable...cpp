#include "ParkingSystem.h"

using namespace std;

int ParkingLot::hashFunction(const string& plate) const {
    unsigned long long hash = 0;
    for (char ch : plate) {
        hash = (hash * 31 + (unsigned char)ch) % TABLE_SIZE;
    }
    return (int)hash;
}

bool ParkingLot::insertVehicleToHash(const Vehicle& v) {
    if (searchVehicle(v.licensePlate) != nullptr) {
        return false;
    }
    int index = hashFunction(v.licensePlate);

    HashNode* newNode = new HashNode;
    newNode->data = v;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    return true;
}

HashNode* ParkingLot::searchVehicle(const string& plate) const {
    int index = hashFunction(plate);
    HashNode* curr = hashTable[index];
    while (curr != nullptr) {
        if (curr->data.licensePlate == plate) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

bool ParkingLot::removeVehicleFromHash(const string& plate, Vehicle& removedOut) {
    int index = hashFunction(plate);
    HashNode* curr = hashTable[index];
    HashNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.licensePlate == plate) {
            removedOut = curr->data;

            if (prev == nullptr) {
                hashTable[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
