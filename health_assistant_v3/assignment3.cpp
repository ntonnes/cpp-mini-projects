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

        // Finds a user in the list
        UserInfo& findUser(const std::string& username) {
            // Find user according to username
            auto user = std::find_if(mylist.begin(), mylist.end(), [&username](const UserInfo& user) {
                return user.name == username;
            });
            // Throw an error if user not found
            if (user == mylist.end()) {
                throw std::runtime_error("User with name " + username + " does not exist.");
            }
            // Return user if found
            return *user;
        }

    public:
        // Constructor
        UserInfoManager() : mylist() {}

        // Destructor
        ~UserInfoManager() {}

        std::vector<UserInfo> getMylist() { return mylist; }

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

            // create a vector of pairs to simplify the measurement input validation process
            std::vector<std::pair<std::string, double*>> attributes = {
                {"Enter your body weight in kilograms: ", &newUser.weight},
                {"Enter your waist measurement in centimeters: ", &newUser.waist},
                {"Enter your neck measurement in centimeters: ", &newUser.neck},
                {"Enter your height measurement in centimeters: ", &newUser.height},
                {"Enter your hip measurement in centimeters: ", &newUser.hip}
            };

            // Prompt for and validate body measurements
            std::pair<double, double> measurementRange = { 0, std::numeric_limits<double>::max() };
            for (auto& attribute : attributes) {
                // Skip hip measurement if the user is a male
                if ((attribute.second == &newUser.hip) && (newUser.gender == "male")){
                    continue;
                };
                validateInput(
                    attribute.first, 
                    "Invalid measurement. Measurement must be greater than 0.0.", 
                    measurementRange, *attribute.second
                );
            };  

            // Prompt for and validate lifestyle
            std::vector<std::string> validLifestyles = { "sedentary", "moderate", "active" };
            validateInput(
                "Enter information about your current lifestyle (sedentary, moderate, or active): ", 
                "Invalid lifestyle. Lifestyle must be either sedentary, moderate, or active.", 
                validLifestyles, newUser.lifestyle
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

        // Getter methods
        std::vector<std::string> getAllUsernames() {
            std::vector<std::string> usernames;
            for (const UserInfo& user : mylist) {
                usernames.push_back(user.name);
            }
            return usernames;
        }
        int getAge(const std::string& username) { return findUser(username).age; }
        std::string getGender(const std::string& username) { return findUser(username).gender; }
        double getWeight(const std::string& username) { return findUser(username).weight; }
        double getWaist(const std::string& username) { return findUser(username).waist; }
        double getNeck(const std::string& username) { return findUser(username).neck; }
        double getHeight(const std::string& username) { return findUser(username).height; }
        double getHip(const std::string& username) { return findUser(username).hip; }
        std::pair<int, std::string> getBfp(const std::string& username) { return findUser(username).bfp; }
        double getCalories(const std::string& username) { return findUser(username).calories; }
        std::string getLifestyle(const std::string& username) { return findUser(username).lifestyle; }

        // Setter methods
        void setBfp(const std::string& username, std::pair<int, std::string> bfp) { findUser(username).bfp = bfp; }
        void setCalories(const std::string& username, double calories) { findUser(username).calories = calories; }
        void setCarbs(const std::string& username, double carbs) { findUser(username).carbs = carbs; }
        void setProtein(const std::string& username, double protein) { findUser(username).protein = protein; }
        void setFat(const std::string& username, double fat) { findUser(username).fat = fat; }
        void setLifestyle(const std::string& username, std::string lifestyle) { findUser(username).lifestyle = lifestyle; }


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
                std::cout << "\033[1;33m|\033[0m" << "\033[1;36m  Hips:\033[0m              " << user.hip << " cm\n";
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
    protected:
        static UserInfoManager mymanager;

    public:
        virtual void getBfp(std::string username) = 0;

        // Method to get user details
        void getUserDetail() { mymanager.addUserInfo(); }
        std::vector<std::string> getAllUsernames() { return mymanager.getAllUsernames(); }

        // Method to get the body fat percentage group for a user
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

        // Method to get the recommended daily calorie intake for a user
        void getDailyCalories(std::string username){
            // Get user information from the linked list
            int age = mymanager.getAge(username);
            std::string gender = mymanager.getGender(username);
            std::string lifestyle = mymanager.getLifestyle(username);

            // Set base calorie intake
            int calories = 1600;

            // Add additional calories based on age
            if (age < 51) { 
                calories += (age > 30) ? 200 : 400;
            }

            // Add additional calories for males
            calories += (gender == "male") ? 400 : 0;

            // Set scaling factor for activity level
            int activityBonus = (gender == "male") ? 300 : 200;

            // Add additional calories based on activity level
            if (lifestyle != "sedentary") {
                calories += (lifestyle == "moderate") ? activityBonus : (2*activityBonus);
            }

            // Set the user's daily calorie intake in the linked list and print the result
            mymanager.setCalories(username, calories);
            std::cout << "\nRecommended daily calorie intake for " << username << " is " << calories << " calories." << std::endl;
        }

        // Method to get the recommended macronutrient breakdown for a user
        void getMealPrep(std::string username){
            // Constants for macronutrient calorie values
            const int carb_calories = 4;
            const int protein_calories = 4;
            const int fat_calories = 9;

            // Constants for recommended macronutrient breakdown percentages
            const double carb_percent = 0.5;
            const double protein_percent = 0.3;
            const double fat_percent = 0.2;

            // If the user's daily calorie intake has not been calculated, print an error and return
            int calories = mymanager.getCalories(username);
            if (calories == 0) {
                throw std::runtime_error("A user's daily calorie intake must be calculated before their meal prep.");
            }

            // Calculate grams for each macronutrient and set the user's macronutrient breakdown
            mymanager.setCarbs(username, (calories * carb_percent) / carb_calories);
            mymanager.setProtein(username, (calories * protein_percent) / protein_calories);
            mymanager.setFat(username, (calories * fat_percent) / fat_calories);
        }

        // Method to calculate body fat percentage, daily calorie intake, and macronutrient breakdown for all users in the linked list
        void massLoadAndCompute(std::string filename){
            // Read user information from file
            mymanager.readFromFile(filename);
            // Iterate through the linked list and calculate body fat percentage, daily calorie intake, and macronutrient breakdown for each user
            for (std::string username : getAllUsernames()) {
                getBfp(username);
                getDailyCalories(username);
                getMealPrep(username);
            }
        }

        // Wrapper method to display user information
        void display(std::string username){ mymanager.display(username); }; 

        // Wrapper method to serialize user information
        void serialize(std::string filename){ mymanager.writeToFile(filename); }; 

        // Wrapper method to read user information from file
        void readFromFile(std::string filename){ mymanager.readFromFile(filename);}; 

        // Wrapper method to delete user
        void deleteUser(std::string username){ mymanager.deleteUser(username);}; 
};

class USNavyMethod : public HealthAssistant {
    public:
        void getBfp(std::string username) {
            double bfp;
            std::string gender = mymanager.getGender(username);
            int age = mymanager.getAge(username);
            double waist = mymanager.getWaist(username);
            double neck = mymanager.getNeck(username);
            double hip = mymanager.getHip(username);
            double height = mymanager.getHeight(username);

            if (gender == "male") {
                bfp = 495 / (1.0324 - 0.19077 * log10(waist - neck) + 0.15456 * log10(height)) - 450;
            } else {
                bfp = 495 / (1.29579 - 0.35004 * log10(waist + hip - neck) + 0.22100 * log10(height)) - 450;
            }

            std::string group = getGroup(bfp, age, gender);
            mymanager.setBfp(username, {bfp, group});
            std::cout << "\nBody fat percentage for " << username << " is " << bfp << "%, which is " << group << "." << std::endl;
        }
};

class BmiMethod : public HealthAssistant {
    public:
        void getBfp (std::string username){
            double weight = mymanager.getWeight(username);
            double divisor = std::pow(mymanager.getHeight(username)/ 100, 2);
            double bfp = (weight / divisor);
            std::string group = getGroup(bfp, mymanager.getAge(username), mymanager.getGender(username));
            mymanager.setBfp(username, {bfp, group});
        }
};

class UserStats {
private:
    UserInfoManager statmanager;

public:

    std::vector<std::string> GetHealthyUsers(std::string method, std::string gender = "") {

        std::vector<std::string> healthyUsers;
        HealthAssistant* ha;

        if (method == "USNavy"){
            ha = new USNavyMethod();
        } else if (method == "bmi"){
            ha = new BmiMethod();
        } else if (method == "all") {
            std::vector<std::string> healthyUsersNavy = GetHealthyUsers("USNavy", gender);
            std::vector<std::string> healthyUsersBmi = GetHealthyUsers("bmi", gender);
            healthyUsers.insert(healthyUsers.end(), healthyUsersNavy.begin(), healthyUsersNavy.end());
            healthyUsers.insert(healthyUsers.end(), healthyUsersBmi.begin(), healthyUsersBmi.end());
            return healthyUsers;
        } else {
            // Handle invalid method
            throw std::invalid_argument("Invalid method. Please enter USNavy, bmi, or all.");
        }

        ha->massLoadAndCompute("user_data.csv");
        for (std::string username : ha->getAllUsernames()){
            if (statmanager.getBfp(username).second == "normal") {
                if (gender.empty() || statmanager.getGender(username) == gender) {
                    healthyUsers.push_back(username);
                }
            }
        }
        delete ha;
        return healthyUsers;
    }

    std::vector<std::string> GetUnfitUsers(std::string method, std::string gender = "") {
        std::vector<std::string> unfitUsers;
        HealthAssistant* ha;

        if (method == "USNavy"){
            ha = new USNavyMethod();
        } else if (method == "bmi"){
            ha = new BmiMethod();
        } else if (method == "all") {
            std::vector<std::string> unfitUsersNavy = GetUnfitUsers("USNavy", gender);
            std::vector<std::string> unfitUsersBmi = GetUnfitUsers("bmi", gender);
            unfitUsers.insert(unfitUsers.end(), unfitUsersNavy.begin(), unfitUsersNavy.end());
            unfitUsers.insert(unfitUsers.end(), unfitUsersBmi.begin(), unfitUsersBmi.end());
            return unfitUsers;
        } else {
            // Handle invalid method
            throw std::invalid_argument("Invalid method. Please enter USNavy, bmi, or all.");
        }

        ha->massLoadAndCompute("user_data.csv");
        std::vector<std::string> usernames = ha->getAllUsernames();
        for (std::string username : usernames){
            std::string bfpStatus = mymanager.getBfp(username).second;
            if (bfpStatus == "high" || bfpStatus == "very high") {
                if (gender.empty() || mymanager.getGender(username) == gender) {
                    unfitUsers.push_back(username);
                }
            }
        }
        delete ha;
        return unfitUsers;
    }

    void GetFullStats() {
        mymanager.massLoadAndCompute("user_data.csv");
        std::vector<std::string> usernames = mymanager.getAllUsernames();
        for (std::string username : usernames){
            std::cout << "User: " << username << std::endl;
            mymanager.display(username);
        }
    }
};

UserInfoManager HealthAssistant::mymanager = UserInfoManager();

// Main function
int main() {
    HealthAssistant* ha = new USNavyMethod();
    std::string userInput;
    // Loop until the user inputs "exit"
    while (true) {
        // Get user details
        ha->getUserDetail();
        // Ask for user input
        std::cout << "Enter 'exit' to quit, or press Enter to continue:";
        std::getline(std::cin, userInput);
        // Check if the user wants to exit
        if (userInput == "exit") {
            break; // Exit the loop
        }
    }
    ha->display("all");
    ha->getBfp("john");
    ha->getDailyCalories("john");
    ha->getMealPrep("john");
    ha->serialize("us_user_data->csv");
    delete ha;
    ha = new BmiMethod();
    // Loop until the user inputs "exit"
    while (true) {
        // Get user details
        ha->getUserDetail();
        // Ask for user input
        std::cout << "Enter 'exit' to quit, or press Enter to continue:";
        std::getline(std::cin, userInput);
        // Check if the user wants to exit
        if (userInput == "exit") {
        break; // Exit the loop
        }
    }
    ha->display("all");
    ha->getBfp("john");
    ha->getDailyCalories("john");
    ha->getMealPrep("john");
    ha->deleteUser("jack"); // assuming we have a user jack
    ha->display("all"); // should display all except for jack
    ha->serialize("bmi_user_data->csv");
    delete ha;
    ha = new USNavyMethod();
    ha->massLoadAndCompute("us_user_data.csv");
    ha->display("all");
    delete ha;
    ha = new BmiMethod();
    ha->massLoadAndCompute("bmi_user_data.csv");
    ha->display("all");
    delete ha;
    UserStats stat;
    stat.GetHealthyUsers("bmi", "female");
    stat.GetHealthyUsers("all");
    stat.GetUnfitUsers("USArmy", "male");
    stat.GetFullStats();
}