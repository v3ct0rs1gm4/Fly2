/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



#include "q_loader.h"




char sellevel[110];
int selfacetest=28;// dump
int selfacetest2=38;


float1 ratio=1.0;
int carrier=0;
int gcc2[8]={0};


vec3 z_pos1,z_pos2,z_ray,*z_normal,*z_deep,*z_obj;
float1 z_len,z_tmin;

vec3 refdir;


int64_t addr1=0,addr2=0;










void transpose(axis4 *mat1,axis4 *mat2)
{
	mat1->x.x=mat2->x.x;
	mat1->x.y=mat2->y.x;
	mat1->x.z=mat2->z.x;
	mat1->x.w=mat2->w.x;
	
	mat1->y.x=mat2->x.y;
	mat1->y.y=mat2->y.y;
	mat1->y.z=mat2->z.y;
	mat1->y.w=mat2->w.y;
	
	mat1->z.x=mat2->x.z;
	mat1->z.y=mat2->y.z;
	mat1->z.z=mat2->z.z;
	mat1->z.w=mat2->w.z;
	
	mat1->w.x=mat2->x.w;
	mat1->w.y=mat2->y.w;
	mat1->w.z=mat2->z.w;
	mat1->w.w=mat2->w.w;
}



// new kdtree        triangle sorrend/pos nem valtozik, szoval eleg egyszer felepiteni
int c_mesh::s_kdtree::build_kdtree_branch(qvector<int> &indx)
{
	int n=indx.size();
	if(n<=0) return -1;
	if(n==1) return indx[0];
	
	s_bbox tmp;
	loopd(i,0,n,1)	tmp.add_bbox(&bboxs[indx[i]]);

	vec3 dbox=tmp.bbmax-tmp.bbmin;
/*	dbox.x=tmp.bbmax.x-tmp.bbmin.x;// vec3i
	dbox.y=tmp.bbmax.y-tmp.bbmin.y;
	dbox.z=tmp.bbmax.z-tmp.bbmin.z;*/
	int op=0;
	if(dbox.y>dbox.x)	if(dbox.y>dbox.z) op=1;//legszelesebb
	if(dbox.z>dbox.x)	if(dbox.z>dbox.y) op=2;

	std::multimap<float1,int> reindx;// ua tobbszor lehet!
	loopd(i,0,n,1)
	{
		int bbid=indx[i];

		float1 t1=0.0;
		switch(op)
		{
		case 0: t1=(bboxs[bbid].bbmin.x+bboxs[bbid].bbmax.x)/2.0;break;
		case 1: t1=(bboxs[bbid].bbmin.y+bboxs[bbid].bbmax.y)/2.0;break;
		case 2: t1=(bboxs[bbid].bbmin.z+bboxs[bbid].bbmax.z)/2.0;break;
		}

		std::pair<float1,int> tmp2(t1,bbid);	
		reindx.insert(reindx.end(),tmp2);// sorba a legszelesebb coord szerint
	}

	qvector<int> indx1;//szedszed 2 fele
	qvector<int> indx2;
	int j=0;
	for(std::multimap<float1,int>::iterator it=reindx.begin();it!=reindx.end();++it)
	{//	printf("%f ",it->first);
		if(j<n/2) indx1.push_back(it->second);//fele
		else      indx2.push_back(it->second);
		j++;
	}
//printf("\n");
		
	reindx.clear();
	int lbranch=build_kdtree_branch(indx1);//recursive
	int rbranch=build_kdtree_branch(indx2);
//printf(".. %d %d %d %d \n",n,j,indx1.size(),indx2.size());	

	s_bbox newbox;
	newbox.bbmin =tmp.bbmin;
	newbox.bbmax =tmp.bbmax;
//	newbox.size=length(bbmax-bbmin);
	newbox.next1=lbranch;//bboxs[] index!
	newbox.next2=rbranch;

	int p=bboxs.size();
	bboxs.push_back(newbox);// a root a base bboxs utan jon majd!
	
	if(lbranch!=-1) bboxs[lbranch].root=p;// ez a parentje a kovetkezo kettonek
	if(rbranch!=-1) bboxs[rbranch].root=p;

	return p;
}

void c_mesh::s_kdtree::build_kdtree(qvector<vec3> &verts,qvector<int> &faces,qvector<int> &types)
{
	bboxs.clear();
	loopd(i,0,faces.size(),3)//triangles
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];

		s_bbox tmp;		
		tmp.set_vec3(verts[f1]);
		tmp.add_vec3(verts[f2]);
		tmp.add_vec3(verts[f3]);
		tmp.indx=i;
		if((types[i]&32)==0 &&(types[i]&64)==0  &&(types[i]&4096)==0 ); else tmp.type=0;	//wheel nem  hook nem
		bboxs.push_back(tmp);
//   bboxs[i/3] <=> faces[i] <==> bboxs[i/3].indx
	}
	//majd mirr kell a rep hez ,eloszor carrier
	
	qvector<int> indxtab;
	loopd(i,0,faces.size()/3,1) indxtab.push_back(i);
	
	printf("kdtree 1 %d \n",bboxs.size());
	rootindx=build_kdtree_branch(indxtab);
	printf("build kdtree 2 %d \n",bboxs.size());
	
	store();
}

void c_mesh::s_kdtree::update_kdtree_branch(int indx)
{
	if(bboxs[indx].next1!=-1) {update_kdtree_branch(bboxs[indx].next1); bboxs[indx].set_bbox(&bboxs[bboxs[indx].next1]);}
	if(bboxs[indx].next2!=-1) {update_kdtree_branch(bboxs[indx].next2); bboxs[indx].add_bbox(&bboxs[bboxs[indx].next2]);}
}
void c_mesh::s_kdtree::update_kdtree(qvector<vec3> &verts,qvector<int> &faces,vec3 pos)
{
#ifdef _ORIGINAL_KDTREE_UPDATE
//#if 1 // test
	if(cc5==0)//ritkan    pontos
//	if(1)//test
	{
		int i3=0;
		loopd(i,0,faces.size(),3)//triangles    0.22ms
		{
			int f1=faces[i];
			int f2=faces[i+1];
			int f3=faces[i+2];

			bboxs[i3].set_vec3(verts[f1]);
			bboxs[i3].add_vec3(verts[f2]);
			bboxs[i3].add_vec3(verts[f3]);
			i3++;
		}
		update_kdtree_branch(rootindx);
//	printf("%d \n",gcc5);//217 ok
	}
	else//approximation     no rotation
	{
		vec3 dpos=pos-pos_kd;//ennyit ment az utolso ota             %4 -> 0.16ms   %8 -> 0.12-0.16ms    (1 frame 0.22)

		loopd(i,0,bboxs.size(),1)
		{
			bboxs[i].bbmin+=dpos;
			bboxs[i].bbmax+=dpos;
		}
	}

	cc5++;
	cc5%=4;//4 8      ne hibazzon sokat!
	pos_kd=pos;//tramat.w;
#endif	
}

void c_mesh::s_kdtree::chk_kdtree(int indx,s_bbox &ray_bbox,qvector<int> &selected_faces)
{
	if(bboxs[indx].chk_bbox(&ray_bbox))// benne van
	{
		if(bboxs[indx].next1!=-1) chk_kdtree(bboxs[indx].next1,ray_bbox,selected_faces);
		if(bboxs[indx].next2!=-1) chk_kdtree(bboxs[indx].next2,ray_bbox,selected_faces);
		
		if(bboxs[indx].indx!=-1) if(bboxs[indx].type==1) selected_faces.push_back(bboxs[indx].indx);// collect insiders
	}
}
//kdtree base ready 1.5 hour xDD   + 1 hour chk_kdtree

void c_mesh::s_kdtree::chk_kdtree2x(qvector<s_bbox> &bboxs2,int indx1,int indx2,qvector<int> &selected_faces)//selected_faces 2-essevel!!!
{
	if(bboxs[indx1].chk_bbox(&bboxs2[indx2]))// benne van
	{
		int indx11=indx1,indx12=indx1,indx21=indx2,indx22=indx2;
		if(bboxs[indx1].next1!=-1) indx11=bboxs[indx1].next1;
		if(bboxs[indx1].next2!=-1) indx12=bboxs[indx1].next2;
		if(bboxs2[indx2].next1!=-1) indx21=bboxs2[indx2].next1;
		if(bboxs2[indx2].next2!=-1) indx22=bboxs2[indx2].next2;

//		int def=0;//barmelyik ag megall, definialni a ket trianglet
		if(indx11!=indx1 || indx21!=indx2) chk_kdtree2x(bboxs2,indx11,indx21,selected_faces);		//else def=1;//ha nincs egy next sem, stop
		if(indx11!=indx1 || indx22!=indx2) chk_kdtree2x(bboxs2,indx11,indx22,selected_faces);		//else def=1;//amugy az egyik megall
		if(indx12!=indx1 || indx21!=indx2) chk_kdtree2x(bboxs2,indx12,indx21,selected_faces);		//else def=1;
		if(indx12!=indx1 || indx22!=indx2) chk_kdtree2x(bboxs2,indx12,indx22,selected_faces);		//else def=1;
		
//?		if(def)
		{
			if(bboxs[indx1].indx!=-1) 
			if(bboxs2[indx2].indx!=-1) 
			if(bboxs[indx1].type==1)// wheel hook nem kell
			if(bboxs2[indx2].type==1)
			{
				selected_faces.push_back(bboxs[indx1].indx);// collect insiders
				selected_faces.push_back(bboxs2[indx2].indx);
			}
		}
		gcc2[6]++;
	}
}


void c_mesh::update_kdtree()
{
	if(invalid_kdtree)
	{
		invalid_kdtree=0;
		
#ifdef _ORIGINAL_KDTREE_UPDATE
//#if 1 //test
		double t1=q_getcpuclock();
		kdt.update_kdtree(pnts_trn,faces,vec3(tramat.w.x,tramat.w.y,tramat.w.z));
	
		t1=q_getcpuclock()-t1;	timedump2+=t1; //  all 0.5ms       4xplane+carrier       update_kdtree!!!   ~0.22 ms
#endif
	}
}
//------------------------------------------------------------------------------------


void chk_bbox5(vec3 &v1,vec3 &mmin,vec3 &mmax)
{
	if(v1.x<mmin.x) mmin.x=v1.x;
	if(v1.x>mmax.x) mmax.x=v1.x;
	if(v1.y<mmin.y) mmin.y=v1.y;
	if(v1.y>mmax.y) mmax.y=v1.y;
	if(v1.z<mmin.z) mmin.z=v1.z;
	if(v1.z>mmax.z) mmax.z=v1.z;
}

void c_mesh::transf4(vec3 pos,axis4 &ax,int mask)//pos NA!
{
QDUMPFNC();
	if(pnts_trn.size()<pnts.size())
		pnts_trn.resize(pnts.size());
	
	loopd(i,0,pnts.size(),1)
	if((mask==0 && vtypes[i]==0) || ((vtypes[i]&mask)==mask)) // nem hianyozhat bit!
	{
		vec4 w4;
		vec3 &v11=pnts[i];
		w4=ax.x*v11.x +ax.y*v11.y +ax.z*v11.z +ax.w;
		
		pnts_trn[i].x=w4.x;
		pnts_trn[i].y=w4.y;
		pnts_trn[i].z=w4.z;
	}
}
void c_mesh::transf4all(vec3 pos,axis4 &ax)
{
QDUMPFNC();
	if(pnts_trn.size()<pnts.size())
		pnts_trn.resize(pnts.size());
	
	loopd(i,0,pnts.size(),1)
	{
		vec4 w4;
		vec3 &v11=pnts[i];
		w4=ax.x*v11.x +ax.y*v11.y +ax.z*v11.z +ax.w;
		
		pnts_trn[i].x=w4.x;
		pnts_trn[i].y=w4.y;
		pnts_trn[i].z=w4.z;
	}
}
void c_mesh::transf42(vec3 pos,axis4 &ax,int mask,int mask2)//pos NA!
{
QDUMPFNC();
	if(pnts_trn.size()<pnts.size())
		pnts_trn.resize(pnts.size());
	
	loopd(i,0,pnts.size(),1)
	if((vtypes[i]&mask)==0 && (mask2==0 || vtypes[i]&mask2))//reverse: ha nem a mask
	{
		vec4 w4;
		vec3 &v11=pnts[i];

		w4=ax.x*v11.x +ax.y*v11.y +ax.z*v11.z +ax.w;
		
		pnts_trn[i].x=w4.x;
		pnts_trn[i].y=w4.y;
		pnts_trn[i].z=w4.z;
	}
}

void c_mesh::trans(vec3 pos,axis &ax)
{
QDUMPFNC();
	if(pnts_trn.size()<pnts.size())
		pnts_trn.resize(pnts.size());
		
	loopd(i,0,pnts.size(),1)
	{
		vec3 &v11=pnts[i];

		pnts_trn[i]=ax.x*v11.x +ax.y*v11.y +ax.z*v11.z +pos;
	}
}
void c_mesh::draw(vec3 pos,axis &ax,int mir)
{
QDUMPFNC();
	qassert(txcoord.size());

	glBegin2(GL_TRIANGLES);

	loopd(i,0,faces.size(),3)//triangles
	if((i+2)<faces.size())
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 v1=pnts_trn[f1];
		vec3 v2=pnts_trn[f2];
		vec3 v3=pnts_trn[f3];
		
		if(mir)
		{
			v1.y=-v1.y;
			v2.y=-v2.y;
			v3.y=-v3.y;
		}

//		vec3 norm=normalize(cross(pnts[f3]-pnts[f1],pnts[f2]-pnts[f1]));//before
		vec3 norm=normalize(cross(v3-v1,v2-v1));//after
		if(inverz_normal[i]) norm=-norm;//vec3(0,0,0)
		if(types[i]&8192) norm=-norm;// _mir


		vec4 col44(1.0,1.0,1.0,1.0);
		
//auto 	texture from normal
		vec3 txc1=txcoord[f1];
		vec3 txc2=txcoord[f2];
		vec3 txc3=txcoord[f3];

		if(	fabs(norm.y)>0.9);//0.9
	    else
	    {
		if(	fabs(norm.x)>0.7)//0.7
		{
//		col44=vec4(1.0,0.0,0.0,1.0);//test
			 txc1=txcoord2[f1];
			 txc2=txcoord2[f2];
			 txc3=txcoord2[f3];
			txc1.x=txc1.y;
			txc2.x=txc2.y;
			txc3.x=txc3.y;
			txc1.y=txc1.z;
			txc2.y=txc2.z;
			txc3.y=txc3.z;
		}
	    else
		{
	//	col44=vec4(0.0,1.0,0.0,1.0);
			txc1.y=txc1.z;
			txc2.y=txc2.z;
			txc3.y=txc3.z;
		}
		}


		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			

		glMultiTexCoord2f(0,txc1.x,1.0-txc1.y);		
		glMultiTexCoord3f(2,norm.x,norm.y,norm.z);		
		glVertex3f(v1.x,v1.y,v1.z);///zY!

		glMultiTexCoord2f(0,txc2.x,1.0-txc2.y);		
		glMultiTexCoord3f(2,norm.x,norm.y,norm.z);		
		glVertex3f(v2.x,v2.y,v2.z);

		glMultiTexCoord2f(0,txc3.x,1.0-txc3.y);		
		glMultiTexCoord3f(2,norm.x,norm.y,norm.z);		
		glVertex3f(v3.x,v3.y,v3.z);

#if 0
//norm ,
		vec3 v4=v1+norm*10.0;
		glVertex3f(v1.x,v1.y,v1.z);
		glVertex3f(v1.x+0.3,v1.y+0.3,v1.z+0.3);
		glVertex3f(v4.x,v4.y,v4.z);
#endif		
	}
	glEnd2();		
}



int c_mesh::getcollision4(vec3 pos,vec3 pos2,vec3 &deep,vec3 &normal,vec3 &obj,int &indx)//kdtree vs ray
{
	s_bbox ray_bbox;
	vec3 ray=(pos2-pos);
	float1 len=length(ray);
	ray=normalize(ray);
	deep=vec3(0,0,0);
	
	if(pos2.y<0.0) {deep.y=-pos2.y;deep.x=fabs(deep.y);normal=vec3(0,1,0);obj=pos2;obj.y=0.0;return 2;}//ground   -?
//printf("%d \n",	faces.size()/3);// carrier: 109 

	ray_bbox.set_vec3(pos);
	ray_bbox.add_vec3(pos2);
	ray_bbox.transf=0;

	update_kdtree();

	double t1=q_getcpuclock();
	qvector<int> selected_faces;
	qvector<int> selected_faces2;
	kdt.chk_kdtree(kdt.rootindx,ray_bbox,selected_faces);// 10  0.1ms !!!!!!!!! xDDDD yeahhh
	t1=q_getcpuclock()-t1;	timedump2+=t1; //  all 0.5ms       4xplane+carrier       update_kdtree!!!   ~0.22 ms

	loopd(j,0,selected_faces.size(),1)// selected triangles only
	{
		int i=selected_faces[j];
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 &v1=pnts_trn[f1];
		vec3 &v2=pnts_trn[f2];
		vec3 &v3=pnts_trn[f3];
		
		vec3 norm=(cross(v3-v1,v2-v1));// no normalize ugyan az mert osztas van
		
		float1 t1=dot(v1-pos,norm)/dot(ray,norm);// v1 a z=0 sik szoval  -pos.z/-ray.z=+
		if(t1>=0.0)
		if(t1<=len)
		{
			obj=pos + ray*t1;

			vec3 w1,n1;//levagja a kilogo reszt, w1 meroleges lesz n1-re
			w1=v3-v1;		n1=(v2-v1);	vec3 edge_norm1=w1-n1*dot(n1,w1)/dot(n1,n1);
			w1=v1-v2;		n1=(v3-v2);	vec3 edge_norm2=w1-n1*dot(n1,w1)/dot(n1,n1);
			w1=v2-v3;		n1=(v1-v3);	vec3 edge_norm3=w1-n1*dot(n1,w1)/dot(n1,n1);
		
			if(dot(obj-v1,edge_norm1)>=0.0)
			if(dot(obj-v2,edge_norm2)>=0.0)
			if(dot(obj-v3,edge_norm3)>=0.0)
			{
				norm=normalize(norm);
				if(inverz_normal[i]) norm=-norm;// carrier only  , (mirrornal forditva kellene , de itt az nincs)
				if(types[i]&8192) norm=-norm;// _mir
				
				float1 deep_1=dot(pos-obj,norm);// a befele eso - lesz
				float1 deep_2=dot(pos2-obj,norm);
//				vec3 deep5;
				if(deep_1<deep_2) deep.y=deep_1;//a nyil hegye kell pos2
				else              deep.y=deep_2;
				deep.x=fabs(deep.y);//?

//				if(deep5.y<deep.y)
				{
					//deep=deep5;
					//metszi tehat valamelyik vege a vonalnak tuti belul van
					normal=norm;
					indx=i;
					return 1; // mind?
				}
			}
		}
	}
	return 0;
}

//ray triangle
int c_mesh::getcollision5(vec3 pos,vec3 pos2,int facei,vec3 &deep,vec3 &normal,vec3 &obj,int &indx)
{
	vec3 ray=(pos2-pos);
	float1 len=length(ray);
	ray=normalize(ray);
	deep=vec3(0,0,0);
	
//??	if(pos2.y<0.0) {deep=-pos2.y;normal=vec3(0,1,0);obj=pos2;obj.y=0.0;return 1;}//ground   -?
//printf("%d \n",	faces.size()/3);// carrier: 109 


		int f1=faces[facei];
		int f2=faces[facei+1];
		int f3=faces[facei+2];
		
		vec3 &v1=pnts_trn[f1];
		vec3 &v2=pnts_trn[f2];
		vec3 &v3=pnts_trn[f3];
		
		vec3 norm=(cross(v3-v1,v2-v1));// no normalize ugyan az mert osztas van
		
		float1 t1=dot(v1-pos,norm)/dot(ray,norm);// v1 a z=0 sik szoval  -pos.z/-ray.z=+
		if(t1>=0.0)
		if(t1<=len)
		{
			obj=pos + ray*t1;

			vec3 w1,n1;//levagja a kilogo reszt, w1 meroleges lesz n1-re
			w1=v3-v1;		n1=(v2-v1);	vec3 edge_norm1=w1-n1*dot(n1,w1)/dot(n1,n1);
			w1=v1-v2;		n1=(v3-v2);	vec3 edge_norm2=w1-n1*dot(n1,w1)/dot(n1,n1);
			w1=v2-v3;		n1=(v1-v3);	vec3 edge_norm3=w1-n1*dot(n1,w1)/dot(n1,n1);
		
			if(dot(obj-v1,edge_norm1)>=0.0)
			if(dot(obj-v2,edge_norm2)>=0.0)
			if(dot(obj-v3,edge_norm3)>=0.0)
			{
				norm=normalize(norm);
				if(inverz_normal[facei]) norm=-norm;// carrier only  , (mirrornal forditva kell)
				if(types[facei]&8192) norm=-norm;// _mir
				
				float1 deep_1=dot(pos-obj,norm);// a befele eso - lesz
				float1 deep_2=dot(pos2-obj,norm);
				if(deep_1<deep_2) deep.y=deep_1;//a nyil hegye kell pos2
				else              deep.y=deep_2;
				deep.x=fabs(deep.y);
				
				//metszi tehat valamelyik vege a vonalnak tuti belul van
				normal=norm;
				indx=facei;
				return 1;
			}
		}

	return 0;
}


int c_mesh::chk_plane_ray(vec3 pos,vec3 pos2,float1 &deep,int ex)
{
	vec3 ray=(pos2-pos);
	float1 len=length(ray);
	ray=normalize(ray);

	loopd(i,0,faces.size(),3)//triangles   gyorsitani?!!
	if(i!=ex)//sajat magat ne
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 v1=pnts[f1];
		vec3 v2=pnts[f2];
		vec3 v3=pnts[f3];
		
		vec3 norm=(cross(v3-v1,v2-v1));//normalize     tuti nem kell! tesztelve ua
		
		float1 t1=dot(v1-pos,norm)/dot(ray,norm);
		if(t1>=0.0)
		if(t1<=len)
		{
			vec3 obj=pos + ray*t1;

			vec3 w1,n1;//levagja a kilogo reszt, w1 meroleges lesz n1-re
			w1=v3-v1;		n1=(v2-v1);	vec3 edge_norm1=w1-n1*dot(n1,w1)/dot(n1,n1);
			w1=v1-v2;		n1=(v3-v2);	vec3 edge_norm2=w1-n1*dot(n1,w1)/dot(n1,n1);
			w1=v2-v3;		n1=(v1-v3);	vec3 edge_norm3=w1-n1*dot(n1,w1)/dot(n1,n1);
		
			if(dot(obj-v1,edge_norm1)>=0.0)
			if(dot(obj-v2,edge_norm2)>=0.0)
			if(dot(obj-v3,edge_norm3)>=0.0)
			{
				deep=fabs(t1);//old
				
				//norm=normalize(norm);// new
				//deep=dot(pos2-obj,norm);
				return 1;
			}
		}
	}
	return 0;
}
void c_mesh::chk_surface_occlusion()
{
	float1 deep;
	loopd(i,0,faces.size(),3)//triangles   gyorsitani?!!
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 &v1=pnts[f1];
		vec3 &v2=pnts[f2];
		vec3 &v3=pnts[f3];
		vec3 norm=normalize(cross(v3-v1,v2-v1));////v31-v21 felfele
		vec3 cent=(v1+v2+v3)/3.0;

		if(chk_plane_ray(cent+norm*0.001,cent+norm*3.0,deep,i))//3m a hatso szarny miatt max4m!
		{
			types[i]|=256;//hided   1sided
		}
		if(chk_plane_ray(cent-norm*0.001,cent-norm*3.0,deep,i))//all direction chk
		{
			types[i]|=512;//hided   1sided  backward
		}
	}
}

void c_mesh::chk_normal_occlusion()//carrier normal
{
	float1 deep1=0.0;
	float1 deep2=0.0;
	inverz_normal.resize(faces.size());

/*	vec3 center;// v2 NA
	loopd(i,0,pnts.size(),1) center+=pnts[i];
	center/=pnts.size();*/
	
	loopd(i,0,faces.size(),3)
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 &v1=pnts[f1];
		vec3 &v2=pnts[f2];
		vec3 &v3=pnts[f3];
		vec3 norm=normalize(cross(v3-v1,v2-v1));////v31-v21 felfele

#if 0		
//		if(norm.y>0.9 ||norm.y<-0.9 )
		if((i/3)==selfacetest ||(i/3)==selfacetest2 ) //dump
			printf("!!!!!!!!!!!! %d %e %e %e \n",i/3,norm.x,norm.y,norm.z);
#endif
			
#if 1		
		vec3 cent=(v1+v2+v3)/3.0;
		int hit1=chk_plane_ray(cent+norm*0.01,cent+norm*2000.0,deep1,i);//kozepebol ket iranyba az egyik sugar utkozik a masik nem
		int hit2=chk_plane_ray(cent-norm*0.01,cent-norm*2000.0,deep2,i);
		
		if(hit1==1 && hit2==0)	inverz_normal[i]=0;//1 ha hit1 akkor nyilvan forditva van a normal
		if(hit1==0 && hit2==1)	inverz_normal[i]=1;//0 TODO BUG forditva kellene!!!!!  lehet a transformalt kifordul , de nem ugy tunt... dunno xD igy jo!

		if((hit1 && hit2) ||(hit1==0 && hit2==0) )// ez okozta a BUG ot   00 rosszat kapott
		{
		//torony fala lefele nez, hatulja kozelebb mint a deck          ? < volt
			//if(fabs(deep2)>fabs(deep1))	inverz_normal[i]=0;//ha a hatulja melyebb  >?
			//else						inverz_normal[i]=1;
			
			if(norm.z>0.0) inverz_normal[i]=1;// ha lefele nez -(fejjel van!), es kesz          z az editor magassag!
		}
#else
/*		if(dot(v1-center,norm)<0.0) inverz_normal[i]=1;// v2 test ????? rossz
		else inverz_normal[i]=0;*/
#endif		
	}
}
void c_mesh::chk_normal_occlusion_trn()//carrier normal     onfly check
{
	float1 deep1=0.0;
	float1 deep2=0.0;
	inverz_normal.resize(faces.size());


	loopd(i,0,faces.size(),3)
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 &v1=pnts_trn[f1];
		vec3 &v2=pnts_trn[f2];
		vec3 &v3=pnts_trn[f3];
		vec3 norm=normalize(cross(v3-v1,v2-v1));////v31-v21 felfele

		vec3 cent=(v1+v2+v3)/3.0;
		int hit1=chk_plane_ray(cent+norm*0.01,cent+norm*2000.0,deep1,i);//kozepebol ket iranyba az egyik sugar utkozik a masik nem
		int hit2=chk_plane_ray(cent-norm*0.01,cent-norm*2000.0,deep2,i);
		
		if(hit1==1 && hit2==0)	inverz_normal[i]=1;//1 ha hit1 akkor nyilvan forditva van a normal
		if(hit1==0 && hit2==1)	inverz_normal[i]=0;//0 TODO BUG forditva kellene!!!!!               !a transformalt kifordult !

		if((hit1 && hit2) ||(hit1==0 && hit2==0) )// ez okozta a BUG ot   00 rosszat kapott
		{
		//torony fala lefele nez, hatulja kozelebb mint a deck         
			if(norm.y<0.0) inverz_normal[i]=1;// ha lefele nez -, es kesz          y az ingame magassag!
		}
	}
}

//##############################################################################3
//c_mesh *tmpmesh=0;c_mesh *tmpmesh2=0;c_mesh *tmpmesh3=0;qvector<vec3> *g_colls;




#ifndef _KDTREE_VS_KDTREE_CHECK

//carries vs ray of airplane	 by kdtree      2nd
int c_mesh::chk_mesh2meshcollis4(c_mesh* mesh,vec3 refd,qvector<s_coll> &_colls,int op)
{
	int ehit=0;

	_colls.clear();

#if 1 //test
//  broadband collision detection
	s_bbox tmp,tmp2;
	tmp.set_bbox(&mesh->kdt.bboxs[mesh->kdt.rootindx]);
	tmp2.set_bbox(&kdt.bboxs[kdt.rootindx]);

	float1 lodlen=length(tmp2.bbmax - tmp2.bbmin) + length(tmp.bbmax - tmp.bbmin);

	vec3 dx=mesh->pnts_trn[0] - pnts_trn[0];
	if(length(dx)>lodlen) return ehit;//  broadband coll tuti messze
#endif	

	
	update_kdtree();// csak 1x csinalja meg / frame
	mesh->update_kdtree();// TODO  igazabol ez nem kellene , mert ray van

#if 1 //test
//  broadband coll tuti messze
	if(!kdt.bboxs[kdt.rootindx].chk_bbox(&mesh->kdt.bboxs[mesh->kdt.rootindx])) // carrier vs airplane
		return ehit;
#endif

	s_cols tmp;

	loopd(i,0,mesh->edges.size(),1)// lines
	{
		int f1=mesh->edges[i].a;
		int f2=mesh->edges[i].b;
		
		vec3 &v1=mesh->pnts_trn[f1];
		vec3 &v2=mesh->pnts_trn[f2];
		
		int hit=getcollision4(v1,v2,tmp.deep,tmp.normal,tmp.obj,tmp.iface);
		if(hit)
		{
			_colls.push_back(tmp);
			ehit=1;
		}
	}
	
	return ehit;
}
#else 
//ez profibb megoldas ,                     de valami meg rossz: kerek hook benne maradt! OK

//carries vs ray of airplane	 by kdtree      2nd
int c_mesh::chk_mesh2meshcollis4(c_mesh* mesh,vec3 refd,qvector<s_coll> &_colls,int op)
{
	int ehit=0;

	_colls.clear();

#if 1 //test
//  broadband collision detection
	s_bbox tmp1,tmp2;
	tmp1.set_bbox(&mesh->kdt.bboxs[mesh->kdt.rootindx]);
	tmp2.set_bbox(&kdt.bboxs[kdt.rootindx]);

	float1 lodlen=length(tmp2.bbmax - tmp2.bbmin) + length(tmp1.bbmax - tmp1.bbmin);

	vec3 dx=mesh->pnts_trn[0] - pnts_trn[0];
	if(length(dx)>lodlen) return ehit;//  broadband coll tuti messze
#endif	

	
	update_kdtree();// csak 1x csinalja meg / frame
	mesh->update_kdtree();// TODO  igazabol ez nem kellene , mert ray van

#if 1 //test
//  broadband coll tuti messze
	if(!kdt.bboxs[kdt.rootindx].chk_bbox(&mesh->kdt.bboxs[mesh->kdt.rootindx])) // carrier vs airplane
		return ehit;
#endif

	s_coll tmp;

//elol legyen a repcsi!!
	gcc2[6]=0;
	double t1=q_getcpuclock();
	qvector<int> selected_faces;
	qvector<int> selected_faces2;	//elol legyen a repcsi
	mesh->kdt.chk_kdtree2x(kdt.bboxs,mesh->kdt.rootindx,kdt.rootindx,selected_faces);
	t1=q_getcpuclock()-t1;	timedump2+=t1; //  8   0.08ms!!!!!!!!!!!!!!!
//printf("%d %d  %d\n",selected_faces.size()/2,selected_faces2.size()/2,gcc2[6]);//  130/200   700

	loopd(j,0,selected_faces.size(),2)// selected triangles pair ! step2
	{
		int i1=selected_faces[j];//   i1=face of airplane
		int i2=selected_faces[j+1];// i2=face of carrier
		int f1=mesh->faces[i1];
		int f2=mesh->faces[i1+1];
		int f3=mesh->faces[i1+2];
		
		vec3 &v1=mesh->pnts_trn[f1];
		vec3 &v2=mesh->pnts_trn[f2];
		vec3 &v3=mesh->pnts_trn[f3];

		int indx=0;
		int hit=getcollision5(v1,v2,i2,tmp.deep,tmp.nor,tmp.obj,tmp.iface);//iface=i2 de nem az kell, airplane torik
		if(hit)
		{
			tmp.iface=i1;
			_colls.push_back(tmp);
			ehit=1;
		}
		hit=getcollision5(v2,v3,i2,tmp.deep,tmp.nor,tmp.obj,tmp.iface);
		if(hit)
		{
			tmp.iface=i1;
			_colls.push_back(tmp);
			ehit=1;
		}
		hit=getcollision5(v3,v1,i2,tmp.deep,tmp.nor,tmp.obj,tmp.iface);
		if(hit)
		{
			tmp.iface=i1;
			_colls.push_back(tmp);
			ehit=1;
		}
	}
	
	return ehit;
}
#endif













void c_mesh::add_edge(int f1,int f2)
{		
	int ok=1;
	loopd(j,0,edges.size(),1)
	{
		if(edges[j].a==f1)	if(edges[j].b==f2) {ok=0;break;}
		if(edges[j].a==f2)	if(edges[j].b==f1) {ok=0;break;}
	}
	if(ok)// nincs meg ilyen
	{
		int2 tmp;
		
		tmp.a=f1;
		tmp.b=f2;
		edges.push_back(tmp);
	}
}


#if 0
void c_mesh::addForce(vec3 v1,int f0,vec3 F,int sp)
{
	pnts[f0]+=(v1-pnts[f0])*0.7;
}
#else
void c_mesh::addForce(vec3 v1,int f0,vec3 F,int sp)
{
	blackverts.push_back(f0);

	vec3 nor=normalize(pnts[f0] - v1);// ero terjed	
	if(dot(F,nor)>0.0)//???  terjedesi irany?
	{
		vec3 F2=nor*dot(F,nor);
//		vec3 F2=F*(1.0-saturate(length(pnts[f0]-v1)/3.0));// v3.0

		accs[f0]+=F2;// F2*dt/mass precalc
//		pnts[f0]+=F2;// move
		F2=F;// v3.0
		
		distorForce(f0,F2,sp,v1,0.01);
	}	
}
#endif

void c_mesh::distorForce(int f0,vec3 F,int sp,vec3 v1,float1 dt) // called addForce
{
	for(int j=0;j<vv_links[f0].size();j++)//all neigh verts
	{
		int f2=vv_links[f0][j];
		int ok=1;
		loopd(k,0,blackverts.size(),1) if(f2==blackverts[k]) {ok=0;break;} // ua a vert 2x ne!
		if(ok)
		{
			blackverts.push_back(f2);
			vec3 nor=normalize(pnts[f2] - pnts[f0]);// ero terjed			float1 len=length(nor);
			if(dot(F,nor)>0.0)//???  terjedesi irany?
			{
				vec3 nor2=normalize(F);
				vec3 F2=nor*dot(F,nor);
//				vec3 F2=F*(1.0-saturate(length(pnts[f2]-v1)/3.0));// v3.0
				
				accs[f2]+=F2;// F2*dt/mass precalc
				vec3 s=(F2/2.0)*dt*dt; // f2=a        s=a/2*t^2   F2!
				pnts[f2]+=s;// becsapodasi ero

				int j=-1;
				loopd(k,0,vv_links[f0].size(),1) if(vv_links[f0][k]==f2) {j=k;break;}
				
				vec3 dv=pnts[f2] - pnts[f0];
				float1 dx=length(dv)-vv_len[f0][j];
				nor=normalize(dv);
				if(fabs(dx)<0.001) dx=0.0;
				vec3 F3=nor*dx*4e5/2500.0;// D
				s=(F3/2.0)*dt*dt; // f2=a
				pnts[f0]+=s; // rugo ero ut
				pnts[f2]-=s;
				
//				pnts[f2]+=F2;// move
				F2=F;// v3.0
				
				if(sp>0) distorForce(f2,F,sp-1,v1,dt);//distorForce(f2,F2*0.7,sp-1);   F2?
			}
		}
	}
}
/*
				float1 r1=1.0-saturate(vv_len[f0][j]/1.5); // 1.5m -en nullara esik az F

				nor=normalize(pnts[f0] - pnts[f2]);//new normal!
				vec3 v1=pnts[f2]+nor*vv_len[f0][j];//restore
				pnts[f0]+=(v1-pnts[f0])*0.95;// kozelit, hogy mind szamitson
*/

#if 0
// ?  restore length of edge
void c_mesh::restoreEdges(int f0,int sp,float1 D)// D=rugoallando
{
	blackverts2.push_back(f0);

	for(int j=0;j<vv_links[f0].size();j++)//all neigh verts
	{
		int f2=vv_links[f0][j];
		int ok=1;
		loopd(k,0,blackverts2.size(),1) if(f2==blackverts2[k]) {ok=0;break;} // ua a vert 2x ne!
		if(ok)
		{
			blackverts2.push_back(f2);
			vec3 nor=pnts[f2] - pnts[f0];
			float1 len=length(nor);
			nor=normalize(nor);
	
			float1 dx=len-vv_len[f0][j];
			if(fabs(dx)>0.001)
			{
				vec3 F=nor*D*dx;		 // ha ossze megy -
				accs[f0]+=F;//  F2*dt/mass
				accs[f2]-=F;
			}	
			if(sp>0) restoreEdges(f2,sp-1,D);
		}
	}
}
#endif
/*			vec3 v1=pnts[f0]+nor*vv_len[f0][j];
			pnts[f2]+=(v1-pnts[f2])*0.8;// kozelit, hogy mind szamitson
			*/


void c_mesh::restoreEdges(float1 dt)  // v3.0
{
	loopd(i,0,edges.size(),1)
	{
		int f1=edges[i].a;
		int f2=edges[i].b;

		vec3 nor=pnts[f2] - pnts[f1];
		float1 len=length(nor);
		nor=normalize(nor);
	
		int j=-1;
		loopd(k,0,vv_links[f1].size(),1) if(vv_links[f1][k]==f2) {j=k;break;}
		if(j==-1) {printf("updateSpd() j==-1 \n");}// ok
		
		vec3 center=(pnts[f2] + pnts[f1])/2.0;
		float1 dx=vv_len[f1][j]/2.0;
		pnts[f1]=center-nor*dx;
		pnts[f2]=center+nor*dx; // rebuild
	}
}
			

#if 0

void c_mesh::updateSpd(float1 dt,float1 D,float1 K) // v2.0
{
return ; // rossz

	loopd(i,0,edges.size(),1)
	{
		int f1=edges[i].a;
		int f2=edges[i].b;

		vec3 nor=pnts[f2] - pnts[f1];
		vec3 relSpd=spds[f2]-spds[f1];
		float1 len=length(nor);
		nor=normalize(nor);
	
		int j=-1;
		loopd(k,0,vv_links[f1].size(),1) if(vv_links[f1][k]==f2) {j=k;break;}
		if(j==-1) {printf("updateSpd() j==-1 \n");}// ok
		
		float1 dx=len-vv_len[f1][j];
		if(fabs(dx)<0.001) dx=0.0;

//		vec3 F=nor*(D*dx);// test
		vec3 F=nor*(dx);		
		accs[f1]+=F;//  F2*dt/mass
		accs[f2]-=F;
	}
	loopd(i,0,spds.size(),1)
	{
		float1 l1=length(accs[i])-0.3;
		if(l1>0.0)
		{
			pnts[i]+=normalize(accs[i])*l1*0.1;
		}
		accs[i]=vec3(0,0,0);
		

	}
}
#else
void c_mesh::updateSpd(float1 dt,float1 D,float1 K)  // v1.0
{

	loopd(i,0,edges.size(),1)
	{
		int f1=edges[i].a;
		int f2=edges[i].b;

		vec3 nor=pnts[f2] - pnts[f1];
		vec3 relSpd=spds[f2]-spds[f1];
		float1 len=length(nor);
		nor=normalize(nor);
	
		int j=-1;
		loopd(k,0,vv_links[f1].size(),1) if(vv_links[f1][k]==f2) {j=k;break;}
		if(j==-1) {printf("updateSpd() j==-1 \n");}// ok
		
		float1 dx=len-vv_len[f1][j];
		if(fabs(dx)<0.001) dx=0.0;

#if 1		
		vec3 a=nor*(D*dx + dot(relSpd,nor)*K); //  D=stiffness   K=?           ha ossze megy -         D-ben van az *dt/mass
//		vec3 F=nor*(D*dx);// test
		accs[f1]+=a;//  F2*dt/mass
		accs[f2]-=a;
#else		
		vec3 F=nor*(D*dx );		
		vec3 F2=F- nor*dot(spds[f1],nor)*K;
		vec3 F3=F- nor*dot(spds[f2],nor)*K;
		accs[f1]+=F2;//  F2*dt/mass
		accs[f2]-=F3;
#endif		
	}
	loopd(i,0,spds.size(),1)
	{
		spds[i]+=accs[i]*dt;
		pnts[i]+=spds[i]*dt;
		accs[i]=vec3(0,0,0);
		spds[i]=vec3(0,0,0);
		
//		spds[i]-=spds[i]*dt*10.5;// 1s elvesz  20  10  TODO   5keves!        ????????? 12     1 keves
	}
}

#endif



//##############################################################################3
void c_mesh::savepnts(char *filename)
{
QDUMPFNC();
printf("savepnts %s \n",filename);
	qfile *ff=new qfile(filename,"wb");


	ff->qwrite3f("wcenter",wcenter.x,wcenter.y,wcenter.z);
	ff->qwrite3f("wcenter2",wcenter2.x,wcenter2.y,wcenter2.z);
	ff->qwrite1f("metr",metr);

	ff->qwrite1i("vertex_size",pnts.size());
	loopd(i,0,pnts.size(),1)	ff->qwrite3f("vertex",pnts[i].x,pnts[i].y,pnts[i].z);


	ff->qwrite1i("txcoord_size",txcoord.size());
	loopd(i,0,txcoord.size(),1)	ff->qwrite3f("txcoord",txcoord[i].x,txcoord[i].y,txcoord[i].z);


	ff->qwrite1i("face_size",faces.size());
	loopd(i,0,faces.size(),3)	ff->qwrite3i("face",faces[i],faces[i+1],faces[i+2]);

			
	ff->qwrite1i("type_size",types.size());
	loopd(i,0,types.size(),3)	ff->qwrite3i("type",types[i],types[i+1],types[i+2]);
	
	delete ff;

}
void c_mesh::loadpnts(const char *filename,const char *txname,int op,float1 scl)
{
QDUMPFNC();

	is_carrier=0;
	if(strstr(filename,"carrier")) is_carrier=1;

#ifdef __AIRPLANE
#ifdef _SU35
	wcenter=vec3(921, 453, 1206);//	(904, 453, 1223);  (936, 453, 1234)   wheel+wing=(1016, 453, 1206)
	wcenter2=vec3(921, 453+3, 1206-388);
	metr=57.0;//meret
#else	
//lines          red green blue
	wcenter=vec3(1151, 322, 895);//	1151, 322, 895
	wcenter2=vec3(1151, 792, 508);//1151, 792, 508
	                  //purple ltblue
	metr=50.0;//meret
#endif	
#else//CARRIER
	wcenter=vec3(921-107, 453-96, 1206-525);
	wcenter2=vec3(921, 453+3+57*12-10, 1206-388-200-4*57-5);
	metr=57.0;//meret
#endif	
	int64_t txi=qhash64(txname);
	txc_center=wcenter/vec3(textures[txi].xh,textures[txi].yh,textures[txi].yh);
	
	pnts.clear();
	txcoord.clear();
	faces.clear();
	faces_LOD.clear();
	types.clear();
		

	qfile *ff=new qfile(filename,"rb");
	if(ff->ff==NULL) return;
	
	ff->qread3f("wcenter",wcenter.x,wcenter.y,wcenter.z);
	ff->qread3f("wcenter2",wcenter2.x,wcenter2.y,wcenter2.z);
	ff->qread1f("metr",metr);

	int n=0;
	ff->qread1i("vertex_size",n); pnts.resize(n);
	loopd(i,0,pnts.size(),1)	ff->qread3f("vertex",pnts[i].x,pnts[i].y,pnts[i].z);


	ff->qread1i("txcoord_size",n); txcoord.resize(n);
	loopd(i,0,txcoord.size(),1)	ff->qread3f("txcoord",txcoord[i].x,txcoord[i].y,txcoord[i].z);


	ff->qread1i("face_size",n); faces.resize(n);
	loopd(i,0,faces.size(),3)	ff->qread3i("face",faces[i],faces[i+1],faces[i+2]);

			
	ff->qread1i("type_size",n); types.resize(n);
	loopd(i,0,types.size(),3)	ff->qread3i("type",types[i],types[i+1],types[i+2]);



	printf("loadpnts %s  %d %d %d %d \n",filename,pnts.size(),txcoord.size(),faces.size(),types.size());
	



#if 0
NA
// tobb haromszog
//	loopd(j,0,2,1)//sok
	{
	qvector<int> faces5;
	qvector<int> types5;
	loopd(i,0,faces.size(),3)
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		vec3 p4=(pnts[f1]+pnts[f2]+pnts[f3])/3.0;
		vec3 txc4=(txcoord[f1]+txcoord[f2]+txcoord[f3])/3.0;
		int f4=pnts.size();
		
		pnts.push_back(p4); // new verts
		txcoord.push_back(txc4);
//		vtypes.push_back(vtypes[f1]);
		
		faces5.push_back(f1);//new 3 faces
		faces5.push_back(f2);
		faces5.push_back(f4);
		types5.push_back(types[i]);
		types5.push_back(types[i]);
		types5.push_back(types[i]);

		faces5.push_back(f2);
		faces5.push_back(f3);
		faces5.push_back(f4);
		types5.push_back(types[i]);
		types5.push_back(types[i]);
		types5.push_back(types[i]);

		faces5.push_back(f3);
		faces5.push_back(f1);
		faces5.push_back(f4);
		types5.push_back(types[i]);
		types5.push_back(types[i]);
		types5.push_back(types[i]);
	}
	faces=faces5;	faces5.clear();
	types=types5;	types5.clear();
	}
#endif




//vert type from face type,  and new vert if type!=type 
	vtypes.resize(pnts.size());
	loopd(i,0,vtypes.size(),1) vtypes[i]=-1;
	
	loopd(i,0,faces.size(),3)
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
		
		if(vtypes[f1]!=-1)
		if(vtypes[f1]!=types[i])//new vert, ha mas tipusu /new mesh?/
		{
			int f1b=pnts.size();
			faces[i]=f1b;
			pnts.push_back(pnts[f1]);
			txcoord.push_back(txcoord[f1]);
			vtypes.push_back(types[i]);
			f1=f1b;
		}
		if(vtypes[f2]!=-1)
		if(vtypes[f2]!=types[i])//new vert
		{
			int f2b=pnts.size();
			faces[i+1]=f2b;
			pnts.push_back(pnts[f2]);
			txcoord.push_back(txcoord[f2]);
			vtypes.push_back(types[i]);
			f2=f2b;
		}
		if(vtypes[f3]!=-1)
		if(vtypes[f3]!=types[i])//new vert
		{
			int f3b=pnts.size();
			faces[i+2]=f3b;
			pnts.push_back(pnts[f3]);
			txcoord.push_back(txcoord[f3]);
			vtypes.push_back(types[i]);
			f3=f3b;
		}
		if(types[i]&32)//elso-hatso kerek  0x20 0x40
		{
			if(pnts[f1].x<-1.0) 
			{
//				types[i]^=32;			types[i]|=64;
			}
		}
		vtypes[f1]=types[i];//3 bol 1 ervenyes
		vtypes[f2]=types[i];
		vtypes[f3]=types[i];//felulirhatja!
	}


#ifndef _EDITOR
if(txcoord.size()==0)//autogen
#endif
{
	txcoord.clear();
	loopd(i,0,pnts.size(),1)
	{
		vec3 v1=pnts[i]; 
		
		v1.x=-v1.x;//transf
		v1.z=-v1.z;
		v1*=metr;//scl
		v1+=wcenter;
		
		v1/=vec3(textures[txi].xh,textures[txi].yh,textures[txi].yh);
		txcoord.push_back(v1);
	}
}
	txcoord2.clear();
	loopd(i,0,pnts.size(),1)
	{
		vec3 v1=pnts[i]; 
		
		v1.x=-v1.x;//transf
		v1.z=-v1.z;
		v1*=metr;//scl
		v1+=wcenter2;
		
		v1/=vec3(textures[txi].xh,textures[txi].xh,textures[txi].yh);
//	v1=vec3(0,0,0);//test
		txcoord2.push_back(v1);
	}

	if(scl!=1.0)	loopd(i,0,pnts.size(),1) pnts[i]*=scl;








	if(!is_carrier)//mirror of half of airplane
	{
		pnts_mir_base=pnts.size();
		faces_mir_base=faces.size();
		
		loopd(i,0,pnts_mir_base,1)
		{
			vec3 v1=pnts[i];
			v1.y=-v1.y;
			pnts.push_back(v1);// ex pnts_mir
		}

		loopd(i,0,faces_mir_base,1)		faces.push_back(faces[i]+pnts_mir_base);

		int nn=txcoord.size();	loopd(i,0,nn,1)		txcoord.push_back(txcoord[i]);
		nn=types.size();		loopd(i,0,nn,1)		types.push_back(types[i]|8192);
		nn=vtypes.size();		loopd(i,0,nn,1)		vtypes.push_back(vtypes[i]|8192);//mirrored
		
		printf("mesh Mirrorred\n");
	}

	faces_LOD.resize(faces.size()); // 3x nagyobb de sebaj
	loopd(i,0,faces.size(),3)
	{
		int v1=faces[i];
		int v2=faces[i+1];
		int v3=faces[i+2];
		
		float1 t1=length(pnts[v2]-pnts[v1]);
		float1 t2=length(pnts[v3]-pnts[v2]);
		float1 t3=length(pnts[v1]-pnts[v3]);

		if(t2>t1) t1=t2;
		if(t3>t1) t1=t3;
		
		faces_LOD[i]=t1;
	}


		
	if(op)//ha nem editor
	{
		float1 all=0,all1=0,all2=0,all3=0,all4=0,all5=0,all6=0,all7=0;	
		vec3 mmin(1e16,1e16,1e16),mmax(-1e16,-1e16,-1e16);
		

		loopd(i,0,faces.size(),3)//triangles
		if((i+2)<faces.size())
		{
			int v1=faces[i];
			int v2=faces[i+1];
			int v3=faces[i+2];

			if((v1>=pnts.size() || v1<0) ||(v2>=pnts.size() || v2<0) ||(v3>=pnts.size() || v3<0) )
			{
				faces[i]=0;// ignore
				faces[i+1]=0;
				faces[i+2]=0;
				v1=v2=v3=0;
			}
			
/*			if(v1>=pnts.size() || v1<0)
			{
				pnts.push_back(vec3(500,500,1200));
				v1=pnts.size()-1;
				faces[i]=v1;
			}
			if(v2>=pnts.size() || v2<0)
			{
				pnts.push_back(vec3(500,500,1200));
				v2=pnts.size()-1;
				faces[i+1]=v2;
			}
			if(v3>=pnts.size() || v3<0)
			{
				pnts.push_back(vec3(500,500,1200));
				v3=pnts.size()-1;
				faces[i+2]=v3;
			}*/

			float1 A=tri_area(pnts[v1],pnts[v2],pnts[v3]);
//			float1 A22=tri_area2(pnts[v1],pnts[v2],pnts[v3]);printf("%e \n",A/A22);			
			vec3 w1=pnts[v1], w2=pnts[v2], w3=pnts[v3];  w1.x=0;  w2.x=0;  w3.x=0;
			float1 A2=tri_area(w1,w2,w3);
			w1=pnts[v1], w2=pnts[v2], w3=pnts[v3];  w1.y=0;  w2.y=0;  w3.y=0;
			float1 A3=tri_area(w1,w2,w3);
			w1=pnts[v1], w2=pnts[v2], w3=pnts[v3];  w1.z=0;  w2.z=0;  w3.z=0;
			float1 A4=tri_area(w1,w2,w3);
			vec3 cent=(pnts[v1] + pnts[v2] + pnts[v3])/3.0;
			vec3 norm=normalize(cross(pnts[v3]-pnts[v1],pnts[v2]-pnts[v1]));
			
			areas.push_back(A);
			normals.push_back(norm);
			tricenters.push_back(cent);
			forces[0].push_back(1.0);
			forces[1].push_back(1.0);
			
			chk_bbox5(pnts[v1],mmin,mmax);
			chk_bbox5(pnts[v2],mmin,mmax);
			chk_bbox5(pnts[v3],mmin,mmax);

			
			all+=A;
			all4+=A2;
			all5+=A3;
			all6+=A4;
			if(types[i]&2) all1+=A;
			if(types[i]&4) all2+=A;
			if(types[i]&8) all3+=A;
			if(types[i]&1) all7+=A;
		}
		wcenter/=metr;//kell ha nem editor
#if 0
//TODO		
		printf("%e coll tris\n",all);
		printf("%e %e %e \n",mmin.x,mmin.y,mmin.z);
		printf("%e %e %e \n",mmax.x,mmax.y,mmax.z);
		printf("%e %e %e \n",mmax.x-mmin.x,mmax.y-mmin.y,mmax.z-mmin.z);
		printf("%e %e %e %e   , %e %e %e AREA\n",all1,all2,all3,all7,all4,all5,all6);
#endif		
		printf("mm: %e %e %e \n",mmax.x-mmin.x,mmax.y-mmin.y,mmax.z-mmin.z);
	}
	//x hossz y kereszbe   z magassag
//40-41 88  pnts wings   453 mirror Y   wcenter 904 453 1223

	chk_normal_occlusion();//mindig!
	

//make edges
	loopd(i,0,faces.size(),3)//triangles   
	if((types[i]&32)==0 &&(types[i]&64)==0  &&(types[i]&4096)==0 )		//wheel nem  hook
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];

		add_edge(f1,f2);		
		add_edge(f2,f3);		
		add_edge(f3,f1);		
	}
	
	
//make linkage
	vf_links.resize(pnts.size());// ahany verts
	vv_links.resize(pnts.size());// ahany verts
	vv_len.resize(pnts.size());// ahany verts
	f_links.resize(faces.size());// ahany face
	
	loopd(i,0,faces.size(),3)//triangles   
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];

		vf_links[f1].push_back(i);//  faces of vert
		vf_links[f2].push_back(i);
		vf_links[f3].push_back(i);
	}
	loopd(i,0,vf_links.size(),1)
	loopd(i2,0,vf_links[i].size(),1) // neigh verts of vert
	{
		int ii=vf_links[i][i2];
		int f1=faces[ii];
		int f2=faces[ii+1];
		int f3=faces[ii+2];
		
		int ok1=1,ok2=1,ok3=1;
		loopd(j,0,vv_links[i].size(),1) 
		{
			if(vv_links[i][j]==f1) ok1=0;// mar van
			if(vv_links[i][j]==f2) ok2=0;
			if(vv_links[i][j]==f3) ok3=0;
		}
		
		if(i!=f1) if(ok1) {vv_links[i].push_back(f1);vv_len[i].push_back(length(pnts[f1]-pnts[i]));}// sajat magat ne
		if(i!=f2) if(ok2) {vv_links[i].push_back(f2);vv_len[i].push_back(length(pnts[f2]-pnts[i]));}
		if(i!=f3) if(ok3) {vv_links[i].push_back(f3);vv_len[i].push_back(length(pnts[f3]-pnts[i]));}
	}
	loopd(i,0,faces.size(),3)// neigh faces of face
	{
		int f1=faces[i];
		int f2=faces[i+1];
		int f3=faces[i+2];
//                                             onmagat ne!
		loopd(j,0,vf_links[f1].size(),1) if(vf_links[f1][j]!=i) f_links[i].push_back(vf_links[f1][j]);// neigh faces of face
		loopd(j,0,vf_links[f2].size(),1) if(vf_links[f2][j]!=i) f_links[i].push_back(vf_links[f2][j]);
		loopd(j,0,vf_links[f3].size(),1) if(vf_links[f3][j]!=i) f_links[i].push_back(vf_links[f3][j]);
	}// f_links[x][]  3-assaval!! mint a face
	

	spds.resize(pnts.size());// ahany verts
	accs.resize(pnts.size());// ahany verts
	


	kdt.build_kdtree(pnts,faces,types);
	kdt.set_tramat(&tramat);

	printf("loadpnts %s  %d %d %d %d \n",filename,pnts.size(),txcoord.size(),faces.size(),types.size());
	
	delete ff;
}

/*
void c_mesh::loadpnts_old(int op)
{
op=1;
	wcenter=vec3(921, 453, 1206);//	(904, 453, 1223);  (936, 453, 1234)   wheel+wing=(1016, 453, 1206)
	wcenter2=vec3(921, 453+3, 1206-388);
	metr=57.0;//meret
//x: 660-1000
//z: 1277-1185  fejjel
	//wcenter=vec3(904, 453, 1226);//old  nem megemelheto
//	wcenter=vec3(921, 453, 1206);//	(904, 453, 1223);  (936, 453, 1234)   wheel+wing=(1016, 453, 1206)
#ifdef _NEWPHY
//wcenter=vec3(1001, 453, 1206);//890-960       453 oldal        magassag 1280 1180 felfele
wcenter=vec3(951, 453, 1226);//wcenter=vec3(951, 453, 1226);	!!	   970 elfujja!  920 nem lehet felhuzni
#endif

	int64_t txi=qhash64("tx43");
	txc_center=wcenter/vec3(textures[txi].xh,textures[txi].yh,textures[txi].yh);
	
		pnts.clear();
		txcoord.clear();
		faces.clear();
		types.clear();
		
		FILE *ff=fopen("data/phy.dat","rb");
		int n=0;
		fread(&n,4,1,ff);
		loopd(i,0,n,1)
		{
			vec3 w1;
			fread(&w1.x,4,1,ff);
			fread(&w1.y,4,1,ff);
			fread(&w1.z,4,1,ff);
			
			vec3 t1=w1; 
			t1/=vec3(textures[txi].xh,textures[txi].yh,textures[txi].yh);
			txcoord.push_back(t1);
			
			if(op)
			{
				w1-=wcenter;
				w1/=metr;
				w1.x=-w1.x;
				w1.z=-w1.z;
			}
			pnts.push_back(w1);
		}
#if 1
//del faces
		fread(&n,4,1,ff);
		loopd(i,0,n,1)
		{
			int f=0;
			fread(&f,4,1,ff);
			faces.push_back(f);
		}

//		loopd(i,0,faces.size(),1)		types.push_back(0);//clr

		fread(&n,4,1,ff);
		loopd(i,0,n,1)
		{
			int f=0;
			fread(&f,4,1,ff);
			types.push_back(f);
		}
#endif
		fclose(ff);




		
	if(op)
	{
		float1 all=0,all1=0,all2=0,all3=0,all4=0,all5=0,all6=0,all7=0;	
		vec3 mmin(1e6,1e6,1e6),mmax(-1e6,-1e6,-1e6);
		

		loopd(i,0,faces.size(),3)//triangles
		if((i+2)<faces.size())
		{
			int v1=faces[i];
			int v2=faces[i+1];
			int v3=faces[i+2];

			
			if(v1>=pnts.size() || v1<0)
			{
				pnts.push_back(vec3(500,500,1200));
				v1=pnts.size()-1;
				faces[i]=v1;
			}
			if(v2>=pnts.size() || v2<0)
			{
				pnts.push_back(vec3(500,500,1200));
				v2=pnts.size()-1;
				faces[i+1]=v2;
			}
			if(v3>=pnts.size() || v3<0)
			{
				pnts.push_back(vec3(500,500,1200));
				v3=pnts.size()-1;
				faces[i+2]=v3;
			}

			float1 A=tri_area(pnts[v1],pnts[v2],pnts[v3]);
			vec3 w1=pnts[v1], w2=pnts[v2], w3=pnts[v3];  w1.x=0;  w2.x=0;  w3.x=0;
			float1 A2=tri_area(w1,w2,w3);
			w1=pnts[v1], w2=pnts[v2], w3=pnts[v3];  w1.y=0;  w2.y=0;  w3.y=0;
			float1 A3=tri_area(w1,w2,w3);
			w1=pnts[v1], w2=pnts[v2], w3=pnts[v3];  w1.z=0;  w2.z=0;  w3.z=0;
			float1 A4=tri_area(w1,w2,w3);
			vec3 cent=(pnts[v1] + pnts[v2] + pnts[v3])/3.0;
			vec3 norm=normalize(cross(pnts[v3]-pnts[v1],pnts[v2]-pnts[v1]));
			
			areas.push_back(A);
			normals.push_back(norm);
			tricenters.push_back(cent);
			forces[0].push_back(1.0);
			forces[1].push_back(1.0);
			
			chk_bbox5(pnts[v1],mmin,mmax);
			chk_bbox5(pnts[v2],mmin,mmax);
			chk_bbox5(pnts[v3],mmin,mmax);
			
			all+=A;
			all4+=A2;
			all5+=A3;
			all6+=A4;
			if(types[i]&2) all1+=A;
			if(types[i]&4) all2+=A;
			if(types[i]&8) all3+=A;
			if(types[i]&1) all7+=A;
		}
//wcenter/=metr;!
#if 0
//TODO
		printf("%e coll tris\n",all);
		printf("%e %e %e \n",mmin.x,mmin.y,mmin.z);
		printf("%e %e %e \n",mmax.x,mmax.y,mmax.z);
		printf("%e %e %e \n",mmax.x-mmin.x,mmax.y-mmin.y,mmax.z-mmin.z);
		printf("%e %e %e %e   , %e %e %e AREA\n",all1,all2,all3,all7,all4,all5,all6);
#endif		
	}
	//x hossz y kereszbe   z magassag
//40-41 88  pnts wings   453 mirror Y   wcenter 904 453 1223
	
}
*/


//--------------------------------------------------------------------------------

void preloadlevel(int op,int extx)//extx=-1
{
	{
	FILE *ff=fopen("level.txt","rb");
	//char *sellevel="sp";//w r sf    m h bb g sp l
	if(ff==NULL) printf("ERROR level.txt\n");
	fread(sellevel,100,1,ff);
	fclose(ff);
	int i=0;while(sellevel[i]!=13 &&sellevel[i]!=10) i++;
	sellevel[i]=0;
	}
//	if(sellevel[0]=='h') {carrier=1;}//frankfurt	
	
}
void loadlevel(int op,int extx,int needgmap)//extx=-1,int needgmap=1
{
QDUMPFNC();
if(sellevel[0]=='a') ratio= 50.0/65.0;//x meter 1 pixel
//if(sellevel[0]=='b') ratio= 50.0/124.0;//deb
if(sellevel[0]=='c') ratio= 50.0/120.0;
if(sellevel[0]=='d') ratio= 50.0/65.0;
if(sellevel[0]=='e') ratio= 50.0/70.0;
if(sellevel[0]=='f') ratio= 100.0/120.0;
if(sellevel[0]=='y') ratio= 100.0/110.0;
//if(sellevel[0]=='s') ratio= 100.0/106.0;//sf
if(sellevel[0]=='r') ratio= 50.0/70.0;//berl 2
if(sellevel[0]=='w') ratio= 100.0/110.0;
if(sellevel[0]=='z') ratio= 50.0/120.0;

if(sellevel[0]=='m') {ratio= 20.0/50.0;hmap=1;}//munich
if(sellevel[0]=='h') {ratio= 20.0/104.0;hmap=1;}//frankfurt
if(sellevel[0]=='b') {ratio= 20.0/55.0;hmap=1;}//berlin
if(sellevel[0]=='g') {ratio= 500.0/67.0;hmap=1;}//greek
if(sellevel[0]=='s') {ratio= 100.0/113.0;hmap=1;}//spyl
if(sellevel[0]=='l') {ratio= 100.0/104.0;hmap=1;}//las vegas
if(sellevel[0]=='i') {ratio= 20.0/99.0;hmap=1;}//insbruck
if(sellevel[0]=='k') {ratio= 20.0/100.0;hmap=1;}//korzika ~



	if(hmap) xmap=2800;
	ratio*=(float1)xmap;//meter szeles a map

#ifdef _USEGMAP
	if(needgmap)
	loopd(i,1,11,1)//9  10  11 10
	if(op==1 || i==extx)
	{
		char ss[92];
		if(hmap) sprintf(ss,"gmaph/%s%d.bmp",sellevel,i);
		else     sprintf(ss,"gmap/%s%d.bmp",sellevel,i);
if(carrier)
if(i==2)	sprintf(ss,"gmaph/%s%dc2.bmp",sellevel,i);//carrier

		loadbmpb("tx"+int2string(i),ss,1);//1280,1024,
	}
	{
	char ss[92];
	sprintf(ss,"data/bump_%s.bmp",sellevel);
	if(needgmap)
	loadbmpb("tx40",ss,0,1);//     l=01   m=03  nocut!
	}
	int64_t txi=qhash64("tx40");
//	if(textures[txi].xh==0) emount=0;


	loadbmpb("tx47","gmaph/h22.bmp",0,1);
#endif
	printf("level: %s %e %d\n",sellevel,ratio,hmap);//ratio = texture wide in meter
}

/*
			if(key=='1')	mesh.types[selface]|=1;//2 side  ,wing
			if(key=='2')	mesh.types[selface]|=2;//wings
			if(key=='3')	mesh.types[selface]|=4;//hatso wing
			if(key=='4')	mesh.types[selface]|=8;
			if(key=='5')	mesh.types[selface]|=16;//mappolas
			if(key=='6')	mesh.types[selface]|=32;//wheel
			if(key=='7')	mesh.types[selface]|=64;//wheelhatso
			if(key=='8')	mesh.types[selface]|=128;//flaps new hatul
//			if(key=='9')	mesh.types[selface]|=1024;//flaps new2 elol
//			if(key=='9')	mesh.types[selface]|=2048;//speed brake
			if(key=='9')	mesh.types[selface]|=4096;//hook
			// 8192 mirrored
*/


