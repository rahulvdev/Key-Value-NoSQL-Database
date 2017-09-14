#pragma once
/////////////////////////////////////////////////////////////////////
// NoSqlDb.h - key/value pair in-memory database                   //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017	   //
// Edited:Rahul Vijaydev                                           //
/////////////////////////////////////////////////////////////////////

//Description:This file provides templates for the NoSql database and associated elements.
//This file provides memeber functions used to access all database operations
//like insertion of database elements,deletion of elements,updating metadata and data values
//and querying the database content.

//Member functions:
//Element<Data> value(Key key);
//bool deleteElement(Key key);
//void deleteAssociatedChild(Key key);
//bool updateMetadata(Key key,std::string paramToModify,std::string value,bool modify);
//bool updateData(Key key, Data data);
//size_t count();
//std::vector<std::string> getChildren(Key key);
//void insertChild(Key key,Element<Data> elem);
//void writeToXmLFile(std::string xmlData, std::string nameOfFile);
//std::string readFromXmlFile(std::string nameOfFile);
//void setxmlFilePath(std::string fname);
//


#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include<ctime>
#include <vector>
#include<iterator>
#include <iomanip>
#include <iostream>
#include <memory>
#include "../Convert/Convert.h"
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

using namespace XmlProcessing;

/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair

template<typename Data>
class Element
{
public:
  using Name = std::string;
  using Category = std::string;
  using TimeDate = time_t;
  using Key = std::string;
  using Children = std::vector<std::string>;
  using Description = std::string;

  Property<Name> name;            // metadata
  Property<Category> category;    // metadata
  Property<TimeDate> timeDate;    // metadata
  Property<Data> data;			  //data 
  Property<Children> children;	  //metadata
  Property<Description> descr;	  //metadata
  

  std::string show();
};

template<typename Data>
std::string Element<Data>::show()
{
  // show children when you've implemented them

  std::ostringstream out;
  out.setf(std::ios::adjustfield, std::ios::left);
  out << "\n    " << std::setw(8) << "name"     << " : " << name;
  out << "\n    " << std::setw(8) << "category" << " : " << category;
  out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
  out << "\n    " << std::setw(8) << "data"     << " : " << data;
  std::vector<Key> vect = children;
  for (std::vector<Key>::iterator it =vect.begin() ; it != vect.end(); ++it) {
  out << "\n    " << std::setw(8) << "Child" << " : " << *it;
  }
  out << "\n";
 
  return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements

template<typename Data>
class NoSqlDb
{
public:
  using Sptr = std::shared_ptr<AbstractXmlElement>;
  using Key = std::string;
  using Keys = std::vector<Key>;
  Keys keys();
  bool save(Key key, Element<Data> elem);
  void toXml(std::string fname);
  void fromXml(std::string& xml);

  Element<Data> value(Key key);
  bool deleteElement(Key key);
  void deleteAssociatedChild(Key key);
  bool updateMetadata(Key key,std::string paramToModify,std::string value,bool modify);
  bool updateData(Key key, Data data);
  size_t count();
  //Query methods
  std::vector<std::string> getChildren(Key key);
  void insertChild(Key key,Element<Data> elem);
  void writeToXmLFile(std::string xmlData, std::string nameOfFile);
  std::string readFromXmlFile(std::string nameOfFile);
  void setxmlFilePath(std::string fname);
  ~NoSqlDb()
  {
	  toXml(xmlFileName);
  }


private:

  using Item = std::pair<Key, Element<Data>>;
  std::string xmlFileName;

  std::unordered_map<Key,Element<Data>> store; 
};

//Returns set of all keys from the DB

template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
  Keys keys;
  for (Item item : store)
  {
    keys.push_back(item.first);
  }
  return keys;
}

//Deletes child value of a particular element when the element associated with the child
//is removed from the DB

template<typename Data>
void NoSqlDb<Data>::deleteAssociatedChild(Key localKey) {
	try {
		Keys k = keys();
		for (std::string ke : k)
		{
			Element<Data> ele = store[ke];
			std::vector<Key> children = ele.children;
			std::vector<std::string> tempChild = children;
			//iterating over children vector
			for (std::string ke : tempChild) {

				if (ke == localKey) {
					//children.erase(localKey);
					children.erase(std::remove(children.begin(), children.end(), localKey), children.end());
				}
			}
			ele.children = children;
			store[ke] = ele;
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
} 

//Deletes element corresponding to a particular key

template<typename Data>
bool NoSqlDb<Data>::deleteElement(Key key) {
	if (store.find(key) != store.end()) {
		store.erase(key);
		deleteAssociatedChild(key);
		return true;
	}
	else
		return false;
}

//Saves an element to the DB

template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
  if(store.find(key) != store.end())
    return false;
  store[key] = elem;
  return true;
}

//Returns the element associated with a particular key

template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	std::cout << "----------------------------------------------------\n";
	std::cout << "Requirement 7: Getting value for key " << key << "\n";
	std::cout << "----------------------------------------------------\n";

  if (store.find(key) != store.end())
    return store[key];
  return Element<Data>();
}

//Used to update metadata information of a particular element

template<typename Data>
bool NoSqlDb<Data>::updateMetadata(Key key, std::string paramToModify, std::string val, bool modify) {
	try{
	std::vector<Key> childrenVect;
	if (store.find(key) != store.end()) {
		Element<Data> elem = value(key);
		if (paramToModify == "category") {
			elem.category = val;
			store[key] = elem;
			return true;
		}
		else if (paramToModify == "name") {
			elem.name = val;
			store[key] = elem;
			return true;
		}
		else if (paramToModify == "children") {
			if (modify == false) {
				childrenVect = elem.children;
				childrenVect.push_back(val);
				elem.children = childrenVect;
				store[key] = elem;
				return true;
			}
			else if (modify == true) {
				childrenVect = elem.children;
				childrenVect.erase(std::remove(childrenVect.begin(), childrenVect.end(), val), childrenVect.end());
				elem.children = childrenVect;
				store[key] = elem;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		return true;
	}
	else
		return false;
}
	catch (std::exception e) {
		std::cout << e.what();
		return false;
	}
}

//Used to update data associated with a particular element

template<typename Data>
bool NoSqlDb<Data>::updateData(Key key, Data data) {
	if (store.find(key) != store.end()) {
		Element<Data> elem = store[key];
		elem.data = data;
		store[key] = elem;
		return true;
	}
	else
		return false;
}

//Used to write to an XML file

template<typename Data>
void NoSqlDb<Data>::toXml(std::string fname) {
	try{
	XmlDocument doc;
	std::vector<Key> keyVect = keys();
	Sptr root = makeTaggedElement("DB");
	doc.docElement() = root;
	for (Key key : keyVect) {
		Sptr pElementTag = makeTaggedElement("Element");
		Element<Data> elem = value(key);
		Sptr pKeytag = makeTaggedElement("Key");
		Sptr pkeyText = makeTextElement(key);
		pKeytag->addChild(pkeyText);
		Sptr pNameTag = makeTaggedElement("Name");
		Sptr pNameText = makeTextElement(elem.name);
		pNameTag->addChild(pNameText);
		Sptr pDescTag = makeTaggedElement("Description");
		Sptr pDescText = makeTextElement(elem.descr);
		pDescTag->addChild(pNameText);
		Sptr pCategoryTag = makeTaggedElement("Category");
		Sptr pCategoryText = makeTextElement(elem.category);
		pCategoryTag->addChild(pNameText);
		std::vector<std::string> childVect = elem.children;
		Sptr pChildrenTag = makeTaggedElement("Children");
		for (std::string child : childVect) {
			Sptr pChildTag = makeTaggedElement("Child");
			Sptr pChildText = makeTextElement(child);
			pChildTag->addChild(pChildText);
			pChildrenTag->addChild(pChildTag);
		}
		Sptr pDataTag = makeTaggedElement("Data");
		Sptr pDataText = makeTextElement(Convert<Data>::toString(elem.data));
		pDataTag->addChild(pDataText);
		pElementTag->addChild(pKeytag);
		pElementTag->addChild(pNameTag);
		pElementTag->addChild(pDescTag);
		pElementTag->addChild(pCategoryTag);
		pElementTag->addChild(pChildrenTag);
		pElementTag->addChild(pDataTag);
		root->addChild(pElementTag);
	}
	std::cout << "\n  X xml string:" << doc.toString();
	writeToXmLFile(doc.toString(), fname);
}
catch (std::exception e) {
		std::cout << e.what();
	}
}


template<typename Data>
void  NoSqlDb<Data>::writeToXmLFile(std::string xmlData,std::string nameOfFile) {
	try {
		std::ofstream ost(nameOfFile);
		if (ost.is_open()) {
			ost << xmlData;
			ost.close();
		}
		else
		{
			std::cout << "Can not access file \n";
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
}

template<typename Data>
std::string  NoSqlDb<Data>::readFromXmlFile(std::string nameOfFile) {
	std::string xmlData;
	std::string line;
	try {
		std::ifstream inSt(nameOfFile);

		if (inSt.is_open()) {
			while (getline(inSt, line)) {
				xmlData = xmlData + line;
			}
			inSt.close();
		}
		else {
			std::cout << "Can not access file \n";
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
		return xmlData;
}

template<typename Data>
inline void NoSqlDb<Data>::setxmlFilePath(std::string fname)
{
	xmlFileName = fname;
}

//Used to read content from an XML file
template<typename Data>
void NoSqlDb<Data>::fromXml(std::string& xmlFileName) {
	try {
		std::string xml = readFromXmlFile(xmlFileName);
		XmlDocument doc(xml, XmlDocument::str);
		std::vector<Sptr> desc = doc.descendents("Element").select();
		for (Sptr ptr : desc) {
			Element<Data> elem;
			std::string key = ptr->children()[0]->children()[0]->value();
			elem.name = ptr->children()[1]->children()[0]->value();
			elem.descr = ptr->children()[2]->children()[0]->value();
			elem.category = ptr->children()[3]->children()[0]->value();
			elem.data = Convert<Data>::fromString(ptr->children()[5]->children()[0]->value());
			std::vector<Sptr> childPtr = ptr->children()[4]->children();
			std::vector<Key> childVect;
			for (Sptr ptr : childPtr) {
				childVect.push_back(ptr->children()[0]->value());
			}
			elem.children = childVect;
			save(key, elem);
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
}


//Returns the size of the DB

template<typename Data>
size_t NoSqlDb<Data>::count()
{
  return store.size();
}


//Returns the list of all children associated with a particular element

template<typename Data>
std::vector<std::string> NoSqlDb<Data>::getChildren(Key key) {
	if (store.find(key) != store.end()) {
		Element<Data> elem = store[key];
		return elem.children;
	}
	else {
		std::vector<Key> emptyVect;
		return emptyVect;
	}
}

template<typename Data>
void NoSqlDb<Data>::insertChild(Key key,Element<Data> elem) {
	store[key] = elem;
}

