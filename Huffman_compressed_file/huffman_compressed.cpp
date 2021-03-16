#include<stdio.h>
#include<algorithm>

int bytes_count = 0;  // ����������Ҷ�ӽ����
long long file_length = 0;  // �����ļ����ֽ�������ʼֵΪ0
const int FILENAMEMAX = 128;  // �ļ���������·���������Ȳ��ܳ���127
const int SUFFIXNAMEMAX = 10;  // �ļ���׺�����Ȳ��ܳ����Ÿ��ַ�
char source_filename[FILENAMEMAX] = "test.doc";  // Դ�ļ�����
char compress_filename[FILENAMEMAX] = "test.buf";  // ѹ���ļ�����
char decompress_filename[FILENAMEMAX] = "detest";  // ��ѹ�ļ�����
char prefix_filename[128];  // �ļ�ǰ׺��
char suffix_filename[128];  // �ļ���׺��
const int BUFF_MAXSIZE = 1024 * 1024;  // �ļ����������뻺�����Ĵ�С
char buff[BUFF_MAXSIZE] = "";  /* �ļ����������뻺����
������ѹ���ļ�ʱ�����ֽڵı����ȴ���buff�У�Ȼ��8����ѹ����һ���ֽڴ浽buff_string�л�������*/
char buff_string[BUFF_MAXSIZE / 8] = "";  // ѹ���ļ�д�뻺����
const int BLOCK_MAXSIZE = 1024 * 1024;  // ��ѹд��ѹ���ļ��Ļ������Ĵ�С
char block[BLOCK_MAXSIZE] = "";  // д���ѹ�ļ��Ļ�����
int block_current = 0;  // д���ѹ�ļ��������ĵ�ǰ��дλ��

struct HaffNode {
	// ���������Ľ��ṹ
	unsigned char byte;  // �������
	long long weigth;  // Ȩֵ
	int parent;  // ˫�׽���±�
	int leftchild;  // �����±�
	int rightchild;  // �Һ����±�
	char code[256];  // ����������
	int code_len;  // ���볤��
};
HaffNode HaffTree[511];  // ��������

void initHaffTree()  // ��ʼ����������
{
	for (int i = 0; i < 511; i++)
	{
		HaffTree[i].parent = -1;
		HaffTree[i].leftchild = HaffTree[i].rightchild = -1;
		HaffTree[i].weigth = 0;
		HaffTree[i].byte = i;
		HaffTree->code_len++;
	}
}

void Statistics(char fileName[])
{
	// ��ȡԴ�ļ���ͳ��Դ�ļ��и��ֽڳ��ֵĴ���
	char c;
	FILE* ifp = NULL;
	ifp = fopen(fileName, "rb");  // ��ֻ����ʽ��fileName[]ָ���������ļ�
	fread(&c, 1, 1, ifp);  // ���ļ��ж�ȡһ���ֽڵ�c
	while (!feof(ifp))  // ����ļ�δ������������ȡ
	{
		HaffTree[c].weigth++;  // c�ַ���ӦԪ��weight�ϼ�һ
		file_length++;  // �ַ�����ԭ�ļ����ȼ�һ��ȫ�ֱ�����
		fread(&c, 1, 1, ifp);  // ���ļ��ж�ȡһ���ֽڵ�c
	}
	fclose(ifp);  // �ر��ļ���
	for (int i = 0; i < 511; i++)
	{
		// ͳ��Ȩֵ������Ľ�������ȫ�ֱ�����Ҷ�ӽ��
		if (HaffTree[i].weigth > 0)
		{
			bytes_count++;
		}
	}
}

bool compare(HaffNode a, HaffNode b)
{
	// ��������Ԫ�صıȽϷ�ʽ
	return a.weigth > b.weigth;  // ����
}

void printhaffNode()
{
	printf("�Ǳ�\tbyte\tweigth\tparent\tlchild\trchild\tcode[]\t\n");
	for (int i = 0; i < bytes_count * 2 - 1; i++)
	{
		printf("%d\t%c\t", i, HaffTree[i].byte);
		printf("%lld\t", HaffTree[i].weigth);
		printf("%ld\t", HaffTree[i].parent);
		printf("%ld\t%ld\t", HaffTree[i].leftchild, HaffTree[i].rightchild);
		printf("%s\t", HaffTree[i].code);

		printf("\n");
	}
}

void creatHaffTree()
{
	printhaffNode();
	int i, k;
	int lnode, rnode;
	double min1, min2;
	for (i = bytes_count; i <= 2 * bytes_count - 2; i++)  
	{
		// �������������types_count-1����֧���
		min1 = min2 = 32767;
		lnode = rnode = -1;  // lnode��rnodeΪ��СȨ�ص���������λ��
		for (k = 0; k <= i - 1; k++)  //  ��0~i-1����Ȩֵ��С���������
		{
			if (HaffTree[k].parent == -1)  // ֻ����δ����������Ľ���в���
			{
				if (HaffTree[k].weigth < min1)
				{
					min2 = min1;rnode = lnode;
					min1 = HaffTree[k].weigth;
					lnode = k;
				}
				else if (HaffTree[k].weigth < min2)
				{
					min2 = HaffTree[k].weigth;
					rnode = k;
				}
			}
		}
		HaffTree[lnode].parent = i; HaffTree[rnode].parent = i;
		HaffTree[i].leftchild = lnode; HaffTree[i].rightchild = rnode;
		HaffTree[i].weigth = HaffTree[lnode].weigth + HaffTree[rnode].weigth;
	}
}

void creatHaffTree()
{
	int root = bytes_count * 2 - 2;
	
	for (int i = root - 1; i >= 0; i--)
	{
		if (HaffTree[i + 1].rightchild = i)
		{
			HaffTree[i].code[i--] = "1";
		}
	}
}


int main(void)
{


	char fileName = NULL;
	printf("�������ļ����ƣ�");
	scanf_s("%s", &fileName, FILENAMEMAX);
	initHaffTree();

	Statistics(&fileName);
	std::sort(HaffTree, HaffTree + 255, compare);  
	// ����˵�������������俪ʼ��ַ�����������������ַ���ȽϷ�ʽ
	creatHaffTree();
	printhaffNode();
	return 0; 

}