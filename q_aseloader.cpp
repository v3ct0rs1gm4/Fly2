/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//---------------------------------------------------------------------------

#include "q_engine.h"


//xboxon ez sok!//#define max_str_buf 45000000


// jpg betoltes,  bump map ref map, atlasmap , refmap txc


 
 float gscale=1.0;
 int txcoord_channel2=1;
 
void conv2axis(vec3 *v1,vec3 *v2,vec3 *ax)
{
	vec3 w1=v2[0];

	v1[0]=ax[0]*w1.x +ax[1]*w1.y +ax[2]*w1.z+ax[3];
}
void conv2axis2(vec3 *v1,vec3 *v2,vec3 *ax)
{
	vec3 w1=v2[0];

	v1[0]=ax[0]*w1.x +ax[1]*w1.y +ax[2]*w1.z;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------//--------------------------------------------------------------------------- 
//#define USE_JPG



#ifdef USE_JPG
#include <jpeglib.h>
#include <setjmp.h>


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
   	printf( "jpg ERROR x \n");
  longjmp(myerr->setjmp_buffer, 1);
}
#endif


class qtexture
{
public:
	int xh=0,yh=0,ch=0,id=0;
	int atlas_x1,atlas_y1;
	uchar *mem=0;
	std::string name;
	
void load_jpg(const char *path,const char *file)
{
#ifdef USE_JPG

  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

	char filename[256];
	strcpy(filename,path);
	strcat(filename,file);


  if ((infile = fopen(filename, "rb")) == NULL) {
    	printf( "can't open %s\n", filename);
    return ;
  }

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
   	printf( "jpg ERROR %s\n", filename);
    return ;
  }

  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);

  (void) jpeg_read_header(&cinfo, TRUE);

  (void) jpeg_start_decompress(&cinfo);
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	xh=cinfo.output_width; 
	yh=cinfo.output_height;
	ch=cinfo.output_components;
	mem= new uchar[xh*yh*ch];
	printf("JPEG %s %d %d %d \n",filename,xh,yh,ch);
	
	int y=0;
  while (cinfo.output_scanline < cinfo.output_height) {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
  //  put_scanline_someplace(buffer[0], row_stride);
  	memcpy(&mem[y*xh*ch],buffer[0],xh*ch);
  	y++;
  }
  (void) jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

	if(ch==1)
	{
		uchar *mem2= new uchar[xh*yh*3];
		
		loopd(y,0,yh,1)
		loopd(x,0,xh,1) 
		{
			int m=y*xh + x;
			int r=mem[m];
			m*=3;
			mem2[m+0]=r;
			mem2[m+1]=r;
			mem2[m+2]=r;
		}
		delete [] mem;
		mem=mem2;
		mem2=NULL;
		ch=3;
	}
	
	if(xh>512) // nem kell nagy tx
	{
		int xh2=xh;
		int yh2=yh;
		xh/=2;// half        1300 bol 650 lesz de jo igy
		yh/=2;
		uchar *mem2= new uchar[xh*yh*ch];
		
		loopd(y,0,yh,1)
		loopd(x,0,xh,1) 
		{
			int r=0,g=0,b=0;
			loopd(y2,0,2,1)
			loopd(x2,0,2,1) 
			{
				int m3=(x*2+x2 + xh2*(y2+y*2))*ch;
				r+=mem[m3  ];
				g+=mem[m3+1];
				b+=mem[m3+2];
			}
			r/=4;
			g/=4;
			b/=4;
			int m1=(y*xh + x)*ch;
			mem2[m1+0]=r;
			mem2[m1+1]=g;
			mem2[m1+2]=b;
		}
		delete [] mem;
		mem=mem2;
		mem2=NULL;
	}
  
//  	id=glxtexture(mem,xh,yh,ch);//3  atlas kell
#endif
}
};


int atlas_xh=4096,atlas_yh=4096;
uchar *mem_atlas=0;

struct s_area
{
	int x1,y1,dx,dy;
	
	s_area() {x1=0;y1=0;dx=0;dy=0;}
	s_area(int _x1,int _y1,int _dx,int _dy) {x1=_x1;y1=_y1;dx=_dx;dy=_dy;}
};
qvector<s_area> atlas_areas;



void add2atlas(qtexture *tx)
{
	if(mem_atlas==0) 
	{
		mem_atlas=new uchar[atlas_xh*atlas_yh*3];

		atlas_areas.clear();
		atlas_areas.push_back(s_area(0,0,atlas_xh-1,atlas_yh-1));// free space
	}
	int ok=0;
	loopd(i,0,atlas_areas.size(),1)
	{
		if(tx->xh<=atlas_areas[i].dx)
		if(tx->yh<=atlas_areas[i].dy)// belefer
		{
			int x1=atlas_areas[i].x1;
			int y1=atlas_areas[i].y1;
			int dx=atlas_areas[i].dx;
			int dy=atlas_areas[i].dy;

			int xh=tx->xh;
			int yh=tx->yh;
			int ch=tx->ch;
			tx->atlas_x1=x1;
			tx->atlas_y1=y1;

			for(int y=0;y<yh;y++)
			for(int x=0;x<xh;x++)
			{
				int x2=x1+x;// atlas map pos
				int y2=y1+y;
				int m1=(y2*atlas_xh+x2)*3;
				int m2=(y*xh+x)*ch;
				
				mem_atlas[m1  ]=tx->mem[m2];
				mem_atlas[m1+1]=tx->mem[m2+1];
				mem_atlas[m1+2]=tx->mem[m2+2];
			}

			atlas_areas[i]=atlas_areas.back();// erase
			atlas_areas.pop_back();
			
			s_area tmp1(x1+xh,y1,dx-xh,yh);// mellette
			s_area tmp2(x1,y1+yh,dx,dy-yh);// alatta es mellette
			atlas_areas.push_back(tmp1);
			atlas_areas.push_back(tmp2);
			ok=1;
			break;
		}
	}
	if(ok==0) printf("atlasmap ERROR %d %d \n",tx->xh,tx->yh);
}


qvector<qtexture> texturesASE;

int gettxid(std::string name)
{
	for(int i=0;i<texturesASE.size();i++) 
	{
		if(texturesASE[i].name==name) return i;// already exists
	}
	
	int txid=texturesASE.size();
	texturesASE.resize(texturesASE.size()+1);
	texturesASE[txid].load_jpg("f18/",name.c_str());//"CHROMIC.JPG"
	texturesASE[txid].name=name;
	add2atlas(&texturesASE[txid]);
	
	return txid;
};

//---------------------------------------------------------------------------//--------------------------------------------------------------------------- 
//iiddee


class q_mesh;

class s_submaterial
{
public:
	vec3 diff;
	int txid[4]={-1,-1,-1,-1};
	std::string txname[4];//_dif _ref _bump x
};
class s_material
{
public:
	s_submaterial submat[8];
};


class asefile
{
public:
	void load_ase(const char *name,float1 scale2,int type,int file_len);
	void ASEparser();
	void draw(vec3 pos);
	s_bbox2 bbox;
	
	int atlasid=0;
	int n_mater=0,nv=0,ni=0;
	qvector<s_material> materials;
	qvector<q_mesh> meshs;
};


class q_mesh
{
public:
	void draw(vec3 pos);

	asefile *ase=0;
	std::string name;
	
	vec3 base;
	vec3 pos;
	vec3 rot;
	float1 rot_ang;
	vec3 axis_base[4];
	
	qvector<vec3> vert;
	qvector<int> faces;
	qvector<int> facesubmatID;
	int n_faces=0;
	int n_vert=0;

	qvector<int> edge;
//	int *smoot_group=0;

	qvector<vec3> norm1;
	qvector<vec3> norm2;
	qvector<vec3> norm3;
	qvector<vec3> normface;


	int n_mater=0;
	int n_txcoord=0;
	int n_txcoordfaces=0;
	
	qvector<vec3> txcoord;
	qvector<int>  txcoordfaces;

	int materef=0;
};

void q_mesh::draw(vec3 pos) // render
{
	vec4 col44(1.0,1.0,0.0,1.0);

	int seltx=-1;
	loopd(i,0,8,1)
	loopd(j,0,4,1)
	{
		int id=ase->materials[materef].submat[i].txid[j];
		
		if(id!=-1) {seltx=id;break;}
	}
//	if(seltx!=-1) qsettxbyid(0,seltx);

	qsettxbyid(0,ase->atlasid);
	
	
	glBegin2(GL_TRIANGLES);
	
	for(int i=0;i<n_faces;i++)
	{
		int i3=i*3;
		
		int matID=facesubmatID[i];
		int txid0=ase->materials[materef].submat[matID].txid[0];// diff
		int txid1=ase->materials[materef].submat[matID].txid[1];// ref
		int txid2=ase->materials[materef].submat[matID].txid[2];// bump

		vec4 atlasmap0(0.0,0.0,1.0,1.0);
		vec4 atlasmap1(0.0,0.0,1.0,1.0);
		vec4 atlasmap2(0.0,0.0,1.0,1.0);
//		if(txid1==-1) txid=seltx;// valami?

		if(txid0!=-1)
		{
			atlasmap0.x=(float)texturesASE[txid0].atlas_x1/(float)atlas_xh;//diff
			atlasmap0.y=(float)texturesASE[txid0].atlas_y1/(float)atlas_yh;
			atlasmap0.z=(float)texturesASE[txid0].xh/(float)atlas_xh;
			atlasmap0.w=(float)texturesASE[txid0].yh/(float)atlas_yh;
		}
		if(txid1!=-1)
		{
			atlasmap1.x=(float)texturesASE[txid1].atlas_x1/(float)atlas_xh;//diff
			atlasmap1.y=(float)texturesASE[txid1].atlas_y1/(float)atlas_yh;
			atlasmap1.z=(float)texturesASE[txid1].xh/(float)atlas_xh;
			atlasmap1.w=(float)texturesASE[txid1].yh/(float)atlas_yh;
		}
		if(txid2!=-1)
		{
			atlasmap2.x=(float)texturesASE[txid2].atlas_x1/(float)atlas_xh;//bump
			atlasmap2.y=(float)texturesASE[txid2].atlas_y1/(float)atlas_yh;
			atlasmap2.z=(float)texturesASE[txid2].xh/(float)atlas_xh;
			atlasmap2.w=(float)texturesASE[txid2].yh/(float)atlas_yh;
		}


		int f1=faces[i3];
		int f2=faces[i3+1];
		int f3=faces[i3+2];
		
		vec3 v1=vert[f1];
		vec3 v2=vert[f2];
		vec3 v3=vert[f3];

		vec3 txc1,txc2,txc3;
		if((i3+2)<txcoordfaces.size())
		{
			int tf1=txcoordfaces[i3];
			int tf2=txcoordfaces[i3+1];
			int tf3=txcoordfaces[i3+2];
			txc1=txcoord[tf1];
			txc2=txcoord[tf2];
			txc3=txcoord[tf3];
		}

		vec3 n1=norm1[i];
		vec3 n2=norm2[i];
		vec3 n3=norm3[i];
		
//?		if(edge[i3  ]) n1=-n1;		if(edge[i3+1]) n2=-n2;		if(edge[i3+2]) n3=-n3;
		
		v1+=pos;
		v2+=pos;
		v3+=pos;
		
		glMultiTexCoord4f(1,col44.x,col44.y,col44.z,col44.w);			
		glMultiTexCoord4f(3,atlasmap0.x,atlasmap0.y,atlasmap0.z,atlasmap0.w);			
		glMultiTexCoord4f(4,atlasmap1.x,atlasmap1.y,atlasmap1.z,atlasmap1.w);			
		glMultiTexCoord4f(5,atlasmap2.x,atlasmap2.y,atlasmap2.z,atlasmap2.w);			

		glMultiTexCoord2f(0,txc1.x,1.0-txc1.y);		
		glMultiTexCoord3f(2,n1.x,n1.y,n1.z);		
		glVertex3f(v1.x,v1.y,v1.z);///zY!

		glMultiTexCoord2f(0,txc2.x,1.0-txc2.y);		
		glMultiTexCoord3f(2,n2.x,n2.y,n2.z);		
		glVertex3f(v2.x,v2.y,v2.z);

		glMultiTexCoord2f(0,txc3.x,1.0-txc3.y);		
		glMultiTexCoord3f(2,n3.x,n3.y,n3.z);		
		glVertex3f(v3.x,v3.y,v3.z);

	}
	glEnd2();
}


void asefile::draw(vec3 pos)
{
	for(auto &mesh:meshs)	mesh.draw(pos);
}
//iiddee

void asefile::ASEparser()
{
	int selmat=-1,seltx=0,selsubmat=0,selface=0,selnface=0;
	q_mesh *mesh=0;
	
	while(search_star())
	{
		read_command();
		
		//SUBMATERIAL == tobb texture

//-----------------------------------		
//-----------------------------------
//-----------------------------------
//-----------------------------------
//-----------------------------------

	// ami sokszor jon, elorebb
	
//	"*MESH_VERTEX_LIST")
	if(strcmp(word2,"*MESH_VERTEX")==0)
	{
		int u=read_int();
		vec3 w1;

		w1.x=read_dd()*gscale;
		w1.y=read_dd()*gscale;
		w1.z=read_dd()*gscale;

//pivot!		conv2axis(&w1,&w1,mesh->axis_base);
		mesh->vert[u]=w1;
		
		bbox.add_vec3(mesh->vert[u]);
	}
//printf("ASE  vert[0]  %f %f %f \n",vert[0].x,vert[0].y,vert[0].z);

//	"*MESH_FACE_LIST")
	if(strcmp(word2,"*MESH_FACE")==0)
	{
		int u=read_int();
		selface=u;
		u*=3;

		mesh->faces[u  ]=read_int();
		mesh->faces[u+1]=read_int();
		mesh->faces[u+2]=read_int();
		mesh->edge[u  ]=read_int();
		mesh->edge[u+1]=read_int();
		mesh->edge[u+2]=read_int();
	}
	else
/*	if(strcmp(word2,"*MESH_SMOOTHING")==0)
	{
		mesh->smoot_group[i]=read_int();
	}*/
	if(strcmp(word2,"*MESH_MTLID")==0)
	{
		mesh->facesubmatID[selface]=read_int();
	}
	else

//-----------------------------------


	if(strcmp(word2,"*MESH_NUMTVERTEX")==0)
	{
		mesh->n_txcoord=read_int();
		if(mesh->n_txcoord>0)
		{
			mesh->txcoord.resize(mesh->n_txcoord);
		}
	}
	else
			//("MESH_TVERTLIST");
	if(strcmp(word2,"*MESH_TVERT")==0)
	{
		int u=read_int();

		mesh->txcoord[u].x=read_dd(); // 3d!
		mesh->txcoord[u].y=read_dd();
		mesh->txcoord[u].z=read_dd();
	}
	else
	if(strcmp(word2,"*MESH_NUMTVFACES")==0)
	{
		mesh->n_txcoordfaces=read_int();
		if(mesh->n_txcoordfaces>0)
		{
			mesh->txcoordfaces.resize(mesh->n_txcoordfaces*3);
		}
	}
	else
	if(strcmp(word2,"*MESH_TFACE")==0)
	{
		int u=read_int()*3;

		mesh->txcoordfaces[u  ]=read_int();
		mesh->txcoordfaces[u+1]=read_int();
		mesh->txcoordfaces[u+2]=read_int();
	}
	else

//-----------------------------------

// *MESH_NORMALS
	if(strcmp(word2,"*MESH_FACENORMAL")==0)
	{
		vec3 w1;

		int u=read_int();
		selnface=u;
		w1.x=read_dd();
		w1.y=read_dd();
		w1.z=read_dd();

		mesh->normface[u]=w1;
	}
	else
	if(strcmp(word2,"*MESH_VERTEXNORMAL")==0)
	{
		vec3 w1;

		int u=read_int(); // vert indx!
		w1.x=read_dd();
		w1.y=read_dd();
		w1.z=read_dd();

		int u3=selnface*3;//MESH_FACENORMAL tol
		if(u==mesh->faces[u3  ]) mesh->norm1[selnface]=w1; // double checked OK! 
		if(u==mesh->faces[u3+1]) mesh->norm2[selnface]=w1; // mind mas lesz!
		if(u==mesh->faces[u3+2]) mesh->norm3[selnface]=w1;
	}
	else

//-----------------------------------
	if(strcmp(word2,"*GEOMOBJECT")==0)
	{
		int selmesh=meshs.size();
		meshs.resize(meshs.size()+1);
		mesh=&meshs[selmesh];
		
		mesh->ase=this;
	}
	else

	if(strcmp(word2,"*NODE_NAME")==0)
	{
		mesh->name=read_str();
		printf("ASE: geom  %s ",mesh->name.c_str());
	}
	else

	if(strcmp(word2,"*TM_ROW0")==0)
	{
		mesh->axis_base[0].x=read_dd();
		mesh->axis_base[0].y=read_dd();
		mesh->axis_base[0].z=read_dd();
//		printf("axis[0] %f %f %f \n",mesh->axis_base[0].x,mesh->axis_base[0].y,mesh->axis_base[0].z);
	}
	else
	if(strcmp(word2,"*TM_ROW1")==0)
	{
		mesh->axis_base[1].x=read_dd();
		mesh->axis_base[1].y=read_dd();
		mesh->axis_base[1].z=read_dd();
//		printf("axis[1] %f %f %f \n",mesh->axis_base[1].x,mesh->axis_base[1].y,mesh->axis_base[1].z);
	}
	else
	if(strcmp(word2,"*TM_ROW2")==0)
	{
		mesh->axis_base[2].x=read_dd();
		mesh->axis_base[2].y=read_dd();
		mesh->axis_base[2].z=read_dd();
//		printf("axis[2] %f %f %f \n",mesh->axis_base[2].x,mesh->axis_base[2].y,mesh->axis_base[2].z);
	}
	else
	if(strcmp(word2,"*TM_ROW3")==0)
	{
		mesh->axis_base[3].x=read_dd()*gscale;
		mesh->axis_base[3].y=read_dd()*gscale;
		mesh->axis_base[3].z=read_dd()*gscale;
//		printf("axis[3] %f %f %f \n",mesh->axis_base[3].x,mesh->axis_base[3].y,mesh->axis_base[3].z);
	}
	else
	if(strcmp(word2,"*TM_POS")==0)
	{
		mesh->base.x=read_dd()*gscale;
		mesh->base.y=read_dd()*gscale;
		mesh->base.z=read_dd()*gscale;
	}
	else
	if(strcmp(word2,"*TM_ROTAXIS")==0)
	{
		mesh->rot.x=read_dd();
		mesh->rot.y=read_dd();
		mesh->rot.z=read_dd();
	}
	else
	if(strcmp(word2,"*TM_ROTANGLE")==0)
	{
		mesh->rot_ang=read_dd();
	}
	else
//-----------------------------------
	

	if(strcmp(word2,"*MESH_NUMVERTEX")==0)
	{
		mesh->n_vert=read_int();
		mesh->vert.resize(mesh->n_vert);
		
		printf("ASE: vert  %d ",mesh->n_vert);	
		nv += mesh->n_vert;
	}
	else
	if(strcmp(word2,"*MESH_NUMFACES")==0)
	{
		mesh->n_faces=read_int();
		mesh->faces.resize(mesh->n_faces*3);
		mesh->edge.resize(mesh->n_faces*3);
		mesh->facesubmatID.resize(mesh->n_faces);

		mesh->norm1.resize(mesh->n_faces);// 3 norms of 3 vertexs of face
		mesh->norm2.resize(mesh->n_faces);
		mesh->norm3.resize(mesh->n_faces);
		mesh->normface.resize(mesh->n_faces);
		
		printf("ASE: faces  %d \n",mesh->n_faces);	
		ni += mesh->n_faces;
	}
	else
	if(strcmp(word2,"*MATERIAL_REF")==0)
	{
		mesh->materef=read_int();
	}
	else
	
//-----------------------------------
//-----------------------------------
	
//	search_str("*MATERIAL_LIST");
	if(strcmp(word2,"*MATERIAL_COUNT")==0)
	{
		n_mater=read_int();
		materials.resize(n_mater);
	}
	else
	if(strcmp(word2,"*MATERIAL")==0)
	{
		if(selmat!=-1)
		if(materials[selmat].submat[0].txname[0].size()==0)// nincs rajta semmi
		{
			std::string name2; 
			name2="LIHGREY.JPG";
			materials[selmat].submat[0].txname[0]=name2;
			materials[selmat].submat[0].txid[0]=gettxid(name2);
		
			name2="CHROMIC.JPG";
			materials[selmat].submat[0].txname[1]=name2;
			materials[selmat].submat[0].txid[1]=gettxid(name2);// overwrite all
		}
		selmat=read_int();
		selsubmat=0;
		seltx=0;
//printf("ASE: mater  %d / %d \n",selmat,n_mater);
	}
	else
// 1st mat=0 -> first submaterial[0] [txname_[dif,ref,bump]]
	if(strcmp(word2,"*SUBMATERIAL")==0)
	{
		selsubmat=read_int();
	}
	else
//-----------------------------------

	if(strcmp(word2,"*MATERIAL_DIFFUSE")==0)
	{
		materials[selmat].submat[selsubmat].diff.x=read_dd();
		materials[selmat].submat[selsubmat].diff.y=read_dd();
		materials[selmat].submat[selsubmat].diff.z=read_dd();
	}
	else
	if(strcmp(word2,"*MAP_DIFFUSE")==0)
	{
		seltx=0;
	}
	else
	if(strcmp(word2,"*MAP_REFLECT")==0)
	{
		seltx=1;
	}
	else
	if(strcmp(word2,"*MAP_BUMP")==0)
	{
		seltx=2;
	}
	else
	if(strcmp(word2,"*BITMAP")==0)
	{
		printf("TX TYPE %d \n",seltx);
		std::string name=read_ss();
		materials[selmat].submat[selsubmat].txname[seltx]=name;
		materials[selmat].submat[selsubmat].txid[seltx]=gettxid(name);
		
	//	printf("?? %s \n",name.c_str());
		std::string name2; // nincs rajta az f18on a diffuse tx.... 3dsmax xD
		name2="LIHGREY.JPG";
		if(name=="TOPBUMP.JPG") name2="TOP.JPG";
		if(name=="TOPBUMP.JPG") name2="TOPWING.JPG";
		if(name=="BOTTBUMP.JPG") name2="BOTTBUMP.JPG";
		if(name=="WINGBBUM.JPG") name2="BOTTWING.JPG";
		if(name=="WINGTBUM.JPG") name2="TOPWING.JPG";
		if(name=="WEAPBUMP.JPG") name2="WEAPSUPP.JPG";
		if(name=="ESTABUMP.JPG") name2="ESTATOP.JPG";
		if(name=="LEFTBUMP.JPG") name2="LEFTTEXT.JPG";
//?		if(name=="RIGHTBUMP.JPG") name2="RIGHTTEXT.JPG";
		if(name=="RUDDBUMP.JPG") name2="RUDDRIGH.JPG";
//		if(name=="MISSBUMP.JPG") name2="LEFTTEXT.JPG";
//		if(name=="MISSBUM2.JPG") name2="LEFTTEXT.JPG";
		if(name2.size()>0)
		{
			materials[selmat].submat[selsubmat].txname[0]=name2;
			materials[selmat].submat[selsubmat].txid[0]=gettxid(name2);
		}
		
		name2="CHROMIC.JPG";
		materials[selmat].submat[selsubmat].txname[1]=name2;
		materials[selmat].submat[selsubmat].txid[1]=gettxid(name2);// overwrite all
	}
	else



	{
	}
//-----------------------------------
//-----------------------------------
//-----------------------------------
//-----------------------------------


	}
}

void asefile::load_ase(const char *name,float1 scale2,int type,int file_len)
{
	init_str();
	gscale=scale2;


//	int file_len=filesize(name);
	if(file_len==-1) {error(name);return;}

	str_buf=new char[file_len];//max_asefile_len
	str_p=0;
	if(str_buf==NULL) error("buffer nincs");
	str_h=load(name,str_buf,file_len);
printf("ASE:  %s %d %d \n",name,file_len,str_h);

	ASEparser();// !!!!!!!!!!!!!!!!!!

  	atlasid=glxtexture(mem_atlas,atlas_xh,atlas_yh,3);
  //	savetga("/home/user4/test.tga",mem_atlas,atlas_xh,atlas_yh,3);
  	
	delete [] str_buf;

	printf("verts: %d    faces: %d \n",nv,ni);
	printf("mmin %f %f %f \n",bbox.bbmin.x,bbox.bbmin.y,bbox.bbmin.z);
	printf("mmax %f %f %f \n",bbox.bbmax.x,bbox.bbmax.y,bbox.bbmax.z);

}


