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
struct Node {                 //用于非递归的后序遍历
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
	void Clear() { Clear(root); }  //清空
	bool Root(T const &x);  //赋予root值
	void MakeTree(const T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree);   //合并左右子树
	void BreakTree(T &x, BinaryTree<T> &ltree, BinaryTree<T> &rtree); //拆树
	//遍历函数 进行封装 visit 为打印data的函数
	//递归遍历  
	void PreOrder_R(void(*Visit)(T &x)) { printf("先序遍历(递归):"); PreOrder_R(Visit, root); printf("\n"); }
	void InOrder_R(void(*Visit)(T &x)) { printf("中序遍历(递归):"); InOrder_R(Visit, root); printf("\n"); }
	void PostOrder_R(void(*Visit)(T &x)) { printf("后序遍历(递归):"); PostOrder_R(Visit, root); printf("\n"); }
	//非递归遍历
	void PreOrder(void(*Visit)(T &x)) { printf("先序遍历(非递归):");  PreOrder(Visit, root); printf("\n"); }
	void InOrder(void(*Visit)(T &x)) { printf("中序遍历(非递归):"); InOrder(Visit, root); printf("\n"); }
	void PostOrder(void(*Visit)(T &x)) { printf("后序遍历(非递归):"); PostOrder(Visit, root); printf("\n"); }
	//使用队列
	void LevelOrder(void(*Visit)(T &x)){ printf("层次遍历:"); LevelOrder(Visit, root); printf("\n"); }
	void Gen_PreAIn(T Pre[],T In[],int len);  //根据先序遍历和中序遍历生成树
	void Print_Parent(void(*Visit)(T &x), T Data) { Visit(Data); printf("的祖先结点:"); Print_Parent(Visit, root, Data); printf("\n"); } //打印结点的祖先结点
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

	void LevelOrder(void(*Visit)(T &x), BTNode<T> *root);
	BTNode<T>* Gen_PreAIn(T Pre[], T In[], int l1, int h1, int l2, int h2);//分别先序和后序的start 和 end
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
	Node<T> p(root,false);
	stack<Node<T>> s;
	while (p.node || !s.empty()) {
		if (p.node) {
			s.push(p);
			p.node = p.node->lchild;
		}
		else {
			p = s.top(); s.pop();
			while (!s.empty() && p.visited) {    //回退到没有被访问的地方
				Visit(p.node->data);
				p = s.top(); s.pop();
			}
			if (p.node == root && p.visited) {   //到根节点，退出
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

//非递归后序遍历   2
//stack<BTNode<T>*> s1,s2;   //s2用来比较结点是否访问
//BTNode<T> *p = root;
//while (p || !s2.empty()) {
//	if (p) {
//		s1.push(p);
//		s2.push(p);
//		p = p->lchild;
//	}
//	else {
//		while (s1.top()!=s2.top()){           //判断该结点是否已经访问过了
//			p = s1.top(); s1.pop();				//如果访问过，则表示是右结点,接下来要返回到上一层
//			Visit(p->data);						//为访问过。表示是左结点，接下来要访问右结点
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
		printf("error:该树已存在\n");
			return;
	}
	root = Gen_PreAIn(Pre, In, 0, len - 1, 0, len - 1); //中序中的下标
}

template<class T>
BTNode<T>* BinaryTree<T>::Gen_PreAIn(T Pre[], T In[], int l1, int h1, int l2, int h2) {
	BTNode<T> *parent = new BTNode<T>(Pre[l1]);
	int mid;
	for (mid = l2; In[mid] != parent->data; mid++); //找到双亲在中序中下标
	int llen = mid - l2; //左子树长度
	int rlen = h2 - mid; //右子树长度
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