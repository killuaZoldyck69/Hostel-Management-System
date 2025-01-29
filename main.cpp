#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Constants for floors, rooms, and seats
const int FLOORS = 9;
const int ROOMS_PER_FLOOR = 10;
const int SEATS_PER_ROOM = 5;

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
            stringstream ss(line);
            Booking booking;
            string temp;

            getline(ss, temp, ':');
            getline(ss, booking.name, ',');
            getline(ss, temp, ':');
            getline(ss, booking.studentId, ',');
            getline(ss, temp, ':');
            ss >> booking.semester;
            getline(ss, temp, ':');
            ss >> booking.floor;
            getline(ss, temp, ':');
            ss >> booking.room;
            getline(ss, temp, ':');
            ss >> booking.seat;

            // Update availability
            availability[booking.floor - 1][booking.room - 1][booking.seat - 1] = false;
        }
        file.close();
    }
}

// Function to check if a student ID already exists
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
                return true;
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
    {
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

// Function to display available seats in a room and return true if available
bool displayAvailableSeats(int floor, int room)
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
    return anyAvailable;
}

// Function to book a seat
void bookSeat(const Booking &booking)
{
    availability[booking.floor - 1][booking.room][booking.seat - 1] = false;

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

// Function to display all bookings with a serial number
void displayAllBookings()
{
    ifstream file("booking_records.txt");
    if (file.is_open())
    {
        string line;
        int count = 0;
        cout << "\n--- All Booking Records ---\n";
        while (getline(file, line))
        {
            count++;
            cout << count << ". " << line << endl; // Adding serial number before each booking
        }
        file.close();
        if (count == 0)
        {
            cout << "No bookings found.\n";
        }
        else
        {
            cout << "Total bookings: " << count << endl;
        }
    }
    else
    {
        cout << "No bookings found.\n";
    }
}

int main()
{
    initializeAvailability();
    loadBookingData();

    int option;
    do
    {
        // Displaying menu
        cout << "\n--- Hostel Room Booking System ---\n";
        cout << "1. Booking a Seat\n";
        cout << "2. See Available Seats\n";
        cout << "3. See All Booking List\n";
        cout << "4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> option;

        switch (option)
        {
        case 1:
        {
            Booking booking;
            cin.ignore(); // To clear the newline character in the buffer

            cout << "\nEnter your name: ";
            getline(cin, booking.name);
            cout << "Enter your student ID: ";
            getline(cin, booking.studentId);

            if (isStudentIdAlreadyBooked(booking.studentId))
            {
                cout << "Error: This student ID is already booked. Duplicate bookings are not allowed.\n";
                break;
            }

            cout << "Enter your semester: ";
            cin >> booking.semester;

            displayAvailableFloors();
            cout << "Select a floor (1-" << FLOORS << "): ";
            cin >> booking.floor;

            displayAvailableRooms(booking.floor);
            cout << "Select a room (1-" << ROOMS_PER_FLOOR << "): ";
            cin >> booking.room;
            booking.room--;

            if (!displayAvailableSeats(booking.floor, booking.room))
            {
                cout << "No seats available in this room.\n";
                break;
            }

            cout << "Select a seat (1-" << SEATS_PER_ROOM << "): ";
            cin >> booking.seat;

            if (!availability[booking.floor - 1][booking.room][booking.seat - 1])
            {
                cout << "Error: This seat is already booked. Please choose another.\n";
                break;
            }

            bookSeat(booking);
            break;
        }
        case 2:
        {
            int floor, room;
            cout << "Enter floor number (1-" << FLOORS << "): ";
            cin >> floor;
            cout << "Enter room number (1-" << ROOMS_PER_FLOOR << "): ";
            cin >> room;
            room--; // Adjust for zero-indexing
            displayAvailableSeats(floor, room);
            break;
        }
        case 3:
        {
            displayAllBookings();
            break;
        }
        case 4:
            cout << "Exiting the system...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (option != 4);

    return 0;
}
