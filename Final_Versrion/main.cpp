#include "node.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

vector<Node*> distanceVectorNodes;
vector< vector< std::string > > NodetoInterface;
void routingAlgo(vector<Node*> distanceVectorNodes);
int main() 
{
    int n; 
	std :: vector <string> nodeNames;
    cin>>n;
    string name;
    distanceVectorNodes.clear();
    for (int i = 0 ; i < n; i++) {
      Node *newnode = new Node();
      cin>>name;
	  nodeNames.push_back(name);
      newnode->setName(name);
      distanceVectorNodes.push_back(newnode);
    }
    cin>>name;
    while(name != "EOE") 
	{
		for (int i =0 ; i < distanceVectorNodes.size(); i++) 
		{
			string myeth,oeth, oname;
			if(distanceVectorNodes[i]->getName() == name) 
			{
				cin>>myeth; 	
				cin>>oeth;
				cin>>oname;
				int cost;
				cin>>cost;
				for(int j = 0 ; j < distanceVectorNodes.size(); j++) 
				{
					if(distanceVectorNodes[j]->getName() == oname) 
					{
						distanceVectorNodes[i]->addInterface(myeth, name, oeth, oname, distanceVectorNodes[j]);
						distanceVectorNodes[i]->addTblEntry(myeth, 0 , name);
						distanceVectorNodes[i]->addTblEntry2(name , myeth, oname,  oeth, cost);
						break;
					}
				}
			}
      	}
      	cin>>name;
    }
    routingAlgo(distanceVectorNodes);
}

