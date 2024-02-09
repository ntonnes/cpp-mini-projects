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


std::string gender;                     ///< The user's gender. Can be "male" or "female".
std::string lifestyle;                  ///< The user's lifestyle. Can be "sedentary", "lightly active", "moderately active", or "very active".
int age;                                ///< The user's age in years. Must be an integer between 20 and 79, inclusive.
double weight;                          ///< The user's weight in kilograms.
double waist;                           ///< The user's waist measurement in centimeters.
double neck;                            ///< The user's neck measurement in centimeters.
double height;                          ///< The user's height in centimeters.
double hip;                             ///< The user's hip measurement in centimeters. Only applicable if the user is female.

int intake;                             ///< The user's daily calorie intake.
std::pair<int, std::string> bodyfat;    ///< The user's body fat percentage and category. The first element is the body fat percentage, and the second element is the category.

double carbs;                           ///< The user's daily carbohydrate intake in grams.
double protein;                         ///< The user's daily protein intake in grams.
double fat;                             ///< The user's daily fat intake in grams.


/**
 * @brief Prompts the user for their age and validates the input.
 *
 * This function repeatedly prompts the user to enter their age until a valid age is entered.
 * A valid age is an integer between 20 and 79, inclusive.
 *
 * @param age Reference to an integer where the validated age will be stored.
 */
void getValidatedAge(int& age) {
    std::cout << "Enter your age: ";

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // Use stringstream to check if the input is a valid integer
        std::stringstream ss(input);
        int tempAge;

        // The age must be an integer, the input must not start with a space, and the age must be between 20 and 79
        if (ss >> tempAge && ss.eof() && !std::isspace(input.at(0)) && (tempAge > 19 && tempAge < 80)) {
            age = tempAge;
            return;
        }
        
        // If the input is invalid, print an error message and prompt the user again
        std::cout << "Invalid age. Please enter an age between 20 and 79: ";
    }
}


/**
 * @brief Prompts the user for a measurement (positive double) value and validates the input.
 *
 * This function repeatedly prompts the user to enter a positive double value until a valid value is entered.
 * A valid value is a double greater than 0.
 *
 * @param prompt The prompt to display to the user.
 * @param errorMessage The error message to display if the input is invalid.
 * @param value Reference to a double where the validated value will be stored.
 */
void getValidatedPositiveDouble(const std::string& prompt, const std::string& errorMessage, double& value) {
    std::cout << prompt;
    
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        // Use stringstream to check if the input is a valid double
        std::stringstream ss(input);
        double temp;

        // The input must be a valid double, the input must not start with a space, and the value must be greater than 0
        if (ss >> temp && ss.eof() && !std::isspace(input.at(0)) && (temp > 0)) {
            value = temp;
            return;
        }
        
        // If the input is invalid, print an error message and prompt the user again
        std::cout << errorMessage;
    }
}


/**
 * @brief Prompts the user for a string input and validates the input.
 *
 * This function repeatedly prompts the user to enter a string until a valid input is entered.
 * A valid input is one of the strings in the validInputs vector.
 *
 * @param prompt The prompt to display to the user.
 * @param errorMessage The error message to display if the input is invalid.
 * @param validInputs A vector of strings containing the valid inputs.
 * @param userInput Reference to a string where the validated input will be stored.
 */
void getValidatedStringInput(const std::string& prompt, const std::string& errorMessage, const std::vector<std::string>& validInputs, std::string& userInput) {
    std::cout << prompt;

    while (true) {
        std::getline(std::cin, userInput);

        // Convert the user input to lowercase
        std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

        // Check if the user input is in the list of valid inputs
        if (std::find(validInputs.begin(), validInputs.end(), userInput) != validInputs.end()) {
            return;
        }

        // If the input is invalid, print an error message and prompt the user again
        std::cout << errorMessage;
    }
}


/**
 * @brief Prompts the user for their personal details and validates the input.
 *
 * This function prompts the user to enter their gender, age, body measurements, and lifestyle.
 * The gender must be either "male" or "female".
 * The age must be an integer between 20 and 79, inclusive.
 * The body measurements (weight, waist, neck, height, and hip) must be positive doubles.
 * The lifestyle must be either "sedentary", "moderate", or "active".
 * If the user is female, they are also prompted to enter their hip measurement.
 */
void getUserDetails() {
    // Valid input options
    std::vector<std::string> validGenders = { "female", "male" };
    std::vector<std::string> validLifestyles = { "sedentary", "moderate", "active" };

    // Prompt for and validate gender
    getValidatedStringInput("Please specify your gender as either male or female: ", "Invalid gender. Please enter male or female: ", validGenders, gender);

    // Prompt for and validate age
    getValidatedAge(age);

    // Prompt for and validate measurements
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


/**
 * @brief Calculates the body fat percentage and category based on the given measurements, gender, and age.
 *
 * This function uses different formulas to calculate the body fat percentage for males and females.
 * It then determines the body fat category based on the calculated body fat percentage and the user's age.
 * The body fat category is determined by comparing the body fat percentage to a set of thresholds that vary based on the user's gender and age.
 *
 * @param waist The user's waist measurement in centimeters.
 * @param neck The user's neck measurement in centimeters.
 * @param height The user's height in centimeters.
 * @param hip The user's hip measurement in centimeters. Only used if the user is female.
 * @param gender The user's gender. Must be either "male" or "female".
 * @param age The user's age in years.
 * @return A pair containing the calculated body fat percentage and category.
 */
std::pair<int, std::string> get_bfp(double waist, double neck, double height, double hip, const std::string& gender, int age) {
    // Initialize body fat percentage
    double bodyFatPercentage;

    // Calculate body fat percentage based on gender
    if (gender == "male") {
        bodyFatPercentage = 495 / (1.0324 - 0.19077 * log10(waist - neck) + 0.15456 * log10(height)) - 450;
    } else if (gender == "female") {
        bodyFatPercentage = 495 / (1.29579 - 0.35004 * log10(waist + hip - neck) + 0.22100 * log10(height)) - 450;
    }

    // Initialize category and thresholds
    std::string category;
    std::vector<std::pair<double, std::string>> thresholds;

    // Set thresholds based on gender and age
    if (gender == "female") {
        thresholds = age >= 60 ? std::vector<std::pair<double, std::string>>{{24, "low"}, {36, "normal"}, {42, "high"}}
                               : age >= 40 ? std::vector<std::pair<double, std::string>>{{23, "low"}, {34, "normal"}, {40, "high"}}
                                           : std::vector<std::pair<double, std::string>>{{21, "low"}, {33, "normal"}, {39, "high"}};
    } else if (gender == "male") {
        thresholds = age >= 60 ? std::vector<std::pair<double, std::string>>{{13, "low"}, {25, "normal"}, {30, "high"}}
                               : age >= 40 ? std::vector<std::pair<double, std::string>>{{11, "low"}, {22, "normal"}, {28, "high"}}
                                           : std::vector<std::pair<double, std::string>>{{8, "low"}, {20, "normal"}, {25, "high"}};
    }

    // Determine the body fat category based on the thresholds
    for (const auto& threshold : thresholds) {
        if (bodyFatPercentage < threshold.first) {
            category = threshold.second;
            break;
        }
    }

    // If no category has been set, set it to "very high"
    if (category.empty()) {
        category = "very high";
    }

    // Return the body fat percentage and category as a pair
    return {static_cast<int>(bodyFatPercentage), category};
}


/**
 * @brief Calculates the daily caloric intake based on age, gender, and lifestyle.
 *
 * This function starts with a base caloric intake and then adjusts it based on the user's age, gender, and lifestyle.
 * The age and lifestyle adjustments are tiered systems where younger and more active people require more calories.
 * Males require additional base calories compared to females.
 *
 * @param age The user's age in years.
 * @param gender The user's gender. Must be either "male" or "female".
 * @param lifestyle The user's lifestyle. Must be either "sedentary", "moderate", or "active".
 * @return The calculated daily caloric intake.
 */
int get_daily_calories(double age, const std::string& gender, const std::string& lifestyle) {
    // Start with a base caloric intake
    int dailyCalories = 1600;

    // Adjust caloric intake based on age
    // People under 30 require an additional 400 calories, people between 30 and 50 require an additional 200 calories
    dailyCalories += (age < 51) ? ((age > 30) ? 200 : 400) : 0;

    // Adjust caloric intake for males
    // Males require an additional 400 calories
    dailyCalories += (gender == "male") ? 400 : 0;

    // Set the activity bonus based on gender
    // Males get a larger bonus for activity than females
    int activityBonus = (gender == "male") ? 300 : 200;

    // Adjust caloric intake based on lifestyle
    // Sedentary lifestyles get no bonus, moderate lifestyles get the activity bonus, active lifestyles get twice the activity bonus
    if (lifestyle != "sedentary") {
        dailyCalories += (lifestyle == "moderate") ? activityBonus : (2 * activityBonus);
    }

    // Return the calculated daily caloric intake
    return dailyCalories;
}


/**
 * @brief Calculates the grams of each macronutrient needed based on a given caloric intake.
 *
 * This function uses a fixed percentage breakdown for each macronutrient: 50% carbs, 30% protein, and 20% fat.
 * It then calculates the grams of each macronutrient needed to meet these percentages based on the given caloric intake.
 * The results are returned through output parameters.
 *
 * @param calories_input The daily caloric intake.
 * @param carbs_output The calculated grams of carbs needed. This is an output parameter.
 * @param protein_output The calculated grams of protein needed. This is an output parameter.
 * @param fat_output The calculated grams of fat needed. This is an output parameter.
 */
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


/**
 * @brief Displays user details, body fat & calorie information, and macronutrient breakdown.
 *
 * This function uses ANSI escape codes to color and format the output.
 * It defines two helper functions, printSectionHeader and printDetail, to print section headers and details, respectively.
 * It then uses these helper functions to print the user details, body fat & calorie information, and macronutrient breakdown.
 *
 * @note This function assumes that the gender, age, weight, waist, neck, height, lifestyle, hip, bodyfat, intake, carbs, protein, and fat variables are defined in the same scope.
 */
void display() {
    // ANSI escape codes for color and formatting
    const std::string yellowBold = "\033[1;33m";
    const std::string cyanBold = "\033[1;36m";
    const std::string reset = "\033[0m";
    const std::string separator = "=========================================";

    // Helper function to print a section header
    auto printSectionHeader = [&](const std::string& title) {
        std::cout << "\n" << yellowBold << separator << reset << "\n";
        std::cout << yellowBold << std::setw((separator.length() + title.length()) / 2) << std::right << title << reset << "\n";
        std::cout << yellowBold << separator << reset << "\n";
    };

    // Helper function to print a detail
    auto printDetail = [&](const std::string& detail, auto value) {
        std::cout << cyanBold << std::setw(20) << std::left << detail << reset << ": " << value << "\n";
    };

    // Print user details
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

    // Print body fat & calorie information
    printSectionHeader("Body Fat & Calorie Information");
    printDetail("Body Fat Percentage", std::to_string(bodyfat.first) + "%, " + bodyfat.second);
    printDetail("Daily Calorie Intake", std::to_string(intake) + " calories");

    // Print macronutrient breakdown
    printSectionHeader("Macronutrient Breakdown");
    printDetail("Carbohydrates", std::to_string(carbs) + " grams");
    printDetail("Protein", std::to_string(protein) + " grams");
    printDetail("Fat", std::to_string(fat) + " grams");
}


/**
 * @brief Serializes user details to a CSV file.
 *
 * This function opens the specified file in append mode, writes the user details to the file in CSV format, and then closes the file.
 * If the file cannot be opened, an error message is printed and the function returns early.
 * The user details are written in the following order: gender, age, weight, waist, neck, hip (only for females), height, lifestyle.
 *
 * @param filename The name of the file to serialize the user details to.
 *
 * @note This function assumes that the gender, age, weight, waist, neck, height, lifestyle, and hip variables are defined in the same scope.
 */
void serialize(const std::string& filename) {
    // Open the file in append mode to preserve existing data
    std::ofstream file(filename, std::ios::app);

    // If the file cannot be opened, print an error message and return early
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


/**
 * @brief Reads user details from a CSV file.
 *
 * This function opens the specified file for reading, reads each line of the file, splits the line into values using commas, and then assigns the values to the user detail variables.
 * If the file cannot be opened, an error message is printed and the function returns early.
 * If a line does not have enough values (at least 8 for all user details), an error message is printed and the line is skipped.
 * The user details are read in the following order: gender, age, weight, waist, neck, hip (only for females), height, lifestyle.
 *
 * @param filename The name of the file to read the user details from.
 *
 * @note This function assumes that the gender, age, weight, waist, neck, height, lifestyle, and hip variables are defined in the same scope.
 */
void readFromFile(const std::string& filename) {
    // Open the file for reading
    std::ifstream file(filename);

    // If the file cannot be opened, print an error message and return early
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


/**
 * @brief The main function of the program.
 *
 * This function performs the following steps:
 * 1. Checks if a filename is provided as a command-line argument. If not, it collects user details.
 * 2. If a filename is provided, it reads user details from the specified file.
 * 3. Calculates the user's body fat percentage, daily caloric intake, and macronutrient breakdown.
 * 4. Displays the user details, body fat & calorie information, and macronutrient breakdown to the screen.
 * 5. Serializes the user details to a default file named "user_data.csv".
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return 0 on successful execution.
 */
int main(int argc, char* argv[]) {
    // Check if a filename is provided as an argument
    if (argc > 1) {
        // Load existing user data from the specified CSV file
        readFromFile(argv[1]);
    } else {
        // Gather user details
        getUserDetails();
    }

    // Calculate body fat percentage
    auto bfpResult = get_bfp(g_waist, g_neck, g_height, g_hip, g_gender, g_age);
    std::cout << "Body Fat Percentage: " << bfpResult.first << "% (" << bfpResult.second << ")" << std::endl;

    // Calculate daily calorie intake
    int dailyCalories = get_daily_calories(g_age, g_gender, g_lifestyle);
    std::cout << "Daily Caloric Intake: " << dailyCalories << " calories" << std::endl;

    // Calculate macronutrient breakdown
    double carbs, protein, fat;
    meal_prep(dailyCalories, carbs, protein, fat);
    std::cout << "Macronutrient Breakdown:\n";
    std::cout << "Carbs: " << carbs << "g, Protein: " << protein << "g, Fat: " << fat << "g" << std::endl;

    // Display user information
    display();

    // Save user data to a CSV file
    serialize("user_data.csv");

    return 0;
}

