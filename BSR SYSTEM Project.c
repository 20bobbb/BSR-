#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SCHEDULES 10
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_SEATS 50
#define MAX_RESERVATIONS 100

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Passengers;

Passengers passengers[] = {
    {"tracy", "yolo"},
    {"user2", "password2"},
    {"user3", "password3"}
};

int numPassengers = sizeof(passengers) / sizeof(Passengers);

int authenticateUser(char* username, char* password, Passengers* users, int numUsers) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;  // Authentication successful
        }
    }
    return 0;  // Authentication failed
}

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Employee;

Employee employees[] = {
    {"employee1", "emp_pass1"},
    {"employee2", "emp_pass2"},
};

int numEmployees = sizeof(employees) / sizeof(Employee);

int authenticateEmployee(char* enteredUsername, char* enteredPassword, Employee* employees, int numEmployees) {
    for (int i = 0; i < numEmployees; i++) {
        if (strcmp(employees[i].username, enteredUsername) == 0 && strcmp(employees[i].password, enteredPassword) == 0) {
            return 1;  // Authentication successful
        }
    }
    return 0;  // Authentication failed
}

struct BusSchedule {
    char departureTime[10];
    char source[50];
    char destination[50];
};

typedef struct {
    char passengerName[50];
    int seatNumber;
    float fare;
    char departureTime[10];
} Reservation;

typedef struct {
    char destination[20];
    int available_seats;
    Reservation reservations[MAX_SEATS];
    char departureTime[10];
} BusRoute;

typedef struct {
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations; // Track the number of reservations
} BookingHistory;

int authenticateUser(char* username, char* password, Passengers* users, int numUsers);
void employeeModule(struct BusSchedule schedules[], int *scheduleCount, BusRoute routes[], int numRoutes);
void passengerModule(BusRoute routes[], int numRoutes, struct BusSchedule schedules[], int scheduleCount, BookingHistory *history);

void addSchedule(struct BusSchedule schedules[], int *scheduleCount) {
    if (*scheduleCount < MAX_SCHEDULES) {
        printf("Enter Departure Time: ");
        scanf("%9s", schedules[*scheduleCount].departureTime); // Limit input to 9 characters to prevent buffer overflow

        printf("Enter Source: ");
        scanf("%49s", schedules[*scheduleCount].source); // Limit input to 49 characters to prevent buffer overflow

        printf("Enter Destination: ");
        scanf("%49s", schedules[*scheduleCount].destination); // Limit input to 49 characters to prevent buffer overflow

        (*scheduleCount)++;
        printf("Bus schedule added successfully!\n");
    } else {
        printf("Maximum schedules reached. Delete or modify existing schedules.\n");
    }
}

void deleteSchedule(struct BusSchedule schedules[], int *scheduleCount) {
    int index;
    printf("Enter the index of the schedule to delete (0-%d): ", *scheduleCount - 1);
    scanf("%d", &index);

    if (index >= 0 && index < *scheduleCount) {
        for (int i = index; i < *scheduleCount - 1; i++) {
            strcpy(schedules[i].departureTime, schedules[i + 1].departureTime);
            strcpy(schedules[i].source, schedules[i + 1].source);
            strcpy(schedules[i].destination, schedules[i + 1].destination);
        }
        (*scheduleCount)--;
        printf("Bus schedule deleted successfully!\n");
    } else {
        printf("Invalid index. Please enter a valid index.\n");
    }
}

void modifySchedule(struct BusSchedule schedules[], int scheduleCount) {
    int index;
    printf("Enter the index of the schedule to modify (0-%d): ", scheduleCount - 1);
    scanf("%d", &index);

    if (index >= 0 && index < scheduleCount) {
        printf("Enter Departure Time: ");
        scanf("%9s", schedules[index].departureTime); // Limit input to 9 characters to prevent buffer overflow

        printf("Enter Source: ");
        scanf("%49s", schedules[index].source); // Limit input to 49 characters to prevent buffer overflow

        printf("Enter Destination: ");
        scanf("%49s", schedules[index].destination); // Limit input to 49 characters to prevent buffer overflow

        printf("Bus schedule modified successfully!\n");
    } else {
        printf("Invalid index. Please enter a valid index.\n");
    }
}

void viewSchedules(struct BusSchedule schedules[], int scheduleCount) {
    printf("Bus Schedules:\n");
    for (int i = 0; i < scheduleCount; i++) {
        printf("Index: %d\n", i);
        printf("Departure Time: %s\n", schedules[i].departureTime);
        printf("Source: %s\n", schedules[i].source);
        printf("Destination: %s\n", schedules[i].destination);
        printf("\n");
    }
}

void initializeRoutes(BusRoute routes[], int numRoutes, struct BusSchedule schedules[], int scheduleCount) {
    for (int i = 0; i < numRoutes; i++) {
        routes[i].available_seats = MAX_SEATS;

        // Assuming the schedules array is sorted by departure time
        strncpy(routes[i].destination, schedules[i].destination, sizeof(routes[i].destination) - 1);
        routes[i].destination[sizeof(routes[i].destination) - 1] = '\0'; // Ensure null-termination
        strncpy(routes[i].departureTime, schedules[i].departureTime, sizeof(routes[i].departureTime) - 1);
        routes[i].departureTime[sizeof(routes[i].departureTime) - 1] = '\0'; // Ensure null-termination
    }
}

void displayAvailableSeats(BusRoute routes[], int numRoutes) {
    printf("Available Seats:\n");
    for (int i = 0; i < numRoutes; i++) {
        printf("To %s: %d\n", routes[i].destination, routes[i].available_seats);
    }
}

int reserveSeat(BusRoute *route, Reservation *reservation, BookingHistory *history) {
    if (route->available_seats <= 0) {
        printf("No available seats for this route.\n");
        strncpy(reservation->departureTime, route->departureTime, sizeof(reservation->departureTime) - 1);
        reservation->departureTime[sizeof(reservation->departureTime) - 1] = '\0'; // Ensure null-termination
        return 0; // Reservation failed
    }

    char name[50];
    printf("Enter your name: ");
    scanf("%49s", name); // Limit input to 49 characters to prevent buffer overflow

    // Set the passenger's name in the reservation
    strncpy(reservation->passengerName, name, sizeof(reservation->passengerName) - 1);
    reservation->passengerName[sizeof(reservation->passengerName) - 1] = '\0'; // Ensure null-termination

    // Set the departure time in the reservation
    strncpy(reservation->departureTime, route->departureTime, sizeof(reservation->departureTime) - 1);
    reservation->departureTime[sizeof(reservation->departureTime) - 1] = '\0'; // Ensure null-termination

    int reservationID = history->numReservations; // Use the history counter as reservation ID

    route->reservations[reservationID].seatNumber = reservationID + 1;

    route->available_seats--;

    // Assign a unique reservation ID
    reservation->seatNumber = reservationID + 1;
    reservation->fare = 0.0; // Initial fare

    // Add the reservation to the booking history
    history->reservations[history->numReservations] = *reservation;
    history->numReservations++;

    return 1; // Reservation successful
}

int processPayment(Reservation *reservation, float amount) {
    printf("Processing payment of Ksh%.2f for reservation ID %d.\n", amount, reservation->seatNumber);
    reservation->fare = amount;
    return 0;
}

void generateReceipt(Reservation *reservation, float fare) {
    printf("------ Bus Reservation Receipt ------\n");
    printf("Reservation ID: %d\n", reservation->seatNumber);
    printf("Passenger Name: %s\n", reservation->passengerName);
    printf("Seat Number: %d\n", reservation->seatNumber);
    printf("Fare: Ksh%.2f\n", fare);
    printf("Departure Time: %s\n", reservation->departureTime);
    printf("------------------------------------\n");
}

void displayBookingHistoryDetails(BookingHistory *history) {
    printf("Booking History:\n");
    for (int i = 0; i < history->numReservations; i++) {
        printf("Details for Reservation ID %d:\n", history->reservations[i].seatNumber);
        printf("Passenger Name: %s\n", history->reservations[i].passengerName);
        printf("Seat Number: %d\n", history->reservations[i].seatNumber);
        printf("Fare: Ksh%.2f\n", history->reservations[i]. fare);
        printf("Departure Time: %s\n", history->reservations[i].departureTime);
        printf("\n");
    }
}

int main() {
    struct BusSchedule schedules[MAX_SCHEDULES];
    int scheduleCount = 2;

    strncpy(schedules[0].departureTime, "06:00 AM", sizeof(schedules[0].departureTime) - 1);
    schedules[0].departureTime[sizeof(schedules[0].departureTime) - 1] = '\0'; // Ensure null-termination
    strncpy(schedules[0].source, "Nairobi", sizeof(schedules[0].source) - 1);
    schedules[0].source[sizeof(schedules[0].source) - 1] = '\0'; // Ensure null-termination
    strncpy(schedules[0].destination, "Nakuru", sizeof(schedules[0].destination) - 1);
    schedules[0].destination[sizeof(schedules[0].destination) - 1] = '\0'; // Ensure null-termination

    strncpy(schedules[1].departureTime, "7:00 AM", sizeof(schedules[1].departureTime) - 1);
    schedules[1].departureTime[sizeof(schedules[1].departureTime) - 1] = '\0'; // Ensure null-termination
    strncpy(schedules[1].source, "Nairobi", sizeof(schedules[1].source) - 1);
    schedules[1].source[sizeof(schedules[1].source) - 1] = '\0'; // Ensure null-termination
    strncpy(schedules[1].destination, "Kisumu", sizeof(schedules[1].destination) - 1);
    schedules[1].destination[sizeof(schedules[1].destination) - 1] = '\0'; // Ensure null-termination

    BusRoute routes[2];
    initializeRoutes(routes, sizeof(routes) / sizeof(routes[0]), schedules, scheduleCount);

    BookingHistory history;
    history.numReservations = 0;

    int choice;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    do {
        printf("Login\n");
        printf("1. Employee Login\n");
        printf("2. Passenger Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Employee Login\n");
                printf("Username: ");
                scanf("%19s", username); // Limit input to 19 characters to prevent buffer overflow
                printf("Password: ");
                scanf("%19s", password); // Limit input to 19 characters to prevent buffer overflow

                if (authenticateEmployee(username, password, employees, numEmployees)) {
                    printf("Employee login successful!\n");
                    employeeModule(schedules, &scheduleCount, routes, sizeof(routes) / sizeof(routes[0]));
                } else {
                    printf("Authentication failed. Invalid username or password.\n");
                }
                break;

            case 2:
                printf("Passenger Login\n");
                printf("Username: ");
                scanf("%19s", username); // Limit input to 19 characters to prevent buffer overflow
                printf("Password: ");
                scanf("%19s", password); // Limit input to 19 characters to prevent buffer overflow

                if (authenticateUser(username, password, passengers, numPassengers)) {
                    printf("Passenger login successful!\n");
                    passengerModule(routes, sizeof(routes) / sizeof(routes[0]), schedules, scheduleCount, &history);
                } else {
                    printf("Authentication failed. Invalid username or password.\n");
                }
                break;

            case 3:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 3);

    return 0;
}

void employeeModule(struct BusSchedule schedules[], int *scheduleCount, BusRoute routes[], int numRoutes) {
    int choice;

    do {
        printf("\nEmployee Module\n");
        printf("1. Add Bus Schedule\n");
        printf("2. Delete Bus Schedule\n");
        printf("3. Modify Bus Schedule\n");
        printf("4. View All Bus Schedules\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addSchedule(schedules, scheduleCount);
                // After adding a new schedule, update routes
                initializeRoutes(routes, numRoutes, schedules, *scheduleCount);
                break;
            case 2:
                deleteSchedule(schedules, scheduleCount);
                // After deleting a schedule, update routes
                initializeRoutes(routes, numRoutes, schedules, *scheduleCount);
                break;
            case 3:
                modifySchedule(schedules, *scheduleCount);
                // After modifying schedules, update routes
                initializeRoutes(routes, numRoutes, schedules, *scheduleCount);
                break;
            case 4:
                viewSchedules(schedules, *scheduleCount);
                break;
            case 5:
                printf("Returning to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 5);
}


void passengerModule(BusRoute routes[], int numRoutes, struct BusSchedule schedules[], int scheduleCount, BookingHistory *history) {
    printf("Passenger Module\n");

    int choice;
    int routeChoice;

    do {
    	Reservation reservation1;
    	
        printf("\nMenu:\n");
        printf("1. Display available seats\n");
        printf("2. Reserve a seat\n");
        printf("3. Make payment\n");
        printf("4. Generate receipt\n");
        printf("5. View Booking History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAvailableSeats(routes, numRoutes);
                break;

            case 2:
                printf("Select route (1. for Nakuru, 2. for Kisumu): ");
                scanf("%d", &routeChoice);
                if (routeChoice >= 1 && routeChoice <= numRoutes) {
                    int reservationStatus = reserveSeat(&routes[routeChoice - 1], &reservation1, history);
                    if (reservationStatus == 1) {
                        printf("Reservation successful.\n");
                    } else {
                        printf("Reservation failed.\n");
                    }
                } else {
                    printf("Invalid route choice.\n");
                }
                break;

            case 3:
                if (reservation1.fare == 0) {
                    float amountToPay = 400.00;
                    int paymentStatus = processPayment(&reservation1, amountToPay);
                    if (paymentStatus == 0) {
                        printf("Payment successful.\n");
                        printf("Updated fare for reservation ID %d: Ksh%.2f\n", reservation1.seatNumber, reservation1.fare);
                    } else {
                        printf("Payment failed.\n");
                    }
                } else {
                    printf("Payment has already been processed.\n");
                }
                break;

            case 4:
            	generateReceipt(&reservation1, reservation1.fare); 
                break;
            case 5:
                displayBookingHistoryDetails(history);
                break;

            case 6:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

	
