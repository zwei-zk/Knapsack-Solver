/*
Junhyuk Lee
jl10324
Knapsack Hill Climbing
2/13/2022
*/

#include <bits/stdc++.h>

using namespace std;

// Parse the first line (target value and max weight) of the input file 
vector<float> readTargets(string inputFile){
	ifstream inputStream;
	inputStream.open(inputFile);
	float tv, mw; // Target value, max weight
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

// Create and return a vector of a vector of neighbor states for the given input state 
vector<vector<int>> generateNeighbors(vector<int> state, vector<string> objectNames){
	vector<int> outOfState; // Objects that are out of the current state
	vector<vector<int>> neighbors;
	unordered_map<int, int> included_objects;
	for (int object : state){
		included_objects[object]++;
	}
	for (int i = 0; i < objectNames.size(); i++){
		if (included_objects[i] == 0){
			outOfState.push_back(i);
		}
	}
	// an object in state is removed
	for (int i = 0; i < state.size(); i++){
		vector<int> neighbor;
		for (int j = 0; j < state.size(); j++){
			if (j == i){
				continue;
			}
			neighbor.push_back(state[j]);
		}
		neighbors.push_back(neighbor);
	}
	// an object out of state is added to the state
	for (int i = 0; i < outOfState.size(); i++){
		vector<int> neighbor = state;
		neighbor.push_back(outOfState[i]);
		neighbors.push_back(neighbor);
	}
	// an object in state is swapped with an object out of state
	for (int j = 0; j < outOfState.size(); j++){
		vector<int> neighbor = state;
		for (int i = 0; i < neighbor.size(); i++){
			neighbor = state;
			neighbor[i] = outOfState[j];
			neighbors.push_back(neighbor);
		}
	}
	return neighbors;
}

// Find the state with the minimum error state and return its index
int selectBestState(vector<float> errorStates, float currError){
	float minError = INT_MAX;
	for (int i = 0; i < errorStates.size(); i++){
		minError = min(minError, errorStates[i]);
	}
	if (minError >= currError){
		return -1;
	}
	int bestState;
	for (int i = 0; i < errorStates.size(); i++){
		if (errorStates[i] == minError){
			bestState = i;
			break;
		}
	}
	return bestState;
}

// Calculate and return the error state of the given input state  
float calculateError(float totalValue, float totalWeight, float targetValue, float maxWeight){
	return max(totalWeight - maxWeight, (float)0) + max(targetValue - totalValue, (float)0);
}

// Create and return a vector of error states of the given neighbor states
vector<float> generateErrors(vector<vector<int>> neighbors, vector<string> objectNames, vector<float> objectValues, vector<float> objectWeights, float targetValue, float maxWeight){
	vector<float> errorStates;
	for (vector<int> neighbor : neighbors){
		float totalValue = 0, totalWeight = 0;
		cout << "Neighbor State:\n";
		for (int object : neighbor){
			cout << objectNames[object] << " ";
			totalValue += objectValues[object];	// Calculate the total value and the total weight of each neighor state
			totalWeight += objectWeights[object];
		}
		cout << "\nValue: " << totalValue << " Weight: " << totalWeight << " Error: ";
		int errorState = calculateError(totalValue, totalWeight, targetValue, maxWeight); // Calculate the error state from the total value and the total weight of each neighbor state 
		cout << errorState << '\n';
		errorStates.push_back(errorState);
	}
	return errorStates;
}

// Simulate a random restart by adding the objects to a state with a 50/50 chance 
vector<int> randomRestart(vector<string> objectNames){
	vector<int> startState;
	for (int i = 0; i < objectNames.size(); i++){
		if (rand() % 2 == 1){ // Generates either 1 or 0
			startState.push_back(i); // If generated value is 1, add to the start state
		}
	}
	return startState;
}

// Parse all the data to create the vector of the next state 
vector<int> processData(vector<int> currentState, vector<string> objectNames, vector<float> objectValues, vector<float> objectWeights, float targetValue, float maxWeight){
	vector<vector<int>> NeighborStates = generateNeighbors(currentState, objectNames);
	int totalValue = 0, totalWeight = 0;
	for (int i = 0; i < currentState.size(); i++){
		totalValue += objectValues[currentState[i]];
		totalWeight += objectWeights[currentState[i]];
	}
	cout << "Value: " << totalValue << " Weight: " << totalWeight;
	int currError = calculateError(totalValue, totalWeight, targetValue, maxWeight);
	cout << " Error: " << currError << "\n\n";
	vector<float> ErrorStates = generateErrors(NeighborStates, objectNames, objectValues, objectWeights, targetValue, maxWeight);
	int nextStateInd = selectBestState(ErrorStates, currError);
	if (nextStateInd == -1){ // When the best error state of the neighbor states is greater than the current error state, we know the search has failed
		return {-1}; // Indicate that by only including -1 in the state array 
	}
	vector<int> nextState = NeighborStates[nextStateInd];
	if (ErrorStates[nextStateInd] == 0){ // When the best error state of the neighbor states is 0, we know a valid state is found
		nextState.push_back(-10); // Indicate that by including a -10 at the end of the state array 
	}
	if (nextState[nextState.size() - 1] != -10){
		cout << "\nNext State:\n";
		for (int object : nextState){
			cout << objectNames[object] << " ";
		}
		cout << '\n';
	}
	return nextState;
}

// Combine all the functions together to simulate hill climbing 
vector<int> hillClimbing(vector<string> objectNames, vector<float> objectValues, vector<float> objectWeights, float targetValue, float maxWeight){
	vector<int> validState;
	for (int i = 1; i <= 11; i++){ // Loop runs 11 times to also account for the starting state 
		if (i != 1){
			cout << "\nRandom Restart: " << i - 1 << '\n';
		}
		vector<int> currentState = randomRestart(objectNames);
		cout << "Starting State:\n";
		for (int object : currentState){
			cout << objectNames[object] << " ";
		}
		cout << '\n';
		while (currentState[0] != -1 && currentState[currentState.size() - 1] != -10){ // While search is still ongoing and the valid state has yet to be found
			currentState = processData(currentState, objectNames, objectValues, objectWeights, targetValue, maxWeight);
		}
		if (currentState[currentState.size() - 1] == -10){
			for (int j = 0; j < currentState.size() - 1; j++){ // Exclude the -10 that we added to indicate the valid state
				validState.push_back(currentState[j]);
			}
			return validState;
		}
		cout << "\nSearch Failed\n";
	}
	return {}; // Return an empty set when there is no valid state 
}

int main(){
	// For storing various data
	vector<string> objectNames;
	vector<float> objectValues, objectWeights;
	float targetValue, maxWeight;

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
	srand(time(NULL)); // Generate new seed for the random function
	cout << "Objects: ";
	for (int i = 0; i < objectNames.size(); i++){
		cout << objectNames[i] << " ";
	}
	cout << '\n';
	cout << "Target Value: " << targetValue << " Max Weight: " << maxWeight << "\n\n";
	vector<int> validState = hillClimbing(objectNames, objectValues, objectWeights, targetValue, maxWeight);
	if (validState.size() == 0){ // If the returned valid state is empty, there is no solution 
		cout << '\n';
		cout << "No Solution";
	}
	else{
		cout << "\nValid State:\n";
		for (int i = 0; i < validState.size(); i++){
			cout << objectNames[validState[i]] << " ";
		}
	}
}