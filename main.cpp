#include <iostream>
#include <stdio.h>
#include<math.h>
#include <iomanip>
#include <algorithm>

using namespace std;

void smith_predictor(char a, int bit_num, int& counter,int& hit_num,int& miss_num)
{
    char pred;

    if(a=='t' || a=='n'){

    //predict
    if(counter>=pow(2,bit_num)/2)//eg, for 3-bit, if counter>=4, predict taken
    { pred='t';}
    else{pred='n';}

    //determine if prediction=actual outcome
    if(pred==a)  {hit_num++;}
    else{miss_num++;}

    //update counter
    if(a=='t') {
        if(counter==pow(2,bit_num)-1)
            {counter=counter+0;}
        else{counter+=1;}
    }
    else{//a==n, n-not taken
        if(counter==0)
            {counter=counter-0;}
        else{counter-=1;}
    }

    }
    else{ std::cout<<"invalid input";}

}


void bimodal_predictor(char a, char& pred2, int& counter2,int& hit_num2,int& miss_num2)
{
    int bit_num=3;   //each entry is 3-bit (decimal:0,1,2..,7)

    if(a=='t' || a=='n'){

    //predict
    if(counter2>=pow(2,bit_num)/2)//eg, for 3-bit, if counter>=4, predict taken
    { pred2='t';}
    else{pred2='n';}

    //determine if prediction=actual outcome
    if(pred2==a)  {hit_num2++;}
    else{miss_num2++;}

    //update counter
    if(a=='t') {
        if(counter2==pow(2,bit_num)-1)
            {counter2=counter2+0;}
        else{counter2+=1;}
    }
    else{//a==n, n-not taken
        if(counter2==0)
            {counter2=counter2-0;}
        else{counter2-=1;}
    }

    }
    else{ std::cout<<"invalid input";}

}


void gshare_predictor(char a, char& pred1,int& counter1,int& hit_num1,int& miss_num1)
{

    int bit_num=3;   //each entry is 3-bit (decimal:0,1,2..,7)

    if(a=='t' || a=='n'){

    //predict
    if(counter1>=pow(2,bit_num)/2)//eg, for 3-bit, if counter>=4, predict taken
    { pred1='t';}
    else{pred1='n';}

    //determine if prediction=actual outcome
    if(pred1==a)  {hit_num1++;}
    else{miss_num1++;}

    //update counter
    if(a=='t') {
        if(counter1==pow(2,bit_num)-1)
            {counter1=counter1+0;}
        else{counter1+=1;}
    }
    else{//a==n, n-not taken
        if(counter1==0)
            {counter1=counter1-0;}
        else{counter1-=1;}
    }

    }
    else{ std::cout<<"invalid input";}

}

void hybird_predictor(char a,char pred2,char pred1,char pred0,int& counter2,int& counter1,int& counter0,int& hit_num0,int& miss_num0)
{
int bit_num=2;   //each entry is 2-bit (decimal:0,1,2,3) for chooser table

    if(a=='t' || a=='n'){// in case there're blank rows in trace file

    //predict
    if(counter0>=pow(2,bit_num)/2)      //for 2-bit, if counter>=2, predict=pred1 of gshare
    {
        pred0=pred1;
      //update counter of gshare
        if(a=='t') {
           if(counter1==7)
            {counter1=counter1+0;}
           else{counter1+=1;}
        }
        else{//a==n, n-not taken
           if(counter1==0)
            {counter1=counter1-0;}
           else{counter1-=1;}
        }
    }
    else{                             // if counter<2, predict=pred2 of bimodal
        pred0=pred2;
            //update counter of bimodal
        if(a=='t') {
            if(counter2==7)
            {counter2=counter2+0;}
            else{counter2+=1;}
        }
        else{//a==n, n-not taken
            if(counter2==0)
            {counter2=counter2-0;}
            else{counter2-=1;}
         }
    }

    //determine if prediction=actual outcome
    if(pred0==a)  {hit_num0++;}
    else{miss_num0++;}

    //update counter of chooser table
    if(((pred2==a)&&(pred1==a))||((pred2!=a)&&(pred1!=a))) {
        counter0=counter0+0;               // if both correct or incorrect, then no change
    }
    else{
        if((pred1==a) && (pred2!=a))     //if gshare correct and bimodal incorrect, increment
        {
           if(counter0==3) //if staturate at 3, then no change
            {counter0=counter0+0;}
           else{counter0+=1;}             //else, increment 1
        }
        if((pred1!=a) && (pred2==a))   //if bimodal correct, gshare incorrect, decrement
        {
          if(counter0==0)                 //if staturate at 0, then no change
            {counter0=counter0-0;}
          else{counter0-=1;}              //else, decrement 1
        }
    }
    }
    else{ std::cout<<"invalid input";}
}

int main(int argc, char* argv[])
{
    const int READ_BUF_LEN = 24;
    char read_buf[READ_BUF_LEN] = { 0 };
    char actual_outcome ;                                 //taken (t) or not taken (n)
    int  op_addr = 0,op_times=0;                          //trace address, operation times
    string predictor=argv[1];                             //get the name of predictor

    //smith B-bit predictor
    if(predictor=="smith")
    {
     int B=atoi(argv[2]);
     string trace_file=argv[3];                            //"/home/laker/Desktop/MachineProblem2Fall2020_Updated/traces/gcc_trace.txt";
     int counter=pow(2,B)/2;                               //smith only has an entry of counter,2^B/2 as initial value
     int hit_num=0,miss_num=0;                             //number of correct prediction and wrong prediction in smith

    FILE* _fille = fopen(trace_file.c_str(), "r");
    if (_fille == nullptr) {
        printf("Can not open the trace file %s!\n", trace_file.c_str());
        return -1;
    }
    while (fgets(read_buf, READ_BUF_LEN, _fille))
    {
        sscanf(read_buf, "%x  %c\n", &op_addr,&actual_outcome);
        op_times += 1;
        smith_predictor(actual_outcome,B,counter,hit_num,miss_num);

    }
    fclose(_fille);
        cout<<"COMMAND"<<"\n";
        cout<<"./sim ";
        for(int j=1;j<argc;j++)
        {cout<<argv[j]<<" ";}
        cout<<"\n";
        cout<<"OUTPUT"<<"\n";
        cout<<"number of predictions: "<<(hit_num+miss_num)<<"\n";
        cout<<"number of mispredictions: "<<miss_num<<"\n";
        printf("misprediction rate: %.2f",(1.0*miss_num/(hit_num+miss_num))*100);
        cout<<"%"<<"\n";
        cout<<"FINAL COUNTER CONTENT: "<<counter<<"\n";

    }

    //bimodal m2 predictor
    if(predictor=="bimodal")
    {
    int m2=atoi(argv[2]);
    string trace_file2=argv[3];                            //"/home/laker/Desktop/MachineProblem2Fall2020_Updated/traces/gcc_trace.txt";
    int entry_num2=pow(2,m2),initial_value2=4;   //2^m1 entries, each entry initialized as 4 in bimodal predictor
    int counter_set2[entry_num2],counter2=0;
    std::fill_n(counter_set2,entry_num2,initial_value2);  //initialize counter array table:each entry is the proof of prediction
    int index2=0;                                         //bimodal index by m2-bit
    char pred2=0;                                         //bimodal prediction
    int hit_num2=0,miss_num2=0;                           //number of correct predictions and wrong predictions in bimodal

    FILE* _fille2 = fopen(trace_file2.c_str(), "r");
    if (_fille2 == nullptr) {
        printf("Can not open the trace file %s!\n", trace_file2.c_str());
        return -1;
    }

    while (fgets(read_buf, READ_BUF_LEN, _fille2))
    {
        sscanf(read_buf, "%x  %c\n", &op_addr,&actual_outcome);
        op_times += 1;
        index2=((op_addr>>2)&((1<<m2)-1)) ;             //extract lowest m2 bits from trace address
        counter2=counter_set2[index2];
        bimodal_predictor(actual_outcome,pred2,counter2,hit_num2,miss_num2);
        counter_set2[index2]=counter2;                   //update counter entry after predicting taken or not taken
    }
    fclose(_fille2);
        cout<<"COMMAND"<<"\n";
        cout<<"./sim ";
        for(int j=1;j<argc;j++)
        {cout<<argv[j]<<" ";}
        cout<<"\n";
        cout<<"OUTPUT"<<"\n";
        cout<<"number of predictions: "<<(hit_num2+miss_num2)<<"\n";
        cout<<"number of mispredictions: "<<miss_num2<<"\n";
        printf("misprediction rate: %.2f",(1.0*miss_num2/(hit_num2+miss_num2))*100);
        cout<<"%"<<"\n";
        cout<<"FINAL BIMODAL CONTENTS"<<"\n";
        for(int ii=0;ii<entry_num2;ii++)
        {
            cout<<ii<<"\t"<<counter_set2[ii]<<"\n";
        }

    }

    //gshare m1,n predictor
    if(predictor=="gshare")
    {
        int m1=atoi(argv[2]);
        int n=atoi(argv[3]);
        string trace_file1=argv[4];
        int entry_num1=pow(2,m1),initial_value1=4;            //2^m1 entries, each entry initialized as 4 in gshare predictor
        int counter_set1[entry_num1],counter1=0;
        std::fill_n(counter_set1,entry_num1,initial_value1);  //initialize counter array table:each entry is the proof of prediction
        int index1=0, idx1=0,idx2=0,temp=0;                   //gshare index by m1-bit,read lowest m1-bit, (m1-1)-bit from trace address
        char pred1=0;                                         //gshare prediction
        int hit_num1=0,miss_num1=0;                           //number of correct predictions and wrong predictions in gshare
        int  global_reg=0;                                    //n-bit single global history register

    FILE* _fille1 = fopen(trace_file1.c_str(), "r");
    if (_fille1 == nullptr) {
        printf("Can not open the trace file %s!\n", trace_file1.c_str());
        return -1;
    }

    while (fgets(read_buf, READ_BUF_LEN, _fille1))
    {
        sscanf(read_buf, "%x  %c\n", &op_addr,&actual_outcome);
        op_times += 1;

        //get index
        idx1=(op_addr>>2)&((1<<m1)-1);                   //get lowest m2-bit
        idx2=(op_addr>>2)&((1<<n)-1);                 //get lowest n-bit, where n<=m2

        idx2=idx2^global_reg;                            //XOR for lowest n-bit with global history table
        idx1=(idx1>>n)<<n;                         //get the lowest (m2-n) bit
        index1=idx1|idx2;                                //use XOR result to replace idx1's lowest n-bit, get m2-bit index

        counter1=counter_set1[index1];                   //counter stores the entry used to predict taken (t) or not taken (n)
        gshare_predictor(actual_outcome,pred1,counter1,hit_num1,miss_num1);

        counter_set1[index1]=counter1;                  //update counter entry after predicting taken or not taken
        if(actual_outcome=='t')                         //update global history reg
            {   temp=pow(2,n-1);
                global_reg=temp|(global_reg>>1);}       //if predict taken (t), right shift 1-bit and place 1 into its MSB
        else{global_reg=(global_reg>>1);}               //if not taken (n), right shift 1-bit and place 0 into its MSB

    }
    fclose(_fille1);
        cout<<"COMMAND"<<"\n";
        cout<<"./sim ";
        for(int j=1;j<argc;j++)
        {cout<<argv[j]<<" ";}
        cout<<"\n";
        cout<<"OUTPUT"<<"\n";
        cout<<"number of predictions: "<<(hit_num1+miss_num1)<<"\n";
        cout<<"number of mispredictions: "<<miss_num1<<"\n";
        printf("misprediction rate: %.2f",(1.0*miss_num1/(hit_num1+miss_num1))*100);
        cout<<"%"<<"\n";
        cout<<"FINAL GSHARE CONTENTS"<<"\n";
        for(int i2=0;i2<entry_num1;i2++)
        {
            cout<<i2<<"\t"<<counter_set1[i2]<<"\n";
        }

    }

    //hybird predictor: k, m1, n, m2
    if(predictor=="hybrid")
    {

    //int  B=4,k=8,m1=14,n=10,m2=5;                         //B-bit smith, m2-bit for bimodal, (m1,n) for gshare, k for chooser table in hybird

    int k=atoi(argv[2]);
    int m11=atoi(argv[3]);
    int nn=atoi(argv[4]);
    int m22=atoi(argv[5]);
    string trace_file0=argv[6];

    int  global_reg0=0;                                    //n-bit single global history register

    int entry_num22=pow(2,m22),initial_value22=4;            //2^m1 entries, each entry initialized as 4 in bimodal predictor
    int counter_set22[entry_num22],counter22=0,counter222=0;
    std::fill_n(counter_set22,entry_num22,initial_value22);  //initialize counter array table:each entry is the proof of prediction

    int entry_num11=pow(2,m11),initial_value11=4;            //2^m1 entries, each entry initialized as 4 in gshare predictor
    int counter_set11[entry_num11],counter11=0,counter111=0;
    std::fill_n(counter_set11,entry_num11,initial_value11);  //initialize counter array table:each entry is the proof of prediction

    int entry_num0=pow(2,k),initial_value0=1;             //2^k entries, each entry initialized as 1 in chooser table
    int counter_set0[entry_num0],counter0=0;
    std::fill_n(counter_set0,entry_num0,initial_value0);  //initialize counter array table:each entry is the proof of prediction

    int index22=0;                                         //bimodal index by m2-bit
    char pred22=0;                                         //bimodal prediction
    int hit_num22=0,miss_num22=0;                           //number of correct predictions and wrong predictions in bimodal

    int index11=0, idx11=0,idx22=0,temp0=0;                   //gshare index by m1-bit,read lowest m1-bit, (m1-1)-bit from trace address
    char pred11=0;                                         //gshare prediction
    int hit_num11=0,miss_num11=0;                           //number of correct predictions and wrong predictions in gshare

    int index0=0;                                         //chooser table index by k-bit
    char pred0=0;                                         //chooser prediction
    int hit_num0=0,miss_num0=0;                           // number of correct predictions and wrong predictions in hybird


    FILE* _fille0 = fopen(trace_file0.c_str(), "r");
    if (_fille0 == nullptr) {
        printf("Can not open the trace file %s!\n", trace_file0.c_str());
        return -1;
    }

    while (fgets(read_buf, READ_BUF_LEN, _fille0))
    {
        sscanf(read_buf, "%x  %c\n", &op_addr,&actual_outcome);
        op_times += 1;

        //get index for bimodal
        index22=((op_addr>>2)&((1<<m22)-1)) ;                //extract lowest m2 bits from trace address
        counter22=counter_set22[index22];
        counter222=counter22;
        bimodal_predictor(actual_outcome,pred22,counter222,hit_num22,miss_num22);

        //get index for gshare
        idx11=(op_addr>>2)&((1<<m11)-1);                     //get lowest m1-bit
        idx22=(op_addr>>2)&((1<<nn)-1);                   //get lowest n-bit, where n<=m1

        idx22=idx22^global_reg0;                             //XOR for lowest n-bit with global history table
        idx11=(idx11>>nn)<<nn;                          //get the lowest (m1-n) bit
        index11=idx11|idx22;                                  //use XOR result to replace idx1's lowest n-bit, get m1-bit index

        counter11=counter_set11[index11];                     //counter stores the entry used to predict taken (t) or not taken (n)
        counter111=counter11;
        gshare_predictor(actual_outcome,pred11,counter111,hit_num11,miss_num11);

        //get index for chooser table
        index0=((op_addr>>2)&((1<<k)-1)) ;                //extract lowest k bits from trace address
        counter0=counter_set0[index0];
        hybird_predictor(actual_outcome,pred22,pred11,pred0,counter22, counter11,counter0,hit_num0,miss_num0);
                                                          //return pred0=pred2 or pred1 to update bimodal or gshare
        counter_set0[index0]=counter0;                    //update counter entry of chooser table

        counter_set22[index22]=counter22;                  //update counter entry of bimodal
        counter_set11[index11]=counter11;                  //update counter entry of gshare

        if(actual_outcome=='t')                           //update global history reg
            {   temp0=pow(2,nn-1);
                global_reg0=temp0|(global_reg0>>1);}         //if predict taken (t), right shift 1-bit and place 1 into its MSB
        else{global_reg0=(global_reg0>>1);}                 //if not taken (n), right shift 1-bit and place 0 into its MSB


    }
    fclose(_fille0);
        cout<<"COMMAND"<<"\n";
        cout<<"./sim ";
        for(int j=1;j<argc;j++)
        {cout<<argv[j]<<" ";}
        cout<<"\n";
        cout<<"OUTPUT"<<"\n";
        cout<<"number of predictions: "<<(hit_num0+miss_num0)<<"\n";
        cout<<"number of mispredictions: "<<miss_num0<<"\n";
        printf("misprediction rate: %.2f",(1.0*miss_num0/(hit_num0+miss_num0))*100);
        cout<<"%"<<"\n";

        cout<<"FINAL CHOOSER CONTENTS"<<"\n";
        for(int i=0;i<entry_num0;i++)
        //for(int i=0;i<100;i++)
        {
            cout<<i<<"\t"<<counter_set0[i]<<"\n";
        }

        cout<<"FINAL GSHARE CONTENTS"<<"\n";
        for(int h=0;h<entry_num11;h++)
        //for(int i=0;i<100;i++)
        {
            cout<<h<<"\t"<<counter_set11[h]<<"\n";
        }

        cout<<"FINAL BIMODAL CONTENTS"<<"\n";
        for(int h1=0;h1<entry_num22;h1++)
        //for(int i=0;i<100;i++)
        {
            cout<<h1<<"\t"<<counter_set22[h1]<<"\n";
        }

    }





    return 0;
}
