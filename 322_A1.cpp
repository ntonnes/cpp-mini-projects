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

std::pair<int, std::string> get_bfp(double waist, double neck, double height, double hip, std::string gender, int age) {
    double BF_percentage;

    if (gender == "male") {
        BF_percentage = 495 / (1.0324 - 0.19077 * log10(waist - neck) + 0.15456 * log10(height)) - 450;
    } else if (gender == "female") {
        BF_percentage = 495 / (1.29579 - 0.35004 * log10(waist + hip - neck) + 0.22100 * log10(height)) - 450;
    } else {
        // Handle invalid gender
        std::cerr << "Invalid gender specified.\n";
        return {0, "Invalid"};
    }

    // Determine the body fat percentage group
    std::string group;
    if (gender == "female") {
        if (age >= 20 && age <= 39) {
            if (BF_percentage < 21) {
                group = "<21";
            } else if (BF_percentage >= 21 && BF_percentage < 32.9) {
                group = "21.0-32.9";
            } else if (BF_percentage >= 33 && BF_percentage < 38.9) {
                group = "33.0-38.9";
            } else {
                group = ">=39";
            }
        } else if (age >= 40 && age <= 59) {
            // Similar logic for other age groups
            // ...
        } else if (age >= 60 && age <= 79) {
            // Similar logic for other age groups
            // ...
        }
    } else if (gender == "male") {
        // Similar logic for male age groups
        // ...
    }

    return {static_cast<int>(BF_percentage), group};
}

int main() {
    // Example of using the getUserDetails function
    getUserDetails();

    // Example of using the get_bfp function
    auto result = get_bfp(waist, neck, height, hip, gender, age);

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

    std::cout << "\nBody Fat Percentage: " << result.first << "%\n";
    std::cout << "Group: " << result.second << "\n";

    return 0;
}
