#include<bits/stdc++.h>
using namespace std;

int rooms; // the total number of rooms

int num_sx,num_tx,num_sy,num_ty;
vector<int>sx_adj;
vector<int>sy_adj;


int adjacent_pairs;
vector<vector<int>>adj; // contains the adjacency of all the rooms
map<pair<int,int>,int>adj_type;

vector<vector<double>> edgesX;
vector<vector<double>> edgesY;
vector<pair<int,int>>edges_setx;
vector<pair<int,int>>edges_sety;
const double  small_positive = 0.01;
const double NEG_INF = -1e8;
const double POS_INF = 1e8;


vector<double>lb_len,ub_len,lb_width,ub_width;

vector<double> x_longest_path,y_longest_path;

vector<double> placementx;
vector<double> placementy;
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
        edges_setx.push_back({0,2*a-1});
        edgesX[0][2*a-1]=0;

    }
    
    cout<<"Please enter the number of rooms that are to be connected to sy: \n";
    cin>>num_sy;

    cout<<"Please enter those rooms: \n";
    for(int i=0;i<num_sy;i++){
        int a;
        cin>>a;
        sy_adj.push_back(a);
        edges_sety.push_back({0,2*a});
        edgesY[0][2*a]=0;
    }
    
}

void input_constraints(){
    // for each room, we are needing the heights and widths range
    cout<<"for each room, we are needing the length and widths range\n";
    double low_len, up_len, low_width,up_width;

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
    edgesX.clear();
    edgesX.resize(2*(rooms+1),vector<double>(2*(rooms+1),POS_INF)); // 2D array for edge weights

    edgesY.clear();
    edgesY.resize(2*(rooms + 1),vector<double>(2*(rooms + 1),POS_INF));
    tblr_rooms();

    adj.clear();
    adj.resize(rooms+1,vector<int>());
    input_adjacency();

    input_constraints();
}



void construct_constraintgraphX(){
    // each node is to be split into 4 nodes - i1,i2,i3,i4. 
    // gx would have nodes i1 and i3 - the left and the right ones.

    // you will have to consider those have adjacencies in the y direction - type 3 and type 4. make the corresponding distances positive

    

    // first add the edges for the widths of each single room - the user constraints
    // the ith room would have 2*i-1 and 2i as the left and right wall respectively
    for(int i=1;i<=rooms;i++){ 
        int left_wall = 2*i-1,right_wall = 2*i;
        edgesX[left_wall][right_wall] = lb_width[i];
        edgesX[right_wall][left_wall] = -1*ub_width[i];
        edges_setx.push_back({left_wall,right_wall});
        edges_setx.push_back({right_wall,left_wall});
    }

    // now add the edges for adjacencies
    for(int i=1;i<=rooms;i++){
        int left_walli = 2*i-1,right_walli = 2*i;
        int top_walli = 2*i-1,bottom_walli=2*i;

        for(int x:adj[i]){
            int left_wallx = 2*x-1,right_wallx = 2*x;
            int top_wallx = 2*x-1,bottom_wallx=2*x;

            int type = adj_type[{i,x}];
            if(type == 1){
                // i is to the right
                edgesX[right_wallx][left_walli]=0;
                edgesX[left_walli][right_wallx]=0;
                edges_setx.push_back({right_wallx,left_walli});
                edges_setx.push_back({left_walli,right_wallx});
                // need to add the constraints for the top walls 

                edgesY[top_walli][bottom_wallx]=NEG_INF;
                edgesY[bottom_wallx][top_walli] = small_positive;
                edges_sety.push_back({top_walli,bottom_wallx});
                edges_sety.push_back({bottom_wallx,top_walli});

                edgesY[top_wallx][bottom_walli]=NEG_INF;
                edgesY[bottom_walli][top_wallx]=small_positive;
                edges_sety.push_back({top_wallx,bottom_walli});
                edges_sety.push_back({bottom_walli,top_wallx});
            }
            else if(type == 2){
                // i is to the left
                edgesX[left_wallx][right_walli]=0;
                edgesX[right_walli][left_wallx]=0;
                edges_setx.push_back({left_wallx,right_walli});
                edges_setx.push_back({right_walli,left_wallx});
                // constraints for top walls
                edgesY[top_walli][bottom_wallx]=NEG_INF;
                edgesY[bottom_wallx][top_walli] = small_positive;
                edges_sety.push_back({top_walli,bottom_wallx});
                edges_sety.push_back({bottom_wallx,top_walli});

                edgesY[top_wallx][bottom_walli]=NEG_INF;
                edgesY[bottom_walli][top_wallx]=small_positive;
                edges_sety.push_back({top_wallx,bottom_walli});
                edges_sety.push_back({bottom_walli,top_wallx});
            }
        }
    }

    
}   

void construct_constraintgraphY(){

    // add the edges for the same room
    for(int i=1;i<=rooms;i++){
        int top_walli = 2*i-1,bottom_walli=2*i;
        edgesY[bottom_walli][top_walli] = lb_len[i];
        edgesY[top_walli][bottom_walli] = -1*ub_len[i];
        edges_sety.push_back({bottom_walli,top_walli});
        edges_sety.push_back({top_walli,bottom_walli});
    }

    for(int i=1;i<=rooms;i++){
        int left_walli = 2*i-1,right_walli = 2*i;
        int top_walli = 2*i-1,bottom_walli=2*i;
        for(int x: adj[i]){
            int left_wallx = 2*x-1,right_wallx = 2*x;
            int top_wallx = 2*x-1,bottom_wallx=2*x;

            int type = adj_type[{i,x}];
            if(type ==3){
                // i is below x
                edgesY[top_walli][bottom_wallx]=0;
                edgesY[bottom_wallx][top_walli]=0;
                edges_sety.push_back({top_walli,bottom_wallx});
                edges_sety.push_back({bottom_wallx,top_walli});
                // constraints for the side walls
                edgesX[left_wallx][right_walli]=small_positive;
                edgesX[right_walli][left_wallx] = NEG_INF;
                edges_setx.push_back({left_wallx,right_walli});
                edges_setx.push_back({right_walli,left_wallx});

                edgesX[left_walli][right_wallx] = small_positive;
                edgesX[right_wallx][left_walli] = NEG_INF;
                edges_setx.push_back({left_walli,right_wallx});
                edges_setx.push_back({right_walli,left_wallx});
            }
            else if(type == 4){
                // x is below i
                edgesY[top_wallx][bottom_walli]=0;
                edgesY[bottom_walli][top_wallx]=0;
                edges_sety.push_back({top_wallx,bottom_walli});
                edges_sety.push_back({bottom_walli,top_wallx});
                // constraints for the side walls
                edgesX[left_wallx][right_walli]=small_positive;
                edgesX[right_walli][left_wallx] = NEG_INF;
                edges_setx.push_back({left_wallx,right_walli});
                edges_setx.push_back({right_walli,left_wallx});

                edgesX[left_walli][right_wallx] = small_positive;
                edgesX[right_wallx][left_walli] = NEG_INF;
                edges_setx.push_back({left_walli,right_wallx});
                edges_setx.push_back({right_wallx,left_walli});
            }
        }
    }
}

void detect_cycles(){

    int flag = 0;


    if(flag==1){
        cout<<"Positive length cycle present - no floorplan exists for given configurations\n";
        exit(1);
    }

}

bool pos_longest_path(vector<double> & placement, vector<pair<int,int>> & edge_set,vector<vector<double>> &edge_weights){
stack<int> s;
s.push(0);
vector<bool> edge_visited(edge_set.size(),false);
vector<bool> is_pushed(2*rooms+1,false);
is_pushed[0]=true;
while(!s.empty()){
    int v=s.top();
    cout << v << endl;
    s.pop();
    for(int i=0;i<edge_set.size();i++){
    int a=edge_set[i].first;
    int b=edge_set[i].second;
    if(a!=v){
        continue; 
    }
    edge_visited[i]=true;
    if(edge_weights[a][b]>=0){
        cout << a << " " << b << " " << placement[b] << " " << placement[a]<< endl;
        if(placement[b]-placement[a]<edge_weights[a][b]){
            placement[b]=placement[a]+edge_weights[a][b];
            cout << a << " " << b << " " << placement[b] << " " << placement[a]<< endl;
        }

    }
    for(int j=1;j<=2*rooms;j++){
        bool all_vis=true;
        for(int i=0;i<edge_set.size();i++){
            int a=edge_set[i].first;
            int b=edge_set[i].second;
            if(b==j&&edge_weights[a][b]>=0&&!edge_visited[i]){
                if(edge_weights[a][b]==0){
                    int x= (a+1)/2;
                    int y=(b+1)/2;
                    if(adj_type[{x,y}]==2||adj_type[{x,y}]==3){
                        all_vis=false;
                    }
                }
                else{
                    all_vis=false;
                }
            }
        }
        if(all_vis&& !is_pushed[j]){

            s.push(j);
            cout << " Pushed "<<j << endl;
            is_pushed[j]=true;
        }
    }
}

}
for(int i=0;i<is_pushed.size();i++){
    if(!is_pushed[i]){
        return false;
        cout << " Returned false in [pos_longest_path]"<<endl;
    }
}
// cout << " Returned true in [pos_longest_path]"<<endl;
return true;
} 

bool longest_path(vector<double> & placement, vector<pair<int,int>> & edge_set,vector<vector<double>> &edge_weights){
int ctr=1;
bool done = false;
int total_neg=0;
for(int i=0;i<edge_set.size();i++){
    int a=edge_set[i].first;
    int b=edge_set[i].second;
    if(edge_weights[a][b]<0){
        total_neg++;
    }
}
cout << " Total_neg = " << total_neg <<endl;
do {
if(!pos_longest_path(placement,edge_set,edge_weights))return false;
ctr++;
done=true;
for(int i=0;i<edge_set.size();i++){
    int a=edge_set[i].first;
    int b=edge_set[i].second;
    if(edge_weights[a][b]<0){
        if(placement[b]-placement[a]<edge_weights[a][b]){
            placement[b]=placement[a]+edge_weights[a][b];
            done=false;
        }
    }

}
}while(ctr<=total_neg&&!done);
if(!done)
cout << " Returned false in [longest_path]"<<endl;
return done;

}
void printPlacements(){
    cout << "Placements in X dir"<<endl;
    for(int i=0;i<placementx.size();i++){
        cout << i << " " << placementx[i]<<endl;
    }
    cout << "Placements in Y dir"<<endl;
    for(int i=0;i<placementy.size();i++){
        cout << i << " " << placementy[i]<<endl;
    }
}
void Compute_Placement(){
    placementx.push_back(0);//sx
    placementy.push_back(0);//sy
    for(int i=0;i<2*rooms;i++){
        placementx.push_back(-1);
        placementy.push_back(-1);
    }
    if(!longest_path(placementx,edges_setx,edgesX)){
       cout << "Not able to assign placement in horizontal constraint graph"<<endl;
    }
    if(!longest_path(placementy,edges_sety,edgesY)){

        cout << "Not able to assign placement in vertical constraint graph"<<endl;
    }

}
void print(){
    cout << "printing horizontal_edges"<<endl;
    for(int i=0;i<edges_setx.size();i++){
        cout << edges_setx[i].first<< " "<< edges_setx[i].second << " " << edgesX[edges_setx[i].first][edges_setx[i].second]<<endl;
    }
    cout << "printing vertical_edges"<<endl;
    for(int i=0;i<edges_sety.size();i++){
        cout << edges_sety[i].first<< " "<< edges_sety[i].second << " " << edgesY[edges_sety[i].first][edges_sety[i].second]<<endl;
    }
}
int main(){
    
    input(); // take all the necessary inputs - 

    construct_constraintgraphX(); // using the inputs
    construct_constraintgraphY();
    print();
    Compute_Placement();
    printPlacements();

    // detect_cycles();

    // longest_path();
}

