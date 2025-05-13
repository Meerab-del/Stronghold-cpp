#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>
using namespace std;

// --- ENUM FOR SOCIAL CLASSES ---
enum SocialClass { PEASANT, MERCHANT, NOBLE };

// --- ABSTRACT BASE CLASS ---
class KingdomComponent
{
public:
    virtual void displayInfo() const = 0;
    virtual ~KingdomComponent() {}
};

// --- POPULATION CLASS ---
class Population
{
public:
    int total;
    int peasants, merchants, nobles;

    Population(int initial = 100);
    int get() const;
    void set(int value);
    void add(int value);
    void subtract(int value);
    void displayComposition() const;
};

// --- RESOURCES CLASS ---
class Resources
{
public:
    int gold, food, wood, stone, iron;
    Population* population;

    Resources();
    ~Resources();
    int getGold() const;
    int getFood() const;
    int getWood() const;
    int getStone() const;
    int getIron() const;
    Population* getPopulation();

    template <typename T>
    void modifyResource(T& resource, T amount)
    {
        resource += amount;
    }
};

// --- ARMY CLASS ---
class Army : public KingdomComponent
{
private:
    int troops;
    int weapons;
    int barracks;

public:
    Army();
    void recruitTroops(int num);
    void loseTroops(int num);
    int getTroops() const;
    int getWeapons() const;
    int getBarracks() const;
    void buildBarracks(int& gold);
    void buildBlacksmith(int& gold);
    void paySoldiers(int& gold);
    void displayInfo() const override;
    void save(ofstream& out) const;
    void load(ifstream& in);
};

// --- BANK CLASS ---
class Bank : public KingdomComponent
{
private:
    int loanBalance;

public:
    Bank();
    void takeLoan(int& gold, int amount);
    void payLoan(int& gold, int amount);
    int getLoan() const;
    void displayInfo() const override;
    void save(ofstream& out) const;
    void load(ifstream& in);
};

// --- MARKET CLASS ---
class Market : public KingdomComponent
{
private:
    int foodPrice, woodPrice, stonePrice, ironPrice;

public:
    Market();
    void updatePrices();
    void displayPrices() const;
    void displayInfo() const override;
};

// --- GENERAL CLASS ---
class General : public KingdomComponent
{
public:
    string name;
    int strategy;

    General();
    void display() const;
    void displayInfo() const override;
};

// --- KING CLASS ---
class King
{
private:
    string name;
    string policy;
    int popularity;

public:
    King(string n = "Arthur", string p = "Moderate Tax", int pop = 50);
    void dropPopularity(int value);
    void displayPolicy() const;
    int taxRate() const;
};

// --- SOCIETY CLASS ---
class Society
{
private:
    int population;

public:
    Society(int initialPop);
    void redistribute(int newPop);
    void display() const;
};

// --- EVENT MANAGER CLASS ---
class EventManager
{
public:
    static void weatherEffect(Resources* res);
    static void specialEvents(Resources* res, Society* soc, King* king);
};

// --- KINGDOM CLASS ---
class Kingdom
{
public:
    Resources* res;
    Army* army;
    Bank* bank;
    Society* society;
    King* king;
    Market* market;
    General* general;
    int councilsConquered;

    Kingdom();
    ~Kingdom();
    void takeLoan(int amount);
    void payLoan(int amount);
    void recruitTroops(int num);
    void progressDay();
    void conquerCouncil();
    bool isGameOver() const;
    bool isWinner() const;
    void displayStatus();
    void saveGame(const string& filename);
    void loadGame(const string& filename);
};


