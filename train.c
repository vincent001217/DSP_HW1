#include "hmm.h"

int main(int argc, char** argv){         //load initial model
	int iteration = atoi(argv[1]);

	HMM model;
	loadHMM(&model,argv[2]);

while(iteration--)
{	
	double sumgamma1[6]={0.0};
    double sumgamma2[6]={0.0};
	double initsumgamma[6]={0.0};
	double recgamma[6][6]={0.0};
	double sumepsilon[6][6]={0.0};
	FILE *fp = open_or_die(argv[3], "r");      //read data
	char observ[MAX_SEQ];
    while( fgets(observ,MAX_SEQ,fp) !=NULL)
    {
    	int s=strlen(observ)-1;          
    	double f[MAX_SEQ][6]={0.0};     //define forward variable
    	for(int i=0;i<6;i++)
        {
    		f[0][i]=model.initial[i]*model.observation[observ[0]-'A'][i];
        }
        for(int t=0;t<(s-1);t++)
        {
    	    for(int j=0;j<6;j++)
    	    {
    		    for(int i=0;i<6;i++)
    		    {
    			    f[t+1][j]=f[t+1][j]+f[t][i]*model.transition[i][j];
    		    }
    		    f[t+1][j]=f[t+1][j]*model.observation[observ[t+1]-'A'][j];
    	    }
        }

        double b[MAX_SEQ][6]={0.0};    //define backward variable
        for(int i=0;i<6;i++)
        {
    	    b[s-1][i]=1;
        }
        for(int t=s-2;t>=0;t--)  
        {
    	    for(int i=0;i<6;i++)
    	    {
    		    for(int j=0;j<6;j++)
    		    {
    			    b[t][i]=b[t][i]+model.transition[i][j]*model.observation[observ[t+1]-'A'][j]*b[t+1][j];
    		    }
    	    }
        }

        double gamma[MAX_SEQ][6]={0.0};    //calculate gamma
        for(int t=0;t<s;t++)
        {
        	double fb_total=0.0;
        	for(int i=0;i<6;i++)
            {
        		fb_total=fb_total+f[t][i]*b[t][i];
            }
        	for(int i=0;i<6;i++)
        	{
        		gamma[t][i]=(f[t][i]*b[t][i])/fb_total;
        	}
        }

        double epsilon[2*MAX_SEQ][6]={0.0};    //calculate epsilon
        for(int t=0;t<(s-1);t++)
        {
        	double fb_total=0.0;
        	for(int i=0;i<6;i++)
        	{
        		for(int j=0;j<6;j++)
        		{
        			fb_total=fb_total+f[t][i]*model.transition[i][j]*model.observation[observ[t+1]-'A'][j]*b[t+1][j];
        		}
        	}

        	for(int i=0;i<6;i++)
        	{
        		for(int j=0;j<6;j++)
        		{
        			epsilon[(6*t)+i][j]=(f[t][i]*model.transition[i][j]*model.observation[observ[t+1]-'A'][j]*b[t+1][j])/fb_total;
        		}
        	}
        }
        
        for(int i=0;i<6;i++)          //accumulate gamma
        {
        	for(int t=0;t<(s-1);t++)
            {
        		sumgamma1[i]=sumgamma1[i]+gamma[t][i];
            }
            sumgamma2[i]=sumgamma1[i]+gamma[s-1][i];
        }

        for(int i=0;i<6;i++)         
        {
        	initsumgamma[i]=initsumgamma[i]+gamma[0][i];
        }

        for(int t=0;t<s;t++)
        {
        	for(int i=0;i<6;i++)
        	{
        		if(observ[t]-'A'==0)
        			recgamma[0][i]=recgamma[0][i]+gamma[t][i];
        		else if(observ[t]-'A'==1)
        			recgamma[1][i]=recgamma[1][i]+gamma[t][i];
        		else if(observ[t]-'A'==2)
        			recgamma[2][i]=recgamma[2][i]+gamma[t][i];
        		else if(observ[t]-'A'==3)
        			recgamma[3][i]=recgamma[3][i]+gamma[t][i];
        		else if(observ[t]-'A'==4)
        			recgamma[4][i]=recgamma[4][i]+gamma[t][i];
        		else if(observ[t]-'A'==5)
        			recgamma[5][i]=recgamma[5][i]+gamma[t][i];
        	}
        }

        for(int i=0;i<6;i++)                 //accumulate epsilon
        {
        	for(int j=0;j<6;j++)
        	{
        		for(int t=0;t<(s-1);t++)
                {
        			sumepsilon[i][j]=sumepsilon[i][j]+epsilon[(6*t)+i][j];
                }
        	}
        }
    }
    
    for(int i=0;i<6;i++)                  //adjust initial probabilities
    {
        model.initial[i]=initsumgamma[i]/10000;
    }

    for(int i=0;i<6;i++)                  //adjust transition probabilities
    {
    	for(int j=0;j<6;j++)
        {
    	    model.transition[i][j]=sumepsilon[i][j]/sumgamma1[i];
        }
    }

    for(int k=0;k<6;k++)                  //adjust observation probabilities
    {
    	for(int i=0;i<6;i++)
        {
    		model.observation[k][i]=recgamma[k][i]/sumgamma2[i];
        }
    }
}

FILE *fp2=open_or_die(argv[4], "w");      //output models
dumpHMM(fp2,&model);
}
