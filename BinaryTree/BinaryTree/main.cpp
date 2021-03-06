#include "stdafx.h"
#include "BinaryTree.h"

using namespace std;

BinaryTree<char>* Init_C();    //初始化一个二叉树
BinaryTree<int>* Init_I();
void Visit(char &a) { printf("%c ",a); }


int main(){
	printf("初始化:");
	BinaryTree<char> start = *Init_C();
	start.PreOrder_R(Visit);
	start.PreOrder(Visit);
	start.InOrder_R(Visit);
	start.InOrder(Visit);
	start.PostOrder_R(Visit);
	start.PostOrder(Visit);
	start.LevelOrder(Visit);
	printf("根据先序中序生成树\n");
	BinaryTree<char> temp;
	char Pre[] = { 'H','A','C','E','D','B','G' };
	char In[] = { 'E','C','D','A','B','H','G' };
	temp.Gen_PreAIn(Pre, In, 7);
	temp.PreOrder_R(Visit);
	temp.InOrder_R(Visit);
	printf("打印D的祖先结点\n");
	temp.Print_Parent(Visit,'D');
	BinaryTree<int> temp2=*Init_I();
	printf("WPL值:%d", temp2.WPL());
    return 0;
}

BinaryTree<char>* Init_C() {
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

BinaryTree<int>* Init_I() {
	BinaryTree<int> *a = new BinaryTree<int>;
	BinaryTree<int> *b = new BinaryTree<int>;
	BinaryTree<int> *c = new BinaryTree<int>;
	BinaryTree<int> *x = new BinaryTree<int>;
	BinaryTree<int> *y = new BinaryTree<int>;
	a->MakeTree(1, *x, *y);     //             20
	b->MakeTree(2, *x, *y);    //       11
	c->MakeTree(9, *a, *b);    //    10    9
	a->MakeTree(10, *x, *y);   //        1   2
	b->MakeTree(11, *a, *c);
	a->MakeTree(20, *b, *x);
	return a;
}