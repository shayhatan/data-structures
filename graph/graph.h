//
// Created by USER on 21/12/2021.
//

#ifndef DATA_STRUCTURES_GRAPH_H
#define DATA_STRUCTURES_GRAPH_H

/*
               * This Graph would represent network of connected states
               * Undirected & Weighted
                                                                                                 */
#include<iostream>
#include <vector>
#include <list>
#include <iterator>
#include <string>
#include <exception>


class Graph {
    struct Edge;

    struct Vertex;

    std::vector<Vertex> vertices;
public:

    Graph() = default;

    ~Graph() = default;

    void addVertex(int id, const std::string &name);

    void addEdge(int id1, int id2, int weight);

    void printMe();

    void deleteVertex(int _id);

    void deleteEdge(int id1, int id2);

    class EdgeAlreadyExists {};

    class VertexAlreadyExists {};

    class VertexDoesNotExists{};

private:
    void validVertex(int id);

    void edgeValid(int id1, int id2, int weight);

    bool deleteOneEdge(int id1, int id2);
};


#endif //DATA_STRUCTURES_GRAPH_H
