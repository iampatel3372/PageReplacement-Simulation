#include<iostream>
#include<vector>
#include<map>
#include<fstream>
using namespace std;
int lruHit=0,lfuHit=0,fifoHit=0,lruMiss=0,lfuMiss=0,fifoMiss=0;

void fifo(vector<vector<int> >&frame_vector, vector<int>page_references)
{
    int hit=0,fault=0;
    int counter=0;
    for(int i=0;i<page_references.size();i++)
    {
        int found=0;
        for(int j=0;j<frame_vector.size();j++)
        {
            if(i>0)
            {
                frame_vector[j][i]=frame_vector[j][i-1];
            }
        }
        for(int j=0;j<frame_vector.size();j++)
        {
            if(frame_vector[j][i] == page_references[i])
            {
                found=1;
                break;
            }
        }

        if(found) hit++;
        else {
            fault++;
            frame_vector[counter%(frame_vector.size())][i]=page_references[i];
            counter++;
        }
        fifoHit=hit;
        fifoMiss=fault;
    }
}
void lru(vector<vector<int> >&a,vector<int>p)
{
    map <int, int> mp;
    int i,j,hit=0,k;

    int m=p.size();
    int n=a.size();
    vector<int>hi(m);
    for(i=0;i<m;i++){
        vector<pair<int,int> > c;
        for(auto q: mp){
            c.push_back({q.second,q.first});
        }
        sort(c.begin(),c.end());
        bool hasrun=false;
        for(j=0;j<n;j++){
            if(a[j][i]==p[i]){
                hit++;
                hi[i]=1;
                mp[p[i]]=1;
                hasrun=true;
                break;
            }
            if(a[j][i]==-1){
                for(k=i;k<m;k++)
                    a[j][k]=p[i];
                mp[p[i]]++;
                hasrun=true;
                break;
            }
        }
        if(j==n||hasrun==false){
            for(j=0;j<n;j++){
                if(a[j][i]==c[c.size()-1].second){
                    mp.erase(a[j][i]);
                    for(k=i;k<m;k++)
                        a[j][k]=p[i];
                    mp[p[i]]++;
                    break;
                }
            }
        }
        for(auto q:mp){
            if(q.first!=p[i]){
                mp[q.first]++;
            }
        }
    }
    lruHit=hit;
    lruMiss=p.size()-hit;
}
void lfu(vector<vector<int> >&a, vector<int>p)
{
    map <int, int> mp,lfmp;
    int i,j,k;
    int m=p.size();
    int n=a.size();
    for(i=0;i<m;i++){
        vector<int> op;
        vector<pair<int,int>> c,lf;
        for(auto q: mp){
            c.push_back({q.second,q.first});
        }
        for(auto q:lfmp){
            lf.push_back({q.second,q.first});
        }
        sort(lf.begin(),lf.end());
        bool dontCall=true;
        if(lf.size()>2){
            if(lf[0].first!=lf[1].first){
                dontCall=false;
            }
        }
        sort(c.begin(),c.end());
        bool hasrun=false;
        for(j=0;j<n;j++){
            if(a[j][i]==p[i]){
                mp[p[i]]++;
                lfmp[p[i]]++;
                hasrun=true;
                break;
            }
            if(a[j][i]==-1){
                for(k=i;k<m;k++)
                    a[j][k]=p[i];
                mp[p[i]]++;
                lfmp[p[i]]++;
                hasrun=true;
                break;
            }
        }
        if(j==n||hasrun==false){
            for(j=0;j<n;j++){
                if(dontCall==true){
                    int q;
                    if(lf[lf.size()-1].second==c[c.size()-1].second&&lf[lf.size()-1].first>1){
                        if(a[j][i]==c[c.size()-2].second){
                            mp.erase(a[j][i]);
                            lfmp.erase(a[j][i]);
                            for(k=i;k<m;k++)
                                a[j][k]=p[i];
                            mp[p[i]]++;
                            lfmp[p[i]]++;
                            break;
                        }

                    }
                    else{
                        if(a[j][i]==c[c.size()-1].second){
                            mp.erase(a[j][i]);
                            lfmp.erase(a[j][i]);
                            for(k=i;k<m;k++)
                                a[j][k]=p[i];
                            mp[p[i]]++;
                            lfmp[p[i]]++;
                            break;
                        }
                    }
                }
                else if(dontCall==false){
                    if(a[j][i]==lf[0].second){
                        mp.erase(a[j][i]);
                        lfmp.erase(a[j][i]);
                        for(k=i;k<m;k++)
                            a[j][k]=p[i];
                        mp[p[i]]++;
                        lfmp[p[i]]++;
                        break;
                    }
                }
            }
        }
        for(auto q:mp){
            if(q.first!=p[i]){
                mp[q.first]++;
            }
        }
    }
    int hit=0;
    vector<int> hitv(m);
    for(i=1;i<m;i++){
        for(j=0;j<n;j++){
            if(p[i]==a[j][i-1]){
                hit++;
                hitv[i]=1;
                break;
            }
        }
    }
    lfuHit=hit;
    lfuMiss=p.size()-hit;
}
void print(vector<vector<int>>frame_vector,string choice,vector<int>page_reference,int hit, int miss)
{
    int count=12+page_reference.size()+page_reference.size()*2;
    cout<<endl;
    for(int i=0;i<(count/2)-3;i++)
        cout<<"*";
    cout<<" ";
    if(choice == "FF") cout<<"FIFO ";
    else if(choice == "LR") cout<<"LRU ";
    else if(choice == "LF") cout<<"LFU ";
    for(int i=0;i<(count/2)-3;i++)
        cout<<"*";
    cout<<endl;
    cout<<"Page Ref  | ";
    for(int i=0;i<page_reference.size();i++)
    {
        cout<<page_reference[i]<<"  ";
    }
    cout<<endl;
    for(int i=0;i<count-2;i++)
    cout<<"-";
    cout<<endl;
    for(int i=0;i<frame_vector.size();i++)
    {
        cout<<"Frame - "<<i+1<<" | ";
        for(int j=0;j<frame_vector[i].size();j++)
        {
            if(frame_vector[i][j] == -1) cout<<"E"<<"  ";
            else
                cout<<frame_vector[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<"Hits : "<<hit<<" , Faults : "<<miss<<endl;

}

void print_header()
{
    cout<<"\nCS558 : COMPUTER SYSTEM LAB ASSIGNMENT - 05\n";
    cout<<"Instructor : Dr. Manas Khatua and Dr. Moumita Patra\n";
    cout<<"TA's : Alakesh Kalita, Arunav Saikia, Meenu Rani Dey, Priya Undriwade, Vinay Kumar Gupta\n";
    cout<<"Sumbittted By : Himanshu Patel(204101029), Ketan Karnakota(204101030)\n";
}
int main(int argc, char *argv[]) {
    print_header();
    cout<<"-----------------------------------------------------------------------------------------\n";
    ifstream fin;
    if(argc<1) { cout<<"Please provide correct command line arguements\n";}
    string filename=argv[1],choice1="",choice2="",choice3="";
    int count =0,c1=0,c2=0,c3=0;

    if(argc>2) count=1;
    if(argc>2)
    choice1=argv[2];
    if(argc>3) { choice2 = argv[3]; }
    if(argc>4)
        choice3=argv[4];
    int pages,frames;
    fin.open(argv[1]);
    fin>>pages>>frames;
    vector<vector<int> >frame_vector(frames,vector<int>(pages,-1));
    vector<int>page_references(pages);
    for(int i=0;i<pages;i++)
        fin>>page_references[i];
    vector<vector<int> >frames1(frame_vector.begin(),frame_vector.end());
    vector<vector<int> >frames2(frame_vector.begin(),frame_vector.end());
    vector<vector<int> >frames3(frame_vector.begin(),frame_vector.end());
    fifo(frames1,page_references);
    lru(frames2,page_references);
    lfu(frames3,page_references);
    if(count == 0)
    {
        print(frames1,"FF",page_references,fifoHit,fifoMiss);
        print(frames2,"LR",page_references,lruHit,lruMiss);
        print(frames3,"LF",page_references,lfuHit,lfuMiss);

    }
    else
    {
        if(choice1!="")
        {
            if(choice1=="FF") print(frames1,choice1,page_references,fifoHit,fifoMiss);
            else if(choice1=="LF") print(frames3,choice1,page_references,lfuHit,lfuMiss);
            else if(choice1=="LR") print(frames2,choice1,page_references,lruHit,lruMiss);
        }
        if(choice2!="")
        {
            if(choice2=="FF") print(frames1,choice2,page_references,fifoHit,fifoMiss);
            else if(choice2=="LF") print(frames3,choice2,page_references,lfuHit,lfuMiss);
            else if(choice2=="LR") print(frames2,choice2,page_references,lruHit,lruMiss);
        }
        if(choice3!="")
        {
            if(choice3=="FF") print(frames1,choice3,page_references,fifoHit,fifoMiss);
            else if(choice3=="LF") print(frames3,choice3,page_references,lfuHit,lfuMiss);
            else if(choice3=="LR") print(frames2,choice3,page_references,lruHit,lruMiss);
        }

    }

    return 0;
}