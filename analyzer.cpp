#include "analyzer.h"
#include "dsstring.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <tweet.h>
#include <fstream>


using namespace std;
/*
 * default constructor for analyzer
 * sets all vectors to default values
 */
Analyzer::Analyzer(){

}

/*
 * set all vectors in analyzer to
 * preferred values
 */
Analyzer::Analyzer(DSVector<DSString>myPositiveWords,DSVector<DSString>myNegativeWords,DSVector<DSString>myStopWords, DSVector<tweet>myTweets,DSVector<DSVector<DSString>>myTweetWords)
{
   positiveWords = myPositiveWords;
   negativeWords = myNegativeWords;
   stopWords = myStopWords;
   tweets = myTweets;
   tweetWords = myTweetWords;
}

/*
 * read all tweets in files
 * user char* buffers to read in
 * get the first line out of the way
 * then grab all data fields for the tweets
 * and push the tweets back into the tweets array
 * as well as break tweets into words and put those
 * in a tweetwords vector which has a vector of DSStrings
 */
void Analyzer :: readtweets(char** argv){
   ifstream inFS;
   inFS.open(argv[1]);
   //inFS.open(argv[1]);
   //DSString firstline;
   char* firstline = new char[22];
   int  rownum;
   char* rowtext = new char[7];
   long  idnum;
   char* idnumtext = new char[10];
   DSString username;
   char* usernametext = new char[25];
   char* messagetext = new char[2000];
   DSString message;
   inFS.getline(firstline,50,'\n');
   int i = 0;
   while(!inFS.eof()){
       tweet sometweet;
       inFS.getline(rowtext,10,',');
       rownum = atoi(rowtext);
       sometweet.setrowNum(rownum);
       inFS.getline(idnumtext,14,',');
       idnum = atol(idnumtext);
       sometweet.setIDNum(idnum);
       inFS.getline(usernametext,100,',');
       sometweet.setusername(usernametext);
       inFS.getline(messagetext,2000);
       sometweet.setText(messagetext);
       tweets.push_back(sometweet);
       tweetWords.push_back(breakintoWords(sometweet));
       i++;
      }
   delete [] firstline;
   delete [] rowtext;
   delete [] idnumtext;
   delete [] usernametext;
   delete [] messagetext;
  }

/*
 * break the tweet into words by dividing a tweet down
 * first converting the strings to cstrings and then
 * pushing words into the vector
 * then finally returning a words array
 */
DSVector<DSString> Analyzer :: breakintoWords(tweet sometweet){
     DSString tobreak = sometweet.getText();
     char* something =tobreak.c_str();

     DSVector<DSString> words;
     //int i  = 0;
     char* tok =strtok(something,"- !\"#$%& ()*+,./:;?@[]\\^_`{}|~");
     while (tok != nullptr){
           DSString temp(tok);
           words.push_back(temp);
           tok =strtok(nullptr,"- !\"#$%& ()*+,./:;?@[]\\^_`{}|~");
     }


     return words;
}


/*
 * train algorithm with training data sets
 * find matching ID for each tweet and assign positivity
 * based on ID. then for each tweet put the word in the wordpositivity
 * map  and if it is in a positive tweet increment the words positivity value by 1
 * otherwise decrement its positivity value by 1.
 */
void Analyzer :: train(char* argv[]){
    ifstream inFS(argv[2]);
    char* buffer = new char[40];
    inFS.getline(buffer,40);
    int i = 0;
    while(!inFS.eof()){
        inFS.getline(buffer,40,',');
        inFS.getline(buffer,40,',');
        int positivity = atoi(buffer);
        tweets[i].setPositivity(positivity);
        inFS.getline(buffer,40);
        i++;
    }
    for(int i =0; i < tweets.getSize();i++){
        for(int j = 0; j < tweetWords[i].getSize();j++){
            auto x = wordpositivity.emplace(tweetWords[i][j],0);
            if(tweets[i].getPositivity() == 4)
                x.first->second++;
            else if(tweets[i].getPositivity() == 0)
                x.first->second--;
            else {
                cout << "Error at tweet " << i << ", word " << j << endl;
            }
        }
    }
    delete [] buffer;
}

/*
 * test algorithm using test data
 * clear tweets and tweetwords from train
 * and then repopulate with test data
 * after repopulating go through every tweet
 * and all tweet words. and add  all of the
 * tweetword values then set the positivity to 4 if
 * if the sum is greater than or equal to 0
 * and to 0 if the sum is less than 0
 */
void Analyzer:: test(char* argv[]){
    ifstream inFS(argv[3]);
    ifstream target(argv[4]);
    ofstream outFS(argv[5]);
    tweets.clear();
    tweetWords.clear();
    char* firstline = new char[22];
    int  rownum;
    char* rowtext = new char[7];
    long  idnum;
    char* idnumtext = new char[10];
    DSString username;
    char* usernametext = new char[25];
    char* messagetext = new char[2000];
    DSString message;
    inFS.getline(firstline,50,'\n');
    int i = 0;
    while(!inFS.eof()){
        tweet sometweet;
        inFS.getline(rowtext,10,',');
        rownum = atoi(rowtext);
        sometweet.setrowNum(rownum);
        inFS.getline(idnumtext,14,',');
        idnum = atol(idnumtext);
        sometweet.setIDNum(idnum);
        inFS.getline(usernametext,100,',');
        sometweet.setusername(usernametext);
        inFS.getline(messagetext,2000);
        sometweet.setText(messagetext);
        tweets.push_back(sometweet);
        tweetWords.push_back(breakintoWords(sometweet));
        i++;
       }
    for(int i = 0; i < tweets.getSize();i++){
        int tempsum = 0;
        for(int j=0 ; j < tweetWords[i].getSize(); i++){
         auto x = wordpositivity.find(tweetWords[i][j]);
         int valtoadd = x->second;
         tempsum += valtoadd;
        }
        if(tempsum >= 0)
            tweets[i].setPositivity(4);
        else if(tempsum <0)
            tweets[i].setPositivity(0);
    }

    DSVector<pair<long,char>> testVector;
    char* buffer = new char[100];
    target.getline(firstline,50);
    i = 0;
    double numCorrect = 0.0;
    while(!target.eof())
    {
        target.getline(buffer,100,',');
        target.getline(buffer,100,',');
        int targetNum = atoi(buffer);
        target.getline(buffer,100);
        long id = atol(buffer);
        char correct;
        if(tweets[i].getPositivity() == targetNum) {
            correct = 'c';
            numCorrect++;
        }
        else {
            correct = 'i';
        }
        testVector.push_back(make_pair(id,correct));
        i++;
    }
    delete [] buffer;
    delete [] usernametext;
    delete [] messagetext;
    delete [] idnumtext;
    delete [] rowtext;
    delete [] firstline;
    double accuracy = numCorrect/i;
    outFS << accuracy << endl;
    for(int i = 0; i < testVector.getSize(); i++)
    {
        outFS << testVector[i].first << "," << testVector[i].second << endl;
    }

}










