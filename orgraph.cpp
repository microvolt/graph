#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <map>
#include <queue>
using namespace std;



class Graph
{
	map < string, map<string, int> > vertices;

	map<string, int> *get_adj_list(const string &name)
	{
		map< string, map<string, int> >::iterator it = vertices.find(name);
		if (it != vertices.end()) {
			return &it->second;
		}
		return NULL;
	}


public:
	void add_vertex(string name)
	{
		if(vertices.find(name) == vertices.end())
		{
			map<string,int > the_list;
			vertices.insert(make_pair(name,the_list));
		}
		else
			return;  //
		
	}
	void add_edge(string name1, string name2, int weight)
	{
		add_vertex(name1);
		add_vertex(name2);
		map < string, map<string, int> >::iterator it1 = vertices.find(name1);
		it1->second.insert(make_pair(name2, weight));
	}

	void delete_vertex(string name)
	{
		map<string, int> *add_list = get_adj_list(name);
		if(!add_list) 
		{
			return;
		}
		map<string, int >::iterator it_m = add_list->begin();
		for(; it_m != add_list->end(); ++it_m)
		{
			map<string, int> *add_list2 = get_adj_list(it_m->first);
			if(add_list2)
			{
				add_list->erase(name);
			}
		}
		vertices.erase(name);
	}

	struct DFSCallback
	{
		vector <string> stack;
		void OnBlack(const string &name)
		{
			stack.push_back(name);
		}

	};

		enum EColor
		{
			white,
			gray,
			black
		};

	template <typename Cb> 
	void DFS_Visit(map<string, EColor> colors, map<string, string> child_parent, string name, Cb &cb)
	{		
		colors[name]=gray;
		map<string, int> discover;
	    map<string, int> finish;
		int time = 0;
		discover.insert(make_pair(name, time + 1));
		time +=time;
		map<string, int> *Adj = get_adj_list(name);
		if(!Adj) 
		{
			return;
		}
		map<string, int >::iterator it_m = Adj->begin();
		for(; it_m != Adj->end(); ++it_m)
		{
			if(colors[it_m->first]=white)
			{
				child_parent.erase(it_m->first);
				child_parent.insert(make_pair(it_m->first, name));
				DFS_Visit(colors, child_parent, it_m->first, cb);
			}
		}
		colors[name]=black;
		finish.insert(make_pair(name, time + 1));
		cb.OnBlack(name);
	}

	template <typename Cb> 
	void DFS(Cb &cb, vector <string> &order = vector <string> ())
	{
		string empty;
		map<string, string> child_parent;
		map<string, EColor> colors;
		{
			map< string, map<string, int> >::iterator it = vertices.begin();
			for(; it != vertices.end(); it++)
			{
				child_parent.insert(make_pair(it->first, empty));
				colors.insert(make_pair(it->first, white));
			}			
		}
		if(order.empty())
		{
			map<string, EColor>::iterator it = colors.begin();
			for(; it != colors.end(); it++)
			{
				if(it->second == white) 
				{
					DFS_Visit(colors, child_parent, it->first, cb);
				}
			}
		}
		else
		{
			map<string, EColor>::iterator it = colors.begin();
			vector<string>::iterator pos = order.begin();
			for(; pos != order.end(); pos++)
			{
				if(colors[*pos] == white) 
					{
						DFS_Visit(colors, child_parent, *pos, cb);
					}
			}
		}
			
	}

	template <typename Cb>
	void print_dfs(Cb &cb)
	{
		DFS(cb);
		map<string, string>::iterator it = child_parent.begin();
		for(; it != child_parent.end(); it++)
			cout<<'edge'<<it->first<<' '<<it->second'\t';
	}

	void Topological_sort()
	{
		DFSCallback cb;
		DFS(cb);

		while(!cb.stack.empty()) 
		{
			cout << cb.stack.back() << endl;
			cb.stack.pop_back();
		}
	}

	void delete_edge(string name1, string name2)
	{
		map<string, int> *add_list1 = get_adj_list(name1);
		add_list1->erase(name2);
	}

	void BFS(string name)
	{
		map<string, EColor> colors;
		map< string, map<string, int> >::iterator it = vertices.begin();
		for(; it != vertices.end(); it++)
		{
			colors.insert(make_pair(it->first, white));
		}
		colors[name] = gray;
		queue <string> q;
		q.push(name);
		while(!q.empty())
		{
			string temp = q.front();
			q.pop();
			map<string, int> *adj = get_adj_list(temp);
			if(!adj)
			{
				return;
			}
			map<string, int>::iterator it = adj->begin();
			for(; it != adj->end(); it++)
			{
				if(colors[it->first] = white)
				{
					colors[it->first] = gray;
					q.push(it->first);
				}
			}
			colors[temp] = black;
		}
	}


	Graph transpose_graph()
	{
		Graph temp;
		map< string, map<string, int> >::iterator it = vertices.begin();
		for(; it != vertices.end(); it++)
		{ 
			temp.add_vertex(it->first);
			map<string, int> *adj = get_adj_list(it->first);
			map<string, int>::iterator it_a = adj->begin();
			for(; it_a != adj->end(); it_a)
			{
				temp.add_vertex(it_a->first);
				map<string, int> *Adj = get_adj_list(it_a->first);
				Adj->insert(make_pair(it->first, it_a->second));
			}
		}
		return temp;
	}

	template <typename Cb> 
	void SCC(Cb &cb)
	{
		DFS(cb);
		Graph g = transpose_graph();
		g.DFS(cb);
		g.print_dfs(cb);
	}
	
};

int main()
{

	return 0;
}

