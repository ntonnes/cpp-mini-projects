#include <iostream>
#include <cmath>
#include <string>
#include <utility>

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
            std::cerr << "Invalid age specified.\n";
            return {0, "Invalid"};
        }
    }

    return {static_cast<int>(BF_percentage), group};
}

int get_daily_calories(double age, std::string gender, std::string lifestyle) {
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
        // Handle invalid gender
        std::cerr << "Invalid gender specified.\n";
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
    auto bfp_result = get_bfp(waist, neck, height, hip, gender, age);
    int daily_calories = get_daily_calories(age, gender, lifestyle);
    double carbs, protein, fat;
    meal_prep(daily_calories, carbs, protein, fat);

    // Displaying the gathered information and results
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

int main() {
    // Example of using the getUserDetails function
    getUserDetails();
    display();
    return 0;
}

