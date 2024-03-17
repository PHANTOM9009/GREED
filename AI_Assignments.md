# Assignments for Artificial Intelligence using Simulations

## 1. Uninformed Search/Blind Search

### a. Problem Statement:

You have to code a DFS algorithm for your battleship to find treasures in the seas. Once you start to collect these treasures, more will appear. You need to write a bona-fide code to check if the treasure has appeared anywhere by using the function `getTargetCoords()`, which will return the coordinates of the treasure. Then, write a depth-first search algorithm to find the path to those coordinates. You don’t have to use any kind of heuristics in your algorithm, just keep it a blind search. Your health and fuel are unlimited.

### Concepts Learned:

Uninformed search, DFS search space, how blind DFS is time-consuming and explores too many states.

### b. Implement the above problem by using Breadth First Search (BFS) algorithm.

After executing these tasks, you can compare the time needed to take a decision by using uninformed searches. These kinds of searches are not feasible in a real-life scenario. Imagine a self-driving car searching all the actions possible without considering the safety of the driver and others!

## 2. Informed Searches: Use of Heuristics

### a. Problem Statement:

Use a heuristic to measure the distance between your current position and the target position, and instead of expanding all the nodes of your tree, just expand the relevant nodes. Apply this in your previously written DFS algorithm. This time your battleship will have just enough fuel to reach the treasure, assuming you are using an optimized approach.

### b. Problem Statement:

Use the same heuristic approach but with BFS algorithm. You might find that using DFS algorithm not all test cases get passed, but BFS can pass all the test cases. Think about the reason. (HINT: the answer lies in the way DFS expands its child nodes)

### c. Problem Statement:

Try to use different heuristics for distance:
- Manhattan distance
- Euclidean distance
- Minkowski distance.

### Concepts Learned:

Difference between BFS and DFS in various scenarios, informed searches with the help of heuristics.

## 3. A* Algorithm

### a. Problem Statement:

Implement A* algorithm using different distance heuristics trying which one is better. Now your ship can move in 8 different directions.

### b. Problem Statement:

Using the A* algorithm increase the cost of the coordinates which are in the radius of cannons so that your ship’s health does not reduce while your battleship is out searching for treasures.

Observe how A* expands even fewer states than previously used algorithms.

## 4. Introducing Warfare Techniques: Classical AI Algorithms

### Concepts Learned:

Min-max, alpha-beta pruning.

In this section, we are introducing warfare strategies that can be used to defeat adversary battleships.

### a. Problem Statement:

You are in the middle of a battle with another pirate battleship, you need to decide between fight and flight, for that you need to decide whether you can take on that ship or not. Write min-max algorithm for finding whether you can defeat the battleship or not.

### b. Problem Statement:

You must have found by now that min-max takes too much time even for health values like 100, apply alpha-beta pruning to save some search space and time.

### c. Problem Statement:

You are given 5 static battleships, you have to defeat all of them to emerge as the master of the seas, every time you defeat a battleship you receive its resources, you have to choose an order in which you have to face them. Use min-max with alpha-beta pruning to sort the enemy battleships according to your strength to defeat them.

## 5. Moving up a Notch: Monte Carlo Tree Search

In this section, we will create better warfare agents by using Monte Carlo Tree Search. Min-max works better for games which are deterministic like chess, tic-tac-toe etc. but for the games which are not deterministic like this one, min-max fails to give the accurate answer as you may have seen in the previous lesson. What we need is an algorithm which returns the probability of actions. This ensures that our algorithm has a stochastic nature and works in less search space and time.

### Problem Statement:

You are in the seas notorious for pirates fighting for more gold, defeat all the battleships present in the arena to emerge as the supreme pirate. Use Monte Carlo tree search to make optimal decisions.

## 6. Introducing Genetic Algorithms

Search trees like we have seen previously explore all the possible combinations which is time taking. Genetic algorithms are known for converging to an optimal solution in limited time and iterations.

### Problem Statement:

Use genetic algorithms to evolve your policies to defeat all the ships present in the seas. Define a fitness score to rank the policies and use crossover and mutation to make new policies.

## 7. Reinforcement Learning: Q Learning

So far, we have seen classical techniques to solve this environment, classical approaches are effective but they may miss finer details of the environment due to programming errors and bad assumptions. Here we will use a tabular form of Q learning.

### Problem Statement:

Use Q learning with hash tables, find Q values for each state-action pair, use epsilon-greedy algorithm to trade between exploration and exploitation. The end goal is to defeat all other bots present in the arena.

## 8. Reinforcement Learning: Deep Q Learning

Hash tables created for each state-action pair can overwhelm the space quickly where state space is very large, so in order to train an effective model we will train a neural network to approximate the Q learning function.

### Problem Statement:

Train a neural network which takes in states and outputs the Q values of every action in a vector. Use gradient descent to reduce the error in the network. You have to approximate the Q learning function using this neural network. The end goal is to defeat all other bots present in the arena.
