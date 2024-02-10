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
#include <iomanip>

// Global variables to store user details
std::string gender, lifestyle;
int age;
double weight, waist, neck, height, hip;

// Global variables to calculated values
int intake;
std::pair<int, std::string> bodyfat;
double carbs, protein, fat;


// Helper functions to validate user input
void getValidatedAge(int& age) {
    std::cout << "Enter your age: ";

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // Use stringstream to check if the input is a valid integer
        std::stringstream ss(input);
        int tempAge;

        // Validate age
        if (ss >> tempAge && ss.eof() && !std::isspace(input.at(0)) && (tempAge > 19 && tempAge < 80)) {
            age = tempAge;
            return;
        }
        
        // Input is invalid
        std::cout << "Invalid age. Please enter an age between 20 and 79: ";
    }
}
void getValidatedPositiveDouble(const std::string& prompt, const std::string& errorMessage, double& value) {
    std::cout << prompt;
    
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // Use stringstream to check if the input is a valid double
        std::stringstream ss(input);
        double temp;

        // Validate all other measurements
        if (ss >> temp && ss.eof() && !std::isspace(input.at(0)) && (temp > 0)) {
            value = temp;
            return;
        }
        
        // Input is not a valid double or is not greater than 0
        std::cout << errorMessage;
    }
}
void getValidatedStringInput(const std::string& prompt, const std::string& errorMessage, const std::vector<std::string>& validInputs, std::string& userInput) {
    std::cout << prompt;

    while (true) {
        std::getline(std::cin, userInput);

        // Convert the user input to lowercase for case-insensitive comparison
        std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

        // Check if the user input is in the list of valid inputs
        if (std::find(validInputs.begin(), validInputs.end(), userInput) != validInputs.end()) {
            return;
        }

        // Invalid input
        std::cout << errorMessage;
    }
}


//*** PART 1 ***//
void promptUserDetails() {
    // Valid inputs
    std::vector<std::string> validGenders = { "female", "male" };
    std::vector<std::string> validLifestyles = { "sedentary", "moderate", "active" };

    // Prompt for and validate gender
    getValidatedStringInput("Please specify your gender as either male or female: ", "Invalid gender. Please enter male or female: ", validGenders, gender);

    // Prompt for and validate age
    getValidatedAge(age);

    // Prompt for and validate double-based measurements
    getValidatedPositiveDouble("Enter your body weight in kilograms: ", "Invalid weight. Please enter a weight greater than 0: ", weight);
    getValidatedPositiveDouble("Enter your waist measurement in centimeters: ", "Invalid waist measurement. Please enter a value greater than 0: ", waist);
    getValidatedPositiveDouble("Enter your neck measurement in centimeters: ", "Invalid neck measurement. Please enter a value greater than 0: ", neck);
    getValidatedPositiveDouble("Enter your height measurement in centimeters: ", "Invalid height measurement. Please enter a value greater than 0: ", height);

    // Prompt for and validate lifestyle
    getValidatedStringInput("Enter information about your current lifestyle (sedentary, moderate, or active): ", "Invalid lifestyle. Please enter sedentary, moderate, or active: ", validLifestyles, lifestyle);

    // Prompt for and validate hip measurement for female users
    if (gender == "female") {
        getValidatedPositiveDouble("Enter your hip measurement in centimeters: ", "Invalid hip measurement. Please enter a value greater than 0: ", hip);
    }
}


//*** PART 2 ***//
std::pair<int, std::string> calculateBodyFatPercentageAndCategory(double waist, double neck, double height, double hip, const std::string& gender, int age) {
    // Calculate bodyfat % by gender
    double bodyFatPercentage;
    if (gender == "male") {
        bodyFatPercentage = 495 / (1.0324 - 0.19077 * log10(waist - neck) + 0.15456 * log10(height)) - 450;
    } else if (gender == "female") {
        bodyFatPercentage = 495 / (1.29579 - 0.35004 * log10(waist + hip - neck) + 0.22100 * log10(height)) - 450;
    } else {
        // Handle invalid gender
        std::cerr << "Invalid gender specified.\n";
        return {0, "Invalid"};
    }

    // Determine the body fat % category
    std::string category;
    std::vector<std::pair<double, std::string>> thresholds;
    if (gender == "female") {
        thresholds = age >= 60 ? std::vector<std::pair<double, std::string>>{{24, "low"}, {36, "normal"}, {42, "high"}}
                               : age >= 40 ? std::vector<std::pair<double, std::string>>{{23, "low"}, {34, "normal"}, {40, "high"}}
                                           : std::vector<std::pair<double, std::string>>{{21, "low"}, {33, "normal"}, {39, "high"}};
    } else if (gender == "male") {
        thresholds = age >= 60 ? std::vector<std::pair<double, std::string>>{{13, "low"}, {25, "normal"}, {30, "high"}}
                               : age >= 40 ? std::vector<std::pair<double, std::string>>{{11, "low"}, {22, "normal"}, {28, "high"}}
                                           : std::vector<std::pair<double, std::string>>{{8, "low"}, {20, "normal"}, {25, "high"}};
    }

    for (const auto& threshold : thresholds) {
        if (bodyFatPercentage < threshold.first) {
            category = threshold.second;
            break;
        }
    }
    if (category.empty()) {
        category = "very high";
    }

    return {static_cast<int>(bodyFatPercentage), category};
}


//*** PART 3 ***//
int calculateDailyCalories(double age, const std::string& gender, const std::string& lifestyle) {
    // Base calories
    int dailyCalories = 1600;

    // Additional calories based on age
    dailyCalories += (age < 51) ? ((age > 30) ? 200 : 400) : 0;

    // Additional calories for males
    dailyCalories += (gender == "male") ? 400 : 0;

    // Scaling factor for activity level
    int activityBonus = (gender == "male") ? 300 : 200;

    // Additional calories based on activity level
    if (lifestyle != "sedentary") {
        dailyCalories += (lifestyle == "moderate") ? activityBonus : (2 * activityBonus);
    }

    return dailyCalories;
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
    const std::string yellowBold = "\033[1;33m";
    const std::string cyanBold = "\033[1;36m";
    const std::string reset = "\033[0m";
    const std::string separator = "=========================================";

    auto printSectionHeader = [&](const std::string& title) {
        std::cout << "\n" << yellowBold << separator << reset << "\n";
        std::cout << yellowBold << std::setw((separator.length() + title.length()) / 2) << std::right << title << reset << "\n";
        std::cout << yellowBold << separator << reset << "\n";
    };

    auto printDetail = [&](const std::string& detail, auto value) {
        std::cout << cyanBold << std::setw(20) << std::left << detail << reset << ": " << value << "\n";
    };

    printSectionHeader("User Details");
    printDetail("Gender", gender);
    printDetail("Age", std::to_string(age) + " years");
    printDetail("Weight", std::to_string(weight) + " kg");
    printDetail("Waist", std::to_string(waist) + " cm");
    printDetail("Neck", std::to_string(neck) + " cm");
    printDetail("Height", std::to_string(height) + " cm");
    printDetail("Lifestyle", lifestyle);
    if (gender == "female") {
        printDetail("Hip Measurement", std::to_string(hip) + " cm");
    }

    printSectionHeader("Body Fat & Calorie Information");
    printDetail("Body Fat Percentage", std::to_string(bodyfat.first) + "%, " + bodyfat.second);
    printDetail("Daily Calorie Intake", std::to_string(intake) + " calories");

    printSectionHeader("Macronutrient Breakdown");
    printDetail("Carbohydrates", std::to_string(carbs) + " grams");
    printDetail("Protein", std::to_string(protein) + " grams");
    printDetail("Fat", std::to_string(fat) + " grams\n");
}


//*** PART 6 ***//
void serialize(const std::string& filename) {
    // Open the file in append mode to preserve existing data
    std::ofstream file(filename, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Error opening file for serialization.\n";
        return;
    }

    // Write user details to the file in CSV format
    file << gender << "," << age << "," << weight << "," << waist << "," << neck << ",";

    // Add hip measurement only for female users
    file << ((gender == "female") ? std::to_string(hip) : "") << ",";

    file << height << "," << lifestyle << "\n";

    // Close the file
    file.close();
}


//*** PART 7 ***//
void readFromFile(const std::string& filename) {
    // Open the file for reading
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file for reading.\n";
        return;
    }

    // Variables to temporarily store CSV values
    std::string line;

    // Read each line from the file
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> values;
        std::string value;

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
            hip = values[5].empty() ? 0.0 : std::stod(values[5]);
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
    const std::string defaultFilename = "user_data.csv";

    // Check if a filename is provided as a command-line argument
    std::string filename = (argc > 1) ? argv[1] : defaultFilename;

    // Read from the provided file or the default file
    readFromFile(filename);

    // Collect user information
    promptUserDetails();
    
    // Calculate body fat %, calorie requirement, and macronutrient breakdown
    bodyfat = calculateBodyFatPercentageAndCategory(waist, neck, height, hip, gender, age);
    intake = calculateDailyCalories(age, gender, lifestyle);
    meal_prep(intake, carbs, protein, fat);

    // Display to screen
    display();

    // Serialize user data to CSV
    serialize(filename);

    return 0;
}

