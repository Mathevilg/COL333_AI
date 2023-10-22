#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <iomanip>
// #include<bits/stdc++.h>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;
class A4{
	private:
	vector<vector<float> > CPT;
	time_t initTime;
	string network_file, data_file;
	map<string, int> mp;
	vector<vector<int> > parents, child;
	vector<string> names; // redundant 
	vector<vector<string> > possibleValues;
	vector<vector<int> > originalData, intermediateData;
	vector<float> weights; // weights for the intermediate data columns !!
	vector<int> missingValues; // the missing variables for data rows !!


	void showDependency(){
		for (int i=0; i < 37; i++){
			cout << names[i] << "  "<< possibleValues[i].size() << "  ----> ";
			for (auto it : parents[i]) cout << names[it] << " " << possibleValues[it].size() << "   ";
			cout << endl << CPT[i].size() << endl;
		}
	}


	void showCPT(){

		for (auto it: CPT){
			for (auto tt: it) cout << tt << " ";
			cout << endl;
		}
	}

	public:


	A4(string network_file, string data_file, time_t initTime){
		this->network_file = network_file;
		this->data_file = data_file;
		this->initTime = initTime + (time_t)(0.01); // THIS NEEDS TO BE CHECKED!
		parents.resize(37);
		child.resize(37);
		names.resize(37);
		possibleValues.resize(37);
		CPT.resize(37);
		mp.clear();
		readNetwork();
		readDataFile();
		solve();
	}


	void readDataFile(){
		string line, name, temp;
		ifstream file(data_file);
		if (!file.is_open()) {
			cout << "error opening data file " << data_file << endl;
			return ;
		}
		while (!file.eof()){

			stringstream stream;
			getline(file, line);
			stream.str(line); // "False" "Normal" "False" "Normal" "Normal" "Normal" "False" "Normal" "Normal" "False" "?" "False" "Normal" "False" "High" "False" "High" "High" "Low" "High" "High" "Low" "False" "False" "Normal" "False" "Normal" "High" "Normal" "Normal" "Normal" "Normal" "Normal" "Normal" "Normal" "Normal" "Normal"
			vector<int> values;
			int questionMarkIdx = -1;
			for(int i=0; i<mp.size(); i++){

				stream >> temp; // one by one values of the variables !!
				if (temp.compare("\"?\"") == 0) {
					questionMarkIdx = i;
					values.push_back(-1);
					continue;
				}

				// for actual values find their index in possible values table !
				for (int j = 0; j<possibleValues[i].size(); j++){
					if (temp.compare(possibleValues[i][j]) == 0){
						values.push_back(j);
						break;
					}
				}

			}
			originalData.push_back(values);
			missingValues.push_back(questionMarkIdx);
			if (questionMarkIdx == -1) {
				// all good the current row is the only interpretable row 
				intermediateData.push_back(values);
				weights.push_back(1.0);
			}
			else {
				// have to asssign weights to all possible values of the "?"
				int combinations = possibleValues[questionMarkIdx].size();
				float weight = 1.0 / combinations;
				vector<int> created_values(values);
				for (int j = 0; j < combinations; j++){
					created_values[questionMarkIdx] = j;
					intermediateData.push_back(created_values);
					weights.push_back(weight);
				}
			}

		}

		// cout << originalData.size() << endl;
		// cout << intermediateData.size() << endl;

		file.close();
	}



	void readNetwork(){
		// used ChatGPT  here !! 
		// have to be removed / modified 
		ifstream file(network_file);
		if (!file.is_open()) {
			cout << "error opening " << network_file << endl;
			return ;
		}

		while (!file.eof()){

			string line, temp, var_name;
			getline(file, line);
			stringstream stream;
			stream.str(line);
			stream >> temp;
			if (temp.compare("probability") == 0){
				stream >> temp; // simply '(' 
				stream >> temp; // the variable name 
				int current_id = mp.find(temp)->second; // the node number in the mapping
				parents[current_id].clear(); // clear the parents vector of the node
											// parents will be assigned now
				// cout << "parents of node " << temp << " ";
				stream >> temp; // parents 
				while(true){
					if (temp.compare(")") == 0) break;
					// cout << temp << " ";
					int parentIdx = mp.find(temp)->second;
					child[parentIdx].push_back(current_id);
					parents[current_id].push_back(parentIdx);
					stream >> temp;
				}
				// cout << endl;
				getline(file, line); // of the form -> table -1 -1 ;
				stringstream stream2;
				stream2.str(line);
				stream2 >> temp;
				stream2 >> temp;
				while(true){
					if (temp.compare(";") == 0) break;
					CPT[current_id].push_back(atof(temp.c_str()));
					stream2 >> temp;
				}
			} else if (temp.compare("variable") == 0){
				stream >> var_name;
				mp[var_name] = mp.size() ;
				names[mp.size()-1] = var_name;
				getline(file, line); // line with format-> type discrete[3] {  "Low"  "Normal"  "High" };
				stringstream stream2;
				stream2.str(line);
				for(int i=0; i < 3; i++) stream2 >> temp;
				possibleValues[mp.size()-1].clear();
				while(true){
					stream2 >> temp;
					if (temp.compare("};") == 0) break;
					possibleValues[mp.size()-1].push_back(temp);
				}
			}
		}

		// for (auto vec: possibleValues){
		// 	for (auto str: vec) cout << str << " ";
		// 	cout << endl;
		// }

		file.close();

	}


	void CPTInitialiser(){
		// assume equiprobable !!
		for (int i=0; i<CPT.size(); i++){
			float prob = 1.0 / possibleValues[i].size();
			for (int j=0; j < CPT[i].size(); j++) {
				CPT[i][j] = prob;
			}
		}
	}

	void solve(int process_time = 5){ // running for 5 seconds by default 
		// since reading and writing could be bottleneck use intemediate
		// data structures to represent records.dat and solved_alarm.bif 


		// showDependency();
		// showCPT(); 
		CPTInitialiser();
		dataUpdater();
		// showCPT();

		float time_to_write = 0.0 ; // expected time to write in solved_alarm.bif
		while (time(NULL) - initTime + time_to_write < process_time){ // this should be time not exceeded 
			// 1. learn the CPT from intermediat data structure 
			CPTUpdater();
			// 2. update the values to intermediate data structure using inference from the CPT learnt
			dataUpdater();
		}

		// wirte to `solved_alarm.bif`
		writeData();
		// cout << initTime << endl;
		// cout << time(NULL) << endl;

	}



	void CPTUpdater(){
		// uses values given in the intermediate data structure to learn the values
		// for the CPT

		// learning algorithm ?? 


		for (int i = 0; i< CPT.size(); i++){
			CPT[i] = vector<float>(CPT[i].size(), 1.0);
			vector<int> ids, Sizes;
			ids.push_back(i);
			Sizes.push_back(possibleValues[i].size());
			for (int j = 0; j < parents[i].size(); j++){
				Sizes.push_back(possibleValues[parents[i][j]].size());
				ids.push_back(parents[i][j]);
			}
			int len = CPT[i].size() / possibleValues[i].size();
			vector<float> sumParts(len, possibleValues[i].size());
			for (int j = 0; j < intermediateData.size(); j++){
				vector<int> values;
				for (int k = 0; k < ids.size(); k++){
					values.push_back(intermediateData[j][ids[k]]);
				}

				int data_index = 0, temp = 1;
				for (int k = values.size()-1; k >= 0; k--){
					data_index += temp * values[k];
					temp *= Sizes[k];
				}
				CPT[i][data_index] += weights[j];
				sumParts[data_index%len] += weights[j];
			}

			for(int j = 0; j < CPT[i].size(); j++){
				if (sumParts[ j % len] == 0) CPT[i][j] = 0.0;
				else CPT[i][j] /= sumParts[j % len]; 
			}
		}
	}


	float calculateProbability(int dataRow, int variable, int state){
		vector<int> values, Sizes;
		// values store the value of EVIDENCE & variable
		// Sizes store the corrsponding size of possible values 
		values.push_back(state);
		Sizes.push_back(possibleValues[variable].size());
		for (int i = 0; i < parents[variable].size(); i++) {
			values.push_back(originalData[dataRow][parents[variable][i]]);
			Sizes.push_back(possibleValues[parents[variable][i]].size());
		}
		int idx = 0, temp = 1;
		for (int i = values.size()-1; i >= 0; i--){
			idx += temp * values[i];
			temp *= Sizes[i];
		}
		return CPT[variable][idx];
	}

	void dataUpdater(){
		// using the intermediate CPT values modifies the data in the intermediate 
		// data structure 

		// USES SOFT METHOD !!
		// Inference algorithm ?? 

		// assign weights to the Intermediate data values ;
		weights.clear();
		for (int dataRow = 0; dataRow < originalData.size(); dataRow++){
			if (missingValues[dataRow] == -1) weights.push_back(1.0);
			else{
				int variable = missingValues[dataRow];
				vector<float> probabilities ;
				float probab_sum = 0.0;
				for (int j = 0; j < possibleValues[variable].size(); j++){
					// calculates P(variable = j | conditions in that data row);
					// will pe problematic for the first time since in the originalData 
					// the value of missing variables are -1 !!
					float probability_of_value_given_evidence = calculateProbability(dataRow, variable, j); // j is the value
					probabilities.push_back(probability_of_value_given_evidence);
					probab_sum += probability_of_value_given_evidence;
					///////////////////////////////////////////////////////////////
					// HAVE A GUT FEELING THAT THIS COULD BE IMPROVED FURTHER !! //
					///////////////////////////////////////////////////////////////
				}
				int argmaxValue = max_element(probabilities.begin(), probabilities.end())-probabilities.begin();
				originalData[dataRow][variable] = argmaxValue;
				float alpha = (1.0)/probab_sum;
				for (int i=0; i < probabilities.size(); i++) weights.push_back(alpha * probabilities[i]);
			}
		}

	}

	void writeData(){
		// write the CPT in solved_alarm.bif
		ifstream input(network_file);
		ofstream out;
		out.open("solved_alarm.bif");
		if (!input.is_open()) return ;
		while (!input.eof()){
			string line, temp, var_name;
			getline(input, line);
			stringstream stream;
			stream.str(line);
			stream >> temp;
			if (temp.compare("probability") == 0){
				stream >> temp; // simply '(' 
				stream >> temp; // the variable name 
				int current_id = mp.find(temp)->second; // the node number in the mapping
				out << line << endl;
				getline(input, line);
				out << "\ttable ";
				for (int i=0; i<CPT[current_id].size(); i++) out << fixed << setprecision(4) << max((float)0.0001, CPT[current_id][i]) << " ";
				out << ";" << endl;
			}
			else if (temp.compare("") == 0) out << line; //THIS LINE ALONE FUCKED IT ALL !!
			else out << line << endl; // have to check for output conditions here !!

		}
		out.close();
		input.close();

		cout << "written to solved_alarm.bif sucessfully" << endl;
	}
};


int main(int argc, char* argv[])
{

	time_t initTime = time(NULL);
	string network_file = argv[1];
	string data_file = argv[2];
	A4* ans = new A4(network_file, data_file, initTime);

	return 0; 
	
}





