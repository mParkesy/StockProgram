/* 
 * File:        Inventory.cpp
 * Author:      100116544
 * Description: CPP file for Inventory class which consists
 *              of StockItem objects.
 * 
 * Created on 26 April 2018, 11:05
 */

using namespace std;

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Inventory.h"
#include "StockItem.h"

Inventory::Inventory() {
}

Inventory::Inventory(const Inventory& orig) {
}

Inventory::~Inventory() {
    for (StockItem *item : inventory) {
        delete item;
    }
}

StockItem& Inventory::HighestStock() {
    StockItem *largest;
    largest->setQuantity(0);
    for (StockItem *stockitem : inventory) {
        if (stockitem->getQuantity() > largest->getQuantity()) {
            largest = stockitem;
        }
    }
    return *largest;
}

int Inventory::searchInfo(string search) {
    int total = 0;
    for (StockItem *stockitem : inventory) {
        if (stockitem->getInfo() == search) {
            total += stockitem->getQuantity();
        }
    }
    return total;
}

double Inventory::getTotalInStockResistance() {
    double total = 0;
    Resistor *r = nullptr;
    for (StockItem *stockitem : inventory) {
        if (stockitem->getQuantity() > 0 && stockitem->getType() == "Resistor") {
            r = static_cast<Resistor*>(stockitem);
            total += r->getResistance() * r->getQuantity();
        }
    }
    return total;
}

int Inventory::getTotalStockAbovePrice(int amount) {
    int stockTotal = 0;
    for (StockItem *stockitem : this->inventory) {
        if (stockitem->getPrice() > amount) {
            stockTotal++;
        }
    }
    return stockTotal;
}

void Inventory::sortInv(int sortType){
    sort(inventory.begin(), inventory.end(), [&sortType](const StockItem* a, 
            const StockItem* b) -> bool{
        if(sortType == 1){
            return a->getPrice() > b->getPrice();
        }else if(sortType == 2){
            return a->getPrice() < b->getPrice();
        } else if(sortType == 3){
            return a->getType() < b->getType();
        } else if(sortType == 4){
            return a->getCode() > b->getCode();
        } else if(sortType == 5){
            return a->getQuantity() > b->getQuantity();
        } else if(sortType == 6){
            return a->getQuantity() < b->getQuantity();
        }       
    });
}

std::ostream& operator<<(std::ostream &os, const Inventory &inv){
    for(StockItem *stockitem : inv.inventory){
        os << *stockitem << endl;
    }
    return os;
}

std::istream& operator>>(std::istream &is, Inventory &inv) {
    ifstream infile(inv.getFile());
    while (infile) {
        string s;
        if (!getline(infile, s)) {
            break;
        }
        istringstream ss(s);
        vector<string> line;
        while (ss) {
            string s;
            if (!getline(ss, s, ',')) {
                break;
            }
            line.push_back(s);
        }
        string type = line.at(0);
        string code = line.at(1);
        string quantity = line.at(2);
        string price = line.at(3);

        if (type == "resistor") {
            string resistance = removeCharacters(line.at(4));
            Resistor *resistor = new Resistor(resistance, type, code, 
                    stoi(quantity), stoi(price));
            inv.addStock(resistor);
        } else if (type == "transistor") {
            string tranType = removeCharacters(line.at(4));
            Transistor *transistor = new Transistor(tranType, type, code, 
                    stoi(quantity), stoi(price));
            inv.addStock(transistor);
        } else if (type == "capacitor") {
            string capacitance = removeCharacters(line.at(4));
            Capacitor *capacitor = new Capacitor(capacitance, type, code, 
                    stoi(quantity), stoi(price));
            inv.addStock(capacitor);
        } else if (type == "diode") {
            Diode *diode = new Diode(type, code, stoi(quantity), stoi(price));
            inv.addStock(diode);
        } else if (type == "IC") {
            string ICtype = removeCharacters(line.at(4));
            IC *ic = new IC(ICtype, type, code, stoi(quantity), stoi(price));
            inv.addStock(ic);
        }
    }
    infile.close();
    return is;
}

string removeCharacters(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ' || str[i] == '\r') {
            str.erase(i, 1);
        }
    }
    return str;
}