#include "Network.h"

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */

    for (int i = 0; i < commands.size(); ++i) {
        istringstream ss(commands[i]);
        string word;
        ss >> word;

        string command_output = "Command: " + commands[i];
        cout << string(command_output.size(), '-') << endl << command_output << endl << string(command_output.size(), '-') << endl;
        if (word == "MESSAGE") {
            ss >> word;

            for (int j = 0; j < clients.size(); ++j) {
                if (word == clients[j].client_id) {
                    ss >> word;
                    for (int k = 0; k < clients.size(); ++k) {
                        if (word == clients[k].client_id) {
                            string delim = "#";
                            size_t pos = commands[i].find(delim);
                            string message = commands[i].substr(pos + 1);
                            message.pop_back();
                            cout << "Message to be sent: \"" << message << "\"" << endl << endl;
                            int frame_count = 0;
                            for (int l = 0; l < message.size(); l += message_limit) {
                                string chunk = message.substr(l, message_limit);
                                stack<Packet*> frame;
                                ApplicationLayerPacket* applicationLayerPacket = new ApplicationLayerPacket(0, clients[j].client_id, clients[k].client_id, chunk);
                                TransportLayerPacket* transportLayerPacket = new TransportLayerPacket(1, sender_port, receiver_port);
                                NetworkLayerPacket* networkLayerPacket = new NetworkLayerPacket(2, clients[j].client_ip, clients[k].client_ip);
                                frame.push(applicationLayerPacket);
                                frame.push(transportLayerPacket);
                                frame.push(networkLayerPacket);

                                string mac;
                                for (int m = 0; m < clients.size(); ++m) {
                                    if (clients[j].routing_table[clients[k].client_id] == clients[m].client_id) {
                                        mac = clients[m].client_mac;
                                        break;
                                    }
                                }

                                PhysicalLayerPacket* physicalLayerPacket = new PhysicalLayerPacket(3, clients[j].client_mac, mac);
                                frame.push(physicalLayerPacket);

                                clients[j].outgoing_queue.push(frame);
                                frame_count++;

                                cout << "Frame #" << frame_count << endl;
                                physicalLayerPacket->print();
                                cout << endl;
                                networkLayerPacket->print();
                                cout << endl;
                                transportLayerPacket->print();
                                cout << endl;
                                applicationLayerPacket->print();
                                cout << endl;
                                cout << "Message chunk carried: \"" << chunk << "\"" << endl;
                                cout << "Number of hops so far: " << physicalLayerPacket->hop_count << endl;
                                cout << "--------" << endl;
                            }
                            clients[j].log_entries.push_back(Log(to_string(time(nullptr)), message, frame_count, 0, clients[j].client_id, clients[k].client_id, true, ActivityType::MESSAGE_SENT));
                        }
                    }
                }
            }
        }
        else if (word == "SHOW_FRAME_INFO") {
            ss >> word;
            int j = 0;
            while (j < clients.size()) {
                if (clients[j].client_id == word) {
                    break;
                }
                j++;
            }
            ss >> word;
            if (word == "out") {
                ss >> word;
                int frame_number = stoi(word);
                if (clients[j].outgoing_queue.size() < frame_number) {
                    cout << "No such frame." << endl;
                }
                else {
                    int k = 1;
                    queue<stack<Packet*>> temp;
                    while (!clients[j].outgoing_queue.empty()) {
                        if (k == frame_number) {
                            PhysicalLayerPacket* physical_packet = static_cast<PhysicalLayerPacket *>(clients[j].outgoing_queue.front().top());
                            clients[j].outgoing_queue.front().pop();
                            NetworkLayerPacket* network_packet = static_cast<NetworkLayerPacket *>(clients[j].outgoing_queue.front().top());
                            clients[j].outgoing_queue.front().pop();
                            TransportLayerPacket* transport_packet = static_cast<TransportLayerPacket *>(clients[j].outgoing_queue.front().top());
                            clients[j].outgoing_queue.front().pop();
                            ApplicationLayerPacket* application_packet = static_cast<ApplicationLayerPacket *>(clients[j].outgoing_queue.front().top());
                            clients[j].outgoing_queue.front().push(transport_packet);
                            clients[j].outgoing_queue.front().push(network_packet);
                            clients[j].outgoing_queue.front().push(physical_packet);
                            cout << "Current Frame #" << k << " on the outgoing queue of client " << clients[j].client_id << endl;
                            cout << "Carried Message: \"" << application_packet->message_data << "\"" << endl;
                            cout << "Layer 0 info: ";
                            application_packet->print();
                            cout << endl;
                            cout << "Layer 1 info: ";
                            transport_packet->print();
                            cout << endl;
                            cout << "Layer 2 info: ";
                            network_packet->print();
                            cout << endl;
                            cout << "Layer 3 info: ";
                            physical_packet->print();
                            cout << endl;
                            cout << "Number of hops so far: " << physical_packet->hop_count << endl;
                        }
                        temp.push(clients[j].outgoing_queue.front());
                        clients[j].outgoing_queue.pop();
                        k++;
                    }
                    while (!temp.empty()) {
                        clients[j].outgoing_queue.push(temp.front());
                        temp.pop();
                    }
                }
            }
            else {
                ss >> word;
                int frame_number = stoi(word);
                if (clients[j].incoming_queue.size() < frame_number) {
                    cout << "No such frame." << endl;
                }
                else {
                    int k = 1;
                    queue<stack<Packet*>> temp;
                    while (!clients[j].incoming_queue.empty()) {
                        if (k == frame_number) {
                            PhysicalLayerPacket* physical_packet = static_cast<PhysicalLayerPacket *>(clients[j].incoming_queue.front().top());
                            clients[j].incoming_queue.front().pop();
                            NetworkLayerPacket* network_packet = static_cast<NetworkLayerPacket *>(clients[j].incoming_queue.front().top());
                            clients[j].incoming_queue.front().pop();
                            TransportLayerPacket* transport_packet = static_cast<TransportLayerPacket *>(clients[j].incoming_queue.front().top());
                            clients[j].incoming_queue.front().pop();
                            ApplicationLayerPacket* application_packet = static_cast<ApplicationLayerPacket *>(clients[j].incoming_queue.front().top());
                            clients[j].incoming_queue.front().push(transport_packet);
                            clients[j].incoming_queue.front().push(network_packet);
                            clients[j].incoming_queue.front().push(physical_packet);
                            cout << "Current Frame #" << k << " on the incoming queue of client " << clients[j].client_id << endl;
                            cout << "Carried Message: \"" << application_packet->message_data << "\"" << endl;
                            cout << "Layer 0 info: ";
                            application_packet->print();
                            cout << endl;
                            cout << "Layer 1 info: ";
                            transport_packet->print();
                            cout << endl;
                            cout << "Layer 2 info: ";
                            network_packet->print();
                            cout << endl;
                            cout << "Layer 3 info: ";
                            physical_packet->print();
                            cout << endl;
                            cout << "Number of hops so far: " << physical_packet->hop_count << endl;
                        }
                        temp.push(clients[j].incoming_queue.front());
                        clients[j].incoming_queue.pop();
                        k++;
                    }
                    while (!temp.empty()) {
                        clients[j].incoming_queue.push(temp.front());
                        temp.pop();
                    }
                }
            }
        }
        else if (word == "SHOW_Q_INFO") {
            ss >> word;
            int j = 0;
            while (j < clients.size()) {
                if (clients[j].client_id == word) {
                    break;
                }
                j++;
            }
            ss >> word;
            if (word == "out") {
                cout << "Client " << clients[j].client_id << " Outgoing Queue Status" << endl;
                cout << "Current total number of frames: " << clients[j].outgoing_queue.size() << endl;
            }
            else {
                cout << "Client " << clients[j].client_id << " Incoming Queue Status" << endl;
                cout << "Current total number of frames: " << clients[j].incoming_queue.size() << endl;
            }
        }
        else if (word == "SEND") {
            for (int j = 0; j < clients.size(); ++j) {
                string message = "";
                int frame = 0;
                while (!clients[j].outgoing_queue.empty()) {
                    PhysicalLayerPacket* physical_packet = static_cast<PhysicalLayerPacket *>(clients[j].outgoing_queue.front().top());
                    int k = 0;
                    while (clients[k].client_mac != physical_packet->receiver_MAC_address) {
                        k++;
                    }

                    clients[j].outgoing_queue.front().pop();
                    NetworkLayerPacket* network_packet = static_cast<NetworkLayerPacket *>(clients[j].outgoing_queue.front().top());
                    clients[j].outgoing_queue.front().pop();
                    TransportLayerPacket* transport_packet = static_cast<TransportLayerPacket *>(clients[j].outgoing_queue.front().top());
                    clients[j].outgoing_queue.front().pop();
                    ApplicationLayerPacket* application_packet = static_cast<ApplicationLayerPacket *>(clients[j].outgoing_queue.front().top());
                    clients[j].outgoing_queue.front().push(transport_packet);
                    clients[j].outgoing_queue.front().push(network_packet);
                    clients[j].outgoing_queue.front().push(physical_packet);

                    message += application_packet->message_data;
                    string chunk = application_packet->message_data;
                    frame++;
                    physical_packet->hop_count++;

                    cout << "Client " << clients[j].client_id << " sending frame #" << frame << " to client " << clients[k].client_id << endl;
                    physical_packet->print();
                    cout << endl;
                    network_packet->print();
                    cout << endl;
                    transport_packet->print();
                    cout << endl;
                    application_packet->print();
                    cout << endl;
                    cout << "Message chunk carried: \"" << chunk << "\"" << endl;
                    cout << "Number of hops so far: " << physical_packet->hop_count << endl;
                    cout << "--------" << endl;

                    if (application_packet->message_data.back() == '.' || application_packet->message_data.back() == '!' || application_packet->message_data.back() == '?') {
                        message = "";
                        frame = 0;
                    }

                    clients[k].incoming_queue.push(clients[j].outgoing_queue.front());
                    clients[j].outgoing_queue.pop();
                }
            }
        }
        else if (word == "RECEIVE") {
            for (int j = 0; j < clients.size(); ++j) {
                int frame = 0;
                string message = "";
                while (!clients[j].incoming_queue.empty()) {
                    PhysicalLayerPacket* physical_packet = static_cast<PhysicalLayerPacket *>(clients[j].incoming_queue.front().top());
                    clients[j].incoming_queue.front().pop();
                    NetworkLayerPacket* network_packet = static_cast<NetworkLayerPacket *>(clients[j].incoming_queue.front().top());
                    clients[j].incoming_queue.front().pop();
                    TransportLayerPacket* transport_packet = static_cast<TransportLayerPacket *>(clients[j].incoming_queue.front().top());
                    clients[j].incoming_queue.front().pop();
                    ApplicationLayerPacket* application_packet = static_cast<ApplicationLayerPacket *>(clients[j].incoming_queue.front().top());
                    clients[j].incoming_queue.front().push(transport_packet);
                    clients[j].incoming_queue.front().push(network_packet);
                    clients[j].incoming_queue.front().push(physical_packet);

                    if (application_packet->receiver_ID == clients[j].client_id) {
                        message += application_packet->message_data;
                        string chunk = application_packet->message_data;
                        frame++;

                        int k = 0;
                        while (clients[k].client_mac != physical_packet->sender_MAC_address) {
                            k++;
                        }

                        cout << "Client " << clients[j].client_id << " receiving frame #" << frame << " from client " << clients[k].client_id << ", originating from client " << application_packet->sender_ID << endl;
                        physical_packet->print();
                        cout << endl;
                        network_packet->print();
                        cout << endl;
                        transport_packet->print();
                        cout << endl;
                        application_packet->print();
                        cout << endl;
                        cout << "Message chunk carried: \"" << chunk << "\"" << endl;
                        cout << "Number of hops so far: " << physical_packet->hop_count << endl;
                        cout << "--------" << endl;

                        if (application_packet->message_data.back() == '.' || application_packet->message_data.back() == '!' || application_packet->message_data.back() == '?') {
                            clients[j].log_entries.push_back(Log(to_string(time(nullptr)), message, frame, physical_packet->hop_count, application_packet->sender_ID, application_packet->receiver_ID,
                                                                 true, ActivityType::MESSAGE_RECEIVED));
                            cout << "Client " << clients[j].client_id << " received the message \"" << message << "\" from client " << application_packet->sender_ID << "." << endl;
                            cout << "--------" << endl;
                            message = "";
                            frame = 0;
                        }

                        delete clients[j].incoming_queue.front().top();
                        clients[j].incoming_queue.front().pop();
                        delete clients[j].incoming_queue.front().top();
                        clients[j].incoming_queue.front().pop();
                        delete clients[j].incoming_queue.front().top();
                        clients[j].incoming_queue.front().pop();
                        delete clients[j].incoming_queue.front().top();
                        clients[j].incoming_queue.front().pop();
                        clients[j].incoming_queue.pop();
                    }

                    else {
                        bool routing = false;
                        int k = 0;
                        while (k < clients.size()) {
                            if (clients[j].routing_table[application_packet->receiver_ID] == clients[k].client_id) {
                                routing = true;
                                break;
                            }
                            k++;
                        }


                        if (routing) {
                            message += application_packet->message_data;
                            frame++;

                            if (frame == 1) {
                                int l = 0;
                                while (clients[l].client_mac != physical_packet->sender_MAC_address) {
                                    l++;
                                }

                                cout << "Client " << clients[j].client_id << " receiving a message from client " << clients[l].client_id << ", but intended for client " << application_packet->receiver_ID << ". Forwarding... " << endl;
                            }

                            string mac;
                            for (int m = 0; m < clients.size(); ++m) {
                                if (clients[j].routing_table[application_packet->receiver_ID] == clients[m].client_id) {
                                    mac = clients[m].client_mac;
                                    break;
                                }
                            }

                            cout << "Frame #" << frame << " MAC address change: New sender MAC " << clients[j].client_mac << ", new receiver MAC " << mac << endl;

                            if (application_packet->message_data.back() == '.' || application_packet->message_data.back() == '!' || application_packet->message_data.back() == '?') {
                                clients[j].log_entries.push_back(Log(to_string(time(nullptr)), message, frame, physical_packet->hop_count, application_packet->sender_ID, application_packet->receiver_ID,
                                                                     true, ActivityType::MESSAGE_FORWARDED));
                                cout << "--------" << endl;
                                message = "";
                                frame = 0;
                            }

                            physical_packet->receiver_MAC_address = mac;
                            physical_packet->sender_MAC_address = clients[j].client_mac;
                            clients[j].outgoing_queue.push(clients[j].incoming_queue.front());
                            clients[j].incoming_queue.pop();
                        }

                        else {
                            message += application_packet->message_data;
                            frame++;

                            int l = 0;
                            while (clients[l].client_mac != physical_packet->sender_MAC_address) {
                                l++;
                            }

                            cout << "Client " << clients[j].client_id << " receiving frame #" << frame << " from client " << clients[l].client_id << ", but intended for client " << application_packet->receiver_ID << ". Forwarding... " << endl;
                            cout << "Error: Unreachable destination. Packets are dropped after " << physical_packet->hop_count << " hops!" << endl;

                            if (application_packet->message_data.back() == '.' || application_packet->message_data.back() == '!' || application_packet->message_data.back() == '?') {
                                clients[j].log_entries.push_back(Log(to_string(time(nullptr)), message, frame, physical_packet->hop_count, application_packet->sender_ID, application_packet->receiver_ID, false, ActivityType::MESSAGE_DROPPED));
                                cout << "--------" << endl;
                                message = "";
                                frame = 0;
                            }
                            delete clients[j].incoming_queue.front().top();
                            clients[j].incoming_queue.front().pop();
                            delete clients[j].incoming_queue.front().top();
                            clients[j].incoming_queue.front().pop();
                            delete clients[j].incoming_queue.front().top();
                            clients[j].incoming_queue.front().pop();
                            delete clients[j].incoming_queue.front().top();
                            clients[j].incoming_queue.front().pop();
                            clients[j].incoming_queue.pop();
                        }
                    }
                }
            }
        }
        else if (word == "PRINT_LOG") {
            ss >> word;
            int j = 0;
            while (j < clients.size()) {
                if (clients[j].client_id == word) {
                    break;
                }
                j++;
            }
            if (!clients[j].log_entries.empty()) {
                cout << "Client " << clients[j].client_id << " Logs:" << endl;
            }
            for (int k = 0; k < clients[j].log_entries.size(); ++k) {
                string success = "";
                if (clients[j].log_entries[k].success_status) {
                    success = "Yes";
                }
                else {
                    success = "No";
                }

                string activity = "";
                if (clients[j].log_entries[k].activity_type == ActivityType::MESSAGE_SENT) {
                    activity = "Message Sent";
                }
                else if (clients[j].log_entries[k].activity_type == ActivityType::MESSAGE_RECEIVED) {
                    activity = "Message Received";
                }
                else if (clients[j].log_entries[k].activity_type == ActivityType::MESSAGE_FORWARDED) {
                    activity = "Message Forwarded";
                }
                else {
                    activity = "Message Dropped";
                }

                cout << "--------------" << endl;
                cout << "Log Entry #" << k + 1 << ":" << endl;
                cout << "Activity: " << activity << endl;
                cout << "Timestamp: " << clients[j].log_entries[k].timestamp << endl;
                cout << "Number of frames: " << clients[j].log_entries[k].number_of_frames << endl;
                cout << "Number of hops: " << clients[j].log_entries[k].number_of_hops << endl;
                cout << "Sender ID: " << clients[j].log_entries[k].sender_id << endl;
                cout << "Receiver ID: " << clients[j].log_entries[k].receiver_id << endl;
                cout << "Success: " << success << endl;
                if (clients[j].log_entries[k].activity_type == ActivityType::MESSAGE_SENT || clients[j].log_entries[k].activity_type == ActivityType::MESSAGE_RECEIVED) {
                    cout << "Message: \"" << clients[j].log_entries[k].message_content << "\"" << endl;
                }
            }
        }
        else {
            cout << "Invalid command." << endl;
        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        istringstream ss(line);

        string id;
        string ip;
        string mac;

        ss >> id;
        ss >> ip;
        ss >> mac;

        Client client(id, ip, mac);

        clients.push_back(client);
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream file(filename);
    std::string line;
    int client_index = 0;

    while (std::getline(file, line)) {
        if (line[0] == '-') {
            client_index++;
            continue;
        }
        istringstream ss(line);

        string id1;
        ss >> id1;
        string id2;
        ss >> id2;

        clients[client_index].routing_table[id1] = id2;
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        commands.push_back(line);
    }

    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}
