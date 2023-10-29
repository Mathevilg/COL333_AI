#include "code.h"
#include <stack>
// only change in code needed here
class Node{
	public:
	int number;
	string name;
	vector<Node*> child;


	Node(int number, string name){
		this->number = number;
		this->name = name;
	}
};

	void A4::solve(){  
		float time_to_write = 2; // max time to write in solved_alarm.bif
		float bufferTime = 3; // for last iteration to take place
		int iter = 0;
		while ((float)initTime->getTime(2)   < (processTime - time_to_write - bufferTime)*(1e6)){ // this should be time not exceeded 
			// restart();
			// cout<<"restarted\n";
			// for (int i=0; i<5; i++) {
				Time* iterTime = new Time();
				// 1. learn the CPT from intermediat data structure 
				CPTUpdater();
				// 2. wirte to `solved_alarm.bif` since time <  1 ms.
				Time* write = new Time();
				writeData();
				int exitcode = std::system("./format_checker");
				// write->showTime("writing in iteration "+to_string(iter+1), 2);
				// 3. update the values to intermediate data structure using inference from the CPT learnt
				if ((float)initTime->getTime(2)   < (processTime - 0.030)*(1e6)) dataUpdater();
				// likelihood();
				iter ++;
				iterTime->showTime("iteration " + to_string(iter), 1);
			// }
		}
	}

	void A4::restart() {
		weights.clear();
		for (int i=0; i<originalData.size(); i+=1) {

			if (missingValues[i] == -1) {
				weights.push_back(1.0);
			}
			else {
				vector<float> temp_weights;
				float sum=0;
				for (int j=0; j < possibleValues[missingValues[i]].size(); j++) {
					float random = getRandom();
					temp_weights.push_back(random);
					sum+=random;
				}
				for (int j=0; j < possibleValues[missingValues[i]].size(); j++) {
					weights.push_back(temp_weights[j]/sum);
				}
			}
		}
	}

	void A4::likelihood() {
		float log_likelihood = 0.0;
		for (int dataRow=0; dataRow<originalData.size(); dataRow++) {
			// calculate the probability of seeing that data row 
			int variable = missingValues[dataRow];
			float prob;
			if (variable == -1) {
				prob=1.0;
				for (int i=0; i<originalData[dataRow].size(); i++) {
					prob*=calculateProbability(dataRow, i, originalData[dataRow][i]);
				}
				
			}
			else {
				prob = 0.0;
				for (int state=0; state<possibleValues[variable].size(); state++) {
					originalData[dataRow][variable] = state;
					float p = 1.0;
					for (int i=0; i<originalData[dataRow].size(); i++) {
						p*=calculateProbability(dataRow, i, originalData[dataRow][i]);
					} 
					prob+=p;
					// cout<<p<<" ";
				}
			}
			// cout<<"\t"<<dataRow<<" "<<prob<<"\t";
			// if (prob==0) cout<<log(prob)<<" ";
			log_likelihood+=log(prob);
		}
		cout<<"\t"<<fixed << setprecision(4) <<log_likelihood<<"\t";
	}

	float A4::calculateProbability(int dataRow, int variable, int state){
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

	void A4::dataUpdater(){
		// using the intermediate CPT values modifies the data in the intermediate 
		// data structure 
		// USES SOFT METHOD !!
		// Inference algorithm = Likelihood weighting in Markov Blanket !
		// assign weights to the Intermediate data values ;
		weights.clear();
		for (int i=0; i<originalData.size(); i++) {
			int var = missingValues[i];
			if (var == -1) {
				weights.push_back(1.0);
			}
			else {
				vector<float> prob;
				float sum=0.0;
				for (int j=0; j<possibleValues[var].size(); j++) {
					// calculates P(variable = j | conditions in that data row);
					// will pe problematic for the first time since in the originalData 
					// the value of missing variables are -1 !!
					float p = calculateProbability(i,var,j);
					originalData[i][var] = j;
					for (auto child: child[var]) {
						p*=calculateProbability(i,child,originalData[i][child]);
					}
					prob.push_back(p);
					sum+=p;
				}
				for (auto p: prob) {
					weights.push_back(p/sum);
				}
			}
		}
	}


	void A4::CPTUpdater(){
		// uses values given in the intermediate data structure to learn the values
		// for the CPT
		// learning algorithm = weighted sampling and smoothing
		for (int i=0; i<CPT.size(); i++) {
			CPT[i] = vector<float>(CPT[i].size(),0);
			int len = CPT[i].size()/possibleValues[i].size();
			vector<float> sumParts(len,0);
			for (int j=0; j<intermediateData.size(); j++) {
				int idx = 0, temp = 1;
				for (int k=parents[i].size()-1; k>=0; k--) {
					idx+=temp*intermediateData[j][parents[i][k]];
					temp*=possibleValues[parents[i][k]].size();
				}
				idx+=temp*intermediateData[j][i];
				CPT[i][idx] += weights[j];
				sumParts[idx%len]+=weights[j];
			}
			for (int j=0; j<CPT[i].size(); j++) {
				if (sumParts[j%len]==0) CPT[i][j] = 0.0;
				else CPT[i][j]/=sumParts[j%len];
			}
		}
	}

	void dfs(Node* currNode, stack<int> &st, vector<int> &vis){
		vis[currNode->number] = 1;
		for (auto child: currNode->child){
			if (!vis[child->number]) dfs(child, st, vis);
		}
		st.push(currNode->number);
	}

	double A4::calculateProbabilityTopoSort(int var, int varState, map<int, int> &stateMap){
		vector<int> values, Sizes;
		values.push_back(varState);
		Sizes.push_back(possibleValues[var].size());
		for (int i=0; i<parents[var].size(); i++){
			values.push_back(stateMap[parents[var][i]]);
			Sizes.push_back(possibleValues[parents[var][i]].size());
		}
		int idx = 0, temp = 1;
		for (int i = values.size()-1; i >= 0; i--){
			idx += temp * values[i];
			temp *= Sizes[i];
		}
		return CPT[var][idx];
	}

	vector<double> A4::genCumulative(int var, map<int, int>& stateMap){
		vector<double> cumulative;
		double base = 0.0;
		for (int i=0; i < possibleValues[var].size(); i++){
			// cout << possibleValues[var][i] << endl;
			base += calculateProbabilityTopoSort(var, i, stateMap);
			cumulative.push_back(base);
		}
		for (int i=0; i<cumulative.size(); i++){
			cumulative[i] /= base;
		}
		return cumulative;
	}

	int findState(vector<double> cumulative, double coinFlip){
		double sum = 0.0;
		for (int i=0; i<cumulative.size(); i++) {
			if (coinFlip < cumulative[i]) return i;
		}
		return cumulative.size()-1;
	}

	vector<int> A4::generateDataRow(vector<int>& topo){
		map<int, int> stateMap;
		stateMap.clear();
		vector<int> row(variables, -1);
		for (auto var: topo){
			double coinFlip = getRandom();
			vector<double> cumulative = genCumulative(var, stateMap);
			int state = findState(cumulative, coinFlip);
			stateMap[var] = state;
			row[var] = state;
		}
		return row;
	}

	vector<int> A4::getTopoSort(){
		map<int, Node*> graph;
		for (int i=0; i<variables; i++){
			graph[i] = new Node(i, names[i]);
		}


		for (int i=0; i<variables; i++){
			for (int j=0; j<child[i].size(); j++){
				(graph[i]->child).push_back(graph[child[i][j]]);
			}
		}

		vector<int> vis(variables, 0);
		stack<int> st;
		for (int i=0; i<variables; i++){
			if (!vis[i]) {
				dfs(graph[i], st, vis);
			}
		}

		vector<int> topo;
		while (!st.empty()){
			topo.push_back(st.top());
			st.pop();
		}
		return topo;
	}

	void A4::genTestcase(string filename){
		//parameters
		double mini = 0.2;
		double maxi = 0.8;
		int dataLines = 20000;
		double missingRate = 0.85;

		//get the structure !
		readNetwork();

		//generate the CPT;
		for (int i=0; i<variables; i++){
			int sz = possibleValues[i].size();
			int len = CPT[i].size() / sz;
			vector<double> probability_sum(len, 0.0);
			for (int j = 0; j< sz * len; j++){
				CPT[i][j] = getRandom(mini, maxi);
				probability_sum[j%len] += CPT[i][j];
			}
			for (int j=0; j<sz*len; j++){
				CPT[i][j] /= probability_sum[j % len];
			}
		}

		// write the network in gold_gen.bif for checking
		writeData("gold_gen.bif");

		//generate the topoSort
		vector<int> topo = getTopoSort();

		// write the data to file_name;
		ofstream out;
		out.open(filename);
		for (int i=0; i<dataLines; i++){
			vector<int> row = generateDataRow(topo);
			vector<string> rowName;
			for (int i=0; i<row.size(); i++){
				rowName.push_back(possibleValues[i][row[i]]);
			}
			double missing = getRandom();
			if (missing < missingRate){
				double idx = getRandom()*variables;
				rowName[(int) idx] = "\"?\"";
			}
			for (auto it: rowName) {
				out << it << " ";
			}
			if (i == dataLines-1) continue;
			else out << endl;
		}
		out.close();
	}

int main(int argc, char* argv[])
{
	Time* t = new Time();
	string network_file = argv[1];
	string data_file = argv[2];
	A4* ans = new A4(network_file, data_file, t);
	t->showTime("wholeProcess", 1);
	return 0; 
	
}





