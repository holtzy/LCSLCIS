//Created by Lisa De Matteo, modified by S. Bérard and V. Ranwez 8 nov. 2017 à finir : tri ???
#include <Rcpp.h>
#include <map>
using namespace Rcpp;

// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins(cpp11)]]


void linearExtentionCode(std::vector<std::vector<std::string> > oh1, std::vector<std::vector<std::string> > oh2, std::vector<std::string> & ext1, std::vector<int> & posExt2){
	// Make ext1 and posExt2
	// ext1[i] = j iff j is the i+1 th element in the linear extension of order_1 according to the known total order on D
	// posExt2[i] = j iff the i+1 th element in the linear extension of order_2 according to the known total order on D is the j th element in ext1

	std::cout << "-- calling linearExtentionCode .." << std::endl;

	// Save positions of order_2 elements in the linear extention which linearize order_2 according to the known total order of D
	
	std::map<std::string, int> dicPosExt2;
	int ind = 1;
	for (int i = 0; i < oh2.size(); i++){
		for (int j = 0; j < oh2[i].size(); j++){
			dicPosExt2[oh2[i][j]] = ind++;
		}
	}

	// Construct ext1 (the linear ext of a according to the known total order of D) and posExt2 (the position of elements in the linear extention ext2, the linear ext of order_2 according to the known total order of D) by browsing order_1
	int posExt1 = 0;
	for (int i = 0; i < oh1.size(); i++){
		for (int j = 0; j < oh1[i].size(); j++){
			ext1.push_back(oh1[i][j]);
			posExt2.push_back(dicPosExt2.find(oh1[i][j])->second);
		}
	}

	std::cout << "-- end running linearExtentionCode" << std::endl;
}

std::list<int> LIS(std::vector<int> seq){
	// See https://www.youtube.com/watch?v=S9oUiVYEq7E for more details on this nlogn LIS function

	std::cout << "-- calling LIS .." << std::endl;

	// Initializations
	std::vector<int> R(seq.size()), T(seq.size());
	for (int i = 0; i < seq.size(); i ++){
		R[i] = -1;
	}
	int lastT = 0;
	T[lastT] = 0;


	for (int i = 1; i < seq.size(); i++){

		if (seq[i] > seq[T[lastT]]){

			T[lastT+1] = i;
			R[i] = T[lastT];
			lastT++;
		}
		else{
			int lo = 0;
			int hi = lastT;

			while (lo <= hi){
				int mid = std::floor((lo+hi)/2);
				if (seq[T[mid]] < seq[i]){
					lo = mid + 1;
				}
				else{
					hi = mid - 1;
				}
			}

			T[lo] = i;
			if (lo - 1 >= 0){
				R[i] = T[lo - 1];
			}
		}
	}

	// Backtrack in R to get the LIS
	std::list<int> L;
	int prev = T[lastT];
	L.push_front(seq[prev]);
	prev = R[prev];

	while (prev != -1){
		L.push_front(seq[prev]);
		prev = R[prev];
	}


	// Display the lis
	/*
	int i = 1;
	for (std::list<int>::iterator it=L.begin() ; it != L.end(); ++it){
		std::cout << "lis[" << i++ << "] = " << *it << std::endl;
	}*/

	std::cout << "-- end running LIS" << std::endl;
	return L;

}

bool pairCompare(const std::pair<std::string, int>& firstElem, const std::pair<std::string, int>& secondElem) {
  return firstElem.second < secondElem.second;

}

std::vector<std::vector<std::string> > Homogeneization(std::vector<std::vector<std::string> > o1, std::vector<std::vector<std::string> > o2){

	std::cout << "-- calling Homogeneization .." << std::endl;

	std::vector<std::vector<std::string> > oh2;

	std::map<std::string, int> dic1;

	for (int i = 0; i < o1.size(); i++){
		for (int j = 0; j < o1[i].size(); j++){
			dic1.insert(std::pair<std::string, int>(o1[i][j],i+1));
		}
	}

	std::map<std::string, int>::iterator it;

	for (int i = 0; i < o2.size(); i++){
		std::vector<std::pair<std::string, int> > Ltemp;
		for (int j = 0; j < o2[i].size(); j++){
			it = dic1.find(o2[i][j]);
			if (it != dic1.end()){
				// the element o2[i][j] is in the two given bucket orders
				Ltemp.push_back(std::pair<std::string, int>(o2[i][j], it->second));
			}
		}

		

		if (Ltemp.size() > 0){

			// Sort Ltemp according to second positions of pairs
			std::sort(Ltemp.begin(), Ltemp.end(), pairCompare);

			int prevPos = Ltemp[0].second;
			std::vector<std::string> buck;

			for (int k = 0; k < Ltemp.size(); k++){
				if (Ltemp[k].second == prevPos){
					// enrich current bucket buck
					buck.push_back(Ltemp[k].first);
				}
				else {
					oh2.push_back(buck);
					buck.clear();
					buck.push_back(Ltemp[k].first);
					prevPos = Ltemp[k].second;
				}
			}

			// Handle last bucket
			oh2.push_back(buck);
		}

	}

	std::cout << "-- end running Homogeneization" << std::endl;
	return oh2;

}

//' Find the Longuest Common Sequence (LCS) between 2 lists.
//'
//' The LCS function takes 2 list of items and return the biggest subset of items
//' showing the same order. If several items are at the same position, all of them
//' will be returned.
//'
//' @title Longuest Common Sequence (LCS)
//' @param order_1 the first list. Must be 2 columns called V1 and V2. V1 being the name
//' of the item, V2 its position
//' @param order_2 the second list. Must be 2 columns called V1 and V2. V1 being the name
//' of the item, V2 its position
//' @return The function returns a list with several informations:
//'			- $order_1: the first list
//'			- $order_2: the second list
//'			- $LLCS: the number of items in the longuest common sequence
//'			- $LCS: the item names in the longuest common sequence
//'			- $QSI: an estimate of the proportion of items kept in the LCS
//'			- $LCS_pos_order_1: the position of kept items in the first list
//'			- $LCS_pos_order_2: the position of kept items in the second list
//' @author Lisa de Matteo
//' @examples
//' # create 2 lists of items, each item having a position
//' o1 <- data.frame( 
//'    	V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
//'    	V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
//'    )
//' o2 <- data.frame( 
//'		V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
//'    	V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
//'    )
//' # run LCS
//' LCS(o1, o2)
// [[Rcpp::export]]
List LCS(DataFrame order_1, DataFrame order_2){

	std::cout << "-- calling LCS .." << std::endl;
	//Positions des éléments commençant à 1 stockées dans ces dictionnaires
	std::map<std::string, int> markerToPos1;
	std::map<std::string, int> markerToPos2;

	// Construct o1 from order_1
	CharacterVector temp_namesO1 = order_1["V1"];

	std::vector<float> posO1 = as<std::vector<float> >(order_1[1]);

	std::vector<std::vector<std::string> > o1;

	int numBuc1 = 0;

	float last = posO1[0];	

	std::vector<std::string> curBuc1;
	curBuc1.push_back(Rcpp::as<std::string>(temp_namesO1[0]));

	for (int j = 1; j < order_1.nrows(); j++){
		if (posO1[j] != last){

			numBuc1++;
			o1.push_back(curBuc1);
			curBuc1.resize(0);
		}

		//o1[numBuc].push_back(Rcpp::as<std::string>(temp_namesO1[j]));
		curBuc1.push_back(Rcpp::as<std::string>(temp_namesO1[j]));

		last = posO1[j];
	}

	o1.push_back(curBuc1);

	std::cout << "o1 gets " << numBuc1+1 << " buckets and " << order_1.nrows() << " markers, o1 :" << std::endl;
	
	// Display o1
	for (int i = 0; i < o1.size(); i++){
		std::cout << "(" ;
		for (int j = 0; j < o1[i].size(); j++){
			std::cout << o1[i][j];
			if (j != o1[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;


	// Construct o2 from order_2
	CharacterVector temp_namesO2 = order_2["V1"];

	std::vector<float> posO2 = as<std::vector<float> >(order_2[1]);

	std::vector<std::vector<std::string> > o2;

	int numBuc2 = 0;

	last = posO2[0];	

	std::vector<std::string> curBuc2;
	curBuc2.push_back(Rcpp::as<std::string>(temp_namesO2[0]));

	for (int j = 1; j < order_2.nrows(); j++){
		if (posO2[j] != last){
			numBuc2++;
			o2.push_back(curBuc2);
			curBuc2.resize(0);
		}

		//o1[numBuc].push_back(Rcpp::as<std::string>(temp_namesO1[j]));
		curBuc2.push_back(Rcpp::as<std::string>(temp_namesO2[j]));

		last = posO2[j];
	}

	o2.push_back(curBuc2);

	std::cout << "o2 gets " << numBuc2+1 << " buckets and " << order_2.nrows() << " markers, o2 : " << std::endl;

	//Construction des dicos de positions
	for (int j = 0; j < order_1.nrows(); j++)
	   markerToPos1.insert(std::pair<std::string, int>(Rcpp::as<std::string>(temp_namesO1[j]),j+1));
	for (int j = 0; j < order_2.nrows(); j++)
	   markerToPos2.insert(std::pair<std::string, int>(Rcpp::as<std::string>(temp_namesO2[j]),j+1));
	
	// Display o2
	for (int i = 0; i < o2.size(); i++){
		std::cout << "(" ;
		for (int j = 0; j < o2[i].size(); j++){
			std::cout << o2[i][j];
			if (j != o2[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;



	// Homogeneization
	std::vector<std::vector<std::string> > oh2 = Homogeneization(o1,o2);
	std::cout << "oh2 :" << std::endl;
	// Sort oh2 bucket lexico and Display oh2
	for (int i = 0; i < oh2.size(); i++){
		std::cout << "(" ;
		std::sort(oh2[i].begin(), oh2[i].end());
		for (int j = 0; j < oh2[i].size(); j++){
			std::cout << oh2[i][j];
			if (j != oh2[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;

	int nbMarker = 0;
	std::cout << "oh1 :" << std::endl;
	std::vector<std::vector<std::string> > oh1 = Homogeneization(o2,o1);
	// Sort oh1 bucket lexico and Display oh1
	for (int i = 0; i < oh1.size(); i++){
		std::cout << "(" ;
		std::sort(oh1[i].begin(), oh1[i].end());
		nbMarker = nbMarker + oh1[i].size();
		for (int j = 0; j < oh1[i].size(); j++){
			std::cout << oh1[i][j];
			if (j != oh1[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;


	// Make ext1 and posExt2
	// ext1[i] = j iff j is the i+1 th element in the linear extension of order_1 according to the known total order on D
	// posExt2[i] = j iff the i+1 th element in the linear extension of order_2 according to the known total order on D is the j th element in ext1
	
	std::vector<std::string> ext1;
	std::vector<int> posExt2;
	linearExtentionCode(oh1,oh2,ext1,posExt2);
	
	// Compute a LIS of posExt2
 	std::list<int> lis = LIS(posExt2);

 	// Make the corresponding LCS
 	std::vector<std::string> LCS;
 	int j = 0;
 	int i = 0;
 	for (std::list<int>::iterator it=lis.begin(); it != lis.end(); ++it){
 		while (*it != posExt2[j]) j++;
 		LCS.push_back(ext1[j]);
 	}

 	// Get the length of the LCS
 	int LLCS = lis.size();


 	// Compute the QSI of the LCS
 	float QSI = float(LLCS)/ float(nbMarker);


 	// Get the numeros of buckets of order_1 which are contain in the LCS
 	// std::vector<int> va;
 	// int indBuc = 0;
 	// int indEl = 0;
 	// for (int i = 0; i < LCS.size(); i++){
 	// 	// For each letter l in the LCS we search the bucket of o1 which contains l
 	// 	indEl = 0;
 	// 	while (o1[indBuc][indEl] != LCS[i]){
 	// 		if (indEl == o1[indBuc].size() - 1){
 	// 			indBuc++;
 	// 			indEl = 0;
 	// 		}
 	// 		else {
 	// 			indEl++;
 	// 		}

 	// 	}
 	// 	va.push_back(indBuc+1);
 	// }
	std::vector<int> va;
	std::vector<int> vb;
 	for (int i = 0; i < LCS.size(); i++){
	   // For each letter l in the LCS we get the position in order_1
	   va.push_back(markerToPos1[LCS[i]]);
	   vb.push_back(markerToPos2[LCS[i]]);
 	}
 	// Get the numeros of buckets of order_2 which are contain in the LCS
 	// std::vector<int> vb;
 	// indBuc = 0;
 	// indEl = 0;
 	// for (int i = 0; i < LCS.size(); i++){
 	// 	// For each letter l in the LCS we search the bucket of o2 which contains l
 	// 	indEl = 0;
 	// 	while (o2[indBuc][indEl] != LCS[i]){
 	// 		if (indEl == o2[indBuc].size() - 1){
 	// 			indBuc++;
 	// 			indEl = 0;
 	// 		}
 	// 		else {
 	// 			indEl++;
 	// 		}

 	// 	}
 	// 	vb.push_back(indBuc+1);
 	// }
	std::cout << "-- end running LCS" << std::endl;

	// return LCS R function format 
	return List::create(Named("order_1") = order_1, Named("order_2") = order_2, Named("LLCS") = LLCS, Named("LCS") = LCS, Named("QSI") = QSI, Named("LCS_pos_order_1") = va, Named("LCS_pos_order_2") = vb);
}

void bucketCode(std::vector<std::vector<std::string> > oh1, std::vector<std::vector<std::string> > oh2, std::vector<int> & posBuc2){
	// Make posBuc2, s.t. posBuc2[i] = j iff the i+1 th bucket of order_1 equal the posBuc2[i] th bucket of order_2
	std::cout << "-- calling bucketCode .." << std::endl;

	// Save in dicPosBuc1 the numero of order_1 buckets (value) for all elements of the domain (key)
	std::map<std::string, int> dicPosBuc1;
	for (int i = 0; i < oh1.size(); i++){
		for (int j = 0; j < oh1[i].size(); j++){
			dicPosBuc1[oh1[i][j]] = i;
		}
	}

	// Construct posBuc2, s.t. posBuc2[i] = j iff the i+1 th bucket of order_1 equal the posBuc2[i] th bucket of order_2
	for (int i = 0; i < oh2.size(); i++){
		posBuc2.push_back(dicPosBuc1.find(oh2[i][0])->second);
	}

	for (int i = 0; i < posBuc2.size(); i++){
		std::cout << "posBuc2[" << i << "] = " << posBuc2[i] << std::endl;
	}

	std::cout << "-- end running bucketCode" << std::endl;
}




//' Find the Longuest Common Induced Sequence (LCIS) between 2 lists.
//'
//' The LCS function takes 2 list of items and return the biggest subset of items
//' showing the same order. If several items are at the same position, only one will
//' be kept per position.
//'
//' @title Longuest Common Induced qSequence (LCIS)
//' @param order_1 the first list. Must be 2 columns called V1 and V2. V1 being the name
//' of the item, V2 its position
//' @param order_2 the second list. Must be 2 columns called V1 and V2. V1 being the name
//' of the item, V2 its position
//' @return The function returns a list with several informations:
//'			- $order_1: the first list
//'			- $order_2: the second list
//'			- $LLCIS: the number of items in the longuest common sequence
//'			- $LCIS: the item names in the longuest common sequence
//'			- $QSI: an estimate of the proportion of items kept in the LCS
//'			- $LCIS_pos_order_1: the position of kept items in the first list
//'			- $LCIS_pos_order_2: the position of kept items in the second list
//' @author Lisa de Matteo
//' @examples
//' # create 2 lists of items, each item having a position
//' o1 <- data.frame( 
//'    	V1=c("g","h","c","f","e","d","m","q","r","a","b","n","o","p"), 
//'    	V2=c(1.1,1.1,3.4,3.4,3.4,3.4,3.5,3.5,6.6,6.6,7.2,7.2,8,8)
//'    )
//' o2 <- data.frame( 
//'		V1=c("k","a","b","l","c","e","d","f","i","j","h","g"), 
//'    	V2=c(0.1,1.2,1.2,4.2,4.2,5,5,5,5.3,5.3,6.7,6.7)
//'    )
//' # run LCS
//' LCIS(o1, o2)
// [[Rcpp::export]]
List LCIS(DataFrame order_1, DataFrame order_2){
	// It supposes that order_1 and order_2 are homogeneized

	std::cout << "-- calling LCIS .." << std::endl;
	//Positions des éléments commençant à 1 stockées dans ces dictionnaires
	std::map<std::string, int> markerToPos1;
	std::map<std::string, int> markerToPos2;


	// Construct o1 from order_1
	CharacterVector temp_namesO1 = order_1["V1"];

	std::vector<float> posO1 = as<std::vector<float> >(order_1[1]);

	std::vector<std::vector<std::string> > o1;

	int numBuc1 = 0;

	float last = posO1[0];	

	std::vector<std::string> curBuc1;
	curBuc1.push_back(Rcpp::as<std::string>(temp_namesO1[0]));

	for (int j = 1; j < order_1.nrows(); j++){
		
		if (posO1[j] != last){

			numBuc1++;
			o1.push_back(curBuc1);
			curBuc1.resize(0);
		}

		//o1[numBuc].push_back(Rcpp::as<std::string>(temp_namesO1[j]));
		curBuc1.push_back(Rcpp::as<std::string>(temp_namesO1[j]));

		last = posO1[j];
	}

	o1.push_back(curBuc1);

	std::cout << "o1 gets " << numBuc1+1 << " buckets and " << order_1.nrows() << " markers, o1 :" << std::endl;
	
	// Display o1
	for (int i = 0; i < o1.size(); i++){
		std::cout << "(" ;
		for (int j = 0; j < o1[i].size(); j++){
			std::cout << o1[i][j];
			if (j != o1[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;


	// Construct o2 from order_2
	CharacterVector temp_namesO2 = order_2["V1"];

	std::vector<float> posO2 = as<std::vector<float> >(order_2[1]);

	std::vector<std::vector<std::string> > o2;

	int numBuc2 = 0;

	last = posO2[0];	

	std::vector<std::string> curBuc2;
	curBuc2.push_back(Rcpp::as<std::string>(temp_namesO2[0]));

	for (int j = 1; j < order_2.nrows(); j++){
		if (posO2[j] != last){
			numBuc2++;
			o2.push_back(curBuc2);
			curBuc2.resize(0);
		}

		//o1[numBuc].push_back(Rcpp::as<std::string>(temp_namesO1[j]));
		curBuc2.push_back(Rcpp::as<std::string>(temp_namesO2[j]));

		last = posO2[j];
	}

	o2.push_back(curBuc2);

	std::cout << "o2 gets " << numBuc2+1 << " buckets and " << order_2.nrows() << " markers, o2 : " << std::endl;

	//Construction des dicos de positions
	for (int j = 0; j < order_1.nrows(); j++)
	   markerToPos1.insert(std::pair<std::string, int>(Rcpp::as<std::string>(temp_namesO1[j]),j+1));
	for (int j = 0; j < order_2.nrows(); j++)
	   markerToPos2.insert(std::pair<std::string, int>(Rcpp::as<std::string>(temp_namesO2[j]),j+1));

	// Display o2
	for (int i = 0; i < o2.size(); i++){
		std::cout << "(" ;
		for (int j = 0; j < o2[i].size(); j++){
			std::cout << o2[i][j];
			if (j != o2[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;



	// Homogeneization
	std::vector<std::vector<std::string> > oh2 = Homogeneization(o1,o2);
	std::cout << "oh2 :" << std::endl;
	// Sort oh2 bucket lexico and Display oh2
	for (int i = 0; i < oh2.size(); i++){
		std::cout << "(" ;
		std::sort(oh2[i].begin(), oh2[i].end());
		for (int j = 0; j < oh2[i].size(); j++){
			std::cout << oh2[i][j];
			if (j != oh2[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;

	int nbMarker = 0;
	std::cout << "oh1 :" << std::endl;
	std::vector<std::vector<std::string> > oh1 = Homogeneization(o2,o1);
	// Sort oh1 bucket lexico and Display oh1
	for (int i = 0; i < oh1.size(); i++){
		std::cout << "(" ;
		std::sort(oh1[i].begin(), oh1[i].end());
		nbMarker = nbMarker + oh1[i].size();
		for (int j = 0; j < oh1[i].size(); j++){
			std::cout << oh1[i][j];
			if (j != oh1[i].size()-1) std::cout << " ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;
	

	// Make posBuc2
	// posBuc2[i] = j iff the i+1 th bucket order_2 equals the j th bucket of order_1
	std::vector<int> posBuc2;
	bucketCode(oh1,oh2,posBuc2);
	
	// Compute a LIS of posBuc2
 	std::list<int> lis = LIS(posBuc2);

 	// Construct the corresponding LCIS
 	std::vector<std::string> LCIS;
 	for (std::list<int>::iterator it = lis.begin(); it != lis.end(); ++it){
 		LCIS.push_back(oh1[*it][0]);
 	}
 	
 	// Get the LCIS lenght
 	int LLCIS = lis.size();

 	// Compute the SQI of LCIS
 	float QSI = float(LLCIS)/ float(nbMarker);


 		std::vector<int> va;
	std::vector<int> vb;
 	for (int i = 0; i < LCIS.size(); i++){
	   // For each letter l in the LCS we get the position in order_1
	   va.push_back(markerToPos1[LCIS[i]]);
	   vb.push_back(markerToPos2[LCIS[i]]);
 	}
// // Get the numeros of buckets of order_1 which are contain in the LCIS
//  	std::vector<int> va;
//  	int indBuc = 0;
//  	int indEl = 0;
//  	for (int i = 0; i < LCIS.size(); i++){
//  		// For each letter l in the LCIS we search the bucket of o1 which contains l
//  		indEl = 0;
//  		while (o1[indBuc][indEl] != LCIS[i]){
//  			if (indEl == o1[indBuc].size() - 1){
//  				indBuc++;
//  				indEl = 0;
//  			}
//  			else {
//  				indEl++;
//  			}

//  		}
//  		va.push_back(indBuc+1);
//  	}

//  	// Get the numeros of buckets of order_2 which are contain in the LCIS
//  	std::vector<int> vb;
//  	indBuc = 0;
//  	indEl = 0;
//  	for (int i = 0; i < LCIS.size(); i++){
//  		// For each letter l in the LCIS we search the bucket of o2 which contains l
//  		indEl = 0;
//  		while (o2[indBuc][indEl] != LCIS[i]){
//  			if (indEl == o2[indBuc].size() - 1){
//  				indBuc++;
//  				indEl = 0;
//  			}
//  			else {
//  				indEl++;
//  			}

//  		}
//  		vb.push_back(indBuc+1);
//  	}

 	std::cout << "-- end running LCIS" << std::endl;
 	// QSI = |LCIS|/|D1 \cap D2|
	return List::create(Named("order_1") = order_1, Named("order_2") = order_2, Named("LLCIS") = LLCIS, Named("LCIS") = LCIS, Named("QSI") = QSI, Named("LCIS_pos_order_1") = va, Named("LCIS_pos_order_2") = vb);
}
