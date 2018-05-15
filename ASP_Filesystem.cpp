#include <stdio.h> 
#include <iostream>
using namespace std;

int Clusters_file[10001];
int Files_Cluster[10001];
int Book[10001];

int result = 0;
int Num = 0;
int N, K;

void move(int index, int count)
{
	//cout << "enter index is " << index << " result is " << result << endl;

	if(Book[index] == 1)
	{
		if(count>0)
		{
			result = result + 1;
			//cout << "count is " << count << endl; 
			//cout << "loop index is " << index << " result is " << result << endl;
		}
		return;
	}
	//Book[index] = 1;
	if(Clusters_file[index] == 0)
	{
		Clusters_file[index] = index;
		Clusters_file[Files_Cluster[index]] = 0;
		Book[index] = 1;
		result ++;
		//cout << "index is " << index << " result is " << result << endl;
	}
	else
	{
		count++;
		result ++ ;
		int temp = Clusters_file[index];
		Clusters_file[index] = index;
		Clusters_file[Files_Cluster[index]] = 0;
		Book[index] = 1;
		move(temp, count);
	}
}

int main()
{
	/*
	   freopen function below opens input.txt file in read only mode, and afterward,
	   the program will read from input.txt file instead of standard(keyboard) input.
	   To test your program, you may save input data in input.txt file,
	   and use freopen function to read from the file when using scanf function.
	   You may remove the comment symbols(//) in the below statement and use it.
	   But before submission, you must remove the freopen function or rewrite comment symbols(//).
	 */
	freopen("input.txt", "r", stdin);
	/*
	   If you remove the statement below, your program's output may not be recorded
	   when your program is aborted due to the time limit.
	   For safety, please use setbuf(stdout, NULL); statement.
	 */
	int test_case = 0;
	//int result;

	int i, j;
	cin >> test_case;
	for (int k =1;k<=test_case;k++)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Please, implement your algorithm from this section.
		*/
		/////////////////////////////////////////////////////////////////////////////////////////////
		for(i=0;i<=10000;i++)
		{
			Clusters_file[i] = 0;
			Files_Cluster[i] = 0;
			Book[i] = 0;
		}
		cin >> N >> K;
		//cout << "N is " << N <<" K is " << K<< endl;
		Num = 0;
		for(i=0;i<K;i++)
		{
			int filec = 0;
			cin >> filec;
			for(j=0;j<filec;j++)
			{
				int cluster = 0;
				cin>>cluster;
				Num++;
				Clusters_file[cluster] = Num;
				Files_Cluster[Num] = cluster;
			}
		}

		for(i=1;i<=N;i++)
		{
			cout << Clusters_file[i] << "\t";
		}
		cout << endl;
		for(i=1;i<=Num;i++)
		{
			cout << Files_Cluster[i] << "\t";
		}
		cout << endl;
		
		result = 0;
		for(i=1;i<=Num;i++)
		{
			//cout << "move num is " << i << endl;
			move( i, 0);	
		}
		
		cout << "operate" << endl;
		for(i=1;i<=N;i++)
		{
			cout << Clusters_file[i] << " ";
		}
		cout << endl;
		cout << "N is " << N << " num is " << Num << endl;
		for(i=1;i<=Num;i++)
		{
			cout << Files_Cluster[i] << " ";
		}
		cout << endl;
		for(i=1;i<=Num;i++)
		{
			cout << Book[i] << " ";
		}
		cout << endl;
		
		cout << "#" << k <<" "<< result << endl;
	}

	return 0;
}

