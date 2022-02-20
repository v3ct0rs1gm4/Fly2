/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//---------------------------------------------------------------------------

#include "q_objloader.h"

// blender export obj triangulate	  ON !! TODO



void q_mesh2::setmat(axis4 ax) 
{
	mat.x=ax.x;
	mat.y=ax.y;
	mat.z=ax.z;
	mat.w=ax.w;
	hasmatrix=1;
}
void q_mesh2::getpivot(vec3 *ax,vec3 *origin)
{
	*ax=pivot_ax;
	*origin=pivot_pos;
}
void q_mesh2::setpivot()
{
	float lmax=0,center;
	vec3 p1,p2,a1,a2;
	int cc1=0,cc2=0;
			
	for(int i=0;i<n_faces;i+=3)
	{
		int f1=faces1[i];
		int f2=faces1[i+1];
		int f3=faces1[i+2];
		vec3 v1=vert[f1];
		vec3 v2=vert[f2];
		vec3 v3=vert[f3];
				
		float l1=length(v2-v1);
		float l2=length(v3-v2);
		float l3=length(v1-v3);
				
		if(l1>lmax) {lmax=l1;p1=v1;p2=v2;}
		if(l2>lmax) {lmax=l2;p1=v2;p2=v3;}
		if(l3>lmax) {lmax=l3;p1=v3;p2=v1;}
	}
	for(int i=0;i<n_vert;i++)
	{
		float t1=length(vert[i]-p1);
		float t2=length(vert[i]-p2);
		
		if(t1<t2)
		{
			a1+=vert[i];cc1++;
		}
		else
		{
			a2+=vert[i];cc2++;
		}
	}
	a1/=cc1;
	a2/=cc2;
//a1=p1;a2=p2;	 // test
	pivot_pos=(a1+a2)/2.0; //a2
	pivot_ax=normalize(a1-a2);
}			
vec3 q_mesh2::getpivotTRA(axis4 *ax)
{
	vec4 v4;
	v4=ax->x*pivot_pos.x +ax->y*pivot_pos.y +ax->z*pivot_pos.z +ax->w; 
	
	return vec3(v4.x,v4.y,v4.z);
}			

void q_mesh2::transf(vec3 axis) 
{
	if(hasmatrix==0) 	printf("unused mesh [hasnomatrix] [%s] \n",name.c_str());
	
	
	vec4 v4;
	if(vert_tra.size()!=vert.size()) vert_tra.resize(vert.size());
	if(norm_tra.size()!=norm.size()) norm_tra.resize(norm.size());
	if(face_normal_tra.size()!=face_normal.size()) face_normal_tra.resize(face_normal.size());

	float1 ymin=1e16;	
	bbox.reset();
	for(int i=0;i<n_vert;i++)
	{
		v4=mat.x*vert[i].x +mat.y*vert[i].y +mat.z*vert[i].z +mat.w; 
		vert_tra[i].x=v4.x;
		vert_tra[i].y=v4.y;
		vert_tra[i].z=v4.z;
		
		if(v4.y<ymin) {ymin=v4.y;vertymin=vert_tra[i];}// legalso pnt

//		bbox.add_vec3(vert_tra[i]);
	}
	for(int i=0;i<n_norm;i++)
	{
		v4=mat.x*norm[i].x +mat.y*norm[i].y +mat.z*norm[i].z; 
		norm_tra[i].x=v4.x;
		norm_tra[i].y=v4.y;
		norm_tra[i].z=v4.z;
	}
	
	v4=mat.x*pivot_pos.x +mat.y*pivot_pos.y +mat.z*pivot_pos.z +mat.w; 
	pivot_pos_tra.x=v4.x;
	pivot_pos_tra.y=v4.y;
	pivot_pos_tra.z=v4.z;


#if 1
// newnewphy
	float tmin=1e16,tmax=1e-16;
	
	
	if(face_color.size()!=faces1.size()) face_color.resize(faces1.size());
	if(face_center.size()!=faces1.size()) face_center.resize(faces1.size());
//	if(face_order.size()!=faces1.size()) face_order.resize(faces1.size());

	for(int i=0;i<faces1.size();i+=3)
	{
		int f1=faces1[i];
		int f2=faces1[i+1];
		int f3=faces1[i+2];

		vec3 v1=vert_tra[f1];
		vec3 v2=vert_tra[f2];
		vec3 v3=vert_tra[f3];
		face_center[i]=(v1+v2+v3)/3.0;
#if 0
		vec3 local=face_center[i] -vec3(mat.w.x,mat.w.y,mat.w.z);
		float t1=dot(axis,local); //  szel iranyu face sorrend
		if(t1>tmax) tmax=t1;
		if(t1<tmin) tmin=t1;
		face_order[i]=t1;
#endif
		v4=mat.x*face_normal[i].x +mat.y*face_normal[i].y +mat.z*face_normal[i].z; 
		face_normal_tra[i].x=v4.x;
		face_normal_tra[i].y=v4.y;
		face_normal_tra[i].z=v4.z;
	}
#if 0	
	for(int i=0;i<face_order.size();i+=3)
	{
		face_order[i]=(1.0-(face_order[i]-tmin)/(tmax-tmin))*1.0+1.0; // 0-1
	}
#endif	
#endif	
}


void q_mesh2::draw(vec3 pos,int gear) // render
{
	if((visible&1)==0) return;
	if(gear==0 && strstr(name.c_str(),"wheel"))  return;// gear  skip

	vec4 col44(1.0,1.0,0.0,1.0);

	
	glBegin2(GL_TRIANGLES);
//	glBegin2(GL_QUADS);

	for(int i=0;i<faces1.size();i+=3)//n_faces
	{
		int f1=faces1[i];
		int f2=faces1[i+1];
		int f3=faces1[i+2];
		if(f1>=vert.size()) continue;		if(f2>=vert.size()) continue;		if(f3>=vert.size()) continue;		
		vec3 v1=vert_tra[f1];
		vec3 v2=vert_tra[f2];
		vec3 v3=vert_tra[f3];

		int ft1=faces2[i];
		int ft2=faces2[i+1];
		int ft3=faces2[i+2];
		if(ft1>=txcoord.size()) continue;		if(ft2>=txcoord.size()) continue;		if(ft3>=txcoord.size()) continue;		
		vec3 txc1=txcoord[ft1];
		vec3 txc2=txcoord[ft2];
		vec3 txc3=txcoord[ft3];
		
		int fn1=faces3[i];
		int fn2=faces3[i+1];
		int fn3=faces3[i+2];
		if(fn1>=norm.size()) continue;		if(fn2>=norm.size()) continue;		if(fn3>=norm.size()) continue;	
		vec3 n1=norm_tra[fn1];
		vec3 n2=norm_tra[fn2];
		vec3 n3=norm_tra[fn3];

		col44=face_color[i];
		
		glMultiTexCoord2f(0,txc1.x,txc1.y);		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,n1.x,n1.y,n1.z);		
		glVertex3f(v1.x,v1.y,v1.z);

		glMultiTexCoord2f(0,txc2.x,txc2.y);		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,n2.x,n2.y,n2.z);		
		glVertex3f(v2.x,v2.y,v2.z);

		glMultiTexCoord2f(0,txc3.x,txc3.y);		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord3f(2,n3.x,n3.y,n3.z);		
		glVertex3f(v3.x,v3.y,v3.z);

	}
	glEnd2();
}
void q_mesh2::makelink()
{
	linked.resize(n_faces/3);
	wind.resize(n_faces/3);// a feluleten aramlik a levego a szomszed haromszogek fele!  csillapitva
	tri_area3.resize(n_faces);
	face_normal.resize(n_faces);
	
	for(int i=0;i<n_faces;i+=3)
	{
		int f1=faces1[i];
		int f2=faces1[i+1];
		int f3=faces1[i+2];
		
		for(int j=0;j<n_faces;j+=3) // melyik face a szomszed
		if(i!=j)
		{
			int l1=faces1[i];
			int l2=faces1[i+1];
			int l3=faces1[i+2];
			
			if(f1==l1 ||
			   f1==l2 ||
			   f1==l3 || 
			   f2==l1 ||
			   f2==l2 ||
			   f2==l3 || 
			   f3==l1 ||
			   f3==l2 ||
			   f3==l3 )		linked[i/3].push_back(j);
		}
		
		vec3 v1=vert[f1];
		vec3 v2=vert[f2];
		vec3 v3=vert[f3];
		tri_area3[i]=tri_area(v1,v2,v3);
		if(f1==f2) tri_area3[i]=0.0; 
		if(f2==f3) tri_area3[i]=0.0; 
		if(f3==f1) tri_area3[i]=0.0; 

		wind[i/3]=vec3(0,0,0);

		int fn1=faces3[i];
		vec3 normal=normalize(cross(v3-v1,v2-v1  ));
		if(dot(normal,norm[fn1])<0.0) normal=-normal;

		face_normal[i]=normal;		
	}
}

		

		




void objfile::draw(vec3 pos,int gear)
{
	for(auto &mesh:meshs)	mesh.draw(pos,gear);
}
//iiddee

void objfile::OBJparser()
{
	int selface=0,selnface=0;
	int base1=0,base2=0,base3=0; // vert/txc/norm kulon face index + minden index global!
	int i1max=0,i2max=0,i3max=0;
	q_mesh2 *mesh=0;
	
	while(read_word())
	{
		if(strcmp(word2,"o")==0)
		{
			if(mesh)
			{
				ni+=mesh->faces1.size()/3;
				nv+=mesh->vert.size();
				
				if(strstr(mesh->name.c_str(),"pivot_"))  cc2+=mesh->faces1.size()/3;
				else cc1+=mesh->faces1.size()/3;

				
				base1+=mesh->vert.size();
				base2+=mesh->txcoord.size();
				base3+=mesh->norm.size();

				mesh->n_faces  =mesh->faces1.size();
				mesh->n_vert   =mesh->vert.size();
				mesh->n_txcoord=mesh->txcoord.size();
				mesh->n_norm=mesh->norm.size();

				printf("faces: %d ",mesh->n_faces/3);
				printf("verts: %d/%d ",mesh->n_vert,i1max);
				printf("txc:   %d/%d ",mesh->n_txcoord,i2max);
				printf("norms: %d/%d ",mesh->n_norm,i3max);
				printf("\n");
			}

			int selmesh=meshs.size();
			meshs.resize(meshs.size()+1);
			mesh=&meshs[selmesh];
			mesh->obj=this;
	
			read_word();
			printf("obj A: %s \n",word2);		
			{
				int j=0;	while(word2[j] && word2[j]!='.') j++; // cut extra name  _Cube _Cylinder.001  _Torus.004 ,  de lehet .002 nelkuli
				while(j>0 && word2[j]!='_') j--;
				word2[j]=0;
			}// 

			mesh->name=word2;
			printf("obj B: %s ",mesh->name.c_str());		

			i1max=0;
			i2max=0;
			i3max=0;
		}
	
		if(strcmp(word2,"v")==0)
		{
			vec3 w1;
			w1.x=read_dd()*gscale;
			w1.y=read_dd()*gscale;
			w1.z=read_dd()*gscale;

			mesh->vert.push_back(w1);
			bbox.add_vec3(w1);
			mesh->bbox.add_vec3(w1);
		}
		if(strcmp(word2,"vt")==0)
		{
			vec3 w1;
			w1.x=read_dd();
			w1.y=read_dd();

			mesh->txcoord.push_back(w1);
		}
		if(strcmp(word2,"vn")==0)
		{
			vec3 w1;
			w1.x=read_dd();
			w1.y=read_dd();
			w1.z=read_dd();

			mesh->norm.push_back(w1);
		}

		if(strcmp(word2,"f")==0)
		{
		 // es a hlye 1 tol indexel     -base-1
		 	int i1=read_int()-base1-1;
		 	int i2=read_int()-base2-1;
		 	int i3=read_int()-base3-1;
		 	if(i1>i1max) i1max=i1;
		 	if(i2>i2max) i2max=i2;
		 	if(i3>i3max) i3max=i3;
			mesh->faces1.push_back(i1);	 // vert/txc/norm
			mesh->faces2.push_back(i2);	
			mesh->faces3.push_back(i3);	
		
		 	i1=read_int()-base1-1;
		 	i2=read_int()-base2-1;
		 	i3=read_int()-base3-1;
		 	if(i1>i1max) i1max=i1;
		 	if(i2>i2max) i2max=i2;
		 	if(i3>i3max) i3max=i3;
			mesh->faces1.push_back(i1);	// vert/txc/norm
			mesh->faces2.push_back(i2);	
			mesh->faces3.push_back(i3);	

		 	i1=read_int()-base1-1;
		 	i2=read_int()-base2-1;
		 	i3=read_int()-base3-1;
		 	if(i1>i1max) i1max=i1;
		 	if(i2>i2max) i2max=i2;
		 	if(i3>i3max) i3max=i3;
			mesh->faces1.push_back(i1);	// vert/txc/norm
			mesh->faces2.push_back(i2);	
			mesh->faces3.push_back(i3);	
		}

	}
	ni+=mesh->faces1.size()/3;
	nv+=mesh->vert.size();
				
	mesh->n_faces  =mesh->faces1.size();
	mesh->n_vert   =mesh->vert.size();
	mesh->n_txcoord=mesh->txcoord.size();
	mesh->n_norm=mesh->norm.size();

	if(strstr(mesh->name.c_str(),"pivot_"))  cc2+=mesh->faces1.size()/3;
	else cc1+=mesh->faces1.size()/3;

	printf("faces: %d ",mesh->n_faces/3);
	printf("verts: %d/%d ",mesh->n_vert,i1max);
	printf("txc:   %d/%d ",mesh->n_txcoord,i2max);
	printf("norms: %d/%d ",mesh->n_norm,i3max);
	printf("\n");
}



q_mesh2 *objfile::getmesh(const char *name)
{
	for(auto &mesh:meshs)	
		if(mesh.name==name) 
		{
			return &mesh;
		}
		
	printf("ERROR getmesh %s \n",name);
	return NULL;
}
void objfile::setmat(const char *name,axis4 ax)
{
	for(auto &mesh:meshs)	
		if(mesh.name==name) 
		{
			mesh.setmat(ax);
			return;
		}
		printf("ERROR setmat %s \n",name);

}
void objfile::getpivot(const char *name,vec3 *ax,vec3 *origin)
{
	for(auto &mesh:meshs)	
		if(mesh.name==name) 
		{
//			printf("obj: %s %s \n",mesh.name.c_str(),name);		 OK
			mesh.getpivot(ax,origin);
			return;
		}
		printf("ERROR getpivot %s \n",name);
}
void objfile::setrotate(const char *name,float ang1,axis4 *root,vec3 *tra)
{
	q_mesh2 *mesh2=getmesh(name);
	
	axis4 ax5=getrotation2axis4(mesh2->pivot_pos,mesh2->pivot_ax,ang1);
	if(tra)	
	{
		axis4 axtra;
		axtra.w.x=tra->x;
		axtra.w.y=tra->y;
		axtra.w.z=tra->z;
		ax5=axis4_mul(ax5,axtra);
	}
	ax5=axis4_mul(ax5,*root);
	mesh2->mat=ax5;
	mesh2->hasmatrix=1;
}
void objfile::transf(vec3 axis)
{
	for(auto &mesh:meshs)	
		if(mesh.visible&2) mesh.transf(axis);
}
void objfile::load_obj(const char *name,float1 scale2,int type,int file_len)
{
	init_str();
	gscale=scale2;


//	int file_len=filesize(name);
	if(file_len==-1) {error(name);return;}

	str_buf=new char[file_len];//max_asefile_len
	str_p=0;
	if(str_buf==NULL) error("buffer nincs");
	str_h=load(name,str_buf,file_len);
printf("OBJ:  %s %d %d \n",name,file_len,str_h);

	OBJparser();// !!!!!!!!!!!!!!!!!!

 	
	delete [] str_buf;

	printf("verts: %d    faces: %d \n",nv,ni);
	printf("used: %d     pivot: %d \n",cc1,cc2);
	printf("mmin %f %f %f \n",bbox.bbmin.x,bbox.bbmin.y,bbox.bbmin.z);
	printf("mmax %f %f %f \n",bbox.bbmax.x,bbox.bbmax.y,bbox.bbmax.z);
	vec3 siz=bbox.bbmax-bbox.bbmin;
	printf("size %f %f %f \n",siz.x,siz.y,siz.z);

	for(auto &mesh:meshs)	
	{
		if(strstr(mesh.name.c_str(),"pivot_") ) 
		{
			mesh.setpivot();
			mesh.visible=0;
		}
		if(strstr(mesh.name.c_str(),"axis_") )  // 4 wheel distance
		{
			mesh.visible=2;// transf only
		}
	}
	for(auto &mesh:meshs)	
		if(strstr(mesh.name.c_str(),"axis_") ==0) 
		if(strstr(mesh.name.c_str(),"pivot_") ==0) 
		{
			std::string name="pivot_"+mesh.name;
			q_mesh2 *mesh2=getmesh(name.c_str());

			if(mesh2)
			{			
				vec3 pivot_pnt,pivot_axis(1,0,0);
				mesh2->getpivot(&pivot_axis,&pivot_pnt);
	
				mesh.pivot_pos=pivot_pnt;
				mesh.pivot_ax=pivot_axis;
				mesh.visible=3;
				mesh.haspivot=1;
			}
		}

	for(auto &mesh:meshs)	 mesh.makelink();

}




