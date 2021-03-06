/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __STORYMAPOPTION__H__
#define __STORYMAPOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <corelib/corelibdef.h>
#include <prolib/core_type.h>
#include <corelib/mconf/IConfigContentSource.h>

#include "reslib/world/res_struct_def.h"
#include "reslib/world/InstMapOption.h"

#include <string>

class CSSOption;

/**
* @class StoryMapOption
* 
* @brief 地图配置信息
**/
class StoryMapOption
{
public:
	StoryMapOption( const char* path, int mapid, int offsetx =0, int offsety =0, bool binst =false);

	/**
	* 加载地图资源
	* @return
	**/
	bool load_mapres();

	/**
	* 获取地图资源配置路径，路径下包含和该地图相关的所有资源
	* @return
	**/
	NS_STL::string get_mapdefpath(){ return res_path_;}

	int get_mapid(){ return mapid_;}

	void world_to_local( float& x, float& y){
		x =x - offsetx_;
		y =y - offsety_;
	}

	void local_to_world( float&x, float& y){
		x =x + offsetx_;
		y =y + offsety_;
	}

	bool worldxy_inthismap( float x, float y);

	NS_STL::string get_respath(){ return res_path_;}

public:
	//地图编号
	int		mapid_;

	//宽度和高度(地图坐标)
	int		leftx_, lefty_;
	int		bottomx_, bottomy_;
	int		width_;
	int		height_;

	NS_STL::string	mapdesc_;

	//相对世界地图的坐标偏移
	int		offsetx_, offsety_;

	//副本地图扩展配置
	//是否副本地图
	bool			is_instancemap_;
	//副本地图相关配置
	InstMapOption	instmap_opt_;

	//所属的css服务器,只对主世界有效，对副本无效
	CSSOption*	owner_css_;

protected:
	//资源位置
	NS_STL::string	res_path_;
};

#endif	//__STORYMAPOPTION__H__
