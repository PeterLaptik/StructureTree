#ifndef TEST_NODE_H_INCLUDED
#define TEST_NODE_H_INCLUDED

#include "itestitem.h"

class TestItem: public ITestItem
{
    public:
        TestItem(std::string name)
        {
            this->name=name;
        }

        virtual ~TestItem()
        { }

        virtual std::string GetName(void)
        {
            return name;
        }

    private:
        std::string name;

};

#endif // TEST_NODE_H_INCLUDED
