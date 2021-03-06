#include "headerfile.h"


void initialize_s(struct student s[],int size)                    // initilize data id=year=hostel_no=floor_no=room_no=id_roomate=-1, name=dept='\0'
{
	int i, j;
	for(i=0;i<size;i++)
	{
		s[i].id=s[i].year=s[i].hostel_no=s[i].floor_no=s[i].room_no=s[i].roommate_id=s[i].p.floor_pref=s[i].p.roommate_pref=-1;
		s[i].name[0]='\0';
		s[i].dept[0]='\0';
		for(j=0;j<N;j++)
		{
			s[i].p.list[j]=-1;
		}
	}	
}

void initialize_r(struct room r[],int size,int floor_no)                    // initialize everything to -1 except room nos.,that will be assigned as per no.
{									    									// of floors available per floor	
	int i;
	for(i=0;i<size;i++)
	{
		r[i + (floor_no-1)*size].vacancy=-1;
		r[i + (floor_no-1)*size].id1=-1;
		r[i + (floor_no-1)*size].id2=-1;
		r[i + (floor_no-1)*size].room_no=(1000*floor_no)+i+1;
	}	
}

void display(struct student old_stud[],int size)
{
	int i;
	printf("id\tname\tyear\tdept\thostel_no\tfloor_no\troom_no\troommate_id\tfloor_pref\troommate_pref");
	for(i=0;i<size;i++)
	printf("%d\t%s\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n",old_stud[i].id,old_stud[i].name,old_stud[i].year,old_stud[i].dept,old_stud[i].hostel_no,old_stud[i].floor_no,old_stud[i].room_no,old_stud[i].roommate_id,old_stud[i].p.floor_pref,old_stud[i].p.roommate_pref);
}


int assign_all(struct student stud[], int size)			//assignment function for part B
{
	FILE *f;
	struct student st;																
	f=fopen("student.dat","rb");	
	int i, j, count=0;
	i=0;														
	while(fread(&st, sizeof(struct student), 1, f)) 
	{
		st.floor_no=st.room_no=st.roommate_id=-1;
		if(st.year!=1)
		{
			stud[i].floor_no=stud[i].room_no=stud[i].roommate_id=-1;
			for(j=0;j<30;j++)
			{
				stud[i].p.list[j]=-1;
			}
			strcpy(stud[i].name, st.name);
			stud[i].id=st.id;
			stud[i].year=st.year;
			strcpy(stud[i].dept, st.dept);
			stud[i].p.floor_pref=st.p.floor_pref;
			stud[i].p.roommate_pref=st.p.roommate_pref;
			stud[i].hostel_no=st.hostel_no;
			i++;
		}
		else
		{
			count++;		//stores count of first years
		}
	}
	fclose(f);
	return count;
}

int assign(struct student old_stud[],struct student new_stud[],int size, int *sh_c, int *ns_c, int *os_c, int *dh_c) 	//assigns the applications to four
{																														//different arrays according to their
	FILE *f; 							    																			//decreasing preferences of processing
	struct student st,same_hostel[size],out_stud[size],diff_hostel[size];
	initialize_s(same_hostel,size);
	initialize_s(out_stud,size);
	initialize_s(diff_hostel,size);
	int i,j,sh_count,ns_count,os_count,dh_count;
	sh_count=ns_count=os_count=dh_count=0; 																		
	f=fopen("student.dat","rb"); 															
	while(fread(&st, sizeof(struct student), 1, f))
	{
		st.floor_no=st.room_no=st.roommate_id=-1;
		if(st.hostel_no == 1)
		{
			same_hostel[sh_count]=st;
			sh_count++;
			(*sh_c)++;
		}
		else if(st.hostel_no == -1)
		{
			if(st.year == 1)
			{
				new_stud[ns_count]=st;
				ns_count++;
				(*ns_c)++;
			}
			else
			{
				out_stud[os_count]=st;
				os_count++;
				(*os_c)++;
			}	
		}
		else 
		{
			diff_hostel[dh_count]=st;
			dh_count++;
			(*dh_c)++;
		}
	}
	j=0;
	fclose(f);
	for(i=0;i<sh_count;i++) 					//from here,the array old_stud[] is filled with details in preference order
	{
		old_stud[j]=same_hostel[i];         
		j++;
	}
	for(i=0;i<os_count;i++)
	{
		old_stud[j]=out_stud[i];
		j++;
	}
	for(i=0;i<size;i++)
	{
		old_stud[j]=diff_hostel[i];
		j++;
	}
	return (dh_count+os_count+sh_count);  									//the size of old_stud[] is returned
}

int new_stud_allocate(struct student new_stud[], struct room room[], int new_stud_size, int room_size)
{
	int ret_val, i, j, floor_room;
	floor_room=(room_size/4)-2;		// number of rooms on first floor, taking care of guest rooms
	i=j=0;
	ret_val=0;
	while((i<floor_room)&&(j<new_stud_size))
	{
		if(j+1 < new_stud_size)      //assigning rooms
		{
			new_stud[j].room_no=new_stud[j+1].room_no=room[i].room_no;		
			room[i].id1=new_stud[j+1].roommate_id=new_stud[j].id;
			room[i].id2=new_stud[j].roommate_id=new_stud[j+1].id;
			new_stud[j].hostel_no=new_stud[j+1].hostel_no=new_stud[j].floor_no=new_stud[j+1].floor_no=1;
			room[i].vacancy=1;
			i++;
			j=j+2;
		}	
		else
		{
			new_stud[j].room_no=room[i].room_no;
			room[i].id1=new_stud[j].id;
			new_stud[j].hostel_no=new_stud[j].floor_no=1;
			i++;
			j++;
		}
	}
	if(j<new_stud_size)		//if there are applicants left but rooms filled
	{
		ret_val=0;
	}
	else if(i<floor_room)		//if applicants are done before all rooms filled
	{
		if(new_stud_size%2 == 0)
			ret_val = 2*(floor_room-i);
		else
			ret_val = 2*(floor_room-i) + 1;
	}
	return ret_val;		//holds number of vacant seats on first floor
}

int find_index(struct student old_stud[], int size, int id)  //returns index where id is found, if not found returns -1
{
	int i,pos=-1;
	for(i=0;i<size && pos==-1;i++)
	{
		if(id == old_stud[i].id)
		{
			pos = i;
		}
	}
	return pos;
}

void old_stud_allocate(struct student old_stud[], struct room room[], int old_stud_size, int room_size, int *r2, int *r3, int *r4)
{
	int x, y, z, i, j;
	i=0;
	while(i<old_stud_size)		//for assigning roommates based on roommate preference
	{
		j=find_index(old_stud, old_stud_size, old_stud[i].p.roommate_pref);
		if(old_stud[i].roommate_id == -1)          //if student has no room partner				
		{
			if(old_stud[j].roommate_id == -1)       // if both students dont have a roompartner
			{
				old_stud[i].roommate_id=old_stud[j].id;
				old_stud[j].roommate_id=old_stud[i].id;
			}
			else if(old_stud[j].p.roommate_pref == old_stud[i].id)    //if j has a roompartner but he prefers i 
			{
				x=find_index(old_stud,old_stud_size,old_stud[j].roommate_id);
				old_stud[x].roommate_id=-1;
				old_stud[i].roommate_id=old_stud[j].id;
				old_stud[j].roommate_id=old_stud[i].id;
			}
		}
		i++;
	}
	i=0;
	while(i<old_stud_size)		//for assigning roommates based on floor preference
	{
		if(old_stud[i].roommate_id == -1)
		{
			j=i+1;
			x=0;
			while(j<old_stud_size && x==0)
			{
				if(old_stud[i].p.floor_pref==old_stud[j].p.floor_pref && old_stud[j].roommate_id==-1)
				{												//if both prefer same floor
					old_stud[i].roommate_id=old_stud[j].id;		//assigning them as roommates
					old_stud[j].roommate_id=old_stud[i].id;
					x=1;
				}
				j++;
			}
		}
		i++;
	}
	(*r2)=(*r3)=(*r4)=0;		//for storing index of next room on that floor to be occupied
	i=0;
	while(i<old_stud_size)		//for assigning rooms to roommate pairs
	{
		if(old_stud[i].room_no==-1 && old_stud[i].roommate_id != -1)
		{
			j=find_index(old_stud, old_stud_size, old_stud[i].roommate_id);
			x=old_stud[i].p.floor_pref;		                //checking for floor preference      
			
			if(x==2)
			{
				y = *r2;
				(*r2)++;
			}
			else if(x==3)         //y stores index of room in room[]
			{
				y = *r3;
				(*r3)++;
			}
			else if(x==4)
			{
				y = *r4;
				(*r4)++;
			}
			if((x==2 && (*r2)<=(room_size/4)) || (x==3 && (*r3)<=(room_size/4)) || (x==4 && (*r4)<=(room_size/4)) ) //checks whether rooms are available or not
			{
				z=((x-1)*(room_size)/4)+y;		
				room[z].id1=old_stud[i].id;		//updating room and student details
				room[z].id2=old_stud[j].id;
				room[z].vacancy=1;
				old_stud[i].hostel_no=old_stud[j].hostel_no=1;
				old_stud[i].floor_no=old_stud[j].floor_no=x;
				old_stud[i].room_no=old_stud[j].room_no=room[z].room_no;
			}
		}
		i++;
	}
	if((*r2)<=(room_size/4))
		(*r2)=2*((room_size/4)-(*r2));
	else										//now *r2, *r3 and *r4 will store number of vacant seats in their respective floors
		*r2 = 0;
	if((*r3)<=(room_size/4))
		(*r3)=2*((room_size/4)-(*r3));
	else
		*r3 = 0;
	if((*r4)<=(room_size/4))
		(*r4)=2*((room_size/4)-(*r4));
	else
		*r4 = 0;
}

void pref_lists(struct student stud[], int stud_size)
{												//for creating preference lists
	int i, j, x;
	i=0;
	while(i<stud_size)
	{
		x=0;						//index for preference list
		j=0;
		while(j<stud_size)
		{
			if(i!=j)
			{
				if(strcmp(stud[i].dept, stud[j].dept)==0 && stud[i].year==stud[j].year)
				{											//same dept same year
					stud[i].p.list[x]=stud[j].id;		//adding to preference list
					x++;
				}
			}
			j++;
		}	
		j=0;
		while(j<stud_size)
		{
			if(i!=j)
			{
				if(strcmp(stud[i].dept, stud[j].dept)!=0 && stud[i].year==stud[j].year)
				{											//same year different dept
					stud[i].p.list[x]=stud[j].id;
					x++;
				}
			}
			j++;
		}
		j=0;
		while(j<stud_size)
		{
			if(i!=j)
			{
				if(stud[i].year>stud[j].year)
				{											//junior, any dept
					stud[i].p.list[x]=stud[j].id;
					x++;
				}
			}
			j++;
		}
		j=0;
		while(j<stud_size)
		{
			if(i!=j)
			{
				if(strcmp(stud[i].dept, stud[j].dept)==0 && stud[i].year<stud[j].year)
				{											//senior, same dept
					stud[i].p.list[x]=stud[j].id;
					x++;
				}
			}
			j++;
		}
		j=0;
		while(j<stud_size)
		{
			if(i!=j)
			{				
				if(strcmp(stud[i].dept, stud[j].dept)!=0 && stud[i].year<stud[j].year)
				{											//senior, different year
					stud[i].p.list[x]=stud[j].id;
					x++;
				}
			}
			j++;
		}
		for(j=stud_size;j<N;j++)
		{
			stud[i].p.list[j]=0;
		}
		i++;
	}
	return;
}
				  
int check_pref(struct student for_check, int existing_id, struct student interested)
{					
	/* to check whether interested features higher in for_check's preferred list than existing */		
	int i, ret_val;
	i=0;
	ret_val=0;		//existing is more preferred than interested
	while(for_check.p.list[i]!=existing_id && ret_val==0 && i<N)
	{
		if(for_check.p.list[i]==interested.id)
		{
			ret_val=1;		//interested is more preferred than existing
		}
		i++;
	}
	return ret_val;
}

void stable_combination(struct student stud[], int stud_size)
{
	int free_count, i, j, x, y, flag;
	free_count=stud_size;
	i=0;
	while(free_count>0)
	{
		if(stud[i].roommate_id==-1)
		{
			x=0;
			flag=0;								//x stores index of preferrence list				
			while(x<stud_size && flag==0)
			{
				j=find_index(stud, stud_size, stud[i].p.list[x]);		//index of wanted roommate
				if(stud[j].roommate_id==-1)
				{												//if j is free, both are assigned as roommates
					stud[i].roommate_id=stud[j].id;
					stud[j].roommate_id=stud[i].id;
					flag=1;
					free_count=free_count-2;
				}
				else if(check_pref(stud[j], stud[j].roommate_id, stud[i]))
				{													//if j prefers i more than current roommate
					y=find_index(stud, stud_size, stud[j].roommate_id);
					stud[y].roommate_id=-1;						//freeing current roommate
					stud[i].roommate_id=stud[j].id;			//assigning new roommates
					stud[j].roommate_id=stud[i].id;
					flag=1;
					free_count--;
				}
				else
				{
					x++;
				}
			}
		}
		i++;
		if(i==stud_size)
		{
			i=0;
		}
	}
	return;
}

void assign_rooms(struct student stud[], struct room room[], int stud_size, int room_size)
{						//to assign rooms and display roommates
	int i, j, r, count;
	r=0;				//index for rooms
	count=0;
	for(i=0;i<stud_size;i++)
	{
		if(stud[i].room_no==-1)
		{
			j=find_index(stud, stud_size, stud[i].roommate_id);
			stud[i].room_no=stud[j].room_no=room[r].room_no;
			room[r].id1=stud[i].id;
			room[r].id2=stud[j].id;
			room[r].vacancy=1;
			r++;
			count++;
		}
	}
	for(i=0;i<count;i++)
	{
		printf("%d - %d\n", room[i].id1, room[i].id2);
	}
	return;
}
