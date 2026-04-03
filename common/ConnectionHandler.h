#pragma once


#include <QObject>


class ConnectionHandler {
    public:
        std::unordered_map<int, QMetaObject::Connection> connections;
        
        bool addConnection(int role, const QMetaObject::Connection& conn) {
            connections[role] = conn;  // Don't move, just copy
            return true;
        }
    
        void disconnect(int role) {
            auto it = connections.find(role);
            if (it != connections.end() && it->second) {
                QObject::disconnect(it->second);
                connections.erase(it);
            }
        }
    
        void disconnectAll() {
            // Safe approach: iterate over copy
            for (const auto& [role, con] : connections) {
                if (con) {
                    QObject::disconnect(con);
                }
            }
            connections.clear();  // Clear after all disconnections
        }
    };