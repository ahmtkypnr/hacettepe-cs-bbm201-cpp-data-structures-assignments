#include <fstream>
#include <iostream>
#include "GameController.h"

void print_final(vector<vector<int>> grid, int score, LeaderboardEntry* highest) {
    cout << "Final grid and score:" << endl << endl;
    cout << "Score: " << score << endl;
    if (highest == nullptr) {
        cout << "High Score: " << 0 << endl;
    }
    else{
        cout << "High Score: " << highest->score << endl;
    }
    for (int i = 0; i < grid.size(); ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            if (grid[i][j] == 1) {
                cout << "██";
            }
            else {
                cout << "▒▒";
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool is_possible(Block* block, vector<vector<int>> grid, int row, int col) {
    if (row < 0 || col < 0) {
        return false;
    }

    for (int i = row; i < block->shape.size() + row; ++i) {
        for (int j = col; j < block->shape[0].size() + col; ++j) {
            if(i >= grid.size() || j >= grid[0].size()) {
                if(block->shape[i - row][j - col]) {
                    return false;
                }
            }

            else{
                if (block->shape[i - row][j - col] && grid[i][j] == 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

void apply_gravity(vector<vector<int>> &grid) {
    for (int i = 0; i < grid[0].size(); ++i) {
        int block_count = 0;
        for (int j = 0; j < grid.size(); ++j) {
            if (grid[j][i] == 1) {
                grid[j][i] = 0;
                block_count++;
            }
        }
        for (int j = grid.size() - 1; j >= 0 && block_count > 0; --j) {
            grid[j][i] = 1;
            block_count--;
        }
    }

    for (int i = 0; i < grid.size() - 1; ++i) {
        for (int j = 0; j < grid[0].size(); ++j) {
            if (grid[i][j] == 1 && grid[i + 1][j] == 0) {
                grid[i][j] = 0;
                grid[i + 1][j] = 1;
            }
        }
    }
}

void scan_powerup(vector<vector<int>> &grid, vector<vector<bool>> power_up, int &score, bool is_printed) {
    for (int i = 0; i <= grid.size() - power_up.size(); ++i) {
        for (int j = 0; j <= grid[0].size() - power_up[0].size(); ++j) {
            bool match = true;
            for (int si = 0; si < power_up.size() && match; ++si) {
                for (int sj = 0; sj < power_up[0].size(); ++sj) {
                    if (grid[i + si][j + sj] != int(power_up[si][sj])) {
                        match = false;
                        break;
                    }
                }
            }
            if (match) {
                score = score + 1000;
                for (int k = 0; k < grid.size(); ++k) {
                    for (int l = 0; l < grid[0].size(); ++l) {
                        if (grid[k][l] == 1) {
                            score = score + 1;
                        }
                        grid[k][l] = 0;
                    }
                }
                if (!is_printed) {
                    cout << "Before clearing:" << endl;
                    for (int l = 0; l < grid.size(); ++l) {
                        for (int k = 0; k < grid[0].size(); ++k) {
                            if (grid[l][k] == 1) {
                                cout << "██";
                            }
                            else {
                                cout << "▒▒";
                            }
                        }
                        cout << endl;
                    }
                    cout << endl;
                    is_printed = true;
                }
                return;  // Shape found
            }
        }
    }
}

void clear(vector<vector<int>> &grid, bool &is_printed, int &score) {
    for (int i = 0; i < grid.size(); ++i) {
        bool is_full = true;
        for (int j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == 0) {
                is_full = false;
                break;
            }
        }
        if (!is_full) {
            continue;
        }

        if (!is_printed) {
            cout << "Before clearing:" << endl;
            for (int j = 0; j < grid.size(); ++j) {
                for (int k = 0; k < grid[0].size(); ++k) {
                    if (grid[j][k] == 1) {
                        cout << "██";
                    }
                    else {
                        cout << "▒▒";
                    }
                }
                cout << endl;
            }
            cout << endl;
            is_printed = true;
        }

        vector<vector<int>> temp = grid;
        for (int k = 1; k < i + 1; ++k) {
            for (int j = 0; j < grid[k].size(); ++j) {
                grid[k][j] = temp[k - 1][j];
            }
        }
        for (int j = 0; j < grid[i].size(); ++j) {
            grid[0][j] = 0;
        }
        score = score + grid[0].size();
    }
}

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    //#define occupiedCellChar "██"
    //#define unoccupiedCellChar "▒▒"
    std::ifstream file(commands_file);
    std::string line;
    Block* active_block;
    int score = 0;
    int row_active = 0;
    int col_active = 0;
    int block_row;
    int block_col;

    active_block = game.active_rotation;
    bool is_printed;
    while (std::getline(file, line)) {
        //line.pop_back();
        if (line == "PRINT_GRID") {
            cout << "Score: " << score << endl;
            if (game.leaderboard.head_leaderboard_entry == nullptr) {
                cout << "High Score: " << 0 << endl;
            }
            else{
                cout << "High Score: " << game.leaderboard.head_leaderboard_entry->score << endl;
            }
            for (int i = 0; i < game.rows; ++i) {
                block_row = i - row_active;
                for (int j = 0; j < game.cols; ++j) {
                    block_col = j - col_active;
                    if ((block_row >= 0 && block_row < active_block->shape.size()) && (block_col >= 0 && block_col < active_block->shape[0].size())) {
                        if (active_block->shape[block_row][block_col] || game.grid[i][j] == 1) {
                            cout << "██";
                        }
                        else {
                            cout << "▒▒";
                        }
                    }
                    else {
                        if (game.grid[i][j] == 1) {
                            cout << "██";
                        }
                        else {
                            cout << "▒▒";
                        }
                    }
                }
                cout << endl;
            }
            cout << endl;
        }

        if (line == "ROTATE_RIGHT") {
            if (is_possible(active_block->right_rotation, game.grid, row_active, col_active)) {
                active_block = active_block->right_rotation;
            }
        }

        if (line == "ROTATE_LEFT") {
            if (is_possible(active_block->left_rotation, game.grid, row_active, col_active)) {
                active_block = active_block->left_rotation;
            }
        }

        if (line == "MOVE_RIGHT") {
            if (is_possible(active_block, game.grid, row_active, col_active + 1)) {
                col_active++;
            }
        }

        if (line == "MOVE_LEFT") {
            if (is_possible(active_block, game.grid, row_active, col_active - 1)) {
                col_active--;
            }
        }

        if (line == "DROP") {
            int blocks_size = 0;
            for (int i = 0; i < active_block->shape.size(); ++i) {
                for (int j = 0; j < active_block->shape[0].size(); ++j) {
                    if (active_block->shape[i][j]) {
                        blocks_size++;
                    }
                }
            }
            while (is_possible(active_block, game.grid, row_active + 1, col_active)) {
                row_active++;
                score = score + blocks_size;
            }

            for (int i = 0; i < active_block->shape.size(); ++i) {
                for (int j = 0; j < active_block->shape[i].size(); ++j) {
                    if (active_block->shape[i][j]) {
                        game.grid[i + row_active][j + col_active] = 1;
                    }
                }
            }

            is_printed = false;

            if (game.gravity_mode_on) {
                apply_gravity(game.grid);
            }

            scan_powerup(game.grid, game.power_up, score, is_printed);
            clear(game.grid, is_printed, score);

            if (game.gravity_mode_on) {
                apply_gravity(game.grid);
                clear(game.grid, is_printed, score);
                apply_gravity(game.grid);
            }

            if (active_block->next_block == nullptr) {
                //change_leaderboard(game.leaderboard, game.player_name, score);
                game.leaderboard.insert_new_entry(new LeaderboardEntry(score, time(nullptr), game.player_name));
                game.leaderboard.write_to_file(game.leaderboard_file_name);
                cout << "YOU WIN!" << endl;
                cout << "No more blocks." << endl;
                print_final(game.grid, score, game.leaderboard.head_leaderboard_entry);
                game.current_score = score;
                game.active_rotation = active_block;
                game.leaderboard.print_leaderboard();
                return true;
            }

            if (is_possible(active_block->next_block, game.grid, 0, 0)) {
                row_active = 0;
                col_active = 0;
                active_block = active_block->next_block;
            }

            else {
                //change_leaderboard(game.leaderboard, game.player_name, score);
                game.leaderboard.insert_new_entry(new LeaderboardEntry(score, time(nullptr), game.player_name));
                game.leaderboard.write_to_file(game.leaderboard_file_name);
                cout << "GAME OVER!" << endl;
                cout << "Next block that couldn't fit:" << endl;
                for (int i = 0; i < active_block->next_block->shape.size(); ++i) {
                    for (int j = 0; j < active_block->next_block->shape[0].size(); ++j) {
                        if (active_block->next_block->shape[i][j]) {
                            cout << "██";
                        }
                        else {
                            cout << "▒▒";
                        }
                    }
                    cout << endl;
                }
                cout << endl;
                print_final(game.grid, score, game.leaderboard.head_leaderboard_entry);
                game.current_score = score;
                game.active_rotation = active_block;
                game.leaderboard.print_leaderboard();
                return false;
            }
        }

        if (line == "GRAVITY_SWITCH") {
            game.gravity_mode_on = !game.gravity_mode_on;
            if (game.gravity_mode_on) {
                is_printed = true;
                apply_gravity(game.grid);
                scan_powerup(game.grid, game.power_up, score, is_printed);
                clear(game.grid, is_printed, score);
                apply_gravity(game.grid);
            }
        }
    }
    file.close();



    //change_leaderboard(game.leaderboard, game.player_name, score);
    game.leaderboard.insert_new_entry(new LeaderboardEntry(score, time(nullptr), game.player_name));
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    cout << "GAME FINISHED!" << endl;
    cout << "No more commands." << endl;
    print_final(game.grid, score, game.leaderboard.head_leaderboard_entry);
    game.current_score = score;
    game.active_rotation = active_block;
    game.leaderboard.print_leaderboard();

    return true;
}
