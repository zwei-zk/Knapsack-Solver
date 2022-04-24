/*
Junhyuk Lee
jl10324
Knapsack Iterative Deepening
2/13/2022
*/

#include <bits/stdc++.h>

using namespace std;

// For storing various data
vector<string> objectNames;
vector<float> objectValues, objectWeights;
float targetValue, maxWeight;
vector<vector<int>> result;
bool found = false;

// Parse the first line (target value and max weight) of the input file 
vector<float> readTargets(string inputFile){
	ifstream inputStream;
	inputStream.open(inputFile);
	float tv, mw; // target value, maximum weight
	inputStream >> tv >> mw;
	inputStream.close();
	return {tv, mw};
}

// Parse the rest of the lines (object name, object value, and object weight) of the input file 
vector<vector<string>> readInput(string inputFile){
	vector<vector<string>> inputs;
	ifstream inputStream;
	inputStream.open(inputFile);
	inputStream.ignore(numeric_limits<streamsize>::max(), '\n');
	string n, v, w; // Object name, value, and weight
	while (inputStream >> n >> v >> w){
		inputs.push_back({n, v, w});
	}
	inputStream.close();
	return inputs;
}

// Perform a depth-first search to find a valid state until the given max depth 
void DFS(float totalValue, float totalWeight, vector<int> currState, int currObject, int maxDepth){
	cout << "Value: " << totalValue << " Weight: " << totalWeight << " " << "Added Object: " << objectNames[currObject] << '\n';
	for (int i = 0; i < currState.size(); i++){
		cout << objectNames[currState[i]] << " ";
	}
	cout << '\n';
	if (totalValue >= targetValue && totalWeight <= maxWeight){ // Valid state has been found and is added to the result vector
		result.push_back(currState);
		found = true;
		return;
	}
	if (found || currState.size() == maxDepth){ // Stop searching into further depths once we reach the max depth or a valid state has been found
		return;
	}
	for (int i = 0; i < objectNames.size(); i++){
		if (currObject >= i){
			continue;
		}
		currState.push_back(i);
		if (totalWeight + objectWeights[i] <= maxWeight){ // Make sure that the state we reach does not exceed the allotted max weight
			DFS(totalValue + objectValues[i], totalWeight + objectWeights[i], currState, i, maxDepth); // Explore further if able to 
		}
		currState.pop_back(); // Backtrack to a previous state to further explore 
	}
}

// Simulate Iterative Deepening by applying DFS with max depths ranging from 1 to the number of objects 
void IDS(int depth){
	for (int i = 1; i <= depth; i++){
		bool complete = false;
		cout << "\nCurrent Max Depth: " << i << '\n';
		vector<int> currState;
		DFS(0, 0, currState, -1, i); // Perform DFS with the max depth of i
		if (result.size() == 0){ // Empty result vector indicates that there is no valid state
			if (i != depth){
				continue;
			}
			else{
				cout << "No Solution";
			}
		}
		else{
			cout << "\nValid State:\n";
			for (int j = 0; j < result[0].size(); j++){ // If the result vector is not empty, we know there is a valid state and print its content
				cout << objectNames[result[0][j]] << " ";
			}
			complete = true;
		}
		cout << '\n';
		if (complete){
			return; // Once the answer is found, we don't need to look into further depths
		}
	}
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// Read inputs from the input file 
	vector<float> targets = readTargets("input.txt");
	vector<vector<string>> lines = readInput("input.txt");

	// Assign values appropriately 
	targetValue = targets[0];
	maxWeight = targets[1];
	for (int i = 0; i < lines.size(); i++){
		objectNames.push_back(lines[i][0]);
		objectValues.push_back(stof(lines[i][1]));
		objectWeights.push_back(stof(lines[i][2]));
	}
	cout << "Target Value: " << targetValue << " Max Weight: " << maxWeight; 
	int depth = objectNames.size(); // Maximum depth is the number of objects
	cout << " Max Depth: " << depth << "\n\n";
	IDS(depth); // Run iterative deepening with the maximum depth from above 
}