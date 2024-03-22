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
#include <functional>


class UserInfoManager
{
    private:

        /** Privately held struct to store user information
         * Only the UserInfoManager can directly access user data or create/destroy UserInfo objects
        **/ 
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
        
        /** A vector of UserInfo objects to store user information
         * Each UserInfo object represents a user and their input information
         * 'mylist' is a private member since only the UserInfoManager can understand the UserInfo type
         * 'mylist' is specific to the UserInfoManager instance
        */
        std::vector<UserInfo> mylist;
        
        /** A template representing inputs for the validateInput function
         * 'Typ' is the type of the attribute to update (string, int, or double)
         * 'Constraint' is the type of the constraints (vector of string, pair of int, or pair of double)
         **/ 
        template <typename Typ, typename Constraint>

        /** Validates a string, integer, or double input based on the given constraints, and updates the given attribute
         * Private member since the UserInfoManager is responsible for collecting user inputs and updating user data
         * Throws a runtime error if the input is invalid or outside the given limits
         **/ 
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

        /** Finds the UserInfo with the given name in the private UserInfo vector 'mylist'
         * The UserInfo vector searched is specific to the UserInfoManager instance
         * Private member since only a UserInfoManager understands the UserInfo type (returns UserInfo&)
         * Throws a runtime error if the user is not found
         **/ 
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
        /** Constructor
         * Initializes the empty vector of UserInfo objects 'mylist'
         * The vector is specific to the UserInfoManager instance
         **/
        UserInfoManager() : mylist() {}

        /** Destructor
         * Clears the vector of UserInfo objects 'mylist'
         * The vector is specific to the UserInfoManager instance
         **/
        ~UserInfoManager() { mylist.clear(); }

        /** Adds a new user to the 'mylist' vector
         * Prompts the user for input and validates the input
         * Throws a runtime error if the input is invalid or outside the given limits
         **/
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

            // Print success message and add user to 'mylist'
            std::cout << "User " << newUser.name << " has been added successfully.\n" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
            mylist.push_back(newUser);
        }

        /** Deletes a user from the 'mylist' vector
         * Removes the first user found with the given username in 'mylist'
         * Throws a runtime error if the user is not found
         **/
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
            std::cout << "User " << username << " has been deleted successfully\n." << std::endl;
        }

        /** Filters usernames based on body fat percentage
         * Returns a vector of strings containing all usernames that fall into the given bfp groups
         **/
        std::vector<std::string> filterUsernames(const std::vector<std::string>& bfpGroups, const std::string& gender) {
            std::vector<std::string> validUsernames;
            // Iterate through all loaded users
            for (const UserInfo &user : mylist) {
                // Continue if the user's bfp group is in the list of valid groups
                if (std::find(bfpGroups.begin(), bfpGroups.end(), user.bfp.second) != bfpGroups.end()) {
                    if (gender == "") {
                        // If the gender isn't specified, add the username to the list
                        validUsernames.push_back(user.name);
                    } else if (user.gender == gender) {
                        // Else add the username to the list if the user's gender matches the input
                        validUsernames.push_back(user.name);
                    }
                }
            }
            return validUsernames;
        }

        /** Gets all usernames in this UserInfoManager instance's 'mylist' vector
         * Returns a vector of strings containing all usernames
         * Public member since other classes need to iterate through all users
         **/


        std::vector<std::string> getBfpUsers(std::vector<std::string> bfpGroups, std::string gender="") {
            std::vector<std::string> bfpUsers;
            if (gender!="male"||gender!="female"||gender!="") {
                throw std::invalid_argument("Gender must be either 'male' or 'female', or left blank.");
            }
            for (const UserInfo &user : mylist) {
                if (user.bfp.second == "none") {
                    throw std::runtime_error("Body fat percentage has not been calculated for all users.");
                } else if (std::find(bfpGroups.begin(), bfpGroups.end(), user.bfp.second) != bfpGroups.end() && (user.gender == gender || gender == "")) {
                    bfpUsers.push_back(user.name);
                }
            }
            return bfpUsers;
        }

        std::vector<std::string> healthyUsers(std::string gender="") {
            return getBfpUsers({"normal"}, gender);
        }

        std::vector<std::string> unhealthyUsers(std::string gender="") {
            return getBfpUsers({"high", "very high"}, gender);
        }

        std::vector<std::string> allUsers(std::string gender="") {
            return getBfpUsers({"low", "normal", "high", "very high"}, gender);
        }

        /** Getter methods to access user information
         * Public member since other classes need to access user information
         * Necessary since the 'mylist' vector and UserInfo struct are private to UserInfoManager
         **/
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

        /** Setter methods to access user information
         * Public member since other classes need to update user information
         * Necessary since the 'mylist' vector and UserInfo struct are private to UserInfoManager
         **/
        void setBfp(const std::string& username, std::pair<int, std::string> bfp) { findUser(username).bfp = bfp; }
        void setCalories(const std::string& username, double calories) { findUser(username).calories = calories; }
        void setCarbs(const std::string& username, double carbs) { findUser(username).carbs = carbs; }
        void setProtein(const std::string& username, double protein) { findUser(username).protein = protein; }
        void setFat(const std::string& username, double fat) { findUser(username).fat = fat; }
        void setLifestyle(const std::string& username, std::string lifestyle) { findUser(username).lifestyle = lifestyle; }


        /** Reads user information from a .csv file and populates the 'mylist' vector
         * Throws a runtime error if the file cannot be opened or is not a .csv file
         **/
        void readFromFile(std::string filename) {

            // Check if the file extension is .csv
            std::string extension = ".csv";
            if (filename.size() <= extension.size() || filename.substr(filename.size() - extension.size()) != extension) {
                throw std::runtime_error("File " + filename + " is not a .csv file. The Health Assistant can only read .csv files.");
            }

            // Attempt to open the file
            std::ifstream file(filename);
            if (!file) {
                throw std::runtime_error("Could not open file " + filename);
            }

            // Skip the header line
            std::string line;
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
            // Print success message
            std::cout << "\nData has been read from file " << filename << std::endl;
        }

        /** Overwrites the .csv file provided with the user information in the 'mylist' vector
         *  Throws a runtime error if the file cannot be opened or is not a .csv file
         **/
        void writeToFile(std::string filename) {
            // Check if the file extension is .csv
            std::string extension = ".csv";
            if (filename.size() <= extension.size() || filename.substr(filename.size() - extension.size()) != extension) {
                throw std::runtime_error("File " + filename + " is not a .csv file. The Health Assistant can only write to .csv files.");
            }

            // Attempt to open the file
            std::ofstream file(filename);
            if (!file) {
                throw std::runtime_error("Could not open file " + filename);
            }

            // Write the header line
            file << "name,gender,age,weight,waist,neck,height,hip,bfp,group,calories,carbs,protein,fat,lifestyle\n";

            // Write each user's information to the file
            for (const auto& user : mylist) {
                file << user.name << "," << user.gender << "," << user.age << "," << user.weight << "," 
                     << user.waist << "," << user.neck << "," << user.height << "," << user.hip << "," 
                     << user.bfp.first << "," << user.bfp.second << "," << user.calories << ","
                     << user.carbs << "," << user.protein << "," << user.fat << ","  << user.lifestyle << "\n";
            }

            // Print success message
            std::cout << "\nData has been written to file " << filename << std::endl;
        }

        /** Displays user information for all users or a specific user
         * Throws a runtime error if the user is not found
         **/
        void display(std::string username) {
            if (username == "all") {
                std::cout << "\nDisplaying information for all users...\n";
                for (const auto& user : mylist) {
                    displayDetails(user);
                }
            } else {
                std::cout << "\nDisplaying information for user " << username << "...\n";
                displayDetails(findUser(username));
            }
            std::cout << "\nDone.\n";
        }

        /** Displays user information for a specific user
         **/
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
            // Display hip measurement if the user is female
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
            std::cout << "\033[1;33m=========================================\033[0m\n";
        }
};


class HealthAssistant {
    protected:

        /** A static instance of the UserInfoManager 'mymanager' to manage user information
         * All HealthAssistant instances share the same UserInfoManager instance
         * This means that all HealthAssistant instances share the same UserInfo vector
         * Protected since derived classes also use the same UserInfo list
         **/
        static UserInfoManager mymanager;

        /** Method to get the body fat percentage group based on the user's age and gender
         *  Returns a string representing the group the user falls into
         *  Protected member since derived classes use the same method to find the group
         *  Does not need to be accessed from outside the class
         **/
        std::string getBfpGroup(double bfp, int age, std::string gender) {
            std::vector<std::pair<int, std::string>> ranges;
            // Set the thresholds for bfp group based on age
            if (gender == "female") {
                if (age >= 20 && age <= 39) ranges = {{21, "low"}, {33, "normal"}, {39, "high"}};
                else if (age >= 40 && age <= 59) ranges = {{23, "low"}, {34, "normal"}, {40, "high"}};
                else if (age >= 60 && age <= 79) ranges = {{24, "low"}, {36, "normal"}, {42, "high"}};
            } else if (gender == "male") {
                if (age >= 20 && age <= 39) ranges = {{8, "low"}, {20, "normal"}, {25, "high"}};
                else if (age >= 40 && age <= 59) ranges = {{11, "low"}, {22, "normal"}, {28, "high"}};
                else if (age >= 60) ranges = {{13, "low"}, {25, "normal"}, {30, "high"}};
            }

            // Return the group based on the bfp and the thresholds
            for (auto& range : ranges) {
                if (bfp < range.first) return range.second;
            }
            return "very high";
        }

    public:

        /** Constructor
         * Protected to prevent instantiation of the HealthAssistant class directly
         * The HealthAssistant class on its own has no way to calculate bfp
         **/
        HealthAssistant() {}

        /** Destructor
         **/
        ~HealthAssistant() {}

        /** Virtual method to calculate body fat percentage
         * Derived classes must implement this method to calculate body fat percentage
         * The BFP calculation method is specific to the derived class
         **/
        virtual void getBfp(std::string username) = 0;

        /** Calculates and updates the recommended daily calorie intake for a user based on age and lifestyle
         **/
        void getDailyCalories(std::string username){
            // Get user information from the static UserInfo vector
            int age = mymanager.getAge(username);
            std::string gender = mymanager.getGender(username);
            std::string lifestyle = mymanager.getLifestyle(username);

            // Set base calorie intake then add additional calories based on age and gender
            int calories = 1600;
            if (age < 51) { 
                calories += (age > 30) ? 200 : 400;
            }
            calories += (gender == "male") ? 400 : 0;

            // Set a scaling factor based on activity level
            int activityBonus = (gender == "male") ? 300 : 200;

            // Add additional calories based on activity level
            if (lifestyle != "sedentary") {
                calories += (lifestyle == "moderate") ? activityBonus : (2*activityBonus);
            }

            // Update the UserInfo object with the calculated calorie intake
            mymanager.setCalories(username, calories);
            std::cout << "\nRecommended daily calorie intake for " << username << " is " << calories << " calories." << std::endl;
        }

        /** Calculates and updates the macronutrient breakdown for a user based on their daily calorie intake
         **/
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
                throw std::runtime_error("A user's daily calorie intake must be calculated before their macronutrient breakdown.");
            }

            // Calculate grams for each macronutrient and set the user's macronutrient breakdown
            mymanager.setCarbs(username, (calories * carb_percent) / carb_calories);
            mymanager.setProtein(username, (calories * protein_percent) / protein_calories);
            mymanager.setFat(username, (calories * fat_percent) / fat_calories);
        }

        /** Overwrites the static UserInfo vector 'mylist' with user information from a .csv file, then updates all users' calculated information
         * Calculates body fat percentage, daily calorie intake, and macronutrient breakdown for each user
         **/
        void massLoadAndCompute(std::string filename){
            // Read user information from the file to populate the static UserInfo vector
            mymanager.readFromFile(filename);
            // Iterate UserInfo vector and update each user's body fat percentage, daily calorie intake, and macronutrient breakdown
            for (std::string username : mymanager.allUsers()) {
                getBfp(username);
                getDailyCalories(username);
                getMealPrep(username);
            }
        }

        /** Wrappers for the public UserInfoManager methods
         **/
        void getUserDetail() { mymanager.addUserInfo(); }
        void display(std::string username){ mymanager.display(username); }; 
        void serialize(std::string filename){ mymanager.writeToFile(filename); }; 
        void readFromFile(std::string filename){ mymanager.readFromFile(filename);}; 
        void deleteUser(std::string username){ mymanager.deleteUser(username);}; 
        std::vector<std::string> healthyUsers(std::string gender){ return mymanager.healthyUsers(gender); };
        std::vector<std::string> unhealthyUsers(std::string gender){ return mymanager.unhealthyUsers(gender); };
        std::vector<std::string> allUsers(std::string gender){ return mymanager.allUsers(gender); };
};

class USNavyMethod : public HealthAssistant {

    public:

        /** Calculates and updates the body fat percentage of a user using the US Navy method
         * Uses gender, age, waist, neck, hip, and height measurements to calculate body fat percentage
         **/
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

            std::string group = getBfpGroup(bfp, age, gender);
            mymanager.setBfp(username, {bfp, group});
            std::cout << "\nBody fat percentage for " << username << " is " << bfp << "%, which is " << group << "." << std::endl;
        }
};

class BmiMethod : public HealthAssistant {
    
    public:

        /** Calculates and updates the body fat percentage of a user using the BMI method
         * Uses weight and height measurements to calculate body fat percentage
        **/
        void getBfp (std::string username) {
            // Get user information from the static UserInfo vector
            int age = mymanager.getAge(username);
            std::string gender = mymanager.getGender(username);
            double weight = mymanager.getWeight(username);
            double height = mymanager.getHeight(username);

            // Calculate body fat percentage using the BMI method
            double bfp = (weight / ((height * height) / 100));
            std::string group = getBfpGroup(bfp, mymanager.getAge(username), mymanager.getGender(username));
            mymanager.setBfp(username, {bfp, group});
        }
};

class UserStats {

    private:

        struct Stats{
            int totalUsers;
            int totalMale;
            int totalFemale;
            int healthyUsNavy;
            int healthyUsNavyMale;
            int healthyUsNavyFemale;
            int healthyBmi;
            int healthyBmiMale;
            int healthyBmiFemale;
        };

    public:

        /** Constructor
         **/
        UserStats() {}

        /** Destructor
         **/
        ~UserStats() {}

        /** Gets a vector containing the usernames of all users with a "normal" body fat percentage
         * Uses the US Navy, BMI method, or both to calculate body fat percentage
         * If using both, returns users with a "normal" body fat percentage from both methods
         **/
        std::vector<std::string> GetHealthyUsers(std::string method, std::string gender="") {

            // Vector to store usernames of healthy users
            std::vector<std::string> healthyUsers;

            // Create a HealthAssistant object based on the method
            HealthAssistant* ha;

            if (method == "USNavy"){
                ha = new USNavyMethod();
                ha->massLoadAndCompute("us_user_data.csv");

            } else if (method == "bmi"){
                ha = new BmiMethod();
                ha->massLoadAndCompute("bmi_user_data.csv");

            } else if (method == "all") {
                // Get healthy users from both methods
                std::vector<std::string> healthyUsersNavy = GetHealthyUsers("USNavy", gender);
                std::vector<std::string> healthyUsersBmi = GetHealthyUsers("bmi", gender);

                // Sort the vectors by username
                std::sort(healthyUsersNavy.begin(), healthyUsersNavy.end());
                std::sort(healthyUsersBmi.begin(), healthyUsersBmi.end());

                // Use std::set_intersection to find common elements
                std::set_intersection(healthyUsersNavy.begin(), healthyUsersNavy.end(),
                                      healthyUsersBmi.begin(), healthyUsersBmi.end(),
                                      std::back_inserter(healthyUsers));

                // Return the vector of common elements
                return healthyUsers;

            } else {
                // Handle invalid method
                throw std::invalid_argument("Invalid bfp method. Must be either 'USNavy', 'bmi', or 'all'.");
            }


            // Get the users who are of the specified gender and have a "normal" body fat percentage
            healthyUsers = ha->healthyUsers(gender);
            delete ha;
            return healthyUsers;
        }

        std::vector<std::string> GetUnfitUsers(std::string method, std::string gender = "") {
            // Vector to store usernames of healthy users
            std::vector<std::string> healthyUsers;

            // Create a HealthAssistant object based on the method
            HealthAssistant* ha;

            if (method == "USNavy"){
                ha = new USNavyMethod();
                ha->massLoadAndCompute("us_user_data.csv");

            } else if (method == "bmi"){
                ha = new BmiMethod();
                ha->massLoadAndCompute("bmi_user_data.csv");

            } else if (method == "all") {
                // Get healthy users from both methods
                std::vector<std::string> healthyUsersNavy = GetHealthyUsers("USNavy", gender);
                std::vector<std::string> healthyUsersBmi = GetHealthyUsers("bmi", gender);

                // Sort the vectors by username
                std::sort(healthyUsersNavy.begin(), healthyUsersNavy.end());
                std::sort(healthyUsersBmi.begin(), healthyUsersBmi.end());

                // Use std::set_intersection to find common elements
                std::set_intersection(healthyUsersNavy.begin(), healthyUsersNavy.end(),
                                      healthyUsersBmi.begin(), healthyUsersBmi.end(),
                                      std::back_inserter(healthyUsers));

                // Return the vector of common elements
                return healthyUsers;

            } else {
                // Handle invalid method
                throw std::invalid_argument("Invalid bfp method. Must be either 'USNavy', 'bmi', or 'all'.");
            }

            // Get the users who are of the specified gender and have a "normal" body fat percentage
            healthyUsers = ha->unhealthyUsers(gender);
            delete ha;
            return healthyUsers;
        }

        void GetFullStats() {
            Stats stat;

            // Initialize a USNavyMethod HeathAssistant object
            HealthAssistant* ha = new USNavyMethod();
            
            stat.healthyUsNavyMale = GetHealthyUsers("UsNavy", "male").size();
            stat.healthyUsNavyFemale = GetHealthyUsers("UsNavy", "female").size();
            stat.healthyUsNavy = stat.healthyUsNavyMale + stat.healthyUsNavyFemale;

            // Initialize a BmiMethod HealthAssistant object
            ha = new BmiMethod();

            stat.healthyBmiMale = GetHealthyUsers("bmi", "male").size();
            stat.healthyBmiFemale = GetHealthyUsers("bmi", "female").size();
            stat.healthyBmi = stat.healthyBmiMale + stat.healthyBmiFemale;

            // Count the number of total users
            stat.totalMale =ha->allUsers("male").size();
            stat.totalFemale = ha->allUsers("female").size();
            stat.totalUsers = stat.totalMale + stat.totalFemale;

            // Display the statistics
            displayStats(stat);
        }

        void displayStats(Stats stat) {

            std::cout << "Total number of users: " 
                      << stat.totalUsers << std::endl;
            std::cout << "Percentage of male users: " 
                      << ((stat.totalMale/stat.totalUsers)*100) << "%" << std::endl;
            std::cout << "Percentage of female users: " 
                      << ((stat.totalFemale/stat.totalUsers)*100) 
                      << "%" << std::endl;
            std::cout << "Percentage of healthy users (BMI method): " 
                      << stat.healthyBmi << "%" << std::endl;
            std::cout << "Percentage of healthy males (BMI method): " 
                      << ((stat.healthyBmiMale/stat.healthyBmi)*100) << "%" << std::endl;
            std::cout << "Percentage of healthy females (BMI method): " 
                      << ((stat.healthyBmiFemale/stat.healthyBmi)*100) << "%" << std::endl;
            std::cout << "Percentage of healthy users (US Navy method): " 
                      << stat.healthyUsNavy << "%" << std::endl;
            std::cout << "Percentage of healthy males (US Army method): " 
                      << ((stat.healthyUsNavyMale/stat.healthyUsNavy)*100) << "%" << std::endl;
            std::cout << "Percentage of healthy females (US Army method): "
                      << ((stat.healthyUsNavyFemale/stat.healthyUsNavy)*100) << "%" << std::endl;
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
    ha->serialize("us_user_data.csv");
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
    ha->serialize("bmi_user_data.csv");
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
    stat.GetUnfitUsers("USNavy", "male");
    stat.GetFullStats();
}