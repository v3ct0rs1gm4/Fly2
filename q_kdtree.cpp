/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "q_kdtree.h"


int rootindx;
qvector<sbbox> bbox;



qvector<vec3> trilist,norlist,txclist,collist;
vec3 wbmin,wbmax,dwbox;
vec3 *kdtree_data1=NULL;
half4 *kdtree_data1half=NULL;
vec3 *quadtree_data1=NULL;

vec3 *kdtree_data2=NULL;
vec3 *kdtree_data3=NULL;
int xy=0,hh=0;


vec3 tx_kdtreesize;
vec3 tx_kdtreemax;


void chk_bbox_mm(vec3 &v1,vec3 &bbmin,vec3 &bbmax)
{
	if(v1.x<bbmin.x) bbmin.x=v1.x;
	if(v1.y<bbmin.y) bbmin.y=v1.y;
	if(v1.z<bbmin.z) bbmin.z=v1.z;
	if(v1.x>bbmax.x) bbmax.x=v1.x;
	if(v1.y>bbmax.y) bbmax.y=v1.y;
	if(v1.z>bbmax.z) bbmax.z=v1.z;
}

int chk_bbox_mm2(vec3 &v1,vec3 &bbmin,vec3 &bbmax)//bbox extension
{
	int ok=0;
	if(f2int(v1.x)<f2int(bbmin.x)) {bbmin.x=v1.x;ok=1;}
	if(f2int(v1.y)<f2int(bbmin.y)) {bbmin.y=v1.y;ok=1;}
	if(f2int(v1.z)<f2int(bbmin.z)) {bbmin.z=v1.z;ok=1;}
	if(f2int(v1.x)>f2int(bbmax.x)) {bbmax.x=v1.x;ok=1;}
	if(f2int(v1.y)>f2int(bbmax.y)) {bbmax.y=v1.y;ok=1;}
	if(f2int(v1.z)>f2int(bbmax.z)) {bbmax.z=v1.z;ok=1;}
	
	return ok;//volt
}


void deftri(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &n1,vec3 &n2,vec3 &n3,vec3 col,int tx)
{
	trilist.push_back(v1);
	trilist.push_back(v2);
	trilist.push_back(v3);
	
	norlist.push_back(n1);
	norlist.push_back(n2);
	norlist.push_back(n3);

	collist.push_back(col);
}
void deftri_txc(vec3 &v1,vec3 &v2,vec3 &v3,vec3 &n1,vec3 &n2,vec3 &n3,vec3 &t1,vec3 &t2,vec3 &t3,vec3 col,int tx)
{
	trilist.push_back(v1);
	trilist.push_back(v2);
	trilist.push_back(v3);
	
	norlist.push_back(n1);
	norlist.push_back(n2);
	norlist.push_back(n3);

	txclist.push_back(t1);
	txclist.push_back(t2);
	txclist.push_back(t3);

	collist.push_back(col);
}
//----------------------------------------------------------------------
void fill_tx_quadtree()// csak shaderhez, ezert itt
{
#ifdef _VOXELBASED
	int p1=0,p2=0;
	printf("quadtree fill\n");


//	quadtree[x,y,z][int array] = i. bbox[i] vagy 1000000+j. cells[j][=empty space min max]
	loopd(z,0,vx_size,1)
	loopd(y,0,vx_size,1)
	loopd(x,0,vx_size,1) // vx_size=64
	{
//		int m=addr(x,y,z); // NA shaderben nem ez a sorrend
		int xc=x+((z%8)*vx_size); // 64x64 x8 x-be 
		int yc=y+((z/8)*vx_size); // x8 y-ba
//printf("%d %d \n",xc,yc);		 // 512 ok
		int m=addr(x,y,z);// quadtree original szerint
		int h=quadtree[m].size();
		p2=(yc)*512+xc; // a quadtree_data1 texture shader szerint
		
// p1/hh     p2/xh_quadtree
		int xc2=p1%hh;// %!
		int yc2=p1/hh;
		   // [xy=kdtree_data1 kezdete]    [z=array size]  
		quadtree_data1[p2]=vec3((float)xc2/(float)hh,(float)yc2/(float)hh,((float)h+0.01f));	p2++; // nem sorba   
		// quadtree_data1 pixelje mutat a kdtree_data1 egy szekvenciajara/sequence/array
		
		loopd(i,0,h,1)
		{
			int u=quadtree[m][i];
			if(u<999999) // tuti
			{
//			printf("%d %d \n",u,bbox[u].tri);  
				int u2=bbox[u].tri*3;// kell
				if(u2>=trilist.size())	printf("WARNING %d %d \n",u,trilist.size());
				kdtree_data1[p1]=vec3(1,0,0);	p1++; // triangle flag

				int p11=p1;
				kdtree_data1[p1]=trilist[u2];	p1++;
				kdtree_data1[p1]=trilist[u2+1];	p1++;
				kdtree_data1[p1]=trilist[u2+2];	p1++;

#ifdef _USENORMAL
				kdtree_data1[p1]=norlist[u2];	p1++;
				kdtree_data1[p1]=norlist[u2+1];	p1++;
				kdtree_data1[p1]=norlist[u2+2];	p1++;
/*				kdtree_data2[p11]=norlist[u2];//	p1++;
				kdtree_data2[p11+1]=norlist[u2+1];
				kdtree_data2[p11+2]=norlist[u2+2];*/
#endif		
#ifdef _USETXCOORD
				kdtree_data1[p1]=txclist[u2];	p1++;
				kdtree_data1[p1]=txclist[u2+1];	p1++;
				kdtree_data1[p1]=txclist[u2+2];	p1++;
/*				kdtree_data3[p11]=txclist[u2];
				kdtree_data3[p11+1]=txclist[u2+1];
				kdtree_data3[p11+2]=txclist[u2+2];*/
#endif	

#if 0
				sbbox tmp;
				chk_bbox_mm(trilist[u2],tmp.bmin,tmp.bmax);            =1  OK
				chk_bbox_mm(trilist[u2+1],tmp.bmin,tmp.bmax);
				chk_bbox_mm(trilist[u2+2],tmp.bmin,tmp.bmax);
				printf("%e %e %e %e %e %e\n",
					(tmp.bmin.x+1)/(bbox[u].bmin.x+1),
					(tmp.bmin.y+1)/(bbox[u].bmin.y+1),
					(tmp.bmin.z+1)/(bbox[u].bmin.z+1),
					tmp.bmax.x/bbox[u].bmax.x,
					tmp.bmax.y/bbox[u].bmax.y,
					tmp.bmax.z/bbox[u].bmax.z	);
#endif					
			}
			else
			{
				u-=1000000;
				if(u>=cells.size())		printf("WARNING %d %d \n",u,cells.size());
				kdtree_data1[p1]=vec3(0,0,0);	p1++; // cell flag
				kdtree_data1[p1]=cells[u];		p1++;  // bbox min
				kdtree_data1[p1]=cells[u+1];	p1++;  // bbox max
			}
		}
	}
	printf("quadtree: \n\tkdtree_data1  : %d [%d/%d] "
					 "\n\tquadtree_data1: %d [%d/%d]\n",p1,p1/hh,hh,   p2,p2/xh_quadtree,xh_quadtree);
#endif
}

//----------------------------------------------------------------------
int walktree(int leaf,int sp)
{
	int tri=0;
	sp++;
	
	
#ifdef _USEFOURTX	
//negy texture
	kdtree_data1[xy]=vec3(0.0,(float1)sp,0.0); 
	kdtree_data2[xy]=bbox[leaf].bmin;
	kdtree_data3[xy]=bbox[leaf].bmax;
	int p=xy;	 
	xy++;//jump
	
	if(bbox[leaf].left  !=-1) tri=walktree(bbox[leaf].left,sp);
	if(bbox[leaf].right !=-1) tri=walktree(bbox[leaf].right,sp);
	
	if(bbox[leaf].tri   !=-1) 
	{
		tri= bbox[leaf].tri;//valamelyik tree megy a boxba
		kdtree_data1[xy]=vec3(1.0,(float1)sp,1.0); 
		kdtree_data2[xy]=trilist[tri*3]; 
		kdtree_data3[xy]=trilist[tri*3+1];
		kdtree_data4[xy]=trilist[tri*3+2];  
		
/*		kdtree_data5[xy]=norlist[tri*3]; 
		kdtree_data6[xy]=norlist[tri*3+1];
		kdtree_data7[xy]=norlist[tri*3+2];  */
		xy++;
	}
//	bbox[leaf].tri2=tri;
	int x=xy%hh;
	int y=xy/hh;
	kdtree_data4[p]=vec3(x,y,0)/(float1)hh;//ide ugrik ha skip bbox



#else
//egy texture
	kdtree_data1[xy]=vec3(0.0,0.0,0.0); xy++;
	kdtree_data1[xy]=bbox[leaf].bmin; xy++;
	kdtree_data1[xy]=bbox[leaf].bmax; xy++;
	int p=xy;	 	xy++;//jump
	
	if(bbox[leaf].left  !=-1) tri=walktree(bbox[leaf].left,sp);
	if(bbox[leaf].right !=-1) tri=walktree(bbox[leaf].right,sp);
	
	if(bbox[leaf].tri   !=-1) 
	{
		tri= bbox[leaf].tri;//valamelyik tree megy a boxba
		kdtree_data1[xy]=vec3(1.0,1.0,1.0);  xy++;
		kdtree_data1[xy]=trilist[tri*3];  xy++;
		kdtree_data1[xy]=trilist[tri*3+1]; xy++;
		kdtree_data1[xy]=trilist[tri*3+2];   xy++;
#ifdef _USENORMAL
		kdtree_data1[xy]=norlist[tri*3];  xy++;
		kdtree_data1[xy]=norlist[tri*3+1]; xy++;
		kdtree_data1[xy]=norlist[tri*3+2];   xy++;
#endif		
#ifdef _USETXCOORD
		kdtree_data1[xy]=txclist[tri*3];  xy++;
		kdtree_data1[xy]=txclist[tri*3+1]; xy++;
		kdtree_data1[xy]=txclist[tri*3+2];   xy++;
#endif	
	}
//	bbox[leaf].tri2=tri;
	int x=xy%hh;
	int y=xy/hh;
#if 0	
	if (x >= (hh - SORXLIMIT)) // ne vagja kette a blockot
	{
		x = 0; y++;
		xy = y * hh + x;
	}
#endif
	kdtree_data1[p]=vec3(x,y,0)/(float1)hh;//ide ugrik ha skip bbox
#endif	
	return tri;
}

int build_kdtree(qvector<int> &indx)
{
	int n=indx.size();
	if(n<=0) return -1;
	if(n==1) return indx[0];
	
	vec3 bbmin=vec3(1e16,1e16,1e16);
	vec3 bbmax=vec3(-1e16,-1e16,-1e16);
	
	loopd(i,0,n,1)
	{
		int tr=indx[i];
//		if(tr<bbox.size())
		{
			chk_bbox_mm2(bbox[tr].bmin,bbmin,bbmax);
			chk_bbox_mm2(bbox[tr].bmax,bbmin,bbmax);
		}
	}

	vec3 dbox=bbmax-bbmin;
	int op=0;
	if(dbox.y>dbox.x)	if(dbox.y>dbox.z) op=1;
	if(dbox.z>dbox.x)	if(dbox.z>dbox.y) op=2;

	std::multimap<float1,int> reindx;
	loopd(i,0,n,1)
	{
		int tr=indx[i];

		float1 t1=0.0;
	//	if(tr<bbox.size())
		{
		if(op==0) t1=(bbox[tr].bmin.x+bbox[tr].bmax.x)/2.0;
		if(op==1) t1=(bbox[tr].bmin.y+bbox[tr].bmax.y)/2.0;
		if(op==2) t1=(bbox[tr].bmin.z+bbox[tr].bmax.z)/2.0;
		}
		std::pair<float1,int> pr(t1,tr);	
		reindx.insert(reindx.end(),pr);
	}

	qvector<int> indx1;
	qvector<int> indx2;
	int j=0;
	for(std::multimap<float1,int>::iterator it=reindx.begin();it!=reindx.end();++it)
	{//	printf("%f ",it->first);
		if(j<n/2) indx1.push_back(it->second);
		else      indx2.push_back(it->second);
		j++;
	}
//printf("\n");
		
	reindx.clear();
	int lbranch=build_kdtree(indx1);
	int rbranch=build_kdtree(indx2);
//printf(".. %d %d %d %d \n",n,j,indx1.size(),indx2.size());	

	sbbox newbox;
	newbox.bmin =bbmin;
	newbox.bmax =bbmax;
//	newbox.size=length(bbmax-bbmin);
	newbox.left =lbranch;//bbox[] index!
	newbox.right=rbranch;
	newbox.root=-1;
	newbox.tri  =-1;

	int p=bbox.size();
	bbox.push_back(newbox);
	
	if(lbranch!=-1) bbox[lbranch].root=p;//parent
	if(rbranch!=-1) bbox[rbranch].root=p;

	return p;
}
void buildtree()
{
	int n3=trilist.size()/3;//egyessevel!
	qvector<int> triindx;
	loopd(i,0,n3,1) triindx.push_back(i);
			
	printf("bbox: %d %d  \n",bbox.size(),n3);
	rootindx=build_kdtree(triindx);
	printf("bbox: %d %d\n",bbox.size(),rootindx);

	xy=0;
#ifdef _USEFOURTX	
	hh=4096/4;
	kdtree_data2=new vec3[hh*hh];
	kdtree_data3=new vec3[hh*hh];
	kdtree_data4=new vec3[hh*hh];

	kdtree_data5=new vec3[hh*hh];
	kdtree_data6=new vec3[hh*hh];
	kdtree_data7=new vec3[hh*hh];
#else
	hh=4096/2;
#endif	
	kdtree_data1=new vec3[hh*hh];
	kdtree_data1half=new half4[hh*hh];

	kdtree_data2=new vec3[hh*hh];
	kdtree_data3=new vec3[hh*hh];
	
#ifdef _VOXELBASED
	quadtree_data1=new vec3[vx_size*vx_size*vx_size];// 64*64*64=512*512
	fill_tx_quadtree();
#else	
	walktree(rootindx,0); // fill kdtree_data1
#endif	
	
	tx_kdtreesize=vec3(1.0/(float1)hh,0.0,0.0);//stp
	tx_kdtreemax=vec3(xy%hh,xy/hh,0)/(float1)hh;
	printf("%d sor , tx_kdtreesize %e \n",xy/hh,1.0/tx_kdtreesize.x);
	
	for(int i=0;i<hh*hh;i++) 
	{
		kdtree_data1half[i].x=float2half(kdtree_data1[i].x);
		kdtree_data1half[i].y=float2half(kdtree_data1[i].y);
		kdtree_data1half[i].z=float2half(kdtree_data1[i].z);
		kdtree_data1half[i].w=0;
	}
}


void precalc()
{	
	xtimer("");
	wbmin=vec3(1e16,1e16,1e16);
	wbmax=vec3(-1e16,-1e16,-1e16);

	loopd(i,0,trilist.size(),3)
	{
		vec3 &v1=trilist[i];
		vec3 &v2=trilist[i+1];
		vec3 &v3=trilist[i+2];

		vec3 edge1=v2-v1;
		vec3 edge2=v3-v2;
		vec3 edge3=v1-v3;
		vec3 norm=normalize(cross(edge1,edge2));	
		
		vec3 ray=eye-v1;
		if(dot(norm,ray)<0.0) norm=vec3(0,0,0)-norm;
	
		vec3 edgenorm1=normalize(cross(edge1,norm));
		vec3 edgenorm2=normalize(cross(edge2,norm));
		vec3 edgenorm3=normalize(cross(edge3,norm));

		if(dot(edgenorm1,v3-v1)<0.0) edgenorm1=vec3(0,0,0)-edgenorm1;
		if(dot(edgenorm2,v1-v2)<0.0) edgenorm2=vec3(0,0,0)-edgenorm2;
		if(dot(edgenorm3,v2-v3)<0.0) edgenorm3=vec3(0,0,0)-edgenorm3;
	
#if 0	
		normlist.push_back(norm);
		normlist.push_back(edgenorm1);
		normlist.push_back(edgenorm2);
		normlist.push_back(edgenorm3);
#endif
	
/*		int tilt2=0;
		if(length(edge1)<1e-5) tilt2=1;//cut
		if(length(edge2)<1e-5) tilt2=1;
		if(length(edge3)<1e-5) tilt2=1;
		tilt.push_back(tilt2);*/

//nem hagyhat ki!
		{	
			vec3 bbmin=vec3(1e16,1e16,1e16);
			vec3 bbmax=vec3(-1e16,-1e16,-1e16);

			chk_bbox_mm(v1,bbmin,bbmax);
			chk_bbox_mm(v2,bbmin,bbmax);
			chk_bbox_mm(v3,bbmin,bbmax);

			sbbox newbox;
			newbox.bmin=bbmin;
			newbox.bmax=bbmax;
//			newbox.size=length(bbmax-bbmin);
			newbox.left=-1;
			newbox.right=-1;
			newbox.root=-1;
			newbox.tri=i/3;// ok ua
			bbox.push_back(newbox);//trilist/3 szerint!

			chk_bbox_mm(bbmin,wbmin,wbmax);
			chk_bbox_mm(bbmax,wbmin,wbmax);
		}
	}
	printf("wbmin: %e %e %e \n",wbmin.x,wbmin.y,wbmin.z);	
	printf("wbmax: %e %e %e \n",wbmax.x,wbmax.y,wbmax.z);	

	
#ifdef _VOXELBASED
	setup_quadtree(wbmin,wbmax); // buildtree elott!      [build_kdtree kiboviti a bbox-ot!]
	dump_quadtree();
#endif

	
	buildtree();
	xtimer("kdtree build");
}


void add_phyfile(const char *name,int s,vec3 base)//oke
{
	int ni,nv;
    FILE *ff=fopen(name,"r");
    if(ff==NULL) {printf("%s \n",name);return;}
    
    fscanf(ff,"%d\n",&nv);
    fscanf(ff,"%d\n",&ni);
printf("%d %d \n",nv,ni);   

    vec3 *vv=new vec3[nv];
	vec3 bbmin=vec3(1e16,1e16,1e16);
	vec3 bbmax=vec3(-1e16,-1e16,-1e16);
    
	loopd(i,0,nv,1)
	{
	    fscanf(ff,"%f %f %f\n",&vv[i].x,&vv[i].y,&vv[i].z);
		chk_bbox_mm(vv[i],bbmin,bbmax);
	}
		vec3 cent=(bbmin+bbmax)/2.0;
	vec3 vsize=(bbmax-bbmin)/2.0;
	vsize.y=320.0/vsize.y;
	loopd(i,0,nv,1)
	{
		vv[i]-=cent;
		vv[i]*=vsize.y;
		vv[i].y+=320.0;
	
		vv[i]*=0.5;
		vv[i].x+=1000.0;//nem lehet -!
		vv[i].z+=1000.0;
	}
	
	int *faces=new int[ni*3];
	vec3 *norms=new vec3[ni*3];
	loopd(y,0,ni,1)
	{
	    int u=-1,f1=0,f2=0,f3=0;
	    
	    fscanf(ff,"%d %d %d %d",&u,&f1,&f2,&f3);
	    faces[y*3  ]=f1;
	    faces[y*3+1]=f2;
	    faces[y*3+2]=f3;
	}
	loopd(y,0,ni,1)
	{
	    int f1=faces[y*3  ];
	    int f2=faces[y*3+1];
	    int f3=faces[y*3+2];
	    vec3 v1=vv[f1];
	    vec3 v2=vv[f2];
	    vec3 v3=vv[f3];

		norms[f1]=normalize(cross(v2-v1,v3-v1));
		norms[f2]=norms[f1];
		norms[f3]=norms[f1];
	}
	
	loopd(y,0,ni,1)
	{
	    int f1=faces[y*3  ];
	    int f2=faces[y*3+1];
	    int f3=faces[y*3+2];
	    vec3 v1=vv[f1];
	    vec3 v2=vv[f2];
	    vec3 v3=vv[f3];
	    vec3 n1=norms[f1];
	    vec3 n2=norms[f2];
	    vec3 n3=norms[f3];

		v1+=base;
		v2+=base;
		v3+=base;
		
		deftri(v1,v2,v3,n1,n2,n3,vec3(0.8,0.65,0.2),0);//gold
//		deftri(v1,v2,v3,n1,n2,n3,vec3(0.6,0.0,0.0),0);//red

	}
	
	delete vv;
}


