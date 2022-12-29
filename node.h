#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class RoutingEntry{
 public:
  string dstip, nexthop;
  string ip_interface;
  string myname , dstname;
  int cost;
};

class Comparator{
 public:
  bool operator()(const RoutingEntry &R1,const RoutingEntry &R2){
    if (R1.cost == R2.cost) {
      return R1.dstip.compare(R2.dstip)<0;
    }
    else if(R1.cost > R2.cost) {
      return false;
    }
    else {
      return true;
    }
  }
} ;

struct routingtbl {
  vector<RoutingEntry> tbl;

  void print(){
    for (int i = 0; i < tbl.size(); ++i) {
      printf("to: %s | src: %s | hop: %s | cost: %d\n",tbl[i].dstip.c_str() , tbl[i].ip_interface.c_str(), tbl[i].nexthop.c_str(), tbl[i].cost );
      // cout<<tbl[i].dstip<<" | "<<tbl[i].nexthop<<" | "<<tbl[i].ip_interface<<" | "<<tbl[i].cost <<endl;
    }
  }
};


class RouteMsg {
 public:
  string my_name;
  struct routingtbl *mytbl;
  vector<struct routingtbl> shared_mytbls;
  vector<string> shared_names;

  void add_tbl(struct routingtbl tbl) {
    struct routingtbl table = tbl;
    shared_mytbls.push_back(table);
  }
  void add_name(string input_n) {
    string abc = input_n;
    shared_names.push_back(abc);
  }
  void add_mytbl(struct routingtbl tbl) {
    mytbl = &tbl;
  }
};


class NetInterface {
 private:
  string ip;
  string ip_name;
  string connectedTo; 	
  string connectedTo_name; 
  
 public:
  string getip() {
    return this->ip;
  }
  string getip_name() {
    return this->ip_name;
  }
  string getConnectedIp_name() {
    return this->connectedTo_name;
  }

  string getConnectedIp() {
    return this->connectedTo;
  }
  void setip(string ip) {
    this->ip = ip;
  }
  void setConnectedip(string ip) {
    this->connectedTo = ip;
  }
  void setip_name(string ip_name) {
    this->ip_name = ip_name;
  }
  void setConnectedip_name(string ip_name) {
    this->connectedTo_name = ip_name;
  }
  
};

class Node {
  private:
  string name;
  vector<pair<NetInterface, Node*> > interfaces;
  protected:
  struct routingtbl mytbl;

  bool isMyInterface(string eth) {
    for (int i = 0; i < interfaces.size(); ++i) 
    {
      if(interfaces[i].first.getip() == eth)
	      return true;
    }
    return false;
  }
  
  public:
  // getter functions
  string getName() {
    return this->name;
  }
  struct routingtbl getTable() {
    return mytbl;
  }
  vector<string> getIps() {
     vector<string> ips;
    for (int i = 0; i < interfaces.size(); ++i) {
      ips.push_back(interfaces[i].first.getip());
    }
    return ips;
  }

  // setter functions
  void setName(string name){
    this->name = name;
  }

  void addInterface(string ip, string ip_name, string connip, string connip_name,  Node *nextHop) {
    NetInterface eth;
    eth.setip(ip);
    eth.setip_name(ip_name);
    eth.setConnectedip_name(connip_name);
    eth.setConnectedip(connip);
    interfaces.push_back({eth, nextHop});
  }

  // adding a new entry to the routing table
  void addTblEntry(string myip, int cost , string myname) {
    RoutingEntry entry;
    entry.dstip = myip;
    entry.nexthop = myip;
    entry.ip_interface = myip;
    entry.cost = cost;
    entry.myname = myname;
    entry.dstname = myname;
    mytbl.tbl.push_back(entry);
  }
  void addTblEntry2(string myname, string myip, string dstname , string dstip, int cost) {
    RoutingEntry entry;
    entry.dstip = dstip;
    entry.nexthop = dstip;
    entry.ip_interface = myip;
    entry.cost = cost;
    entry.myname = myname;
    entry.dstname = dstname;
    mytbl.tbl.push_back(entry);
    
  }

  // Print the routing table
  void printTable() {
    Comparator myobject;
    sort(mytbl.tbl.begin(),mytbl.tbl.end(),myobject);
    cout<<"\t "<<this->getName()<<":"<<endl;
    for (int i = 0; i < mytbl.tbl.size(); ++i) {
      printf("\t to: %s | src: %s | hop: %s | cost: %d\n",mytbl.tbl[i].dstip.c_str() , mytbl.tbl[i].ip_interface.c_str(), mytbl.tbl[i].nexthop.c_str(), mytbl.tbl[i].cost );
      // cout<<mytbl.tbl[i].dstip<<" | "<<mytbl.tbl[i].nexthop<<" | "<<mytbl.tbl[i].ip_interface<<" | "<<mytbl.tbl[i].cost <<endl;
    }
  }

  bool checkConvergence(routingtbl prev_tbl) {
    // printf("Previous table:");
    // for (int i = 0; i < prev_tbl.tbl.size(); ++i) {
    //   printf("\t to: %s | src: %s | hop: %s | cost: %d\n",prev_tbl.tbl[i].dstip.c_str() , prev_tbl.tbl[i].ip_interface.c_str(), prev_tbl.tbl[i].nexthop.c_str(), prev_tbl.tbl[i].cost );
    // }
    // printf("Current table:");
    // for( int i=0; i<mytbl.tbl.size(); i++)
    // {
    //   printf("\t to: %s | src: %s | hop: %s | cost: %d\n",mytbl.tbl[i].dstip.c_str() , mytbl.tbl[i].ip_interface.c_str(), mytbl.tbl[i].nexthop.c_str(), mytbl.tbl[i].cost );
    // }
    for(int i=0; i<mytbl.tbl.size(); i++)
    {
      int flag = 0;
      for(int j=0; j<prev_tbl.tbl.size(); j++){
        RoutingEntry entry = mytbl.tbl[i];
        RoutingEntry prev_entry = prev_tbl.tbl[j];
        if (entry.nexthop == prev_entry.nexthop && entry.cost == prev_entry.cost && entry.dstip == prev_entry.dstip && entry.ip_interface == prev_entry.ip_interface && entry.myname == prev_entry.myname && entry.dstname == prev_entry.dstname)
        {
          // printf("to: %s | src: %s | hop: %s | cost: %d\n",mytbl.tbl[i].dstip.c_str() , mytbl.tbl[i].ip_interface.c_str(), mytbl.tbl[i].nexthop.c_str(), mytbl.tbl[i].cost );
          // printf("to: %s | src: %s | hop: %s | cost: %d\n",prev_tbl.tbl[j].dstip.c_str() , prev_tbl.tbl[j].ip_interface.c_str(), prev_tbl.tbl[j].nexthop.c_str(), prev_tbl.tbl[j].cost );
          flag= 1;
          // return false;
        }
      }
      if(flag == 0)
        return false;
    }
    return true;
  }

  bool entryexiste(string myip, string dstip) {
    for (int i=0; i<mytbl.tbl.size(); i++){
      RoutingEntry entry = mytbl.tbl[i];
      if (entry.dstip == dstip && entry.ip_interface == myip)
        return true;
      else if (myip == dstip)
        return true;
    }
    return false;
  }

  void add_entry(string dstip, string dstname, string myip, string myname) {
    int found = 0;
    int cost;
    string nexthop;
    for (int i=0; i<mytbl.tbl.size(); i++)
    {
      RoutingEntry entry = mytbl.tbl[i];
      if (entry.dstname == dstname){
        cost = entry.cost;
        found = 1;  
        if(dstip == entry.dstip){
          nexthop = entry.ip_interface;
          }  
        else if(myip == entry.ip_interface){
            nexthop = entry.dstip;
          }
        else{
            nexthop = entry.nexthop;
          }
      }
    }

    if(myname == dstname)
    {
      // printf("*****myname == dstname\n");
      RoutingEntry entry;
      entry.dstip = dstip;
      entry.nexthop = dstip;
      entry.ip_interface = myip;
      entry.cost = 0;
      entry.myname = myname;
      entry.dstname = dstname;
      mytbl.tbl.push_back(entry);
      // printf("\t !! [+] Added entry: %s | %s | %s | %d\n",entry.dstip.c_str() , entry.ip_interface.c_str(), entry.nexthop.c_str(), entry.cost);
    }
    else if(found == 1)
    {
      RoutingEntry entry;
      entry.dstip = dstip;
      entry.nexthop = nexthop;
      entry.ip_interface = myip;
      entry.cost = cost;
      entry.myname = myname;
      entry.dstname = dstname;
      mytbl.tbl.push_back(entry);
      // printf("\t !! [+] Added entry: %s | %s | %s | %d\n",entry.dstip.c_str() , entry.ip_interface.c_str(), entry.nexthop.c_str(), entry.cost);

    }
    else
    {
      RoutingEntry entry;
      entry.dstip = dstip;
      entry.nexthop = myip;
      entry.ip_interface = myip;
      entry.cost = INT16_MAX;
      entry.myname = myname;
      entry.dstname = dstname;
      mytbl.tbl.push_back(entry);
      // printf("\t !! [+] Added entry: %s | %s | %s | %d\n",entry.dstip.c_str() , entry.ip_interface.c_str(), entry.nexthop.c_str(), entry.cost);

    }
  }

  bool interfaceExists(string eth) {
    for (int i = 0; i < interfaces.size(); ++i) {
      if(interfaces[i].first.getip() == eth)
            return true;
    } 
    return false;
  }

  vector<pair<NetInterface, Node*> > getInterfaces() {
    return interfaces;
  }

  void updateTblEntry(string dstip, int cost) {
    // to update the dstip hop count in the routing table (if dstip already exists)
    // new hop count will be equal to the cost 
    for (int i=0; i<mytbl.tbl.size(); i++){
      RoutingEntry entry = mytbl.tbl[i];

      if (entry.dstip == dstip) 
        mytbl.tbl[i].cost = cost;

    }
    // remove interfaces 
    for(int i=0; i<interfaces.size(); ++i){
      // if the interface ip is matching with dstip then remove
      // the interface from the list
      if (interfaces[i].first.getConnectedIp() == dstip) {
        interfaces.erase(interfaces.begin() + i);
      }
    }
  }
  
  public:
  void sendMsg(vector<Node*> nd , int flag)
  {
      RouteMsg msg;
      for(int j = 0 ; j < nd.capacity() ; ++j)
      {
        if(j == flag)
        {
          // printf("my name = %s\n", name.c_str());
          Node* bor = nd[j];
          msg.my_name = bor->getName();
          msg.add_mytbl(bor->getTable());
        }
        // printf("j=%d, flag=%d\n",j, flag);
        if(j != flag)
        {
          // add the shared table of the node to the message
          Node* bor = nd[j];
          // printf("node name = %s\n", bor->getName().c_str());
          msg.add_tbl(bor->getTable());
          // printf("table received\n");
          string names = bor->getName();
          // printf("stored in string %s\n", names.c_str());
          msg.add_name(names);
          // printf("shared table of %s\n", bor->getName().c_str());
        }
      }
      // send the message to all the nodes
      for(int j = 0 ; j < nd.capacity() ; ++j)
      {
        if(j == flag)
        {
          Node* bor = nd[j];
          // sending message to jth node
          bor->recvMsg(&msg);

        }
      }
  }
  public:
  void recvMsg(RouteMsg *msg);
};
