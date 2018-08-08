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
	start.LevelOrder(Visit);
	BinaryTree<char> temp;
	char Pre[] = { 'H','A','C','E','D','B','G' };
	char In[] = { 'E','C','D','A','B','H','G' };
	temp.Gen_PreAIn(Pre, In, 7);
	temp.PreOrder_R(Visit);
	temp.InOrder_R(Visit);
	temp.Print_Parent(Visit,'D');
    return 0;
}

BinaryTree<char>* Init_R() {
	BinaryTree<char> *a = new BinaryTree<char>;
	BinaryTree<char> *b = new BinaryTree<char>;
	BinaryTree<char> *c = new BinaryTree<char>;
	BinaryTree<char> *x = new BinaryTree<char>;
	BinaryTree<char> *y = new BinaryTree<char>;
	//d,e为空指针               //           H
	a->MakeTree('E', *x, *y);   //	     A       G
	b->MakeTree('D', *x, *y);	//	   C   B
	c->MakeTree('C', *a, *b);   //	 E  D
	b->MakeTree('B', *x, *y);       
	a->MakeTree('A', *c, *b);
	b->MakeTree('G', *x, *y);
	c->MakeTree('H', *a, *b);
	return c;
}
