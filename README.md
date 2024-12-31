# Public transpotation System

This project is a C++ implementation of a city  Public transpotation System. It models a city as a graph where nodes represent locations and edges represent paths with weighted attributes: distance, traffic, and red lights. The system calculates all possible paths between two locations and identifies the most convenient path based on a custom convenience score.

---

## Features

- **Graph Representation**: Nodes (locations) and edges (paths) with weights for distance, traffic, and red lights.
- **Pathfinding**: Uses Depth-First Search (DFS) to find all paths between two nodes.
- **Convenience Score**: A custom scoring system evaluates paths based on distance, traffic, and red lights.
- **Visualization**: Displays the city graph, all paths, and the most convenient path.

---

## How It Works

1. **City Model**: The city is modeled as a directed graph.
2. **Pathfinding**: All paths between a start node and an end node are determined using a recursive DFS algorithm.
3. **Scoring**: A custom formula scores paths, prioritizing:
   - Shorter distance
   - Fewer traffic delays
   - Minimal red lights
4. **Output**:
   - A list of all possible paths.
   - The most convenient path, based on the lowest score.

---

## Setup Instructions

1. **Prerequisites**:
   - A C++ compiler ( GCC)
   - A text editor or IDE for C++ development ( Visual Studio Code).

2. **Compilation**:
   Compile the code using a C++ compiler. For example:
   ```bash
   cd clone https://github.com/Geekyash10/Public_Transportation_System.git
   g++ main.cpp -o main.cpp


