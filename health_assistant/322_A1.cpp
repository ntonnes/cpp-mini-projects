#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>

// Global variables to store user details
std::string gender;
int age;
double weight;
double waist;
double neck;
double height;
std::string lifestyle;
double hip;



void getUserDetails() {
    // Prompt user for input
    std::cout << "Please specify your gender as either male or female: ";
    std::cin >> gender;

    // Validate gender input
    while (!(gender == "male" || gender == "female")) {
        std::cout << "Invalid gender. Please enter male or female: ";
        std::cin >> gender;
    }

    std::cout << "Enter your age: ";
    // Validate age input
    while (!(std::cin >> age) || age < 20 || age > 79) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid age. Please enter an age between 20 and 79: ";
    }

    std::cout << "Enter your body weight in kilograms: ";
    // Validate weight input
    while (!(std::cin >> weight) || weight <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid weight. Please enter a weight greater than 0: ";
    }

    std::cout << "Enter your waist measurement in centimeters: ";
    // Validate waist input
    while (!(std::cin >> waist) || waist <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid waist measurement. Please enter a value greater than 0: ";
    }

    std::cout << "Enter your neck measurement in centimeters: ";
    // Validate neck input
    while (!(std::cin >> neck) || neck <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid neck measurement. Please enter a value greater than 0: ";
    }

    std::cout << "Enter your height measurement in centimeters: ";
    // Validate height input
    while (!(std::cin >> height) || height <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid height measurement. Please enter a value greater than 0: ";
    }

    std::cout << "Enter information about your current lifestyle (sedentary, moderate, or active): ";
    // Validate lifestyle input
    while (!(std::cin >> lifestyle) || !(lifestyle == "sedentary" || lifestyle == "moderate" || lifestyle == "active")) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid lifestyle. Please enter sedentary, moderate, or active: ";
    }

    // Additional input for female users
    if (gender == "female") {
        std::cout << "Enter your hip measurement in centimeters: ";
        // Validate hip input
        while (!(std::cin >> hip) || hip <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid hip measurement. Please enter a value greater than 0: ";
        }
    }
}

std::pair<int, std::string> get_bfp(double waist, double neck, double height, double hip, std::string gender, int age) {
    // Calculate bodyfat % by gender
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

    // Determine the body fat % category
    std::string group;
    if (gender == "female") {
        if (age >= 20 && age <= 39) {
            if (BF_percentage < 21) {
                group = "low";
            } else if (BF_percentage >= 21 && BF_percentage < 32.9) {
                group = "normal";
            } else if (BF_percentage >= 33 && BF_percentage < 38.9) {
                group = "high";
            } else {
                group = "very high";
            }
        } else if (age >= 40 && age <= 59) {
            if (BF_percentage < 23) {
                group = "low";
            } else if (BF_percentage >= 23 && BF_percentage < 33.9) {
                group = "normal";
            } else if (BF_percentage >= 34 && BF_percentage < 39.9) {
                group = "high";
            } else {
                group = "very high";
            }
        } else if (age >= 60 && age <= 79) {
            if (BF_percentage < 24) {
                group = "low";
            } else if (BF_percentage >= 24 && BF_percentage < 35.9) {
                group = "normal";
            } else if (BF_percentage >= 36 && BF_percentage < 41.9) {
                group = "high";
            } else {
                group = "very high";
            }
        }
    } else if (gender == "male") {
        if (age >= 20 && age <= 39) {
            if (BF_percentage < 8) {
                group = "low";
            } else if (BF_percentage >= 8 && BF_percentage < 19.9) {
                group = "normal";
            } else if (BF_percentage >= 20 && BF_percentage < 24.9) {
                group = "high";
            } else {
                group = "very high";
            }
        } else if (age >= 40 && age <= 59) {
            if (BF_percentage < 11) {
                group = "low";
            } else if (BF_percentage >= 11 && BF_percentage < 21.9) {
                group = "normal";
            } else if (BF_percentage >= 22 && BF_percentage < 27.9) {
                group = "high";
            } else {
                group = "very high";
            }
        } else if (age >= 60) {
            if (BF_percentage < 13) {
                group = "low";
            } else if (BF_percentage >= 13 && BF_percentage <= 24.9) {
                group = "normal";
            } else if (BF_percentage >= 25 && BF_percentage <= 29.9) {
                group = "high";
            } else {
                group = "very high";
            }
        } else {
            // Handle invalid age
            std::cerr << "Age out of range 20-79.\n";
            return {0, "Invalid"};
        }
    }

    return {static_cast<int>(BF_percentage), group};
}

int get_daily_calories(double age, std::string gender, std::string lifestyle) {
    // Determine the required calories from activity level
    int daily_calories = 0;

    if (gender == "male") {
        if (age >= 19 && age <= 30) {
            if (lifestyle == "sedentary") {
                daily_calories = 2400;
            } else if (lifestyle == "moderate") {
                daily_calories = 2700;
            } else if (lifestyle == "active") {
                daily_calories = 3000;
            }
        } else if (age >= 31 && age <= 50) {
            if (lifestyle == "sedentary") {
                daily_calories = 2200;
            } else if (lifestyle == "moderate") {
                daily_calories = 2500;
            } else if (lifestyle == "active") {
                daily_calories = 2900;
            }
        } else if (age >= 51) {
            if (lifestyle == "sedentary") {
                daily_calories = 2000;
            } else if (lifestyle == "moderate") {
                daily_calories = 2300;
            } else if (lifestyle == "active") {
                daily_calories = 2600;
            }
        }
    } else if (gender == "female") {
        if (age >= 19 && age <= 30) {
            if (lifestyle == "sedentary") {
                daily_calories = 2000;
            } else if (lifestyle == "moderate") {
                daily_calories = 2100;
            } else if (lifestyle == "active") {
                daily_calories = 2400;
            }
        } else if (age >= 31 && age <= 50) {
            if (lifestyle == "sedentary") {
                daily_calories = 1800;
            } else if (lifestyle == "moderate") {
                daily_calories = 2000;
            } else if (lifestyle == "active") {
                daily_calories = 2200;
            }
        } else if (age >= 51) {
            if (lifestyle == "sedentary") {
                daily_calories = 1600;
            } else if (lifestyle == "moderate") {
                daily_calories = 1800;
            } else if (lifestyle == "active") {
                daily_calories = 2100;
            }
        }
    } else {
        // Handle invalid activity level
        std::cerr << "Invalid activity level specified.\n";
    }

    return daily_calories;
}

void meal_prep(int calories_input, double& carbs_output, double& protein_output, double& fat_output) {
    // Constants for macronutrient calorie values
    const int carb_calories = 4;
    const int protein_calories = 4;
    const int fat_calories = 9;

    // Macronutrient breakdown percentages
    const double carb_percentage = 0.5;
    const double protein_percentage = 0.3;
    const double fat_percentage = 0.2;

    // Calculate grams for each macronutrient
    carbs_output = (calories_input * carb_percentage) / carb_calories;
    protein_output = (calories_input * protein_percentage) / protein_calories;
    fat_output = (calories_input * fat_percentage) / fat_calories;
}

void display() {
    // Calculate body fat %, calorie requirement, and macronutrient breakdown
    auto bfp_result = get_bfp(waist, neck, height, hip, gender, age);
    int daily_calories = get_daily_calories(age, gender, lifestyle);
    double carbs, protein, fat;
    meal_prep(daily_calories, carbs, protein, fat);

    // Display the gathered information and results
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

    std::cout << "\nBody Fat Percentage: " << bfp_result.first << "%, " << bfp_result.second << "\n";
    std::cout << "\nDaily Calorie Intake: " << daily_calories << " calories\n";

    // Display macronutrient breakdown
    std::cout << "\nMacronutrient Breakdown:\n";
    std::cout << "Carbohydrates: " << carbs << " grams\n";
    std::cout << "Protein: " << protein << " grams\n";
    std::cout << "Fat: " << fat << " grams\n";
}

void serialize(std::string filename) {
    // Open the file in append mode to preserve existing data
    std::ofstream file(filename, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Error opening file for serialization.\n";
        return;
    }

    // Write user details to the file in CSV format
    file << gender << "," << age << "," << weight << "," << waist << "," << neck << ",";

    // Add hip measurement only for female users
    if (gender == "female") {
        file << hip << ",";
    } else {
        file << ",";
    }

    file << height << "," << lifestyle << "\n";

    // Close the file
    file.close();
}

void readFromFile(std::string filename) {
    // Open the file for reading
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file for reading.\n";
        return;
    }

    // Variables to temporarily store CSV values
    std::string line;
    std::string value;

    // Read each line from the file
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> values;

        // Split the line into values using commas
        while (std::getline(iss, value, ',')) {
            values.push_back(value);
        }

        // Check if the line has enough values (at least 8 for all user details)
        if (values.size() >= 8) {
            gender = values[0];
            age = std::stoi(values[1]);
            weight = std::stod(values[2]);
            waist = std::stod(values[3]);
            neck = std::stod(values[4]);
            hip = values[5] == "" ? 0.0 : std::stod(values[5]);
            height = std::stod(values[6]);
            lifestyle = values[7];
        } else {
            std::cerr << "Invalid line in the CSV file.\n";
        }
    }

    // Close the file
    file.close();
}

int main(int argc, char* argv[]) {
    // Check if a filename is provided as a command-line argument
    if (argc > 1) {
        // Read from the provided file
        readFromFile(argv[1]);
    }

    // Collect user information
    getUserDetails();
    // Display to screen
    display();
    // Serialize user data to CSV
    serialize("user_data.csv");

    return 0;
}

