#include "hmm.h"

double max(double da[]){       //define function max for future use
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<5;j++)
        {
            if (da[j]<da[j+1])
            {
                da[j]=da[j]+da[j+1];
                da[j+1]=da[j]-da[j+1];
                da[j]=da[j]-da[j+1];
            }
        }
    }
    return da[0];
}

int main(int argc, char** argv){               //load model_01~05
	HMM hmms[5];
	load_models( argv[1], hmms, 5);

	FILE *fp = open_or_die(argv[2], "r");      //read data
	char observ[MAX_SEQ];
    int cnt=0;
    int record[10*MAX_LINE];
    double prob[10*MAX_LINE];
    while( fgets(observ,MAX_SEQ,fp) !=NULL)
    {
    	int s=strlen(observ)-1;
    	double p[6];
    	double comp[5];
    	p[5]=0.0;      //in order to match size 6 and use function max
    	for(int k=0;k<5;k++)       //calculate delta
    	{
    		double delta[MAX_SEQ][6]={0.0};
    		for(int i=0;i<6;i++)
            {
    			delta[0][i]=hmms[k].initial[i]*hmms[k].observation[observ[0]-'A'][i];
            }
    		for(int t=1;t<s;t++)
    		{
    			for(int j=0;j<6;j++)
    			{
    				double tmp[6];     //store delta * a
    				for(int i=0;i<6;i++)
    				{
    					tmp[i]=delta[t-1][i]*hmms[k].transition[i][j];
    				}
    				delta[t][j]=max(tmp)*hmms[k].observation[observ[t]-'A'][j];
    			}
    		}
    		double tmp2[6];
    		for(int i=0;i<6;i++)
            {
    			tmp2[i]=delta[s-1][i];
            }

    		p[k]=max(tmp2);       //record probability of each model
    		comp[k]=max(tmp2);
        }
    	for(int k=0;k<5;k++)
    	{
    		if(comp[k]==max(p))
            {
    			//big_num=k;
                record[cnt]=k;       //record probabilty and number of the most probable model
                prob[cnt]=max(p);
                break;
            }
    	}
        cnt++;
    }

    FILE *fp2=open_or_die(argv[3], "w");       //output result
    for(int i=0;i<cnt;i++)
    {
        fprintf( fp2, "model_0%d.txt", record[i]+1);
        fprintf( fp2, " %e\n", prob[i]);
    }
        

    if(strcmp(argv[2],"testing_data1.txt")==0)        //if reading testing_data1, output acc.txt
    {
    	FILE *fp3 = open_or_die(argv[3], "r");
    	FILE *fp4 = open_or_die("testing_answer.txt", "r");
    	char cmp1[MAX_SEQ];
    	char cmp2[MAX_SEQ];
    	int count=0;   //how many are the same
    	double acc;
    	while( (fgets(cmp1,MAX_SEQ,fp3) !=NULL) && (fgets(cmp2,MAX_SEQ,fp4) !=NULL))
    	{
    		if(cmp1[7]==cmp2[7])
    			count++;
    	}
    	acc=(double)count/2500;
    	FILE *fp5=open_or_die("acc.txt", "a");
    	fprintf( fp5, "%lf\n", acc);
    }
}