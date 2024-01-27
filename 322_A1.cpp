#include <iostream>
#include <string>

// Global variables to store user details
std::string gender;
int age;
double weight;
double waist;
double neck;
double height;
std::string lifestyle;

// Additional variable for female users
double hip;

void getUserDetails() {
    // Prompt user for input
    std::cout << "1. Gender: Please specify your gender as either male or female: ";
    std::cin >> gender;

    std::cout << "2. Age: Enter your age: ";
    std::cin >> age;

    std::cout << "3. Weight: Enter your body weight in kilograms: ";
    std::cin >> weight;

    std::cout << "4. Waist: Input your waist measurement using centimeters: ";
    std::cin >> waist;

    std::cout << "5. Neck: Provide your neck measurement in centimeters: ";
    std::cin >> neck;

    std::cout << "6. Height: Input height measurement in centimeters: ";
    std::cin >> height;

    std::cout << "7. Lifestyle: Provide information about your current lifestyle (sedentary, moderate, or active): ";
    std::cin >> lifestyle;

    // Additional input for female users
    if (gender == "female") {
        std::cout << "Hip Measurement: Enter your hip measurement in centimeters: ";
        std::cin >> hip;
    }
}

int main() {
    // Example of using the getUserDetails function
    getUserDetails();

    // Displaying the gathered information (for demonstration purposes)
    std::cout << "\nUser Details:\n";
    std::cout << "Gender: " << gender << "\n";
    std::cout << "Age: " << age << " years\n";
    std::cout << "Weight: " << weight << " kg\n";
    std::cout << "Waist: " << waist << " cm\n";
    std::cout << "Neck: " << neck << " cm\n";
    std::cout << "Height: " << height << " cm\n";
    std::cout << "Lifestyle: " << lifestyle << "\n";

    if (gender == "female") {
        std::cout << "Hip Measurement: " << hip << " cm\n";
    }

    return 0;
}
