#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <utility>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

// Global variables to store user details
std::string gender, lifestyle;
int age;
double weight, waist, neck, height, hip;

// Global variables to calculated values
int intake;
std::pair<int, std::string> bodyfat;
double carbs, protein, fat;

// Helper functions to validate user input
void validateAge() {
    std::cout << "Enter your age: ";

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // Use stringstream to check if the input is a valid integer
        std::stringstream ss(input);
        if (ss >> age && ss.eof() && !std::isspace(input.at(0)) && (age > 19 && age < 80)) {
            return;
        }
        
        // Input is invalid
        std::cout << "Invalid age. Please enter an age between 20 and 79: ";
    }
}
void validateDouble(const std::string& strOut, const std::string& strErr, double& param) {
    std::cout << strOut;
    
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // Use stringstream to check if the input is a valid integer
        std::stringstream ss(input);
        double temp;

        // Validate all other measurements
        if (ss >> temp && ss.eof() && !std::isspace(input.at(0)) && (temp>0)) {
            param = temp;
            return;
        }
        
        // Input is not a valid integer or is not greater than 0
        std::cout << strErr;
    }
}
void validateStr(const std::string& strOut, const std::string& strErr, const std::vector<std::string>& validStrs, std::string& param) {
    std::cout << strOut;

    while (true) {
        std::getline(std::cin, param);

        // Convert the user input to lowercase for case-insensitive comparison
        std::transform(param.begin(), param.end(), param.begin(), ::tolower);

        // Check if the user input is in the list of valid strings
        if (std::find(validStrs.begin(), validStrs.end(), param) != validStrs.end()) {
            return;
        }

        // Invalid input
        std::cout << strErr;
    }
}

//*** PART 1 ***//
void getUserDetails() {

    // Prompt for and validate gender
    std::vector<std::string> validGenders = { "female", "male" };
    validateStr("Please specify your gender as either male or female: ", "Invalid gender. Please enter male or female: ", validGenders, gender);

    // Prompt for and validate age
    validateAge();

    // Prompt for and validate double-based measurements
    validateDouble("Enter your body weight in kilograms: ", "Invalid weight. Please enter a weight greater than 0: ", weight);
    validateDouble("Enter your waist measurement in centimeters: ", "Invalid waist measurement. Please enter a value greater than 0: ", waist);
    validateDouble("Enter your neck measurement in centimeters: ", "Invalid neck measurement. Please enter a value greater than 0: ", neck);
    validateDouble("Enter your height measurement in centimeters: ", "Invalid height measurement. Please enter a value greater than 0: ", height);

    // Prompt for and validate lifestyle
    std::vector<std::string> validLifestyles = { "sedentary", "moderate", "active" };
    validateStr("Enter information about your current lifestyle (sedentary, moderate, or active): ", "Invalid lifestyle. Please enter sedentary, moderate, or active: ", validLifestyles, lifestyle);

    // Prompt for and validate hip measurement for female users
    if (gender == "female") {
        validateDouble("Enter your hip measurement in centimeters: ", "Invalid hip measurement. Please enter a value greater than 0: ", hip);
    }
}

//*** PART 2 ***//
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

//*** PART 3 ***//
int get_daily_calories(double age, std::string gender, std::string lifestyle) {
    // Start with minimum calories
    int daily_calories = 1600;

    // Add additional calories based on age
    if (age < 51) { 
        daily_calories += (age > 30) ? 200 : 400;
    }

    // Add additional calories for males
    daily_calories += (gender == "male") ? 400 : 0;

    // Set scaling factor for activity level
    int activityBonus = (gender == "male") ? 300 : 200;
    // Add additional calories based on activity level
    if (lifestyle != "sedentary") {
        daily_calories += (lifestyle == "moderate") ? activityBonus : (2*activityBonus);
    }

    return daily_calories;
}

//*** PART 4 ***//
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

//*** PART 5 ***//
void display() {
    // Display the gathered information and results
    std::cout << "\n\033[1;33m=========================================\033[0m\n";
    std::cout << "\033[1;33m              User Details\033[0m\n";
    std::cout << "\033[1;33m=========================================\033[0m\n";
    std::cout << "\033[1;36m  Gender:\033[0m             " << gender << "\n";
    std::cout << "\033[1;36m  Age:\033[0m                " << age << " years\n";
    std::cout << "\033[1;36m  Weight:\033[0m             " << weight << " kg\n";
    std::cout << "\033[1;36m  Waist:\033[0m              " << waist << " cm\n";
    std::cout << "\033[1;36m  Neck:\033[0m               " << neck << " cm\n";
    std::cout << "\033[1;36m  Height:\033[0m             " << height << " cm\n";
    std::cout << "\033[1;36m  Lifestyle:\033[0m          " << lifestyle << "\n";
    if (gender == "female") {
        std::cout << "\033[1;36m  Hip Measurement:\033[0m    " << hip << " cm\n";
    }
    std::cout << "\033[1;33m=========================================\033[0m\n";

    std::cout << "\n\033[1;33m=========================================\033[0m\n";
    std::cout << "\033[1;33m      Body Fat & Calorie Information\033[0m\n";
    std::cout << "\033[1;33m=========================================\033[0m\n";
    std::cout << "\033[1;36m  Body Fat Percentage:\033[0m " << bodyfat.first << "%, " << bodyfat.second << "\n";
    std::cout << "\033[1;36m  Daily Calorie Intake:\033[0m " << intake << " calories\n";
    std::cout << "\033[1;33m=========================================\033[0m\n";

    // Display macronutrient breakdown
    std::cout << "\n\033[1;33m=========================================\033[0m\n";
    std::cout << "\033[1;33m         Macronutrient Breakdown\033[0m\n";
    std::cout << "\033[1;33m=========================================\033[0m\n";
    std::cout << "\033[1;36m  Carbohydrates:\033[0m       " << carbs << " grams\n";
    std::cout << "\033[1;36m  Protein:\033[0m             " << protein << " grams\n";
    std::cout << "\033[1;36m  Fat:\033[0m                 " << fat << " grams\n";
    std::cout << "\033[1;33m=========================================\033[0m\n";
}


//*** PART 6 ***//
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

//*** PART 7 ***//
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

//*** MAIN ***///
int main(int argc, char* argv[]) {
    // Check if a filename is provided as a command-line argument
    if (argc > 1) {
        // Read from the provided file
        readFromFile(argv[1]);
    }

    // Collect user information
    getUserDetails();
    
    // Calculate body fat %, calorie requirement, and macronutrient breakdown
    bodyfat = get_bfp(waist, neck, height, hip, gender, age);
    intake = get_daily_calories(age, gender, lifestyle);
    meal_prep(intake, carbs, protein, fat);

    // Display to screen
    display();

    // Serialize user data to CSV
    serialize("user_data.csv");

    return 0;
}

