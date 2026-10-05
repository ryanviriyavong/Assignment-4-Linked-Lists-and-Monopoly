/*
 * ============================================================================
 * Academic Integrity & AI Use Declaration
 * Course: CS 210 - Data Structures / Algorithms
 * Assignment: Assignment 04 - Circular Linked List Monopoly
 * File Name: Viriyavong_Ryan_Assignment04.cpp
 * Author: Ryan Viriyavong
 * Date: 10/4/2026
 * 
 * AI Tool Used: Gemini
 * Nature of AI Assistance:
 *    - Assisted with structuring the custom circular linked list operations (add, search, remove).
 *    - Helped set up the player movement pointer traversal logic and game loop simulation.
 * 
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// struct for each space on the board
struct Property {
    std::string name;
    int cost;
    std::string owner;
    Property* next;

    Property(std::string n, int c) {
        name = n;
        cost = c;
        owner = "None";
        next = nullptr;
    }
};

// custom circular linked list 
class Board {
private:
    Property* head;
    Property* tail;
    int count;

public:
    Board() {
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    ~Board() {
        if (!head) return;
        Property* curr = head;
        for (int i = 0; i < count; i++) {
            Property* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    Property* getHead() { return head; }
    int getCount() { return count; }

    // add new property to the list
    void add(std::string name, int cost) {
        Property* node = new Property(name, cost);
        if (head == nullptr) {
            head = node;
            tail = node;
            node->next = head; // wrap back around
        } else {
            tail->next = node;
            tail = node;
            tail->next = head; // keep circular
        }
        count++;
    }

    // search for property by name
    Property* search(std::string name) {
        if (!head) return nullptr;
        Property* curr = head;
        for (int i = 0; i < count; i++) {
            if (curr->name == name) {
                return curr;
            }
            curr = curr->next;
        }
        return nullptr;
    }

    // remove a property by name
    bool remove(std::string name) {
        if (!head) return false;

        Property* curr = head;
        Property* prev = tail;

        for (int i = 0; i < count; i++) {
            if (curr->name == name) {
                if (count == 1) {
                    head = nullptr;
                    tail = nullptr;
                } else {
                    prev->next = curr->next;
                    if (curr == head) head = curr->next;
                    if (curr == tail) tail = prev;
                }
                delete curr;
                count--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    // display the whole board
    void display() {
        if (!head) {
            std::cout << "board is empty\n";
            return;
        }
        Property* curr = head;
        std::cout << "\n--- current board ---\n";
        for (int i = 0; i < count; i++) {
            std::cout << i + 1 << ". " << curr->name 
                      << " ($" << curr->cost << ") - owner: " 
                      << curr->owner << "\n";
            curr = curr->next;
        }
        std::cout << "---------------------\n";
    }
};

// struct to track player stats
struct Player {
    std::string name;
    int money;
    Property* pos;

    Player(std::string n, int m, Property* start) {
        name = n;
        money = m;
        pos = start;
    }

    void move(int steps) {
        for (int i = 0; i < steps; i++) {
            pos = pos->next;
        }
    }
};

// game simulation
int main() {
    srand(time(0));

    // set up board with 11 properties
    Board b;
    b.add("GO", 0);
    b.add("Mediterranean Ave", 60);
    b.add("Baltic Ave", 60);
    b.add("Reading Railroad", 200);
    b.add("Oriental Ave", 100);
    b.add("Vermont Ave", 100);
    b.add("Connecticut Ave", 120);
    b.add("St. Charles Place", 140);
    b.add("States Ave", 140);
    b.add("Virginia Ave", 160);
    b.add("Boardwalk", 400);

    b.display();

    Player p1("Ryan", 1000, b.getHead());
    Player p2("Jason", 1000, b.getHead());

    Player* players[2] = { &p1, &p2 };

    // run 10 turns
    std::cout << "\nstarting simulation...\n";
    for (int turn = 1; turn <= 10; turn++) {
        std::cout << "\n--- turn " << turn << " ---\n";

        for (int i = 0; i < 2; i++) {
            Player* p = players[i];
            int roll = (rand() % 6) + 1;
            p->move(roll);

            Property* spot = p->pos;
            std::cout << p->name << " rolled a " << roll << " and landed on " << spot->name << "\n";

            // buy property if unowned and not GO
            if (spot->cost > 0 && spot->owner == "None") {
                if (p->money >= spot->cost) {
                    p->money -= spot->cost;
                    spot->owner = p->name;
                    std::cout << "  -> " << p->name << " bought " << spot->name 
                              << " for $" << spot->cost << " (money left: $" << p->money << ")\n";
                } else {
                    std::cout << "  -> " << p->name << " didn't have enough money\n";
                }
            } else if (spot->owner != "None") {
                std::cout << "  -> already owned by " << spot->owner << "\n";
            }
        }
    }

    std::cout << "\n--- final results ---";
    b.display();

    std::cout << "player status:\n";
    for (int i = 0; i < 2; i++) {
        std::cout << players[i]->name << ": $" << players[i]->money 
                  << " at " << players[i]->pos->name << "\n";
    }

    return 0;
}