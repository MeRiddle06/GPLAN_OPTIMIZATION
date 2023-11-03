#include<bits/stdc++.h>
using namespace std;

int rooms; // the total number of rooms

int num_sx,num_tx,num_sy,num_ty;
vector<int>sx_adj;
vector<int>sy_adj;


int adjacent_pairs;
vector<vector<int>>adj; // contains the adjacency of all the rooms
map<pair<int,int>,int>adj_type;

vector<int>lb_len,ub_len,lb_width,ub_width;

/*
LEFT WALL - 1
TOP WALL - 2
RIGHT WALL - 3
BOTTOM WALL - 4

1-3 adjacency - type 1 (the first room in the pair is to the right)
3-1 adjacency - type 2 (the first room in the pair is to the left)
2-4 adjacency - type 3 (the first room in the pair is to the bottom side)
4-2 adjacency - type 4 (the first room in the pair is to the top side)
*/

void input_adjacency(){ 
    // for adjacency, if room i is adjacent to room j, we need to know that how are they adjacent 
    // that is, which wall of room i is adjacent to which wall of room j. 
    
    cout<<"PLease enter the number of pairs of rooms that are adjacent: ";
    cin>>adjacent_pairs;

    
    cout<<"Enter adjacencies in this format: ROOM 1, ROOM 2, adjacency type\n";

    for(int i=0;i<adjacent_pairs;i++){
        int ri,rj,type;
        cin>>ri>>rj>>type;

        if(type<1 || type>4){
            cout<<"WRONG TYPE OF ADJACECNCY\n";
            exit(1);
        }

        adj_type[{ri,rj}]=type;
        if(type%2)
            adj_type[{rj,ri}]=type+1;
        else 
            adj_type[{rj,ri}]=type-1;

        adj[ri].push_back(rj);
        adj[rj].push_back(ri);

    }
}

void tblr_rooms(){
    // need this for connections to sx,sy,tx,ty

    cout<<"Please enter the number of rooms that are to be connected to sx: \n";
    cin>>num_sx;

    cout<<"Please enter those rooms: \n";
    for(int i=0;i<num_sx;i++){
        int a;
        cin>>a;
        sx_adj.push_back(a);
    }

    cout<<"Please enter the number of rooms that are to be connected to sy: \n";
    cin>>num_sy;

    cout<<"Please enter those rooms: \n";
    for(int i=0;i<num_sy;i++){
        int a;
        cin>>a;
        sy_adj.push_back(a);
    }
    
}

void input_constraints(){
    // for each room, we are needing the heights and widths range
    cout<<"for each room, we are needing the length and widths range\n";
    int low_len, up_len, low_width,up_width;

    lb_len.push_back(-1),ub_len.push_back(-1),lb_width.push_back(-1),ub_width.push_back(-1);

    for(int i=1;i<=rooms;i++){
        
        cout<<"For room "<<i<<", please enter the lower and upper bound for length\n";

        cout<<"Lower Length: \n";
        cin>>low_len;
        cout<<"Upper Length: \n";
        cin>>up_len;

        cout<<"Lower width: \n";
        cin>>low_width;
        cout<<"Lower width: \n";
        cin>>up_width;

        lb_len.push_back(low_len);
        ub_len.push_back(up_len);
        lb_width.push_back(low_width);
        ub_width.push_back(up_width);

    }
}

void input(){
    cout<<"Please enter the number of rooms: ";
    cin>>rooms;
    tblr_rooms();

    adj.clear();
    adj.resize(rooms+1,vector<int>());
    input_adjacency();

    input_constraints();
}


void construct_constraintgraph(){

}

void detect_cycles(){

    int flag = 0;


    if(flag==1){
        cout<<"Positive length cycle present - no floorplan exists for given configurations\n";
        exit(1);
    }

}

void longest_path(){

}

int main(){
    
    input(); // take all the necessary inputs - 

    construct_constraintgraph(); // using the inputs

    // detect_cycles();

    // longest_path();
}