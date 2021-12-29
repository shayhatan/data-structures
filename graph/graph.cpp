//
// Created by USER on 21/12/2021.
//

#include "graph.h"

#include <utility>

using std::list;
using std::string;
using std::vector;

using std::cout;
using std::endl;
using std::cin;

struct Graph::Edge {
    int dest_id;
    int weight;

    Edge() = default;

    Edge(int d_id, int w) : dest_id(d_id), weight(w) {}

    void setEdgeValues(int d_id, int w) {
        dest_id = d_id;
        weight = w;
    }

    void setWeight(int w) {
        weight = w;
    }

    int getDestinationVertexID() const {
        return dest_id;
    }

    int getWeight() const {
        return weight;
    }

};

struct Graph::Vertex {
    int state_id;
    string state_name;

    list<Edge> edge_list;

    explicit Vertex(int id = 0, string name = "") : state_id(id), state_name(std::move(name)) {}

    int getStateID() const {
        return state_id;
    }

    string getStateName() const {
        return state_name;
    }

    void setID(int id) {
        state_id = id;
    }

    void setStateName(string name) {
        state_name = std::move(name);
    }

    list<Edge> getEdgeList() const {
        return edge_list;
    }
};

void Graph::addVertex(int id, const string &name) {
    validVertex(id);
    vertices.emplace_back(id, name);
}

void Graph::validVertex(int id) {
    for (auto &vertex : vertices) {
        if (vertex.getStateID() == id) {
            throw VertexAlreadyExists{};
        }
    }
}

void Graph::printMe() {
    for (auto &vertex : vertices) {
        cout << vertex.getStateName();
        cout << " -> list <id, name>: ";
        for (auto &edge : vertex.edge_list) {
            cout << "(" << edge.getDestinationVertexID() << ", " << edge.getWeight() << ") ";
        }
        cout << endl;
    }
}

void Graph::addEdge(int id1, int id2, int weight) {
    edgeValid(id1, id2, weight);
    edgeValid(id2, id1, weight);
}

void Graph::edgeValid(int id1, int id2, const int weight) {
    for (auto &vertex : vertices) {
        if (vertex.getStateID() == id1) {
            for (auto &edge : vertex.edge_list) {
                if (edge.dest_id == id2) {
                    throw EdgeAlreadyExists{};
                }
            }
            vertex.edge_list.emplace_back(id2, weight);
            return;
        }
    }
    throw VertexDoesNotExists{};
}

void Graph::deleteVertex(int _id) {

    int v_index = 0;
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].getStateID() == _id) {
            v_index = i;
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        for (auto it = vertices[i].edge_list.begin(); it != vertices.at(i).edge_list.end(); it++) {
            if (it->getDestinationVertexID() == _id) {
                vertices[i].edge_list.erase(it);
                break;
            }
        }
    }
    vertices.erase(vertices.begin() + v_index);
}

void Graph::deleteEdge(int id1, int id2) {
    if (deleteOneEdge(id1, id2)) {
        deleteEdge(id2, id1);
    }
}

bool Graph::deleteOneEdge(int id1, int id2) {
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].getStateID() == id1) {
            for (auto it = vertices[i].edge_list.begin(); it != vertices[i].edge_list.end(); it++) {
                if (it->getDestinationVertexID() == id2) {
                    vertices[i].edge_list.erase(it);
                    return true;
                }
            }
        }
    }
    return false;
}

int main() {
    Graph g;
    string name1 = "Shay";
    int id1 = 1;
    int id2 = 2;
    string name2 = "Yossi";
    int w = 4;
    g.addVertex(id1, name1);
    g.addVertex(id2, name2);
    g.addEdge(id1, id2, 4);
    g.deleteVertex(1);
    g.printMe();
    return 0;
}