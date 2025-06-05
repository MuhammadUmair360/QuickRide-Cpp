#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

// Global variable to store the currently logged-in user
string current_user = "";

// Convert string to lowercase
string to_lower(const string& str) {
    string result = str;
    for (int i = 0; i < result.length(); ++i) { 
        result[i] = tolower(result[i]);
    }
    return result;
}                    

void quit();
void register_user();
bool login_user();
void book_ride();
void ride_history();
void ride_menu();

// Exit the program with a thank-you message
void quit() {
    cout << "Thank you for using QUICKRIDE. We hope to see you again soon!" << endl;
    exit(0);
}

// Handles new user registration with validations
void register_user() {
    string username, password, cnic, mobile, city;
    int age;
    string line, f_username, f_password, f_cnic, f_mobile;
    ifstream infile;
    bool username_used;

    // Ensure the username is unique
    do {
        cout << "Please enter a new username: ";
        cin.ignore();
        getline(cin, username);

        infile.open("users.csv");
        username_used = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            getline(ss, f_username, ',');
            if (to_lower(f_username) == to_lower(username)) {
                username_used = true;
                cout << "This username is already in use. Please choose another one." << endl;
                break;
            }
        }
        infile.close();
    } while (username_used);

    // Validate password length
    cout << "Create a secure password (at least 8 characters): ";
    getline(cin, password);
    while (password.length() < 8) {
        cout << "Password is too short. Please enter a password with at least 8 characters: ";
        getline(cin, password);
    }

    // Ensure password is not already used
    bool password_used;
    do {
        infile.open("users.csv");
        password_used = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            getline(ss, f_username, ',');
            getline(ss, f_password, ',');
            if (f_password == password) {
                password_used = true;
                cout << "This password is already in use. Please choose another one: ";
                getline(cin, password);
                break;
            }
        }
        infile.close();
    } while (password_used || password.length() < 8);

    // Check for duplicate CNIC
    bool cnic_used;
    do {
        cout << "Enter your CNIC number: ";
        getline(cin, cnic);
        infile.open("users.csv");
        cnic_used = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            getline(ss, f_username, ',');
            getline(ss, f_password, ',');
            getline(ss, f_cnic, ',');
            if (f_cnic == cnic) {
                cnic_used = true;
                cout << "This CNIC is already registered. Please enter a different one." << endl;
                break;
            }
        }
        infile.close();
    } while (cnic_used);

    // Check for duplicate mobile number
    bool mobile_used;
    do {
        cout << "Enter your mobile number: ";
        getline(cin, mobile);
        infile.open("users.csv");
        mobile_used = false;
        while (getline(infile, line)) {
            stringstream ss(line);
            getline(ss, f_username, ',');
            getline(ss, f_password, ',');
            getline(ss, f_cnic, ',');
            getline(ss, f_mobile, ',');
            if (f_mobile == mobile) {
                mobile_used = true;
                cout << "This mobile number is already registered. Please enter a different number." << endl;
                break;
            }
        }
        infile.close();
    } while (mobile_used);

    // Collect remaining info
    cout << "Which city are you located in? ";
    getline(cin, city);

    cout << "Please provide your age: ";
    cin >> age;
    
    // Save user data
    ofstream outfile("users.csv", ios::app);
    outfile << username << "," << password << "," << cnic << "," << mobile << "," << city << "," << age << endl;
    cout << "You have successfully registered! Welcome to QuickRide." << endl;
}

// Handles user login
bool login_user() {
    string username, password;
    bool logged_in = false;

    cin.ignore();
    cout << "Please enter your username: ";
    getline(cin, username);
    cout << "Please enter your password: ";
    getline(cin, password);

    ifstream infile("users.csv");
    if (!infile) {
        cout << "User database not found. Please register first." << endl;
        return false;
    }

    string line, f_username, f_password;
    // Match credentials
    while (getline(infile, line)) {
        stringstream ss(line);
        getline(ss, f_username, ',');
        getline(ss, f_password, ',');
        if (to_lower(f_username) == to_lower(username) && to_lower(f_password) == to_lower(password)) {
            current_user = f_username;
            logged_in = true;
            break;
        }
    }
    infile.close();

    if (logged_in) {
        cout << "Login successful! Welcome back, " << current_user << "." << endl;
        return true;
    } else {
        cout << "Invalid credentials. Please try again." << endl;
        return false;
    }
}

// Handles ride booking flow
void book_ride() {
    string pickup, dropoff, vehicle_type;
    double distance, total_fare, fare_per_km;

    // Driver lists
    string bike_drivers[] = {"Ahmed (Bike Driver)", "Ali (Bike Driver)", "Omar (Bike Driver)", "Hassan (Bike Driver)", "Usman (Bike Driver)"}; 
    string car_drivers[] = {"Imran (Car Driver)", "Zain (Car Driver)", "Bilal (Car Driver)", "Hassan (Car Driver)", "Fahad (Car Driver)", "Ali (Car Driver)", "Rehan (Car Driver)"}; 
    string van_drivers[] = {"Usman (Van Driver)", "Khalid (Van Driver)", "Farhan (Van Driver)", "Zahid (Van Driver)", "Adeel (Van Driver)", "Hassan (Van Driver)"}; 

    // Get ride details
    cout << "Please enter your pickup location: ";
    cin.ignore(); 
    getline(cin, pickup);

    cout << "Please enter your drop-off location: ";
    getline(cin, dropoff);

    // Select vehicle type
    while (true) {
        cout << "Choose your vehicle type (car, bike, van): ";
        getline(cin, vehicle_type);
        vehicle_type = to_lower(vehicle_type);
        if (vehicle_type == "car" || vehicle_type == "bike" || vehicle_type == "van") {
            break;
        } else {
            cout << "Invalid vehicle type. Please select from car, bike, or van." << endl;
        }
    }

    // Get ride distance and calculate fare
    cout << "Enter the distance of your journey (in km): ";
    cin >> distance;
    cin.ignore();

    double min_rate;
    if (vehicle_type == "bike") {
        min_rate = 10.0;
    } else if (vehicle_type == "car") {
        min_rate = 25.0;
    } else if (vehicle_type == "van") {
        min_rate = 18.0;
    }

    double min_fare = min_rate * distance;
    int attempt = 0;
    while (true) {
        cout << "Enter your total fare (in PKR): ";
        cin >> total_fare;
        cin.ignore();
        fare_per_km = total_fare / distance;
        attempt++;
        if (fare_per_km < min_rate) {
            if (attempt == 1) {
                cout << "Please increase the fare amount." << endl;
            } else {
                cout << "Fare is too low. Minimum fare: " << min_fare << " PKR" << endl;
            }
        } else {
            break;
        }
    }

    // Random driver and arrival time assignment
    srand(time(0));
    string driver;
    int arrival_time = rand() % 30 + 1;

    if (vehicle_type == "bike") {
        driver = bike_drivers[rand() % 5];
    } else if (vehicle_type == "car") {
        driver = car_drivers[rand() % 7];
    } else {
        driver = van_drivers[rand() % 6];
    }

    // Display ride summary
    cout << "\nBooking Summary:" << endl;
    cout << "Pickup Location: " << pickup << endl;
    cout << "Drop-off Location: " << dropoff << endl;
    cout << "Vehicle Type: " << vehicle_type << endl;
    cout << "Assigned Driver: " << driver << endl;
    cout << "Distance: " << distance << " km" << endl;
    cout << "Fare: " << total_fare << " PKR (" << fare_per_km << " PKR/km)" << endl;
    cout << "\nYour driver, " << driver << ", will arrive in approximately " << arrival_time << " minutes." << endl;

    // Save ride to history
    ofstream history_file("ride_history.txt", ios::app);
    history_file << "User: " << current_user << endl;
    history_file << "Pickup: " << pickup << endl;
    history_file << "Drop-off: " << dropoff << endl;
    history_file << "Vehicle: " << vehicle_type << endl;
    history_file << "Driver: " << driver << endl;
    history_file << "Distance: " << distance << " km" << endl;
    history_file << "Fare: " << total_fare << " PKR" << endl;
    history_file << "Arrival in: " << arrival_time << " minutes" << endl;
    history_file << "--------------------------" << endl;
    history_file.close();

    // Post-ride options
    string choice;
    cout << "\nWhat would you like to do next?" << endl;
    cout << "1. Book another ride" << endl;
    cout << "2. View ride history" << endl;
    cout << "3. Quit" << endl;
    cout << "Please choose an option (1/2/3): ";
    getline(cin, choice);

    if (to_lower(choice) == "1" || to_lower(choice) == "book") {
        book_ride();
    } else if (to_lower(choice) == "2" || to_lower(choice) == "view history") {
        ride_history();
    } else {
        quit();
    }
}

// Displays past ride history for the logged-in user
void ride_history() {
    ifstream history_file("ride_history.txt");
    if (!history_file.is_open()) {
        cout << "No ride history available." << endl;
        return;
    }

    string line;
    bool user_found = false;
    cout << "\nRide History for " << current_user << ":\n";
    cout << "--------------------------" << endl;
    while (getline(history_file, line)) {
        if (line.find("User: " + current_user) != string::npos) {
            user_found = true;
        }
        if (user_found) {
            cout << line << endl;
            if (line == "--------------------------") {
                user_found = false;
            }
        }
    }
    history_file.close();

    // Options after viewing history
    string choice;
    cout << "\nWhat would you like to do next?" << endl;
    cout << "1. Book a ride" << endl;
    cout << "2. Quit" << endl;
    cout << "Please choose an option (1/2): ";
    cin.ignore();
    getline(cin, choice);

    if (to_lower(choice) == "1" || to_lower(choice) == "book") {
        book_ride();
    } else {
        quit();
    }
}

// Main menu after login
void ride_menu() {
    int choice;
    while (true) {
        cout << "\n1. Book Ride" << endl;
        cout << "2. View Ride History" << endl;
        cout << "3. Quit" << endl;
        cout << "Please choose an option: ";
        cin >> choice;

        if (choice == 1) {
            book_ride();
        } else if (choice == 2) {
            ride_history();
        } else if (choice == 3) {
            quit();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Program entry point
int main() {
    int choice;
    cout << "\n\t\tWELCOME TO THE QUICKRIDE" << endl;
    while (true) {
        cout << "\n1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Quit" << endl;
        cout << "Please choose an option: ";
        cin >> choice;

        if (choice == 1) {
            register_user();
        } else if (choice == 2) {
            if (login_user()) {
                ride_menu();
            }
        } else if (choice == 3) {
            quit();
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}

