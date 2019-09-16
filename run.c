
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>

typedef struct
{
	int c_custkey;    	   
	char c_mkgsegment[20]; 
}customer;				    

typedef struct
{
	int o_orderkey;    	 
	int o_custkey;    	 
	char o_orderdate[10]; 
}order;				

typedef struct
{
	int l_orderkey;
	double l_extendedprice;
	char l_shipdate[10];
}line;

typedef struct
{
	int l_orderkey;
	char o_orderdate[10];
	double l_extendedprice;
}select_result;

order * read_orders_txt()
{
	int i =0; 
	order * a=NULL;
	a = (order * )malloc(4000*sizeof(order));
	char b,c;
	long long d;
	FILE *fp;
	fp = fopen("/home/orders.txt","r");
	if(fp == NULL)
	{
		printf("error!");
		return NULL;
	}
	while(!feof(fp))
	{	
		//printf("%d!",i);
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,&a[i].o_orderdate);
		a[i].o_custkey=d%100;
		i++;
	}
	fclose(fp);
	return a;
}
customer * read_customer_txt()  
{
	FILE * fp;
	customer *a=NULL;
	a = (customer *)malloc(100*sizeof(customer));
	int i=0;
	char b;
	fp = fopen("/home/customer.txt","r");
	if(NULL==fp)
	{
		printf("error!");
		return NULL;
	}
	while(!feof(fp))
	{	
		//printf("%d  ",i);
		fscanf(fp,"%d%c%s",&a[i].c_custkey,&b,&a[i].c_mkgsegment);
		//printf("%d%c%s\n",a[i].c_custkey,b,a[i].c_mkgsegment);
		i++;
	}
	fclose(fp);
	return a;
}


line * read_lineitem_txt()
{
	FILE * fp;
	line * l=NULL;
	l = (line *)malloc(1000*sizeof(line));
	int i=0;
	char b,c;
	fp = fopen("/home/lineitem.txt","r");
	if(fp==NULL)
	{
		printf("error!");
		return NULL;
	}
	while(!feof(fp))
	{
		//printf("%d!",i);
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,&l[i].l_shipdate);
		//printf("%d,%lf,%s\n",l[i].l_orderkey,l[i].l_extendedprice,l[i].l_shipdate);
		i++;
	}
	fclose(fp);
	return l; 
}

select_result * select1(customer * cus,order * ord,line * item,char * order_date,char * ship_date,char * ship_type)
{
	int i,j,k,l=0,m=0;
	select_result * result1=NULL;
	select_result * result2=NULL;
	select_result  temp;
	result1 = (select_result *)malloc(1000*sizeof(select_result));
	result2 = (select_result *)malloc(1000*sizeof(select_result));
	//char ship[]="BUILDING";
	//printf("%s %s\n",order_date,ship_date);
	//printf("%s\n",ship_type);
	for(i=0;i<100;i++)
	{
		if(strcmp(cus[i].c_mkgsegment,ship_type)==0)
		{
		for(j=0;j<4000;j++)
		{
			for(k=0;k<1000;k++)
			if(cus[i].c_custkey==ord[j].o_custkey&&ord[j].o_orderkey==item[k].l_orderkey&&(strcmp(ord[j].o_orderdate,order_date)<0)&&(strcmp(item[k].l_shipdate,ship_date)>0))
			{
				result1[l].l_orderkey=item[k].l_orderkey;
				strcpy(result1[l].o_orderdate,ord[j].o_orderdate);
				result1[l].l_extendedprice=item[k].l_extendedprice;
				l++;
			}
		}
		}
		else continue;
	}
	/*printf("ÇóºÍ\n\n\n");*/
	for(i=0;i<l;i++)
	{
		//printf("%d\n",i);
		if(i==0)
		{
			result2[m].l_orderkey = result1[i].l_orderkey;
			strcpy(result2[m].o_orderdate,result1[i].o_orderdate);
			result2[m].l_extendedprice = result1[i].l_extendedprice;
			continue;
		}
		if(result1[i].l_orderkey==result1[i-1].l_orderkey)
		{
			result2[m].l_extendedprice = result2[m].l_extendedprice + result1[i].l_extendedprice;
			
		}
		else
		{
			
			m++;
			result2[m].l_orderkey = result1[i].l_orderkey;
			strcpy(result2[m].o_orderdate,result1[i].o_orderdate);
			result2[m].l_extendedprice = result1[i].l_extendedprice;
			
		}
	}
	for(i=0;i<m-1;i++)//Ã°ÅÝÅÅÐò 
	{
		for(j=0;j<m-1-i;j++)
		{
			if(result2[j].l_extendedprice<result2[j+1].l_extendedprice)
			{
				//printf("123");
				temp.l_extendedprice=result2[j].l_extendedprice;
				temp.l_orderkey=result2[j].l_orderkey;
				strcpy(temp.o_orderdate,result2[j].o_orderdate);
				result2[j].l_extendedprice=result2[j+1].l_extendedprice;
				result2[j].l_orderkey=result2[j+1].l_orderkey;
				strcpy(result2[j].o_orderdate,result2[j+1].o_orderdate);
				result2[j+1].l_extendedprice=temp.l_extendedprice;
				result2[j+1].l_orderkey=temp.l_orderkey;
				strcpy(result2[j+1].o_orderdate,temp.o_orderdate);
			}
		}
	}
	return result2;
}


int main(int argc,char **argv)
{
	int i;
	int a;
	int j;
	//printf("%s\n",argv[4]);
	unsigned int n=atoi(argv[4]);
	//printf("%d\n",n);
	int limit; 
	for(i=1;i<=n;i++)
	{
		unsigned int t=atoi(argv[4*i+4]);
		//printf("%d",t);
		customer* cus = NULL;
		order* ord = NULL;
		line* item = NULL;
		cus = read_customer_txt();
		ord = read_orders_txt();
		item = read_lineitem_txt();
		limit = t;
		select_result* result = NULL;
		result = select1(cus, ord, item, argv[4 * i + 2], argv[4 * i + 3], argv[4 * i + 1]);
		printf("l_orderkey|o_orderdate|revenue\n");
		for (j = 0; j < limit; j++)
		{
			printf("%-10d|%-11s|%-20.2lf\n", result[i].l_orderkey, result[i].o_orderdate, result[i].l_extendedprice);
		}
	}
	return 0;
}
