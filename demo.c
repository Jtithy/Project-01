// Parking Lot Management System

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<windows.h>

#define MAX_CARS 15
#define MAX_BIKES 10
#define MAX_JEEPS 5
#define LICENSE_PLATE_LEN 20
#define NAME_LEN 50
#define CONTACT_LEN 12
#define PASSWORD_LEN 30
#define EMAIL_LEN 30
#define MAX_ADMINS 3
#define MAX_TRANSACTIONS 1000
#define MAX_CUSTOMERS 500

#define DATA_FILE "parking_history.txt" //File name

// Vehicle types
typedef enum{
    VEHICLE_TYPE_CAR = 1,
    VEHICLE_TYPE_BIKE = 2,
    VEHICLE_TYPE_JEEP = 3
} VehicleType;

typedef struct{
    int spotID;
    VehicleType type; //1= Car, 2= Bike, 3= Jeep
    int isOccupied;
    char licensePlate[LICENSE_PLATE_LEN];
    char driverName[NAME_LEN];
    char driverContact[CONTACT_LEN];
    time_t entrTime;
} ParkingSpot;

typedef struct{
    int spotID;
    VehicleType vehicleType; //1= Car, 2= Bike, 3= Jeep
    char licensePlate[LICENSE_PLATE_LEN];
    char driverName[NAME_LEN];
    char driverContact[CONTACT_LEN];
    time_t entryTime;
    time_t exitTime;
    double cost;
} Transaction;

typedef struct{
    VehicleType vehicleType; //1= Car, 2= Bike, 3= Jeep
    char licensePlate[LICENSE_PLATE_LEN];
    char name[NAME_LEN];
    char contact[CONTACT_LEN];
} Customer;

typedef struct{
    char name[NAME_LEN];
    char phoneNumber[CONTACT_LEN];
    char email[EMAIL_LEN];
    char password[PASSWORD_LEN];
} Admin;

typedef struct{
    Admin admins[MAX_ADMINS];
    int numAdmins;
    ParkingSpot carSpots[MAX_CARS];
    ParkingSpot bikeSpots[MAX_BIKES];
    ParkingSpot jeepSpots[MAX_JEEPS];
    Transaction transactions[MAX_TRANSACTIONS];
    int numTransactions;
    Customer customers[MAX_CUSTOMERS];
    int numCustomers;
    double carPricePerHour;
    double bikePricePerHour;
    double jeepPricePerHour;
} ParkingLotData;

ParkingLotData systemData;

// Function Prototypes

int isValidName();
int isValidPhoneNumber();
int isValidEmail();
int isValidPassword();
void initializeDefaultData();
void saveAllData();
void loadAllData();
void registerAdmin();
int loginAdmin();
void recordEntry();
void recordExit();
void listParkedVehicles();
void displayAdminDetails();
void parkingRate();
void calculateDailyRevenue();
void viewCustomerDetails();
void searchCustomerDetails();
void displayParkingStatus();
void displayEmptySpots();
void displayTotalAmount();
void adminMenu();
void vehicleMenu();
void customerMenu();
void slotManagementMenu();
int main();

// Function for Validation

int isValidName(const char *name){
    if(strlen(name) == 0 || strlen(name) > NAME_LEN){
        printf("                      Invalid name.\n");
        return 0;
    }
    else{
        for(int i = 0; name[i] != '\0'; i++){
            if(!isalpha(name[i]) && !isspace(name[i])){
                printf("                      Name can only contain letters and spaces.\n");
                return 0;
            }
        }
    }
    return 1;
}

int isValidPhoneNumber(const char *phone){
    if(strlen(phone) != 11){
        printf("                      Phone number must be exactly 11 digits.\n");
        return 0;
    }
    else {
        for(int i = 0; i < 11; i++){
            if(!isdigit(phone[i])){
                printf("                      Phone number must contain only digits.\n");
                return 0;
            }
        }
        if(phone[0] != '0' || phone[1] != '1'){
            printf("                      Phone number must start with '01'.\n");
            return 0;
        }
    }
    return 1;
}

int isValidEmail(const char *email){
    if(strstr(email, "@gmail.com") == NULL){
        printf("                      Email must contain '@gmail.com'.\n");
        return 0;
    }
    return 1;
}

int isValidPassword(const char *password){
    if(strlen(password)< 8){
        printf("                      Password must be at least 8 characters long.\n");
        return 0;
    }
    return 1;
}

void initializeDefaultData(){
    int i;
    systemData.numAdmins = 0;
    systemData.numTransactions = 0;
    systemData.numCustomers = 0;

    //Initial parking rates
    systemData.carPricePerHour = 400.0;
    systemData.bikePricePerHour = 200.0;
    systemData.jeepPricePerHour = 350.0;

    // Initial parking spots
    for(i = 0; i < MAX_CARS; i++){
        systemData.carSpots[i].spotID = i + 1;
        systemData.carSpots[i].type = VEHICLE_TYPE_CAR;
        systemData.carSpots[i].isOccupied = 0;
        systemData.carSpots[i].licensePlate[0] = '\0';
        systemData.carSpots[i].driverName[0] = '\0';
        systemData.carSpots[i].driverContact[0] = '\0';
    }
    for(i = 0; i < MAX_BIKES; i++){
        systemData.bikeSpots[i].spotID = i + 1;
        systemData.bikeSpots[i].type = VEHICLE_TYPE_BIKE;
        systemData.bikeSpots[i].isOccupied = 0;
        systemData.bikeSpots[i].licensePlate[0] = '\0';
        systemData.bikeSpots[i].driverName[0] = '\0';
        systemData.bikeSpots[i].driverContact[0] = '\0';
    }
    for(i = 0; i < MAX_JEEPS; i++){
        systemData.jeepSpots[i].spotID = i + 1;
        systemData.jeepSpots[i].type = VEHICLE_TYPE_JEEP;
        systemData.jeepSpots[i].isOccupied = 0;
        systemData.jeepSpots[i].licensePlate[0] = '\0';
        systemData.jeepSpots[i].driverName[0] = '\0';
        systemData.jeepSpots[i].driverContact[0] = '\0';
    }
}

// Function to store data
void saveAllData(){
    FILE *file = fopen(DATA_FILE, "a");
    if(file == NULL){
        printf("                      Error opening data.\n");
        return;

        // Write Admins
        fprintf(file, "ADMINS\n");
        fprintf(file, "%d\n", systemData.numAdmins);
        for(int i = 0; i < systemData.numAdmins; i++){
            fprintf(file, "%s,%s,%s,%s\n", 
                systemData.admins[i].name, 
                systemData.admins[i].phoneNumber, 
                systemData.admins[i].email, 
                systemData.admins[i].password);
        }
    }
    
    // Write Parking Rates
    fprintf(file, "PARKING_RATES\n");
    fprintf(file, "%.2lf,%.2lf,%.2lf\n",
         systemData.carPricePerHour,
         systemData.bikePricePerHour, 
         systemData.jeepPricePerHour);

    // Save Car Spots
    fprintf(file, "CAR_SPOTS\n");
    for (int i = 0; i < MAX_CARS; i++) {
        fprintf(file, "%d,%d,%d,%s,%s,%s,%ld\n", 
            systemData.carSpots[i].spotID, 
            systemData.carSpots[i].type, 
            systemData.carSpots[i].isOccupied, 
            systemData.carSpots[i].licensePlate, 
            systemData.carSpots[i].driverName, 
            systemData.carSpots[i].driverContact,
            (long) systemData.carSpots[i].entrTime);
    }

    // Save Bike Spots
    fprintf(file, "BIKE_SPOTS\n");
    for (int i = 0; i < MAX_BIKES; i++) {
        fprintf(file, "%d,%d,%d,%s,%s,%s,%ld\n", 
            systemData.bikeSpots[i].spotID, 
            systemData.bikeSpots[i].type, 
            systemData.bikeSpots[i].isOccupied, 
            systemData.bikeSpots[i].licensePlate, 
            systemData.bikeSpots[i].driverName, 
            systemData.bikeSpots[i].driverContact,
            (long) systemData.bikeSpots[i].entrTime);
    }

    // Save Jeep Spots
    fprintf(file, "JEEP_SPOTS\n");
    for (int i = 0; i < MAX_JEEPS; i++) {
        fprintf(file, "%d,%d,%d,%s,%s,%s,%ld\n", 
            systemData.jeepSpots[i].spotID, 
            systemData.jeepSpots[i].type, 
            systemData.jeepSpots[i].isOccupied, 
            systemData.jeepSpots[i].licensePlate, 
            systemData.jeepSpots[i].driverName, 
            systemData.jeepSpots[i].driverContact,
            (long) systemData.jeepSpots[i].entrTime);
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
            systemData.transactions[i].spotID, 
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

void loadAllData(){
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("No existing data file found. Initializing with default settings.\n");
        initializeDefaultData();
        return;
    }
    
    // We must call initialize here, because we want to clear the memory before loading old data from the file.
    initializeDefaultData(); 

    char line[500]; 
    
    printf("Data loaded successfully.\n");
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "ADMINS") == 0) {
            if (fgets(line, sizeof(line), file) != NULL) {
                systemData.numAdmins = atoi(line);
                for (int i = 0; i < systemData.numAdmins; i++) {
                    if (fgets(line, sizeof(line), file) != NULL) {
                        line[strcspn(line, "\n")] = 0;
                        sscanf(line, "%[^,],%[^,],%[^,],%[^,]", 
                               systemData.admins[i].name, 
                               systemData.admins[i].phoneNumber, 
                               systemData.admins[i].email, 
                               systemData.admins[i].password);
                    }
                }
            }
        } else if (strcmp(line, "PARKING_RATES") == 0) {
            if (fgets(line, sizeof(line), file) != NULL) {
                line[strcspn(line, "\n")] = 0;
                sscanf(line, "%lf,%lf,%lf", 
                       &systemData.carPricePerHour, 
                       &systemData.bikePricePerHour, 
                       &systemData.jeepPricePerHour);
            }
        } else if (strcmp(line, "CAR_SPOTS") == 0) {
            for (int i = 0; i < MAX_CARS; i++) {
                if (fgets(line, sizeof(line), file) != NULL) {
                    line[strcspn(line, "\n")] = 0;
                    long entryTimeLong;
                    sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%ld", 
                           &systemData.carSpots[i].spotID, 
                           (int*)&systemData.carSpots[i].type, 
                           &systemData.carSpots[i].isOccupied, 
                           systemData.carSpots[i].licensePlate, 
                           systemData.carSpots[i].driverName, 
                           systemData.carSpots[i].driverContact, 
                           &entryTimeLong);
                    systemData.carSpots[i].entrTime = (time_t)entryTimeLong;
                }
            }
        } else if (strcmp(line, "BIKE_SPOTS") == 0) {
            for (int i = 0; i < MAX_BIKES; i++) {
                if (fgets(line, sizeof(line), file) != NULL) {
                    line[strcspn(line, "\n")] = 0;
                    long entryTimeLong;
                    sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%ld", 
                           &systemData.bikeSpots[i].spotID, 
                           (int*)&systemData.bikeSpots[i].type, 
                           &systemData.bikeSpots[i].isOccupied, 
                           systemData.bikeSpots[i].licensePlate, 
                           systemData.bikeSpots[i].driverName, 
                           systemData.bikeSpots[i].driverContact, 
                           &entryTimeLong);
                    systemData.bikeSpots[i].entrTime = (time_t)entryTimeLong;
                }
            }
        } else if (strcmp(line, "JEEP_SPOTS") == 0) {
            for (int i = 0; i < MAX_JEEPS; i++) {
                if (fgets(line, sizeof(line), file) != NULL) {
                    line[strcspn(line, "\n")] = 0;
                    long entryTimeLong;
                    sscanf(line, "%d,%d,%d,%[^,],%[^,],%[^,],%ld", 
                           &systemData.jeepSpots[i].spotID, 
                           (int*)&systemData.jeepSpots[i].type, 
                           &systemData.jeepSpots[i].isOccupied, 
                           systemData.jeepSpots[i].licensePlate, 
                           systemData.jeepSpots[i].driverName, 
                           systemData.jeepSpots[i].driverContact, 
                           &entryTimeLong);
                    systemData.jeepSpots[i].entrTime = (time_t)entryTimeLong;
                }
            }
        } else if (strcmp(line, "TRANSACTIONS") == 0) {
            if (fgets(line, sizeof(line), file) != NULL) {
                systemData.numTransactions = atoi(line);
                for (int i = 0; i < systemData.numTransactions; i++) {
                    if (fgets(line, sizeof(line), file) != NULL) {
                        line[strcspn(line, "\n")] = 0;
                        long entryTimeLong, exitTimeLong;
                        sscanf(line, "%d,%d,%[^,],%[^,],%[^,],%ld,%ld,%lf",
                               &systemData.transactions[i].spotID,
                               (int*)&systemData.transactions[i].vehicleType, 
                               systemData.transactions[i].licensePlate,
                               systemData.transactions[i].driverName,
                               systemData.transactions[i].driverContact,
                               &entryTimeLong, 
                               &exitTimeLong, 
                               &systemData.transactions[i].cost);
                        systemData.transactions[i].entryTime = (time_t)entryTimeLong;
                        systemData.transactions[i].exitTime = (time_t)exitTimeLong;
                    }
                }
            }
        } else if (strcmp(line, "CUSTOMERS") == 0) {
            if (fgets(line, sizeof(line), file) != NULL) {
                systemData.numCustomers = atoi(line);
                for (int i = 0; i < systemData.numCustomers; i++) {
                    if (fgets(line, sizeof(line), file) != NULL) {
                        line[strcspn(line, "\n")] = 0;
                        sscanf(line, "%d,%[^,],%[^,],%[^,]",
                               (int*)&systemData.customers[i].vehicleType,
                               systemData.customers[i].licensePlate,
                               systemData.customers[i].name,
                               systemData.customers[i].contact);
                    }
                }
            }
        }
    }
    fclose(file);
}


void registerAdmin(){
    if(systemData.numAdmins >= MAX_ADMINS) {
        printf("                      Maximum number of admins reached.\n");
        return;
    }

    Admin newAdmin;
    char temp_email[EMAIL_LEN];
    char temp_phone[CONTACT_LEN];
    printf("\n               --- Admin Registration ---\n");

    // Admin name
    printf("                      Enter Admin Name: ");
    fgets(newAdmin.name, sizeof(newAdmin.name), stdin);
    if (strlen(newAdmin.name) > 0 && newAdmin.name[strlen(newAdmin.name) - 1] == '\n') {
    newAdmin.name[strlen(newAdmin.name) - 1] = '\0';
    }
    if (!isValidName(newAdmin.name)) {
        printf("                      Invalid name.\n");
        return;
    }

    // Admin email
    printf("                      Enter Admin Email: ");
    fgets(temp_email, sizeof(temp_email), stdin);
    if(!isValidEmail(temp_email)) {
        printf("                      Invalid email format.\n");
        return;
    }

    // Admin phone
    printf("                      Enter Admin Phone: ");
    fgets(temp_phone, sizeof(temp_phone), stdin);
    if(!isValidPhoneNumber(temp_phone)) {
        printf("                      Invalid phone number format.\n");
        return;
    }

    // If all validations pass, set the admin data
    strcpy(newAdmin.email, temp_email);
    strcpy(newAdmin.phoneNumber, temp_phone);
    systemData.admins[systemData.numAdmins++] = newAdmin;
    printf("                      Admin registered successfully.\n");
}

int loginAdmin(){
    char name[NAME_LEN];
    char password[PASSWORD_LEN];
    printf("\n                ------- Admin Login -------\n");

    // Get admin name and password
    printf("                      Enter Admin Name: ");
    fgets(name, sizeof(name), stdin);
    printf("                      Enter Password: ");
    fgets(password, sizeof(password), stdin);

    for(int i = 0; i < systemData.numAdmins; i++) {
        if(strcmp(systemData.admins[i].name, name) == 0 && strcmp(systemData.admins[i].password, password) == 0) {
            printf("                      Login successful! Welcome, %s.\n", systemData.admins[i].name);
            return 1;
        }
    }
    printf("                      Login failed! Invalid name or password.\n");
    return 0;
}

void recordEntry(){
    int vehicleType;
    char licensePlate[LICENSE_PLATE_LEN];
    char driverName[NAME_LEN];
    char driverContact[CONTACT_LEN];
    int i, spotFound = 0;
    time_t currentTime;

    printf("\n                      --- Record Vehicle Entry ---\n");
    printf("                            Select Vehicle Type:\n");
    printf("                                1. Car\n");
    printf("                                2. Bike\n");
    printf("                                3. Jeep\n");
    printf("                             Enter your choice: ");
    scanf("%d", &vehicleType);

    if(vehicleType < 1 || vehicleType > 3) {
        printf("                      Invalid vehicle type choice.\n");
        return;
    }
    printf("                      Enter License Plate: ");
    scanf("%s", licensePlate);
    printf("                      Enter Driver Name: ");
    fgets(driverName, sizeof(driverName), stdin);
    if (strlen(driverName) > 0 && driverName[strlen(driverName) - 1] == '\n') {
        driverName[strlen(driverName) - 1] = '\0';
    }
    if (!isValidName(driverName)) {
        printf("                      Invalid name.\n");
        return;
    }
    printf("                      Enter Driver Contact: ");
    fgets(driverContact, sizeof(driverContact), stdin);
    if(!isValidPhoneNumber(driverContact)) {
        printf("                      Invalid phone number format.\n");
        return;
    }

    currentTime = time(NULL);

    switch (vehicleType) {
        // Record entry for Car
        case 1:
            for (i = 0; i < MAX_CARS; i++) {
                if (!systemData.carSpots[i].isOccupied) {
                    systemData.carSpots[i].isOccupied = 1;
                    strcpy(systemData.carSpots[i].licensePlate, licensePlate);
                    strcpy(systemData.carSpots[i].driverName, driverName);
                    strcpy(systemData.carSpots[i].driverContact, driverContact);
                    systemData.carSpots[i].entrTime = currentTime;
                    printf("                      Car parked at spot C%d. Entry time: %s", systemData.carSpots[i].spotID, ctime(&currentTime));
                    printf("                      Price per hour: %.2lf\n", systemData.carPricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("                      No available car spots.\n");
            }
            break;
        // Record entry for Bike
        case 2:
            for (i = 0; i < MAX_BIKES; i++) {
                if (!systemData.bikeSpots[i].isOccupied) {
                    systemData.bikeSpots[i].isOccupied = 1;
                    strcpy(systemData.bikeSpots[i].licensePlate, licensePlate);
                    strcpy(systemData.bikeSpots[i].driverName, driverName);
                    strcpy(systemData.bikeSpots[i].driverContact, driverContact);
                    systemData.bikeSpots[i].entrTime = currentTime;
                    printf("                      Bike parked at spot B%d. Entry time: %s", systemData.bikeSpots[i].spotID, ctime(&currentTime));
                    printf("                      Price per hour: %.2lf\n", systemData.bikePricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("                      No available bike spots.\n");
            }
            break;
        // Record entry for Jeep
        case 3:
            for (i = 0; i < MAX_JEEPS; i++) {
                if (!systemData.jeepSpots[i].isOccupied) {
                    systemData.jeepSpots[i].isOccupied = 1;
                    strcpy(systemData.jeepSpots[i].licensePlate, licensePlate);
                    strcpy(systemData.jeepSpots[i].driverName, driverName);
                    strcpy(systemData.jeepSpots[i].driverContact, driverContact);
                    systemData.jeepSpots[i].entrTime = currentTime;
                    printf("                      Jeep parked at spot J%d. Entry time: %s", systemData.jeepSpots[i].spotID, ctime(&currentTime));
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

void recordExit(){
    char licensePlate[LICENSE_PLATE_LEN];
    int i, vehicleFound = 0;
    time_t exitTime;
    double durationInSeconds, durationInHours, totalCost = 0.0;
    Transaction transaction;

    printf("\n                      --- Record Vehicle Exit ---\n");

    printf("                            Enter License Plate: ");
    fgets(licensePlate, sizeof(licensePlate), stdin);

    exitTime = time(NULL);

    for(i = 1; i <= MAX_CARS; i++) {
        if(systemData.carSpots[i].isOccupied && strcmp(systemData.carSpots[i].licensePlate, licensePlate) == 0) {
            vehicleFound = 1;
            durationInSeconds = difftime(exitTime, systemData.carSpots[i].entrTime);
            durationInHours = durationInSeconds / 3600.0;
            totalCost = durationInHours * systemData.carPricePerHour;

            printf("                      Car exited from spot C%d. Exit time: %s", systemData.carSpots[i].spotID, ctime(&exitTime));
            printf("                      Duration: %.2lf hours\n", durationInHours);
            printf("                      Total cost: %.2lf\n", totalCost);

            // Record transaction
            transaction.vehicleType = VEHICLE_TYPE_CAR;  // 1= Car
            strcpy(transaction.licensePlate, licensePlate);
            transaction.entryTime = systemData.carSpots[i].entrTime;
            transaction.exitTime = exitTime;
            transaction.cost = totalCost;
            recordTransaction(transaction);

            // Free the parking spot
            systemData.carSpots[i].isOccupied = 0;
            systemData.carSpots[i].licensePlate[0] = '\0';
            systemData.carSpots[i].driverName[0] = '\0';
            systemData.carSpots[i].driverContact[0] = '\0';
            vehicleFound = 1;
            break;
        }
    }

    if(!vehicleFound){
        for(i = 1; i <= MAX_BIKES; i++) {
            if(systemData.bikeSpots[i].isOccupied && strcmp(systemData.bikeSpots[i].licensePlate, licensePlate) == 0) {
                durationInSeconds = difftime(exitTime, systemData.bikeSpots[i].entrTime);
                durationInHours = durationInSeconds / 3600.0;
                totalCost = durationInHours * systemData.bikePricePerHour;

                printf("                      Bike exited from spot B%d. Exit time: %s", systemData.bikeSpots[i].spotID, ctime(&exitTime));
                printf("                      Duration: %.2lf hours\n", durationInHours);
                printf("                      Total cost: %.2lf\n", totalCost);

                // Record transaction
                transaction.vehicleType = VEHICLE_TYPE_BIKE;  // 2= Bike
                strcpy(transaction.licensePlate, systemData.bikeSpots[i].licensePlate);
                transaction.entryTime = systemData.bikeSpots[i].entrTime;
                transaction.exitTime = exitTime;
                transaction.cost = totalCost;
                recordTransaction(transaction);

                // Free the parking spot
                systemData.bikeSpots[i].isOccupied = 0;
                systemData.bikeSpots[i].licensePlate[0] = '\0';
                systemData.bikeSpots[i].driverName[0] = '\0';
                systemData.bikeSpots[i].driverContact[0] = '\0';
                vehicleFound = 1;
                break;
            }
        }
    }

    if(!vehicleFound){
        for(i = 1; i <= MAX_JEEPS; i++) {
            if(systemData.jeepSpots[i].isOccupied && strcmp(systemData.jeepSpots[i].licensePlate, licensePlate) == 0) {
                durationInSeconds = difftime(exitTime, systemData.jeepSpots[i].entrTime);
                durationInHours = durationInSeconds / 3600.0;
                totalCost = durationInHours * systemData.jeepPricePerHour;

                printf("                      Jeep exited from spot J%d. Exit time: %s", systemData.jeepSpots[i].spotID, ctime(&exitTime));
                printf("                      Duration: %.2lf hours\n", durationInHours);
                printf("                      Total cost: %.2lf\n", totalCost);

                // Record transaction
                transaction.vehicleType = VEHICLE_TYPE_JEEP;  // 3= Jeep
                strcpy(transaction.licensePlate, systemData.jeepSpots[i].licensePlate);
                transaction.entryTime = systemData.jeepSpots[i].entrTime;
                transaction.exitTime = exitTime;
                transaction.cost = totalCost;
                recordTransaction(transaction);

                // Free the parking spot
                systemData.jeepSpots[i].isOccupied = 0;
                systemData.jeepSpots[i].licensePlate[0] = '\0';
                systemData.jeepSpots[i].driverName[0] = '\0';
                systemData.jeepSpots[i].driverContact[0] = '\0';
                vehicleFound = 1;
                break;
            }
        }
    }

    if(!vehicleFound) {
        printf("                      No vehicle found with license plate: %s\n", licensePlate);
    }

    saveAllData();
}

void listParkedVehicles(){
    int i;
    int foundVehicle = 0;
    printf("\n                      --- List Parked Vehicles ---\n");

    printf("                      Cars:\n");
    for(i = 0; i < MAX_CARS; i++){
        if(systemData.carSpots[i].isOccupied) {
            printf("                      Spot C%d: %s (Driver: %s, Contact: %s, Entry: %s)\n",
                   systemData.carSpots[i].spotID,
                   systemData.carSpots[i].licensePlate,
                   systemData.carSpots[i].driverName,
                   systemData.carSpots[i].driverContact,
                   ctime(&systemData.carSpots[i].entrTime));
            foundVehicle = 1;
        }
    }

    if(!foundVehicle) {
        printf("                      No cars parked.\n");
    }
    foundVehicle = 0; // Reset for next

    printf("                      Bikes:\n");
    for(i = 0; i < MAX_BIKES; i++){
        if(systemData.bikeSpots[i].isOccupied) {
            printf("                      Spot B%d: %s (Driver: %s, Contact: %s, Entry: %s)\n",
                   systemData.bikeSpots[i].spotID,
                   systemData.bikeSpots[i].licensePlate,
                   systemData.bikeSpots[i].driverName,
                   systemData.bikeSpots[i].driverContact,
                   ctime(&systemData.bikeSpots[i].entrTime));
            foundVehicle = 1;
        }
    }

    if(!foundVehicle) {
        printf("                      No bikes parked.\n");
    }
    foundVehicle = 0; // Reset for next

    printf("                      Jeeps:\n");
    for(i = 0; i < MAX_JEEPS; i++){
        if(systemData.jeepSpots[i].isOccupied) {
            printf("                      Spot J%d: %s (Driver: %s, Contact: %s, Entry: %s)\n",
                   systemData.jeepSpots[i].spotID,
                   systemData.jeepSpots[i].licensePlate,
                   systemData.jeepSpots[i].driverName,
                   systemData.jeepSpots[i].driverContact,
                   ctime(&systemData.jeepSpots[i].entrTime));
            foundVehicle = 1;
        }
    }

    if(!foundVehicle) {
        printf("                      No jeeps parked.\n");
    }
    printf("                      ----------------------\n");
}

void displayAdminDetails(){
    printf("\n                      --- Admin Details ---\n");
    if(systemData.numAdmins == 0) {
        printf("                      No admin registered yet.\n");
        return;
    }
    for(int i = 0; i < systemData.numAdmins; i++) {
        printf("                      Admin %d:\n", i + 1);
        printf("                      Name: %s\n", systemData.admins[i].name);
        printf("                      Phone: %s\n", systemData.admins[i].phoneNumber);
        printf("                      Email: %s\n", systemData.admins[i].email);
        printf("                      ----------------------\n");
    }
}

void parkingRate(){
    double newCarRate, newBikeRate, newJeepRate;

    printf("\n                      --- Configure Parking Rates ---\n");

    //Car Rate
    printf("                      Current Car Rate: %.2lf per hour\n", systemData.carPricePerHour);
    printf("                      Enter new car rates:\n");
    scanf("%lf", &newCarRate);
    if (newCarRate >= 0) {
        systemData.carPricePerHour = newCarRate;
    } else {
        printf("                      Invalid rate.\n");
    }

    //Bike Rate
    printf("                      Current Bike Rate: %.2lf per hour\n", systemData.bikePricePerHour);
    printf("                      Enter new bike rates:\n");
    scanf("%lf", &newBikeRate);
    if (newBikeRate >= 0) {
        systemData.bikePricePerHour = newBikeRate;
    } else {
        printf("                      Invalid rate.\n");
    }

    //Jeep Rate
    printf("                      Current Jeep Rate: %.2lf per hour\n", systemData.jeepPricePerHour);
    printf("                      Enter new jeep rates:\n");
    scanf("%lf", &newJeepRate);
    if (newJeepRate >= 0) {
        systemData.jeepPricePerHour = newJeepRate;
    } else {
        printf("                      Invalid rate.\n");
    }

    printf("                      Parking rates updated successfully.\n");
    saveAllData();
}

void calculateDailyRevenue(){
    double currentDailyRevenue = 0.0;
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    int currentDay = localTime->tm_mday;
    int currentMonth = localTime->tm_mon + 1;
    int currentYear = localTime->tm_year + 1900;

    // Calculate daily revenue
    printf("\n                      --- Daily Revenue Summary ---\n");

    for (int i = 0; i < systemData.numTransactions; i++){
        struct tm *local_exit = localtime(&systemData.transactions[i].exitTime);
        if (local_exit->tm_year == localTime->tm_year &&
            local_exit->tm_mon == localTime->tm_mon &&
            local_exit->tm_mday == localTime->tm_mday){
            currentDailyRevenue += systemData.transactions[i].cost;
        }
    }
    printf("                      Daily Revenue for %02d/%02d/%04d: %.2lf\n",
           currentDay, currentMonth, currentYear, currentDailyRevenue);
    printf("                      --------------------------------\n");
}

void viewCustomerDetails(){
    if(systemData.numCustomers == 0){
        printf("                      No customer data available.\n");
        return;
    }

    printf("\n                      --- All Customer Details ---\n");
    for(int i = 0; i < systemData.numCustomers; i++){
        printf("                           License Plate: %s\n", systemData.customers[i].licensePlate);
        printf("                           Name: %s\n", systemData.customers[i].name);
        printf("                           Contact: %s\n", systemData.customers[i].contact);
        printf("                     --------------------------\n");
    }
}

void searchCustomerDetails(){
    char searchPlate[LICENSE_PLATE_LEN];
    printf("\n                      --- Search Customer by License Plate ---\n");
    printf("                            Enter License Plate to search: ");
    scanf("%s", searchPlate);

    Customer *foundCustomer = NULL;
    for(int i = 0; i < systemData.numCustomers; i++){
        if(strcmp(systemData.customers[i].licensePlate, searchPlate) == 0){
            foundCustomer = &systemData.customers[i];
            break;
        }
    }

    if(foundCustomer != NULL){
        printf("\n                      Customer Found:\n");
        printf("                        Vehicle Type: %s\n", foundCustomer->vehicleType);
        printf("                        License Plate: %s\n", foundCustomer->licensePlate);
        printf("                        Name: %s\n", foundCustomer->name);
        printf("                        Contact: %s\n", foundCustomer->contact);
        printf("                      --------------------\n");
    } else {
        printf("                      No customer found with license plate: %s\n", searchPlate);
    }
}

void displayParkingStatus(){
    int i;
    printf("\n                      --- Current Parking Status ---\n");
    
    printf("                      Cars:\n");
    for (i = 0; i < MAX_CARS; i++) {
        if(systemData.carSpots[i].isOccupied) {
            printf("                           License Plate: %s\n", systemData.carSpots[i].licensePlate);
            printf("                           Driver Name: %s\n", systemData.carSpots[i].driverName);
            printf("                           Contact: %s\n", systemData.carSpots[i].driverContact);
            printf("                           Parking Spot: %d\n", systemData.carSpots[i].spotID);
            printf("                     --------------------------\n");
        }
    }

    printf("                      Bikes:\n");
    for (i = 0; i < MAX_BIKES; i++) {
        if(systemData.bikeSpots[i].isOccupied) {
            printf("                           License Plate: %s\n", systemData.bikeSpots[i].licensePlate);
            printf("                           Driver Name: %s\n", systemData.bikeSpots[i].driverName);
            printf("                           Contact: %s\n", systemData.bikeSpots[i].driverContact);
            printf("                           Parking Spot: %d\n", systemData.bikeSpots[i].spotID);
            printf("                     --------------------------\n");
        }
    }

    printf("                      Jeeps:\n");
    for (i = 0; i < MAX_JEEPS; i++) {
        if(systemData.jeepSpots[i].isOccupied) {
            printf("                           License Plate: %s\n", systemData.jeepSpots[i].licensePlate);
            printf("                           Driver Name: %s\n", systemData.jeepSpots[i].driverName);
            printf("                           Contact: %s\n", systemData.jeepSpots[i].driverContact);
            printf("                           Parking Spot: %d\n", systemData.jeepSpots[i].spotID);
            printf("                     --------------------------\n");
        }
    }
}

void displayEmptySpots(){
    int i;
    int emptyCars = 0;
    int emptyBikes = 0;
    int emptyJeeps = 0;

    for (i = 0; i < MAX_CARS; i++) {
        if (!systemData.carSpots[i].isOccupied) emptyCars++;
    }
    for (i = 0; i < MAX_BIKES; i++) {
        if (!systemData.bikeSpots[i].isOccupied) emptyBikes++;
    }
    for (i = 0; i < MAX_JEEPS; i++) {
        if (!systemData.jeepSpots[i].isOccupied) emptyJeeps++;
    }

    printf("\n                      --- Total Empty Spots ---\n");
    printf("                            Empty Car Spots: %d/%d\n", emptyCars, MAX_CARS);
    printf("                            Empty Bike Spots: %d/%d\n", emptyBikes, MAX_BIKES);
    printf("                            Empty Jeep Spots: %d/%d\n", emptyJeeps, MAX_JEEPS);
    printf("                            Total Empty Spots: %d\n", emptyCars + emptyBikes + emptyJeeps);
    printf("                        -------------------------\n");
}

void displayTotalAmount(){
    double totalAmount = 0.0;
    for(int i = 0; i < systemData.numTransactions; i++){
        totalAmount += systemData.transactions[i].cost;
    }
    printf("\n                      --- Total Amount Collected ---\n");
    printf("                            Total Amount: %.2lf\n", totalAmount);
    printf("                        ------------------------------\n");
}

void adminMenu(){
    int choice;
    do {
        printf("\n                            --- Admin Menu ---\n");
        printf("                            1. Admin Details\n");
        printf("                            2. Configure Parking Rates\n");
        printf("                            3. Display Total Amount Collected\n");
        printf("                            4. Main Menu\n");
        printf("                            Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                displayAdminDetails();
                break;
            case 2:
                parkingRate();
                break;
            case 3:
                displayTotalAmount();
                break;
            case 4:
                printf("                            Exiting Admin Menu...\n");
                break;
            default:
                printf("                            Invalid choice. Please try again.\n");
        }
    } while(choice != 4);
}

void vehicleMenu(){
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

void customerMenu(){
    int choice;
    do {
        printf("\n                      --- Customer Info ---\n");
        printf("                      1. View All Customer Details\n");
        printf("                      2. Search Customer by License Plate\n");
        printf("                      3. Back to Main Menu\n");
        printf("                      Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewCustomerDetails();
                break;
            case 2:
                searchCustomerDetails();
                break;
            case 3:
                break;
            default:
                printf("                      Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void slotManagementMenu(){
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
                displayEmptySpots();
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
        printf("           ................................................................\n");
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
