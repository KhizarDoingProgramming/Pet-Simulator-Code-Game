#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;
class Pet {
protected:
    char name[50];
    int hunger, happiness, energy, age, level, health;
public:
    virtual ~Pet() {}
    Pet(string petname): hunger(70), happiness(70), energy(70), age(0), level(1), health(100) {
        strncpy(name, petname.c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
    void feed();
    int clamp(int value, int minval, int maxval);
    virtual void play();
    virtual void rest();
    virtual void ageup();
    virtual void evolve();
    virtual void petstatus();
    void update_hp();
    virtual string getClassType() const;
    void give_meds();
    string getmood();
    void savepet(const char* filename);
    static Pet* loadpet(const char* filename);
    void setAttributes(int h, int ha, int e, int a, int l, int hp);
    string getname() const;
    int gethunger() const;
    int getenergy() const;
    int gethappiness() const;
    int getlevel() const;
    int getage() const;
};


class Firepet : public Pet {
public:
    Firepet(string Petname) : Pet(Petname) {}
    void play() override;
    void evolve() override;
    string getClassType() const override;
};

void Pet::feed() {
    string mood = getmood();
    if (mood == "🤒 Sick" && rand() % 3 == 0) {
        cout << name << " is sick and refuses to eat! 🤢" << endl;
        return;
    }
    hunger = clamp(hunger - 20, 0, 100);
    happiness = clamp(happiness + 5, 0, 100);
    cout << name << " has been fed! Hunger now: " << hunger << endl;
    update_hp();
}

int Pet::clamp(int value, int minval, int maxval) {
    if (value < minval) return minval;
    if (value > maxval) return maxval;
    return value;
}

void Pet::play() {
    string mood = getmood();
    if (energy < 20) {
        cout << name << " is too tired to Play!!" << endl;
        return;
    }
    if (mood == "🤒 Sick") {
        cout << name << " is sick and refuses to play. 🤒" << endl;
        return;
    }
    int happiness_bost = 15;
    if (mood == "😢 Sad") {
        happiness_bost = 8;
        cout << name << " is sad and doesn't enjoy playing much... 😢" << endl;
    }
    else if (mood == "😄 Happy") {
        happiness_bost = 25;
        cout << name << " had a blast playing! 🎉" << endl;
    }
    else {
        cout << name << " played a little. 🐾" << endl;
    }

    happiness = clamp(happiness + happiness_bost, 0, 100);
    energy = clamp(energy - 20, 0, 100);
    hunger = clamp(hunger + 10, 0, 100);

    update_hp();
}

void Firepet::play() {
    if (energy < 20) {
        cout << name << " is too tired to Play with Fire!!" << endl;
        return;
    }
    happiness = clamp(happiness + happiness, 0, 100);
    energy = clamp(energy - 20, 0, 100);
    hunger = clamp(hunger + 10, 0, 100);
    cout << name << " played happily with Fire!! Happiness: " << happiness << ", Energy: " << energy << endl;
}

void Pet::rest() {
    string mood = getmood();
    int energy_bost = 30;
    if (mood == "😢 Sad")
        energy_bost = 20;
    else if (mood == "😄 Happy")
        energy_bost = 40;

    energy = clamp(energy + energy_bost, 0, 100);
    hunger = clamp(hunger + 10, 0, 100);
    cout << name << " took a rest. Energy: " << energy << ", Hunger: " << hunger << endl;
    update_hp();
}

void Pet::ageup() {
    age++;
    if (age % 3 == 0) {
        level++;
        cout << name << " Leveled Up to Level: " << level << "!" << endl;
        evolve();
    }
}

void Pet::evolve() {
    if (level >= 3) {
        cout << name << " is Starting to Evolve..." << endl;
    }
}

void Firepet::evolve() {
    if (level >= 3) {
        cout << name << " Evolved into a Fire Dragon!" << endl;
    }
}

void Pet::petstatus() {
    cout << "\n======== PET STATUS ========\n";
    cout << "Name     : " << name << endl;
    cout << "Age      : " << age << " 🕒" << endl;
    cout << "Level    : " << level << " 🌟" << endl;
    cout << "Health   : " << health << " ❤️" << endl;
    cout << "Hunger   : " << hunger << " 🍗" << endl;
    cout << "Happiness: " << happiness << " 😊" << endl;
    cout << "Energy   : " << energy << " ⚡" << endl;
    cout << "Mood     : " << getmood() << endl;
    cout << "============================\n";
}

void Pet::update_hp() {
    if (hunger > 80 || happiness < 30 || energy < 20) {
        health = clamp(health - 10, 0, 100);
        cout << name << " isn't doing well... Health is now " << health << " 💔" << endl;
    }
    else if (hunger < 40 && happiness > 70 && energy > 60) {
        health = clamp(health + 5, 0, 100);
        cout << name << " is thriving! Health increased to " << health << " ❤️" << endl;
    }
    if (health == 0) {
        cout << "\n💀 " << name << " has passed away due to poor health.\n";
        exit(0);
    }
}

string Pet::getClassType() const {
    return "Pet";
}

string Firepet::getClassType() const {
    return "Firepet";
}

void Pet::give_meds() {
    if (health >= 100) {
        cout << name << " is already fully healthy. 🧬" << endl;
        return;
    }
    cout << name << " takes the medicine... ❤️‍🩹" << endl;
    health = clamp(health + 10, 0, 100);
    cout << name << "'s health is Now: " << health << endl;
}

string Pet::getmood() {
    if (health <= 30)
        return "🤒 Sick";
    if (hunger >= 80)
        return "🍗 Hungry";
    if (energy <= 20)
        return "😴 Tired";
    if (happiness <= 30)
        return "😢 Sad";
    if (happiness >= 80 && energy >= 60 && hunger < 40)
        return "😄 Happy";
    return "😐 Neutral";
}

void Pet::savepet(const char* filename) {
    ofstream file(filename);
    if (!file) {
        cout << "❌ Error saving file\n";
        return;
    }

    file << getClassType() << endl;
    file << name << endl;
    file << hunger << " " << happiness << " " << energy << " "
        << age << " " << level << " " << health << endl;

    file.close();
    cout << "✅ Pet saved successfully.\n";
}


Pet* Pet::loadpet(const char* filename) {
    ifstream file(filename);
    if (!file) {
        cout << "❌ Error loading file\n";
        return nullptr;
    }

    string type, pname;
    int h, ha, e, a, l, hp;

    getline(file, type);
    getline(file, pname);
    file >> h >> ha >> e >> a >> l >> hp;

    Pet* pet;
    if (type == "Firepet")
        pet = new Firepet(pname);
    else
        pet = new Pet(pname);

    pet->setAttributes(h, ha, e, a, l, hp);
    file.close();
    cout << "✅ Pet loaded successfully.\n";
    return pet;
}

void Pet::setAttributes(int h, int ha, int e, int a, int l, int hp) {
    hunger = h;
    happiness = ha;
    energy = e;
    age = a;
    level = l;
    health = hp;
}

string Pet::getname() const { return name; }
int Pet::gethunger() const { return hunger; }
int Pet::getenergy() const { return energy; }
int Pet::gethappiness() const { return happiness; }
int Pet::getlevel() const { return level; }
int Pet::getage() const { return age; }

int main() {
    system("chcp 65001");
    Pet* pet = nullptr;
    string petName;
    int ch;
    cout << "1. To Create a New Pet." << endl;
    cout << "2. To Load Pet. " << endl;
    cout << "Enter your Choice: ";
    cin >> ch;
    switch (ch)
    {
    case 1:
        cout << "🐾 Choose your pet type:\n";
        cout << "1. Regular Pet 🐶\n";
        cout << "2. Fire Pet 🔥\n";
        cout << "Enter choice: ";
        cin >> ch; cin.ignore();
        cout << "🐾 Name your pet: ";
        getline(cin, petName);
        switch (ch)
        {
        case 1:
            pet = new Pet(petName);
            break;
        case 2:
            pet = new Firepet(petName);
            break;
        default:
            cout << "Invalid choice. Exiting...\n";
            return 1;
        }
        break;
    case 2:
        pet = Pet::loadpet("savepet.bin");
        if (!pet) {
            cout << "Failed to load pet. Exiting...\n";
            return 1;
        }
        break;
    default:
        cout << "Invalid choice. Exiting...\n";
        return 1;
    }

    int choice;
    int actionCount = 0;

    do {
        cout << "\n========= MENU =========" << endl;
        cout << "1. Feed 🍗" << endl;
        cout << "2. Play 🎾" << endl;
        cout << "3. Rest 💤" << endl;
        cout << "4. Give Medicine 💊" << endl;
        cout << "5. Show Status 📋" << endl;
        cout << "6. Save Game 💾" << endl;
        cout << "7. Load Game 📂" << endl;
        cout << "8. Exit 🚪" << endl;
        cout << "Choose an action: ";
        cin >> choice;

        switch (choice) {
        case 1: pet->feed(); break;
        case 2: pet->play(); break;
        case 3: pet->rest(); break;
        case 4: pet->give_meds(); break;
        case 5: pet->petstatus(); break;
        case 6:
            pet->savepet("savepet.bin"); break;
        case 7:
            delete pet;
            pet = Pet::loadpet("savepet.bin");
            if (!pet) {
                cout << "Failed to load pet. Exiting...\n";
                return 1;
            }
            break;
        case 8:
            cout << "\nThanks for taking care of " << pet->getname() << "! 🐶 See you next time!\n";
            break;
        default:
            cout << "Invalid choice. Try again!" << endl;
        }

        actionCount++;
        if (actionCount % 5 == 0) {
            pet->ageup();
        }

    } while (choice != 8);
    delete pet;
    return 0;
}