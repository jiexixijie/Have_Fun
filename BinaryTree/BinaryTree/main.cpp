#include "stdafx.h"
#include "BinaryTree.h"

using namespace std;

BinaryTree<char>* Init_R();    //初始化一个二叉树
void Visit(char &a) { printf("%c ",a); }


int main(){
	BinaryTree<char> start = *Init_R();
	start.PreOrder_R(Visit);
	start.PreOrder(Visit);
	start.InOrder_R(Visit);
	start.InOrder(Visit);
	start.PostOrder_R(Visit);
	start.PostOrder(Visit);
    return 0;
}

BinaryTree<char>* Init_R() {
	BinaryTree<char> *a = new BinaryTree<char>;
	BinaryTree<char> *b = new BinaryTree<char>;
	BinaryTree<char> *c = new BinaryTree<char>;
	BinaryTree<char> *x = new BinaryTree<char>;
	BinaryTree<char> *y = new BinaryTree<char>;
	//d,e为空指针
	a->MakeTree('E', *x, *y);   //	     A
	b->MakeTree('D', *x, *y);	//	   C   B
	c->MakeTree('C', *a, *b);   //	 E  D
	b->MakeTree('B', *x, *y);       
	a->MakeTree('A', *c, *b);
	b->MakeTree('G', *x, *y);
	c->MakeTree('H', *a, *b);
	return c;
}
