/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include "lexer.h"
#include "inputbuf.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

string reserved1[] = { "END_OF_FILE", "ARROW", "HASH", "DOUBLEHASH", "ID", "ERROR" };
struct Rules{
    string lhs;
    string rhs[150];
    int rhs_size;
};
struct Grammar{
    string lrhs[150];
    int lrhs_size;
};
int useless=0;

int getIndex(string array[], string element, int size)
{
    for(int i=0;i<size;i++)
    {
        if(array[i]==element)
        {
            return i;
        }
    }    
}

void Task1(Grammar rss[],string nt[],string t[],int lc, int rc,int tc)
{
    string ntl[100];
    string temp;
    bool flag = false;
    int nc=0;
    for(int i=0;i<lc;i++)
    {
        for(int j=0;j<=rss[i].lrhs_size;j++)
        {
            temp=rss[i].lrhs[j];
            for(int x=0;x<tc;x++)
            {
                if(temp==t[x])
                {
                    goto label11;
                }
            }
            for(int k=0;k<nc;k++)
            {
                if(temp==ntl[k])
                {
                    goto label11;
                }
            }
            ntl[nc]=temp;
            nc++;
            label11:
            flag=true;
            
        }
    }
    for(int i=0;i<tc;i++)
    {
        cout<<t[i]<<" ";
    }
    for(int i=0;i<nc;i++)
    {
        cout<<ntl[i]<"";
    }
     
}

void Task2(Rules rs[],int lc, string allelements[],int noofe, string t[], int tc, string nt[], int ntc,int check)
{
    bool gencheck[noofe];
    bool ruless[lc];
    gencheck[0]= true;
    int counter=0,y=0;
    Rules genr[50];
    int nog=0;
    bool changed=false;
    for(int i=1;i<noofe;i++)
        gencheck[i]=false;
    for(int i=0;i<lc;i++)
        ruless[i]=false;   
    for(int i=1;i<(tc+1);i++)
        gencheck[i]=true;
    cout<<"\n";
    int x=0;
    
    while(x!=10)
    {
        for(int i=0;i<lc;i++)
        {
            for(int j=0;j<rs[i].rhs_size;j++)
            {   
                if(gencheck[getIndex(allelements, rs[i].rhs[j],noofe)]==true)
                {
                    counter++;
                }
            }   
            
            if(counter==rs[i].rhs_size)
            {
                gencheck[getIndex(allelements,rs[i].lhs,noofe)]=true;
                ruless[i]=true;
            }
            counter=0;
        }
        x++;
    }
    for(int i=0;i<lc;i++)
    {
        if(ruless[i]==true)
        {
            genr[nog].lhs=rs[i].lhs;
            genr[nog].rhs_size = rs[i].rhs_size;
            for(int j=0;j<rs[i].rhs_size;j++)
            {
                genr[nog].rhs[j]=rs[i].rhs[j];
            }
            nog++;

        }
    }
    bool rcheck[noofe];
    for(int i=0;i<noofe;i++)
        rcheck[i]=false;
    rcheck[0]=true;
    rcheck[getIndex(allelements,rs[0].lhs,noofe)]=gencheck[getIndex(allelements,rs[0].lhs,noofe)];
    bool rrules[nog];
    for(int i=0;i<nog;i++)
        rrules[i]=false;
    while(y!=10)
    {
        for(int i=0;i<nog;i++)
        {
            for(int j=0;j<genr[i].rhs_size;j++)
            {
                if(rcheck[getIndex(allelements,genr[i].lhs,noofe)]==true)
                {
                    rcheck[getIndex(allelements,genr[i].rhs[j],noofe)]=true;
                    rrules[i]=true;
                }
            }
        }
        y++;
    }
    for(int i=0;i<noofe;i++)
    {
        if(gencheck[i]==true)
            useless=1;
        else
        {
            useless=0;
            break;   
        }
    }
    for(int i=0;i<nog;i++)
    {
        if(rcheck[i]==true)
            {useless=1;}
        else
            {useless=0;
            break;}   
    }
    if(check==1)
    {
    for(int i=0;i<nog;i++)
    {
        if(rrules[i]==true)
        {
            cout<<genr[i].lhs<<"->";
            for(int j=0;j<genr[i].rhs_size;j++)
            {
                cout<<genr[i].rhs[j];
            }
            cout<<"\n";
        }
    }
    }
}

void Task4(Rules rs[],int lcn,string nall[], int noe,string nt[], string t[], int ntc, int tc,int check)
{
    int fnf=tc+2;
    int count=0,count1=0;
    bool flag=false;
    int frule[lcn];
    for(int i=0;i<lcn;i++)
        frule[i]=0;
    string fnfu[fnf];
    fnfu[0]="#";
    fnfu[1]="$";
    for(int i=2;i<fnf;i++)
        fnfu[i]=t[i-2];
    bool first[noe][fnf],follow[noe][fnf];
    for(int i=0;i<noe;i++)
    {
        for(int j=0;j<fnf;j++)
        {
            first[i][j]=false;
            follow[i][j]=false;
        }
    }
    follow[fnf][1]=true;//for start state
    first[0][0]=true;//for #
    first[1][1]=false;//for $
    for(int i=0;i<tc;i++)
    {
        first[i+2][i+2]=true;//for terminals
    }
    int x=0;
    while(x!=10)
    {
    for(int i=0;i<lcn;i++)
    {
        count=0;
        count1=0;
        int lhsi = getIndex(nall,rs[i].lhs,noe);
        //cout<<lhsi;
        for(int j=0;j<rs[i].rhs_size;j++)
        {
            for(int k=0;k<fnf;k++)
            {
                int rhsi=getIndex(nall,rs[i].rhs[0],noe);
                for(int q=0;q<tc;q++)
                {
                    if(rs[i].rhs[0]==t[q] || rs[i].rhs[0]=="#")
                    {
                        if(first[rhsi][k]==true)
                        {
                            first[lhsi][k]=true;
                        }
                    }
                }
            }
            for(int k=1;k<fnf;k++)
            {
                int rhsi=getIndex(nall,rs[i].rhs[0],noe);
                for(int q=0;q<ntc;q++)
                {
                    if(rs[i].rhs[0]==nt[q])
                    {
                        if(first[rhsi][k]==true)
                        {
                            first[lhsi][k]=true;
                        }
                    }
                }
            }
        }
        //rule 2
        for(int w=0;w<rs[i].rhs_size;w++)
        {
            int inrhss = getIndex(nall,rs[i].rhs[w],noe);
            if(first[inrhss][0]==true)
            {
                count1++;
                frule[i]=count1;
            }  
            else
                break;
        }
        if(count1!=rs[i].rhs_size)
        {   
            for(int w1=0;w1<count1;w1++)
            {
                int rhss=getIndex(nall,rs[i].rhs[w1+1],noe);
                for(int k=2;k<fnf;k++)
                {  
                    if(first[rhss][k]==true)
                        first[lhsi][k]=true;
                }  
            }
        }  
        //rule 3
        for(int w=0;w<rs[i].rhs_size;w++)
        {
            int inrhs = getIndex(nall,rs[i].rhs[w],noe);
            for(int q=0;q<ntc;q++)
            {
                if(rs[i].rhs[w]==nt[q])
                {
                    if(first[inrhs][0]==true)
                        count++;
                }
            }
        }

        if(count==rs[i].rhs_size)
        {
            first[lhsi][0]=true;
        }
        //cout<<"\n";

    }
    x++;
    }
    
    for(int i=0;i<lcn;i++)
    {
        //cout<<frule[i]<<"  ";
        int lhi = getIndex(nall,rs[i].lhs,noe);  
        for(int w1=0;w1<frule[i];w1++)
        {
            int rhss=getIndex(nall,rs[i].rhs[w1],noe);
            for(int k=2;k<fnf;k++)
            {  
                if(first[rhss][k]==true)
                    first[lhi][k]=true;
            }  
        }    
    }
    
    if(check==0)
    {
        int pc=0;
        for(int i=fnf;i<noe;i++)
        {
        //cout<<nall[i]<<" ";
        //cout<<"here";
            cout<<"FIRST("<<nall[i]<<") = { ";
            for(int j=0;j<fnf;j++)
            {
                if(first[i][j]==true)
                {
                    pc++;
                    if(pc>1)
                        cout<<", ";
                    cout<<fnfu[j]; 
                }
            }
            pc=0;
            cout<<" }"<<"\n";
        }
    }
    int countf=0,y=0,countf1=0,countf2=0,tp=0;
    while(y!=5)
    {
    for(int i=0;i<lcn;i++)
    {
        int lhsi=getIndex(nall,rs[i].lhs,noe);
        //rule 2
        int rhsi=getIndex(nall,rs[i].rhs[rs[i].rhs_size-1],noe);
        if(rhsi>(tc+2) && rhsi<noe)
        {
            for(int k=0;k<fnf;k++)
            {
                if(follow[lhsi][k]==true)
                    follow[rhsi][k]=true;
            }
        }
            //rule 3
        for(int j=rs[i].rhs_size-1;j>0;j--)
        {
            int rhs1 = getIndex(nall,rs[i].rhs[j],noe);
            if(rhs1>(tc+2))
            {
                if(first[rhs1][0]==true)
                {
                    countf++;
                }
                else
                    break;
            }
            else
                break;
        }
        for(int r=countf;r>=0;r--)
        {
            if(rs[i].rhs_size-r-1>=0)
            {
            int rhsi1=getIndex(nall,rs[i].rhs[rs[i].rhs_size-r-1],noe);
                for(int k=0;k<fnf;k++)
                {
                    if(follow[lhsi][k]==true)
                        follow[rhsi1][k]=true;
                }
            }
        }
        //rule4
        for(int j=0;j<rs[i].rhs_size-1;j++)
        {
            int rhsi3=getIndex(nall,rs[i].rhs[j],noe);
            if(rhsi3>(tc+2) && rhsi3<noe)
            {
                int rhs3n = getIndex(nall,rs[i].rhs[j+1],noe);
                for(int k=1;k<fnf;k++)
                {
                    if(first[rhs3n][k]==true)
                        follow[rhsi3][k]=true;
                }
            }
        }
        //last rule
        for(int j=0;j<rs[i].rhs_size;j++)
        {
            int rhsi3=getIndex(nall,rs[i].rhs[j],noe);
            for(int k=j+1;k<rs[i].rhs_size;k++)
            {
                int rhsi4n=getIndex(nall,rs[i].rhs[k],noe);
                if(first[rhsi4n][0]==true)
                {
                    for(int k1=1;k1<fnf;k1++)
                    {
                        if(first[rhsi4n][k1]==true)
                            follow[rhsi3][k1]=true;
                    }
                }
                else
                {
                    tp=k;
                    break;
                }
                int rhsi4nn=getIndex(nall,rs[i].rhs[tp],noe);
                if(rhsi4nn<(tc+2))
                    follow[rhsi3][rhsi4nn]=true;
            }
        }   
        countf=0;
        countf2=0;
    }
    y++;
    }
    if(check==1)
    {
        int pc=0;
        for(int i=fnf;i<noe;i++)
        {
        //cout<<nall[i]<<" ";
        //cout<<"here";
            cout<<"FOLLOW("<<nall[i]<<") = { ";
            for(int j=1;j<fnf;j++)
            {
                if(follow[i][j]==true)
                {
                    pc++;
                    if(pc>1)
                        cout<<", ";
                    cout<<fnfu[j]; 
                }
            }
            pc=0;
            cout<<" }"<<"\n";
        }
    }
    //rule 1
    int ispre=1;
    for(int i=fnf;i<noe;i++)
    {
        if(first[i][0]==true)
        {
            for(int k=2;k<fnf;k++)
            {
                if(first[i][k]==true && follow[i][k]==true)
                {   
                    ispre=0;
                    break;
                }
            }
        }
    }
    //rule 2
    for(int i=fnf;i<noe;i++)
    {
        for(int k=0;k<lcn;k++)
        {
            if(nall[i]==rs[k].lhs)
            {
                for(int j=k+1;j<lcn;j++)
                {
                    if(nall[i]==rs[j].lhs)
                    {
                        int rhsi=getIndex(nall,rs[k].rhs[0],noe);
                        int rhsi2=getIndex(nall,rs[j].rhs[0],noe);
                        for(int k=2;k<fnf;k++)
                        {
                            if(first[rhsi][k]==true && first[rhsi2][k]==true)
                            {
                                ispre=0;
                                goto lar;
                            }
                        }
                    }
                }
            }
        }
    }
    lar:
    if(check==2)
    {
        if(ispre==1 && useless==1)
            cout<<"YES";
        else if(ispre==0)
            cout<<"NO";
    }
}

void Task5(Rules rs[],int lcn,string nall[], int noe,string nt[], string t[], int ntc, int tc)
{
    int fnf=tc+2;
    int count=0,count1=0;
    bool flag=false;
    int frule[lcn];
    for(int i=0;i<lcn;i++)
        frule[i]=0;
    string fnfu[fnf];
    fnfu[0]="#";
    fnfu[1]="$";
    for(int i=2;i<fnf;i++)
        fnfu[i]=t[i-2];
    bool first[noe][fnf],follow[noe][fnf];
    for(int i=0;i<noe;i++)
    {
        for(int j=0;j<fnf;j++)
        {
            first[i][j]=false;
            follow[i][j]=false;
        }
    }
    follow[fnf][1]=true;//for start state
    first[0][0]=true;//for #
    first[1][1]=false;//for $
    for(int i=0;i<tc;i++)
    {
        first[i+2][i+2]=true;//for terminals
    }
    int x=0;
    while(x!=10)
    {
    for(int i=0;i<lcn;i++)
    {
        count=0;
        count1=0;
        int lhsi = getIndex(nall,rs[i].lhs,noe);
        //cout<<lhsi;
        for(int j=0;j<rs[i].rhs_size;j++)
        {
            for(int k=0;k<fnf;k++)
            {
                int rhsi=getIndex(nall,rs[i].rhs[0],noe);
                for(int q=0;q<tc;q++)
                {
                    if(rs[i].rhs[0]==t[q] || rs[i].rhs[0]=="#")
                    {
                        if(first[rhsi][k]==true)
                        {
                            first[lhsi][k]=true;
                        }
                    }
                }
            }
            for(int k=1;k<fnf;k++)
            {
                int rhsi=getIndex(nall,rs[i].rhs[0],noe);
                for(int q=0;q<ntc;q++)
                {
                    if(rs[i].rhs[0]==nt[q])
                    {
                        if(first[rhsi][k]==true)
                        {
                            first[lhsi][k]=true;
                        }
                    }
                }
            }
        }
        //rule 2
        for(int w=0;w<rs[i].rhs_size;w++)
        {
            int inrhss = getIndex(nall,rs[i].rhs[w],noe);
            if(first[inrhss][0]==true)
            {
                count1++;
                frule[i]=count1;
            }  
            else
                break;
        }
        if(count1!=rs[i].rhs_size)
        {   
            for(int w1=0;w1<count1;w1++)
            {
                int rhss=getIndex(nall,rs[i].rhs[w1+1],noe);
                for(int k=2;k<fnf;k++)
                {  
                    if(first[rhss][k]==true)
                        first[lhsi][k]=true;
                }  
            }
        }  
        //rule 3
        for(int w=0;w<rs[i].rhs_size;w++)
        {
            int inrhs = getIndex(nall,rs[i].rhs[w],noe);
            for(int q=0;q<ntc;q++)
            {
                if(rs[i].rhs[w]==nt[q])
                {
                    if(first[inrhs][0]==true)
                        count++;
                }
            }
        }

        if(count==rs[i].rhs_size)
        {
            first[lhsi][0]=true;
        }
        //cout<<"\n";

    }
    x++;
    }
    
    for(int i=0;i<lcn;i++)
    {
        //cout<<frule[i]<<"  ";
        int lhi = getIndex(nall,rs[i].lhs,noe);  
        for(int w1=0;w1<frule[i];w1++)
        {
            int rhss=getIndex(nall,rs[i].rhs[w1],noe);
            for(int k=2;k<fnf;k++)
            {  
                if(first[rhss][k]==true)
                    first[lhi][k]=true;
            }  
        }    
    }
    
    /*if(check==0)
    {
        int pc=0;
        for(int i=fnf;i<noe;i++)
        {
        //cout<<nall[i]<<" ";
        //cout<<"here";
            cout<<"FIRST("<<nall[i]<<") = { ";
            for(int j=0;j<fnf;j++)
            {
                if(first[i][j]==true)
                {
                    pc++;
                    if(pc>1)
                        cout<<", ";
                    cout<<fnfu[j]; 
                }
            }
            pc=0;
            cout<<" }"<<"\n";
        }
    }*/
    int countf=0,y=0,countf1=0,countf2=0,tp=0;
    while(y!=5)
    {
    for(int i=0;i<lcn;i++)
    {
        int lhsi=getIndex(nall,rs[i].lhs,noe);
        //rule 2
        int rhsi=getIndex(nall,rs[i].rhs[rs[i].rhs_size-1],noe);
        if(rhsi>(tc+2) && rhsi<noe)
        {
            for(int k=0;k<fnf;k++)
            {
                if(follow[lhsi][k]==true)
                    follow[rhsi][k]=true;
            }
        }
            //rule 3
        for(int j=rs[i].rhs_size-1;j>0;j--)
        {
            int rhs1 = getIndex(nall,rs[i].rhs[j],noe);
            if(rhs1>(tc+2))
            {
                if(first[rhs1][0]==true)
                {
                    countf++;
                }
                else
                    break;
            }
            else
                break;
        }
        for(int r=countf;r>=0;r--)
        {
            if(rs[i].rhs_size-r-1>=0)
            {
            int rhsi1=getIndex(nall,rs[i].rhs[rs[i].rhs_size-r-1],noe);
                for(int k=0;k<fnf;k++)
                {
                    if(follow[lhsi][k]==true)
                        follow[rhsi1][k]=true;
                }
            }
        }
        //rule4
        for(int j=0;j<rs[i].rhs_size-1;j++)
        {
            int rhsi3=getIndex(nall,rs[i].rhs[j],noe);
            if(rhsi3>(tc+2) && rhsi3<noe)
            {
                int rhs3n = getIndex(nall,rs[i].rhs[j+1],noe);
                for(int k=1;k<fnf;k++)
                {
                    if(first[rhs3n][k]==true)
                        follow[rhsi3][k]=true;
                }
            }
        }
        //last rule
        for(int j=0;j<rs[i].rhs_size;j++)
        {
            int rhsi3=getIndex(nall,rs[i].rhs[j],noe);
            for(int k=j+1;k<rs[i].rhs_size;k++)
            {
                int rhsi4n=getIndex(nall,rs[i].rhs[k],noe);
                if(first[rhsi4n][0]==true)
                {
                    for(int k1=1;k1<fnf;k1++)
                    {
                        if(first[rhsi4n][k1]==true)
                            follow[rhsi3][k1]=true;
                    }
                }
                else
                {
                    tp=k;
                    break;
                }
                int rhsi4nn=getIndex(nall,rs[i].rhs[tp],noe);
                if(rhsi4nn<(tc+2))
                    follow[rhsi3][rhsi4nn]=true;
            }
        }   
        countf=0;
        countf2=0;
    }
    y++;
    }
    /*if(check==1)
    {
        int pc=0;
        for(int i=fnf;i<noe;i++)
        {
        //cout<<nall[i]<<" ";
        //cout<<"here";
            cout<<"FOLLOW("<<nall[i]<<") = { ";
            for(int j=1;j<fnf;j++)
            {
                if(follow[i][j]==true)
                {
                    pc++;
                    if(pc>1)
                        cout<<", ";
                    cout<<fnfu[j]; 
                }
            }
            pc=0;
            cout<<" }"<<"\n";
        }
    }*/
    //rule 1
    int ispre=1;
    for(int i=fnf;i<noe;i++)
    {
        if(first[i][0]==true)
        {
            for(int k=2;k<fnf;k++)
            {
                if(first[i][k]==true && follow[i][k]==true)
                {   
                    ispre=0;
                    break;
                }
            }
        }
    }
    //rule 2
    for(int i=fnf;i<noe;i++)
    {
        for(int k=0;k<lcn;k++)
        {
            if(nall[i]==rs[k].lhs)
            {
                for(int j=k+1;j<lcn;j++)
                {
                    if(nall[i]==rs[j].lhs)
                    {
                        int rhsi=getIndex(nall,rs[k].rhs[0],noe);
                        int rhsi2=getIndex(nall,rs[j].rhs[0],noe);
                        for(int k=2;k<fnf;k++)
                        {
                            if(first[rhsi][k]==true && first[rhsi2][k]==true)
                            {
                                ispre=0;
                                goto lar;
                            }
                        }
                    }
                }
            }
        }
    }
    lar:
        if(ispre==1 && useless==1)
            cout<<"YES";
        else
            cout<<"NO";
}


int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    Token token;
    string l,r;
    string allelements[200],nall[200];
    int rc1=0;
    Rules rs[50];
    Grammar rss[50];
    string nt[100],t[100],ntl[100];
    int rcn=0,lcn=0,lrcn=0,c=0;
    string temp,temp1;
    int ntc=0,tc=0,noofe=0,noe=0,nc=0;
    bool flag = false;
    //getting the grammar
    LexicalAnalyzer la;
    token = la.GetToken();
    TokenType tt;
    int counter = 0;
    tt = token.token_type;
    string tts = reserved1[(int)tt ]; 
    while(reserved1[(int)tt] != "DOUBLEHASH")
    {
        l=token.lexeme;
        rss[lcn].lrhs[lrcn]=l;
        lrcn++;
        rs[lcn].lhs=l;
        token = la.GetToken();
        tt = token.token_type;
        if(reserved1[(int)tt] == "ARROW")
        {
            token = la.GetToken();
            tt = token.token_type;
            if(reserved1[(int) tt]=="HASH")
            {
                //rss[lcn].lrhs[1]="#";
                rs[lcn].rhs[0] = "#";
                rcn++;
            }   
            while(reserved1[(int)tt] != "HASH")
            {   
                //lrcn++;
                r=token.lexeme;
                rs[lcn].rhs[rcn] = r;
                rss[lcn].lrhs[lrcn]=r;
                token = la.GetToken();
                tt = token.token_type;
                rcn++;
                lrcn++;
            }
            rs[lcn].rhs_size = rcn;
            rss[lcn].lrhs_size = lrcn;
            lcn++;
            //cout<<lcn;
            lrcn=0;
            rcn=0; 
        }
    }
    
    //get all terminals and non terminals in an array
    for(int i=0;i<lcn;i++)
    {
        temp=rs[i].lhs;
        for(int j=0;j<ntc;j++)
        {
            if(temp==nt[j])
            {
                goto label;
            }
        }
        nt[ntc]=temp;
        ntc++;
        label:
        flag=true;
    }
    for(int i=0;i<lcn;i++)
    {
        for(int j=0;j<rs[i].rhs_size;j++)
        {
            temp=rs[i].rhs[j];
            if(temp=="#")
                goto label1;
            for(int x=0;x<ntc;x++)
            {
                if(temp==nt[x])
                {
                    goto label1;
                }
            }
            for(int k=0;k<tc;k++)
            {
                if(temp==t[k])
                {
                    goto label1;
                }
            }
            t[tc]=temp;
            tc++;
            label1:
            flag=true;
            
        }
    }
    for(int i=0;i<lcn;i++)
    {
        temp1=rs[i].lhs;
        for(int x=0;x<tc;x++)
        {
            if(temp1==t[x])
            {
                goto label111;
            }
        }
        for(int k=0;k<nc;k++)
        {
            if(temp1==ntl[k])
            {
                goto label111;
            }
        }
        ntl[nc]=temp1;
        nc++;
        label111:
        flag=true;
        for(int j=0;j<rs[i].rhs_size;j++)
        {
            temp1=rs[i].rhs[j];
            for(int x=0;x<tc;x++)
            {
                if(temp1==t[x]  || temp1=="#")
                {
                    goto label11;
                }
            }
            for(int k=0;k<nc;k++)
            {
                if(temp1==ntl[k])
                {
                    goto label11;
                }
            }
            ntl[nc]=temp1;
            nc++;
            label11:
            flag=true;  
        }
    }
    //for(int i=0;i<nc;i++)
      //  cout<<ntl[i];
    allelements[0]="#";
    nall[0]="#";
    nall[1]="$";
    noe++;
    noe++;
    noofe++;
    for(int i=0;i < tc;i++)
    {
        allelements[noofe]=t[i];
        nall[noe]=t[i];
        noe++;
        counter++;
        noofe++;
    }
    for(int j=0;j<ntc;j++)
    {
        allelements[noofe]=nt[j];
        //nall[noe]=nt[j];
        //noe++;
        noofe++;
    }
    for(int i=0;i<nc;i++)
    {
        nall[noe]=ntl[i];
        noe++;
    }
   
    
    
    // TODO: Read the input grammar at this point from standard input
   
    /*
       Hint: You can modify and use the lexer from previous project
       to read the input. Note that there are only 4 token types needed
       for reading the input in this project.

       WARNING: You will need to modify lexer.cc and lexer.h to only
       support the tokens needed for this project if you are going to
       use the lexer.
     */

    switch (task) {
        case 1:
            // TODO: perform task 1.
            Task1(rss,nt,t,lcn,rcn,tc);
            break;

        case 2:
            // TODO: perform task 2.
            Task2(rs,lcn,allelements,noofe,t,tc,nt,ntc,1);
            //Task2(rs,lcn,nall,noe,t,tc,ntl,nc);
            break;

        case 3:
            // TODO: perform task 3.
            Task4(rs,lcn,nall,noe,ntl,t,nc,tc,0);
            break;

        case 4:
            // TODO: perform task 4.
            Task4(rs,lcn,nall,noe,ntl,t,nc,tc,1);
            break;

        case 5:
            // TODO: perform task 5.
            Task2(rs,lcn,allelements,noofe,t,tc,nt,ntc,0);
            Task5(rs,lcn,nall,noe,ntl,t,nc,tc);
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

