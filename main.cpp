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

// Class for booking information
class Booking
{
private:
    string name;
    string studentId;
    int semester;
    int floor;
    int room;
    int seat;

public:
    // Constructor
    Booking(string n, string id, int sem, int fl, int rm, int st)
        : name(n), studentId(id), semester(sem), floor(fl), room(rm), seat(st) {}

    // Getter functions
    string getName() const { return name; }
    string getStudentId() const { return studentId; }
    int getSemester() const { return semester; }
    int getFloor() const { return floor; }
    int getRoom() const { return room; }
    int getSeat() const { return seat; }
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

// Function to load booking data from file and update availability
void loadBookingData()
{
    ifstream file("booking_records.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string name, studentId, temp;
            int semester, floor, room, seat;

            getline(ss, temp, ':');
            getline(ss, name, ',');
            getline(ss, temp, ':');
            getline(ss, studentId, ',');
            getline(ss, temp, ':');
            ss >> semester;
            getline(ss, temp, ':');
            ss >> floor;
            getline(ss, temp, ':');
            ss >> room;
            getline(ss, temp, ':');
            ss >> seat;

            // Update availability
            availability[floor - 1][room - 1][seat - 1] = false;
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

// Function to display available rooms on a floor
void displayAvailableRooms(int floor)
{
    cout << "Available Rooms on Floor " << floor << ":\n";
    for (int room = 0; room < ROOMS_PER_FLOOR; room++)
    {
        cout << "Room " << floor * 1000 + room + 1 << "\n";
    }
}

// Function to display available seats in a room
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
    availability[booking.getFloor() - 1][booking.getRoom()][booking.getSeat() - 1] = false;

    ofstream file("booking_records.txt", ios::app);
    if (file.is_open())
    {
        file << "Name: " << booking.getName() << ", ID: " << booking.getStudentId()
             << ", Semester: " << booking.getSemester() << ", Floor: " << booking.getFloor()
             << ", Room: " << booking.getRoom() + 1 << ", Seat: " << booking.getSeat() << "\n";
        file.close();
    }

    cout << "Booking successful! Your seat has been reserved.\n";
}

// Function to display all bookings
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
        // Display menu
        cout << "\n--- Hostel Room Booking System ---\n";
        cout << "1. Book a Seat\n";
        cout << "2. See Available Seats\n";
        cout << "3. View All Bookings\n";
        cout << "4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> option;

        switch (option)
        {
        case 1:
        {
            string name, studentId;
            int semester, floor, room, seat;

            cin.ignore(); // To clear newline from buffer

            cout << "\nEnter your name: ";
            getline(cin, name);
            cout << "Enter your student ID: ";
            getline(cin, studentId);

            if (isStudentIdAlreadyBooked(studentId))
            {
                cout << "Error: This student ID is already booked. Duplicate bookings are not allowed.\n";
                break;
            }

            cout << "Enter your semester: ";
            cin >> semester;

            displayAvailableFloors();
            cout << "Select a floor (1-" << FLOORS << "): ";
            cin >> floor;

            displayAvailableRooms(floor);
            cout << "Select a room (1-" << ROOMS_PER_FLOOR << "): ";
            cin >> room;
            room--; // Adjust for zero-indexing

            if (!displayAvailableSeats(floor, room))
            {
                cout << "No seats available in this room.\n";
                break;
            }

            cout << "Select a seat (1-" << SEATS_PER_ROOM << "): ";
            cin >> seat;

            if (!availability[floor - 1][room][seat - 1])
            {
                cout << "Error: This seat is already booked. Please choose another.\n";
                break;
            }

            Booking booking(name, studentId, semester, floor, room, seat);
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
            displayAllBookings();
            break;
        case 4:
            cout << "Exiting the system...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (option != 4);

    return 0;
}
