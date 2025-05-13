#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <fstream>

#include "strongled.h"
using namespace std;



// --- POPULATION ---
Population::Population(int initial) { total = initial; peasants = total * 60 / 100; merchants = total * 30 / 100; nobles = total - peasants - merchants; }
int Population::get() const { return total; }
void Population::set(int value) { total = value; peasants = total * 60 / 100; merchants = total * 30 / 100; nobles = total - peasants - merchants; }
void Population::add(int value) { set(total + value); }
void Population::subtract(int value) { set(total - value); }
void Population::displayComposition() const { cout << "Peasants: " << peasants << ", Merchants: " << merchants << ", Nobles: " << nobles << endl; }

// --- RESOURCES ---
Resources::Resources() { gold = 1000; food = 500; wood = 300; stone = 200; iron = 100; population = new Population(100); }
Resources::~Resources() { delete population; }
int Resources::getGold() const { return gold; }
int Resources::getFood() const { return food; }
int Resources::getWood() const { return wood; }
int Resources::getStone() const { return stone; }
int Resources::getIron() const { return iron; }
Population* Resources::getPopulation() { return population; }

// --- ARMY ---
Army::Army() { troops = weapons = barracks = 0; }
void Army::recruitTroops(int num) { troops += num; }
void Army::loseTroops(int num) { troops -= num; }
int Army::getTroops() const { return troops; }
int Army::getWeapons() const { return weapons; }
int Army::getBarracks() const { return barracks; }
void Army::buildBarracks(int& gold) { if (gold >= 200) { barracks++; gold -= 200; } }
void Army::buildBlacksmith(int& gold) { if (gold >= 150) { weapons++; gold -= 150; } }
void Army::paySoldiers(int& gold) { if (gold >= troops * 10) { gold -= troops * 10; } }
void Army::displayInfo() const { cout << "Troops: " << troops << ", Weapons: " << weapons << ", Barracks: " << barracks << endl; }
void Army::save(ofstream& out) const { out << troops << " " << weapons << " " << barracks << "\n"; }
void Army::load(ifstream& in) { in >> troops >> weapons >> barracks; }

// --- BANK ---
Bank::Bank() { loanBalance = 0; }
void Bank::takeLoan(int& gold, int amount) { loanBalance += amount; gold += amount; }
void Bank::payLoan(int& gold, int amount) { if (loanBalance >= amount && gold >= amount) { loanBalance -= amount; gold -= amount; } else { throw runtime_error("Insufficient gold or loan amount too large."); } }
int Bank::getLoan() const { return loanBalance; }
void Bank::displayInfo() const { cout << "Loan Balance: " << loanBalance << endl; }
void Bank::save(ofstream& out) const { out << loanBalance << "\n"; }
void Bank::load(ifstream& in) { in >> loanBalance; }

// --- MARKET ---
Market::Market() { updatePrices(); }
void Market::updatePrices() { foodPrice = rand() % 11 + 5; woodPrice = rand() % 11 + 10; stonePrice = rand() % 11 + 15; ironPrice = rand() % 11 + 20; }
void Market::displayPrices() const { cout << "Market Prices - Food: " << foodPrice << ", Wood: " << woodPrice << ", Stone: " << stonePrice << ", Iron: " << ironPrice << endl; }
void Market::displayInfo() const { displayPrices(); }

// --- GENERAL ---
General::General() { string names[] = { "Marcus", "Aquila", "Julius", "Tiberius" }; name = names[rand() % 4]; strategy = rand() % 2 + 1; }
void General::display() const { cout << "General: " << name << " | Strategy: " << (strategy == 1 ? "Aggressive" : "Defensive") << endl; }
void General::displayInfo() const { display(); }

// --- KING ---
King::King(string n, string p, int pop) { name = n; policy = p; popularity = pop; }
void King::dropPopularity(int value) { popularity -= value; }
void King::displayPolicy() const { cout << "King " << name << " - Policy: " << policy << " | Popularity: " << popularity << endl; }
int King::taxRate() const { return 10; }

// --- SOCIETY ---
Society::Society(int initialPop) { population = initialPop; }
void Society::redistribute(int newPop) { population = newPop; }
void Society::display() const { cout << "Society's current population: " << population << endl; }

// --- EVENT MANAGER ---
void EventManager::weatherEffect(Resources* res)
{
    int w = rand() % 3;
    if (w == 0) { cout << "Severe drought! Food -30 "; res->food -= min(30, res->food); }
    else if (w == 1) { cout << "Harsh winter! Wood -20 "; res->wood -= min(20, res->wood); }
    else { cout << "Weather is calm today. "; }
}
void EventManager::specialEvents(Resources* res, Society* soc, King* king)
{
    int e = rand() % 5;
    if (e == 0) { cout << "A plague has struck! Population -25% "; res->population->set(res->population->get() * 3 / 4); soc->redistribute(res->population->get()); king->dropPopularity(10); }
    else if (e == 1) { cout << "Attempted coup! Military loyalty drops. "; king->dropPopularity(15); }
}

// --- KINGDOM ---
Kingdom::Kingdom() { res = new Resources(); army = new Army(); bank = new Bank(); society = new Society(res->getPopulation()->get()); king = new King(); market = new Market(); general = new General(); councilsConquered = 0; }
Kingdom::~Kingdom() { delete res; delete army; delete bank; delete society; delete king; delete market; delete general; }
void Kingdom::takeLoan(int amount) { bank->takeLoan(res->gold, amount); }
void Kingdom::payLoan(int amount) { bank->payLoan(res->gold, amount); }
void Kingdom::recruitTroops(int num) { army->recruitTroops(num); }
void Kingdom::progressDay()
{
    try {
        res->gold += king->taxRate() * res->population->get() / 10;
        res->food -= res->population->get() / 2;
        if (res->food < 0) throw underflow_error("Food ran out!");
        res->population->add(res->population->get() / 10);
        society->redistribute(res->population->get());
        army->paySoldiers(res->gold);
        market->updatePrices();
        EventManager::weatherEffect(res);
        EventManager::specialEvents(res, society, king);
        cout << "Day progressed.";
    }
    catch (const exception& e) {
        cout << "Error during day progression: " << e.what() << endl;
    }
}
void Kingdom::conquerCouncil()
{
    try {
        if (army->getTroops() < 10) throw runtime_error("Not enough troops to conquer a council.");
        army->loseTroops(10);
        res->gold += 100;
        councilsConquered++;
        society->redistribute(res->population->get());
        cout << "Council conquered! Total councils conquered: " << councilsConquered << endl;
    }
    catch (const runtime_error& e) { cout << e.what() << endl; }
}
bool Kingdom::isGameOver() const { return res->getPopulation()->get() <= 0 || res->gold < 0; }
bool Kingdom::isWinner() const { return councilsConquered >= 3; }
void Kingdom::displayStatus()
{
    cout << "\n--- Kingdom Status ---\n";
    king->displayPolicy();
    army->displayInfo();
    market->displayInfo();
    society->display();
    cout << "Gold: " << res->getGold() << ", Food: " << res->getFood()
        << ", Wood: " << res->getWood() << ", Stone: " << res->getStone()
        << ", Iron: " << res->getIron() << endl;
    bank->displayInfo();
}
void Kingdom::saveGame(const string& filename)
{
    ofstream out(filename);
    out << res->gold << " " << res->food << " " << res->wood << " " << res->stone << " " << res->iron << "\n";
    out << res->getPopulation()->get() << "\n";
    army->save(out);
    bank->save(out);
    out << councilsConquered << "\n";
    out.close();
    cout << "Game saved.\n";
}
void Kingdom::loadGame(const string& filename)
{
    ifstream in(filename);
    if (!in) { cout << "Save file not found.\n"; return; }
    int pop;
    in >> res->gold >> res->food >> res->wood >> res->stone >> res->iron;
    in >> pop;
    res->population->set(pop);
    army->load(in);
    bank->load(in);
    in >> councilsConquered;
    society->redistribute(pop);
    in.close();
    cout << "Game loaded.\n";
}
