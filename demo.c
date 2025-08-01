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
                systemData.admins[i].password);
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

void loadAllData() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("                      No existing data file found. Initializing with default settings.\n");
        initializeDefaultData();
        return;
    }

    char line[250];
    char section[50];
    int count;

    initializeDefaultData(); 

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "ADMINS") == 0) {
            fscanf(file, "%d\n", &systemData.numAdmins);
            for (int i = 0; i < systemData.numAdmins; i++) {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "%[^,],%[^,],%[^,],%s",
                       systemData.admins[i].name,
                       systemData.admins[i].phoneNumber,
                       systemData.admins[i].email,
                       systemData.admins[i].password);
            }
        } else if (strcmp(line, "PARKING_RATES") == 0) {
            fgets(line, sizeof(line), file);
            line[strcspn(line, "\n")] = 0;
            sscanf(line, "%lf,%lf,%lf",
                   &systemData.carPricePerHour,
                   &systemData.bikePricePerHour,
                   &systemData.jeepPricePerHour);
        } else if (strcmp(line, "CAR_SPOTS") == 0) {
            for (int i = 0; i < MAX_CARS; i++) {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                long entryTimeLong;
                sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%ld",
                       &systemData.carSpots[i].spotId,
                       &systemData.carSpots[i].type,
                       &systemData.carSpots[i].isOccupied,
                       systemData.carSpots[i].licensePlate,
                       systemData.carSpots[i].driverName,
                       systemData.carSpots[i].driverContact,
                       &entryTimeLong);
                systemData.carSpots[i].entryTime = (time_t)entryTimeLong;
            }
        } else if (strcmp(line, "BIKE_SPOTS") == 0) {
            for (int i = 0; i < MAX_BIKES; i++) {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                long entryTimeLong;
                sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%ld",
                       &systemData.bikeSpots[i].spotId,
                       &systemData.bikeSpots[i].type,
                       &systemData.bikeSpots[i].isOccupied,
                       systemData.bikeSpots[i].licensePlate,
                       systemData.bikeSpots[i].driverName,
                       systemData.bikeSpots[i].driverContact,
                       &entryTimeLong);
                systemData.bikeSpots[i].entryTime = (time_t)entryTimeLong;
            }
        } else if (strcmp(line, "JEEP_SPOTS") == 0) {
            for (int i = 0; i < MAX_JEEPS; i++) {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                long entryTimeLong;
                sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%ld",
                       &systemData.jeepSpots[i].spotId,
                       &systemData.jeepSpots[i].type,
                       &systemData.jeepSpots[i].isOccupied,
                       systemData.jeepSpots[i].licensePlate,
                       systemData.jeepSpots[i].driverName,
                       systemData.jeepSpots[i].driverContact,
                       &entryTimeLong);
                systemData.jeepSpots[i].entryTime = (time_t)entryTimeLong;
            }
        } else if (strcmp(line, "TRANSACTIONS") == 0) {
            fscanf(file, "%d\n", &systemData.numTransactions);
            for (int i = 0; i < systemData.numTransactions; i++) {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                long entryTimeLong, exitTimeLong;
                sscanf(line, "%[^,],%[^,],%[^,],%d,%d,%ld,%ld,%lf",
                       systemData.transactions[i].licensePlate,
                       systemData.transactions[i].driverName,
                       systemData.transactions[i].driverContact,
                       &systemData.transactions[i].vehicleType,
                       &systemData.transactions[i].spotId,
                       &entryTimeLong,
                       &exitTimeLong,
                       &systemData.transactions[i].cost);
                systemData.transactions[i].entryTime = (time_t)entryTimeLong;
                systemData.transactions[i].exitTime = (time_t)exitTimeLong;
            }
        } else if (strcmp(line, "CUSTOMERS") == 0) {
            fscanf(file, "%d\n", &systemData.numCustomers);
            for (int i = 0; i < systemData.numCustomers; i++) {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "%[^,],%[^,],%s",
                       systemData.customers[i].licensePlate,
                       systemData.customers[i].name,
                       systemData.customers[i].contact);
            }
        }
    }
    fclose(file);
}

void appendTransaction(VehicleTransaction transaction) {
    if (systemData.numTransactions < MAX_TRANSACTIONS) {
        systemData.transactions[systemData.numTransactions++] = transaction;
    } else {
        printf("                      Transaction history is full. Cannot add more transactions.\n");
    }
    saveAllData();
}

void addCustomer(Customer newCustomer) {
    if (systemData.numCustomers < MAX_CUSTOMERS) {
        systemData.customers[systemData.numCustomers++] = newCustomer;
    } else {
        printf("                      Customer database is full. Cannot add more customers.\n");
    }
    saveAllData();
}

void registerAdmin() {
    if (systemData.numAdmins >= MAX_ADMINS) {
        printf("                      Maximum number of admins reached.\n");
        return;
    }

    Admin newAdmin;
    char temp_email[NAME_LEN];
    char temp_phone[CONTACT_LEN];

    printf("\n               --- Admin Registration ---\n");

    printf("                      Enter Admin Name: ");
    scanf(" %[^\n]%*c", newAdmin.name);

    do {
        printf("                      Enter Phone Number (exactly 11 digits & starts with '01'): ");
        scanf("%s", temp_phone);
        if (!isValidPhoneNumber(temp_phone)) {
            printf("                      Invalid phone number. Please enter exactly 11 digits & starts with '01'.\n");
        }
    } while (!isValidPhoneNumber(temp_phone));
    strcpy(newAdmin.phoneNumber, temp_phone);

    do {
        printf("                      Enter Email (must contain '@gmail.com'): ");
        scanf("%s", temp_email);
        if (!isValidEmail(temp_email)) {
            printf("                      Invalid email. Email must contain '@gmail.com'.\n");
        }
    } while (!isValidEmail(temp_email));
    strcpy(newAdmin.email, temp_email);

    do {
        printf("                      Enter Password (at least 8 characters): ");
        scanf("%s", newAdmin.password);
        if (!isValidPassword(newAdmin.password)) {
            printf("                      Invalid password. Password must be at least 8 characters long.\n");
        }
    } while (!isValidPassword(newAdmin.password));

    systemData.admins[systemData.numAdmins++] = newAdmin;
    printf("                      Admin registered successfully!\n");
    saveAllData();
}

int loginAdmin() {
    char name[NAME_LEN];
    char password[PASSWORD_LEN];
    printf("\n                ------- Admin Login -------\n");
    printf("                      Enter Admin Name: ");
    scanf(" %[^\n]%*c", name);
    printf("                      Enter Password: ");
    scanf("%s", password);

    for (int i = 0; i < systemData.numAdmins; i++) {
        if (strcmp(systemData.admins[i].name, name) == 0 && strcmp(systemData.admins[i].password, password) == 0) {
            printf("                      Login successful! Welcome, %s.\n", systemData.admins[i].name);
            return 1;
        }
    }
    printf("                      Invalid admin name or password.\n");
    return 0;
}

void recordEntry() {
    int vehicleTypeChoice;
    char licensePlate[LICENSE_PLATE_LEN];
    char driverName[NAME_LEN];
    char driverContact[CONTACT_LEN];
    int i, spotFound = 0;
    time_t currentTime;

    printf("                      Select vehicle type:\n");
    printf("                      1. Car\n");
    printf("                      2. Bike\n");
    printf("                      3. Jeep\n");
    printf("                      Enter your choice: ");
    scanf("%d", &vehicleTypeChoice);

    printf("                      Enter license plate: ");
    scanf("%s", licensePlate);

    printf("                      Enter driver name: ");
    scanf(" %[^\n]%*c", driverName);

    do {
        printf("                      Enter driver phone number (exactly 11 digits & starts with '01'): ");
        scanf("%s", driverContact);
        if (!isValidPhoneNumber(driverContact)) {
            printf("                      Invalid phone number. Please enter exactly 11 digits & starts with '01'.\n");
        }
    } while (!isValidPhoneNumber(driverContact));


    currentTime = time(NULL);

    switch (vehicleTypeChoice) {
        case 1:
            for (i = 0; i < MAX_CARS; i++) {
                if (!systemData.carSpots[i].isOccupied) {
                    systemData.carSpots[i].isOccupied = 1;
                    strcpy(systemData.carSpots[i].licensePlate, licensePlate);
                    strcpy(systemData.carSpots[i].driverName, driverName);
                    strcpy(systemData.carSpots[i].driverContact, driverContact);
                    systemData.carSpots[i].entryTime = currentTime;
                    printf("                      Car parked at spot C%d. Entry time: %s", systemData.carSpots[i].spotId, ctime(&currentTime));
                    printf("                      Price per hour: %.2lf\n", systemData.carPricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("                      No available car spots.\n");
            }
            break;
        case 2:
            for (i = 0; i < MAX_BIKES; i++) {
                if (!systemData.bikeSpots[i].isOccupied) {
                    systemData.bikeSpots[i].isOccupied = 1;
                    strcpy(systemData.bikeSpots[i].licensePlate, licensePlate);
                    strcpy(systemData.bikeSpots[i].driverName, driverName);
                    strcpy(systemData.bikeSpots[i].driverContact, driverContact);
                    systemData.bikeSpots[i].entryTime = currentTime;
                    printf("                      Bike parked at spot B%d. Entry time: %s", systemData.bikeSpots[i].spotId, ctime(&currentTime));
                    printf("                      Price per hour: %.2lf\n", systemData.bikePricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("                      No available bike spots.\n");
            }
            break;
        case 3:
            for (i = 0; i < MAX_JEEPS; i++) {
                if (!systemData.jeepSpots[i].isOccupied) {
                    systemData.jeepSpots[i].isOccupied = 1;
                    strcpy(systemData.jeepSpots[i].licensePlate, licensePlate);
                    strcpy(systemData.jeepSpots[i].driverName, driverName);
                    strcpy(systemData.jeepSpots[i].driverContact, driverContact);
                    systemData.jeepSpots[i].entryTime = currentTime;
                    printf("                      Jeep parked at spot J%d. Entry time: %s", systemData.jeepSpots[i].spotId, ctime(&currentTime));
                    printf("                      Price per hour: %.2lf\n", systemData.jeepPricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("                      No available jeep spots.\n");
            }
            break;
        default:
            printf("                      Invalid vehicle type choice.\n");
    }
    saveAllData();
}

void recordExit() {
    char licensePlate[LICENSE_PLATE_LEN];
    int i, vehicleFound = 0;
    time_t exitTime;
    double durationInSeconds, durationInHours, totalCost = 0.0;
    VehicleTransaction transaction;

    printf("                      Enter license plate number to exit: ");
    scanf("%s", licensePlate);

    exitTime = time(NULL);

    for (i = 0; i < MAX_CARS; i++) {
        if (systemData.carSpots[i].isOccupied && strcmp(systemData.carSpots[i].licensePlate, licensePlate) == 0) {
            durationInSeconds = difftime(exitTime, systemData.carSpots[i].entryTime);
            durationInHours = durationInSeconds / 3600.0;
            totalCost = durationInHours * systemData.carPricePerHour;

            printf("                      Vehicle exited from spot C%d.\n", systemData.carSpots[i].spotId);
            printf("                      Entry time: %s", ctime(&systemData.carSpots[i].entryTime));
            printf("                      Exit time: %s", ctime(&exitTime));
            printf("                      Duration: %.2f hours\n", durationInHours);
            printf("                      Total cost: %.2lf\n", totalCost);

            strcpy(transaction.licensePlate, systemData.carSpots[i].licensePlate);
            strcpy(transaction.driverName, systemData.carSpots[i].driverName);
            strcpy(transaction.driverContact, systemData.carSpots[i].driverContact);
            transaction.vehicleType = VEHICLE_TYPE_CAR;
            transaction.spotId = systemData.carSpots[i].spotId;
            transaction.entryTime = systemData.carSpots[i].entryTime;
            transaction.exitTime = exitTime;
            transaction.cost = totalCost;
            appendTransaction(transaction);

            systemData.carSpots[i].isOccupied = 0;
            memset(systemData.carSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
            memset(systemData.carSpots[i].driverName, 0, NAME_LEN);
            memset(systemData.carSpots[i].driverContact, 0, CONTACT_LEN);
            vehicleFound = 1;
            break;
        }
    }

    if (!vehicleFound) {
        for (i = 0; i < MAX_BIKES; i++) {
            if (systemData.bikeSpots[i].isOccupied && strcmp(systemData.bikeSpots[i].licensePlate, licensePlate) == 0) {
                durationInSeconds = difftime(exitTime, systemData.bikeSpots[i].entryTime);
                durationInHours = durationInSeconds / 3600.0;
                totalCost = durationInHours * systemData.bikePricePerHour;

                printf("                      Vehicle exited from spot B%d.\n", systemData.bikeSpots[i].spotId);
                printf("                      Entry time: %s", ctime(&systemData.bikeSpots[i].entryTime));
                printf("                      Exit time: %s", ctime(&exitTime));
                printf("                      Duration: %.2f hours\n", durationInHours);
                printf("                      Total cost: %.2lf\n", totalCost);

                strcpy(transaction.licensePlate, systemData.bikeSpots[i].licensePlate);
                strcpy(transaction.driverName, systemData.bikeSpots[i].driverName);
                strcpy(transaction.driverContact, systemData.bikeSpots[i].driverContact);
                transaction.vehicleType = VEHICLE_TYPE_BIKE;
                transaction.spotId = systemData.bikeSpots[i].spotId;
                transaction.entryTime = systemData.bikeSpots[i].entryTime;
                transaction.exitTime = exitTime;
                transaction.cost = totalCost;
                appendTransaction(transaction);

                systemData.bikeSpots[i].isOccupied = 0;
                memset(systemData.bikeSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
                memset(systemData.bikeSpots[i].driverName, 0, NAME_LEN);
                memset(systemData.bikeSpots[i].driverContact, 0, CONTACT_LEN);
                vehicleFound = 1;
                break;
            }
        }
    }

    if (!vehicleFound) {
        for (i = 0; i < MAX_JEEPS; i++) {
            if (systemData.jeepSpots[i].isOccupied && strcmp(systemData.jeepSpots[i].licensePlate, licensePlate) == 0) {
                durationInSeconds = difftime(exitTime, systemData.jeepSpots[i].entryTime);
                durationInHours = durationInSeconds / 3600.0;
                totalCost = durationInHours * systemData.jeepPricePerHour;

                printf("                      Vehicle exited from spot J%d.\n", systemData.jeepSpots[i].spotId);
                printf("                      Entry time: %s", ctime(&systemData.jeepSpots[i].entryTime));
                printf("                      Exit time: %s", ctime(&exitTime));
                printf("                      Duration: %.2f hours\n", durationInHours);
                printf("                      Total cost: %.2lf\n", totalCost);

                strcpy(transaction.licensePlate, systemData.jeepSpots[i].licensePlate);
                strcpy(transaction.driverName, systemData.jeepSpots[i].driverName);
                strcpy(transaction.driverContact, systemData.jeepSpots[i].driverContact);
                transaction.vehicleType = VEHICLE_TYPE_JEEP;
                transaction.spotId = systemData.jeepSpots[i].spotId;
                transaction.entryTime = systemData.jeepSpots[i].entryTime;
                transaction.exitTime = exitTime;
                transaction.cost = totalCost;
                appendTransaction(transaction);

                systemData.jeepSpots[i].isOccupied = 0;
                memset(systemData.jeepSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
                memset(systemData.jeepSpots[i].driverName, 0, NAME_LEN);
                memset(systemData.jeepSpots[i].driverContact, 0, CONTACT_LEN);
                vehicleFound = 1;
                break;
            }
        }
    }

    if (!vehicleFound) {
        printf("                      Vehicle with license plate %s not found in parking.\n", licensePlate);
    }
    saveAllData();
}

void listParkedVehicles() {
    int i;
    int foundVehicles = 0;
    printf("\n                      --- Currently Parked Vehicles ---\n");

    printf("                      Cars:\n");
    for (i = 0; i < MAX_CARS; i++) {
        if (systemData.carSpots[i].isOccupied) {
            printf("                      Spot C%d: License Plate: %s, Driver: %s, Contact: %s, Entry Time: %s",
                   systemData.carSpots[i].spotId, systemData.carSpots[i].licensePlate,
                   systemData.carSpots[i].driverName, systemData.carSpots[i].driverContact,
                   ctime(&systemData.carSpots[i].entryTime));
            foundVehicles = 1;
        }
    }
    if (!foundVehicles) {
        printf("                      No cars currently parked.\n");
    }
    foundVehicles = 0;

    printf("                      Bikes:\n");
    for (i = 0; i < MAX_BIKES; i++) {
        if (systemData.bikeSpots[i].isOccupied) {
            printf("                      Spot B%d: License Plate: %s, Driver: %s, Contact: %s, Entry Time: %s",
                   systemData.bikeSpots[i].spotId, systemData.bikeSpots[i].licensePlate,
                   systemData.bikeSpots[i].driverName, systemData.bikeSpots[i].driverContact,
                   ctime(&systemData.bikeSpots[i].entryTime));
            foundVehicles = 1;
        }
    }
    if (!foundVehicles) {
        printf("                      No bikes currently parked.\n");
    }
    foundVehicles = 0;

    printf("                      Jeeps:\n");
    for (i = 0; i < MAX_JEEPS; i++) {
        if (systemData.jeepSpots[i].isOccupied) {
            printf("                      Spot J%d: License Plate: %s, Driver: %s, Contact: %s, Entry Time: %s",
                   systemData.jeepSpots[i].spotId, systemData.jeepSpots[i].licensePlate,
                   systemData.jeepSpots[i].driverName, systemData.jeepSpots[i].driverContact,
                   ctime(&systemData.jeepSpots[i].entryTime));
            foundVehicles = 1;
        }
    }
    if (!foundVehicles) {
        printf("                      No jeeps currently parked.\n");
    }
    printf("                      ---------------------------------\n");
}

void displayAdminDetails() {
    printf("\n                      --- Admin Details ---\n");
    if (systemData.numAdmins == 0) {
        printf("                      No admin registered yet.\n");
        return;
    }
    for (int i = 0; i < systemData.numAdmins; i++) {
        printf("                      Admin Name: %s\n", systemData.admins[i].name);
        printf("                      Phone Number: %s\n", systemData.admins[i].phoneNumber);
        printf("                      Email: %s\n", systemData.admins[i].email);
        printf("                      ----------------------\n");
    }
}

void configureParkingRates() {
    double newCarRate, newBikeRate, newJeepRate;

    printf("\n                      --- Configure Parking Rates ---\n");
    printf("                      Current Car Rate: %.2lf per hour\n", systemData.carPricePerHour);
    printf("                      Enter new Car Rate: ");
    scanf("%lf", &newCarRate);
    if (newCarRate >= 0) {
        systemData.carPricePerHour = newCarRate;
    } else {
        printf("                      Invalid rate. Rate must be positive integer.\n");
    }

    printf("                      Current Bike Rate: %.2lf per hour\n", systemData.bikePricePerHour);
    printf("                      Enter new Bike Rate: ");
    scanf("%lf", &newBikeRate);
    if (newBikeRate >= 0) {
        systemData.bikePricePerHour = newBikeRate;
    } else {
        printf("                      Invalid rate. Rate must be positive integer.\n");
    }

    printf("                      Current Jeep Rate: %.2lf per hour\n", systemData.jeepPricePerHour);
    printf("                      Enter new Jeep Rate: ");
    scanf("%lf", &newJeepRate);
    if (newJeepRate >= 0) {
        systemData.jeepPricePerHour = newJeepRate;
    } else {
        printf("                      Invalid rate. Rate must be positive integer.\n");
    }
    printf("                      Parking rates updated successfully.\n");
    saveAllData();
}

void calculateDailyRevenueSummary() {
    double currentDailyRevenue = 0.0;
    time_t now = time(NULL);
    struct tm *local_now = localtime(&now);

    printf("\n                      --- Daily Revenue Summary ---\n");

    for (int i = 0; i < systemData.numTransactions; i++) {
        struct tm *local_exit = localtime(&systemData.transactions[i].exitTime);
        if (local_exit->tm_year == local_now->tm_year &&
            local_exit->tm_mon == local_now->tm_mon &&
            local_exit->tm_mday == local_now->tm_mday) {
            currentDailyRevenue += systemData.transactions[i].cost;
        }
    }
    printf("                      Total Revenue: %.2lf\n", currentDailyRevenue);
    printf("                      -----------------------------\n");
}

// --- Customer Info Functions ---
void addCustomerDetails() {
    Customer newCustomer;
    char temp_contact[CONTACT_LEN];

    printf("\n                      --- Add Customer Details ---\n");
    printf("                      Enter License Plate: ");
    scanf("%s", newCustomer.licensePlate);
    printf("                      Enter Customer Name: ");
    scanf(" %[^\n]%*c", newCustomer.name);
    
    do {
        printf("                      Enter Contact Info (Phone Number - exactly 11 digits & starts with 01): ");
        scanf("%s", temp_contact);
        if (!isValidPhoneNumber(temp_contact)) {
            printf("                      Invalid phone number. Please enter exactly 11 digits & starts with 01.\n");
        }
    } while (!isValidPhoneNumber(temp_contact));
    strcpy(newCustomer.contact, temp_contact);

    addCustomer(newCustomer);
    printf("                      Customer details added.\n");
}

void viewCustomerDetails() {
    if (systemData.numCustomers == 0) {
        printf("                      No customer data available.\n");
        return;
    }

    printf("\n                      --- All Customer Details ---\n");
    for (int i = 0; i < systemData.numCustomers; i++) {
        printf("                      License Plate: %s\n", systemData.customers[i].licensePlate);
        printf("                      Name: %s\n", systemData.customers[i].name);
        printf("                      Contact: %s\n", systemData.customers[i].contact);
        printf("                      --------------------------\n");
    }
}

void searchCustomerDetails() {
    char searchPlate[LICENSE_PLATE_LEN];
    printf("\n                      --- Search Customer by License Plate ---\n");
    printf("                      Enter License Plate to search: ");
    scanf("%s", searchPlate);

    Customer *foundCustomer = NULL;
    for (int i = 0; i < systemData.numCustomers; i++) {
        if (strcmp(systemData.customers[i].licensePlate, searchPlate) == 0) {
            foundCustomer = &systemData.customers[i];
            break;
        }
    }

    if (foundCustomer != NULL) {
        printf("\n                      Customer Found:\n");
        printf("                      License Plate: %s\n", foundCustomer->licensePlate);
        printf("                      Name: %s\n", foundCustomer->name);
        printf("                      Contact: %s\n", foundCustomer->contact);

        printf("\n                      --- Parking History for %s ---\n", searchPlate);
        int foundHistory = 0;
        for (int i = 0; i < systemData.numTransactions; i++) {
            if (strcmp(systemData.transactions[i].licensePlate, searchPlate) == 0) {
                printf("                      Spot ID: %d, Type: ", systemData.transactions[i].spotId);
                switch (systemData.transactions[i].vehicleType) {
                    case VEHICLE_TYPE_CAR: printf("                      Car\n"); break;
                    case VEHICLE_TYPE_BIKE: printf("                      Bike\n"); break;
                    case VEHICLE_TYPE_JEEP: printf("                      Jeep\n"); break;
                    default: printf("                      Unknown\n"); break;
                }
                printf("                      Driver: %s, Contact: %s\n", systemData.transactions[i].driverName, systemData.transactions[i].driverContact);
                printf("                      Entry: %s", ctime(&systemData.transactions[i].entryTime));
                if (systemData.transactions[i].exitTime != 0) {
                    printf("                      Exit: %s", ctime(&systemData.transactions[i].exitTime));
                    printf("                      Cost: %.2lf\n", systemData.transactions[i].cost);
                } else {
                    printf("                      (Currently Parked)\n");
                }
                printf("                      --------------------\n");
                foundHistory = 1;
            }
        }
        if (!foundHistory) {
            printf("                      No parking history found for this license plate.\n");
        }
    } else {
        printf("                      Customer with license plate %s not found.\n", searchPlate);
    }
}

void displayParkingStatus() {
    int i;
    printf("\n                      --- Parking Status ---\n");

    printf("                      Cars:\n");
    for (i = 0; i < MAX_CARS; i++) {
        printf("                      Spot C%d: %s", systemData.carSpots[i].spotId, systemData.carSpots[i].isOccupied ? systemData.carSpots[i].licensePlate : "Available");
        if (systemData.carSpots[i].isOccupied) {
            printf("                      (Driver: %s, Contact: %s, Entry: %s)", systemData.carSpots[i].driverName, systemData.carSpots[i].driverContact, ctime(&systemData.carSpots[i].entryTime));
        }
        printf("\n");
    }

    printf("                      Bikes:\n");
    for (i = 0; i < MAX_BIKES; i++) {
        printf("                      Spot B%d: %s", systemData.bikeSpots[i].spotId, systemData.bikeSpots[i].isOccupied ? systemData.bikeSpots[i].licensePlate : "Available");
        if (systemData.bikeSpots[i].isOccupied) {
            printf("                      (Driver: %s, Contact: %s, Entry: %s)", systemData.bikeSpots[i].driverName, systemData.bikeSpots[i].driverContact, ctime(&systemData.bikeSpots[i].entryTime));
        }
        printf("\n");
    }

    printf("                      Jeeps:\n");
    for (i = 0; i < MAX_JEEPS; i++) {
        printf("                      Spot J%d: %s", systemData.jeepSpots[i].spotId, systemData.jeepSpots[i].isOccupied ? systemData.jeepSpots[i].licensePlate : "Available");
        if (systemData.jeepSpots[i].isOccupied) {
            printf("                      (Driver: %s, Contact: %s, Entry: %s)", systemData.jeepSpots[i].driverName, systemData.jeepSpots[i].driverContact, ctime(&systemData.jeepSpots[i].entryTime));
        }
        printf("\n");
    }
    printf("                      ----------------------\n");
}

void displayTotalEmptySlots() {
    int emptyCars = 0;
    int emptyBikes = 0;
    int emptyJeeps = 0;

    for (int i = 0; i < MAX_CARS; i++) {
        if (!systemData.carSpots[i].isOccupied) emptyCars++;
    }
    for (int i = 0; i < MAX_BIKES; i++) {
        if (!systemData.bikeSpots[i].isOccupied) emptyBikes++;
    }
    for (int i = 0; i < MAX_JEEPS; i++) {
        if (!systemData.jeepSpots[i].isOccupied) emptyJeeps++;
    }

    printf("\n                      --- Total Empty Slots ---\n");
    printf("                      Empty Car Spots: %d/%d\n", emptyCars, MAX_CARS);
    printf("                      Empty Bike Spots: %d/%d\n", emptyBikes, MAX_BIKES);
    printf("                      Empty Jeep Spots: %d/%d\n", emptyJeeps, MAX_JEEPS);
    printf("                      Total Empty Slots: %d\n", emptyCars + emptyBikes + emptyJeeps);
    printf("                      -------------------------\n");
}

void displayTotalAmount() {
    double grandTotalRevenue = 0.0;

    printf("\n                      --- Total Revenue ---\n");

    for (int i = 0; i < systemData.numTransactions; i++) {
        grandTotalRevenue += systemData.transactions[i].cost;
    }
    printf("                      Total Revenue(All Time): %.2lf\n", grandTotalRevenue);
    printf("                      ---------------------------\n");
}

void adminMenu() {
    int choice;
    do {
        printf("\n                      --- Admin Panel ---\n");
        printf("                      1. Admin Details\n");
        printf("                      2. Configure Parking Rates\n");
        printf("                      3. View Daily Revenue Summary\n");
        printf("                      4. Back to Main Menu\n");
        printf("                      Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAdminDetails();
                break;
            case 2:
                configureParkingRates();
                break;
            case 3:
                calculateDailyRevenueSummary();
                break;
            case 4:
                break;
            default:
                printf("                      Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

void vehicleMenu() {
    int choice;
    do {
        printf("\n                      --- Vehicle Panel ---\n");
        printf("                      1. Record Vehicle Entry\n");
        printf("                      2. Record Vehicle Exit\n");
        printf("                      3. List Parked Vehicles\n");
        printf("                      4. Back to Main Menu\n");
        printf("                      Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                recordEntry();
                break;
            case 2:
                recordExit();
                break;
            case 3:
                listParkedVehicles();
                break;
            case 4:
                break;
            default:
                printf("                      Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

void customerMenu() {
    int choice;
    do {
        printf("\n                      --- Customer Info ---\n");
        printf("                      1. Add Customer Details\n");
        printf("                      2. View All Customer Details\n");
        printf("                      3. Search Customer by License Plate\n");
        printf("                      4. Back to Main Menu\n");
        printf("                      Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addCustomerDetails();
                break;
            case 2:
                viewCustomerDetails();
                break;
            case 3:
                searchCustomerDetails();
                break;
            case 4:
                break;
            default:
                printf("                      Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
}

void slotManagementMenu() {
    int choice;
    do {
        printf("\n                      --- Slot Management ---\n");
        printf("                      1. Display Parking Status\n");
        printf("                      2. Display Total Empty Slots\n");
        printf("                      3. Back to Main Menu\n");
        printf("                      Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayParkingStatus();
                break;
            case 2:
                displayTotalEmptySlots();
                break;
            case 3:
                break;
            default:
                printf("                      Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

int main() {
    int mainChoice;
    loadAllData();

    do {
        printf("\n         ................................................................\n");
        printf("           ------ Welcome to Parking Lot Management System ------\n");
        printf("         ................................................................\n");
        printf("                      1. Register as Admin\n");
        printf("                      2. Log in as Admin\n");
        printf("                      3. Exit\n");
        printf("                      Enter your choice: ");
        scanf("%d", &mainChoice);

        switch (mainChoice) {
            case 1:
                registerAdmin();
                break;
            case 2:
                if (loginAdmin()) {
                    int loggedInChoice;
                    do {
                        printf("\n                   --- Admin Menu ---\n");
                        printf("                  ...........................\n");
                        printf("                       1. Admin Panel\n");
                        printf("                       2. Vehicle Panel\n");
                        printf("                       3. Customer Info\n");
                        printf("                       4. Slot Management\n");
                        printf("                       5. Total Revenue (All Time)\n");
                        printf("                       6. Log out\n");
                        printf("                      Enter your choice: ");
                        scanf("%d", &loggedInChoice);

                        switch (loggedInChoice) {
                            case 1:
                                adminMenu();
                                break;
                            case 2:
                                vehicleMenu();
                                break;
                            case 3:
                                customerMenu();
                                break;
                            case 4:
                                slotManagementMenu();
                                break;
                            case 5:
                                displayTotalAmount();
                                break;
                            case 6:
                                printf("                      Logging out from admin session.\n");
                                break;
                            default:
                                printf("                      Invalid choice. Please try again.\n");
                        }
                    } while (loggedInChoice != 6);
                }
                break;
            case 3:
                printf("                      Exiting program. Goodbye!\n");
                break;
            default:
                printf("                      Invalid choice. Please try again.\n");
        }
    } while (mainChoice != 3);

    return 0;
}
