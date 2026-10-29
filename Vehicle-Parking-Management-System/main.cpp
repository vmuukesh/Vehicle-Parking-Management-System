#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <ctime>
#include <cctype>

using namespace std;

const int TOTAL_SLOTS = 10;
const double RATE_PER_HOUR = 20.0;
const string DATA_FILE = "parking_records.txt";

struct Vehicle {
    string registrationNumber;
    string vehicleType;
    int slotNumber;
    time_t entryTime;
    bool parked;
};

class ParkingManagementSystem {
private:
    vector<Vehicle> vehicles;

    string currentTime(time_t timestamp) const {
        tm* local = localtime(&timestamp);
        if (local == nullptr) return "Unknown";
        stringstream result;
        result << put_time(local, "%Y-%m-%d %H:%M:%S");
        return result.str();
    }

    int findVehicle(const string& registrationNumber) const {
        for (int i = 0; i < static_cast<int>(vehicles.size()); ++i) {
            if (vehicles[i].registrationNumber == registrationNumber &&
                vehicles[i].parked) {
                return i;
            }
        }
        return -1;
    }

    int findAvailableSlot() const {
        for (int slot = 1; slot <= TOTAL_SLOTS; ++slot) {
            bool occupied = false;
            for (const Vehicle& vehicle : vehicles) {
                if (vehicle.parked && vehicle.slotNumber == slot) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) return slot;
        }
        return -1;
    }

    void saveRecords() const {
        ofstream file(DATA_FILE);
        if (!file) {
            cout << "Warning: parking records could not be saved.\n";
            return;
        }

        for (const Vehicle& vehicle : vehicles) {
            file << vehicle.registrationNumber << '|'
                 << vehicle.vehicleType << '|'
                 << vehicle.slotNumber << '|'
                 << static_cast<long long>(vehicle.entryTime) << '|'
                 << vehicle.parked << '\n';
        }
    }

    void loadRecords() {
        ifstream file(DATA_FILE);
        if (!file) return;

        string line;
        while (getline(file, line)) {
            stringstream row(line);
            string registration, type, slotText, timeText, parkedText;

            if (!getline(row, registration, '|') ||
                !getline(row, type, '|') ||
                !getline(row, slotText, '|') ||
                !getline(row, timeText, '|') ||
                !getline(row, parkedText)) {
                continue;
            }

            try {
                Vehicle vehicle;
                vehicle.registrationNumber = registration;
                vehicle.vehicleType = type;
                vehicle.slotNumber = stoi(slotText);
                vehicle.entryTime = static_cast<time_t>(stoll(timeText));
                vehicle.parked = (parkedText == "1");

                if (!registration.empty() &&
                    vehicle.slotNumber >= 1 &&
                    vehicle.slotNumber <= TOTAL_SLOTS) {
                    vehicles.push_back(vehicle);
                }
            } catch (...) {
                // Skip malformed records rather than stopping the program.
            }
        }
    }

    static string readLine(const string& prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    }

    static int readMenuChoice() {
        string input;
        cout << "Enter your choice: ";
        getline(cin, input);
        try {
            size_t used = 0;
            int choice = stoi(input, &used);
            if (used == input.size()) return choice;
        } catch (...) {}
        return -1;
    }

public:
    ParkingManagementSystem() {
        loadRecords();
    }

    void vehicleEntry() {
        cout << "\n--- Vehicle Entry ---\n";
        string registration = readLine("Enter registration number: ");
        transform(registration.begin(), registration.end(), registration.begin(),
                  [](unsigned char c) { return static_cast<char>(toupper(c)); });

        if (registration.empty()) {
            cout << "Registration number cannot be empty.\n";
            return;
        }
        if (findVehicle(registration) != -1) {
            cout << "This vehicle is already parked.\n";
            return;
        }

        int slot = findAvailableSlot();
        if (slot == -1) {
            cout << "Parking is full. No slot is currently available.\n";
            return;
        }

        cout << "Vehicle types: Car, Bike, Other\n";
        string type = readLine("Enter vehicle type: ");
        if (type.empty()) {
            cout << "Vehicle type cannot be empty.\n";
            return;
        }

        time_t now = time(nullptr);
        vehicles.push_back({registration, type, slot, now, true});
        saveRecords();

        cout << "\nVehicle parked successfully!\n";
        cout << "Registration : " << registration << '\n';
        cout << "Vehicle type : " << type << '\n';
        cout << "Slot number  : " << slot << '\n';
        cout << "Entry time   : " << currentTime(now) << '\n';
    }

    void vehicleExit() {
        cout << "\n--- Vehicle Exit ---\n";
        string registration = readLine("Enter registration number: ");
        transform(registration.begin(), registration.end(), registration.begin(),
                  [](unsigned char c) { return static_cast<char>(toupper(c)); });

        int index = findVehicle(registration);
        if (index == -1) {
            cout << "No parked vehicle found with that registration number.\n";
            return;
        }

        time_t exitTime = time(nullptr);
        double seconds = difftime(exitTime, vehicles[index].entryTime);
        long long hours = max(1LL, static_cast<long long>((seconds + 3599) / 3600));
        double fee = hours * RATE_PER_HOUR;

        cout << "\n--------- Parking Receipt ---------\n";
        cout << "Registration : " << vehicles[index].registrationNumber << '\n';
        cout << "Vehicle type : " << vehicles[index].vehicleType << '\n';
        cout << "Slot number  : " << vehicles[index].slotNumber << '\n';
        cout << "Entry time   : " << currentTime(vehicles[index].entryTime) << '\n';
        cout << "Exit time    : " << currentTime(exitTime) << '\n';
        cout << "Billable time: " << hours << " hour(s)\n";
        cout << fixed << setprecision(2);
        cout << "Parking fee  : Rs. " << fee << '\n';
        cout << "-----------------------------------\n";

        vehicles[index].parked = false;
        saveRecords();
        cout << "Vehicle exit recorded. Slot released successfully.\n";
    }

    void displayStatus() const {
        int occupied = 0;
        for (const Vehicle& vehicle : vehicles) {
            if (vehicle.parked) ++occupied;
        }

        cout << "\n--- Parking Status ---\n";
        cout << "Total slots     : " << TOTAL_SLOTS << '\n';
        cout << "Occupied slots  : " << occupied << '\n';
        cout << "Available slots : " << TOTAL_SLOTS - occupied << '\n';
        cout << "\nSlot details:\n";

        for (int slot = 1; slot <= TOTAL_SLOTS; ++slot) {
            bool found = false;
            for (const Vehicle& vehicle : vehicles) {
                if (vehicle.parked && vehicle.slotNumber == slot) {
                    cout << "Slot " << setw(2) << slot << " : OCCUPIED - "
                         << vehicle.registrationNumber << " (" << vehicle.vehicleType << ")\n";
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Slot " << setw(2) << slot << " : AVAILABLE\n";
            }
        }
    }

    void displayParkedVehicles() const {
        cout << "\n--- Currently Parked Vehicles ---\n";
        cout << left << setw(18) << "Registration"
             << setw(14) << "Type"
             << setw(10) << "Slot"
             << "Entry Time\n";
        cout << string(62, '-') << '\n';

        bool any = false;
        for (const Vehicle& vehicle : vehicles) {
            if (vehicle.parked) {
                any = true;
                cout << left << setw(18) << vehicle.registrationNumber
                     << setw(14) << vehicle.vehicleType
                     << setw(10) << vehicle.slotNumber
                     << currentTime(vehicle.entryTime) << '\n';
            }
        }
        if (!any) cout << "No vehicles are currently parked.\n";
    }

    void run() {
        while (true) {
            cout << "\n========================================\n";
            cout << "   VEHICLE PARKING MANAGEMENT SYSTEM\n";
            cout << "========================================\n";
            cout << "1. Register vehicle entry\n";
            cout << "2. Record vehicle exit and generate bill\n";
            cout << "3. View parking status\n";
            cout << "4. View parked vehicles\n";
            cout << "5. Exit application\n";
            cout << "----------------------------------------\n";

            int choice = readMenuChoice();
            switch (choice) {
                case 1: vehicleEntry(); break;
                case 2: vehicleExit(); break;
                case 3: displayStatus(); break;
                case 4: displayParkedVehicles(); break;
                case 5:
                    saveRecords();
                    cout << "Thank you for using the Parking Management System.\n";
                    return;
                default:
                    cout << "Invalid choice. Please enter a number from 1 to 5.\n";
            }
        }
    }
};

int main() {
    ParkingManagementSystem system;
    system.run();
    return 0;
}
