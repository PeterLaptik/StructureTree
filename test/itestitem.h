#ifndef ITESTNODE_H_INCLUDED
#define ITESTNODE_H_INCLUDED

#include <string>

// An item interface for structure tree testing and debugging
// The item has to implement the methods below to return data for console output in debugging mode
class ITestItem
{
    // Node name for output to console
    virtual std::string GetName(void) = 0;
};

#endif // ITESTNODE_H_INCLUDED
