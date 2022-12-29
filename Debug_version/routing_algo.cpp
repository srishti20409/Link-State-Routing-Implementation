#include "node.h"
#include <iostream>

using namespace std;

void printRT(vector<Node*> nd){
/*Print routing table entries*/
	for (int i = 0; i < nd.size(); i++) {
	  nd[i]->printTable();
	}
}

bool Convergence(vector<Node*> nd , vector<routingtbl> prev_tbl){
/*Check if routing tables have converged*/
  bool convergence = true;
  for (int i = 0; i < nd.size(); i++) {
    convergence = convergence && nd[i]->checkConvergence(prev_tbl[i]);
  }
  return convergence;
}

void fillRoutingTable(vector<Node*> nd){
  for(int j = 0 ; j < nd.capacity() ; ++j)
  {
    Node *n = nd[j];
    string myname = n->getName();
    printf("Filling table for %s \n", myname.c_str());
    vector<string> myip = n->getIps();
    for(int k = 0 ; k < nd.capacity() ; k++)
    {
      Node *n2 = nd[k];
      string dstname = n2->getName();
      printf("\t-----Checking if all node %s in %s-----\n", dstname.c_str(), myname.c_str());
      vector<pair<NetInterface, Node*> > check_interfaces = n2->getInterfaces();
      for(int l = 0 ; l < check_interfaces.size() ; l++)
      {
        string to_ip = check_interfaces[l].first.getip();  
        string to_name = check_interfaces[l].first.getip_name();  
        for(int ml = 0 ; ml < myip.size(); ml++)
        {
          if(n->entryexiste(myip[ml] ,to_ip))
          {
            printf("\t [+] Interface %s to %s is present \n",myip[ml].c_str(), to_ip.c_str());
          }
          else
          {
            printf("\t [-] Interface %s to %s is not present adding entry :) \n",myip[ml].c_str(), to_ip.c_str());
            n->add_entry(to_ip, to_name, myip[ml], myname);

          }
        }
      }
    }
  }
}

void routingAlgo(vector<Node*> nd)
{
  bool saturation=false;

  printf("\n~~~~~~~~~~~~~~Routing Algorithm function started~~~~~~~~~~~~~~~\n\n");
  printf("\n~~~~~~~~~~~~~~Printing Inputted Routing tables~~~~~~~~~~~~~~~\n");
  printRT(nd);

  // update all routing entries in my routing table
  printf("\n~~~~~~~~~~~~~~Updating tables~~~~~~~~~~~~~~~\n");
  fillRoutingTable(nd);
  
  //storing previous table for convergence
  vector<routingtbl> prev_tbl;
  for(int i = 0; i<nd.size(); ++i)
  {
    prev_tbl.push_back(nd[i]->getTable());
  }

  // print intial routing table
  printf("\n~~~~~~~~~~~~~~Printing tables before convergences~~~~~~~~~~~~~~~\n");
  printRT(nd);

  
  
  printf("\n~~~~~~~~~~~~~~Calling sendMsg Function~~~~~~~~~~~~~~~\n");
  int j=0;
  for (Node* node: nd){
    node->sendMsg(nd , j);
    j++;
  }

  int i = 0;
  while(1)
  {

      printf("iteration %d \n", i);
      if(Convergence(nd , prev_tbl) || i == 5){
        printf("Convergence reached \n");
        break;  
      }
      else
      {
        printf("Convergence not reached \n");
        int j=0;
        printf("~~~~~~~~~~~~~~Calling sendMsg Function~~~~~~~~~~~~~~~\n");
        for (Node* node: nd){
          // printf("x-----------------------x\n");
          node->sendMsg(nd , j);
          j++;
        }
        prev_tbl.clear();
        for(int k = 0; k<nd.size(); ++k)
        {
          prev_tbl.push_back(nd[k]->getTable());
        }
      }
      i++;
  }
  

  /*Print routing table entries after routing algo converges */
  printf("Printing the routing tables after the convergence \n");
  printRT(nd);
}

int find_cost(routingtbl *mytbl , pair<string,string> my_info , pair<string,string> to_info)
{
  int min = 1807888289;
  for(int i=0; i<mytbl->tbl.size(); ++i)
  {
    RoutingEntry myEntry = mytbl->tbl[i];
    pair<string,string> my_info2 = make_pair(myEntry.ip_interface, myEntry.myname);
    pair<string,string> to_info2 = make_pair(myEntry.dstip, myEntry.dstname);

    if(my_info.first == my_info2.first && to_info.first == to_info2.first)
    {
      if (myEntry.cost <= min)
      {
        min = myEntry.cost;
      }
    }
  }
  // printf("minimum cost between %s and %s is %d \n", my_info.first.c_str(), to_info.first.c_str(), min);
  return min;
}

pair<string, string> find_minimum(vector< pair<int, pair<string, string> > >N_list )
{
  int min = INT32_MAX;
  pair<string, string> minimum;
  for(int i=0; i<N_list.size(); ++i)
  {
    if(N_list[i].first <= min)
    {
      min = N_list[i].first;
      minimum = N_list[i].second;
    }
  }
  return minimum;
}

int find_index_in_vector(vector<string> names, string name)
{
  for(int i=0; i<names.size(); ++i)
  {
    // printf("name in list  %s  normal %s \n", names[i].c_str() , name.c_str());
    if(names[i] == name)
    {
      return i;
    }
  }
  return -1;
}

void Node::recvMsg(RouteMsg *msg) 
{
  vector<routingtbl> recvRoutingTable2 = msg->shared_mytbls;
  vector<pair<string,string>> N_dash; 
  vector< pair<int, pair<string, string> > > N_list_my_table;
  for(int i=0; i<mytbl.tbl.size(); ++i)
  {
    RoutingEntry myEntry = mytbl.tbl[i];
    string dst_ip = myEntry.dstip;
    int cost = myEntry.cost;
    string dst_name = myEntry.dstname;  
    if(myEntry.cost != 0)
    {
      N_list_my_table.push_back(make_pair(cost,make_pair(dst_ip, dst_name)));
    }
    
  }

  // my routing table
  routingtbl *myRoutingTable = &mytbl;
  int flag = 0;

  // while loop that implements djiakstra's algorithm
  while(true)
  {
    // finding the minimum cost entry addming it to N_dash and removing it from N
    pair<string,string> min_cost_entry = find_minimum(N_list_my_table);
    // printf("min cost entry %s \n", min_cost_entry.first.c_str());
    for(int k= 0 ; k < N_list_my_table.size() ; k++)
    {
      if(N_list_my_table[k].second == min_cost_entry)
      {
        N_list_my_table.erase(N_list_my_table.begin()+k);
      }
    }
    N_dash.push_back(min_cost_entry);
    
    
    // comparing costs of the received routing table and my routing table
    for(int i=0; i<mytbl.tbl.size(); ++i)
    {
      RoutingEntry myEntry = mytbl.tbl[i];
      pair<string,string> my_info = make_pair(myEntry.ip_interface, myEntry.myname);
      // printf("min cost entry %s \n", min_cost_entry.first.c_str());
     
      // finding the cost of the minimum cost entry in our routing table
      string dst_ip = myEntry.dstip;
      string next_hop = myEntry.ip_interface;
      string dst_ip_name = myEntry.dstname;

      if(dst_ip_name != min_cost_entry.second && my_info.second != dst_ip_name)
        {
        // printf("src %s --- dst ip %s  %s \n",my_info.first.c_str(),  dst_ip.c_str() , dst_ip_name.c_str());
        pair<string,string> to_info = make_pair(dst_ip, dst_ip_name);
        int mycost = myEntry.cost;
        int index_in_vector = find_index_in_vector(msg->shared_names, min_cost_entry.second);
        routingtbl shared_table = recvRoutingTable2[index_in_vector];
        // printf("cost between: %d  and added : %d \n", find_cost(myRoutingTable, my_info , min_cost_entry) , find_cost(&shared_table , min_cost_entry , to_info) );
        int added_cost;
        // printf("cost of %s to %s is %d \n", my_info.second.c_str() , min_cost_entry.second.c_str(), find_cost(myRoutingTable, my_info , min_cost_entry));
        int min = INT32_MAX;
        for(int z = 0; z < shared_table.tbl.size(); ++z)
        {
          RoutingEntry entry22 = shared_table.tbl[z];
          if(entry22.dstname == to_info.second){
            if(entry22.cost <= min){
              min = entry22.cost;
            }     
          }
        }
        added_cost = min;
        int new_cost = find_cost(myRoutingTable, my_info , min_cost_entry) + added_cost;
        // printf("original cost %d new cost %d \n", mycost, new_cost);
        if(new_cost <= mycost)
        {
          mytbl.tbl[i].cost = new_cost;
          mytbl.tbl[i].nexthop = min_cost_entry.first;
        }
      }
    }
    // flag++;
    if(N_list_my_table.size() == 0)
    {
      flag = 1;
    }
    if(flag == 1)
    {
      break;
    }
  }
}



