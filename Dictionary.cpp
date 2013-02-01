/*
 * Onur Yilmaz
 * May 11, 2012
 */

// Headers
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace std;



// Struct for synonym list using linked lists

struct Synonym {
    char * kelime;
    Synonym* next;
    Synonym* previous;
};

// Class for nodes in dictionary prefix tree

// content : actual content of node
// endOrNot: shows whether there is an end of word
// children: vector for children nodes
// synonymList: list for synonym list

class DictionaryNode {
public:

    DictionaryNode(char insert, int end);
    DictionaryNode();
    ~DictionaryNode();
    void insertIntoEmptyNode(char*, DictionaryNode*);
    void insertSynonyms(DictionaryNode* node, vector<const char*> synonymList);
    
    char content;
    bool endOrNot;
    vector<DictionaryNode*> children;
    vector<const char*> synonymList;

private:
};

// Main class for dictionary
// This class uses vectors for dictionary nodes

class Dictionary {
public:
    Dictionary();
    ~Dictionary();
    bool insert(const char* item);
    bool check(const char* item);
    void insertSynonyms(DictionaryNode* node, vector<const char*> synonymVector);
    bool addSynonyms(const char* itemOfInterest, const std::vector<const char*> &synonyms);
    bool showSynonyms(const char* itemOfInterest) const;
    Dictionary(const Dictionary &d);
    Dictionary(const std::vector<const char*> &list);
    Dictionary& operator=(const Dictionary &other);
    std::vector<const char*> toList()  ;
    void toListNode(const DictionaryNode* node, vector<char> append) ;
    void show();
    Dictionary operator+ (Dictionary& other);
    Dictionary operator- (const Dictionary& other) const;
    Dictionary operator-= (const Dictionary& other);
    Dictionary operator+= (const Dictionary& other);
    vector<DictionaryNode*> mainDictionary;
    vector<const char*> DictionaryList;
};

// Create empty dictionary node

DictionaryNode::DictionaryNode() {
    content = ' ';
    endOrNot = false;

}


// Destructor for dictionary node

DictionaryNode::~DictionaryNode() {

    delete &content;
    delete &endOrNot;
    children.clear();
    synonymList.clear();

}

/*
 * Copy constructor for Dictionary
 */
Dictionary::Dictionary(const Dictionary &d) {
    for (int i = 0; i < d.mainDictionary.size(); i++)
        mainDictionary.push_back(d.mainDictionary[i]);

}

/*
 * Memberwise assignment
 */
Dictionary& Dictionary::operator=(const Dictionary &other) {

    for (int i = 0; i < other.mainDictionary.size(); i++)
        mainDictionary.push_back(other.mainDictionary[i]);

}

/*
 * Empty dictionary
 */
Dictionary::Dictionary() {
    //
}

/*
 * This constructor will create a dictionary containing all the words in the 
 * given list
 */
Dictionary::Dictionary(const std::vector<const char*> &list) {
    for (int k = 0; k < list.size(); k++) {
        insert(list[k]);
    }
}

/*
 * Destructor for dictionary
 */
Dictionary::~Dictionary() {
    mainDictionary.clear();
}

// Function for printing out dictionary node

void showNode(DictionaryNode* node, int priorIdent) {

    for (int i = 0; i < priorIdent; i++)
        cout << " ";

    cout << node->content << "-->";

    if (node->endOrNot) cout << "*" << endl;
    else cout << endl;

    priorIdent++;

    for (int i = 0; i < node->children.size(); i++) {

        showNode((node->children)[i], priorIdent);

    }
}

/*
 * Implementation of the show function that complies with the the 
 * explanation in the sample I/O section. 
 */
void Dictionary::show() {

    if (mainDictionary.size() == 0)
        cout << " " << endl;
    else {
        cout << " " << endl;
        for (int i = 0; i < mainDictionary.size(); i++)
            showNode(mainDictionary[i], 0);
    }
}

/*
 * Function for inserting a character array into empty dictionary node
 */

void insertIntoEmptyNode(char* wordToBeAdded, DictionaryNode* nodeToBeAdded) {

    if (wordToBeAdded[0] != '\0') {
        nodeToBeAdded->content = wordToBeAdded[0];

        wordToBeAdded++;

        int EndOfWord;
        if (wordToBeAdded[0] != '\0') {

            for (int i = 0; wordToBeAdded[0] != '\0'; wordToBeAdded++, i++) {

                DictionaryNode* nodeToBeAddedChildren = new DictionaryNode();
                nodeToBeAddedChildren->content = wordToBeAdded[0];
                (nodeToBeAdded->children).push_back(nodeToBeAddedChildren);

                nodeToBeAdded = nodeToBeAdded->children.front();

            }
            nodeToBeAdded->endOrNot = true;
        } else
            nodeToBeAdded->endOrNot = true;
    }
}

/* Function for appending character array to the children of a dictionary
 * node
 */
void appendChildrenTo(char* wordToBeAdded, DictionaryNode* toChildren) {
    int ChildrenPlace;

    // If empty word
    if (wordToBeAdded[0] == '\0') return;

    // If children list  is empty
    if (toChildren->children.size() == 0) {
        DictionaryNode* nodeToBeAddedToChildren = new DictionaryNode();
        insertIntoEmptyNode(wordToBeAdded, nodeToBeAddedToChildren);
        toChildren->children.push_back(nodeToBeAddedToChildren);
        return;
    } else {

        for (int i = 0; i < toChildren->children.size(); i++) {
            if (toChildren->children[i]->content == wordToBeAdded[0])
                ChildrenPlace = i;
        }

    }
    // If word will be added into a child 
    if (ChildrenPlace < toChildren->children.size()) {

        wordToBeAdded++;

        if (wordToBeAdded[0] == '\0') toChildren->endOrNot = true;
        else
            appendChildrenTo(wordToBeAdded, (toChildren->children[ChildrenPlace]));

        return;
    }// If word will be added into first or last position
    else {
        // Insert to beginning
        if (((toChildren->children.front())->content) > wordToBeAdded[0]) {

            DictionaryNode* nodeToBeAdded = new DictionaryNode();
            insertIntoEmptyNode(wordToBeAdded, nodeToBeAdded);
            toChildren->children.insert(toChildren->children.begin(), nodeToBeAdded);

            return;
            // Insert to end
        } else if (((toChildren->children.back())->content) < wordToBeAdded[0]) {

            DictionaryNode* nodeToBeAdded = new DictionaryNode();
            insertIntoEmptyNode(wordToBeAdded, nodeToBeAdded);
            toChildren->children.insert(toChildren->children.end(), nodeToBeAdded);

            return;

        } else;
    }


}

/*
 * Function for finding the character array in the dictionary node
 */
DictionaryNode* findInNode(char* toFound, DictionaryNode* searchIn) {

    if (toFound[0] == '\0') {
        return NULL;
    }
    if (toFound[0] != searchIn->content) {
        return NULL;
    }
    if (toFound[1] == '\0' && searchIn->endOrNot == true) return searchIn;
    if (toFound[1] == '\0' && searchIn->endOrNot == false) {
        return NULL;
    }

    // Continue search in children
    toFound++;
    for (int i = 0; i < searchIn->children.size(); i++) {
        if (findInNode(toFound, searchIn->children[i]) != NULL) return searchIn->children[i];

    }
    return NULL;

}

/* 
 * Function for inserting synonyms to a dictionary node
 */
void Dictionary::insertSynonyms(DictionaryNode* node, vector<const char*> synonymVector) {

    for (int i = 0; i < synonymVector.size(); i++) {
        node->synonymList.push_back(synonymVector[i]);

    }
}

/*
 * This function will check if a given item exists in the dictionary. 
 * If it exists, check will return true 
 */
bool Dictionary::check(const char* item) {
    char* wordToBeChecked = const_cast<char *> (item);

    for (int i = 0; i < mainDictionary.size(); i++) {
        if (findInNode(wordToBeChecked, mainDictionary[i]) != NULL) return true;
    }
    return false;
}

/* This function will insert a list of synonyms to the entry itemOfInterest
 * in the dictionary, and return true for success.
 * If no such entry exists, it should return false.
 */
bool Dictionary::addSynonyms(const char* itemOfInterest, const std::vector<const char*> &synonyms) {

    char* wordToBeChecked = const_cast<char *> (itemOfInterest);
    DictionaryNode* found;

    for (int i = 0; i < mainDictionary.size(); i++) {
        found = findInNode(wordToBeChecked, mainDictionary[i]);
        if (found != NULL) {
            insertSynonyms(found, synonyms);
            return true;
        }

    }
    return false;
}

/*
 * Given an item, this function should show all of its synonyms one per line,
 * return false if the entry does not exists.
 */
bool Dictionary::showSynonyms(const char* itemOfInterest) const {
    char* wordToBeChecked = const_cast<char *> (itemOfInterest);
    DictionaryNode* found;
    for (int i = 0; i < mainDictionary.size(); i++) {
        found = findInNode(wordToBeChecked, mainDictionary[i]);
        if (found != NULL) {

            for (int j = 0; j < found->synonymList.size(); j++)
                cout << found->synonymList[j] << endl;

            return true;
        }

    }
    return false;
}

/*
 * This function will insert a given item to the dictionary in the first argument.
 * If the item already exists in the dictionary, it is skipped and the function
 * should return false.
 */

bool Dictionary::insert(const char* item) {

    if (check(item) == true) return false;

    // Variable declarations
    int ChildrenPlace;
    char* wordToBeAdded = const_cast<char *> (item);

    // If the dictionary is empty
    if (mainDictionary.size() == 0) {

        DictionaryNode* nodeToBeAdded = new DictionaryNode();
        insertIntoEmptyNode(wordToBeAdded, nodeToBeAdded);
        mainDictionary.push_back(nodeToBeAdded);
        return true;

        // If the word comes at the beginning of dictionary   
    } else if ((mainDictionary.front()->content) > item[0]) {

        vector<DictionaryNode*>::iterator it;
        it = mainDictionary.begin();

        DictionaryNode* nodeToBeAdded = new DictionaryNode();
        insertIntoEmptyNode(wordToBeAdded, nodeToBeAdded);
        it = mainDictionary.insert(it, nodeToBeAdded);

        return true;
        // If the word comes at the end of dictionary   
    } else if ((mainDictionary.back()->content) < item[0]) {

        vector<DictionaryNode*>::iterator it;
        it = mainDictionary.end();

        DictionaryNode* nodeToBeAdded = new DictionaryNode();
        insertIntoEmptyNode(wordToBeAdded, nodeToBeAdded);
        it = mainDictionary.insert(it, nodeToBeAdded);
        return true;

    } else
        // The word will be added to a children
    {
        for (int i = 0; i < mainDictionary.size(); i++) {
            if (mainDictionary[i]->content == item[0])
                ChildrenPlace = i;
        }

        wordToBeAdded++;

        if (wordToBeAdded[0] == '\0') mainDictionary[ChildrenPlace]->endOrNot = true;
        else
            appendChildrenTo(wordToBeAdded, (mainDictionary[ChildrenPlace]));
        return true;
    }
}

/* toList function for Dictionary node
 * It uses a char vector for holding previous characters
 */
void Dictionary::toListNode(const DictionaryNode* node, vector<char> append) {

    append.push_back(node->content);

    char* eklenecek = (char *) malloc(sizeof (char) *100);
    int i;

    if (node->endOrNot) {
        for (i = 0; i < append.size(); i++)
            eklenecek[i] = append[i];
        eklenecek[i] = '\0';
        const char* send = eklenecek;
        DictionaryList.insert(DictionaryList.end(), send);
    }

    for (int k = 0; k < node->children.size(); k++)
        toListNode(node->children[k], append);
}

/*
 * This function will return an array of item objects containing
 * all the words in the dictionary.
 */
std::vector <const char*> Dictionary::toList() {
    vector<char> append;
    const DictionaryNode* gonder;

    for (int i = 0; i < mainDictionary.size(); i++) {
        gonder = mainDictionary[i];
        toListNode(gonder, append);
        append.clear();
    }

    return DictionaryList;
}

void merger(DictionaryNode* thisDict, vector<DictionaryNode*> intoList) {
    for (int k = 0; k < intoList.size(); k++) {
        if (intoList[k]->content != thisDict->content)
            intoList.push_back(thisDict);

    }
}

/* 
 * This function will merge the dictionary in the first argument into the
 * current dictionary. This operator does not modify the original object.
 */
Dictionary Dictionary::operator+(Dictionary& other) {

    for (int i = 0; i < other.mainDictionary.size(); i++)
        merger(other.mainDictionary[i], mainDictionary);
    
}

/* 
 * This function take the difference of the Dictionary object from the input 
 * dictionary (b) and return it as a separate Dictionary object. This operator
 * does not modify the original object.
 */


Dictionary Dictionary::operator-(const Dictionary& other) const {
}

/*
 * These are the versions of - and + where the result is set to the 
 * original Dictionary object.
 */
Dictionary Dictionary::operator-=(const Dictionary& other) {

}

Dictionary Dictionary::operator+=(const Dictionary& other) {
}

/*
 * End of code
 * May 11, 2012
*/

