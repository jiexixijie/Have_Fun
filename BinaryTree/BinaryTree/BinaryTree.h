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
	void Clear() { Clear(root); }  //���
	bool Root(T const &x);  //����rootֵ
	void MakeTree(const T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree);   //�ϲ���������
	void BreakTree(T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree); //����
	//�������� ���з�װ visit Ϊ��ӡdata�ĺ���
	//�ݹ����  
	void PreOrder_R(void(*Visit)(T &x)) { printf("�������:"); PreOrder_R(Visit, root); printf("\n"); }
	void InOrder_R(void(*Visit)(T &x)) { printf("�������:"); InOrder_R(Visit, root); printf("\n"); }
	void PostOrder_R(void(*Visit)(T &x)) { printf("�������:"); PostOrder_R(Visit, root); printf("\n"); }
	//�ǵݹ����
	void PreOrder(void(*Visit)(T &x)) { printf("�������:");  PreOrder(Visit, root); printf("\n"); }
	void InOrder(void(*Visit)(T &x)) { printf("�������:"); InOrder(Visit, root); printf("\n"); }
	void PostOrder(void(*Visit)(T &x)) { printf("�������:"); PostOrder(Visit, root); printf("\n"); }
protected:
	BTNode<T> *root;
private:
	void Clear(BTNode<T>* &node);
	//�ݹ� ����
	void PreOrder_R(void(*Visit)(T &x), BTNode<T> *p);
	void InOrder_R(void(*Visit)(T &x), BTNode<T> *p);
	void PostOrder_R(void(*Visit)(T &x), BTNode<T> *p);
	//�ǵݹ� ����
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
void BinaryTree<T>::Clear(BTNode<T>* &node) {  //�������
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
		printf("error:������Ѵ���\n");
	}
	else if (&ltree == &rtree) {
		printf("error:����������ͬ\n");
	}
	else {
		root = new BTNode<T>(x, ltree.root, rtree.root);
		ltree.root = rtree.root = NULL;   //ֻ��һ�����ڵ�
	}
	return;
}

template<class T>
void BinaryTree<T>::BreakTree(T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree) {
	if (!root) {
		printf("error:�����޷��ֽ�\n");
	}
	else if (ltree == rtree) {
		printf("error:��������Ϊͬһָ��\n");
	}
	else if (ltree.root || ltree.root) {   //����Ѿ�ָ��һ������fail
		printf("��������ָ�벻Ϊ��\n");
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
	stack<BTNode<T>*> s1,s2;   //s2�����ȽϽ���Ƿ����
	BTNode<T> *p = root;
	while (p || !s2.empty()) {
		if (p) {
			s1.push(p);
			s2.push(p);
			p = p->lchild;
		}
		else {
			while (s1.top()!=s2.top()){           //�жϸý���Ƿ��Ѿ����ʹ���
				p = s1.top(); s1.pop();				//������ʹ������ʾ���ҽ��,������Ҫ���ص���һ��
				Visit(p->data);						//Ϊ���ʹ�����ʾ�����㣬������Ҫ�����ҽ��
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