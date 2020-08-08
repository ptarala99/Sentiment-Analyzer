#define CATCH_CONFIG_RUNNER
#include <iostream>
#include "analyzer.h"
#include "dsstring.h"
#include "tweet.h"
#include "catch.hpp"
#include "dsvector.h"
using namespace std;

/*
 * create analyzer and then read the tweets
 * train algorithm, then finally test the algorithm
 * also has mode to run tests for DSVector class
 */
int main(int argc, char* argv[])
{
    if(argc == 1){
        return Catch::Session().run();
     }
    Analyzer* analyzer = new Analyzer();
    analyzer->readtweets(argv);
    analyzer->train(argv);
    analyzer->test(argv);

    delete analyzer;
}
