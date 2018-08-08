#pragma once
#include "stdafx.h"
#include <stack>
#include <queue>

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
struct Node {                 //���ڷǵݹ�ĺ������
	BTNode<T> *node;
	bool visited;
	Node(BTNode<T> *p, bool a) {
		node = p;  visited = a;
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
	void PreOrder_R(void(*Visit)(T &x)) { printf("�������(�ݹ�):"); PreOrder_R(Visit, root); printf("\n"); }
	void InOrder_R(void(*Visit)(T &x)) { printf("�������(�ݹ�):"); InOrder_R(Visit, root); printf("\n"); }
	void PostOrder_R(void(*Visit)(T &x)) { printf("�������(�ݹ�):"); PostOrder_R(Visit, root); printf("\n"); }
	//�ǵݹ����
	void PreOrder(void(*Visit)(T &x)) { printf("�������(�ǵݹ�):");  PreOrder(Visit, root); printf("\n"); }
	void InOrder(void(*Visit)(T &x)) { printf("�������(�ǵݹ�):"); InOrder(Visit, root); printf("\n"); }
	void PostOrder(void(*Visit)(T &x)) { printf("�������(�ǵݹ�):"); PostOrder(Visit, root); printf("\n"); }
	//ʹ�ö���
	void LevelOrder(void(*Visit)(T &x)){ printf("��α���:"); LevelOrder(Visit, root); printf("\n"); }
	void Gen_PreAIn(T Pre[],T In[],int len);  //��������������������������
	void Print_Parent(void(*Visit)(T &x), T Data) { Visit(Data); printf("�����Ƚ��:"); Print_Parent(Visit, root, Data); printf("\n"); } //��ӡ�������Ƚ��
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

	void LevelOrder(void(*Visit)(T &x), BTNode<T> *root);
	BTNode<T>* Gen_PreAIn(T Pre[], T In[], int l1, int h1, int l2, int h2);//�ֱ�����ͺ����start �� end
	bool Print_Parent(void(*Visit)(T &x), BTNode<T> *p, T Data);      


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
	Node<T> p(root,false);
	stack<Node<T>> s;
	while (p.node || !s.empty()) {
		if (p.node) {
			s.push(p);
			p.node = p.node->lchild;
		}
		else {
			p = s.top(); s.pop();
			while (!s.empty() && p.visited) {    //���˵�û�б����ʵĵط�
				Visit(p.node->data);
				p = s.top(); s.pop();
			}
			if (p.node == root && p.visited) {   //�����ڵ㣬�˳�
				Visit(root->data);
				break;
			}
			else {
				p.visited = true;
				s.push(p);
				p.node = p.node->rchild;
			}
		}
	}
}

//�ǵݹ�������   2
//stack<BTNode<T>*> s1,s2;   //s2�����ȽϽ���Ƿ����
//BTNode<T> *p = root;
//while (p || !s2.empty()) {
//	if (p) {
//		s1.push(p);
//		s2.push(p);
//		p = p->lchild;
//	}
//	else {
//		while (s1.top()!=s2.top()){           //�жϸý���Ƿ��Ѿ����ʹ���
//			p = s1.top(); s1.pop();				//������ʹ������ʾ���ҽ��,������Ҫ���ص���һ��
//			Visit(p->data);						//Ϊ���ʹ�����ʾ�����㣬������Ҫ�����ҽ��
//		}
//		p = s2.top(); s2.pop();
//		p = p->rchild;
//	}
//}
//while (!s1.empty()){
//	p = s1.top(); s1.pop();
//	Visit(p->data);
//}

template<class T>
void BinaryTree<T>::LevelOrder(void(*Visit)(T &x), BTNode<T> *root) {
	queue<BTNode<T>*> q;
	BTNode<T> *p = root;
	q.push(p);
	while (!q.empty()){
		p = q.front(); q.pop();
		if (p) {
			Visit(p->data);
			q.push(p->lchild);
			q.push(p->rchild);
		}
	}
}

template<class T>
void BinaryTree<T>::Gen_PreAIn(T Pre[], T In[], int len) {
	if (root || len <= 0) {
		printf("error:�����Ѵ���\n");
			return;
	}
	root = Gen_PreAIn(Pre, In, 0, len - 1, 0, len - 1); //�����е��±�
}

template<class T>
BTNode<T>* BinaryTree<T>::Gen_PreAIn(T Pre[], T In[], int l1, int h1, int l2, int h2) {
	BTNode<T> *parent = new BTNode<T>(Pre[l1]);
	int mid;
	for (mid = l2; In[mid] != parent->data; mid++); //�ҵ�˫�����������±�
	int llen = mid - l2; //����������
	int rlen = h2 - mid; //����������
	if (llen) {
		parent->lchild = Gen_PreAIn(Pre, In, l1 + 1, l1 + llen, l2, l2 + llen - 1);
	}
	else {
		parent->lchild = NULL;
	}
	if (rlen) {
		parent->rchild = Gen_PreAIn(Pre, In, h1 - rlen + 1, h1, h2 - rlen + 1, h2);
	}
	else {
		parent->rchild = NULL;
	}
	return parent;
}

template<class T>
bool BinaryTree<T>::Print_Parent(void(*Visit)(T &x), BTNode<T> *p,T Data) {
	if (p) {
		if (Print_Parent(Visit, p->lchild, Data) || Print_Parent(Visit, p->rchild, Data)) {
			Visit(p->data);
			return true;
		}
		if (p->data == Data) {
			return true;
		}
	}
	return false;
}