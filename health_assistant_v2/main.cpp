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

            // Prompt for and validate name
            std::string input;
            std::cout << "\nPlease enter your name: ";
            while (true) {
                std::cin >> input;
                bool valid = true;
                for (char c : input) { if (!std::isalpha(c)) { valid = false; break; } }

                if (valid) { 
                    newUser->gender = input; break;
                } else { std::cout << "Invalid name. Please enter a name containing only letters: "; }
            }
            newUser->name = input;

            // Prompt for and validate gender
            validateString(
                "Please specify your gender as either male or female: ", 
                "Invalid gender. Please enter male or female: ", 
                { "female", "male" }, newUser->gender
            );

            // Prompt for and validate age
            validateInteger(
                "Enter your age: ", 
                "Invalid age. Please enter an age between 19 and 80: ", 
                18, 100, newUser->age
            );

            // Prompt for and validate double-based measurements
            std::vector<std::pair<std::string, double*>> attributes = {
                {"Enter your body weight in kilograms: ", &newUser->weight},
                {"Enter your waist measurement in centimeters: ", &newUser->waist},
                {"Enter your neck measurement in centimeters: ", &newUser->neck},
                {"Enter your height measurement in centimeters: ", &newUser->height},
                {"Enter your hip measurement in centimeters: ", &newUser->hip}
            };
            for (auto& attribute : attributes) {
                if ((attribute.second == &newUser->hip) && (newUser->gender == "male")){
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
                { "sedentary", "moderate", "active" }, newUser->lifestyle
            );

            std::cout << "User " << newUser->name << " has been added.\n" << std::endl;
            newUser->next = mylist;
            mylist = newUser;
        }

        UserInfo* findUser(const std::string& username) {
            UserInfo* current = mylist;
            while (current != nullptr) {
                if (current->name == username) {
                    return current;
                }
                current = current->next;
            }
            std::cerr << "Error: User with name " << username << " does not exist." << std::endl;
            return nullptr;
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
                    std::cout << "User " << username << " has been deleted." << std::endl;
                    return;
                }
                prev = current;
                current = current->next;
            }
            std::cout << "User " << username << " does not exist." << std::endl;
        }

        // Reads and populates list from file
        void readFromFile(std::string filename) {
            std::ifstream file(filename);
            if (!file) {
                std::cerr << "Error: Could not open file " << filename << std::endl;
                return;
            }
            std::string line;

            // Skip the header line
            std::getline(file, line);

            while (std::getline(file, line)) {
                std::istringstream iss(line);
                UserInfo* newUser = new UserInfo;
                std::string field;

                std::getline(iss, newUser->name, ',');
                std::getline(iss, field, ','); newUser->gender = field;
                std::getline(iss, field, ','); newUser->age = std::stoi(field);
                std::getline(iss, field, ','); newUser->weight = std::stod(field);
                std::getline(iss, field, ','); newUser->waist = std::stod(field);
                std::getline(iss, field, ','); newUser->neck = std::stod(field);
                std::getline(iss, field, ','); newUser->height = std::stod(field);
                std::getline(iss, field, ','); newUser->hip = std::stod(field);
                std::getline(iss, field, ','); newUser->bfp.first = std::stoi(field);
                std::getline(iss, field, ','); newUser->bfp.second = field;
                std::getline(iss, field, ','); newUser->calories = std::stoi(field);
                std::getline(iss, field, ','); newUser->carbs = std::stod(field);
                std::getline(iss, field, ','); newUser->protein = std::stod(field);
                std::getline(iss, field, ','); newUser->fat = std::stod(field);
                std::getline(iss, newUser->lifestyle, ',');

                newUser->next = mylist;
                mylist = newUser;
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

            UserInfo* current = mylist;
            while (current != nullptr) {
                file << current->name << "," << current->gender << "," << current->age << "," << current->weight << "," 
                     << current->waist << "," << current->neck << "," << current->height << "," << current->hip << "," 
                     << current->bfp.first << "," << current->bfp.second << "," << current->calories << ","
                     << current->carbs << "," << current->protein << "," << current->fat << ","  << current->lifestyle << "\n";
                current = current->next;
            }
            std::cout << "\nData has been written to file " << filename << std::endl;
        }

        // Displays user information
        void display(std::string username) {
            if (username == "all") {
                std::cout << "\nDisplaying information for all users...\n";
                UserInfo* current = mylist;
                while (current != nullptr) {
                    displayDetails(current);
                    current = current->next;
                }
                std::cout << "Done.";
            } else {
                std::cout << "\nDisplaying information for user " << username << "...\n";
                UserInfo* user = findUser(username);
                if (user == nullptr) { return; }
                displayDetails(user);
                std::cout << "Done.\n";
            }
        }
        // Displays user information
        void displayDetails(UserInfo* user) {

            int lineLength = 42;

            // Display the gathered information and results
            std::cout << "\n\033[1;33m=========================================\033[0m\n";
            std::cout << "\033[1;33m|            User: " << user->name << "\033[0m\n";
            std::cout << "\033[1;33m=========================================\033[0m\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Gender:\033[0m               " << user->gender << "\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Age:\033[0m                  " << user->age << " years\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Weight:\033[0m               " << user->weight << " kg\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Waist:\033[0m                " << user->waist << " cm\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Neck:\033[0m                 " << user->neck << " cm\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Height:\033[0m               " << user->height << " cm\n";
            if (user->gender == "female") {
                std::cout << "\033[1;33m|\033[0m" << "\033[1;36m Hips:\033[0m              " << user->hip << " cm\n";
            }
            std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Lifestyle:\033[0m            " << user->lifestyle << "\n";
            std::cout << "\033[1;33m|\033[0m\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;34m  Body Fat Percentage:\033[0m  " << user->bfp.first << "%, " << user->bfp.second << "\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;34m  Daily Calorie Intake:\033[0m " << user->calories << " calories\n";
            std::cout << "\033[1;33m|\033[0m\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;35m  Carbohydrates:\033[0m        " << user->carbs << " grams\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;35m  Protein:\033[0m              " << user->protein << " grams\n";
            std::cout << "\033[1;33m|\033[0m" << "\033[1;35m  Fat:\033[0m                  " << user->fat << " grams\n";
            std::cout << "\033[1;33m=========================================\033[0m\n\n";
        }

    private:
        UserInfo* mylist; // pointer to first element in linked list

        // Validates string inputs
        void validateString(const std::string& prompt, const std::string& error, const std::vector<std::string>& validStrings, std::string& attribute) {
            std::string input;
            std::cout << prompt;
            while (true) {
                std::cin >> input;

                // Convert to lowercase
                std::transform(input.begin(), input.end(), input.begin(), ::tolower);

                if (std::find(validStrings.begin(), validStrings.end(), input) != validStrings.end()) {
                    break;
                } else {
                    std::cout << error;
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
                    std::cout << error;
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
                    std::cout << error;
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
        void getUserDetail(){ mymanager.addUserInfo(); }

        // Gets body fat percentage
        void getBfp(std::string username) {
            UserInfo* user = mymanager.findUser(username);
            if (user == nullptr) { return; }

            // Calculate bodyfat % by gender
            double bfp;
            if (user->gender == "male") {
                bfp = 495 / (1.0324 - 0.19077 * log10(user->waist - user->neck) + 0.15456 * log10(user->height)) - 450;
            } else if (user->gender == "female") {
                bfp = 495 / (1.29579 - 0.35004 * log10(user->waist + user->hip - user->neck) + 0.22100 * log10(user->height)) - 450;
            }
                // Determine the body fat % category
            std::string group;
            if (user->gender == "female") {
                if (user->age >= 20 && user->age <= 39) {
                    if (bfp < 21) {
                        group = "low";
                    } else if (bfp >= 21 && bfp < 32.9) {
                        group = "normal";
                    } else if (bfp >= 33 && bfp < 38.9) {
                        group = "high";
                    } else {
                        group = "very high";
                    }
                } else if (user->age >= 40 && user->age <= 59) {
                    if (bfp < 23) {
                        group = "low";
                    } else if (bfp >= 23 && bfp < 33.9) {
                        group = "normal";
                    } else if (bfp >= 34 && bfp < 39.9) {
                        group = "high";
                    } else {
                        group = "very high";
                    }
                } else if (user->age >= 60 && user->age <= 79) {
                    if (bfp < 24) {
                        group = "low";
                    } else if (bfp >= 24 && bfp < 35.9) {
                        group = "normal";
                    } else if (bfp >= 36 && bfp < 41.9) {
                        group = "high";
                    } else {
                        group = "very high";
                    }
                }
            } else if (user->gender == "male") {
                if (user->age >= 20 && user->age <= 39) {
                    if (bfp < 8) {
                        group = "low";
                    } else if (bfp >= 8 && bfp < 19.9) {
                        group = "normal";
                    } else if (bfp >= 20 && bfp < 24.9) {
                        group = "high";
                    } else {
                        group = "very high";
                    }
                } else if (user->age >= 40 && user->age <= 59) {
                    if (bfp < 11) {
                        group = "low";
                    } else if (bfp >= 11 && bfp < 21.9) {
                        group = "normal";
                    } else if (bfp >= 22 && bfp < 27.9) {
                        group = "high";
                    } else {
                        group = "very high";
                    }
                } else if (user->age >= 60) {
                    if (bfp < 13) {
                        group = "low";
                    } else if (bfp >= 13 && bfp <= 24.9) {
                        group = "normal";
                    } else if (bfp >= 25 && bfp <= 29.9) {
                        group = "high";
                    } else {
                        group = "very high";
                    }
                }
            }
            user->bfp = {bfp, group};
            std::cout << "\nBody fat percentage for " << username << " is " << bfp << "%, which is " << group << "." << std::endl;
        }

        // Gets daily calorie intake
        void getDailyCalories(std::string username){
            // Get user information
            UserInfo* user = mymanager.findUser(username);
            if (user == nullptr) { return; }

            // Start with minimum calories
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

            user->calories = calories;
            std::cout << "\nRecommended daily calorie intake for " << username << " is " << calories << " calories." << std::endl;
        }

        // Gets meal preparation details
        void getMealPrep(std::string username){
            // Get user information
            UserInfo* user = mymanager.findUser(username);
            if (user == nullptr) { return; }

            // Constants for macronutrient calorie values
            const int carb_calories = 4;
            const int protein_calories = 4;
            const int fat_calories = 9;

            // Macronutrient breakdown percentages
            const double carb_percent = 0.5;
            const double protein_percent = 0.3;
            const double fat_percent = 0.2;

            if (user->calories == 0) {
                std::cerr << "Error: Please calculate daily calories before meal prep." << std::endl;
                return;
            }

            // Calculate grams for each macronutrient
            user->carbs = (user->calories * carb_percent) / carb_calories;
            user->protein = (user->calories * protein_percent) / protein_calories;
            user->fat = (user->calories * fat_percent) / fat_calories;
        }


        // Wrapper method to display user information
        void display(std::string username){ mymanager.display(username); }; 

        // Wrapper method to serialize user information
        void serialize(std::string filename){ mymanager.writeToFile(filename); }; 

        // Wrapper method to read user information from file
        void readFromFile(std::string filename){ mymanager.readFromFile(filename);}; 

        // Wrapper method to delete user
        void deleteUser(std::string username){ mymanager.deleteUser(username);}; 

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