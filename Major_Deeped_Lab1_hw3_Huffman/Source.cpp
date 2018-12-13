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
unsigned int bitsToBytes(unsigned char *bits)
{
	unsigned int sum = 0;
	for (int i = 0; i < 8; i++)
	{
		sum += bits[i] - '0';
		sum <<= 1;
	}
	return sum;

}

template<typename T>
void pop_front(std::vector<T>& vec)
{
	assert(!vec.empty());
	vec.erase(vec.begin());
}
void test(vector<int> myvector[]) {
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < myvector[i].size(); j++)
		{
			if (j == 0)
			{
				std::cout << i << " : ";
			}
			std::cout << myvector[i][j];
			if (j == myvector[i].size() - 1)
				std::cout << endl;

		}
	}
	printf("test\n");

}
void histogram_generate(unsigned char **in, int * hist)
{

	//double hist_cdf[256] = { 0.0 };//HistogramEqualization 을 담을 배열입니다.
	//unsigned char** eq_img = mem_alloc2_d(X_MAX, Y_MAX, 0);
	//double histogram_sum = 0;
	for (int i = 0; i < 256; i++)
		hist[i] = 0;//initialze
	for (int h = 0; h < Y_MAX; h++)
	{
		for (int w = 0; w < X_MAX; w++)
		{
			hist[in[h][w]]++;//히스토 그램을 만듭니다.
		}
	}
	//for (int i = 0; i < 256; i++)
	//{
	//	histogram_sum = histogram_sum + hist[i];// 히스토그램의 누적 합을 [0,1]로 정규화 한 cdf를 구한다.
	//	hist_cdf[i] = histogram_sum / (Y_MAX *X_MAX);//HistogramEqualization을 실행합니다.
	//}
	//for (int h = 0; h < Y_MAX; h++)
	//{
	//	for (int w = 0; w < X_MAX; w++)
	//	{//히스토그램의 누적 합을 [0,1]로 정규화 한 cdf를 구한다.
	//		eq_img[h][w] = hist_cdf[in[h][w]] * 255;//히스템그램을 이미지로 만듭니다.
	//	}
	//}
	//string output = "Histogram_Equalization";//결과를 raw 이미지로 출력합니다.
	//output.append(".raw");
	//FILE* outfile = fopen(output.c_str(), "w+b");
	//for (int i = 0; i < Y_MAX; i++)
	//{
	//	fwrite(eq_img[i], sizeof(char), X_MAX, outfile);
	//}
	//fclose(outfile);
}
unsigned char extract(unsigned char data, int i) {
	return (data >> i) & 1;
}
int encode() {
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
	//int num = 0;
	//for (int i = 0; i < 256; i++)
	//{
	//	if (hist[i] != 0)
	//	{
	//		printf("%d : %d\n", i, hist[i]);
	//		num++;
	//	}

	//}
	printf("Complete!\n");
	//unsigned char arr[] = {10,100,150,250,1,2};
	//int freq[] = { 10,20,30,40,1,2};
	//int size = sizeof(arr) / sizeof(arr[0]);

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
	int N = 256;
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
		b0[i] = saved_vector[i + saved_vector.size() - 8];

	}
	for (int i = 0; i < 8 - saved_vector.size() % 8; i++)
	{
		b0[i] = 0;

	}
	unsigned char  a = b0.to_ulong();
	fwrite(&a, sizeof(unsigned char), 1, huf_out);

	fclose(huf_out);
	std::cout << test_count << endl;

	return 0;
}
int decode() {

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
					adj[open_index].push_back(line[i]-48);
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
	//saved_vector.clear();//초기화 해보고 다시 가보자
	vector<int> saved_vector;//이걸 array로 하면 빨라서 좋을거 같기도하다.

	char bit[8];

	while (fread(&test, sizeof(char), 1, huf_in) == 1) {
		for (int i = 0; i < 8; i++) {
			bit[i] = extract(test, i);
			saved_vector.push_back(bit[i]);
		}
	}

	fclose(huf_in);



	//saved vector를 write 해야하고
	//saved vector처럼 써야하네
	vector<int> temp_vector;//이진법화 하는게 잘 안되네

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
			out_data[count / 256][count % 256] = iter->second;

			count++;
			temp_string = "";
		}


		/*cout << "test";*/


		/*pop_front(saved_vector);*/
		//for (int i = 0; i < 256; i++)
		//{
		//	if (adj[i].size() == 0)//continue
		//		continue;
		//	if (temp_vector.size() == adj[i].size())
		//	{
		//		for (int j = 0; j < temp_vector.size(); j++)
		//		{

		//			if (temp_vector[j] != adj[i][j])
		//			{
		//				break;
		//			}
		//			if (j == temp_vector.size() - 1)//마지막꺼 까지 같을때
		//			{
		//				out_data[count / 256][count % 256] = i;

		//				count++;
		//				temp_vector.clear();
		//				oh = false;//2중 for문 탈출
		//				break;
		//			}
		//		}
		//		if (oh == false)
		//		{
		//			oh = true;
		//			break;
		//		}
		//	}
		//}
	}
	std::cout << count << endl;
	FILE* outfile = fopen("output.raw", "w+b");

	for (int i = 0; i < 256; i++)
	{
		fwrite(out_data[i], sizeof(unsigned char), 256, outfile);
	}
	return 0;
}
int main(void)
{
	printf("encode start\n");
	encode();
	printf("decode start\n");
	decode();
	return 0;
}