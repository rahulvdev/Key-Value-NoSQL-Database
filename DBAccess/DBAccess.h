#pragma once
//////////////////////////////////////////////////////////////////////
// DBAccess.h - It provides member functions for database operations//
//                                                                  //
// Rahul Vijaydev, CSE687 - Object Oriented Design, Spring 2017     //
/////////////////////////////////////////////////////////////////////
//
//Description:This file provides memeber functions used to access all database operations
//like insertion of database elements,deletion of elements,updating metadata and data values
//and querying the database content.
//
//
//Member functions:
//bool deleteElement(std::string key);
//void showElement(Key key);
//std::vector<Key> getKeys();
//void saveDBElement(std::string name, std::string category, std::string desc, Data data, std::vector<Key> childrenVect);
//bool modifyMetaData(Key key, std::string property, std::string newVal, bool typeOfOp);
//bool modifyData(Key key, Data data);
//void addChild(Key key, std::string child);
//time_t DateTimeFromString(std::string Dt);
//std::string DateTimeToString(time_t time);
//void toXMl(std::string nameOfFile);
//void fromXml(std::string nameOfFile);
//std::vector<std::string> queryTime(std::string startTime, std::string endTime);
//std::vector<std::string> retrieveChildrenList(Key key);
//std::vector<std::string> queryElement(std::vector<std::string> keyList, std::string pattern, std::string property);
//std::vector<std::string> formKeyUnion(std::vector<std::string> keySet1, std::vector<std::string> keySet2);
//std::vector<std::string> queryElemRegex(std::vector<std::string> keyList, std::string regexPattern, std::string property);
//

#include <iostream>
#include <vector>
#include <string>
#include <regex>


template<typename Data>
class DBHandler {

private :
	NoSqlDb<Data> dbObj;
	int noOfWrites;
	int currentWrites = 0;
	std::string nameOffile;

public:
	
	using StrData = std::string;
	using intData = int;
	using Key = std::string;
	using Keys = std::vector<Key>;
	DBHandler() {
	
	}

	DBHandler(std::string fname, int writeNum)
	{
		nameOffile = fname;
		noOfWrites = writeNum;
		dbObj.setxmlFilePath(fname);
	}

	
	bool deleteElement(std::string key);
	void showElement(Key key); 
	std::vector<Key> getKeys();
	void saveDBElement(std::string name, std::string category, std::string desc, Data data,std::vector<Key> childrenVect);
	bool modifyMetaData(Key key, std::string property, std::string newVal,bool typeOfOp);
	bool modifyData(Key key, Data data);
	void addChild(Key key,std::string child);
	time_t DateTimeFromString(std::string Dt);
	std::string DateTimeToString(time_t time);
	void toXMl(std::string nameOfFile);
	void fromXml(std::string nameOfFile);
	void showDbContent();
	std::vector<std::string> queryTime(std::string startTime, std::string endTime);
	std::vector<std::string> retrieveChildrenList(Key key);
	std::vector<std::string> queryElement(std::vector<std::string> keyList,std::string pattern,std::string property);
	std::vector<std::string> formKeyUnion(std::vector<std::string> keySet1, std::vector<std::string> keySet2);
	std::vector<std::string> queryElemRegex(std::vector<std::string> keyList, std::string regexPattern, std::string property);
};



template<typename Data>
bool DBHandler<Data>::deleteElement(std::string key) {
	std::cout << "--------------------------------------- \n";
	std::cout << "Demonstrating requirement 3 \n";
	std::cout << "--------------------------------------- \n";
	std::cout << "Deleting element with key= " << key << "\n";
	if (dbObj.deleteElement(key))
		return true;
	else
		return false;
}


template<typename Data>
std::vector<std::string> DBHandler<Data>::getKeys(){
	return dbObj.keys();
}

template<typename Data>
void DBHandler<Data>::toXMl(std::string nameOfFile) {
	std::cout<< "----------------------------------- \n";
	std::cout << "Requirement 5:writing to XML \n";
	std::cout << "-------------------------------- \n";
	dbObj.toXml(nameOfFile);
}

template<typename Data>
void  DBHandler<Data>::fromXml(std::string nameOfFile) {
	std::cout<< "----------------------------------- \n";
	std::cout << "Requirement 5:reading from XML file \n";
	std::cout << "-------------------------------- \n";
	dbObj.fromXml(nameOfFile);
}

template<typename Data>
inline void DBHandler<Data>::showDbContent()
{
	std::cout << "\n  size of db = " << dbObj.count() << "\n";
	Keys keys = dbObj.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << dbObj.value(key).show();
	}
}

template<typename Data>
void DBHandler<Data>::showElement(Key key) {
	std::cout << "\n";
	std::cout << "Displaying contents of element with key " << key << "\n";
	std::cout<<dbObj.value(key).show();
	std::cout << "-----------------------------------------------------------------------" << "\n";
	std::cout << "\n";
}


template<typename Data>
void DBHandler<Data>::saveDBElement(std::string name, std::string category, std::string desc, Data data,std::vector<Key> childrenVect) {
	Element<Data> elem;
	elem.name = name;
	elem.category = category;
	elem.descr = desc;
	elem.data = data;
	elem.timeDate = time(0);
	elem.children = childrenVect;
	dbObj.save(elem.name, elem);
	currentWrites++;
	if (currentWrites > noOfWrites) {
		toXMl(nameOffile);
		currentWrites = 0;
	}
}

template<typename Data>
bool DBHandler<Data>::modifyMetaData(Key key, std::string property, std::string newVal, bool typeOfOp) {
	std::cout << "--------------------------------- \n";
	std::cout << "Demonstrating reqirement 4 : Updating metadata information \n";
	std::cout << " Property " << property << " is being updated to new value " << newVal << "\n";
	std::cout << "--------------------------------- \n";
	if (dbObj.updateMetadata(key,property,newVal,typeOfOp))
		return true;
	else
		return false;
}


template<typename Data>
bool DBHandler<Data>::modifyData(Key key, Data data) {
	if (dbObj.updateData(key,data))
		return true;
	else
		return false;

}

template<typename Data>
std::vector<std::string> DBHandler<Data>::retrieveChildrenList(Key key) {
	std::cout << "--------------------------------------------------- \n";
	std::cout << "Requirement 7: Getting children of key " << key << "\n";
	std::cout << "--------------------------------------------------- \n";
	std::vector<Key> childrenVect = dbObj.getChildren(key);
	if (childrenVect.size() != 0) {
		return childrenVect;
	}
	else {
		std::vector<Key> emptyVect;
		std::cout << "\n";
		std::cout << " Element does not contain any children" << "\n";
		std::cout << "\n";
		return emptyVect;
	}
}

template<typename Data>
void DBHandler<Data>::addChild(Key key,std::string child) {
	Element<Data> elem = dbObj.value(key);
	std::vector<Key> childrenVect = elem.children;
	childrenVect.push_back(child);
	elem.children = childrenVect;
	dbObj.insertChild(key, elem);
}


template<typename Data>
std::vector<std::string> DBHandler<Data>::queryElement(std::vector<std::string> keyList, std::string pattern, std::string property){
	std::vector<Key> queryProd;
	try {
		std::string compareVal;
		std::cout << "--------------Requirement 7: Checking if pattern " << pattern << " is present in property " << property << " of each element------------------ \n";
		if (property.compare("Keys") == 0) {
			for (Key key : keyList) {
				if (key.find(pattern) != std::string::npos) {
					queryProd.push_back(key);
				}}}
		else if (property.compare("Item name") == 0) {
			for (Key key : keyList) {
				Element<Data> elem = dbObj.value(key);
				std::string nameOfelem = elem.name;
				if (nameOfelem.find(pattern) != std::string::npos) {
					queryProd.push_back(key);
				}}}
		else if (property.compare("Category") == 0) {
			for (Key key : keyList) {
				Element<Data> elem = dbObj.value(key);
				std::string categoryOfElem = elem.category;
				if (categoryOfElem.find(pattern) != std::string::npos) {
					queryProd.push_back(key);
				}}}
		else if (property.compare("Data") == 0) {
			for (Key key : keyList) {
				Element<Data> elem = dbObj.value(key);
				if (typeid(Data) == typeid(int)) {
					std::string val = Convert<Data>::toString(elem.data);
					if (val.find(pattern) != std::string::npos) {
						queryProd.push_back(key);
					}}
				else if (typeid(Data) == typeid(std::string)) {
					std::string val = Convert<Data>::toString(elem.data);
					if (val.find(pattern) != std::string::npos) {
						queryProd.push_back(key);
					}}
				else
				{
					std::cout << "Invalid type specifed" << "\n";
				}}}
		else
		{
			std::cout << "Invalid query parameter" << property << "\n";
		}}
	catch (std::exception e) {
		std::cout << e.what();}
	return queryProd;
}


template<typename Data>
std::vector<std::string> DBHandler<Data>::queryElemRegex(std::vector<std::string> keyList, std::string regexPat, std::string property) {
	std::cout << "Requirement 12:Querying for pattern " << regexPat << "in each element's " << property << " using regular expressions \n";
	std::vector<Key> queryProd;
	try {
		std::regex reg(regexPat);
		if (property.compare("Keys") == 0) {
			for (Key key : keyList) {
				if (std::regex_match(key, reg)) {
					queryProd.push_back(key);
				}}}
		else if (property.compare("Item name") == 0) {
			for (Key key : keyList) {
				Element<Data> elem = dbObj.value(key);
				std::string nameOfelem = elem.name;
				if (std::regex_match(nameOfelem, reg)) {
					queryProd.push_back(key);
				}}}
		else if (property.compare("Category") == 0) {
			for (Key key : keyList) {
				Element<Data> elem = dbObj.value(key);
				std::string categoryOfElem = elem.category;
				if (std::regex_match(categoryOfElem, reg)) {
					queryProd.push_back(key);
				}}}
		else if (property.compare("Data") == 0) {
			for (Key key : keyList) {
				Element<Data> elem = dbObj.value(key);
				if (typeid(Data) == typeid(int)) {

					std::string val = Convert<Data>::toString(elem.data);
					if (std::regex_match(val, reg)) {
						queryProd.push_back(key);
					}}
				else if (typeid(Data) == typeid(std::string)) {
					std::string val = Convert<Data>::toString(elem.data);
					if (std::regex_match(val, reg)) {
						queryProd.push_back(key);
					}}
				else {
					std::cout << "Invalid type specifed" << "\n";
				}}}
		else {
			std::cout << "Invalid query parameter" << property << "\n";
		}}
	catch (std::exception e) {
		std::cout << e.what();
	}
	return queryProd;
}


template<typename Data>
time_t  DBHandler<Data>::DateTimeFromString(std::string Dt) {
	time_t tmDat;
	try {
		struct tm dtim = { 0 };
		int year = 0;
		int month = 0;
		int day = 0;
		int hour = 0;
		int min = 0;
		int sec = 0;
		const char* DtStr = Dt.c_str();
		sscanf_s(DtStr, "%d-%d-%d.%d:%d:%d", &year, &month, &day, &hour, &min, &sec);
		dtim.tm_year = year - 1900;
		dtim.tm_mon = month - 1;
		dtim.tm_mday = day;
		dtim.tm_hour = hour;
		dtim.tm_min = min;
		dtim.tm_sec = sec;
		tmDat = mktime(&dtim);
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	return tmDat;
}


template<typename Data>
std::string DBHandler<Data>::DateTimeToString(time_t time) {
	try {
		struct tm tstruct = {};
		char charArr[80];
		localtime_s(&tstruct, &t);
		strftime(charArr, sizeof(charArr), "%Y-%m-%d.%X", &tstruct);
		std::string dTim(charArr);
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	return dTim;
}

template<typename Data>
std::vector<std::string> DBHandler<Data>::queryTime(std::string startTime, std::string endTime) {
	std::cout << "-------------------------------------------------------------------------- \n";
	std::cout << "Requirement 7:Checking if element was stored within specified time frame \n";
	std::cout << "-------------------------------------------------------------------------- \n";
	std::vector<Key> retVect;
	try {
		std::vector<Key> dtVect = dbObj.keys();
		std::cout << "Number of elements in the database are " << dtVect.size() << "\n";
		std::cout << "\n";
		time_t stTime = DateTimeFromString(startTime);
		for (Key k : dtVect) {
			Element<Data> elem = dbObj.value(k);
			time_t storeDt = elem.timeDate;
			if (endTime == "DEFAULT") {
				time_t eTime = time(0);
				if (storeDt > stTime && storeDt < eTime) {
					retVect.push_back(k);
				}
				else
					std::cout << "Element not stored within specified time" << "\n";
			}
			else {
				time_t enTime = DateTimeFromString(endTime);
				if (storeDt > stTime && storeDt < enTime) {
					retVect.push_back(k);
				}
				else
					std::cout << "Element not stored within specified time" << "\n";
			}
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
	return retVect;
}


template<typename Data>
std::vector<std::string> DBHandler<Data>::formKeyUnion(std::vector<std::string> keySet1, std::vector<std::string> keySet2) {
	std::cout << "--------------------------------------------- \n";
	std::cout << "Requirement 9:Union of vectors \n";
	std::cout << "--------------------------------------------- \n";
	bool keyPresent = false;
	int outerLoop = 0;
	int innerLoop = 0;
	int keySet1_size = keySet1.size();
	int keySet2_size = keySet2.size();
	std::vector<Key> keyVect;
	for (outerLoop = 0; outerLoop < keySet1_size; outerLoop++) {
		for (innerLoop = 0; innerLoop < keySet2_size; innerLoop++) {
			if (keySet1[outerLoop] == keySet2[innerLoop]) {
				keyPresent = true;
			}
			else
				keyPresent = false;
		}
		if (innerLoop == (keySet2_size - 1) && keyPresent == false) {
			keySet2.push_back(keySet1[outerLoop]);
		}
	}
	return keySet2;
}

