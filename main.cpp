#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>    // For numeric limits
#include <cctype>    // For checking character properties
#include <algorithm> // For all_of

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

// Function to handle invalid input for floor, room, and seat numbers
template <typename T>
T getValidInput(const string &prompt, int min, int max)
{
    T value;
    while (true)
    {
        cout << prompt;
        cin >> value;

        if (cin.fail() || value < min || value > max)
        {
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
        }
        else
        {
            break;
        }
    }
    return value;
}

// Function to validate the name (starts with a letter)
bool isValidName(const string &name)
{
    return isalpha(name[0]);
}

// Function to validate the student ID (only digits)
bool isValidStudentId(const string &studentId)
{
    return !studentId.empty() && all_of(studentId.begin(), studentId.end(), ::isdigit);
}

// Function to validate semester (positive number)
bool isValidSemester(int semester)
{
    return semester > 0;
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

        while (true)
        {
            cin >> option;

            // Check for invalid input (non-integer or out of range)
            if (cin.fail() || option < 1 || option > 4)
            {
                cin.clear();                                         // Clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid option. Please enter a number between 1 and 4.\n";
                cout << "Enter your choice (1-4): ";
            }
            else
            {
                break; // Exit the loop if the input is valid
            }
        }

        switch (option)
        {
        case 1:
        {
            string name, studentId;
            int semester, floor, room, seat;

            cin.ignore(); // To clear newline from buffer

            // Input and validate name
            do
            {
                cout << "\nEnter your name (starts with a letter): ";
                getline(cin, name);
                if (!isValidName(name))
                {
                    cout << "Invalid name. Name should start with a letter.\n";
                }
            } while (!isValidName(name));

            // Input and validate student ID
            do
            {
                cout << "Enter your student ID (numeric digits only): ";
                getline(cin, studentId);
                if (!isValidStudentId(studentId))
                {
                    cout << "Invalid student ID. It should only contain digits.\n";
                }
            } while (!isValidStudentId(studentId));

            if (isStudentIdAlreadyBooked(studentId))
            {
                cout << "Error: This student ID is already booked. Duplicate bookings are not allowed.\n";
                break;
            }

            // Input and validate semester
            do
            {
                cout << "Enter your semester (positive number): ";
                cin >> semester;
                if (!isValidSemester(semester))
                {
                    cout << "Invalid semester. Please enter a positive number.\n";
                }
            } while (!isValidSemester(semester));

            displayAvailableFloors();
            floor = getValidInput<int>("Select a floor (1-" + to_string(FLOORS) + "): ", 1, FLOORS);

            displayAvailableRooms(floor);
            room = getValidInput<int>("Select a room (1-" + to_string(ROOMS_PER_FLOOR) + "): ", 1, ROOMS_PER_FLOOR);
            room--; // Adjust for zero-indexing

            bool seatAvailable = false;
            while (!seatAvailable)
            {
                if (!displayAvailableSeats(floor, room))
                {
                    cout << "No seats available in this room. Please choose a different room or floor.\n";
                    // Allow the user to select a different room or floor if no seats are available
                    room = getValidInput<int>("Select a different room (1-" + to_string(ROOMS_PER_FLOOR) + "): ", 1, ROOMS_PER_FLOOR);
                    room--; // Adjust for zero-indexing
                }
                else
                {
                    seat = getValidInput<int>("Select a seat (1-" + to_string(SEATS_PER_ROOM) + "): ", 1, SEATS_PER_ROOM);
                    if (!availability[floor - 1][room][seat - 1])
                    {
                        cout << "Error: This seat is already booked. Please choose another.\n";
                    }
                    else
                    {
                        seatAvailable = true; // Seat is available, proceed with booking
                    }
                }
            }

            Booking booking(name, studentId, semester, floor, room, seat);
            bookSeat(booking);
            break;
        }
        case 2:
        {
            int floor, room;
            floor = getValidInput<int>("Enter floor number (1-" + to_string(FLOORS) + "): ", 1, FLOORS);
            room = getValidInput<int>("Enter room number (1-" + to_string(ROOMS_PER_FLOOR) + "): ", 1, ROOMS_PER_FLOOR);

            // Adjust room number to zero-based indexing
            int zeroBasedRoom = room - 1;

            displayAvailableSeats(floor, zeroBasedRoom);
            break;
        }
        case 3:
            displayAllBookings();
            break;
        case 4:
            cout << "Exiting the program.\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (option != 4);

    return 0;
}
