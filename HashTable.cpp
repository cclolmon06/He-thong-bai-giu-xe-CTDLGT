// HashTable.cpp - Thanh vien 2: Bang bam & xu ly dung do (Separate Chaining)
#include "ParkingSystem.h"

// Ham bam da thuc (Polynomial Rolling Hash, co so 31), chia lay du cho TABLE_SIZE.
// Lay mod trong moi vong lap de tranh tran so.
int ParkingLot::hashFunction(const string& plate) {
    unsigned long long hash = 0;
    for (char ch : plate) {
        hash = (hash * 31 + (unsigned char)ch) % TABLE_SIZE;
    }
    return (int)hash;
}

// Them xe vao dau danh sach lien ket tai bucket tuong ung. Do phuc tap O(1).
// Tra ve false neu bien so da ton tai (tranh trung lap).
bool ParkingLot::insertVehicleToHash(const Vehicle& v) {
    if (searchVehicle(v.licensePlate) != nullptr) {
        return false;
    }
    int index = hashFunction(v.licensePlate);

    HashNode* newNode = new HashNode;
    newNode->data = v;
    newNode->next = hashTable[index];   // tro vao dau chuoi cu
    hashTable[index] = newNode;         // node moi tro thanh dau chuoi
    return true;
}

// Tra cuu xe theo bien so. Trung binh O(1), xau nhat O(N).
// Tra ve con tro toi HashNode, hoac nullptr neu khong tim thay.
HashNode* ParkingLot::searchVehicle(const string& plate) {
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

// Xoa xe khoi bang bam: noi lai con tro truoc/sau, sao luu du lieu ra removedOut,
// roi giai phong node. Tra ve false neu khong tim thay.
bool ParkingLot::removeVehicleFromHash(const string& plate, Vehicle& removedOut) {
    int index = hashFunction(plate);
    HashNode* curr = hashTable[index];
    HashNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.licensePlate == plate) {
            removedOut = curr->data;            // sao luu truoc khi xoa

            if (prev == nullptr) {
                hashTable[index] = curr->next;  // xoa node o dau chuoi
            } else {
                prev->next = curr->next;        // xoa node o giua/cuoi chuoi
            }
            delete curr;                        // giai phong bo nho
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}
