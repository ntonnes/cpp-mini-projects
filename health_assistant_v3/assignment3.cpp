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

class UserInfoManager
{
    private:
        // Struct to hold user information
        struct UserInfo {
            int age=0;
            double weight=0.00;
            double waist=0.00;
            double neck=0.00;
            double height=0.00;
            double hip=0.00;
            std::pair<int, std::string> bfp={0, "none"};
            double calories=0;
            double carbs=0;
            double protein=0;
            double fat=0;
            std::string name;
            std::string gender;
            std::string lifestyle;
        };
        std::vector<UserInfo> mylist;

        // Template representing inputs for validateInput function
        // The first type is the attribute type (int, double, or string)
        // The second type is the constraint type (vector of string, pair of int, or pair of double)
        template <typename Typ, typename Constraint>

        // Function to validate string, integer, or double input
        void validateInput(const std::string& prompt, const std::string& error, Constraint constraints, Typ& attribute) {
            Typ input;
            std::cout << prompt;
            std::cin >> input;

            // Check if the attribute to update is of type int or double
            if constexpr (std::is_same<Typ, int>::value || std::is_same<Typ, double>::value) {
                // Check if input is invalid or outside the given limits
                if (!std::cin || input < constraints.first || input > constraints.second) {
                    // Clear the input buffer and throw an error
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    throw std::runtime_error(error);
                }
                // Set the attribute to the input value
                attribute = input;

            // Check if the attribute to update is of type string
            } else if constexpr (std::is_same<Typ, std::string>::value) {
                // Convert to lowercase
                std::transform(input.begin(), input.end(), input.begin(), ::tolower);
                // Check if input is not the list of valid strings
                if (std::find(constraints.begin(), constraints.end(), input) == constraints.end()) {
                    // Throw an error
                    throw std::runtime_error(error);
                }
                // Set the attribute to the input value
                attribute = input;

            // Throw an error if either the attribute type is unsupported
            } else {
                throw std::runtime_error("Input type" + std::string(typeid(Typ).name()) + " is not supported.");
            }
        }

    public:
        // Constructor
        UserInfoManager() {}

        // Destructor
        ~UserInfoManager() {}

        // Adds a user and their input information to the list
        void addUserInfo() {
            UserInfo newUser;

            // Prompt for name input
            std::string input;
            std::cout << "\nPlease enter your name: ";

            // Validate name input (only letters)
            std::cin >> input;
            for (char c : input) {
                if (!std::isalpha(c)) {
                    throw std::runtime_error("Invalid name. Please enter a name containing only letters.");
                }
            }
            newUser.name = input;
            
            // Prompt for and validate gender
            std::vector<std::string> validGenders = { "female", "male" };
            validateInput(
                "Please specify your gender as either male or female: ", 
                "Invalid gender. Gender must be either male or female.", 
                validGenders, newUser.gender
            );

            // Prompt for and validate age
            std::pair<int, int> ageRange = { 19, 79 };
            validateInput(
                "Enter your age: ", 
                "Invalid age. Age must be between 19 and 79.", 
                ageRange, newUser.age
            );

            // Prompt for and validate double-based measurements
            std::vector<std::pair<std::string, double*>> attributes = {
                {"Enter your body weight in kilograms: ", &newUser.weight},
                {"Enter your waist measurement in centimeters: ", &newUser.waist},
                {"Enter your neck measurement in centimeters: ", &newUser.neck},
                {"Enter your height measurement in centimeters: ", &newUser.height},
                {"Enter your hip measurement in centimeters: ", &newUser.hip}
            };

            // Prompt for and validate hip measurement if user is female
            for (auto& attribute : attributes) {
                if ((attribute.second == &newUser.hip) && (newUser.gender == "male")){
                    continue;
                };
                validateDouble(
                    attribute.first, 
                    "Invalid measurement. Please enter a value greater than 0: ", 
                    0, std::numeric_limits<double>::max(), *attribute.second
                );
            };  

            // Prompt for and validate lifestyle
            validateString(
                "Enter information about your current lifestyle (sedentary, moderate, or active): ", 
                "Invalid lifestyle. Please enter sedentary, moderate, or active: ", 
                { "sedentary", "moderate", "active" }, newUser.lifestyle
            );

            // Print success message and add user to list
            std::cout << "User " << newUser.name << " has been added.\n" << std::endl;
            mylist.push_back(newUser);
        }

        // Removes a user from the list
        void deleteUser(const std::string& username) {
            // Find user according to username
            auto user = std::find_if(mylist.begin(), mylist.end(), [&username](const UserInfo& user) {
                return user.name == username;
            });
            // Throw an error if user not found
            if (user == mylist.end()) {
                throw std::runtime_error("User with name " + username + " does not exist.");
            }
            // Remove user from list
            mylist.erase(user);
            std::cout << "User " << username << " has been deleted." << std::endl;
        }

        UserInfo findUser(const std::string& username) {
            // Find user according to username
            auto user = std::find_if(mylist.begin(), mylist.end(), [&username](const UserInfo& user) {
                return user.name == username;
            });
            // Throw an error if user not found
            if (user == mylist.end()) {
                throw std::runtime_error("User with name " + username + " does not exist.");
            }
            // Return user if found
            return *(user);
        }

        // Reads and populates list from file
        void readFromFile(std::string filename) {
            // Attempt to open file
            std::ifstream file(filename);
            if (!file) {
                std::cerr << "Error: Could not open file " << filename << std::endl;
                return;
            }
            std::string line;

            // Skip the header line
            std::getline(file, line);

            // For each user in the file, read and populate the list
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                UserInfo newUser;
                std::string field;

                std::getline(iss, newUser.name, ',');
                std::getline(iss, field, ','); newUser.gender = field;
                std::getline(iss, field, ','); newUser.age = std::stoi(field);
                std::getline(iss, field, ','); newUser.weight = std::stod(field);
                std::getline(iss, field, ','); newUser.waist = std::stod(field);
                std::getline(iss, field, ','); newUser.neck = std::stod(field);
                std::getline(iss, field, ','); newUser.height = std::stod(field);
                std::getline(iss, field, ','); newUser.hip = std::stod(field);
                std::getline(iss, field, ','); newUser.bfp.first = std::stoi(field);
                std::getline(iss, field, ','); newUser.bfp.second = field;
                std::getline(iss, field, ','); newUser.calories = std::stoi(field);
                std::getline(iss, field, ','); newUser.carbs = std::stod(field);
                std::getline(iss, field, ','); newUser.protein = std::stod(field);
                std::getline(iss, field, ','); newUser.fat = std::stod(field);
                std::getline(iss, newUser.lifestyle, ',');

                mylist.push_back(newUser);
            }
            std::cout << "\nData has been read from file " << filename << std::endl;
        }

        // Writes list to file
        void writeToFile(std::string filename) {
            std::ofstream file(filename);
            if (!file) {
                std::cerr << "Error: Could not open file " << filename << std::endl;
                return;
            }

            file << "name,gender,age,weight,waist,neck,height,hip,bfp,group,calories,carbs,protein,fat,lifestyle\n";

            for (const auto& user : mylist) {
                file << user.name << "," << user.gender << "," << user.age << "," << user.weight << "," 
                     << user.waist << "," << user.neck << "," << user.height << "," << user.hip << "," 
                     << user.bfp.first << "," << user.bfp.second << "," << user.calories << ","
                     << user.carbs << "," << user.protein << "," << user.fat << ","  << user.lifestyle << "\n";
            }
            std::cout << "\nData has been written to file " << filename << std::endl;
        }

        // Displays user information
        void display(std::string username) {
            if (username == "all") {
                std::cout << "\nDisplaying information for all users...\n";
                for (const auto& user : mylist) {
                    displayDetails(user);
                }
                std::cout << "Done.";
            } else {
                std::cout << "\nDisplaying information for user " << username << "...\n";
                UserInfo user = findUser(username);
                displayDetails(user);
                std::cout << "Done.\n";
            }
        }

        // Displays user information
        void displayDetails(const UserInfo& user) {
            int lineLength = 42;

            // Display the gathered information and results
            std::cout << "\n\033[1;33m=========================================\033[0m\n";
            std::cout << "\033[1;33m|            User: " << user.name << "\033[0m\n";
            std::cout << "\033[1;33m=========================================\033[0m\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Gender:\033[0m               " << user.gender << "\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Age:\033[0m                  " << user.age << " years\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Weight:\033[0m               " << user.weight << " kg\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Waist:\033[0m                " << user.waist << " cm\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Neck:\033[0m                 " << user.neck << " cm\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Height:\033[0m               " << user.height << " cm\n";
            if (user.gender == "female") {
                std::cout << "\033[1;33m|\033[0m" << "\033[1;36m Hips:\033[0m              " << user.hip << " cm\n";
            }
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Lifestyle:\033[0m            " << user.lifestyle << "\n";
            std::cout << "\033[1;33m|\033[0m\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;34m  Body Fat Percentage:\033[0m  " << user.bfp.first << "%, " << user.bfp.second << "\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;34m  Daily Calorie Intake:\033[0m " << user.calories << " calories\n";
            std::cout << "\033[1;33m|\033[0m\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;35m  Carbohydrates:\033[0m        " << user.carbs << " grams\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;35m  Protein:\033[0m              " << user.protein << " grams\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;35m  Fat:\033[0m                  " << user.fat << " grams\n";
            std::cout << "\033[1;33m=========================================\033[0m\n\n";
        }
    
};

class HealthAssistant {
    private:
        UserInfoManager mymanager;

    public:
        virtual double getBfp(std::string username) = 0;

        // Method to get the recommended daily calorie intake for a user
        void getDailyCalories(std::string username){
            // Get user information from the linked list
            mymanager.findUser(username).calories = 1600;

            // Set base calorie intake
            int calories = 1600;

            // Add additional calories based on age
            if (user->age < 51) { 
                calories += (user->age > 30) ? 200 : 400;
            }

            // Add additional calories for males
            calories += (user->gender == "male") ? 400 : 0;

            // Set scaling factor for activity level
            int activityBonus = (user->gender == "male") ? 300 : 200;

            // Add additional calories based on activity level
            if (user->lifestyle != "sedentary") {
                calories += (user->lifestyle == "moderate") ? activityBonus : (2*activityBonus);
            }

            // Set the user's daily calorie intake in the linked list and print the result
            user->calories = calories;
            std::cout << "\nRecommended daily calorie intake for " << username << " is " << calories << " calories." << std::endl;
        }
    
};