#include <fstream>
#include <iostream>
#include <algorithm>
#include "BlockFall.h"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

vector<vector<bool>> BlockFall::rotate_right(vector<vector<bool>> block) {
    std::vector<vector<bool>> rotated;
    for (int j = 0; j < block[0].size(); ++j) {
        std::vector<bool> rotated_part;
        for (int k = 0; k < block.size(); ++k) {
            rotated_part.push_back(block[k][j]);
        }
        reverse(rotated_part.begin(), rotated_part.end());
        rotated.push_back(rotated_part);
    }
    return rotated;
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    std::ifstream file(input_file);
    std::string line;

    std::vector<vector<vector<bool>>> blocks;

    while (std::getline(file, line)) {
        if (!line.empty() && (line[0] == '[' || line[0] == '1' || line[0] == '0')) {
            if (line[0] == '[') {
                std::vector<vector<bool>> block;
                while (true) {
                    if (!line.empty() && (line[0] == '[' || line[0] == '1' || line[0] == '0')) {
                        std::vector<bool> part;
                        for (int j = 0; j < line.size(); ++j) {
                            if (line[j] == '0' || line[j] == '1') {
                                int b = line[j] - '0';
                                part.push_back(b);
                            }
                        }

                        block.push_back(part);

                        if (line.back() == ']' || line[line.size() - 2] == ']') {
                            blocks.push_back(block);
                            break;
                        }
                    }
                    std::getline(file, line);
                }
            }
        }
    }
    file.close();

    initial_block = new Block;
    initial_block->shape = blocks[0];

    power_up = blocks.back();
    blocks.pop_back();

    active_rotation = initial_block;

    vector<vector<bool>> rotated;
    Block* current_block = initial_block;
    Block* current_rotation_block;
    Block* next_rotation_block;
    Block* next_block;
    for (int i = 0; i < blocks.size(); ++i) {
        if (i == blocks.size() - 1) {
            next_block = nullptr;
        }
        else {
            next_block = new Block;
            next_block->shape = blocks[i + 1];
        }
        current_block->next_block = next_block;
        current_rotation_block = current_block;
        while (true) {
            rotated = rotate_right(current_rotation_block->shape);

            if (rotated == blocks[i]) {
                current_rotation_block->right_rotation = current_block;
                current_block->left_rotation = current_rotation_block;
                break;
            }

            next_rotation_block = new Block;
            next_rotation_block->shape = rotated;
            next_rotation_block->next_block = next_block;
            next_rotation_block->left_rotation = current_rotation_block;

            current_rotation_block->right_rotation = next_rotation_block;
            current_rotation_block = next_rotation_block;
        }
        current_block = next_block;
    }
}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    std::ifstream file(input_file);
    std::string line;
    std::vector<vector<int>> new_grid;

    while (std::getline(file, line)) {
        vector<int> grid_part;
        for (int i = 0; i < line.size(); i += 2) {
            grid_part.push_back(line[i] - '0');
        }
        new_grid.push_back(grid_part);
    }
    file.close();

    grid = new_grid;
    rows = new_grid.size();
    cols = new_grid[0].size();
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block* active_block = initial_block;
    Block* temp;
    while (active_block != nullptr) {
        Block* active_rotate = active_block->right_rotation;
        while (active_rotate != active_block) {
            temp = active_rotate->right_rotation;
            delete active_rotate;
            active_rotate = temp;
        }
        temp = active_block->next_block;
        delete active_block;
        active_block = temp;
    }
}
