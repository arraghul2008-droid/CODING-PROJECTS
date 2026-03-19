#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int flightNo;
    char destination[50];
    int availableSeats;
} Flight;

typedef struct {
    int flightNo;
} Booking;

const char *FLIGHTS_FILE = "flights.dat";
const char *BOOKINGS_FILE = "bookings.dat";

void initializeFlights();
void viewFlights();
void bookFlight();
void cancelReservation();
void viewBookings();
void updateSeatCount(int flightNo, int change);

int main() {

    initializeFlights();

    int choice;
    while (1) {
        printf("\n===== FLIGHT RESERVATION SYSTEM =====\n");
        printf("1. View Available Flights\n");
        printf("2. Book a Flight\n");
        printf("3. Cancel Reservation\n");
        printf("4. View My Bookings\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1: viewFlights(); break;
            case 2: bookFlight(); break;
            case 3: cancelReservation(); break;
            case 4: viewBookings(); break;
            case 5: exit(0);
            default: printf("Invalid selection!\n");
        }
    }
    return 0;
}

void initializeFlights() {
    FILE *file = fopen(FLIGHTS_FILE, "rb");
    if (file == NULL) {
        file = fopen(FLIGHTS_FILE, "wb");
        Flight initial[] = {
            {101, "New York", 50},
            {102, "London", 30},
            {103, "Tokyo", 20},
            {104, "Dubai", 15}
        };
        fwrite(initial, sizeof(Flight), 4, file);
    }
    fclose(file);
}

void viewFlights() {
    FILE *file = fopen(FLIGHTS_FILE, "rb");
    Flight f;
    printf("\n%-10s %-20s %-15s\n", "ID", "Destination", "Seats Available");
    printf("----------------------------------------------\n");
    while (fread(&f, sizeof(Flight), 1, file)) {
        printf("%-10d %-20s %-15d\n", f.flightNo, f.destination, f.availableSeats);
    }
    fclose(file);
}

void bookFlight() {
    int fNo, found = 0;
    printf("Enter Flight Number to book: ");
    scanf("%d", &fNo);

    FILE *file = fopen(FLIGHTS_FILE, "rb");
    Flight f;
    while (fread(&f, sizeof(Flight), 1, file)) {
        if (f.flightNo == fNo) {
            found = 1;
            if (f.availableSeats > 0) {
                
                FILE *bFile = fopen(BOOKINGS_FILE, "ab");
                Booking b = {fNo};
                fwrite(&b, sizeof(Booking), 1, bFile);
                fclose(bFile);

                
                updateSeatCount(fNo, -1);
                printf("Booking Confirmed for Flight %d to %s!\n", fNo, f.destination);
            } else {
                printf("Sorry, no seats available on this flight.\n");
            }
            break;
        }
    }
    if (!found) printf("Flight not found.\n");
    fclose(file);
}

void cancelReservation() {
    int fNo, found = 0;
    printf("Enter Flight Number to cancel: ");
    scanf("%d", &fNo);

    FILE *file = fopen(BOOKINGS_FILE, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!file) {
        printf("No active bookings found.\n");
        return;
    }

    Booking b;
    while (fread(&b, sizeof(Booking), 1, file)) {
        if (b.flightNo == fNo && !found) {
            found = 1; 
            updateSeatCount(fNo, 1); 
        } else {
            fwrite(&b, sizeof(Booking), 1, temp);
        }
    }
    fclose(file);
    fclose(temp);

    remove(BOOKINGS_FILE);
    rename("temp.dat", BOOKINGS_FILE);

    if (found) printf("Reservation for Flight %d cancelled successfully.\n", fNo);
    else printf("No booking found with that Flight Number.\n");
}

void viewBookings() {
    FILE *file = fopen(BOOKINGS_FILE, "rb");
    if (!file) {
        printf("\nYou have no current bookings.\n");
        return;
    }

    Booking b;
    printf("\n--- Your Current Bookings ---\n");
    while (fread(&b, sizeof(Booking), 1, file)) {
        printf("Flight Number: %d\n", b.flightNo);
    }
    fclose(file);
}

void updateSeatCount(int flightNo, int change) {
    FILE *file = fopen(FLIGHTS_FILE, "rb+"); 
    Flight f;
    while (fread(&f, sizeof(Flight), 1, file)) {
        if (f.flightNo == flightNo) {
            f.availableSeats += change;
            fseek(file, -(long)sizeof(Flight), SEEK_CUR); 
            fwrite(&f, sizeof(Flight), 1, file);
            break;
        }
    }
    fclose(file);
}