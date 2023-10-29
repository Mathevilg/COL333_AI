#include "code.h"
// only change in code needed here

void A4::solve(){  
		float time_to_write = 0.005; // max time to write in solved_alarm.bif
		float bufferTime = 0.800; // for last iteration to take place
		int iter = 0;
		while ((float)initTime->getTime(2)   < (processTime - time_to_write - bufferTime)*(1e6)){ // this should be time not exceeded 
			restart();
			cout<<"restarted\n";
			for (int i=0; i<5; i++) {
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
				likelihood();
				iter ++;
				iterTime->showTime("iteration " + to_string(iter), 1);
			}
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
		float likelihood = 1.0;
		for (int dataRow=0; dataRow<originalData.size(); dataRow++) {
			// calculate the probability of seeing that data row 
			int variable = missingValues[dataRow];
			float prob = 0.0;
			for (int state=0; state<possibleValues[variable].size(); state++) {
				originalData[dataRow][variable] = state;
				float p = 1.0;
				for (int i=0; i<originalData[dataRow].size(); i++) {
					p*=calculateProbability(dataRow, i, originalData[dataRow][i]);
				} 
				prob+=1000*p;
				// cout<<calculateProbability(dataRow,variable,state)<<" ";
			}
			// cout<<"\t"<<dataRow<<" "<<prob<<"\t";
			// if (prob==0) cout<<log(prob)<<" ";
			likelihood*=prob;
		}
		cout<<"\t"<<likelihood<<"\t";
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
				// float cumulativeProb = 0.0;
				// double random = getRandom();
				// for (int i=0; i<probabilities.size(); i++) {
				// 	cumulativeProb += probabilities[i];
				// 	if (random < cumulativeProb){
				// 		originalData[dataRow][variable] = i;
				// 		break;
				// 	}
				// }
			}
		}
		out.close();
	}


void A4::CPTUpdater(){
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

int main(int argc, char* argv[])
{
	Time* t = new Time();
	string network_file = argv[1];
	string data_file = argv[2];
	A4* ans = new A4(network_file, data_file, t);
	t->showTime("wholeProcess", 1);
	return 0; 
	
}





