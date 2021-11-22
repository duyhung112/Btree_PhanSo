/*
	THỰC HÀNH CẤU TRÚC DỮ LIỆU VÀ GIẢI THUẬT
	GIÁO VIÊN: TRẦN VĂN THỌ
	TÊN: TÀI ĐẠI DUY HÙNG
	MSSV: 2001202091
	BUỔI THỨ 3 , TIẾT 1->5
*/

#include "BTree_PS.h"


void initBTree(BTree &bt)
{
	bt.Root = NULL;
}
void insert(NodePtr &root, ItemType key)
{
	NodePtr newNode;
	ItemType upKey;
	KeyStatus result;
	result = insertNode(root, key, &upKey, &newNode);
	if (result == Duplicate)
		printf("Key already available\n");
	if (result == InsertIt)
	{
		NodePtr upRoot = root;
		root = new BNode;
		root->numTree = 1;
		root->Keys[0] = upKey;
		root->Branch[0] = upRoot;
		root->Branch[1] = newNode;
	}/*End of if */
}/*End of insert()*/

KeyStatus insertNode(NodePtr pCurrent, ItemType key, ItemType *upKey, NodePtr* newNode)
{
	NodePtr newPtr, lastPtr;
	int pos, i, numTree, splitPos;
	ItemType newKey, lastKey;
	KeyStatus result;
	if (pCurrent == NULL)
	{
		*newNode = NULL;
		*upKey = key;
		return InsertIt;
	}
	numTree = pCurrent->numTree;
	pos = searchPosition(key, pCurrent->Keys, numTree);
	if (pos < numTree && SoSanh2PS(key, pCurrent->Keys[pos]) == 0)
		return Duplicate;
	result = insertNode(pCurrent->Branch[pos], key, &newKey, &newPtr);
	if (result != InsertIt)
		return result;
	/*If Keys in node is less than M-1 where M is order of B tree*/
	if (numTree < M - 1)
	{
		pos = searchPosition(newKey, pCurrent->Keys, numTree);
		/*Shifting the key and pointer right for inserting the new key*/
		for (i = numTree; i > pos; i--)
		{
			pCurrent->Keys[i] = pCurrent->Keys[i - 1];
			pCurrent->Branch[i + 1] = pCurrent->Branch[i];
		}
		/*Key is inserted at exact location*/
		pCurrent->Keys[pos] = newKey;
		pCurrent->Branch[pos + 1] = newPtr;
		++pCurrent->numTree; /*incrementing the number of Keys in node*/
		return Success;
	}/*End of if */
	/*If Keys in nodes are maximum and position of node to be inserted is last*/
	if (pos == M - 1)
	{
		lastKey = newKey;
		lastPtr = newPtr;
	}
	else /*If Keys in node are maximum and position of node to be inserted is not last*/
	{
		lastKey = pCurrent->Keys[M - 2];
		lastPtr = pCurrent->Branch[M - 1];
		for (i = M - 2; i > pos; i--)
		{
			pCurrent->Keys[i] = pCurrent->Keys[i - 1];
			pCurrent->Branch[i + 1] = pCurrent->Branch[i];
		}
		pCurrent->Keys[pos] = newKey;
		pCurrent->Branch[pos + 1] = newPtr;
	}
	splitPos = (M - 1) / 2;
	(*upKey) = pCurrent->Keys[splitPos];

	(*newNode) = new BNode;/*Right node after split*/
	pCurrent->numTree = splitPos; /*No. of Keys for left splitted node*/
	(*newNode)->numTree = M - 1 - splitPos; /*No. of Keys for right splitted node*/
	for (i = 0; i < (*newNode)->numTree; i++)
	{
		(*newNode)->Branch[i] = pCurrent->Branch[i + splitPos + 1];
		if (i < (*newNode)->numTree - 1)
			(*newNode)->Keys[i] = pCurrent->Keys[i + splitPos + 1];
		else
			(*newNode)->Keys[i] = lastKey;
	}
	(*newNode)->Branch[(*newNode)->numTree] = lastPtr;
	return InsertIt;
}/*End of insertNode()*/

void displayBTree(NodePtr pRoot, int blanks) {
	if (pRoot) {
		int i;
		printf("%d\t", pRoot->numTree);
		for (i = 1; i <= blanks; i++)
			printf(" ");
		for (i = 0; i < pRoot->numTree; i++)
			printf("%d/%d   ", pRoot->Keys[i].tu, pRoot->Keys[i].mau);
		printf("\n");
		for (i = 0; i <= pRoot->numTree; i++)
			displayBTree(pRoot->Branch[i], blanks + 10);
	}/*End of if*/
}/*End of displayBTree()*/

int searchPosition(ItemType key, ItemType *keyArray, int numTree)
{
	int pos = 0;
	while (pos < numTree && SoSanh2PS(key, keyArray[pos]) == 1)
		pos++;
	return pos;
}/*End of searchPosition()*/

NodePtr searchNode(NodePtr root, ItemType key)
{
	int pos, numTree;
	NodePtr pCurrent = root;
	while (pCurrent)
	{
		numTree = pCurrent->numTree;
		pos = searchPosition(key, pCurrent->Keys, numTree);
		if (pos < numTree && SoSanh2PS(key, pCurrent->Keys[pos]) == 0)
		{
			return pCurrent;
		}
		pCurrent = pCurrent->Branch[pos];
	}
	return NULL;
}/*End of search()*/

void search(NodePtr root, ItemType key)
{
	int pos, i, numTree;
	NodePtr pCurrent = root;
	printf("Search path:\n");
	while (pCurrent)
	{
		numTree = pCurrent->numTree;
		for (i = 0; i < pCurrent->numTree; i++)
			printf(" %d/%d", pCurrent->Keys[i].tu, pCurrent->Keys[i].mau);
		printf("\n");
		pos = searchPosition(key, pCurrent->Keys, numTree);
		if (pos < numTree && SoSanh2PS(key, pCurrent->Keys[pos]) == 0)
		{
			printf("Key %d/%d found in position %d of last dispalyed node\n", key.tu, key.mau, pos);
			return;
		}
		pCurrent = pCurrent->Branch[pos];
	}
	printf("Key %d is not available\n", key);
}/*End of search()*/

void deleteNode(NodePtr &root, ItemType key)
{
	NodePtr upRoot;
	KeyStatus result;
	result = remove(root, root, key);
	switch (result)
	{
	case SearchFailure:
		printf("Key %d/%d is not available\n", key.tu, key.mau);
		break;
	case LessKeys:
		upRoot = root;
		root = root->Branch[0];
		free(upRoot);
		printf("\nBtree after removing the %d/%d value:\n", key.tu, key.mau);
		displayBTree(root, 0);
		break;
	case Success:
		printf("\nBtree after removing the %d/%d value:\n", key.tu, key.mau);
		displayBTree(root, 0);
		break;
	}/*End of switch*/
}/*End of delnode()*/

KeyStatus remove(NodePtr &root, NodePtr pCurrent, ItemType key)
{
	int pos, i, pivot, numTree, min;
	ItemType *keyArray;
	KeyStatus result;
	NodePtr *Branch, leftPtr, rightPtr;

	if (pCurrent == NULL)
		return SearchFailure;
	/*Assigns values of node*/
	numTree = pCurrent->numTree;
	keyArray = pCurrent->Keys;
	Branch = pCurrent->Branch;
	min = (M - 1) / 2;/*Minimum number of Keys*/

	pos = searchPosition(key, keyArray, numTree);
	if (Branch[0] == NULL)
	{
		if (pos == numTree || SoSanh2PS(key, keyArray[pos]) == 1)
			return SearchFailure;
		/*Shift Keys and pointers left*/
		for (i = pos + 1; i < numTree; i++)
		{
			keyArray[i - 1] = keyArray[i];
			Branch[i] = Branch[i + 1];
		}
		return --pCurrent->numTree >= (pCurrent == root ? 1 : min) ? Success : LessKeys;
	}/*End of if */

	if (pos < numTree && SoSanh2PS(key, keyArray[pos]) == 0)
	{
		NodePtr qp = Branch[pos], qp1;
		int nkey;// -----------------------------------------------
		while (1)
		{
			nkey = qp->numTree;
			qp1 = qp->Branch[nkey];
			if (qp1 == NULL)
				break;
			qp = qp1;
		}/*End of while*/
		keyArray[pos] = qp->Keys[nkey - 1];
		qp->Keys[nkey - 1] = key;
	}/*End of if */
	result = remove(root, Branch[pos], key);
	if (result != LessKeys)
		return result;

	if (pos > 0 && Branch[pos - 1]->numTree > min)
	{
		pivot = pos - 1; /*pivot for left and right node*/
		leftPtr = Branch[pivot];
		rightPtr = Branch[pos];
		/*Assigns values for right node*/
		rightPtr->Branch[rightPtr->numTree + 1] = rightPtr->Branch[rightPtr->numTree];
		for (i = rightPtr->numTree; i > 0; i--)
		{
			rightPtr->Keys[i] = rightPtr->Keys[i - 1];
			rightPtr->Branch[i] = rightPtr->Branch[i - 1];
		}
		rightPtr->numTree++;
		rightPtr->Keys[0] = keyArray[pivot];
		rightPtr->Branch[0] = leftPtr->Branch[leftPtr->numTree];
		keyArray[pivot] = leftPtr->Keys[--leftPtr->numTree];
		return Success;
	}/*End of if */
	if (pos < numTree && Branch[pos + 1]->numTree > min)
	{
		pivot = pos; /*pivot for left and right node*/
		leftPtr = Branch[pivot];
		rightPtr = Branch[pivot + 1];
		/*Assigns values for left node*/
		leftPtr->Keys[leftPtr->numTree] = keyArray[pivot];
		leftPtr->Branch[leftPtr->numTree + 1] = rightPtr->Branch[0];
		keyArray[pivot] = rightPtr->Keys[0];
		leftPtr->numTree++;
		rightPtr->numTree--;
		for (i = 0; i < rightPtr->numTree; i++)
		{
			rightPtr->Keys[i] = rightPtr->Keys[i + 1];
			rightPtr->Branch[i] = rightPtr->Branch[i + 1];
		}/*End of for*/
		rightPtr->Branch[rightPtr->numTree] = rightPtr->Branch[rightPtr->numTree + 1];
		return Success;
	}/*End of if */

	if (pos == numTree)
		pivot = pos - 1;
	else
		pivot = pos;

	leftPtr = Branch[pivot];
	rightPtr = Branch[pivot + 1];
	/*merge right node with left node*/
	leftPtr->Keys[leftPtr->numTree] = keyArray[pivot];
	leftPtr->Branch[leftPtr->numTree + 1] = rightPtr->Branch[0];
	for (i = 0; i < rightPtr->numTree; i++)
	{
		leftPtr->Keys[leftPtr->numTree + 1 + i] = rightPtr->Keys[i];
		leftPtr->Branch[leftPtr->numTree + 2 + i] = rightPtr->Branch[i + 1];
	}
	leftPtr->numTree = leftPtr->numTree + rightPtr->numTree + 1;
	free(rightPtr); /*Remove right node*/
	for (i = pos + 1; i < numTree; i++)
	{
		keyArray[i - 1] = keyArray[i];
		Branch[i] = Branch[i + 1];
	}
	return --pCurrent->numTree >= (pCurrent == root ? 1 : min) ? Success : LessKeys;
}/*End of remove()*/

int main() {
	process3();
	return 0;
}

void process3()
{
	int luachon;
	BTree bt;
	initBTree(bt);
	while (true)
	{
		system("cls");
		cout << "\n\t\t\t\t ------------------------------- MENU3 -------------------------------";
		cout << "\n\t\t1. Tao cay tu mang.";
		cout << "\n\t\t2. Xuat cay hien tai.";
		cout << "\n\t\t3. Them phan so.";
		cout << "\n\t\t4. Tim phan so.";
		cout << "\n\t\t5. Xoa phan so.";
		cout << "\n\t\t6. Xoa cac phan so lon hon 2.";
		cout << "\n\t\t7. Xoa cac phan so co mau la So Nguyen To.";
		cout << "\n\t\t8. Dem so phan tu co tu lon hon mau.";
		cout << "\n\t\t9. Xoa toan bo cay.";
		cout << "\n\t\t0. Thoat.";
		cout << "\n\t\t\t\t ---------------------------------------------------------------------";
		cout << "\n\tNhap lua chon: ";
		cin >> luachon;


		if (luachon == 0)
		{
			break;
		}
		else if (luachon == 1)
		{
			ItemType a[50];
			int n = 50;
			srand(time(NULL));
			for (int i = 0; i < n; i++)
			{
				a[i].tu = rand() % 1001;
				do
				{
					a[i].mau = rand() % 1001;
				} while (a[i].mau == 0);
			}
			ItemType key;

			cout << "Creation of B tree for node " << M << endl;
			for (int i = 0; i < n; i++)
			{
				key = a[i];
				insert(bt.Root, key);
			}
			cout << "Btree is:\n";
			displayBTree(bt.Root, 0);
		}
		else if (luachon == 2)
		{
			cout << "Btree is:\n";
			displayBTree(bt.Root, 0);
		}
		else if (luachon == 3)
		{
			ItemType key;
			NhapPS(key);
			insert(bt.Root, key);
			cout << "Btree is:\n";
			displayBTree(bt.Root, 0);
		}
		else if (luachon == 4)
		{
			ItemType key;
			NhapPS(key);
			search(bt.Root, key);
		}
		else if (luachon == 5)
		{
			ItemType key;
			NhapPS(key);
			deleteNode(bt.Root, key);
		}
		else if (luachon == 6)
		{
			PS tam;
			tam.tu = 2;
			tam.mau = 1;
			delete_AllPS_Bigger2(bt.Root, tam);
		}
		else if (luachon == 7)
		{

		}
		else if (luachon == 8)
		{
			int dem = 0;
			count_Node_TuBiggerMau(bt.Root, dem);
			if (dem == 0)
				cout << "\nTrong cay khong ton tai node co Tu lon hon Mau!";
			else cout << "\nTrong cay co " << dem << " node co Tu lon hon Mau.";
		}
		else if (luachon == 9)
		{
			
		}
		else
		{
			cout << "\nVui long nhap cho chinh xac!";
		}
		_getch();
	}
}

int SoSanh2PS(PS x, PS y)
{
	int tux = x.tu * y.mau;
	int tuy = y.tu * x.mau;
	if (tux == tuy)
		return 0;
	if (tux > tuy)
		return 1;
	return -1;
}

void NhapPS(PS &x)
{
	cout << "\n\t\tNHAP PHAN SO\n";
	cout << "\nNhap tu so: ";
	cin >> x.tu;
	do
	{
		cout << "\nNhap mau so: ";
		cin >> x.mau;
		if (x.mau == 0)
		{
			cout << "\nVui long nhap mau khac 0!";
			_getch();
		}
	} while (x.mau == 0);
}

void delete_AllPS_Bigger2(BNode* root, ItemType tam)
{
	if (root)
	{
		for (int i = 0; i <= root->numTree; i++)
		{
			delete_AllPS_Bigger2(root->Branch[i], tam);
		}
		for (int i = 0; i < root->numTree; i++)
		{
			if (SoSanh2PS(root->Keys[i], tam) > 0)
			{
				deleteNode(root, root->Keys[i]);
			}
		}
	}
}

void count_Node_TuBiggerMau(BNode* root, int &dem)
{
	if (root)
	{
		for (int i = 0; i < root->numTree; i++)
		{
			if (root->Keys[i].tu > root->Keys[i].mau)
				dem++;
		}
		for (int i = 0; i <= root->numTree; i++)
			count_Node_TuBiggerMau(root->Branch[i], dem);
	}
}
