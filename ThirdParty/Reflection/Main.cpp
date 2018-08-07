#include <vector>
#include "Reflect.h"

struct Node 
{
    std::string key;
    int value;
    float fVal;
    std::vector<Node> children;

    REFLECT()       // Enable reflection for this type
};


// Define Node's type descriptor
REFLECT_STRUCT_BEGIN(Node)
REFLECT_STRUCT_MEMBER(key)
REFLECT_STRUCT_MEMBER(value)
REFLECT_STRUCT_MEMBER(fVal)
REFLECT_STRUCT_MEMBER(children)
REFLECT_STRUCT_END()


int main() 
{
    // Create an object of type Node
    Node node = {"apple", 3, 1.23f, {{"banana", 7, {}}, {"cherry", 11, {}}}};

    // Find Node's type descriptor
    reflect::TypeDescriptor* typeDesc = reflect::TypeResolver<Node>::get();

    // Dump a description of the Node object to the console
    typeDesc->dump(&node);

    return 0;
}


/*
===============================================================================
说明：
这个DEMO只能用来反射类的成员变量。
===============================================================================
*/
