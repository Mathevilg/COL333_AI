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
#include <chrono>
#include <random>

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;
#define float double

class Time{
	private:
		chrono::high_resolution_clock::time_point init_time;
		string line;
		int flag; // 1 means ms, 2 means µs, and 3 means ns
	
	
	public:
		Time(){
			init_time = chrono::high_resolution_clock::now();
		}

		void showTime(string line, int flag = 2){
			std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
			cout << line << "\t---->\t" ;
			switch (flag)
			{
				case 1:
					{
						chrono::milliseconds d1 = chrono::duration_cast<chrono::milliseconds>(current_time - init_time);
						cout << d1.count() << " ms";
						break;
					}
				case 2:
					{
						chrono::microseconds d2 = chrono::duration_cast<chrono::microseconds>(current_time - init_time);
						cout << d2.count() << " µs";
						break;
					}
				case 3:
					{
						chrono::nanoseconds d3 = chrono::duration_cast<chrono::nanoseconds>(current_time - init_time);
						cout << d3.count() << " ns";
						break;
					}
				default:
					cout << endl;
			}
			cout << endl;
		}


		long long getTime(int flag = 1){
			std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
			switch (flag)
			{
				case 1:
					{
						chrono::milliseconds d1 = chrono::duration_cast<chrono::milliseconds>(current_time - init_time);
						return  d1.count();
					}
				case 2:
					{
						chrono::microseconds d2 = chrono::duration_cast<chrono::microseconds>(current_time - init_time);
						return d2.count();
					}
				case 3:
					{
						chrono::nanoseconds d3 = chrono::duration_cast<chrono::nanoseconds>(current_time - init_time);
						return d3.count();
					}
				default:
					return 0;
			}
		}

};


class A4{
	private:
	vector<vector<float> > CPT;
	Time* initTime;
	float processTime, smoothingFactor;
	string network_file, data_file;
	map<string, int> mp;
	vector<vector<int> > parents, child;
	vector<string> names; // redundant 
	vector<vector<string> > possibleValues;
	vector<vector<int> > originalData, intermediateData;
	vector<float> weights; // weights for the intermediate data columns !!
	vector<int> missingValues; // the missing variables for data rows !!
	

	void showDependency(){
		int ans = 0;
		for (int i=0; i < 37; i++){
			// cout << names[i] << "  "<< possibleValues[i].size() << "  ----> ";
			// for (auto it : parents[i]) cout << names[it] << " " << possibleValues[it].size() << "   ";
			// cout << endl << CPT[i].size() << endl;
			ans += CPT[i].size();
		}
		cout << ans << endl;
	}


	void showCPT(){

		for (int i=0; i<37; i++){
			vector<float> it = CPT[i];
			for (int i=0; i<it.size(); i++) cout << it[i] << " ";
			cout << endl;
		}
	}

	public:


	A4(string network_file, string data_file, Time* initTime){
		this->network_file = network_file;
		this->data_file = data_file;
		this->initTime = initTime;
		this->processTime = 5; // 2 mins 
		parents.resize(37);
		child.resize(37);
		names.resize(37);
		possibleValues.resize(37);
		CPT.resize(37);
		mp.clear();
		Time* read = new Time();
		readNetwork();
		readDataFile();
		// showDependency();
		smoothingFactor = (originalData.size() / intermediateData.size()); // ANALYSE IT MANI SARTHAK !! (this is everything!)
		// smoothingFactor = 0.0035;
		CPTInitialiser();
		dataUpdater();
		writeData();
		read->showTime("reading and initialising both network and data", 1);
		Time* solving = new Time();
		solve();
		solving->showTime("learning and inferencing", 1);
	}


	void solve(){  
		float time_to_write = 0.005; // max time to write in solved_alarm.bif
		float bufferTime = 0.800; // for last iteration to take place
		int iter = 0;
		while ((float)initTime->getTime(2)   < (processTime - time_to_write - bufferTime)*(1e6)){ // this should be time not exceeded 
			Time* iterTime = new Time();
			// 1. learn the CPT from intermediat data structure 
			CPTUpdater();
			// 2. wirte to `solved_alarm.bif` since time <  1 ms.
			Time* write = new Time();
			writeData();
			write->showTime("writing in iteration "+to_string(iter+1), 2);
			// 3. update the values to intermediate data structure using inference from the CPT learnt
			if ((float)initTime->getTime(2)   < (processTime - 0.030)*(1e6)) dataUpdater();
			iter ++;
			iterTime->showTime("iteration " + to_string(iter), 1);
		}
	}


	double getRandom(){
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<double> dist(0.0, 1.0);
		return dist(gen);
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


	void CPTUpdater(){
		// uses values given in the intermediate data structure to learn the values
		// for the CPT

		// learning algorithm = weighted sampling and smoothing


		for (int i = 0; i< CPT.size(); i++){
			CPT[i] = vector<float>(CPT[i].size(), smoothingFactor);
			vector<int> ids, Sizes;
			ids.push_back(i);
			Sizes.push_back(possibleValues[i].size());
			for (int j = 0; j < parents[i].size(); j++){
				Sizes.push_back(possibleValues[parents[i][j]].size());
				ids.push_back(parents[i][j]);
			}
			int len = CPT[i].size() / possibleValues[i].size();
			vector<float> sumParts(len, possibleValues[i].size()*smoothingFactor);
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
				if (sumParts[ j % len] == 0) CPT[i][j] = 0.0; // wont be the case ever
															// still to avoid div by 0 err
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
		// Inference algorithm = Likelihood weighting in Markov Blanket !

		// assign weights to the Intermediate data values ;
		weights.clear();
		ofstream out;
		out.open("a.txt");
		for (int dataRow = 0; dataRow < originalData.size(); dataRow++){
			
			if (missingValues[dataRow] == -1) {
				weights.push_back(1.0);
			}
			else{
				int variable = missingValues[dataRow];
				vector<float> probabilities ;
				float probab_sum = 0.0;
				for (int j = 0; j < possibleValues[variable].size(); j++){
					// calculates P(variable = j | conditions in that data row);
					// will pe problematic for the first time since in the originalData 
					// the value of missing variables are -1 !!
					float probability_of_value_given_evidence = calculateProbability(dataRow, variable, j); // j is the value
					for (int k=0; k < child[variable].size(); k++){
						originalData[dataRow][variable] = j; // fixing the variable to current
															// value for inferencing on childrens
						int childVar = child[variable][k];
						float factor1 = calculateProbability(dataRow, childVar, originalData[dataRow][childVar]);
						
						// float factor2 = 1.0;
						// for (int l=0; l < parents[childVar].size(); l++){
						// 	int parentVar = parents[childVar][l];
						// 	factor2 *= calculateProbability(dataRow, parentVar, originalData[dataRow][parentVar]);
						// }

						// the weight for the fact child value in the dataRow 
						// given the parents of that childVar
						probability_of_value_given_evidence *= factor1;
						// probability_of_value_given_evidence *= factor2;
					}
					probabilities.push_back(probability_of_value_given_evidence);
					probab_sum += probability_of_value_given_evidence;
				}


				float alpha = (1.0)/probab_sum;
				for (int i=0; i < probabilities.size(); i++) {
					probabilities[i] *= alpha;
					weights.push_back(probabilities[i]);
					// out << probabilities[i] << " ";
				}



				// // HARD !!!
				// int argmaxValue = max_element(probabilities.begin(), probabilities.end())-probabilities.begin();
				// originalData[dataRow][variable] = argmaxValue;


				// SOFT !!!
				float cumulativeProb = 0.0;
				double random = getRandom();
				for (int i=0; i<probabilities.size(); i++) {
					cumulativeProb += probabilities[i];
					if (random < cumulativeProb){
						originalData[dataRow][variable] = i;
						break;
					}
				}
			}
		}
		out.close();
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

		// cout << "written to solved_alarm.bif sucessfully" << endl;
	}
};


int main(int argc, char* argv[])
{
	Time* t = new Time();
	string network_file = argv[1];
	string data_file = argv[2];
	A4* ans = new A4(network_file, data_file, t);
	t->showTime("wholeProcess", 1);
	return 0; 
	
}





