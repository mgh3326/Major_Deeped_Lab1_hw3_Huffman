#include<iostream>
#include<string>
#include<vector>
#include<assert.h>
#include <bitset>  
#include <fstream>
#include <iostream>
#include <map>

#include"HuffmanCodes.h"
using namespace std;

#define X_MAX 256
#define Y_MAX 256


unsigned char** mem_alloc2_d(int n_height, int n_width, unsigned char nlnit_val);

unsigned char** mem_alloc2_d(int n_height, int n_width, unsigned char nlnit_val)
{
	unsigned char** rtn = new unsigned char*[n_height];
	for (int n = 0; n < n_height; n++)
	{
		rtn[n] = new unsigned char[n_width];
		memset(rtn[n], nlnit_val, sizeof(unsigned char)*n_width);
	}
	return rtn;
}


void histogram_generate(unsigned char **in, int * hist)
{


	for (int i = 0; i < 256; i++)
		hist[i] = 0;//initialze
	for (int h = 0; h < Y_MAX; h++)
	{
		for (int w = 0; w < X_MAX; w++)
		{
			hist[in[h][w]]++;//히스토 그램을 만듭니다.
		}
	}

}
unsigned char extract(unsigned char data, int i) {
	return (data >> i) & 1;
}
int encode() {
	printf("encode start\n");

	unsigned char** in_data = mem_alloc2_d(X_MAX, Y_MAX, 0);
	// 이미지를 읽어온다

	FILE* in = fopen("lena256.raw", "rb");//file input
	if (in == NULL)
	{
		printf("File not found!!\n");
		return 0;
	}
	for (int i = 0; i < Y_MAX; i++)
	{
		fread(in_data[i], sizeof(char), X_MAX, in);
	}
	fclose(in);
	int *hist = new int[256];//histogram을 받을 배열입니다.
	histogram_generate(in_data, hist);//histogram을 만듭니다.
	unsigned char* arr = new unsigned char[256];//넉넉하게 256 을 할당해줘도 문제가 없을까?
	int* freq = new int[256];
	int size = 0;
	for (int i = 0; i < 256; i++) {
		if (hist[i] != 0)
		{
			arr[size] = i;
			freq[size] = hist[i];
			size++;
		}
	}
	vector<int> adj[256];

	HuffmanCodes(arr, freq, size, adj);
	/*test(adj);*/
	vector<int> saved_vector;//이걸 array로 하면 빨라서 좋을거 같기도하다.
	int test_count = 0;
	for (int h = 0; h < Y_MAX; h++)
	{
		for (int w = 0; w < X_MAX; w++)
		{
			test_count++;
			for (int i = 0; i < adj[in_data[h][w]].size(); i++)
			{
				saved_vector.push_back(adj[in_data[h][w]][i]);
			}
		}
	}
	string filePath = "huffman_table.txt";

	// write File
	ofstream writeFile(filePath.data());
	if (writeFile.is_open()) {
		for (int i = 0; i < 256; i++)
		{
			for (int j = 0; j < adj[i].size(); j++)
			{
				writeFile << adj[i][j];

			}
			writeFile << "\n";

		}
		writeFile.close();
	}



	bitset<8> b0;
	FILE* huf_out = fopen("huffman.data", "w+b");
	for (int k = 0; k < saved_vector.size() / 8; k++)
	{

		for (int i = 0; i < 8; i++)
		{
			b0[i] = saved_vector[i + k * 8];

		}

		unsigned char  a = b0.to_ulong();

		fwrite(&a, sizeof(unsigned char), 1, huf_out);
	}

	for (int i = 0; i < saved_vector.size() % 8; i++)
	{
		b0[i] = saved_vector[i + saved_vector.size() - saved_vector.size() % 8];

	}
	for (int i = 0; i < 8 - saved_vector.size() % 8; i++)
	{

		b0[(saved_vector.size() % 8) + i] = 0;
	}
	unsigned char  a = b0.to_ulong();
	fwrite(&a, sizeof(unsigned char), 1, huf_out);

	fclose(huf_out);
	printf("encode end\n");
	return 0;
}
int decode() {
	printf("decode start\n");
	string filePath = "huffman_table.txt";
	map< string, int > m;
	// read File
	vector<int> adj[256];
	int open_index = 0;
	ifstream openFile(filePath.data());
	if (openFile.is_open()) {
		string line;
		while (getline(openFile, line)) {
			if (line != "")
			{
				for (int i = 0; i < line.size(); i++)
				{
					adj[open_index].push_back(line[i] - 48);
					m.insert(make_pair(line, open_index));
				}

			}
			open_index++;
		}
		openFile.close();
	}
	FILE* huf_in = fopen("huffman.data", "rb");//file input
	if (huf_in == NULL)
	{
		printf("File not found!!\n");
		return 0;
	}
	unsigned char test;
	vector<int> saved_vector;//이걸 array로 하면 빨라서 좋을거 같기도하다.

	char bit[8];

	while (fread(&test, sizeof(char), 1, huf_in) == 1) {
		for (int i = 0; i < 8; i++) {
			bit[i] = extract(test, i);
			saved_vector.push_back(bit[i]);
		}
	}

	fclose(huf_in);

	vector<int> temp_vector;

	int count = 0;
	bool oh = true;
	int my_index = 0;
	unsigned char** out_data = mem_alloc2_d(X_MAX, Y_MAX, 0);
	string temp_string = "";
	map< string, int, greater<int>>::iterator iter;


	while (count < 256 * 256)
	{
		temp_vector.push_back(saved_vector[my_index]);
		temp_string.append(to_string(saved_vector[my_index]));
		my_index++;
		iter = m.find(temp_string);
		if (iter != m.end())
		{
			out_data[count / 256][count % 256] = iter->second;//out_data에 담기

			count++;
			temp_string = "";
		}
	}
	FILE* outfile = fopen("output.raw", "w+b");

	for (int i = 0; i < 256; i++)
	{
		fwrite(out_data[i], sizeof(unsigned char), 256, outfile);
	}
	printf("decode end\n");
	fclose(outfile);
	return 0;
}
int is_equal() {
	cout << "is_equal Start" << endl;

	unsigned char** in_data = mem_alloc2_d(X_MAX, Y_MAX, 0);
	// 이미지를 읽어온다
	unsigned char** out_data = mem_alloc2_d(X_MAX, Y_MAX, 0);
	// 이미지를 읽어온다

	FILE* in = fopen("lena256.raw", "rb");//file input
	if (in == NULL)
	{
		printf("File not found!!\n");
		return 0;
	}
	for (int i = 0; i < Y_MAX; i++)
	{
		fread(in_data[i], sizeof(char), X_MAX, in);
	}
	fclose(in);




	FILE* in_out = fopen("output.raw", "rb");//file input
	if (in == NULL)
	{
		printf("File not found!!\n");
		return 0;
	}
	for (int i = 0; i < Y_MAX; i++)
	{
		fread(out_data[i], sizeof(char), X_MAX, in_out);
	}
	fclose(in_out);
	for (int h = 0; h < Y_MAX; h++)
	{
		for (int w = 0; w < X_MAX; w++)
		{
			if (in_data[h][w] != out_data[h][w])
			{
				cout << "[" << h << "][" << w << "] 의 값이 달라버립니다." << endl;
			}
		}
	}
	cout << "is_equal End" << endl;
	return 0;
}
int main(void)
{

	encode();
	decode();
	is_equal();
	return 0;
}