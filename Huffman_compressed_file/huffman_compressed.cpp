#include<stdio.h>
#include<algorithm>

int bytes_count = 0;  // 哈夫曼树的叶子结点数
long long file_length = 0;  // 保存文件的字节数，初始值为0
const int FILENAMEMAX = 128;  // 文件名（包括路径），长度不能超过127
const int SUFFIXNAMEMAX = 10;  // 文件后缀名长度不能超过九个字符
char source_filename[FILENAMEMAX] = "test.doc";  // 源文件名字
char compress_filename[FILENAMEMAX] = "test.buf";  // 压缩文件名字
char decompress_filename[FILENAMEMAX] = "detest";  // 解压文件名字
char prefix_filename[128];  // 文件前缀名
char suffix_filename[128];  // 文件后缀名
const int BUFF_MAXSIZE = 1024 * 1024;  // 文件哈夫曼编码缓冲区的大小
char buff[BUFF_MAXSIZE] = "";  /* 文件哈夫曼编码缓冲区
（生成压缩文件时，各字节的编码先存在buff中，然后8个码压缩成一个字节存到buff_string中缓冲区）*/
char buff_string[BUFF_MAXSIZE / 8] = "";  // 压缩文件写入缓冲区
const int BLOCK_MAXSIZE = 1024 * 1024;  // 解压写入压缩文件的缓冲区的大小
char block[BLOCK_MAXSIZE] = "";  // 写入解压文件的缓冲区
int block_current = 0;  // 写入解压文件缓冲区的当前读写位置

struct HaffNode {
	// 哈夫曼树的结点结构
	unsigned char byte;  // 结点数据
	long long weigth;  // 权值
	int parent;  // 双亲结点下标
	int leftchild;  // 左孩子下标
	int rightchild;  // 右孩子下标
	char code[256];  // 哈夫曼编码
	int code_len;  // 编码长度
};
HaffNode HaffTree[511];  // 哈夫曼树

void initHaffTree()  // 初始化哈夫曼树
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
	// 读取源文件，统计源文件中各字节出现的次数
	char c;
	FILE* ifp = NULL;
	ifp = fopen(fileName, "rb");  // 以只读方式打开fileName[]指定二进制文件
	fread(&c, 1, 1, ifp);  // 从文件中读取一个字节到c
	while (!feof(ifp))  // 如果文件未结束，继续读取
	{
		HaffTree[c].weigth++;  // c字符对应元素weight上加一
		file_length++;  // 字符出现原文件长度加一（全局变量）
		fread(&c, 1, 1, ifp);  // 从文件中读取一个字节到c
	}
	fclose(ifp);  // 关闭文件流
	for (int i = 0; i < 511; i++)
	{
		// 统计权值大于零的结点个数（全局变量）叶子结点
		if (HaffTree[i].weigth > 0)
		{
			bytes_count++;
		}
	}
}

bool compare(HaffNode a, HaffNode b)
{
	// 定义两个元素的比较方式
	return a.weigth > b.weigth;  // 降序
}

void printhaffNode()
{
	printf("角标\tbyte\tweigth\tparent\tlchild\trchild\tcode[]\t\n");
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
		// 构造哈夫曼树的types_count-1个分支结点
		min1 = min2 = 32767;
		lnode = rnode = -1;  // lnode和rnode为最小权重的两个结点的位置
		for (k = 0; k <= i - 1; k++)  //  在0~i-1中找权值最小的两个结点
		{
			if (HaffTree[k].parent == -1)  // 只在尚未构造二叉树的结点中查找
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
	printf("请输入文件名称：");
	scanf_s("%s", &fileName, FILENAMEMAX);
	initHaffTree();

	Statistics(&fileName);
	std::sort(HaffTree, HaffTree + 255, compare);  
	// 参数说明：待排序区间开始地址，待排序区间结束地址，比较方式
	creatHaffTree();
	printhaffNode();
	return 0; 

}