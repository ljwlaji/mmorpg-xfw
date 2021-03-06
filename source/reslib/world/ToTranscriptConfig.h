/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __TOTRANSCRIPTCONFIG__H__
#define __TOTRANSCRIPTCONFIG__H__

#include <corelib/corelibdef.h>

#include <map>

#include "reslib/world/res_struct_def.h"

class TiXmlElement;

/**
* @class ToTranscriptConfig
* 
* @brief 副本转跳点管理
* 
**/
class ToTranscriptConfig
{
	typedef NS_STL::map<int, transcript_teleport_info>	TRANSCRIPTTELEPORTS_MAP;
public:
	ToTranscriptConfig();

	/**
	* 初始化转跳点信息
	* @return
	**/
	bool init_teleports( TiXmlElement* root);

	transcript_teleport_info* get_teleportinfobyid( int telid);

private:
	//副本转跳点列表
	TRANSCRIPTTELEPORTS_MAP	all_transcript_teleports_;
};

#endif	//__TOTRANSCRIPTCONFIG__H__
