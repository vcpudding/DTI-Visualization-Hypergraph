#extension GL_ARB_draw_buffers : enable       
#extension GL_ARB_texture_rectangle : enable 
#define MAXDISTANSE 2147483647
#define TEX1DMAXWIDTH 4096
#define SHIFTBIT 12
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_texture_integer : enable
varying out uvec4 fragData0;
varying out uvec4 fragData1;
varying out uvec4 fragData2;
varying out uvec4 fragData3;
uniform vec4 worldCoordRange;
uniform ivec2 textureSize;
uniform usampler2DRect idTex0;
uniform usampler2DRect idTex1;
uniform usampler2DRect idTex2;
uniform usampler2DRect idTex3;
uniform sampler2DRect detailTex;
//uniform ivec4 endIndex;
uniform int stepLength;

void main()
{
	vec2 textureCoords =vec2(gl_FragCoord.x,gl_FragCoord.y);
	int neighborN=0;
	unsigned short inputIds[16];//store the input ids
	unsigned short resultIds[16]= unsigned short[16](0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);//store the result ids
	vec2 result_max_min_dist[16];//store the max and min dist to the result objs
    int realSiteId,savedSiteId;//savedSiteId=realSiteId+1
	int nearestSiteId=0; 
    float dist, minFarDist; 
	float distX,distY,farDist,nearDist;
    minFarDist = MAXDISTANSE; 
	vec2 worldCoord;
	worldCoord.x=textureCoords.x*(worldCoordRange.y-worldCoordRange.x)/textureSize.x+worldCoordRange.x;
	worldCoord.y=textureCoords.y*(worldCoordRange.w-worldCoordRange.z)/textureSize.y+worldCoordRange.z;
    
	vec2 coord;//texture coordinates of the current neighbor being processed
	vec4 pInfor;

	int neighborIndexX,neighborIndexY;//index of the current neighbor being processed
	int rowN;
	short i_obj_index,r_obj_index;
	int error_bound=50;
	for(neighborIndexX=-1;neighborIndexX<2;++neighborIndexX)
	{
		coord.x=textureCoords.x + float(neighborIndexX*stepLength);
		if (coord.x >= 0 &&coord.x < textureSize.x)
		{
			for(neighborIndexY=-1;neighborIndexY<2;++neighborIndexY)
			{
				coord.y=textureCoords.y + float(neighborIndexY*stepLength);
				if (coord.y >= 0 && coord.y < textureSize.y) 
				{
					uvec4 temp_inputIds = uvec4(texture2DRect(idTex0, coord.xy));
					inputIds[0]=temp_inputIds[0];
					inputIds[1]=temp_inputIds[1];
					inputIds[2]=temp_inputIds[2];
					inputIds[3]=temp_inputIds[3];
					for(i_obj_index=1;i_obj_index<=inputIds[0];++i_obj_index)//index of the current obj stored in the neighbor being processed
					{
						if(i_obj_index==4)
						{
							temp_inputIds = uvec4(texture2DRect(idTex1, coord.xy));
							inputIds[4]=temp_inputIds[0];
							inputIds[5]=temp_inputIds[1];
							inputIds[6]=temp_inputIds[2];
							inputIds[7]=temp_inputIds[3];
						}
						else if(i_obj_index==8)
						{
							temp_inputIds = uvec4(texture2DRect(idTex2, coord.xy));
							inputIds[8]=temp_inputIds[0];
							inputIds[9]=temp_inputIds[1];
							inputIds[10]=temp_inputIds[2];
							inputIds[11]=temp_inputIds[3];
						}
						else if(i_obj_index==12)
						{
							temp_inputIds = uvec4(texture2DRect(idTex3, coord.xy));
							inputIds[12]=temp_inputIds[0];
							inputIds[13]=temp_inputIds[1];
							inputIds[14]=temp_inputIds[2];
							inputIds[15]=temp_inputIds[3];
						}
						savedSiteId = inputIds[i_obj_index];//get ID
						realSiteId=savedSiteId-1;
						if(realSiteId<TEX1DMAXWIDTH)//get detail information
							pInfor=	texture2DRect(detailTex, vec2(realSiteId,0));
						else
						{
							rowN=realSiteId>>12;
							pInfor=	texture2DRect(detailTex, vec2(realSiteId-(rowN<<12),rowN));
						}
						//compute distance
						distX=pInfor.x-worldCoord.x;
						distY=pInfor.y-worldCoord.y;
						dist=sqrt(distX*distX+distY*distY);
						farDist=dist+pInfor.z;	
						nearDist=dist-pInfor.z;	//?if inside the circle, I'm not sure whether there will be a problem
						if(minFarDist<nearDist) break;//block all the following input object, no need to continue traversing the input objects
						bool saveI=true;
						for(r_obj_index=1;r_obj_index<=resultIds[0];++r_obj_index)
						{
							if(savedSiteId==resultIds[r_obj_index])//the current input obj is already saved in result before
							{
								saveI=false;
								break;
							}
							if(farDist<result_max_min_dist[r_obj_index].y)//the current input obj blocks all the following result object
							{
								//no need to add 1 to the r_obj_index, since the input will take place of the r_obj_index
								break;
							}
						}
						if(saveI)
						{
							//bool breakout=false;
							resultIds[0]=r_obj_index;//counted the input obj
							//insert I
							for(r_obj_index=resultIds[0]-1;r_obj_index>0;--r_obj_index)
							{
								if(nearDist<result_max_min_dist[r_obj_index].y)
								{
									short r_obj_index_p_1=r_obj_index+1;
									resultIds[r_obj_index_p_1]=resultIds[r_obj_index];
									result_max_min_dist[r_obj_index_p_1]=result_max_min_dist[r_obj_index];
								}else
								{
									//breakout=true;
									break;
								}
							}
							//if(breakout)
								++r_obj_index;
							resultIds[r_obj_index]=savedSiteId;
							result_max_min_dist[r_obj_index].x=farDist;
							result_max_min_dist[r_obj_index].y=nearDist;
							//update minFar
							if(minFarDist>farDist)minFarDist=farDist;
						}//if(saveI)
					}//for(short i_obj_index=1;i_obj_index<=inputIds[0];++i_obj_index)
				}//if (coord.y >= 0 && coord.y < textureSize.y) 
			}//for(neighborIndexY=-1;neighborIndexY<2;++neighborIndexY)
		}//if (coord.x >= 0 &&coord.x < textureSize.x)
	}//for(neighborIndexX=-1;neighborIndexX<2;++neighborIndexX)
	fragData0 = uvec4(resultIds[0],resultIds[1],resultIds[2],resultIds[3]);		
		if(resultIds[0]>3)
		{
			fragData1 = uvec4(resultIds[4],resultIds[5],resultIds[6],resultIds[7]);
			if(resultIds[0]>7)
			{
				fragData2 = uvec4(resultIds[8],resultIds[9],resultIds[10],resultIds[11]);
				if(resultIds[0]>11)
				{
					fragData3 = uvec4(resultIds[12],resultIds[13],resultIds[14],resultIds[15]);
				}
			}
		}
}