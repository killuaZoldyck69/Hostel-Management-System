#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

// Constants for floors, rooms, and seats
const int FLOORS = 9;           // Total floors (excluding ground)
const int ROOMS_PER_FLOOR = 10; // Rooms per floor
const int SEATS_PER_ROOM = 5;   // Seats per room

// Booking data structure
struct Booking
{
    string name;
    string studentId;
    int semester;
    int floor;
    int room;
    int seat;
};

// Hostel availability array
bool availability[FLOORS][ROOMS_PER_FLOOR][SEATS_PER_ROOM];

// Function to initialize availability
void initializeAvailability()
{
    for (int i = 0; i < FLOORS; i++)
    {
        for (int j = 0; j < ROOMS_PER_FLOOR; j++)
        {
            for (int k = 0; k < SEATS_PER_ROOM; k++)
            {
                availability[i][j][k] = true; // All seats are available initially
            }
        }
    }
}

// Function to load booking data from the file and update availability
void loadBookingData()
{
    ifstream file("booking_records.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            // Parse the line to extract booking details
            stringstream ss(line);
            Booking booking;
            string temp;

            getline(ss, temp, ':');              // Skip "Name:"
            getline(ss, booking.name, ',');      // Read name
            getline(ss, temp, ':');              // Skip "ID:"
            getline(ss, booking.studentId, ','); // Read ID
            getline(ss, temp, ':');              // Skip "Semester:"
            ss >> booking.semester;
            getline(ss, temp, ':'); // Skip "Floor:"
            ss >> booking.floor;
            getline(ss, temp, ':'); // Skip "Room:"
            ss >> booking.room;
            getline(ss, temp, ':'); // Skip "Seat:"
            ss >> booking.seat;

            // Update availability
            availability[booking.floor - 1][booking.room - 1][booking.seat - 1] = false;
        }
        file.close();
    }
}

// Function to check if a student ID already exists in the file
bool isStudentIdAlreadyBooked(const string &studentId)
{
    ifstream file("booking_records.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line.find("ID: " + studentId) != string::npos)
            {
                return true; // ID already exists
            }
        }
        file.close();
    }
    return false;
}

// Function to display available floors
void displayAvailableFloors()
{
    cout << "Available Floors:\n";
    for (int i = 1; i <= FLOORS; i++)
    { // Floors start from 1
        cout << "Floor " << i << "\n";
    }
}

// Function to display available rooms in a floor
void displayAvailableRooms(int floor)
{
    cout << "Available Rooms on Floor " << floor << ":\n";
    for (int room = 0; room < ROOMS_PER_FLOOR; room++)
    {
        cout << "Room " << floor * 1000 + room + 1 << "\n";
    }
}

// Function to display available seats in a room
void displayAvailableSeats(int floor, int room)
{
    cout << "Available Seats in Room " << floor * 1000 + room + 1 << ":\n";
    bool anyAvailable = false;
    for (int seat = 0; seat < SEATS_PER_ROOM; seat++)
    {
        if (availability[floor - 1][room][seat])
        {
            cout << "Seat " << seat + 1 << "\n";
            anyAvailable = true;
        }
    }
    if (!anyAvailable)
    {
        cout << "No available seats in this room.\n";
    }
}

// Function to book a seat
void bookSeat(const Booking &booking)
{
    // Update availability
    availability[booking.floor - 1][booking.room][booking.seat - 1] = false;

    // Save to file
    ofstream file("booking_records.txt", ios::app);
    if (file.is_open())
    {
        file << "Name: " << booking.name << ", ID: " << booking.studentId
             << ", Semester: " << booking.semester << ", Floor: " << booking.floor
             << ", Room: " << booking.room + 1 << ", Seat: " << booking.seat << "\n";
        file.close();
    }

    cout << "Booking successful! Your seat has been reserved.\n";
}

int main()
{
    initializeAvailability();
    loadBookingData(); // Load existing bookings from the file

    char choice;
    do
    {
        cout << "\n--- Hostel Room Booking System ---\n";
        Booking booking;

        // Get student details
        cin.ignore(); // Clear input buffer before getline
        cout << "Enter your name: ";
        getline(cin, booking.name);
        cout << "Enter your student ID: ";
        getline(cin, booking.studentId);

        // Check if the student ID is already booked
        if (isStudentIdAlreadyBooked(booking.studentId))
        {
            cout << "Error: This student ID is already used for a booking. Duplicate bookings are not allowed.\n";
            continue; // Skip to the next iteration
        }

        cout << "Enter your semester: ";
        cin >> booking.semester;

        // Display and select floor
        displayAvailableFloors();
        cout << "Select a floor (1-" << FLOORS << "): ";
        cin >> booking.floor;

        // Display and select room
        displayAvailableRooms(booking.floor);
        cout << "Select a room (1-" << ROOMS_PER_FLOOR << "): ";
        cin >> booking.room;
        booking.room--; // Adjust for zero-based indexing

        // Display and select seat
        displayAvailableSeats(booking.floor, booking.room);
        cout << "Select a seat (1-" << SEATS_PER_ROOM << "): ";
        cin >> booking.seat;

        // Check if the seat is already booked
        if (!availability[booking.floor - 1][booking.room][booking.seat - 1])
        {
            cout << "Error: This seat is already booked. Please choose another.\n";
            continue; // Skip to the next iteration
        }

        // Book the seat
        bookSeat(booking);

        // Check if the user wants to book another seat
        cout << "Do you want to make another booking? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Thank you for using the Hostel Room Booking System!\n";
    return 0;
}
