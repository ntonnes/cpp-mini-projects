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

class HealthAssistant {
public:
    virtual double getBfp(std::string username) = 0;

    void getUserDetail(){ mymanager.addUserInfo(); }

    void getDailyCalories(std::string username){
        UserInfo* user = mymanager.findUser(username);

        int calories = 1600;

        if (user->age < 51) { 
            calories += (user->age > 30) ? 200 : 400;
        }

        calories += (user->gender == "male") ? 400 : 0;

        int activityBonus = (user->gender == "male") ? 300 : 200;
        if (user->lifestyle != "sedentary") {
            calories += (user->lifestyle == "moderate") ? activityBonus : (2*activityBonus);
        }

        user->calories = calories;
        std::cout << "\nRecommended daily calorie intake for " << username << " is " << calories << " calories." << std::endl;
    }

};

class USNavyMethod : public HealthAssistant {
public:
    std::string getGroup(double bfp, int age, std::string gender) {
        std::vector<std::pair<int, std::string>> ranges;
        if (gender == "female") {
            if (age >= 20 && age <= 39) ranges = {{21, "low"}, {33, "normal"}, {39, "high"}};
            else if (age >= 40 && age <= 59) ranges = {{23, "low"}, {34, "normal"}, {40, "high"}};
            else if (age >= 60 && age <= 79) ranges = {{24, "low"}, {36, "normal"}, {42, "high"}};
        } else if (gender == "male") {
            if (age >= 20 && age <= 39) ranges = {{8, "low"}, {20, "normal"}, {25, "high"}};
            else if (age >= 40 && age <= 59) ranges = {{11, "low"}, {22, "normal"}, {28, "high"}};
            else if (age >= 60) ranges = {{13, "low"}, {25, "normal"}, {30, "high"}};
        }

        for (auto& range : ranges) {
            if (bfp < range.first) return range.second;
        }
        return "very high";
    }

    void getBfp(std::string username) {
        UserInfo* user = mylist.findUser(username);
        if (user == nullptr) { return; }

        double bfp;
        if (user->gender == "male") {
            bfp = 495 / (1.0324 - 0.19077 * log10(user->waist - user->neck) + 0.15456 * log10(user->height)) - 450;
        } else if (user->gender == "female") {
            bfp = 495 / (1.29579 - 0.35004 * log10(user->waist + user->hip - user->neck) + 0.22100 * log10(user->height)) - 450;
        }

        std::string group = getGroup(bfp, user->age, user->gender);
        user->bfp = {bfp, group};
        std::cout << "\nBody fat percentage for " << username << " is " << bfp << "%, which is " << group << "." << std::endl;
    }
};

class BmiMethod : public HealthAssistant {
public:
    double getBfp(std::string username) override {
        UserInfo user = mymanager.findUser(username);
        if (user == nullptr) { return; }

        user.height /= 100.0;
        return user.weight / (user.height * user.height);
    }
};


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

    public:
        // Constructor
        UserInfoManager() {}

        // Destructor
        ~UserInfoManager() {}

        // Adds info to list
        void addUserInfo() {
            UserInfo newUser;

            // Prompt for and validate name
            std::string input;
            std::cout << "\nPlease enter your name: ";
            while (true) {
                std::cin >> input;
                bool valid = true;
                for (char c : input) { if (!std::isalpha(c)) { valid = false; break; } }

                if (valid) { 
                    newUser.gender = input; break;
                } else { std::cout << "Invalid name. Please enter a name containing only letters: "; }
            }
            newUser.name = input;

            // Prompt for and validate gender
            validateString(
                "Please specify your gender as either male or female: ", 
                "Invalid gender. Please enter male or female: ", 
                { "female", "male" }, newUser.gender
            );

            // Prompt for and validate age
            validateInteger(
                "Enter your age: ", 
                "Invalid age. Please enter an age between 19 and 80: ", 
                18, 100, newUser.age
            );

            // Prompt for and validate double-based measurements
            std::vector<std::pair<std::string, double*>> attributes = {
                {"Enter your body weight in kilograms: ", &newUser.weight},
                {"Enter your waist measurement in centimeters: ", &newUser.waist},
                {"Enter your neck measurement in centimeters: ", &newUser.neck},
                {"Enter your height measurement in centimeters: ", &newUser.height},
                {"Enter your hip measurement in centimeters: ", &newUser.hip}
            };
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

            std::cout << "User " << newUser.name << " has been added.\n" << std::endl;
            mylist.push_back(newUser);
        }

        double get

        // Finds a user
        UserInfo* findUser(const std::string& username) {
            auto it = std::find_if(mylist.begin(), mylist.end(), [&username](const UserInfo& user) {
                return user.name == username;
            });

            if (it != mylist.end()) {
                return &(*it);
            }

            std::cerr << "Error: User with name " << username << " does not exist." << std::endl;
            return nullptr;
        }

        // Removes a user
        void deleteUser(std::string username) {
            auto it = std::find_if(mylist.begin(), mylist.end(), [&username](const UserInfo& user) {
                return user.name == username;
            });

            if (it != mylist.end()) {
                mylist.erase(it);
                std::cout << "User " << username << " has been deleted." << std::endl;
            } else {
                std::cout << "User " << username << " does not exist." << std::endl;
            }
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
                UserInfo* user = findUser(username);
                if (user == nullptr) { return; }
                displayDetails(*user);
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
    public:
        void getUserDetail(){ mymanager.addUserInfo(); }

        void getDailyCalories(std::string username){
            UserInfo* user = mymanager.findUser(username);
            if (user == nullptr) { return; }

            int calories = 1600;

            if (user->age < 51) { 
                calories += (user->age > 30) ? 200 : 400;
            }

            calories += (user->gender == "male") ? 400 : 0;

            int activityBonus = (user->gender == "male") ? 300 : 200;
            if (user->lifestyle != "sedentary") {
                calories += (user->lifestyle == "moderate") ? activityBonus : (2*activityBonus);
            }

            user->calories = calories;
            std::cout << "\nRecommended daily calorie intake for " << username << " is " << calories << " calories." << std::endl;
        }

        void getMealPrep(std::string username){
            UserInfo* user = mymanager.findUser(username);
            if (user == nullptr) { return; }

            const int carb_calories = 4;
            const int protein_calories = 4;
            const int fat_calories = 9;

            const double carb_percent = 0.5;
            const double protein_percent = 0.3;
            const double fat_percent = 0.2;

            if (user->calories == 0) {
                std::cerr << "Error: Please calculate daily calories before meal prep." << std::endl;
                return;
            }

            user->carbs = (user->calories * carb_percent) / carb_calories;
            user->protein = (user->calories * protein_percent) / protein_calories;
            user->fat = (user->calories * fat_percent) / fat_calories;
        }

        void display(std::string username){ mymanager.display(username); }; 

        void serialize(std::string filename){ mymanager.writeToFile(filename); }; 

        void readFromFile(std::string filename){ mymanager.readFromFile(filename);}; 

        void deleteUser(std::string username){ mymanager.deleteUser(username);}; 

    private:
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