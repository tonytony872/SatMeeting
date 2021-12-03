#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
    stringstream ss;
    ss<<argv[1];
    int num_file=0, num_gates=0, num_route=0, route=0;
    ss>>num_file;
    ss.str("");
    ss.clear();
    ss<<argv[2];
    ss>>num_gates;
    ss.str("");
    ss.clear();
    ss<<argv[3];
    ss>>num_route;
    for(int i=0;i<num_file;i++){
        string s=to_string(i);
        ofstream ofile("poke_map_"+ s +".txt");
        route=0;
        ofile<<"total_route "<<num_route<<"\n";
        for(int j=0;j<num_gates/2;j++){// F GATE AND OW GATE FOR VARIABLES
            string s1=to_string(3*j+1), s2=to_string(3*j+2), s3=to_string(3*j+3);
            ofile<<"F "<<s1<<" "<<s2<<" "<<s3<<"\n";
            // ofile<<"OW "<<s2<<" "<<(3*(j+1)+1)<<"\n";
            // ofile<<"OW "<<s3<<" "<<(3*(j+1)+1)<<"\n";
        }
        for(int j=0;j< (num_gates/2);j++){//ON gates, random input
            ofile<<"ON "<<(3*j+2)<<" ";
            int h=rand()%5+1, k2=route;
            for(int k=k2;k<k2+h;k++){
                ofile<<3*(num_gates/2)+2+k<<" ";
                route++;
                if(3*(num_gates/2)+2+route>=num_route/2){break;}
            }
            ofile<<"\n";
            if(3*(num_gates/2)+2+route>num_route/2){break;}
            h=rand()%5+1;
            k2=route;
            ofile<<"ON "<<(3*j+3)<<" ";
            for(int k=k2;k<h+k2;k++){
                ofile<<3*(num_gates/2)+2+k<<" ";
                route++;
            }
            ofile<<"\n";
        }
        for(int j=0;j<route;j++){
            if(rand()%10+1 > 3){
                int a=j+3*(num_gates/2)+2;
                int b=3*(num_gates/2)+2+rand()%route;
                if(a!=b){
                    ofile<<"XOR ";
                    ofile<<a<<" ";
                    ofile<<b<<"\n";
                }
            }
        }
        int h_route=0;
        int l_route=0;
        while(h_route<=num_route/4 && 3*(num_gates/2)+2+route+h_route<num_route-1){
            bool flag=0;
            while((!flag||rand()%10+1>2)&& 3*(num_gates/2)+2+route+h_route){
                int a=3*(num_gates/2)+2+rand()%route;
                int b=3*(num_gates/2)+2+rand()%route;
                if(a!=b){
                    ofile<<"H ";
                    ofile<<a<<" ";
                    ofile<<b<<" ";
                    ofile<<3*(num_gates/2)+2+route+h_route<<"\n";
                    h_route++;
                    flag=1;
                }
            }
            ofile<<"L ";
            ofile<<3*(num_gates/2)+1<<" ";
            ofile<<0<<" ";
            ofile<<3*(num_gates/2)+2+route+h_route-1<<" \n";
        }
        ofile.close();
    }
}