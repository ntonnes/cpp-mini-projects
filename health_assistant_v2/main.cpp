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


// Struct to hold user information
struct UserInfo {
    int age;
    double weight;
    double waist;
    double neck;
    double height;
    double hip;
    double carbs;
    double protein;
    double fat;
    std::string name;
    std::string gender;
    std::string lifestyle;
    UserInfo* next;
};


class UserInfoManager
{
    public:
        // Constructor. Initializes linked list
        UserInfoManager() : mylist(nullptr) {}

        // Destructor. Deletes allocated memory
        ~UserInfoManager() {
            UserInfo* current = mylist;
            while (current != nullptr) {
                UserInfo* next = current->next;
                delete current;
                current = next;
            }
        }

        // Adds info to list
        void addUserInfo() {
            UserInfo* newUser = new UserInfo;

            // Prompt for and validate gender
            validateString(
                "Please specify your gender as either male or female: ", 
                "Invalid gender. Please enter male or female: ", 
                { "female", "male" }, newUser->gender
            );

            // Prompt for and validate age
            validateInteger(
                "Enter your age: ", 
                "Invalid age. Please enter an age between 18 and 100: ", 
                18, 100, newUser->age
            );

            // Prompt for and validate double-based measurements
            std::vector<std::pair<std::string, double*>> attributes = {
                {"Enter your body weight in kilograms: ", &newUser->weight},
                {"Enter your waist measurement in centimeters: ", &newUser->waist},
                {"Enter your neck measurement in centimeters: ", &newUser->neck},
                {"Enter your height measurement in centimeters: ", &newUser->height}
            };
            for (auto& attribute : attributes) {
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
                { "sedentary", "moderate", "active" }, newUser->lifestyle);

            // Prompt for and validate hip measurement for female users
            if (newUser->gender == "female") {
                validateDouble(
                    "Enter your hip measurement in centimeters: ", 
                    "Invalid hip measurement. Please enter a value greater than 0: ", 
                    0, std::numeric_limits<double>::max(), newUser->hip
                );
            }

            newUser->next = mylist;
            mylist = newUser;
        }

        // Removes a user
        void deleteUser(std::string username) {
            UserInfo* current = mylist;
            UserInfo* prev = nullptr;
            while (current != nullptr) {
                if (current->name == username) {
                    if (prev != nullptr) {
                        prev->next = current->next;
                    } else {
                        mylist = current->next;
                    }
                    delete current;
                    return;
                }
                prev = current;
                current = current->next;
            }
        }

        // Reads and populates list from file
        void readFromFile(std::string filename) {
            std::ifstream file(filename);
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                UserInfo* newUser = new UserInfo;
                iss >> newUser->name >> newUser->age;
                newUser->next = mylist;
                mylist = newUser;
            }
        }

        // Writes list to file
        void writeToFile(std::string filename) {
            std::ofstream file(filename);
            UserInfo* current = mylist;
            while (current != nullptr) {
                file << current->name << " " << current->age << "\n";
                current = current->next;
            }
        }

        // Displays user information
        void display(std::string username) {
            UserInfo* current = mylist;
            while (current != nullptr) {
                if (current->name == username) {
                    std::cout << "Name: " << current->name << ", Age: " << current->age << "\n";
                    return;
                }
                current = current->next;
            }
        }

    private:
        UserInfo* mylist; // pointer to first element in linked list

        // Validates string inputs
        void validateString(const std::string& prompt, const std::string& error, const std::vector<std::string>& validStrings, std::string& attribute) {
            std::string input;

            while (true) {
                std::cout << prompt;
                std::cin >> input;

                // Convert to lowercase
                std::transform(input.begin(), input.end(), input.begin(), ::tolower);

                if (std::find(validStrings.begin(), validStrings.end(), input) != validStrings.end()) {
                    break;
                } else {
                    std::cout << error << std::endl;
                }
            }

            attribute = input;
        };

        // Validates double inputs
        void validateDouble(const std::string& prompt, const std::string& error, double lower, double upper, double& attribute) {
            double input;

            while (true) {
                std::cout << prompt;
                std::cin >> input;

                if (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << error << std::endl;
                } else if (input < lower || input > upper) {
                    std::cout << error << std::endl;
                } else {
                    break;
                }
            }

            attribute = input;
        }

        // Validates integer inputs
        void validateInteger(const std::string& prompt, const std::string& error, int lower, int upper, int& attribute) {
            int input;

            while (true) {
                std::cout << prompt;
                std::cin >> input;

                if (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << error << std::endl;
                } else if (input < lower || input > upper) {
                    std::cout << error << std::endl;
                } else {
                    break;
                }
            }

            attribute = input;
        }
};


// Class to assist with health-related tasks
class HealthAssistant {
    public:
        // Wrapper method that simply calls addUserInfo in class UserInfoManager
        void getUserDetail(); 

        // Gets body fat percentage
        void get_bfp(std::string username); {
            // Calculate bodyfat % by gender
            double BF_percentage;
            if (gender == "male") {
                BF_percentage = 495 / (1.0324 - 0.19077 * log10(waist - neck) + 0.15456 * log10(height)) - 450;
            } else if (gender == "female") {
                BF_percentage = 495 / (1.29579 - 0.35004 * log10(waist + hip - neck) + 0.22100 * log10(height)) - 450;
            }
        }

        // Gets daily calorie intake
        void getDailyCalories(std::string username);

        // Gets meal preparation details
        void getMealPrep(std::string username);

        // Wrapper method to display user information
        void display(std::string username); 

        // Wrapper method to serialize user information
        void serialize(std::string filename); 

        // Wrapper method to read user information from file
        void readFromFile(std::string filename); 

        // Wrapper method to delete user
        void deleteUser(std::string username); 

    private:
        // Instance of UserInfoManager
        UserInfoManager mymanager;
};


// Main function
int main() {
    // Create HealthAssistant instance
    HealthAssistant ha;

    // Get user details from console input for 1st user
    ha.getUserDetail();

    // Get user details from console input for 2nd user
    ha.getUserDetail();

    // Display information for john (assuming that john is the name of the first user)
    ha.display("john");

    // Display information for all the users
    ha.display("all");

    // Get bfp for the 1st user
    ha.getBfp("john");

    // Get daily calories for the 1st user
    ha.getDailyCalories("john");

    // Get meal prep for the 1st user
    ha.getMealPrep("john");

    // Write all the data for all the users to file
    ha.serialize("user_data.csv");

    // Now let's get the data from file using a new instance of HealthAssistant
    HealthAssistant ha2;
    ha2.readFromFile("user_data.csv");

    // Display information for all the users
    ha2.display("all");

    // Delete user jack (assuming 2nd user is jack)
    ha2.deleteUser("jack"); 

    // Display information for all the users (should only display for john at this point)
    ha2.display("all"); 

    // Now use the first object to display all (both john and jack should be present)
    ha.display("all"); 
}