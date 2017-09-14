/////////////////////////////////////////////////////////////////////
// DBAccess.cpp - It is used to handle all the database operations //
//                                                                 //
// Rahul Vijaydev, CSE687 - Object Oriented Design, Spring 2017    //
/////////////////////////////////////////////////////////////////////
//
//Description: This file demonstrates calls to the functions in the DBHandler class.
//
#include<iostream>
#include"DBAccess.h"

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main() {


	std::cout << "---------------------------------------------------------- \n";
	std::cout << "String Database \n";
	std::cout << "---------------------------------------------------------- \n";

	DBHandler<StrData> dbH("../XMLfiles/StringDB.xml",10);
	std::vector<Key> childVect1;
	std::vector<Key> childVect2;
	std::vector<Key> childVect3;
	std::vector<Key> childVect4;
	childVect1.push_back("Child1");
	childVect1.push_back("Child2");
	dbH.saveDBElement("elem1", "test", "elem1's StrData","Hello World",childVect1);
	dbH.saveDBElement("elem2", "Code", "elem2's StrData", "Bjarne Stroustrup", childVect2);
	dbH.saveDBElement("elem3", "test", "elem3's StrData", "Big Data", childVect3);
	dbH.saveDBElement("elem4", "test", "elem4's StrData", "Artificial Intelligence", childVect4);
	dbH.toXMl("../XMLfiles/StringDB.xml");

	std::cout << "--------------------------------------------------------------------" << "\n";
	std::cout << "\n";
	std::vector<Key> keyVect=dbH.getKeys();
	std::cout << "-------------------------------------------------------------------------------------------------- \n";
	std::cout << "Demonstrating requirement 2 where each element consists of name,description,category and datetime";
	std::cout << " as metadata and corresponding string data \n";
	std::cout << "-------------------------------------------------------------------------------------------------- \n";
	for (Key key : keyVect) {
		dbH.showElement(key);
	}

	
	dbH.modifyMetaData("elem2","name","Albert Einstein",false);
	dbH.showElement("elem2");
	dbH.modifyMetaData("elem3", "category", "Execution", false);
	dbH.showElement("elem3");
	std::cout << "Adding child to element with key elem1 :Requirement 4 \n";
	dbH.modifyMetaData("elem1", "children", "Child3", false);
	dbH.showElement("elem1");
	std::cout << "Deleting child to element with key elem1 :Requirement 4 \n";
	dbH.modifyMetaData("elem1", "children", "Child1", true);
	dbH.showElement("elem1");

	std::cout << "-----------------------------------------------------" << "\n";

	
	dbH.deleteElement("elem1");
	std::vector<Key> updatedKeyVect = dbH.getKeys();
	for (Key key : updatedKeyVect) {
		dbH.showElement(key);
	}

	dbH.modifyData("elem2","Elon Musk");
	dbH.showElement("elem2");

	std::vector<Key> childrenVect=dbH.retrieveChildrenList("elem2");
	for (Key k : childrenVect) {
		std::cout << "Child " << k << "\n";
		std::cout << "\n";
	}

	dbH.addChild("elem2","elem2Child1");
	dbH.addChild("elem2", "elem2Child2");
	dbH.addChild("elem2", "elem2Child3");
	std::vector<Key> childrenVect1 = dbH.retrieveChildrenList("elem2");
	for (Key k : childrenVect1) {
		std::cout << "Child " << k << "\n";
	}

	std::vector<Key> dateTimeVect=dbH.queryTime("2017-01-01.12:00:00", "2017-03-01.12:00:00");
	if (dateTimeVect.size() != 0) {
		std::cout << "Displaying elements stored within specified time \n";
		for (Key k : dateTimeVect) {
			std::cout << k << "\n";
		}
	}
	else
	{
		std::cout << "None of the elements are within specified time interval \n";
		std::cout << "\n";
	}

	std::cout << "----------------------------------------------------------- \n";
	std::cout << "Query Test \n";
	std::cout << "----------------------------------------------------------- \n";
	std::vector<Key> strKeyList=dbH.getKeys();
	std::vector<Key> resVect=dbH.queryElement(strKeyList,"ert", "Item name");
	for (Key k : resVect) {
		std::cout << "Element with key " << k << " has the pattern -ert- in the item name \n";
		std::cout << "------------------------------------------------------ \n";
	}

	std::vector<Key> resVect1 = dbH.queryElement(strKeyList, "test", "Category");
	for (Key k : resVect) {
		std::cout << "Element with key "<< k << " has the pattern -test- in its category \n";
		std::cout << "------------------------------------------------------ \n";
	}
	std::vector<Key> strQueryProdVect = dbH.queryElement(resVect1, "ig", "Data");
	std::cout << "Requirement 8: Querying with keyset obtained from previous query \n";
	std::cout << "\n";
	for (Key k : strQueryProdVect) {
		std::cout << "Key " << k << " contains the specified pattern \n";
	}

	std::vector<Key> resVect2 = dbH.queryElement(strKeyList, "ial", "Data");
	for (Key k : resVect2) {
		std::cout << "Element with key " << k << " has the pattern -ial- in the item name \n";
		std::cout << "------------------------------------------------------ \n";
	}

	std::vector<Key> resVect3 = dbH.queryElemRegex(strKeyList, "(.*)(io)(.*)", "Category");
	for (Key k : resVect3) {
		std::cout << "Element with key " << k << " has the pattern -(.*)(io)(.*)- in its category \n";
		std::cout << "------------------------------------------------------ \n";
	}

	std::string queryPattern = "arne";
	std::vector<Key> strDataVect = dbH.queryElement(strKeyList,queryPattern,"Data");
	for (Key ke : strDataVect) {
		std::cout << " Element with key " << ke << "contains pattern " << queryPattern << "in its Data \n";
	}

	std::cout << "---------------------------------------------------------- \n";
	std::cout << "Integer Database \n";
	std::cout << "---------------------------------------------------------- \n";

	DBHandler<intData> idbH(" ,",10);
	std::vector<Key> intChildVect1;
	std::vector<Key> intChildVect2;
	std::vector<Key> intChildVect3;
	std::vector<Key> intChildVect4;
	intChildVect1.push_back("Child1");
	intChildVect1.push_back("Child2");
	idbH.saveDBElement("ielem1", "test", "elem1's StrData", 10, childVect1);
	idbH.saveDBElement("ielem2", "Code", "elem2's StrData", 32, childVect2);
	idbH.saveDBElement("ielem3", "test", "elem3's StrData", 56, childVect3);
	idbH.saveDBElement("ielem4", "test", "elem4's StrData", 49, childVect4);


	std::cout << "-------------------------------------------------------------------------------------------------- \n";
	std::cout << "Demonstrating requirement 2 where each element consists of name,description,category and datetime";
	std::cout << " as metadata and corresponding integer data \n";
	std::cout << "-------------------------------------------------------------------------------------------------- \n";
	std::vector<Key> intVect=idbH.getKeys();
	for (Key k : intVect) {
		idbH.showElement(k);
	}

	idbH.deleteElement("ielem4");

	idbH.modifyMetaData("ielem2", "name", "Galileo", false);
	idbH.showElement("ielem2");
	idbH.modifyMetaData("ielem3", "category", "Execution", false);
	idbH.showElement("ielem3");
	std::cout << "Adding child to element with key elem1 :Requirement 4 \n";
	idbH.modifyMetaData("ielem1", "children", "Child3", false);
	idbH.showElement("ielem1");
	std::cout << "Deleting child to element with key elem1 :Requirement 4 \n";
	idbH.modifyMetaData("ielem1", "children", "Child1", true);
	idbH.showElement("ielem1");

	std::cout << "----------------------------------------------------------- \n";
	std::cout << "Query Test \n";
	std::cout << "----------------------------------------------------------- \n";
	std::vector<Key> intKeyList = idbH.getKeys();
	std::vector<Key> intResVect = idbH.queryElement(intKeyList, "lil", "Item name");
	for (Key k : intResVect) {
		std::cout << "Element with key " << k << " has the pattern -lil- in the item name \n";
		std::cout << "------------------------------------------------------ \n";
	}

	std::vector<Key> intResVect1 = idbH.queryElement(intKeyList, "test", "Category");
	for (Key k : intResVect1) {
		std::cout << "Element with key " << k << " has the pattern -test- in its category \n";
		std::cout << "------------------------------------------------------ \n";
	}

	std::vector<Key> queryProdVect = idbH.queryElement(intResVect1,"6","Data");
	std::cout << "Requirement 8: Querying with keyset obtained from previous query \n";
	std::cout << "\n";
	for (Key k : queryProdVect) {
		std::cout << "Key " << k << " contains the specified pattern \n";
	}

	std::string iqueryPattern = "2";
	std::vector<Key> intDataVect = dbH.queryElement(strKeyList, iqueryPattern, "Data");
	for (Key ke : intDataVect) {
		std::cout << " Element with key " << ke << "contains pattern " << iqueryPattern << "in its Data \n";
	}

	std::vector<Key> intResVect2 = idbH.queryElemRegex(intKeyList, "(.*)(io)(.*)", "Category");
	for (Key k : intResVect2) {
		std::cout << "Element with key " << k << " has the pattern -(.*)(io)(.*)- in its category \n";
		std::cout << "------------------------------------------------------ \n";
	}

	std::vector<Key> idateTimeVect = idbH.queryTime("2017-01-01.12:00:00", "2017-03-01.12:00:00");
	if (idateTimeVect.size() != 0) {
		std::cout << "Displaying elements stored within specified time \n";
		for (Key k : idateTimeVect) {
			std::cout << k << "\n";
		}
	}
	else
	{
		std::cout << "None of the elements are within specified time interval \n";
		std::cout << "\n";
	}


	std::getchar();
}