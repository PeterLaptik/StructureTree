#include <iostream>
#include "test_item.h"
#include "../include/node.h"

Node<TestItem>* create_tree(void);

int main(int argc, char **argv)
{
    std::cout<<"Started..."<<std::endl;
    // Create simple assembly
    Node<TestItem> *root = create_tree();
    root->ToConsole();
    Node<TestItem>::LinkOutput();
    delete root;
    Node<TestItem>::LinkOutput();
    return 0;
}


Node<TestItem>* create_tree()
{
    // Following structure is being created:
    // root assembly
    // |
    // |--Assembly_1
    // |
    // |--Assembly_2
    // |
    // |--Assembly_3
    TestItem *item_root = new TestItem("Root assembly");
    TestItem *item_1 = new TestItem("Assembly_1");
    TestItem *item_2 = new TestItem("Assembly_2");
    TestItem *item_3 = new TestItem("Assembly_3");
    TestItem *multi_item = new TestItem("MultiDetail 1");
    Node<TestItem> *root = new Node<TestItem>(item_root);
    Node<TestItem> *assembly_1 = new Node<TestItem>(item_1);
    Node<TestItem> *a_1 = assembly_1->AppendChild(new TestItem("Detail_A"));
    assembly_1->AppendChild(new TestItem("Detail_B"))->AppendChild(new TestItem("det_B.1"));
    assembly_1->AppendChild(new Node<TestItem>());    // empty node
    a_1->AppendChild(new TestItem("Det_A.1"));
    a_1->AppendChild(new TestItem("Det_A.2"));
    root->AppendChild(assembly_1);
    Node<TestItem> *asm2_node = root->AppendChild(new Node<TestItem>(item_2));
    Node<TestItem> *asm3_node = root->AppendChild(new Node<TestItem>(item_3))->AppendChild(new TestItem("det_X.3"));
    asm2_node->AppendChild(multi_item);
    asm3_node->AppendChild(multi_item);
    return root;
}
