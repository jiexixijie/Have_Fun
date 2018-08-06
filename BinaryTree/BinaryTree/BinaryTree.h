#pragma once
#include "stdafx.h"
#include <stack>

using namespace std;

template<class T>
struct BTNode {
	BTNode<T> *lchild;
	BTNode<T> *rchild;
	T data;
	BTNode() {
		lchild = rchild = NULL;
	}
	BTNode(const T &element) {
		data = element; lchild = rchild = NULL;
	}
	BTNode(const T &element, BTNode<T> *l, BTNode<T> *r) {
		data = element; lchild = l; rchild = r;
	}
};

template<class T>
class BinaryTree {
public:
	BinaryTree() { root = NULL; }
	~BinaryTree() { Clear(); }
	bool isEmpty();
	void Clear() { Clear(root); }  //清空
	bool Root(T const &x);  //赋予root值
	void MakeTree(const T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree);   //合并左右子树
	void BreakTree(T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree); //拆树
	//遍历函数 进行封装 visit 为打印data的函数
	//递归遍历  
	void PreOrder_R(void(*Visit)(T &x)) { printf("先序遍历:"); PreOrder_R(Visit, root); printf("\n"); }
	void InOrder_R(void(*Visit)(T &x)) { printf("中序遍历:"); InOrder_R(Visit, root); printf("\n"); }
	void PostOrder_R(void(*Visit)(T &x)) { printf("后序遍历:"); PostOrder_R(Visit, root); printf("\n"); }
	//非递归遍历
	void PreOrder(void(*Visit)(T &x)) { printf("先序遍历:");  PreOrder(Visit, root); printf("\n"); }
	void InOrder(void(*Visit)(T &x)) { printf("中序遍历:"); InOrder(Visit, root); printf("\n"); }
	void PostOrder(void(*Visit)(T &x)) { printf("后序遍历:"); PostOrder(Visit, root); printf("\n"); }
protected:
	BTNode<T> *root;
private:
	void Clear(BTNode<T>* &node);
	//递归 遍历
	void PreOrder_R(void(*Visit)(T &x), BTNode<T> *p);
	void InOrder_R(void(*Visit)(T &x), BTNode<T> *p);
	void PostOrder_R(void(*Visit)(T &x), BTNode<T> *p);
	//非递归 遍历
	void PreOrder(void(*Visit)(T &x), BTNode<T> *root);
	void InOrder(void(*Visit)(T &x), BTNode<T> *root);
	void PostOrder(void(*Visit)(T &x), BTNode<T> *root);
};

template<class T>
bool BinaryTree<T>::isEmpty() {
	if (root == NULL) {
		return true;
	}
	else {
		return false;
	}
}

template<class T>
void BinaryTree<T>::Clear(BTNode<T>* &node) {  //后序遍历
	if (node) {
		Clear(node->lchild);
		Clear(node->rchild);
		delete node;
	}
}

template<class T>
bool BinaryTree<T>::Root(T const &x) {
	if (root) {
		root->data = x;
		return true;
	}
	else {
		return false;
	}
}

template<class T>
void BinaryTree<T>::MakeTree(const T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree) {
	if (root) {
		printf("error:根结点已存在\n");
	}
	else if (&ltree == &rtree) {
		printf("error:左右子树相同\n");
	}
	else {
		root = new BTNode<T>(x, ltree.root, rtree.root);
		ltree.root = rtree.root = NULL;   //只有一个根节点
	}
	return;
}

template<class T>
void BinaryTree<T>::BreakTree(T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree) {
	if (!root) {
		printf("error:空树无法分解\n");
	}
	else if (ltree == rtree) {
		printf("error:左右子树为同一指针\n");
	}
	else if (ltree.root || ltree.root) {   //如果已经指向一颗树则fail
		printf("左右子树指针不为空\n");
	}
	else {
		x = root->data;
		ltree = root->lchild;
		rtree = root->rchild;
		delete root;
	}
	return;
}

template<class T>
void BinaryTree<T>::PreOrder_R(void(*Visit)(T &x), BTNode<T> *p) {
	if (p) {
		Visit(p->data);
		PreOrder_R(Visit, p->lchild);
		PreOrder_R(Visit, p->rchild);
	}
}

template<class T>
void BinaryTree<T>::InOrder_R(void(*Visit)(T &x), BTNode<T> *p) {
	if (p) {
		InOrder_R(Visit, p->lchild);
		Visit(p->data);
		InOrder_R(Visit, p->rchild);
	}
}

template<class T>
void BinaryTree<T>::PostOrder_R(void(*Visit)(T &x), BTNode<T> *p) {
	if (p) {
		PostOrder_R(Visit, p->lchild);
		PostOrder_R(Visit, p->rchild);
		Visit(p->data);
	}
}

template<class T>
void BinaryTree<T>::PreOrder(void(*Visit)(T &x), BTNode<T> *root) {
	stack<BTNode<T>*> s;
	BTNode<T> *p=root;
	while (p || !s.empty()) {
		if (p) {
			Visit(p->data);
			s.push(p);
			p = p->lchild;
		}
		else {
			p = s.top();  s.pop();
			p = p->rchild;
		}
	}
}

template<class T>
void BinaryTree<T>::InOrder(void(*Visit)(T &x), BTNode<T> *root) {
	stack<BTNode<T>*> s;
	BTNode<T> *p = root;
	while (p || !s.empty()) {
		if (p) {
			s.push(p);
			p = p->lchild;
		}
		else {
			p = s.top(); s.pop();
			Visit(p->data);
			p = p->rchild;
		}
	}
}

template<class T>
void BinaryTree<T>::PostOrder(void(*Visit)(T &x), BTNode<T> *root) {
	stack<BTNode<T>*> s1,s2;   //s2用来比较结点是否访问
	BTNode<T> *p = root;
	while (p || !s2.empty()) {
		if (p) {
			s1.push(p);
			s2.push(p);
			p = p->lchild;
		}
		else {
			while (s1.top()!=s2.top()){           //判断该结点是否已经访问过了
				p = s1.top(); s1.pop();				//如果访问过，则表示是右结点,接下来要返回到上一层
				Visit(p->data);						//为访问过。表示是左结点，接下来要访问右结点
			}
			p = s2.top(); s2.pop();
			p = p->rchild;
		}
	}
	while (!s1.empty()){
		p = s1.top(); s1.pop();
		Visit(p->data);
	}
}