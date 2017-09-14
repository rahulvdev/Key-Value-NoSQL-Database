/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "NoSqlDb.h"
#include <iostream>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main()
{
  std::cout << "\n  Demonstrating NoSql Helper Code";
  std::cout << "\n =================================\n";

  std::cout << "\n  Creating and saving NoSqlDb elements with string data";
  std::cout << "\n -------------------------------------------------------\n";

  NoSqlDb<StrData> db;


  Element<StrData> strelem1;
  strelem1.name = "elem1";
  strelem1.category = "test";
  strelem1.data = "elem1's StrData";

  db.save(strelem1.name, strelem1);

  Element<StrData> strelem2;
  strelem2.name = "elem2";
  strelem2.category = "test";
  strelem2.data = "elem2's StrData";

  db.save(strelem2.name, strelem2);

  Element<StrData> strelem3;
  strelem3.name = "elem3";
  strelem3.category = "test";
  strelem3.data = "elem3's StrData";

  db.save(strelem3.name, strelem3);

  std::cout << "\n  Retrieving elements from NoSqlDb<string>";
  std::cout << "\n ------------------------------------------\n";

  std::cout << "\n  size of db = " << db.count() << "\n";
  Keys keys = db.keys();
  for (Key key : keys)
  {
    std::cout << "\n  " << key << ":";
    std::cout << db.value(key).show();
  }
  std::cout << "\n\n";

  std::cout << "\n  Creating and saving NoSqlDb elements with int data";
  std::cout << "\n ----------------------------------------------------\n";

 /* std::cout << "QUERYING DEMO" << "\n";
  std::cout << "----------------------------" << "\n";
  std::vector<std::string> initKeySet = db.keys();
  std::vector<Key> patternMatchKeys=db.patternMatch(initKeySet,"1","Keys" );
  std::vector<Key> dataMatch = db.patternMatch(initKeySet, "2", "Data");
  int dataVectSize = dataMatch.size();
  for (int i = 0; i < dataVectSize; i++) {
	  std::cout << dataMatch[i] << "\n";
	  std::cout << "\n";
  }
  int vectorSize = patternMatchKeys.size();
  for (int i = 0; i < vectorSize; i++) {
	  std::cout << patternMatchKeys[i] << "\n";
  }*/

  NoSqlDb<intData> idb;

  Element<intData> ielem1;
  std::vector<Key> vect;
  vect.push_back("elem2");
  vect.push_back("What up yo");
  ielem1.name = "elem1";
  ielem1.category = "test";
  ielem1.data = 1;
  ielem1.children = vect;

  idb.save(ielem1.name, ielem1);

  Element<intData> ielem2;
  ielem2.name = "elem2";
  ielem2.category = "test";
  ielem2.data = 2;

  idb.save(ielem2.name, ielem2);

  Element<intData> ielem3;
  ielem3.name = "elem3";
  ielem3.category = "test";
  ielem3.data = 3;

  idb.save(ielem3.name, ielem3);

 
  std::cout << "-----------------------------------------------------" << "/n";
  std::cout << "\n";
  Keys initKeySet = idb.keys();

  std::cout << "\n  Retrieving elements from NoSqlDb<int>";
  std::cout << "\n ---------------------------------------\n";

  std::cout << "\n  size of idb = " << idb.count() << "\n";
  Keys ikeys = idb.keys();
  for (Key key : ikeys)
  {
    std::cout << "\n  " << key << ":";
    std::cout << idb.value(key).show();
  }
  std::cout << "\n\n";

   ikeys = idb.keys();
  for (Key key : ikeys)
  {
	  std::cout << "\n  " << key << ":";
	  std::cout << idb.value(key).show();
  }
  std::cout << "\n\n";

//  idb.toXml();
  std::getchar();
}