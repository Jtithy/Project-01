#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_CARS 15
#define MAX_BIKES 10
#define MAX_JEEPS 5
#define LICENSE_PLATE_LEN 20
#define NAME_LEN 50
#define CONTACT_LEN 11 //Phone number
#define PASSWORD_LEN 50
#define MAX_ADMINS 3    //Total admins
#define MAX_TRANSACTIONS 1000 
#define MAX_CUSTOMERS 500 

#define DATA_FILE "parking_data.txt" // File to store data

// Vehicle types
#define VEHICLE_TYPE_CAR 1
#define VEHICLE_TYPE_BIKE 2
#define VEHICLE_TYPE_JEEP 3

typedef struct {
    int spotId;
    int type; // 1: Car, 2: Bike, 3: Jeep
    int isOccupied;
    char licensePlate[LICENSE_PLATE_LEN];
    char driverName[NAME_LEN];
    char driverContact[CONTACT_LEN]; 
    time_t entryTime;
} ParkingSpot;

typedef struct {
    char licensePlate[LICENSE_PLATE_LEN];
    char driverName[NAME_LEN];
    char driverContact[CONTACT_LEN];
    int vehicleType; // 1: Car, 2: Bike, 3: Jeep
    int spotId;
    time_t entryTime;
    time_t exitTime;
    double cost;
} VehicleTransaction;

typedef struct {
    char licensePlate[LICENSE_PLATE_LEN];
    char name[NAME_LEN];
    char contact[CONTACT_LEN];
} Customer;

typedef struct {
    char name[NAME_LEN];
    char phoneNumber[CONTACT_LEN];
    char email[NAME_LEN];
    char password[PASSWORD_LEN];
} Admin;


typedef struct {
    Admin admins[MAX_ADMINS];
    int numAdmins;
    ParkingSpot carSpots[MAX_CARS];
    ParkingSpot bikeSpots[MAX_BIKES];
    ParkingSpot jeepSpots[MAX_JEEPS];
    VehicleTransaction transactions[MAX_TRANSACTIONS];
    int numTransactions;
    Customer customers[MAX_CUSTOMERS];
    int numCustomers;
    double carPricePerHour;
    double bikePricePerHour;
    double jeepPricePerHour;
} ParkingLotData;

ParkingLotData systemData;

// --- Functions for Validation ---
int isValidPhoneNumber(const char *phone) {
    if (strlen(phone) != 11) {
        return 0;
    }
    for (int i = 0; i < 11; i++) {
        if (!isdigit(phone[i])) {
            return 0; // Contains non-digit characters
        }
        else if (i == 0 && phone[i] != '0' && i == 1 && phone[i] != '1') {
            return 0;
        }
    }
    return 1;
}

int isValidEmail(const char *email) {
    if (strstr(email, "@gmail.com") == NULL) {
        return 0;
    }
    return 1;
}

int isValidPassword(const char *password) {
    if (strlen(password) < 8) {
        return 0;
    }
    return 1;
}

void initializeDefaultData() {
    int i;
    // Initialize parking spots
    for (i = 0; i < MAX_CARS; i++) {
        systemData.carSpots[i].spotId = i + 1;
        systemData.carSpots[i].type = VEHICLE_TYPE_CAR;
        systemData.carSpots[i].isOccupied = 0;
        memset(systemData.carSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
        memset(systemData.carSpots[i].driverName, 0, NAME_LEN);
        memset(systemData.carSpots[i].driverContact, 0, CONTACT_LEN);
    }
    for (i = 0; i < MAX_BIKES; i++) {
        systemData.bikeSpots[i].spotId = i + 1;
        systemData.bikeSpots[i].type = VEHICLE_TYPE_BIKE;
        systemData.bikeSpots[i].isOccupied = 0;
        memset(systemData.bikeSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
        memset(systemData.bikeSpots[i].driverName, 0, NAME_LEN);
        memset(systemData.bikeSpots[i].driverContact, 0, CONTACT_LEN);
    }
    for (i = 0; i < MAX_JEEPS; i++) {
        systemData.jeepSpots[i].spotId = i + 1;
        systemData.jeepSpots[i].type = VEHICLE_TYPE_JEEP;
        systemData.jeepSpots[i].isOccupied = 0;
        memset(systemData.jeepSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
        memset(systemData.jeepSpots[i].driverName, 0, NAME_LEN);
        memset(systemData.jeepSpots[i].driverContact, 0, CONTACT_LEN);
    }

    // Initialize admin settings
    systemData.carPricePerHour = 300.0;
    systemData.bikePricePerHour = 150.0;
    systemData.jeepPricePerHour = 250.0;

    // Initialize counts
    systemData.numAdmins = 0;
    systemData.numTransactions = 0;
    systemData.numCustomers = 0;
}

// Function to save all data
void saveAllData() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("Error: Could not open data file for saving.\n");
        return;
    }

    // Save Admins
    fprintf(file, "ADMINS\n");
    fprintf(file, "%d\n", systemData.numAdmins);
    for (int i = 0; i < systemData.numAdmins; i++) {
        fprintf(file, "%s,%s,%s,%s\n",
                systemData.admins[i].name,
                systemData.admins[i].phoneNumber,
                systemData.admins[i].email,
                systemData.admins[i].password); // Storing password in plain text for simplicity
    }

    // Save Parking Rates
    fprintf(file, "PARKING_RATES\n");
    fprintf(file, "%.2lf,%.2lf,%.2lf\n",
            systemData.carPricePerHour,
            systemData.bikePricePerHour,
            systemData.jeepPricePerHour);

    // Save Car Spots
    fprintf(file, "CAR_SPOTS\n");
    for (int i = 0; i < MAX_CARS; i++) {
        fprintf(file, "%d,%d,%d,%s,%s,%s,%ld\n",
                systemData.carSpots[i].spotId,
                systemData.carSpots[i].type,
                systemData.carSpots[i].isOccupied,
                systemData.carSpots[i].licensePlate,
                systemData.carSpots[i].driverName,
                systemData.carSpots[i].driverContact,
                (long)systemData.carSpots[i].entryTime);
    }

    // Save Bike Spots
    fprintf(file, "BIKE_SPOTS\n");
    for (int i = 0; i < MAX_BIKES; i++) {
        fprintf(file, "%d,%d,%d,%s,%s,%s,%ld\n",
                systemData.bikeSpots[i].spotId,
                systemData.bikeSpots[i].type,
                systemData.bikeSpots[i].isOccupied,
                systemData.bikeSpots[i].licensePlate,
                systemData.bikeSpots[i].driverName,
                systemData.bikeSpots[i].driverContact,
                (long)systemData.bikeSpots[i].entryTime);
    }

    // Save Jeep Spots
    fprintf(file, "JEEP_SPOTS\n");
    for (int i = 0; i < MAX_JEEPS; i++) {
        fprintf(file, "%d,%d,%d,%s,%s,%s,%ld\n",
                systemData.jeepSpots[i].spotId,
                systemData.jeepSpots[i].type,
                systemData.jeepSpots[i].isOccupied,
                systemData.jeepSpots[i].licensePlate,
                systemData.jeepSpots[i].driverName,
                systemData.jeepSpots[i].driverContact,
                (long)systemData.jeepSpots[i].entryTime);
    }

    // Save Transactions
    fprintf(file, "TRANSACTIONS\n");
    fprintf(file, "%d\n", systemData.numTransactions);
    for (int i = 0; i < systemData.numTransactions; i++) {
        fprintf(file, "%s,%s,%s,%d,%d,%ld,%ld,%.2lf\n",
                systemData.transactions[i].licensePlate,
                systemData.transactions[i].driverName,
                systemData.transactions[i].driverContact,
                systemData.transactions[i].vehicleType,
                systemData.transactions[i].spotId,
                (long)systemData.transactions[i].entryTime,
                (long)systemData.transactions[i].exitTime,
                systemData.transactions[i].cost);
    }

    // Save Customers
    fprintf(file, "CUSTOMERS\n");
    fprintf(file, "%d\n", systemData.numCustomers);
    for (int i = 0; i < systemData.numCustomers; i++) {
        fprintf(file, "%s,%s,%s\n",
                systemData.customers[i].licensePlate,
                systemData.customers[i].name,
                systemData.customers[i].contact);
    }

    fclose(file);
}
