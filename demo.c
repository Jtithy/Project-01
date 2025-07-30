#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CARS 5
#define MAX_BIKES 5
#define MAX_JEEPS 5
#define LICENSE_PLATE_LEN 20

typedef struct {
    int spotId;
    int type; // 0 for Car, 1 for Bike, 2 for Jeep
    int isOccupied;
    char licensePlate[LICENSE_PLATE_LEN];
    time_t entryTime;
} ParkingSpot;

// Global arrays for parking spots
ParkingSpot carSpots[MAX_CARS];
ParkingSpot bikeSpots[MAX_BIKES];
ParkingSpot jeepSpots[MAX_JEEPS];

// Global variables for pricing and revenue
double carPricePerHour = 200.0;
double bikePricePerHour = 100.0;
double jeepPricePerHour = 150.0;
double totalDailyRevenue = 0.0;

// Initializes all parking spots to be available
void initializeParkingSpots() {
    int i;
    for (i = 0; i < MAX_CARS; i++) {
        carSpots[i].spotId = i + 1;
        carSpots[i].type = 0; // Car
        carSpots[i].isOccupied = 0;
        memset(carSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
    }
    for (i = 0; i < MAX_BIKES; i++) {
        bikeSpots[i].spotId = i + 1;
        bikeSpots[i].type = 1; // Bike
        bikeSpots[i].isOccupied = 0;
        memset(bikeSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
    }
    for (i = 0; i < MAX_JEEPS; i++) {
        jeepSpots[i].spotId = i + 1;
        jeepSpots[i].type = 2; // Jeep
        jeepSpots[i].isOccupied = 0;
        memset(jeepSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
    }
}

// Records a vehicle entry, assigns a spot, and timestamps it
void recordEntry() {
    int vehicleTypeChoice;
    char licensePlate[LICENSE_PLATE_LEN];
    int i, spotFound = 0;
    time_t currentTime;

    printf("Select vehicle type:\n");
    printf("1. Car\n");
    printf("2. Bike\n");
    printf("3. Jeep\n");
    printf("Enter your choice: ");
    scanf("%d", &vehicleTypeChoice);

    printf("Enter license plate: ");
    scanf("%s", licensePlate);

    currentTime = time(NULL);

    switch (vehicleTypeChoice) {
        case 1: // Car
            for (i = 0; i < MAX_CARS; i++) {
                if (!carSpots[i].isOccupied) {
                    carSpots[i].isOccupied = 1;
                    strcpy(carSpots[i].licensePlate, licensePlate);
                    carSpots[i].entryTime = currentTime;
                    printf("Car parked at spot C%d. Entry time: %s", carSpots[i].spotId, ctime(&currentTime));
                    printf("Price per hour: %.2lf\n", carPricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("No available car spots.\n");
            }
            break;
        case 2: // Bike
            for (i = 0; i < MAX_BIKES; i++) {
                if (!bikeSpots[i].isOccupied) {
                    bikeSpots[i].isOccupied = 1;
                    strcpy(bikeSpots[i].licensePlate, licensePlate);
                    bikeSpots[i].entryTime = currentTime;
                    printf("Bike parked at spot B%d. Entry time: %s", bikeSpots[i].spotId, ctime(&currentTime));
                    printf("Price per hour: %.2lf\n", bikePricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("No available bike spots.\n");
            }
            break;
        case 3: // Jeep
            for (i = 0; i < MAX_JEEPS; i++) {
                if (!jeepSpots[i].isOccupied) {
                    jeepSpots[i].isOccupied = 1;
                    strcpy(jeepSpots[i].licensePlate, licensePlate);
                    jeepSpots[i].entryTime = currentTime;
                    printf("Jeep parked at spot J%d. Entry time: %s", jeepSpots[i].spotId, ctime(&currentTime));
                    printf("Price per hour: %.2lf\n", jeepPricePerHour);
                    spotFound = 1;
                    break;
                }
            }
            if (!spotFound) {
                printf("No available jeep spots.\n");
            }
            break;
        default:
            printf("Invalid vehicle type choice.\n");
    }
}

// Records a vehicle exit, calculates duration and cost, and updates revenue
void recordExit() {
    char licensePlate[LICENSE_PLATE_LEN];
    int i, vehicleFound = 0;
    time_t exitTime;
    double durationInSeconds, durationInHours, totalCost = 0.0;

    printf("Enter license plate of vehicle to exit: ");
    scanf("%s", licensePlate);

    exitTime = time(NULL);

    // Check car spots
    for (i = 0; i < MAX_CARS; i++) {
        if (carSpots[i].isOccupied && strcmp(carSpots[i].licensePlate, licensePlate) == 0) {
            durationInSeconds = difftime(exitTime, carSpots[i].entryTime);
            durationInHours = durationInSeconds / 3600.0;
            totalCost = durationInHours * carPricePerHour;
            totalDailyRevenue += totalCost; // Add to daily revenue

            printf("Vehicle exited from spot C%d.\n", carSpots[i].spotId);
            printf("Entry time: %s", ctime(&carSpots[i].entryTime));
            printf("Exit time: %s", ctime(&exitTime));
            printf("Duration: %.2f hours\n", durationInHours);
            printf("Total cost: %.2lf\n", totalCost);

            carSpots[i].isOccupied = 0;
            memset(carSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
            vehicleFound = 1;
            break;
        }
    }

    // If not found in car spots, check bike spots
    if (!vehicleFound) {
        for (i = 0; i < MAX_BIKES; i++) {
            if (bikeSpots[i].isOccupied && strcmp(bikeSpots[i].licensePlate, licensePlate) == 0) {
                durationInSeconds = difftime(exitTime, bikeSpots[i].entryTime);
                durationInHours = durationInSeconds / 3600.0;
                totalCost = durationInHours * bikePricePerHour;
                totalDailyRevenue += totalCost; // Add to daily revenue

                printf("Vehicle exited from spot B%d.\n", bikeSpots[i].spotId);
                printf("Entry time: %s", ctime(&bikeSpots[i].entryTime));
                printf("Exit time: %s", ctime(&exitTime));
                printf("Duration: %.2f hours\n", durationInHours);
                printf("Total cost: %.2lf\n", totalCost);

                bikeSpots[i].isOccupied = 0;
                memset(bikeSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
                vehicleFound = 1;
                break;
            }
        }
    }

    // If not found in bike spots, check jeep spots
    if (!vehicleFound) {
        for (i = 0; i < MAX_JEEPS; i++) {
            if (jeepSpots[i].isOccupied && strcmp(jeepSpots[i].licensePlate, licensePlate) == 0) {
                durationInSeconds = difftime(exitTime, jeepSpots[i].entryTime);
                durationInHours = durationInSeconds / 3600.0;
                totalCost = durationInHours * jeepPricePerHour;
                totalDailyRevenue += totalCost; // Add to daily revenue

                printf("Vehicle exited from spot J%d.\n", jeepSpots[i].spotId);
                printf("Entry time: %s", ctime(&jeepSpots[i].entryTime));
                printf("Exit time: %s", ctime(&exitTime));
                printf("Duration: %.2f hours\n", durationInHours);
                printf("Total cost: %.2lf\n", totalCost);

                jeepSpots[i].isOccupied = 0;
                memset(jeepSpots[i].licensePlate, 0, LICENSE_PLATE_LEN);
                vehicleFound = 1;
                break;
            }
        }
    }

    if (!vehicleFound) {
        printf("Vehicle with license plate %s not found in parking.\n", licensePlate);
    }
}

// Displays the current occupancy status of all parking spots
void displayParkingStatus() {
    int i;
    printf("\n--- Parking Status ---\n");

    printf("Cars:\n");
    for (i = 0; i < MAX_CARS; i++) {
        printf("Spot C%d: %s", carSpots[i].spotId, carSpots[i].isOccupied ? carSpots[i].licensePlate : "Available");
        if (carSpots[i].isOccupied) {
            printf(" (Entry: %s)", ctime(&carSpots[i].entryTime));
        }
        printf("\n");
    }

    printf("Bikes:\n");
    for (i = 0; i < MAX_BIKES; i++) {
        printf("Spot B%d: %s", bikeSpots[i].spotId, bikeSpots[i].isOccupied ? bikeSpots[i].licensePlate : "Available");
        if (bikeSpots[i].isOccupied) {
            printf(" (Entry: %s)", ctime(&bikeSpots[i].entryTime));
        }
        printf("\n");
    }

    printf("Jeeps:\n");
    for (i = 0; i < MAX_JEEPS; i++) {
        printf("Spot J%d: %s", jeepSpots[i].spotId, jeepSpots[i].isOccupied ? jeepSpots[i].licensePlate : "Available");
        if (jeepSpots[i].isOccupied) {
            printf(" (Entry: %s)", ctime(&jeepSpots[i].entryTime));
        }
        printf("\n");
    }
    printf("----------------------\n");
}

// Allows configuration of hourly parking rates for different vehicle types
void configureParkingRates() {
    double newCarRate, newBikeRate, newJeepRate;

    printf("\n--- Configure Parking Rates ---\n");
    printf("Current Car Rate: %.2lf per hour\n", carPricePerHour);
    printf("Enter new Car Rate: ");
    scanf("%lf", &newCarRate);
    if (newCarRate >= 0) {
        carPricePerHour = newCarRate;
    } else {
        printf("Invalid rate. Rate must be non-negative.\n");
    }

    printf("Current Bike Rate: %.2lf per hour\n", bikePricePerHour);
    printf("Enter new Bike Rate: ");
    scanf("%lf", &newBikeRate);
    if (newBikeRate >= 0) {
        bikePricePerHour = newBikeRate;
    } else {
        printf("Invalid rate. Rate must be non-negative.\n");
    }

    printf("Current Jeep Rate: %.2lf per hour\n", jeepPricePerHour);
    printf("Enter new Jeep Rate: ");
    scanf("%lf", &newJeepRate);
    if (newJeepRate >= 0) {
        jeepPricePerHour = newJeepRate;
    } else {
        printf("Invalid rate. Rate must be non-negative.\n");
    }
    printf("Parking rates updated successfully.\n");
}

// Lists all vehicles currently parked in the lot
void listParkedVehicles() {
    int i;
    int foundVehicles = 0;
    printf("\n--- Currently Parked Vehicles ---\n");

    printf("Cars:\n");
    for (i = 0; i < MAX_CARS; i++) {
        if (carSpots[i].isOccupied) {
            printf("  Spot C%d: License Plate: %s, Entry Time: %s", carSpots[i].spotId, carSpots[i].licensePlate, ctime(&carSpots[i].entryTime));
            foundVehicles = 1;
        }
    }
    if (!foundVehicles) {
        printf("  No cars currently parked.\n");
    }
    foundVehicles = 0; // Reset for next vehicle type

    printf("Bikes:\n");
    for (i = 0; i < MAX_BIKES; i++) {
        if (bikeSpots[i].isOccupied) {
            printf("  Spot B%d: License Plate: %s, Entry Time: %s", bikeSpots[i].spotId, bikeSpots[i].licensePlate, ctime(&bikeSpots[i].entryTime));
            foundVehicles = 1;
        }
    }
    if (!foundVehicles) {
        printf("  No bikes currently parked.\n");
    }
    foundVehicles = 0; // Reset for next vehicle type

    printf("Jeeps:\n");
    for (i = 0; i < MAX_JEEPS; i++) {
        if (jeepSpots[i].isOccupied) {
            printf("  Spot J%d: License Plate: %s, Entry Time: %s", jeepSpots[i].spotId, jeepSpots[i].licensePlate, ctime(&jeepSpots[i].entryTime));
            foundVehicles = 1;
        }
    }
    if (!foundVehicles) {
        printf("  No jeeps currently parked.\n");
    }
    printf("---------------------------------\n");
}

// Displays the total revenue generated since the program started
void displayDailyRevenueSummary() {
    printf("\n--- Daily Revenue Summary ---\n");
    printf("Total Revenue Generated: %.2lf\n", totalDailyRevenue);
    printf("-----------------------------\n");
}

int main() {
    int choice;
    initializeParkingSpots(); // Initialize parking spots at program start

    do {
        printf("\n--- Parking Lot Management System ---\n");
        printf("1. Record Vehicle Entry\n");
        printf("2. Record Vehicle Exit\n");
        printf("3. Display Parking Status\n");
        printf("4. Configure Parking Rates\n");
        printf("5. List Parked Vehicles\n");
        printf("6. Display Daily Revenue Summary\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                recordEntry();
                break;
            case 2:
                recordExit();
                break;
            case 3:
                displayParkingStatus();
                break;
            case 4:
                configureParkingRates();
                break;
            case 5:
                listParkedVehicles();
                break;
            case 6:
                displayDailyRevenueSummary();
                break;
            case 7:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
