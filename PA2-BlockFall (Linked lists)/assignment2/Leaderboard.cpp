#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
    if (head_leaderboard_entry == nullptr || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry* current = head_leaderboard_entry;
        while (current->next_leaderboard_entry != nullptr && new_entry->score <= current->next_leaderboard_entry->score) {
            current = current->next_leaderboard_entry;
        }

        new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
        current->next_leaderboard_entry = new_entry;
    }

    size++;

    if (size > 10) {
        LeaderboardEntry* lastNode = head_leaderboard_entry;
        while (lastNode->next_leaderboard_entry->next_leaderboard_entry != nullptr) {
            lastNode = lastNode->next_leaderboard_entry;
        }

        delete lastNode->next_leaderboard_entry;
        lastNode->next_leaderboard_entry = nullptr;

        size--;
    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    ofstream file;
    file.open(filename);

    LeaderboardEntry* active_entry = head_leaderboard_entry;
    while (active_entry != nullptr) {
        file << active_entry->score << " " << active_entry->last_played << " " << active_entry->player_name << endl;
        active_entry = active_entry->next_leaderboard_entry;
    }
    file.close();
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        std::vector<std::string> words;
        while (iss >> word) {
            words.push_back(word);
        }

        insert_new_entry(new LeaderboardEntry(stoi(words[0]), stoi(words[1]), words[2]));
    }
    file.close();
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions

    cout << "Leaderboard" << endl;
    cout << "-----------" << endl;

    int i = 0;
    LeaderboardEntry* entry = head_leaderboard_entry;
    while (entry != nullptr) {
        struct tm* timeinfo = localtime(&entry->last_played);

        char timeString[20];

        strftime(timeString, sizeof(timeString), "%H:%M:%S/%d.%m.%Y", timeinfo);

        std::string t_string = std::string(timeString);

        cout << i + 1 << ". " << entry->player_name << " " << entry->score << " " << t_string << endl;

        entry = entry->next_leaderboard_entry;
        i++;
    }
    cout << endl;
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* temp;
    while (current != nullptr) {
        temp = current->next_leaderboard_entry;
        delete current;
        current = temp;
    }
}
