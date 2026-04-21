#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

// Abstract Base Class
class Waste {
protected:
    double weight;

public:
    Waste(double w) {
        weight = w;
    }

    virtual void processWaste() = 0;
    virtual string getType() = 0;

    double getWeight() {
        return weight;
    }

    virtual ~Waste() {}
};

// Derived Classes
class OrganicWaste : public Waste {
public:
    OrganicWaste(double w) : Waste(w) {}

    void processWaste() override {
        cout << "Processing Organic Waste into Compost...\n";
    }

    string getType() override {
        return "Organic";
    }
};

class PlasticWaste : public Waste {
public:
    PlasticWaste(double w) : Waste(w) {}

    void processWaste() override {
        cout << "Recycling Plastic Waste...\n";
    }

    string getType() override {
        return "Plastic";
    }
};

class MetalWaste : public Waste {
public:
    MetalWaste(double w) : Waste(w) {}

    void processWaste() override {
        cout << "Melting and Reusing Metal Waste...\n";
    }

    string getType() override {
        return "Metal";
    }
};

// Main System Class
class WasteManagementSystem {
private:
    vector<Waste*> wasteList;
    double totalWaste = 0;

public:

    // ✅ Load only last saved entry
    void loadFromFile() {
        ifstream file("waste_data.txt");

        if (!file) return; // file not found

        string type;
        double weight;

        if (file >> type >> weight) {
            Waste* waste = nullptr;

            if (type == "Organic")
                waste = new OrganicWaste(weight);
            else if (type == "Plastic")
                waste = new PlasticWaste(weight);
            else if (type == "Metal")
                waste = new MetalWaste(weight);

            if (waste) {
                wasteList.push_back(waste);
                totalWaste += weight;
            }
        }

        file.close();
    }

    // ✅ Save ONLY latest entry (overwrite file)
    void saveToFile(Waste* waste) {
        ofstream file("waste_data.txt"); // overwrite mode

        if (file) {
            file << waste->getType() << " " << waste->getWeight();
        }

        file.close();
    }

    void addWaste() {
        int choice;
        double weight;

        cout << "\nSelect Waste Type:\n";
        cout << "1. Organic\n";
        cout << "2. Plastic\n";
        cout << "3. Metal\n";
        cout << "Enter choice: ";
        cin >> choice;

        cout << "Enter weight (kg): ";
        cin >> weight;

        Waste* waste = nullptr;

        switch (choice) {
            case 1:
                waste = new OrganicWaste(weight);
                break;
            case 2:
                waste = new PlasticWaste(weight);
                break;
            case 3:
                waste = new MetalWaste(weight);
                break;
            default:
                cout << "Invalid choice!\n";
                return;
        }

        wasteList.push_back(waste);
        totalWaste += weight;

        saveToFile(waste);  // ✅ Save only latest entry

        cout << "Waste added successfully!\n";
    }

    void processAllWaste() {
        if (wasteList.empty()) {
            cout << "\nNo waste to process!\n";
            return;
        }

        cout << "\nProcessing Waste...\n";
        for (auto waste : wasteList) {
            cout << waste->getType() << " Waste (" 
                 << waste->getWeight() << " kg): ";
            waste->processWaste();
        }
    }

    void showStatistics() {
        double organic = 0, plastic = 0, metal = 0;

        for (auto waste : wasteList) {
            if (waste->getType() == "Organic")
                organic += waste->getWeight();
            else if (waste->getType() == "Plastic")
                plastic += waste->getWeight();
            else if (waste->getType() == "Metal")
                metal += waste->getWeight();
        }

        cout << fixed << setprecision(2);
        cout << "\n------ Waste Statistics ------\n";
        cout << "Total Waste: " << totalWaste << " kg\n";
        cout << "Organic Waste: " << organic << " kg\n";
        cout << "Plastic Waste: " << plastic << " kg\n";
        cout << "Metal Waste: " << metal << " kg\n";

        if (totalWaste > 0) {
            cout << "\nRecycling Efficiency:\n";
            cout << "Organic: " << (organic / totalWaste) * 100 << "%\n";
            cout << "Plastic: " << (plastic / totalWaste) * 100 << "%\n";
            cout << "Metal: " << (metal / totalWaste) * 100 << "%\n";
        }
    }

    // Destructor (cleanup memory only)
    ~WasteManagementSystem() {
        for (auto waste : wasteList) {
            delete waste;
        }
    }
};

// Main Function
int main() {
    WasteManagementSystem system;

    system.loadFromFile(); // ✅ Load last saved entry

    int choice;

    do {
        cout << "\n===== Smart Waste Management System =====\n";
        cout << "1. Add Waste\n";
        cout << "2. Process Waste\n";
        cout << "3. Show Statistics\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system.addWaste();
                break;
            case 2:
                system.processAllWaste();
                break;
            case 3:
                system.showStatistics();
                break;
            case 4:
                cout << "Exiting System...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 4);

    return 0;
}